CC      ?= gcc
FTDI_PKG := $(shell pkg-config --exists libftdi1 2>/dev/null && echo libftdi1 || echo libftdi)
CFLAGS  += $(shell pkg-config --cflags $(FTDI_PKG) 2>/dev/null)
LIBS    += $(shell pkg-config --libs   $(FTDI_PKG) 2>/dev/null || echo -lftdi)

SPI_LIB_SRC = ../../../spi/spi_host/spi_lib.c
TARGET = comet2_inout_host

all: $(TARGET)

$(TARGET): $(SPI_LIB_SRC) comet2_inout_host.c
	$(CC) $(CFLAGS) $(SPI_LIB_SRC) comet2_inout_host.c -o $@ $(LIBS)

clean:
	rm -f $(TARGET)
