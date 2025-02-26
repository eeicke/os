#ifndef _MEMMAP_H
#define _MEMMAP_H

#include "stdint.h"

#define KILO 0x400
#define MEGA 0x100000
#define GIGA 0x40000000
#define TERA 0x10000000000

typedef struct MemoryMapEntry
{
    uint64_t baseAddress;
    uint64_t length;
    uint32_t type;
    uint32_t extended;
} MemoryMapEntry;

void DisplayMemoryMap(MemoryMapEntry *, int);

#endif