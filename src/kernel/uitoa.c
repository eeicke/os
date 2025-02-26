#include "uitoa.h"
#include "string.h"
#include "math.h"

char * uitoa(uint32_t value, char *buf, uint32_t base)
{
    const char * nums = "0123456789ABCDEF";

    if (base != 2 && base != 10 && base != 16)
    {
        *(buf) = '\0';
        return buf;
    }

    char *result = buf;    
    char *start = buf;

    do
    {
        *buf++ = nums[value % base];
        value /= base;
        
    } while (value > 0);

    *buf-- = '\0';
    
    while (start < buf)
    {
        char tmp = *start;
        *start++ = *buf;
        *buf-- = tmp;
    }

    return result;
}

char * ftoa(float value, char *buf, uint32_t precision)
{
    int intPart = (int)value;
    
    char *temp = uitoa(intPart, buf, 10);   
        
    if (precision > 0)
    {        
        for (;*temp != '\0'; ++temp);

        *temp = '.';

        ++temp;

        float fracPart = (value - (float)intPart) * pow(10, precision);

        temp = uitoa(fracPart, temp, 10);   
    }

    return buf;
}