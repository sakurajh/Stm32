#include "led.h"
#include "delay.h"
#include "sys.h"

/*实现LED灯交替闪烁*/
//int main(void)
//{

//    Stm32_Clock_Init(9); 	//系统时钟设置
//    delay_init(72);	     	//延时初始化
//    LED_Init();	 //初始化与LED连接的硬件接口

//    while (1)
//    {
//        LED0 = 0; //LED0亮
//        LED1 = 1; //LED1灭
//        delay_ms(500);
//        LED0 = 1; //LED0灭
//        LED1 = 0; //LED1亮
//        delay_ms(500);
//    }
//}


/*实现流水灯*/

 int main(void)
 {
	u8 LED=0x00;
	u8 i=0;

	Stm32_Clock_Init(9); 	//系统时钟设置
	delay_init(72);	     	//延时初始化
	LED_Init();	            //初始化与LED连接的硬件接口

	while(1)
	{
		for(i=0;i<8;i++)
		{
			LED++;
			GPIOC->BRR = LED;
			delay_ms(200);
			LED<<=1;//LED依次点亮
		}
		LED++;
		GPIOC->BRR = LED;
		delay_ms(200);
		LED=0xff;//LED全灭
		GPIOC->BSRR = LED;
		delay_ms(200);
	}
 }
