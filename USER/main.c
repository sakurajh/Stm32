#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"

int main(void)
{
    Stm32_Clock_Init(9); 	//系统时钟设置
    delay_init(72);	     	//延时初始化
    LED_Init();		  	    //初始化与LED连接的硬件接口
    BEEP_Init();            //初始化蜂鸣器端口

    while (1)
    {
        LED1 = 0;
        BEEP = 0;
        delay_ms(300);//延时300ms
        LED1 = 1;
        BEEP = 1;
        delay_ms(300);//延时300ms
    }
}

