# Homebrew's RISC-V toolchain ships as riscv64-unknown-elf-* but supports rv32i
# via -march=rv32i / -mabi=ilp32 flags, so the 64-bit prefix is intentional.
RISCV_PREFIX ?= riscv64-unknown-elf-

CC      = $(RISCV_PREFIX)gcc
AS      = $(RISCV_PREFIX)as
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump

ARCH_FLAGS = -march=rv32i -mabi=ilp32 -O0

all: main

main.o: main.c
	$(CC) $(ARCH_FLAGS) -c main.c -o main.o

start.o: start.S
	$(AS) -march=rv32i -mabi=ilp32 start.S -o start.o

# lgcc / libgcc contains soft-float helpers (e.g. mulsi3)
# nostartfiles avoids linking the default _start from crt0
main.oo: main.o start.o script.lds
	$(CC) -T script.lds -lgcc -nostartfiles start.o main.o -o main.oo

main: main.oo
	$(OBJCOPY) -O binary main.oo main

objdump: main.oo
	$(OBJDUMP) -d main.oo

clean:
	rm -f main.o start.o main.oo main
