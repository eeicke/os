#include "ata.h"

static int hasSecondDrive = 0;

void InitATA()
{
    //Check for Second Drive
    hasSecondDrive = 0;

    OutByte(BASEPORT + 6, 0x0F0);

    for(int i = 0; i < 1000; ++i)
    {
      if(InByte(BASEPORT + 7) != 0)
      {        
        hasSecondDrive = 1;
        break;
      }
    }
        
    //Switch to Primary Drive
    OutByte(BASEPORT + 6, 0xE0);
}

int HasSecondDrive()
{
    return hasSecondDrive;
}

int ReadBlockPIO(uint8_t drive, uint32_t lba, void* buffer)
{
    if (drive > 1)
        drive = 1;

    OutByte(BASEPORT + 6, ((0x0E0 | (drive << 4)) | ((lba & 0x0F000000 ) >> 24)));            
    OutByte(BASEPORT + 2, 1);
    OutByte(BASEPORT + 3, lba & 0x000000FF);
    OutByte(BASEPORT + 4, (lba & 0x0000FF00) >> 8);
    OutByte(BASEPORT + 5, (lba & 0x00FF0000) >> 16);
    OutByte(BASEPORT + 7, 0x20); 

    if (WaitForDriveReady() < 0)
        return - 1;
    
    for (int i = 0; i < BLOCKSIZE / 4; ++i)
        ((uint32_t *)buffer)[i] = InDword(BASEPORT);

    return 0;
}

int WriteBlockPIO(uint8_t drive, uint32_t lba, void* buffer)
{
    if (drive > 1)
        drive = 1;    

    OutByte(BASEPORT + 6, ((0x0E0 | (drive << 4)) | ((lba & 0x0F000000 ) >> 24)));
    OutByte(BASEPORT + 2, 1);
    OutByte(BASEPORT + 3, lba & 0x000000FF);
    OutByte(BASEPORT + 4, (lba & 0x0000FF00) >> 8);
    OutByte(BASEPORT + 5, (lba & 0x00FF0000) >> 16);
    OutByte(BASEPORT + 7, 0x30 );
    
    if (WaitForDriveReady() < 0)
        return - 1;
    
    for (int i = 0; i < (BLOCKSIZE / 4); ++i)
        OutDword(BASEPORT, ((uint32_t *)buffer)[i]);
        
    return WaitForDriveReady();
}

int WaitForDriveReady()
{
    int status = 0;
    
    do
    {
        status = InWord(BASEPORT + 7);
    } 
    while ((status ^ 0x80) == 0x80);

    if ((status & 0x21) > 0) //Error
        return -1;    

    return 0;
}