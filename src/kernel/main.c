#include "stdint.h"
#include "display.h"
#include "memmap.h"
#include "ata.h"

unsigned char int32FiredOnce = 0;

void kernel_main()
{         
    kprint("Starting OS\n");     

    InitATA();    

    while( 1 );
}

void kernel_interrupt_handler( int number )
{     
    if (int32FiredOnce && number == 32)
        return;

    if (number == 32)
        int32FiredOnce = 1;

    //kprintf("Int %d\n", number);
}