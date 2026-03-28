MEMORY
{
   ram (rwxa)  : ORIGIN = 0x0, LENGTH = 32K
}

SECTIONS
{
   .text : {
      start.o(.text)    /* _start を必ず 0x0 に配置 */
      *(.text)
      *(.text.*)
   } > ram
   .data : { *(.data) *(.data.*) *(.rodata) *(.rodata.*) } > ram
   .bss  : { *(.bss) *(.bss.*) *(COMMON) } > ram
}
