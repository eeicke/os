extern void kprintWithColor(char *, unsigned char);
extern void kprint(char *);

void kernel_main()
{        
    kprint("Starting OS\n");    

    while( 1 );
}
    
void kernel_interrupt_handler( int number )
{             
}