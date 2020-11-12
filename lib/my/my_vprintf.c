#include "my.h"

#include <stdio.h>

int my_vprintf(const char *fmt, va_list ap)
{
    int res;
    
    res = vprintf(fmt, ap);

    fflush(stdout);
    fflush(stderr);

    return (res);
}