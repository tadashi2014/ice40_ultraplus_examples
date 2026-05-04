/*
 * comet2_spi_loader — host-side SPI batch runner for COMET II.
 *
 * Features:
 *   - load an arbitrary .bin image into COMET II RAM
 *   - start the CPU
 *   - respond to SVC 1 IN_REQUEST packets (opcode 0x30) with input chars
 *   - collect bytes returned by SVC 2 (OUT)
 *
 * The OUT-side receive buffer is intentionally capped at 256 bytes.
 * Additional received bytes are drained from SPI and discarded.
 */

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include "spi_lib.h"

#define SPI_INIT              0x01
#define SPI_SEND_FIRMWARE     0x02
#define SPI_START_CPU         0x03
#define COMET_CMD_IN_CHAR     0x10
#define COMET_CMD_IN_REQUEST  0x30  /* firmware→host: "send me a character" */
#define COMET_RSP_OUT_CHAR    0x20
#define DEFAULT_FIRMWARE_PATH "./spi_debug_firmware/comet2_inout_main.bin"
#define MAX_FIRMWARE_BYTES    (0xF000u * 2u)
#define RX_BUFFER_SIZE        256u
#define DEFAULT_IDLE_MS       200u
#define BURST_WORDS           8u
#define INPUT_LINE_MAX       1024u

struct run_options {
   const char *firmware_path;
   const char *input_text;
   unsigned int idle_timeout_ms;
   int use_burst;
   int probe_reload;
   int reload_safe;
   int local_echo;
   int echo_newline;
};

struct drain_report {
   unsigned int reads;
   unsigned int packets;
   unsigned int quiet_reads;
   uint8_t first_status;
   uint8_t last_status;
};

struct rx_capture {
   uint8_t data[RX_BUFFER_SIZE];
   size_t used;
   size_t dropped;
};

/*
 * input_source: tracks which character to supply to the CPU on each
 * IN_REQUEST (opcode 0x30) from the firmware.
 *
 * If text != NULL: consume text[pos] one character at a time, then supply
 *                  '\n' indefinitely (terminates the last scanf field).
 * If text == NULL: read stdin one character at a time via fgetc(); supply
 *                  '\n' after EOF.
 */
struct input_source {
   const char *text;
   size_t pos;
   int eof;
   int eof_sent;
   int local_echo;
   int echo_newline;
   uint8_t line[INPUT_LINE_MAX];
   size_t line_len;
   size_t line_pos;
};

static struct termios g_saved_term;
static int            g_raw_mode = 0;

static void restore_terminal(void)
{
   if (g_raw_mode) {
      tcsetattr(STDIN_FILENO, TCSANOW, &g_saved_term);
      g_raw_mode = 0;
   }
}

static void sig_cleanup(int sig)
{
   (void)sig;
   restore_terminal();
   _exit(1);
}

static void setup_raw_mode(void)
{
   struct termios raw;

   if (!isatty(STDIN_FILENO))
      return;

   if (tcgetattr(STDIN_FILENO, &g_saved_term) != 0)
      return;

   raw = g_saved_term;
   cfmakeraw(&raw);
   raw.c_oflag |= OPOST;

   if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0)
      return;

   g_raw_mode = 1;
   atexit(restore_terminal);
   signal(SIGINT,  sig_cleanup);
   signal(SIGTERM, sig_cleanup);
}

static void print_usage(const char *prog)
{
   fprintf(stderr,
           "Usage: %s [--burst] [--probe-reload] [--reload-safe] [--local-echo|--no-local-echo] [--echo-newline|--no-echo-newline] [-f firmware.bin] [-i input_text] [-t idle_ms]\n"
           "  -f firmware.bin : COMET II main memory image to load\n"
           "  -i input_text   : characters to supply to SVC 1 IN_REQUEST packets\n"
           "                    if omitted, stdin is read on demand\n"
           "  -t idle_ms      : stop after this many ms without OUT data (default: %u)\n"
           "  --burst         : batch 8 firmware packets per SPI transaction\n"
           "                    (requires the updated spi_slave queue bitstream)\n"
           "  --probe-reload  : print bounded pre-INIT drain observations\n"
           "  --reload-safe   : drain stale SPI output before INIT using the\n"
           "                    conservative reload-safe path\n"
           "  --local-echo    : echo interactive input locally (default: on)\n"
           "  --no-local-echo : disable local echo of interactive input\n"
           "  --echo-newline  : echo Enter locally when local echo is on (default: off)\n"
           "  --no-echo-newline : suppress local echo for Enter\n"
           "Examples:\n"
            "  %s -f prog.bin -i \"12 3.5 Z hello\"\n"
            "  %s --burst -f prog.bin -i \"12 3.5 Z hello\"\n"
           "  %s --reload-safe -f prog.bin\n"
            "  echo \"42\" | %s -f prog.bin\n",
           prog, DEFAULT_IDLE_MS, prog, prog, prog, prog);
}

