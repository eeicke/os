global OutByte
global OutWord
global OutDword
global InByte
global InWord
global InDword

;void OutByte(unsigned int port, unsigned char val)
OutByte:
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

;void OutWord(unsigned int port, unsigned short val)
OutWord:
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

;void OutDword(unsigned int port, unsigned int val)
OutDword:
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

;unsigned usigned char InByte(unsigned int port)
InByte:
    ;save this because we need to use it
    push edx

    xor edx, edx
    xor eax, eax
    
    mov dx, [esp + 8]
    
    in al, dx
    
    pop edx

    ret

;unsigned short InWord(unsigned int port)
InWord:
    ;save this because we need to use it
    push edx

    xor edx, edx
    xor eax, eax
    
    mov dx, [esp + 8]
    
    in ax, dx
    
    pop edx

    ret

;unsigned int InDword(unsigned int port)
InDword:
    ;save this because we need to use it
    push edx

    xor edx, edx
    xor eax, eax
    
    mov dx, [esp + 8]
    
    in eax, dx
    
    pop edx

    ret