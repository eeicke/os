#include "string.h"

int strlen(char *val)
{
    int result = 0;

    for (;*val != '\0'; ++result, ++val);

    return result;
}