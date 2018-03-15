
/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��SPI.c
 * ����    ��SPI��ʼ��  
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-08-20
 * Ӳ������: SCLK--PA5;MISO---PA6;MOSI--PA7;SCS---PA4
 * ���Է�ʽ��J-Link-OB
******************************************************************************/
#include "SPIx.h"

 /**
  * @file   WIZ_SPI_Init
  * @brief  SPI1��ʼ��  
  * @param  ��
  * @retval ��
  */
void WIZ_SPI_Init(void)
{
	SPI_InitTypeDef   SPI_InitStructure;

	RCC_APB1PeriphClockCmd(WIZ_SPI_RCC , ENABLE);
  /* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  /* Enable SPI */
  SPI_Cmd(WIZ_SPI, ENABLE);
}


 /**
  * @file   SPIX_SendByte
  * @brief  ͨ������SPIx����һ������
  * @param  uint8_t byte
  * @retval ͨ������SPIx����һ������
  */
uint8_t SPIX_SendByte(uint8_t byte)
{
		
	while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	  

	SPI_I2S_SendData(WIZ_SPI, byte); //ͨ������SPIx����һ������
	while (SPI_I2S_GetFlagStatus(WIZ_SPI, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
							
	return SPI_I2S_ReceiveData(WIZ_SPI); //����ͨ��SPIx������յ�����	
	
}


