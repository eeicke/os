#include "uitoa.h"

char * uitoa(unsigned int value, char *buf, unsigned int base)
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