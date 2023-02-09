#include "display.h"
#include "uitoa.h"

void kernel_main()
{            
    kprint("Starting OS\n");

    while( 1 );
}
    
void kernel_interrupt_handler( int number )
{     
    unsigned char color = getTextColor();
    setTextColor(RED);
    kprintf("Intterupt %d\n", number);
    setTextColor(color);
}