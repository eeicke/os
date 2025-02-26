#include "ata.h"

int ReadBlockPIO(int lba, void* buffer)
{
    outByte(BASEPORT + 6, (0x0e0 | ((lba & 0x0F000000 ) >> 24)));
    outByte(BASEPORT + 2, 1);
    outByte(BASEPORT + 3, lba & 0x000000FF);
    outByte(BASEPORT + 4, (lba & 0x0000FF00 ) >> 8);
    outByte(BASEPORT + 5, (lba & 0x00FF0000 ) >> 16);
    outByte(BASEPORT + 7, 0x20); 

    if (WaitForDriveReady() < 0)
        return - 1;
    
    for (int i = 0; i < BLOCKSIZE / 4; ++i)
        ((uint32_t *)buffer)[i] = inDoubleWord(BASEPORT);

    return 0;
}

int WriteBlockPIO(int lba, void* buffer)
{
    outByte(BASEPORT + 6, (0x0e0 | ((lba & 0x0F000000) >> 24)));
    outByte(BASEPORT + 2, 1);
    outByte(BASEPORT + 3, lba & 0x000000FF);
    outByte(BASEPORT + 4, (lba & 0x0000FF00) >> 8);
    outByte(BASEPORT + 5, (lba & 0x00FF0000) >> 16);
    outByte(BASEPORT + 7, 0x30 );
    
    if (WaitForDriveReady() < 0)
        return - 1;
    
    for (int i = 0; i < (BLOCKSIZE / 4); ++i)
        outDoubleWord(BASEPORT, ((uint32_t *)buffer)[i]);
        
    return WaitForDriveReady();
}

int WaitForDriveReady()
{
    int status = 0;
    
    do
    {
        status = inWord(BASEPORT + 7);
    } 
    while ((status ^ 0x80) == 0x80);

    if ((status & 0x21) > 0) //Error
        return -1;    

    return 0;
}