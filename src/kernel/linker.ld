SECTIONS
{
  .text 0x09000 : { *(.text) }

  .data : { *(.data) *(.rodata) }

  .bss : { *(.bss) }  
} 