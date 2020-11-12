#include "my.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int my_printf(const char *fmt, ...)
{
    int res = 0;
    va_list ap;
    
    va_start(ap, fmt);
    res = my_vprintf(fmt, ap);
    va_end(ap);

    return (res);
}