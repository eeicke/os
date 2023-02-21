global outByte
global outWord
global outDoubleWord
global inDoubleWord

;void outByte(unsigned int port, unsigned char val)
outByte:
    ;save these because we need to use them
    push edx
    push eax

    ;zero them out
    xor edx, edx
    xor eax, eax

    mov dx, [esp + 12]
    mov al, [esp + 16]

    out dx,al

    pop eax
    pop edx

    ret

;void outWord(unsigned int port, unsigned short val)
outWord:
    ;save these because we need to use them
    push edx
    push eax

    ;zero them out
    xor edx, edx
    xor eax, eax

    mov dx, [esp + 12]
    mov ax, [esp + 16]

    out dx,ax

    pop eax
    pop edx

    ret

;void outDoubleWord(unsigned int port, unsigned int val)
outDoubleWord:
    ;save these because we need to use them
    push edx
    push eax

    ;zero them out
    xor edx, edx
    xor eax, eax

    mov dx, [esp + 12]
    mov eax, [esp + 16]

    out dx,eax
    
    pop eax    
    pop edx

    ret

;unsigned int inDoubleWord(unsigned int port)
inDoubleWord:
    ;save this because we need to use it
    push edx

    xor edx, edx
    xor eax, eax
    
    mov dx, [esp + 8]
    
    in ax, dx
    
    pop edx

    ret