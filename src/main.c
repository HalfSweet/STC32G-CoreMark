#include "stc32g.h"
#include "stc32_stc8_usb.h"
#include "Type_def.h"
#include "STC32G_Delay.h"
#include "coremark.h"
#include "STC32G_Timer.h"
#include "STC32G_NVIC.h"

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#"; // 不停电自动 ISP 下载命令

volatile u32 SysTickFlag = 0; // 从启动开始每1ms加1

void CDC_init(void);
void Timer_config(void);
void main()
{
    CDC_init();
    Timer_config();
    IE2 |= 0x80; // 使能 USB 中断
    EA = 1;
    while (DeviceState != DEVSTATE_CONFIGURED)
        ; // 等待 USB 完成配置
		delay_ms(500);
    ee_printf("Hello \n");
    coremark_main();
    while (1)
    {
    }
}

void CDC_init(void)
{
    P_SW2 |= 0x80;
    P3M0 &= ~0x03;   // P3.0/P3.1 和 USB 的 D-/D+共用 PIN 脚，
    P3M1 |= 0x03;    // 需要将 P3.0/P3.1 设置为高阻输入模式
    IRC48MCR = 0x80; // 使能内部 48M 的 USB 专用 IRC
    //while (!(IRC48MCR & 0x01))
        ;
    USBCLK = 0x00; // 设置 USB 时钟源为内部 48M 的 USB 专用 IRC
    USBCON = 0x90; // 使能 USB 功能
    usb_init();    // 调用 USB CDC 初始化库函数
}

void Timer_config(void)
{
    TIM_InitTypeDef TIM_InitStructure;                                   // 结构定义
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload;                    // 指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;                      // 指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut = DISABLE;                              // 是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value = (u16)(65536UL - (MAIN_Fosc / 1000UL)); // 初值,
    TIM_InitStructure.TIM_Run = ENABLE;                                  // 是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);                           // 初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(ENABLE, Priority_0);                                // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}