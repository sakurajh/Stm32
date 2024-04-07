#include "norflash.h"
#include "spi.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//W25QX��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/3/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

u16 NORFLASH_TYPE = NM25Q16; //Ĭ�Ͼ���NM25Q16
//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector

//��ʼ��SPI FLASH��IO��
void Norflash_Init(void)
{
    RCC->APB2ENR |= 1 << 3;       	//PORTBʱ��ʹ��
    GPIOB->CRH &= 0XFFF0FFFF;
    GPIOB->CRH |= 0X00030000;		//PB12����
    GPIOB->ODR |= 0X1 << 12;    	//PB12����
    SPI2_Init();		   		//��ʼ��SPI2
    SPI2_SetSpeed(SPI_SPEED_4);	//����Ϊ18Mʱ��,����ģʽ
    NORFLASH_TYPE = Norflash_ReadID(); //��ȡFLASH ID.
}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 Norflash_ReadSR(void)
{
    u8 byte = 0;
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte = SPI2_ReadWriteByte(0Xff);           //��ȡһ���ֽ�
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    return byte;
}
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void Norflash_Write_SR(u8 sr)
{
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_WriteStatusReg);   //����дȡ״̬�Ĵ�������
    SPI2_ReadWriteByte(sr);               //д��һ���ֽ�
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
}
//SPI_FLASHдʹ��
//��WEL��λ
void Norflash_Write_Enable(void)
{
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_WriteEnable);      //����дʹ��
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
}
//SPI_FLASHд��ֹ
//��WEL����
void Norflash_Write_Disable(void)
{
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_WriteDisable);     //����д��ָֹ��
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
}
//��ȡоƬID W25X16��ID:0XEF14
u16 Norflash_ReadID(void)
{
    u16 Temp = 0;
    NORFLASH_CS = 0;
    SPI2_ReadWriteByte(0x90);//���Ͷ�ȡID����
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    Temp |= SPI2_ReadWriteByte(0xFF) << 8;
    Temp |= SPI2_ReadWriteByte(0xFF);
    NORFLASH_CS = 1;
    return Temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void Norflash_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    u16 i;
    NORFLASH_CS = 0;                             //ʹ������
    SPI2_ReadWriteByte(FLASH_ReadData);         //���Ͷ�ȡ����
    SPI2_ReadWriteByte((u8)((ReadAddr) >> 16)); //����24bit��ַ
    SPI2_ReadWriteByte((u8)((ReadAddr) >> 8));
    SPI2_ReadWriteByte((u8)ReadAddr);

    for (i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI2_ReadWriteByte(0XFF); //ѭ������
    }

    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void Norflash_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 i;
    Norflash_Write_Enable();                  //SET WEL
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_PageProgram);      //����дҳ����
    SPI2_ReadWriteByte((u8)((WriteAddr) >> 16)); //����24bit��ַ
    SPI2_ReadWriteByte((u8)((WriteAddr) >> 8));
    SPI2_ReadWriteByte((u8)WriteAddr);

    for (i = 0; i < NumByteToWrite; i++)SPI2_ReadWriteByte(pBuffer[i]); //ѭ��д��

    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    Norflash_Wait_Busy();					   //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void Norflash_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���

    if (NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //������256���ֽ�

    while (1)
    {
        Norflash_Write_Page(pBuffer, WriteAddr, pageremain);

        if (NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���

            if (NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite; 	 //����256���ֽ���
        }
    };
}
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
u8 _25QXX_BUFFER[4096];
void Norflash_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;

    secpos = WriteAddr / 4096; //������ַ 0~511 for w25x16
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С

    if (NumByteToWrite <= secremain)secremain = NumByteToWrite; //������4096���ֽ�

    while (1)
    {
        Norflash_Read(_25QXX_BUFFER, secpos * 4096, 4096); //������������������

        for (i = 0; i < secremain; i++) //У������
        {
            if (_25QXX_BUFFER[secoff + i] != 0XFF)break; //��Ҫ����
        }

        if (i < secremain) //��Ҫ����
        {
            Norflash_Erase_Sector(secpos);//�����������

            for (i = 0; i < secremain; i++)	 //����
            {
                _25QXX_BUFFER[i + secoff] = pBuffer[i];
            }

            Norflash_Write_NoCheck(_25QXX_BUFFER, secpos * 4096, 4096); //д����������

        }
        else Norflash_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.

        if (NumByteToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;				//�ֽ����ݼ�

            if (NumByteToWrite > 4096)secremain = 4096;	//��һ����������д����
            else secremain = NumByteToWrite;			//��һ����������д����
        }
    };
}
//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//�ȴ�ʱ�䳬��...
void Norflash_Erase_Chip(void)
{
    Norflash_Write_Enable();                  //SET WEL
    Norflash_Wait_Busy();
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_ChipErase);        //����Ƭ��������
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    Norflash_Wait_Busy();   				   //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void Norflash_Erase_Sector(u32 Dst_Addr)
{
    Dst_Addr *= 4096;
    Norflash_Write_Enable();                  //SET WEL
    Norflash_Wait_Busy();
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_SectorErase);      //������������ָ��
    SPI2_ReadWriteByte((u8)((Dst_Addr) >> 16)); //����24bit��ַ
    SPI2_ReadWriteByte((u8)((Dst_Addr) >> 8));
    SPI2_ReadWriteByte((u8)Dst_Addr);
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    Norflash_Wait_Busy();   				   //�ȴ��������
}
//�ȴ�����
void Norflash_Wait_Busy(void)
{
    while ((Norflash_ReadSR() & 0x01) == 0x01); // �ȴ�BUSYλ���
}
//�������ģʽ
void Norflash_PowerDown(void)
{
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_PowerDown);        //���͵�������
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    delay_us(3);                               //�ȴ�TPD
}
//����
void Norflash_WAKEUP(void)
{
    NORFLASH_CS = 0;                          //ʹ������
    SPI2_ReadWriteByte(FLASH_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    NORFLASH_CS = 1;                          //ȡ��Ƭѡ
    delay_us(3);                               //�ȴ�TRES1
}


























