#include "stc32g.h"
#include "stc32_stc8_usb.h"
#include "STC32G_Delay.h"

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#"; // 不停电自动 ISP 下载命令

void CDC_init(void);
const char helloStr[] = "Hello World!\r\n";
void main()
{
    CDC_init();
    IE2 |= 0x80; // 使能 USB 中断
    EA = 1;
    while (DeviceState != DEVSTATE_CONFIGURED)
        ; // 等待 USB 完成配置

    while (1)
    {
        USB_SendData((BYTE *)helloStr, sizeof(helloStr));
        delay_ms(1000);
    }
}

void CDC_init(void)
{
    P_SW2 |= 0x80;
    P3M0 &= ~0x03;   // P3.0/P3.1 和 USB 的 D-/D+共用 PIN 脚，
    P3M1 |= 0x03;    // 需要将 P3.0/P3.1 设置为高阻输入模式
    IRC48MCR = 0x80; // 使能内部 48M 的 USB 专用 IRC
    while (!(IRC48MCR & 0x01))
        ;
    USBCLK = 0x00; // 设置 USB 时钟源为内部 48M 的 USB 专用 IRC
    USBCON = 0x90; // 使能 USB 功能
    usb_init();    // 调用 USB CDC 初始化库函数
}