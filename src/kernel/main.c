#include "display.h"
#include "uitoa.h"

void kernel_main()
{            
    kprint("Starting OS\n");    

    while( 1 );
}
    
void kernel_interrupt_handler( int number )
{        
    char *buf = (char *)0x100000;

    buf = uitoa(number, buf, 10);

    unsigned char color = getTextColor();
    setTextColor(RED);
    kprint("Here ");
    kprint(buf);
    kprint("\n");
    setTextColor(color);
}