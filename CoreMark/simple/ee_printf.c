#include "coremark.h"
#include "stdio.h"
#include "stc32_stc8_usb.h"

int ee_printf(const char *fmt, ...)
{
    char buf[32];
    va_list args;

    va_start(args, fmt);
    int len = vsprintf(buf, fmt, args);
    va_end(args);
    USB_SendData((BYTE *)buf, len);
    return len;
}