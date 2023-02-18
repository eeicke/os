bits 16

extern kernel_main ;this is main.c

main:        
    mov ax, cs
    mov ds, ax    
        
    cli    

    ;Set video mode to clear the screen
    mov ah, 0h
    mov al, 03h
    int 10h
    
    ;Disable cursor for now
    mov ah, 01h
    mov cx, 2000h
    int 10h

    ;Set up GDT
    ;We subtract here to get the offset of gdtr because it's relative to the DS in real mode.
    lgdt [gdtr - main]

    ;Enter protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ;Remap PIC
    mov al, 11h
    
    ;Init primary PIC
    out 0x20, al
        
    ;Init secondary PIC
    out 0xa0, al
        
    ;Remap IRQ 0-7 to 32-39
    mov al, 32d
    out 0x21, al
    
    ;Remap IRQ 8-15 to 40-47    
    mov al, 40d
    out 0xa1, al 
    
    ;Connect secondary PIC to primary on IRQ 2
    mov al, 04h
    out 0x21, al
    
    ;Connect secondary PIC to primary on IRQ 2
    mov al, 02h
    out 0xa1, al
    
    ;Set primary and secondary PIC to x86 arch
    mov al, 01h
    out 0x21, al
    out 0xa1, al
    
    ;Enable all IRQs on both PICs
    mov al, 0h    
    out 0x21, al        
    out 0xa1, al
    
    ;Load interrupt table
    ;We subtract here to get the offset of idtr because it's relative to the DS in real mode.
    lidt [idtr - main]

    ;Do required jmp after switching to protected mode
    jmp 08h:start
    
bits 32
start:
    mov eax, 10h
    mov ds, eax
    mov ss, eax
    mov eax, 0h
    mov es, eax
    mov fs, eax
    mov gs, eax
    sti
    jmp kernel_main

%include "gdt.asm"
%include "idt.asm"