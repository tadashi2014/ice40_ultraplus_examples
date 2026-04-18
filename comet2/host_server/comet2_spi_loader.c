/*
 * Host-side SPI runner for COMET II spi_debug.
 *
 * Features:
 *   - load an arbitrary .bin image into COMET II RAM
 *   - start the CPU
 *   - optionally feed bytes to SVC 1 (IN)
 *   - collect bytes returned by SVC 2 (OUT)
 *
 * The OUT-side receive buffer is intentionally capped at 256 bytes.
 * Additional received bytes are drained from SPI and discarded.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "../../spi/spi_host/spi_lib.h"

#define SPI_INIT            0x01
#define SPI_SEND_FIRMWARE   0x02
#define SPI_START_CPU       0x03
#define COMET_CMD_IN_CHAR   0x10
#define COMET_RSP_OUT_CHAR  0x20
#define DEFAULT_FIRMWARE_PATH "./spi_debug_firmware/comet2_inout_main.bin"
#define MAX_FIRMWARE_BYTES  (0xF000u * 2u)
#define RX_BUFFER_SIZE      256u
#define DEFAULT_IDLE_MS     200u
#define INTER_CHAR_DRAIN_MS 5u

struct run_options {
   const char *firmware_path;
   const char *input_text;
   unsigned int idle_timeout_ms;
};

struct rx_capture {
   uint8_t data[RX_BUFFER_SIZE];
   size_t used;
   size_t dropped;
};

static void print_usage(const char *prog)
{
   fprintf(stderr,
           "Usage: %s [-f firmware.bin] [-i input_text] [-t idle_ms]\n"
           "  -f firmware.bin : COMET II main memory image to load\n"
           "  -i input_text   : bytes to send through SVC 1 after start\n"
           "  -t idle_ms      : stop after this many ms without OUT data (default: %u)\n",
           prog, DEFAULT_IDLE_MS);
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

   opts->firmware_path = DEFAULT_FIRMWARE_PATH;
   opts->input_text = NULL;
   opts->idle_timeout_ms = DEFAULT_IDLE_MS;

   while ((ch = getopt(argc, argv, "f:i:t:h")) != -1) {
      switch (ch) {
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

static int load_and_start_comet_firmware(const char *firmware_path)
{
   uint8_t no_param[3] = {0x00, 0x00, 0x00};
   uint8_t spi_status = 0;
   uint8_t *firmware = NULL;
   size_t firmware_size = 0;
   size_t i;

   firmware = read_firmware_file(firmware_path, &firmware_size);
   if (firmware == NULL) {
      return -1;
   }

   sleep(1);
   if (spi_send(SPI_INIT, no_param, NULL) != 0) {
      fprintf(stderr, "failed to send SPI_INIT\n");
      free(firmware);
      return -1;
   }
   printf("loaded firmware image: %s (%zu bytes)\n", firmware_path, firmware_size);

   for (i = 0; i < firmware_size; i += 2) {
      uint8_t packet[3] = {firmware[i], firmware[i + 1], 0x00};
      if (spi_send(SPI_SEND_FIRMWARE, packet, &spi_status) != 0) {
         fprintf(stderr, "firmware transfer failed at byte %zu (status=%02x)\n", i, spi_status);
         free(firmware);
         return -1;
      }
      usleep(1000);
   }

   free(firmware);

   sleep(1);
   if (spi_send(SPI_START_CPU, no_param, &spi_status) != 0) {
      fprintf(stderr, "failed to send SPI_START_CPU (status=%02x)\n", spi_status);
      return -1;
   }
   sleep(1);
   return 0;
}

static int comet_send_in_char(uint8_t ch)
{
   uint8_t status = 0;
   uint8_t payload[3] = {0x00, ch, 0x00};
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

static int drain_output_until_idle(struct rx_capture *capture, unsigned int idle_timeout_ms)
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

      fprintf(stderr, "ignored packet: %02x %02x %02x (status=%02x)\n",
              packet[0], packet[1], packet[2], status);
   }

   return 0;
}

static void drain_output_for_window(struct rx_capture *capture, unsigned int window_ms)
{
   uint64_t deadline = now_ms() + window_ms;

   while (now_ms() < deadline) {
      uint8_t packet[3] = {0, 0, 0};
      uint8_t status = 0;

      if (comet_read_packet(packet, &status) != 0) {
         continue;
      }
      deadline = now_ms() + window_ms;

      if (packet[0] == COMET_RSP_OUT_CHAR) {
         rx_capture_push(capture, packet[1]);
      }
   }
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
   size_t i;
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

   if (load_and_start_comet_firmware(opts.firmware_path) != 0) {
      return 1;
   }

   if (opts.input_text != NULL) {
      printf("sending input: %s", opts.input_text);
      if (opts.input_text[0] != '\0' && opts.input_text[strlen(opts.input_text) - 1] != '\n') {
         printf("\n");
      }
      for (i = 0; opts.input_text[i] != '\0'; ++i) {
         if (comet_send_in_char((uint8_t)opts.input_text[i]) != 0) {
            fprintf(stderr, "failed to send char 0x%02x at index %zu\n",
                    (unsigned int)(uint8_t)opts.input_text[i], i);
            return 1;
         }
         /*
          * The legacy SPI IN/OUT firmware echoes one character at a time and the
          * host->CPU path is only single-entry buffered, so we opportunistically
          * drain immediate responses between transmitted bytes.
          */
         drain_output_for_window(&capture, INTER_CHAR_DRAIN_MS);
      }
   }

   if (drain_output_until_idle(&capture, opts.idle_timeout_ms) != 0) {
      return 1;
   }

   print_capture_summary(&capture);
   return 0;
}
