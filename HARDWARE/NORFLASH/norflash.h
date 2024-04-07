#ifndef __NORFLASH_H
#define __NORFLASH_H
#include "sys.h"
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

//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 	    0XEF13          /* W25Q80   оƬID */
#define W25Q16 	    0XEF14          /* W25Q16   оƬID */
#define W25Q32 	    0XEF15          /* W25Q32   оƬID */
#define W25Q64 	    0XEF16          /* W25Q64   оƬID */
#define W25Q128	    0XEF17          /* W25Q128  оƬID */
#define NM25Q16     0X6814          /* NM25Q16  оƬID */
#define NM25Q64     0X5216          /* NM25Q64  оƬID */
#define NM25Q128    0X5217          /* NM25Q128 оƬID */
#define BY25Q64     0X6816          /* BY25Q64  оƬID */
#define BY25Q128    0X6817          /* BY25Q128 оƬID */


extern u16 NORFLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�

#define	NORFLASH_CS PBout(12)  //NORFLASH Ƭѡ�ź�
////////////////////////////////////////////////////////////////////////////

extern u8 _25QXX_BUFFER[4096];

//ָ���
#define FLASH_WriteEnable       0x06
#define FLASH_WriteDisable      0x04
#define FLASH_ReadStatusReg     0x05
#define FLASH_WriteStatusReg    0x01
#define FLASH_ReadData          0x03
#define FLASH_FastReadData      0x0B
#define FLASH_FastReadDual      0x3B
#define FLASH_PageProgram       0x02
#define FLASH_BlockErase        0xD8
#define FLASH_SectorErase       0x20
#define FLASH_ChipErase         0xC7
#define FLASH_PowerDown         0xB9
#define FLASH_ReleasePowerDown  0xAB
#define FLASH_DeviceID          0xAB
#define FLASH_ManufactDeviceID  0x90
#define FLASH_JedecDeviceID     0x9F

void Norflash_Init(void);
u16  Norflash_ReadID(void);  	      //��ȡFLASH ID
u8	 Norflash_ReadSR(void);         //��ȡ״̬�Ĵ���
void Norflash_Write_SR(u8 sr);  	  //д״̬�Ĵ���
void Norflash_Write_Enable(void);   //дʹ��
void Norflash_Write_Disable(void);  //д����
void Norflash_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead); //��ȡflash
void Norflash_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite); //д��flash
void Norflash_Erase_Chip(void);    	   //��Ƭ����
void Norflash_Erase_Sector(u32 Dst_Addr);//��������
void Norflash_Wait_Busy(void);           //�ȴ�����
void Norflash_PowerDown(void);           //�������ģʽ
void Norflash_WAKEUP(void);			   //����
#endif
