static uint64_t now_ms(void)
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return ((uint64_t)tv.tv_sec * 1000u) + ((uint64_t)tv.tv_usec / 1000u);
}

static int parse_args(int argc, char **argv, struct run_options *opts)
{
   int ch;
   static const struct option long_options[] = {
      {"burst", no_argument, NULL, 'b'},
      {"probe-reload", no_argument, NULL, 'p'},
      {"reload-safe", no_argument, NULL, 'r'},
      {"local-echo", no_argument, NULL, 1000},
      {"no-local-echo", no_argument, NULL, 1001},
      {"echo-newline", no_argument, NULL, 1002},
      {"no-echo-newline", no_argument, NULL, 1003},
      {0, 0, 0, 0}
   };

   opts->firmware_path = DEFAULT_FIRMWARE_PATH;
   opts->input_text = NULL;
   opts->idle_timeout_ms = DEFAULT_IDLE_MS;
   opts->use_burst = 0;
   opts->probe_reload = 0;
   opts->reload_safe = 0;
   opts->local_echo = 1;
   opts->echo_newline = 0;

   while ((ch = getopt_long(argc, argv, "bprf:i:t:h", long_options, NULL)) != -1) {
      switch (ch) {
      case 'b':
         opts->use_burst = 1;
         break;
      case 'p':
         opts->probe_reload = 1;
         break;
      case 'r':
         opts->reload_safe = 1;
         break;
      case 1000:
         opts->local_echo = 1;
         break;
      case 1001:
         opts->local_echo = 0;
         break;
      case 1002:
         opts->echo_newline = 1;
         break;
      case 1003:
         opts->echo_newline = 0;
         break;
      case 'f':
         opts->firmware_path = optarg;
         break;
      case 'i':
         opts->input_text = optarg;
         break;
      case 't': {
         char *end = NULL;
         unsigned long value = strtoul(optarg, &end, 0);
         if (optarg[0] == '\0' || end == NULL || *end != '\0' || value == 0 || value > 600000ul) {
            fprintf(stderr, "invalid idle timeout: %s\n", optarg);
            return -1;
         }
         opts->idle_timeout_ms = (unsigned int)value;
         break;
      }
      case 'h':
         print_usage(argv[0]);
         return 1;
      default:
         print_usage(argv[0]);
         return -1;
      }
   }

   if (optind < argc) {
      fprintf(stderr, "unexpected argument: %s\n", argv[optind]);
      print_usage(argv[0]);
      return -1;
   }

   return 0;
}

static uint8_t *read_firmware_file(const char *path, size_t *firmware_size)
{
   FILE *file = NULL;
   uint8_t *firmware = NULL;
   long file_size = 0;
   size_t read_size = 0;

   file = fopen(path, "rb");
   if (file == NULL) {
      fprintf(stderr, "failed to open firmware image '%s': %s\n", path, strerror(errno));
      return NULL;
   }

   if (fseek(file, 0, SEEK_END) != 0) {
      fprintf(stderr, "failed to seek firmware image '%s'\n", path);
      fclose(file);
      return NULL;
   }
   file_size = ftell(file);
   if (file_size < 0) {
      fprintf(stderr, "failed to tell firmware image size '%s'\n", path);
      fclose(file);
      return NULL;
   }
   if (fseek(file, 0, SEEK_SET) != 0) {
      fprintf(stderr, "failed to rewind firmware image '%s'\n", path);
      fclose(file);
      return NULL;
   }

   if ((unsigned long)file_size > MAX_FIRMWARE_BYTES) {
      fprintf(stderr, "firmware image too large: %ld bytes (max %u)\n",
              file_size, (unsigned int)MAX_FIRMWARE_BYTES);
      fclose(file);
      return NULL;
   }
   if (((unsigned long)file_size & 1u) != 0u) {
      fprintf(stderr, "firmware size must be even, got %ld bytes\n", file_size);
      fclose(file);
      return NULL;
   }

   firmware = (uint8_t *)malloc((size_t)file_size);
   if (firmware == NULL) {
      fprintf(stderr, "out of memory while reading firmware\n");
      fclose(file);
      return NULL;
   }

   read_size = fread(firmware, sizeof(uint8_t), (size_t)file_size, file);
   fclose(file);
   if (read_size != (size_t)file_size) {
      fprintf(stderr, "short read on firmware image '%s'\n", path);
      free(firmware);
      return NULL;
   }

   *firmware_size = read_size;
   return firmware;
}

