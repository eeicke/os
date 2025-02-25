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
    
    kprintf("Memory Map Size: %d\n", (uint32_t)*mmapSize);
    
    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)0x8004;

    for (int i = 0; i < *mmapSize; ++i, ++mmap)
    {                
        //This is a 32-bit system so cast the 64bit addresses
        //to 32bit since we won't have anything above a 32bit address.
        //Otherwise operations on uint64_t may not compile properly.
        kprintf("Base Address: %08x", (uint32_t)mmap->baseAddress);        
        
        if (mmap->length >= (1024 * 1024 * 1024))
            kprintf(" Length: %uG", ((uint32_t)mmap->length) / (1024 * 1024 * 1024));
        else if (mmap->length >= (1024 * 1024))
            kprintf(" Length: %uM", ((uint32_t)mmap->length) / (1024 * 1024));
        else if (mmap->length >= 1024)
            kprintf(" Length: %uK", ((uint32_t)mmap->length) / 1024);

        //kprintf(" Type: %u\n", mmap->type);
        kprint(" Type: ");

        switch (mmap->type)
        {
            case 1:
                kprint("Free");
            break;

            case 2:
                kprint("Reserved");
            break;

            default:
                kprintf("%u", mmap->type);
            break;
        }

        kprint("\n");
        
    }

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