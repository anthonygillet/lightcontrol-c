#include "printer.h"
#include "main.h"
#include "rs232.h"
#include "insteon.h"

void print_result(char* fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    vprintf(fmt,args);
    va_end(args);
}

void print_debug(char* fmt, ...)
{
    if (DEBUG_ENABLED)
    {
        va_list args;
        va_start(args,fmt);
        vfprintf(stderr,fmt,args);
        va_end(args);
    }
}

void print_error(char* fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    vfprintf(stderr,fmt,args);
    va_end(args);
}
