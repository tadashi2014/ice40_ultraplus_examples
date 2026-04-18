# Homebrew's RISC-V toolchain ships as riscv64-unknown-elf-* but supports rv32i
# via -march=rv32i / -mabi=ilp32 flags, so the 64-bit prefix is intentional.
RISCV_PREFIX ?= riscv64-unknown-elf-

CC      = $(RISCV_PREFIX)gcc
AS      = $(RISCV_PREFIX)as
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump

ARCH_FLAGS = -march=rv32i -mabi=ilp32 -O0
PROGRAM ?= main

all: $(PROGRAM)

%.o: %.c
	$(CC) $(ARCH_FLAGS) -c $< -o $@

start.o: start.S
	$(AS) -march=rv32i -mabi=ilp32 start.S -o start.o

%.oo: %.o start.o script.lds
	$(CC) $(ARCH_FLAGS) -T script.lds -lgcc -nostartfiles start.o $< -o $@

%: %.oo
	$(OBJCOPY) -O binary $< $@

objdump: $(PROGRAM).oo
	$(OBJDUMP) -d $(PROGRAM).oo

clean:
	rm -f *.o *.oo main uart_echo uart_tx_test
