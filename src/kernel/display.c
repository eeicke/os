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
    spacePadding = 3,
    precision = 4,
    longInt = 5
} State;


int kprintf(char *format, ...)
{    
    if (format == NULL)
        return 0;

    va_list arg;
    va_start(arg, format);

    enum State state = looking;    
    int zeroPadWidth = 0;    
    int spacePadWidth = 0;
    int precisionWidth = 0;
    int precisionFlag = 0;

    int position = 0;

    for (;*(format) != '\0'; ++format)
    {
        switch (state)
        {
            case looking:                
                if (*format == '%')
                {
                    zeroPadWidth = 0;
                    precisionFlag = 0;
                    precisionWidth = 0;
                    spacePadWidth = 0;
                    state = specifier;

                    continue;
                }
                
                kputchar(*format); 
                ++position;
            break;

            case specifier:                                
                switch (*format)
                {
                    case '%':
                        state = looking;
                        kputchar(*format); 
                        ++position;
                    break;

                    case '.':                           
                        precisionFlag = 1;                        
                        state = precision;
                    break;

                    case '0':                              
                        state = zeroPadding;
                    break;         
                    
                    case '1': case '2': case '3': 
                    case '4': case '5': case '6': 
                    case '7': case '8': case '9':
                        spacePadWidth = *format - '0';                        
                        state = spacePadding;
                    break;

                    case 'n':
                        int32_t *nval = va_arg(arg, int32_t *);
                        *nval = position;
                        state = looking;
                    break;

                    case 'd':
                        position += printSignedInteger(va_arg(arg, int32_t), zeroPadWidth, 10);
                        state = looking;
                    break;

                    case 'u':
                        position += printUnsignedInteger(va_arg(arg, uint32_t), zeroPadWidth, 10);
                        state = looking;
                    break;

                    case 'x':
                        position += printUnsignedInteger(va_arg(arg, uint32_t), zeroPadWidth, 16);
                        state = looking;
                    break;

                    case 'l':
                        state = longInt;                        
                    break;

                    case 'f':                                                              
                        position += printFloat(va_arg(arg, double), zeroPadWidth, precisionFlag == 1 ? precisionWidth : 6);
                        state = looking;
                    break;

                    case 'c':
                        kputchar((char)va_arg(arg, uint32_t));
                        ++position;
                        state = looking;
                    break;

                    case 's':                        
                        char *sval = va_arg(arg, char *);

                        int len = strlen(sval);  

                        if (spacePadWidth > 0 && (len < spacePadWidth))
                        {                                                                                  
                            for (int i = 0; i < (spacePadWidth - len); ++i)
                            {
                                kputchar(' ');
                                ++position;
                            }                                                
                        }

                        position += len;
                        
                        kprint(sval);

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
                    precisionFlag = 1;                     
                    state = precision;
                }
                else
                {
                    --format;
                    state = specifier;
                }
            break;

            case spacePadding:
                if (*format >= '0' && *format <= '9')
                    spacePadWidth = (spacePadWidth * 10) + (*format - '0');
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
                    position += printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 16);
                else
                {
                    position += printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 10);
                    --format;
                }
                        
                state = looking;                    
            break;
        }
    }

    if (state == longInt)
        position += printUnsignedInteger(va_arg( arg, uint64_t ), zeroPadWidth, 10);

    return position;
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

int printSignedInteger(int32_t val, uint32_t zeroPadWith, uint32_t base)
{    
    char tmp[30];

    int result = 0;    

    if (val < 0)
    {
        kputchar('-');
        ++result;
        val = abs32(val);
    }

    uitoa(val, tmp, base);
    
    if (zeroPadding > 0)
    {
        int len = strlen(tmp);

        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
            {
                kputchar('0');
                ++result;
            }
        }
    }    

    result += strlen(tmp);

    kprint(tmp);

    return result;
}

int printUnsignedInteger(uint32_t val, uint32_t zeroPadWith, uint32_t base)
{    
    char tmp[30];

    int result = 0;    

    uitoa(val, tmp, base);
    
    if (zeroPadding > 0)
    {
        int len = strlen(tmp);

        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
            {
                kputchar('0');
                ++result;
            }
        }
    }    

    result += strlen(tmp);

    kprint(tmp);

    return result;
}

int printFloat(float val, uint32_t zeroPadWith, uint32_t precisionWidth)
{      
    char tmp[30];
    
    int result = 0;

    ftoa(val, tmp, precisionWidth);
    
    if (zeroPadding > 0)
    {        
        int len = 0;
        
        for (;tmp[len] != '\0' && tmp[len] != '.'; ++len);
        
        if (len < zeroPadWith)
        {
            for (int i = 0; i < zeroPadWith - len; ++i)
            {
                kputchar('0');
                ++result;
            }
        }
    }    

    result += strlen(tmp);

    kprint(tmp);

    return result;
}

void setTextColor(unsigned char color)
{
    currentColor = color;
}

unsigned char getTextColor()
{
    return currentColor;
}
