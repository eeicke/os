#ifndef _ATA_H
#define _ATA_H

#include "ioport.h"
#include "stdint.h"

#define BASEPORT 0x1F0
#define BLOCKSIZE 0x200 //512 bytes

int ReadBlockPIO(int, void*);
int WriteBlockPIO(int, void*);
int WaitForDriveReady();

#endif