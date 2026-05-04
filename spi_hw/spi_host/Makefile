# all:
# 	gcc spi_lib.c main.c -o host -lftdi
#Makefile stays simple for this example

CC      ?= gcc
# Detect libftdi via pkg-config; prefer the libftdi1 name used by Homebrew/newer distros
FTDI_PKG := $(shell pkg-config --exists libftdi1 2>/dev/null && echo libftdi1 || echo libftdi)
CFLAGS  += $(shell pkg-config --cflags $(FTDI_PKG) 2>/dev/null)
LIBS    += $(shell pkg-config --libs   $(FTDI_PKG) 2>/dev/null || echo -lftdi)

SRC     = spi_lib.c main.c
TARGET  = host

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)