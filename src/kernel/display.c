#include "stdarg.h"
#include "display.h"
#include "uitoa.h"
#include "string.h"
#include "math.h"

#define NULL 0

unsigned int currentX = 0;
unsigned int currentY = 0;
unsigned char currentColor = MAKECOLOR(GRAY,BLACK);

typedef enum State
{
    looking = 0,
    specifier = 1,
    zeroPadding = 2,
    precision = 3,
    longInt = 4
} State;


void kprintf(char *format, ...)
{    
    if (format == NULL)
        return;

    va_list arg;
    va_start(arg, format);

    enum State state = looking;    
    int zeroPadWidth = 0;    
    int precisionWidth = 0;

    for (;*(format) != '\0'; ++format)
    {
        switch (state)
        {
            case looking:
                if (*format == '%')
                    state = specifier;
                else
                    kputchar(*format); 
            break;

            case specifier:                                
                switch (*format)
                {
                    case '%':
                        state = looking;
                        kputchar(*format); 
                    break;

                    case '.':                           
                        precisionWidth = 0;
                        state = precision;
                    break;

                    case '0':                           
                        zeroPadWidth = 0;
                        state = zeroPadding;
                    break;                    

                    case 'd':
                        printSignedInteger(va_arg( arg, int32_t ), zeroPadWidth, 10);
                        state = looking;
                    break;

                    case 'u':
                        printUnsignedInteger(va_arg( arg, uint32_t ), zeroPadWidth, 10);
                        state = looking;
                    break;

                    case 'x':
                        printUnsignedInteger(va_arg( arg, uint32_t ), zeroPadWidth, 16);
                        state = looking;
                    break;

                    case 'l':
                        state = longInt;                        
                    break;

                    case 'f':                                                              
                        printFloat(va_arg( arg, double ), zeroPadWidth, precisionWidth);
                        state = looking;
                    break;

                    case 'c':
                        kputchar((char)va_arg( arg, uint32_t));
                        state = looking;
                    break;

                    case 's':
                        kprint(va_arg( arg, char *));
                        state = looking;
                    break;

                    default:
                        state = looking;
                    break;
                }
            break;

            case zeroPadding:
                if (*format >= '0' && *format <= '9')
                    zeroPadWidth = (zeroPadWidth * 10) + (*format - '0');
                else if (*format == '.')
                {
                    precisionWidth = 0;
                    state = precision;
                }
                else
                {
                    --format;
                    state = specifier;
                }
            break;

            case precision:                
                if (*format >= '0' && *format <= '9')
                    precisionWidth = (precisionWidth * 10) + (*format - '0');
                else
                {
                    --format;
                    state = specifier;
                }                
            break;

            case longInt:
                if (*format == 'x')
                    printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 16);
                else
                {
                    printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 10);
                    --format;
                }
                        
                state = looking;                    
            break;
        }
    }

    if (state == longInt)
        printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 10);
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

void kprintmem(uint8_t * start, uint32_t length)
{
    int bytesPrinted = 0;

    for (int i = 0; i < length; ++i)
    {
        if (bytesPrinted == 0)
            kprintf("%08x: ", start);

        kprintf("%02x ", *start);

        ++bytesPrinted;
        ++start;

        if (bytesPrinted != 0 && bytesPrinted % 16 == 0)
        {
            kprint(" ");
            for (int j = 16; j > 0; --j)
            {
                char val = *(start - j);

                if (val > 32)
                    kputchar(val);
                else 
                    kputchar('.');
            }

            bytesPrinted = 0;
            kprint("\n");
        }
    }

    if (bytesPrinted != 0)
    {
        for (int j = bytesPrinted; j < 16; ++j)
            kprint("   ");

        kprint(" ");

        for (int j = bytesPrinted; j > 0; --j)
        {
            char val = *(start - j);

            if (val > 32)
                kputchar(val);
            else 
                kputchar('.');
        }

        kprint("\n");
    }
}

void printSignedInteger(int32_t val, uint32_t zeroPadWith, uint32_t base)
{    
    char tmp[30];

    if (val < 0)
    {
        kputchar('-');
        val = abs32(val);
    }

    uitoa(val, tmp, base);
    
    if (zeroPadding > 0)
    {
        int len = strlen(tmp);

        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
                kputchar('0');
        }
    }    

    kprint(tmp);
}

void printUnsignedInteger(uint32_t val, uint32_t zeroPadWith, uint32_t base)
{    
    char tmp[30];

    uitoa(val, tmp, base);
    
    if (zeroPadding > 0)
    {
        int len = strlen(tmp);

        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
                kputchar('0');
        }
    }    

    kprint(tmp);
}

void printFloat(float val, uint32_t zeroPadWith, uint32_t precisionWidth)
{      
    char tmp[30];    

    ftoa(val, tmp, precisionWidth);
    
    if (zeroPadding > 0)
    {        
        int len = 0;
        
        for (;tmp[len] != '\0' && tmp[len] != '.'; ++len);
        
        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
                kputchar('0');
        }
    }    

    kprint(tmp);
}

void setTextColor(unsigned char color)
{
    currentColor = color;
}

unsigned char getTextColor()
{
    return currentColor;
}
