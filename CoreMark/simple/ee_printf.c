#include "coremark.h"
#include "stdio.h"
#include "stc32_stc8_usb.h"
#include <stdarg.h>

int ee_printf(const char *fmt, ...)
{
    char xdata buf[1024];
		int len;
    va_list args;

    va_start(args, fmt);
    len = vsprintf(buf, fmt, args);
    va_end(args);
    USB_SendData((BYTE *)buf, len);
    return len;
}