static void drain_spi_output_until_quiet(struct drain_report *report,
                                         unsigned int max_reads,
                                         unsigned int quiet_goal)
{
   uint8_t drain_pkt[3] = {0, 0, 0};
   uint8_t drain_st = 0;

   memset(report, 0, sizeof(*report));

   while (report->reads < max_reads && report->quiet_reads < quiet_goal) {
      if (spi_read(drain_pkt, &drain_st)) {
         if (report->packets == 0) {
            report->first_status = drain_st;
         }
         report->last_status = drain_st;
         report->packets++;
         report->quiet_reads = 0;
      } else {
         report->quiet_reads++;
      }
      report->reads++;
   }
}

static void print_drain_report(const char *phase, const struct drain_report *report)
{
   if (report->packets == 0) {
      fprintf(stderr, "%s: drained 0 packets in %u reads\n",
              phase, report->reads);
      return;
   }

   fprintf(stderr,
           "%s: drained %u packet(s) in %u reads, first_status=%02x last_status=%02x\n",
           phase,
           report->packets,
           report->reads,
           report->first_status,
           report->last_status);
}

static int send_firmware_legacy(const uint8_t *firmware, size_t firmware_size, uint8_t *spi_status)
{
   size_t i;

   for (i = 0; i < firmware_size; i += 2) {
      uint8_t packet[3] = {firmware[i], firmware[i + 1], 0x00};
      if (spi_send(SPI_SEND_FIRMWARE, packet, spi_status) != 0) {
         fprintf(stderr, "firmware transfer failed at byte %zu (status=%02x)\n", i, *spi_status);
         return -1;
      }
      usleep(50);
   }

   return 0;
}

static int send_firmware_burst(const uint8_t *firmware, size_t firmware_size, uint8_t *spi_status)
{
   size_t i = 0;

   while (i < firmware_size) {
      unsigned int words = (unsigned int)((firmware_size - i) / 2u);
      unsigned int w;
      uint8_t batch[BURST_WORDS * 4u];

      if (words > BURST_WORDS) {
         words = BURST_WORDS;
      }
      for (w = 0; w < words; w++) {
         batch[w * 4u + 0u] = SPI_SEND_FIRMWARE;
         batch[w * 4u + 1u] = firmware[i];
         batch[w * 4u + 2u] = firmware[i + 1u];
         batch[w * 4u + 3u] = 0x00;
         i += 2;
      }
      if (spi_send_batch_packets(batch, words, spi_status) != 0) {
         fprintf(stderr, "burst firmware transfer failed at byte %zu (status=%02x)\n", i, *spi_status);
         return -1;
      }
   }

   return 0;
}

