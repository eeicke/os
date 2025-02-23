#include "display.h"
#include "uitoa.h"
#include "ioport.h"
#include "stdint.h"

unsigned char int32FiredOnce = 0;

struct MemoryMapEntry
{
    uint64_t baseAddress;
    uint64_t length;
    uint32_t type;
    uint32_t extended;
};

void kernel_main()
{            
    kprint("Starting OS\n");    
        
    uint32_t *mmapSize = (uint32_t *)0x8000;    
    
    kprintf("Size: %d\n", (uint32_t)*mmapSize);
    
    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)0x8004;

    for (int i = 0; i < *mmapSize; ++i, ++mmap)
    {        
        kprintf("Base Address: %lx", mmap->baseAddress);
        
        if (mmap->length >= (1024 * 1024 * 1024))
            kprintf(" Length: %lG", mmap->length / (1024 * 1024 * 1024));
        else if (mmap->length >= (1024 * 1024))
            kprintf(" Length: %lM", mmap->length / (1024 * 1024));
        else if (mmap->length >= 1024)
            kprintf(" Length: %lK", mmap->length / 1024);
     
        kprintf(" Type: %d\n", mmap->type);
    }

    while( 1 );
}
    
void kernel_interrupt_handler( int number )
{     
    if (int32FiredOnce && number == 32)
        return;

    if (number == 32)
        int32FiredOnce = 1;

    kprintf("Int %d\n", number);
}