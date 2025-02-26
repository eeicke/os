#include "math.h"

int32_t abs32(int32_t val)
{
    if (val >= 0)
        return val;

    return -val;
}

int64_t abs64(int64_t val)
{
    if (val >= 0)
        return val;

    return -val;
}

int32_t pow(int32_t base, int32_t exponent)
{
    int32_t result = 1;
    
    for (int32_t i = 0; i < exponent; ++i)
        result *= base;    

    return result;
}