static int load_and_start_comet_firmware(const char *firmware_path,
                                         int use_burst,
                                         int reload_safe,
                                         int probe_reload)
{
   uint8_t no_param[3] = {0x00, 0x00, 0x00};
   uint8_t spi_status = 0;
   uint8_t *firmware = NULL;
   size_t firmware_size = 0;

   firmware = read_firmware_file(firmware_path, &firmware_size);
   if (firmware == NULL) {
      return -1;
   }

   /* Stable baseline path: keep the original bounded drain before INIT.
    * Reload-safe mode switches to a bounded "drain until quiet" pass
    * without changing spi_send() semantics. */
   if (reload_safe || probe_reload) {
      struct drain_report report;
      drain_spi_output_until_quiet(&report, 64, 4);
      if (probe_reload) {
         print_drain_report("pre-init drain", &report);
      }
   } else {
      int drain_i;
      uint8_t drain_pkt[3] = {0, 0, 0};
      uint8_t drain_st = 0;
      for (drain_i = 0; drain_i < 10; drain_i++) {
         spi_read(drain_pkt, &drain_st);
      }
   }
   usleep(20000);
   if (spi_send(SPI_INIT, no_param, &spi_status) != 0) {
      fprintf(stderr, "failed to send SPI_INIT (status=%02x)\n", spi_status);
      free(firmware);
      return -1;
   }
   if (probe_reload) {
      fprintf(stderr, "probe: SPI_INIT acknowledged with status=%02x\n", spi_status);
   }
   fprintf(stderr, "loaded firmware image: %s (%zu bytes)\n", firmware_path, firmware_size);

   if (use_burst) {
      if (send_firmware_burst(firmware, firmware_size, &spi_status) != 0) {
         free(firmware);
         return -1;
      }
   } else {
      if (send_firmware_legacy(firmware, firmware_size, &spi_status) != 0) {
         free(firmware);
         return -1;
      }
   }

   free(firmware);

   usleep(20000);
   if (spi_send(SPI_START_CPU, no_param, &spi_status) != 0) {
      fprintf(stderr, "failed to send SPI_START_CPU (status=%02x)\n", spi_status);
      return -1;
   }
   usleep(20000);
   return 0;
}

static int comet_send_in_char(uint8_t ch)
{
   uint8_t status = 0;
   /*
    * SPI packet layout: spi_send(cmd, {val[0], val[1], val[2]}) sends
    * the 4-byte sequence [cmd, val[0], val[1], val[2]].  The FPGA SPI
    * slave maps:
    *   val[0] → COMET #F001 (opcode register, firmware checks == OPIN = 0x10)
    *   val[1] → COMET #F002 (data register, the character to receive)
    * So val[0] must be COMET_CMD_IN_CHAR (= OPIN = 0x10).
    */
   uint8_t payload[3] = {COMET_CMD_IN_CHAR, ch, 0x00};
   return spi_send(COMET_CMD_IN_CHAR, payload, &status);
}

static int comet_read_packet(uint8_t packet[3], uint8_t *status)
{
   return spi_read(packet, status) ? 0 : -1;
}

static void rx_capture_push(struct rx_capture *capture, uint8_t ch)
{
   if (capture->used < RX_BUFFER_SIZE) {
      capture->data[capture->used++] = ch;
   } else {
      capture->dropped++;
   }
}

/*
 * input_source_next: return the next character to supply to the CPU on an
 * IN_REQUEST.  For -i mode, consume src->text one character at a time and
 * return '\n' after the last one.  For stdin mode, call fgetc() (blocks
 * until a character or EOF is available) and return '\n' on EOF.
 */
static int refill_interactive_line(struct input_source *src)
{
   src->line_len = 0;
   src->line_pos = 0;

   if (src->eof) {
      if (src->eof_sent) {
         return -1;
      }
      src->line[0] = 0x04;
      src->line_len = 1;
      return 0;
   }

   for (;;) {
      int c = fgetc(stdin);
      if (c == EOF) {
         src->eof = 1;
         if (g_raw_mode && src->local_echo && src->echo_newline) {
            putchar('\n');
            fflush(stdout);
         }
         src->line[0] = 0x04;
         src->line_len = 1;
         return 0;
      }

      if (c == '\r')
         c = '\n';

      if (g_raw_mode && (c == 0x08 || c == 0x7f)) {
         if (src->line_len != 0) {
            src->line_len--;
            fputs("\b \b", stdout);
            fflush(stdout);
         }
         continue;
      }

      if (c == 0x04) {
         if (src->line_len == 0) {
            src->line[0] = 0x04;
            src->line_len = 1;
            return 0;
         }
         c = '\n';
      }

      if (g_raw_mode && src->local_echo &&
          (c != '\n' || src->echo_newline)) {
         putchar(c);
         fflush(stdout);
      }

      if (src->line_len < INPUT_LINE_MAX - 1) {
         src->line[src->line_len++] = (uint8_t)c;
      }

      if (c == '\n') {
         return 0;
      }
   }
}

