CC      ?= gcc
# Detect libftdi via pkg-config; prefer the libftdi1 name used by Homebrew/newer distros
FTDI_PKG := $(shell pkg-config --exists libftdi1 2>/dev/null && echo libftdi1 || echo libftdi)
CFLAGS  += $(shell pkg-config --cflags $(FTDI_PKG) 2>/dev/null)
LIBS    += $(shell pkg-config --libs   $(FTDI_PKG) 2>/dev/null || echo -lftdi)

SRC     = ../../spi/spi_host/spi_lib.c main.c
TARGET  = host
COMET_INOUT_SRC = ../../spi/spi_host/spi_lib.c comet2_inout_host.c
COMET_INOUT_TARGET = comet2_inout_host

all: $(TARGET) $(COMET_INOUT_TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

$(COMET_INOUT_TARGET): $(COMET_INOUT_SRC)
	$(CC) $(CFLAGS) $(COMET_INOUT_SRC) -o $(COMET_INOUT_TARGET) $(LIBS)

clean:
	rm -f $(TARGET) $(COMET_INOUT_TARGET)