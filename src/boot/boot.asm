start:        
    mov ax, 07C0h    
    mov ds, ax 
    call loadKernel   
    jmp 0900h:0000
    
%include "loadkernel.asm"

times 510-($-$$) db 0
dw 0xAA55