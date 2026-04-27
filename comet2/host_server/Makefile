CC      ?= gcc
FTDI_PKG := $(shell pkg-config --exists libftdi1 2>/dev/null && echo libftdi1 || echo libftdi)
CFLAGS  += $(shell pkg-config --cflags $(FTDI_PKG) 2>/dev/null)
LIBS    += $(shell pkg-config --libs   $(FTDI_PKG) 2>/dev/null || echo -lftdi)

SPI_LIB_SRC = ../../spi/spi_host/spi_lib.c
TARGETS = comet2_spi_loader comet2_spi_run

all: $(TARGETS)

comet2_spi_loader: $(SPI_LIB_SRC) comet2_spi_loader.c
	$(CC) $(CFLAGS) $(SPI_LIB_SRC) comet2_spi_loader.c -o $@ $(LIBS)

comet2_spi_run: $(SPI_LIB_SRC) comet2_spi_run.c
	$(CC) $(CFLAGS) $(SPI_LIB_SRC) comet2_spi_run.c -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)
