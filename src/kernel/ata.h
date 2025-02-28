#ifndef _ATA_H
#define _ATA_H

#include "ioport.h"
#include "stdint.h"

#define BASEPORT 0x1F0
#define BLOCKSIZE 0x200 //512 bytes

void InitATA();
int HasSecondDrive();
int ReadBlockPIO(uint8_t, uint32_t, void*);
int WriteBlockPIO(uint8_t, uint32_t, void*);
int WaitForDriveReady();

#endif