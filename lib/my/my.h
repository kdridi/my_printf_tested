#ifndef __MY_H__
#define __MY_H__

#include <stdarg.h>

int my_vprintf(const char *fmt, va_list ap);
int my_printf(const char *fmt, ...);

#endif /* __MY_H__ */