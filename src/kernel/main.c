#include "stdint.h"
#include "display.h"
#include "memmap.h"
#include "ata.h"

unsigned char int32FiredOnce = 0;

void kernel_main()
{         
    kprint("Starting OS\n");     
    
    // uint8_t *buf = (uint8_t *)0x100000;

    // for (int i = 0; i < 512; ++i)
    //     buf[i] = '\0';


    // buf[0] = 'D';
    // buf[1] = 'u';
    // buf[2] = 'm';
    // buf[3] = 'p';
    // buf[4] = 'y';

    // if (WriteBlockPIO(0, 2048, buf) < 0)
    //     kprint("Failed\n");
    // else
    //     kprint("OK\n");
    
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