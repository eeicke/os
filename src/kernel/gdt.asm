gdt:
    dw 0x0000, 0x0000, 0x0000, 0x0000 ;NULL Entry
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF ;Kernel Code
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF ;Kernel Data
    dw 0xFFFF, 0x0000, 0xFA00, 0x00CF ;User Code
    dw 0xFFFF, 0x0000, 0xF200, 0x00CF ;User Data

gdtr:
    dw gdtr - gdt
    dd gdt