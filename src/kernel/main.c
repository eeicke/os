#include "stdint.h"
#include "display.h"
#include "memmap.h"
#include "ata.h"

unsigned char int32FiredOnce = 0;

void ShowMemoryMap();

void kernel_main()
{         
    kprint("Starting OS\n");  
        
    while( 1 );
}

void ShowMemoryMap()
{
    //The boot process stores the length of the array at address 0x8000
    //and the first entry is stored at 0x8004
    uint32_t *mmapSize = (uint32_t *)0x8000;
    kprintf("Memory Map Size: %d\n", (uint32_t)*mmapSize);
    displayMemoryMap((MemoryMapEntry *)0x8004, *mmapSize);
}
    
void kernel_interrupt_handler( int number )
{     
    if (int32FiredOnce && number == 32)
        return;

    if (number == 32)
        int32FiredOnce = 1;

    //kprintf("Int %d\n", number);
}