#include "Log.h"
#include <cstdarg>
#include <cstdio>

static const char *log_type_strings[] = {
    "INFO",
    "ERROR",
    "DEBUG",
    "TRACE",
    "WARNING"
};

// Basic logging function
void Log(LogTag type, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    printf("[%s] ", log_type_strings[type]);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}
