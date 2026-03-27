.globl _start
_start:
    lui  sp, 0x4
    addi sp, sp, -4
    j main
loop: j loop
