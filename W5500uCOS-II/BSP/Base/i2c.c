/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��i2c.c
 * ����    ����ʼ��IIC��һЩ�����Ĳ��� 
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-07-14
 * Ӳ������: PB6-I2C1_SCL��PB7-I2C1_SDA
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

//ͷ�ļ�
#include "i2c.h"
#include "usart.h"

 /**
  * @file   I2C_Configuration
  * @brief  EEPROM�ܽ�����
  * @param  ��
  * @retval ��
  */
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  /* Configure I2C2 pins: PB6->SCL and PB7->SDA */
  RCC_APB2PeriphClockCmd(EEPROM_I2C_SCL_GPIO_RCC|EEPROM_I2C_SDA_GPIO_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  EEPROM_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

 /**
  * @file   I2C_delay
  * @brief  �ӳ�ʱ��
  * @param  ��
  * @retval ��
  */
static void I2C_delay(void)
{	
   uint8_t i=50;
   while(i) 
   { 
     i--; 
   } 
}

 /**
  * @file   I2C_Start
  * @brief  ��ʼ�ź�
  * @param  ��
  * @retval ��
  */
static FunctionalState I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	I2C_delay();
	if(SDA_read) return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	SDA_L;
	I2C_delay();
	return ENABLE;
}

 /**
  * @file   I2C_Stop
  * @brief  ֹͣ�ź�
  * @param  ��
  * @retval ��
  */
static void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

 /**
  * @file   I2C_Ack
  * @brief  Ӧ���ź�
  * @param  ��
  * @retval ��
  */
static void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

 /**
  * @file   I2C_NoAck
  * @brief  ��Ӧ���ź�
  * @param  ��
  * @retval ��
  */
static void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

 /**
  * @file   I2C_WaitAck
  * @brief  �ȴ�Ack
  * @param  ��
  * @retval ����Ϊ:=1��ACK,=0��ACK
  */
static FunctionalState I2C_WaitAck(void) 	
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

 /**
  * @file   I2C_SendByte
  * @brief  ���ݴӸ�λ����λ
  * @param  - SendByte: ���͵�����
  * @retval ��
  */
static void I2C_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
			SCL_L;
			I2C_delay();
			if(SendByte&0x80)
			SDA_H;  
			else 
			SDA_L;   
			SendByte<<=1;
			I2C_delay();
			SCL_H;
			I2C_delay();
    }
    SCL_L;
}


 /**
  * @file   I2C_ReceiveByte
  * @brief  ���ݴӸ�λ����λ
  * @param  ��
  * @retval I2C���߷��ص�����
  */
static uint8_t I2C_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
			ReceiveByte<<=1;      
			SCL_L;
			I2C_delay();
			SCL_H;
			I2C_delay();	
			if(SDA_read)
			{
			ReceiveByte|=0x01;
			}
    }
    SCL_L;
    return ReceiveByte;
}
   
 /**
  * @file   I2C_WriteByte
  * @brief  дһ�ֽ�����
  * @param  
	*          - SendByte: ��д������
	*          - WriteAddress: ��д���ַ
	*          - DeviceAddress: ��������(24c08)
  * @retval ����Ϊ:=1�ɹ�д��,=0ʧ��
  */
FunctionalState I2C_WriteOneByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress)
{		
    if(!I2C_Start())return DISABLE;
    I2C_SendByte((((WriteAddress & 0x0700) >>7) | DeviceAddress) & 0xFFFE); /*���ø���ʼ��ַ+������ַ */
    if(!I2C_WaitAck()){I2C_Stop(); return DISABLE;}
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();   
    I2C_Stop(); 
    return ENABLE;
}									 

 /**
  * @file   I2C_ReadBytes
  * @brief  ��ȡһ������
  * @param  
	*					- pBuffer: ��Ŷ�������
	*     	  - length: ����������
	*         - ReadAddress: ��������ַ
	*         - DeviceAddress: ��������(24c08)
  * @retval ����Ϊ:=1�ɹ�����,=0ʧ��
  */
FunctionalState I2C_ReadBytes(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress,  uint8_t DeviceAddress)
{		
    if(!I2C_Start())return DISABLE;
    I2C_SendByte((((ReadAddress & 0x0700) >>7) | DeviceAddress) & 0xFFFE); /* ���ø���ʼ��ַ+������ַ */ 
    if(!I2C_WaitAck()){I2C_Stop(); return DISABLE;}
    I2C_SendByte((uint8_t)(ReadAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress | 0x0001);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return ENABLE;
}
/**
  * @file   I2c_WriteBytes
  * @brief  д����ֽ�����
  * @param  
	*          - buff: ��д������
	*          - WriteAddr: ��д���ַ
*          - length: ��д���ݳ���
* @retval ����Ϊ
  */
void I2C_WriteBytes(uint8_t* buff, uint8_t WriteAddr, uint16_t length)
{
	uint16_t t;
	for(t=0; t<length; t++)
	{
		while( !I2C_WriteOneByte(buff[t],WriteAddr+t,ADDR_24C08));
	}
}

/**
  * @file   I2C_Test
  * @brief  I2C(AT24C08)��д���ԡ�
  * @param  ��
  * @retval ��
  */
void I2C_Test(void)
{
		uint16_t Addr;
		uint8_t WriteBuffer[256],ReadBuffer[256];
		for(Addr=0; Addr<256; Addr++)
	  {
			WriteBuffer[Addr]=255;	 /* ���WriteBuffer */
			printf("0x%02x ",WriteBuffer[Addr]);
				if(Addr%16 == 15)    
		printf("\n\r");
		
    }
		/* ��ʼ��EEPROMд���� */
		printf("\r\n EEPROM 24C08 Write Test \r\n");
		I2C_WriteBytes(WriteBuffer,0,256);
//		for(Addr=0; Addr<256; Addr++)
//		while( !I2C_WriteOneByte(WriteBuffer[Addr], Addr,  ADDR_24C08) );
		printf("\r\n EEPROM Write Test OK \r\n");

		/* EEPROM������ */
		printf("\r\n EEPROM 24C08 Read Test \r\n");
		I2C_ReadBytes(ReadBuffer, sizeof(WriteBuffer),0, ADDR_24C08);
    for(Addr=0; Addr<256; Addr++)
		{
		   printf("0x%02x ",ReadBuffer[Addr]);
				if(Addr%16 == 15)    
		printf("\n\r");
		}

		if(  memcmp(WriteBuffer,ReadBuffer,sizeof(WriteBuffer)) == 0 ) /* ƥ������ */
		printf("\r\n EEPROM 24C08 Read Test OK\r\n");
		else
		printf("\r\n EEPROM 24C08 Read Test False\r\n");
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
