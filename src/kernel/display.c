#include "display.h"

#define NULL 0

unsigned int currentX = 0;
unsigned int currentY = 0;
unsigned char currentColor = MAKECOLOR(GRAY,BLACK);

void kprint(char *message)
{    
    const unsigned char WIDTH = 80;
    const unsigned char HEIGHT = 25;    
    volatile unsigned short * const video = (unsigned short *)0xb8000;

    if (message == NULL)
        return;

    for (;*(message) != '\0'; ++message)
    {
        if (currentX >= WIDTH)
        {
            ++currentY;
            currentX = 0;
        }

        if (currentY >= HEIGHT)
        {                 
            //scroll up                   
            for (int i = 0; i < (HEIGHT * WIDTH); ++i)
            {
                if (i < ((HEIGHT - 1) * WIDTH))
                    video[i] = video[i + WIDTH];
                else
                    video[i] = 0x0;                    
            }

            --currentY;
        }

        if (*(message) == '\n')
        {
            currentX = 0;
            ++currentY;            
            continue;
        }

        if (*(message) == '\r')
        {
            currentX = 0;
            continue;
        }

        unsigned int actualLocation = (currentY * WIDTH) + currentX;

        video[actualLocation] = (currentColor << 8) | *message;        

        ++currentX;     
    }
}


void setTextColor(unsigned char color)
{
    currentColor = color;
}

unsigned char getTextColor()
{
    return currentColor;
}