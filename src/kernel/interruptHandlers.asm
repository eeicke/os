extern kernel_interrupt_handler ;this is main.c

int0:
    cli
    push 0
    jmp isrgeneric

int1:
    cli
    push 1
    jmp isrgeneric
    
int2:
    cli
    push 2
    jmp isrgeneric
    
int3:
    cli
    push 3
    jmp isrgeneric
    
int4:
    cli
    push 4
    jmp isrgeneric
    
int5:
    cli
    push 5
    jmp isrgeneric
    
int6:
    cli
    push 6
    jmp isrgeneric
    
int7:
    cli
    push 7
    jmp isrgeneric
    
int8:
    cli
    push 8
    jmp isrgeneric
    
int9:
    cli
    push 9
    jmp isrgeneric
    
int10:
    cli
    push 10
    jmp isrgeneric
    
int11:
    cli
    push 11
    jmp isrgeneric
    
int12:
    cli
    push 12
    jmp isrgeneric
    
int13:
    cli
    push 13
    jmp isrgeneric
    
int14:
    cli
    push 14
    jmp isrgeneric
    
int15:
    cli
    push 15
    jmp isrgeneric
    
int16:
    cli
    push 16
    jmp isrgeneric
    
int17:
    cli
    push 17
    jmp isrgeneric
    
int18:
    cli
    push 18
    jmp isrgeneric
    
int19:
    cli
    push 19
    jmp isrgeneric
    
int20:
    cli
    push 20
    jmp isrgeneric
    
int21:
    cli
    push 21
    jmp isrgeneric

int22:
    cli
    push 22
    jmp isrgeneric
    
int23:
    cli
    push 23
    jmp isrgeneric
    
int24:
    cli
    push 24
    jmp isrgeneric
    
int25:
    cli
    push 25
    jmp isrgeneric
    
int26:
    cli
    push 26
    jmp isrgeneric
    
int27:
    cli
    push 27
    jmp isrgeneric
    
int28:
    cli
    push 28
    jmp isrgeneric
    
int29:
    cli
    push 29
    jmp isrgeneric
    
int30:
    cli
    push 30
    jmp isrgeneric
    
int31:
    cli
    push 31
    jmp isrgeneric
    
int32:
    cli
    push 32
    jmp irqgeneric
    
int33:
    cli
    push 33
    jmp irqgeneric
    
int34:
    cli
    push 34
    jmp irqgeneric
    
int35:
    cli
    push 35
    jmp irqgeneric
    
int36:
    cli
    push 36
    jmp irqgeneric
    
int37:
    cli
    push 37
    jmp irqgeneric

int38:
    cli
    push 38
    jmp irqgeneric

int39:
    cli
    push 39
    jmp irqgeneric

int40:
    cli
    push 40
    jmp irqgeneric

int41:
    cli
    push 41
    jmp irqgeneric

int42:
    cli
    push 42
    jmp irqgeneric

int43:
    cli
    push 43
    jmp irqgeneric

int44:
    cli
    push 44
    jmp irqgeneric

int45:
    cli
    push 45
    jmp irqgeneric

int46:
    cli
    push 46
    jmp irqgeneric

int47:
    cli
    push 47
    jmp irqgeneric
    
int48:
    cli
    push 48
    jmp irqgeneric

;Generic handler for software interrupts
isrgeneric: 
    call kernel_interrupt_handler    
    pop eax    
    sti
    iret

;Generic handler for hardware interrupts
irqgeneric:
    call kernel_interrupt_handler
    ;Let primary PIC know we handled it.    
    mov al, 0x20
    out 0x20, al
    ;Was the IRQ from the secondary PIC? (IRQs 40-47)
    cmp byte [esp], 40
    jnge irqgenericDone
    ;Let secondary PIC know we handled it.
    mov al, 0xa0
    out 0x20, al    
irqgenericDone:
    pop eax        
    sti
    iret    