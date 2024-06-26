#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "stmflash.h"
#include "iap.h"
/************************************************
 ALIENTEK NANO STM32开发板实验25
 串口IAP实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

//FLASH分区: 0X0800 0000~0x0800 2800,给IAP使用,共10K字节,FLASH APP还有128-10=118KB可用.
// SRAM分区: 0X2000 1000处开始,用于存放SRAM IAP代码,共16K字节可用,用户可以自行对16K空间进行ROM和RAM区的分配
//           特别注意，SRAM APP的ROM占用量不能大于10K字节，因为本例程最大是一次接收10K字节，不能超过这个限制。

int main(void)
{
    u8 t;
    u8 key;
    u16 oldcount = 0;	//老的串口接收数据值
    u16 applenth = 0;	//接收到的app代码长度
    Stm32_Clock_Init(9);	//系统时钟设置
    uart_init(72, 256000);	//串口初始化为256000
    delay_init(72);	   	 	//延时初始化
    LED_Init();		  		//初始化与LED连接的硬件接口
    KEY_Init();				//按键初始化
    printf("NANO STM32\r\n");
    printf("IAP TEST\r\n");
    printf("WK_UP:Copy APP2FLASH\r\n");
    printf("KEY0:Run SRAM APP\r\n");
    printf("KEY1:Run FLASH APP\r\n");

    while (1)
    {
        if (USART_RX_CNT)
        {
            if (oldcount == USART_RX_CNT) //新周期内,没有收到任何数据,认为本次数据接收完成.
            {
                applenth = USART_RX_CNT;
                oldcount = 0;
                USART_RX_CNT = 0;
                printf("用户程序接收完成!\r\n");
                printf("代码长度:%dBytes\r\n", applenth);
            }
            else oldcount = USART_RX_CNT;
        }

        t++;
        delay_ms(10);

        if (t == 30)
        {
            LED0 = !LED0; //LED0闪烁表示程序在运行
            t = 0;
        }

        key = KEY_Scan(0);

        if (key == WKUP_PRES) //WK_UP按键按下
        {
            if (applenth)
            {
                printf("开始更新固件...\r\n");

                if (((*(vu32 *)(0X20001000 + 4)) & 0xFF000000) == 0x08000000) //判断是否为0X08XXXXXX.
                {
                    iap_write_appbin(FLASH_APP1_ADDR, USART_RX_BUF, applenth); //更新FLASH代码
                    printf("固件更新完成!\r\n");
                }
                else
                {
                    printf("非FLASH应用程序!\r\n");
                }
            }
            else
            {
                printf("没有可以更新的固件!\r\n");
            }
        }

        if (key == KEY1_PRES) //KEY1按键按下
        {
            printf("开始执行FLASH用户代码!!\r\n");

            if (((*(vu32 *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) //判断是否为0X08XXXXXX.
            {
                iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
            }
            else
            {
                printf("非FLASH应用程序,无法执行!\r\n");
                printf("\r\n");
            }
        }

        if (key == KEY0_PRES) //KEY0按键按下
        {
            printf("开始执行SRAM用户代码!!\r\n");

            if (((*(vu32 *)(0X20001000 + 4)) & 0xFF000000) == 0x20000000) //判断是否为0X20XXXXXX.
            {
                iap_load_app(0X20001000);//SRAM地址
            }
            else
            {
                printf("非SRAM应用程序,无法执行!\r\n");
                printf("\r\n");
            }
        }

    }
}












