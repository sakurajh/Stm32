#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32开发板
//SPI驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2018/3/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2018-2028
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////    
 

//以下是SPI模块的初始化代码，配置成主机模式						  
//SPI口初始化
//这里针是对SPI1的初始化

void SPI2_Init(void)
{

    RCC->APB2ENR|=1<<3;       //PORTB时钟使能 	 
	RCC->APB1ENR|=1<<14;      //SPI2时钟使能 
	
	//这里只针对SPI口初始化
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0XBBB00000;//PB13.14.15复用 	    
	GPIOB->ODR|=0X7<<13;    //PB13.14.15上拉
   
    SPI2->CR1|=0<<10;//全双工模式	
	SPI2->CR1|=1<<9; //软件nss管理
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; //SPI主机
	SPI2->CR1|=0<<11;//8bit数据格式	
	SPI2->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI2->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI2->CR1|=7<<3; //Fsck=Fcpu/256
	SPI2->CR1|=0<<7; //MSBfirst   
	SPI2->CR1|=1<<6; //SPI设备使能
	
	SPI2_ReadWriteByte(0xff);//启动传输(主要作用：维持MOSI为高)	


}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  
void SPI2_SetSpeed(u8 SpeedSet)
{	
	SpeedSet&=0X07;			//限制范围
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet<<3;	//设置SPI2速度  
	SPI2->CR1|=1<<6; 		//SPI设备使能 	
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
    u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据	
   
	
}

