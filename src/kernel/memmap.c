#include "memmap.h"
#include "display.h"

void DisplayMemoryMap()
{
    //The boot process stores the length of the array at address 0x8000
    //and the first entry is stored at 0x8004    
    int size = *((uint32_t *)0x8000);
    MemoryMapEntry *mmap = (MemoryMapEntry *)0x8004;

    kprintf("Memory Map Size: %d\n", size);

    for (int i = 0; i < size; ++i, ++mmap)
    {                
        //This is a 32-bit system so cast the 64bit addresses
        //to 32bit since we won't have anything above a 32bit address.
        //Otherwise operations on uint64_t may not compile properly.
        kprintf("Base Address: %08x", (uint32_t)mmap->baseAddress);        
        
        if (mmap->length >= GIGA)
            kprintf(" Length: %uG", ((uint32_t)mmap->length) / GIGA);
        else if (mmap->length >= MEGA)
            kprintf(" Length: %uM", ((uint32_t)mmap->length) / MEGA);
        else if (mmap->length >= KILO)
            kprintf(" Length: %uK", ((uint32_t)mmap->length) / KILO);
        
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
}