static int input_source_next(struct input_source *src)
{
   if (src->text != NULL) {
      if (src->text[src->pos] != '\0') {
         return (int)(uint8_t)src->text[src->pos++];
      }
      return '\n';   /* terminating newline after end of -i string */
   }

   if (src->line_pos >= src->line_len) {
      if (refill_interactive_line(src) != 0) {
         return -1;
      }
   }
   {
      uint8_t ch = src->line[src->line_pos++];
      if (ch == 0x04) {
         src->eof_sent = 1;
      }
      return (int)ch;
   }
}

/*
 * run_io_loop: unified event loop after firmware start.
 *
 * Collects SVC 2 OUT bytes (opcode 0x20) into capture.
 * Responds to SVC 1 IN_REQUEST packets (opcode 0x30) with the next
 * character from src – no host-side polling or timing guesswork needed.
 * Exits after idle_timeout_ms with no OUT or IN_REQUEST activity.
 */
static int run_io_loop(struct rx_capture *capture, struct input_source *src,
                       unsigned int idle_timeout_ms)
{
   uint64_t deadline = now_ms() + idle_timeout_ms;

   while (now_ms() < deadline) {
      uint8_t packet[3] = {0, 0, 0};
      uint8_t status = 0;

      if (comet_read_packet(packet, &status) != 0) {
         continue;
      }
      deadline = now_ms() + idle_timeout_ms;

      if (packet[0] == COMET_RSP_OUT_CHAR) {
         rx_capture_push(capture, packet[1]);
         continue;
      }

      if (packet[0] == COMET_CMD_IN_REQUEST) {
         int ch = input_source_next(src);
         if (ch < 0) {
            continue;
         }
         if (comet_send_in_char((uint8_t)ch) != 0) {
            fprintf(stderr, "failed to send char 0x%02x\n", (unsigned int)(uint8_t)ch);
            return -1;
         }
         continue;
      }

      fprintf(stderr, "ignored packet: %02x %02x %02x (status=%02x)\n",
              packet[0], packet[1], packet[2], status);
   }

   return 0;
}

static void print_capture_summary(const struct rx_capture *capture)
{
   size_t i;

   printf("received %zu byte(s)", capture->used);
   if (capture->dropped != 0) {
      printf(", dropped %zu byte(s) after %u-byte buffer filled", capture->dropped, RX_BUFFER_SIZE);
   }
   printf("\n");

   if (capture->used == 0) {
      return;
   }

   printf("OUT text:\n");
   for (i = 0; i < capture->used; ++i) {
      uint8_t ch = capture->data[i];
      putchar((ch >= 0x20 && ch <= 0x7e) || ch == '\n' || ch == '\r' || ch == '\t' ? ch : '.');
   }
   if (capture->data[capture->used - 1] != '\n') {
      putchar('\n');
   }

   printf("OUT hex:");
   for (i = 0; i < capture->used; ++i) {
      printf(" %02x", capture->data[i]);
   }
   putchar('\n');
}

int main(int argc, char **argv)
{
   struct run_options opts;
   struct rx_capture capture;
   struct input_source src;
   int parse_result;

   memset(&capture, 0, sizeof(capture));

   parse_result = parse_args(argc, argv, &opts);
   if (parse_result > 0) {
      return 0;
   }
   if (parse_result < 0) {
      return 1;
   }

   if (spi_init() != 0) {
      return 1;
   }

   setup_raw_mode();

   if (load_and_start_comet_firmware(opts.firmware_path,
                                     opts.use_burst,
                                     opts.reload_safe,
                                     opts.probe_reload) != 0) {
      return 1;
   }

   src.text = opts.input_text;
   src.pos = 0;
   src.eof = 0;
   src.eof_sent = 0;
   src.local_echo = opts.local_echo;
   src.echo_newline = opts.echo_newline;
   src.line_len = 0;
   src.line_pos = 0;

   if (opts.input_text != NULL) {
      fprintf(stderr, "sending input: %s\n", opts.input_text);
   }

   if (run_io_loop(&capture, &src, opts.idle_timeout_ms) != 0) {
      restore_terminal();
      return 1;
   }

   restore_terminal();
   print_capture_summary(&capture);
   return 0;
}
