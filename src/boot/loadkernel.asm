loadKernel:
    mov ax, [currentSector]
    sub ax, 2
    mov bx, 512
    mul bx
    mov bx, ax

    mov ax, 0900h
    mov es, ax

    mov ah, 02h
    mov al, 1
    mov ch, 0
    mov cl, [currentSector]
    mov dh, 0
    mov dl, 80h
    int 13h

    jc loadKernelError

    sub byte [numberOfSectors], 1
    add byte [currentSector], 1
    cmp byte [numberOfSectors], 0
    jne loadKernel
    ret

loadKernelError:
    mov si, loadKernelErrorMessage        
    call sprint
    jmp $

sprint:    
    mov ah, 0Eh
cprint:
    lodsb
    cmp al, 0
    je cprintFinished
    int 10h
    jmp cprint
cprintFinished:   
    mov ah, 03h
    mov bh, 0
    int 10h
    mov ah, 02h    
    mov dl, 0
    int 10h
    ret

numberOfSectors db 6
currentSector db 2
loadKernelErrorMessage db 'Kernel Load Error', 10, 0