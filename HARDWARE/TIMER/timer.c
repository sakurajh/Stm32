#include "timer.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32开发板
//通用定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2018/3/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////     
 

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM2_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<0; //TIM2时钟使能    
 	TIM2->ARR=arr;  	//设定计数器自动重装值 
	TIM2->PSC=psc;  	//预分频器设置
	TIM2->DIER|=1<<0;   //允许更新中断				
//	TIM2->CR1|=0x01;    //使能定时器2
	TIM2->CR1&=~(0x01); //关闭定时器2
  	MY_NVIC_Init(0,0,TIM2_IRQn,2);//抢占0，子优先级0，组2		
							 
}
