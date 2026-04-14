CC      ?= gcc
FTDI_PKG := $(shell pkg-config --exists libftdi1 2>/dev/null && echo libftdi1 || echo libftdi)
CFLAGS  += $(shell pkg-config --cflags $(FTDI_PKG) 2>/dev/null)
LIBS    += $(shell pkg-config --libs   $(FTDI_PKG) 2>/dev/null || echo -lftdi)

SRC     = ../../spi/spi_host/spi_lib.c comet2_inout_host.c
TARGET  = comet2_inout_host

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
