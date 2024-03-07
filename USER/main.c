#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"

int main(void)
{
    Stm32_Clock_Init(9); 	//ϵͳʱ������
    delay_init(72);	     	//��ʱ��ʼ��
    LED_Init();		  	    //��ʼ����LED���ӵ�Ӳ���ӿ�
    BEEP_Init();            //��ʼ���������˿�

    while (1)
    {
        LED1 = 0;
        BEEP = 0;
        delay_ms(300);//��ʱ300ms
        LED1 = 1;
        BEEP = 1;
        delay_ms(300);//��ʱ300ms
    }
}

