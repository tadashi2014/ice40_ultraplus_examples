/*
 * comet2_spi_run — interactive SPI runner for COMET II.
 *
 * Usage:
 *   comet2_spi_run [-f firmware.bin] [-i input_text] [-t idle_ms] [-e eof_ms]
 *
 * After loading and starting the firmware this program enters a full
 * duplex I/O loop:
 *
 *   • SVC 2 OUT chars (opcode 0x20) are written to stdout immediately.
 *   • SVC 1 IN_REQUEST packets (opcode 0x30, new firmware) trigger an
 *     immediate fgetc() on stdin so the CPU gets the character it asked
 *     for.  This is the demand-driven path that requires runtime.cas to
 *     be assembled with the IN_REQUEST addition to SPINRW.
 *   • For firmware that does NOT send 0x30 (old .bin without the
 *     IN_REQUEST patch), the loop also monitors stdin with select() so
 *     that characters the user types are forwarded proactively.
 *   • The loop exits after idle_ms milliseconds with no CPU activity.
 *
 * The program therefore works correctly with both old and new firmware.
 */

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include "../../spi/spi_host/spi_lib.h"

/* ------------------------------------------------------------------ */
/* SPI opcodes                                                          */
/* ------------------------------------------------------------------ */
#define SPI_INIT              0x01
#define SPI_SEND_FIRMWARE     0x02
#define SPI_START_CPU         0x03
#define COMET_CMD_IN_CHAR     0x10  /* host→CPU: deliver one character   */
#define COMET_CMD_IN_REQUEST  0x30  /* CPU→host: "I need a character"    */
#define COMET_RSP_OUT_CHAR    0x20  /* CPU→host: "print this character"  */

/* ------------------------------------------------------------------ */
/* Defaults                                                            */
/* ------------------------------------------------------------------ */
#define DEFAULT_FIRMWARE_PATH "./spi_debug_firmware/comet2_inout_main.bin"
#define MAX_FIRMWARE_BYTES    (0xF000u * 2u)
#define DEFAULT_IDLE_MS       5000u   /* 5 s while input is still open      */
#define DEFAULT_EOF_DRAIN_MS   200u   /* short post-EOF output drain        */
#define STDIN_POLL_US         1000u   /* 1 ms select() timeout for stdin    */
#define BURST_WORDS           8u

/* ------------------------------------------------------------------ */
/* Types                                                               */
/* ------------------------------------------------------------------ */
struct run_options {
   const char *firmware_path;
   const char *input_text;   /* NULL → read stdin interactively */
   unsigned int idle_timeout_ms;
   unsigned int eof_drain_ms;
   int use_burst;
   int probe_reload;
   int reload_safe;
};

struct drain_report {
   unsigned int reads;
   unsigned int packets;
   unsigned int quiet_reads;
   uint8_t first_status;
   uint8_t last_status;
};

/*
 * input_source: unified character source.
 *
 *   text != NULL  → consume text[pos], then supply '\n' forever.
 *   text == NULL  → read from stdin (may block or use select()).
 */
struct input_source {
   const char *text;
   size_t      pos;
   int         eof;      /* set after fgetc() returns EOF                */
};

