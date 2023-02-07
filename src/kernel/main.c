#include "display.h"

void kernel_main()
{            
    kprint("Starting OS\n");    

    while( 1 );
}
    
void kernel_interrupt_handler( int number )
{        
    unsigned char color = getTextColor();
    setTextColor(RED);
    kprint("Here\n");
    setTextColor(color);
}