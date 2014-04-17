
#include <stdarg.h>
#include <stdlib.h>
#include "usart.h"

void usart_write (const char *msg,...){
        char buf[256];
        va_list argp;
        va_start(argp, msg);
        vsnprintf(buf,256, msg, argp);
        printf(buf);
        va_end(argp);
}