/* ------------------------------------------------------------------ */
/* Raw terminal mode                                                   */
/*                                                                     */
/* Enabled only when stdin is a real terminal.  In raw mode every     */
/* keystroke is delivered to the program immediately (no Enter needed),*/
/* Ctrl-D (0x04) is visible as a character (not silently converted to  */
/* EOF by the kernel), and the terminal does not echo typed characters */
/* (we echo them ourselves).                                           */
/* ------------------------------------------------------------------ */
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
   /* Keep output post-processing so '\n' still becomes '\r\n'. */
   raw.c_oflag |= OPOST;

   if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0)
      return;

   g_raw_mode = 1;
   atexit(restore_terminal);
   signal(SIGINT,  sig_cleanup);
   signal(SIGTERM, sig_cleanup);
}

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */
static void print_usage(const char *prog)
{
   fprintf(stderr,
           "Usage: %s [--burst] [--probe-reload] [--reload-safe] [-f firmware.bin] [-i input_text] [-t idle_ms] [-e eof_ms]\n"
           "  -f firmware.bin : COMET II main-memory image to load\n"
           "  -i input_text   : characters to feed on each IN_REQUEST\n"
           "                    if omitted, stdin is read interactively\n"
           "  -t idle_ms      : exit after this many ms of silence\n"
           "                    while input is still open (default: %u ms)\n"
           "  -e eof_ms       : after Ctrl-D / EOF, wait this many ms for\n"
           "                    trailing firmware output before exit\n"
           "                    (default: %u ms)\n"
           "  --burst         : batch 8 firmware packets per SPI transaction\n"
           "                    (requires the updated spi_slave queue bitstream)\n"
           "  --probe-reload  : print bounded pre-INIT drain observations\n"
           "  --reload-safe   : drain stale SPI output before INIT using the\n"
           "                    conservative reload-safe path\n"
           "Examples:\n"
           "  %s -f prog.bin -i \"42\"\n"
           "  %s --burst -f prog.bin -i \"42\"\n"
           "  %s --reload-safe -f prog.bin\n"
           "  echo \"42\" | %s -f prog.bin\n",
           prog, DEFAULT_IDLE_MS, DEFAULT_EOF_DRAIN_MS, prog, prog, prog, prog);
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
      {0, 0, 0, 0}
   };

   opts->firmware_path    = DEFAULT_FIRMWARE_PATH;
   opts->input_text       = NULL;
   opts->idle_timeout_ms  = DEFAULT_IDLE_MS;
   opts->eof_drain_ms     = DEFAULT_EOF_DRAIN_MS;
   opts->use_burst        = 0;
    opts->probe_reload     = 0;
    opts->reload_safe      = 0;

   while ((ch = getopt_long(argc, argv, "bprf:i:t:e:h", long_options, NULL)) != -1) {
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
      case 'f':
         opts->firmware_path = optarg;
         break;
      case 'i':
         opts->input_text = optarg;
         break;
      case 't': {
         char *end = NULL;
         unsigned long value = strtoul(optarg, &end, 0);
         if (optarg[0] == '\0' || end == NULL || *end != '\0' ||
             value == 0 || value > 600000ul) {
            fprintf(stderr, "invalid idle timeout: %s\n", optarg);
            return -1;
         }
         opts->idle_timeout_ms = (unsigned int)value;
         break;
      }
      case 'e': {
         char *end = NULL;
         unsigned long value = strtoul(optarg, &end, 0);
         if (optarg[0] == '\0' || end == NULL || *end != '\0' ||
             value > 600000ul) {
            fprintf(stderr, "invalid EOF drain timeout: %s\n", optarg);
            return -1;
         }
         opts->eof_drain_ms = (unsigned int)value;
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
   FILE   *file = NULL;
   uint8_t *firmware = NULL;
   long    file_size = 0;
   size_t  read_size = 0;

   file = fopen(path, "rb");
   if (file == NULL) {
      fprintf(stderr, "failed to open firmware image '%s': %s\n",
              path, strerror(errno));
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
      fprintf(stderr, "firmware size must be even, got %ld bytes\n",
              file_size);
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
   uint8_t  no_param[3] = {0x00, 0x00, 0x00};
   uint8_t  spi_status = 0;
   uint8_t *firmware = NULL;
   size_t   firmware_size = 0;

   firmware = read_firmware_file(firmware_path, &firmware_size);
   if (firmware == NULL) {
      return -1;
   }

   /* Drain any data the FPGA has queued in its SPI TX register.
    * This unblocks the CPU if it is stuck in the SPOTRL/SPINRQ
    * busy-wait loop (TX buffer full), giving it time to clear
    * status_register[0] before SPI_INIT arrives.  Without this drain,
    * a stale status_register[0]==1 from the previous run can cause
    * spi_mm to silently drop SPI_INIT, preventing firmware reload
    * without reflashing the FPGA bitstream. */
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
   fprintf(stderr, "loaded firmware image: %s (%zu bytes)\n",
           firmware_path, firmware_size);

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
      fprintf(stderr, "failed to send SPI_START_CPU (status=%02x)\n",
              spi_status);
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

/*
 * input_source_next_blocking: for the 0x30 (IN_REQUEST) path.
 * Blocks until a character is available from stdin or -i buffer.
 * After EOF / Ctrl-D / end-of-string, returns '\n' indefinitely.
 * In raw mode each keystroke unblocks fgetc() immediately (no Enter
 * needed), and Ctrl-D (0x04) is treated as end-of-input (sends '\n').
 */
static uint8_t input_source_next_blocking(struct input_source *src)
{
   if (src->text != NULL) {
      if (src->text[src->pos] != '\0') {
         return (uint8_t)src->text[src->pos++];
      }
      return '\n';
   }
   if (!src->eof) {
      int c = fgetc(stdin);
      if (c == EOF || c == 0x04) {   /* EOF or Ctrl-D */
         src->eof = 1;
         if (g_raw_mode) {
            putchar('\n');
            fflush(stdout);
         }
         return '\n';
      }
      if (c == '\r')
         c = '\n';   /* raw mode delivers CR for the Enter key */
      if (g_raw_mode) {
         putchar(c);   /* echo: terminal suppresses it in raw mode */
         fflush(stdout);
      }
      return (uint8_t)c;
   }
   return '\n';
}

/*
 * stdin_poll_char: non-blocking stdin read via select().
 * Returns character if one is ready, -1 if nothing is available.
 * Used for proactive forwarding when firmware does NOT send 0x30.
 *
 * In raw mode the select() timeout is 0 (purely non-blocking) because
 * each keystroke is delivered immediately without waiting for Enter.
 * Ctrl-D (0x04) is treated as end-of-input and causes '\n' to be
 * returned once (and eof to be set).  CR is mapped to LF.
 * Characters are echoed in raw mode because the terminal suppresses echo.
 */
static int stdin_poll_char(struct input_source *src)
{
   struct timeval tv;
   fd_set rfds;
   int sel;

   if (src->text != NULL) {
      /* -i mode: consume text[] without blocking */
      if (src->text[src->pos] != '\0') {
         return (int)(unsigned char)src->text[src->pos++];
      }
      if (!src->eof) {
         src->eof = 1;
         return '\n';
      }
      return -1;   /* exhausted */
   }

   if (src->eof) {
      return -1;
   }

   FD_ZERO(&rfds);
   FD_SET(STDIN_FILENO, &rfds);
   tv.tv_sec  = 0;
   /* Raw mode: non-blocking (0 µs).  Canonical mode: 1 ms. */
   tv.tv_usec = g_raw_mode ? 0L : (long)STDIN_POLL_US;
   sel = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
   if (sel <= 0) {
      return -1;   /* no data ready */
   }

   int c = fgetc(stdin);
   if (c == EOF || c == 0x04) {   /* EOF or Ctrl-D */
      src->eof = 1;
      if (g_raw_mode) {
         putchar('\n');
         fflush(stdout);
      }
      return '\n';   /* send '\n' to firmware as a line terminator */
   }
   if (c == '\r')
      c = '\n';   /* raw mode delivers CR for the Enter key */
   if (g_raw_mode) {
      putchar(c);   /* echo: terminal suppresses it in raw mode */
      fflush(stdout);
   }
   return c;
}

/*
 * run_io_loop: the main I/O event loop after firmware start.
 *
 * Packet handling:
 *   0x20  OUT_CHAR   → write char to stdout immediately.
 *   0x30  IN_REQUEST → demand-driven: call input_source_next_blocking()
 *                      (in raw mode each keystroke unblocks it instantly;
 *                      Ctrl-D is treated as end-of-input).
 *   other            → logged to stderr and ignored.
 *
 * Proactive forwarding:
 *   When no SPI packet is received in a polling iteration, stdin is
 *   checked (non-blocking in raw mode, 1 ms select() in canonical mode).
 *   Characters are forwarded immediately, handling firmware that does
 *   NOT send 0x30.
 *
 * Exit condition:
 *   Interactive tty (raw mode, no -i flag): the idle timer is held back
 *   while the user has not yet pressed Ctrl-D (src->eof is still 0).
 *   Once Ctrl-D is received the loop exits after eof_drain_ms of
 *   silence (giving the firmware a short chance to finish output).
 *   Non-tty / -i mode also switches to eof_drain_ms after EOF.
 */
static unsigned int current_timeout_ms(const struct input_source *src,
                                       unsigned int idle_timeout_ms,
                                       unsigned int eof_drain_ms)
{
   return src->eof ? eof_drain_ms : idle_timeout_ms;
}

static int run_io_loop(struct input_source *src,
                       unsigned int idle_timeout_ms,
                       unsigned int eof_drain_ms)
{
   /*
    * In interactive tty mode keep pushing the deadline while the user
    * has not yet finished input (src->eof == 0).  This removes the
    * "time window" problem: the user can take as long as needed to type.
    * After EOF the timer is allowed to count down so we eventually exit
    * once the firmware goes silent.
    */
   const int interactive = (g_raw_mode && src->text == NULL);
   uint64_t deadline =
      now_ms() + current_timeout_ms(src, idle_timeout_ms, eof_drain_ms);

   fflush(stdout);

   while (1) {
      /* Keep deadline in the future until input is complete. */
      if (interactive && !src->eof) {
         deadline =
            now_ms() + current_timeout_ms(src, idle_timeout_ms, eof_drain_ms);
      }

      if (now_ms() >= deadline) {
         break;
      }

      uint8_t packet[3] = {0, 0, 0};
      uint8_t status    = 0;

      if (comet_read_packet(packet, &status) == 0) {
         /* Got a SPI packet – reset the idle timer. */
         deadline =
            now_ms() + current_timeout_ms(src, idle_timeout_ms, eof_drain_ms);

         if (packet[0] == COMET_RSP_OUT_CHAR) {
            /* Print CPU output immediately; don't buffer. */
            uint8_t ch = packet[1];
            if ((ch >= 0x20 && ch <= 0x7e) ||
                ch == '\n' || ch == '\r' || ch == '\t') {
               putchar((int)ch);
            } else {
               putchar('.');
            }
            fflush(stdout);
            continue;
         }

         if (packet[0] == COMET_CMD_IN_REQUEST) {
            /* Demand-driven path: firmware explicitly asked for a char. */
            uint8_t ch = input_source_next_blocking(src);
            if (comet_send_in_char(ch) != 0) {
               fprintf(stderr,
                       "failed to send char 0x%02x\n",
                       (unsigned int)ch);
               return -1;
            }
            deadline =
               now_ms() + current_timeout_ms(src, idle_timeout_ms, eof_drain_ms);
            continue;
         }

         fprintf(stderr,
                 "ignored packet: %02x %02x %02x (status=%02x)\n",
                 packet[0], packet[1], packet[2], status);
         continue;
      }

      /*
       * No SPI packet this iteration.  Try the proactive stdin path so
       * we can forward input to firmware that does not send 0x30.
       */
      {
         int c = stdin_poll_char(src);
         if (c >= 0) {
            if (comet_send_in_char((uint8_t)c) != 0) {
               fprintf(stderr,
                       "failed to send char 0x%02x\n",
                       (unsigned int)c);
               return -1;
            }
            deadline =
               now_ms() + current_timeout_ms(src, idle_timeout_ms, eof_drain_ms);
         }
      }
   }

   return 0;
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main(int argc, char **argv)
{
   struct run_options opts;
   struct input_source src;
   int parse_result;

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

   /* Switch stdin to raw mode when running interactively.  In raw mode
    * every keystroke is immediately available (no Enter required), and
    * Ctrl-D (0x04) is detected as an explicit end-of-input signal.
    * The terminal is automatically restored on exit or SIGINT/SIGTERM. */
   setup_raw_mode();

   if (load_and_start_comet_firmware(opts.firmware_path,
                                     opts.use_burst,
                                     opts.reload_safe,
                                     opts.probe_reload) != 0) {
      return 1;
   }

   src.text = opts.input_text;
   src.pos  = 0;
   src.eof  = 0;

   if (run_io_loop(&src, opts.idle_timeout_ms, opts.eof_drain_ms) != 0) {
      restore_terminal();
      return 1;
   }

   restore_terminal();
   putchar('\n');
   fflush(stdout);
   return 0;
}
