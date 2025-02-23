#include<stdarg.h>
#include "display.h"
#include "uitoa.h"
#include "stdint.h"

#define NULL 0

unsigned int currentX = 0;
unsigned int currentY = 0;
unsigned char currentColor = MAKECOLOR(GRAY,BLACK);

void kprintf(char *format, ...)
{
    char tmp[30];

    if (format == NULL)
        return;

    va_list arg;
    va_start(arg, format);

    for (;*(format) != '\0'; ++format)
    {
        if (*format == '%')
        {
            ++format;

            switch (*format)
            {
                case 'l':
                    
                    int base = 10;

                    ++format;
                    if (*format == 'x')
                        base = 16;
                    else
                        --format;
                                        
                    uitoa(va_arg( arg, uint64_t ), tmp, base);
                    kprint(tmp);
                break;

                case 'd':
                    uitoa(va_arg( arg, uint32_t ), tmp, 10);
                    kprint(tmp);
                break;

                case 'x':
                    uitoa(va_arg( arg, uint32_t ), tmp, 16);
                    kprint(tmp);
                break;

                case 'c':                    
                    kputchar((char)va_arg( arg, uint32_t));
                break;

                case 's':                    
                    kprint(va_arg( arg, char *));
                break;
            }
        }
        else
            kputchar(*format);        
    }

    va_end(arg);
}

void kprint(char *message)
{    
    if (message == NULL)
        return;

    for (;*(message) != '\0'; ++message)
        kputchar(*message);
}

void kputchar(char c)
{
    const unsigned char WIDTH = 80;
    const unsigned char HEIGHT = 25;    
    volatile unsigned short * const video = (unsigned short *)0xb8000;

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

    if (c == '\n')
    {
        currentX = 0;
        ++currentY;            
        return;
    }

    if (c == '\r')
    {
        currentX = 0;
        return;
    }

    unsigned int actualLocation = (currentY * WIDTH) + currentX;

    video[actualLocation] = (currentColor << 8) | c;        

    ++currentX;  
}

void setTextColor(unsigned char color)
{
    currentColor = color;
}

unsigned char getTextColor()
{
    return currentColor;
}