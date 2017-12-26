/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��usart.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ�������ݴ�
 *           ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-11-11
 * Ӳ������: TX->PA9;RX->PA10
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "usart.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


 /**
  * @file   USART1_Config
  * @brief  USART1 GPIO ����,����ģʽ���á�9600-8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    USART_ClockInitTypeDef USART_ClockInitStructure;
    RCC_APB2PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX;	       //ѡ�д���Ĭ������ܽ�         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
    /*����ͨѶ��������*/
    USART_InitStructure.USART_BaudRate = BaudRate; //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;//����ʱ��Ϊ�͵�ƽ
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;//ʱ�ӵڶ������ؽ������ݲ���
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;//���һλ���ݵ�ʱ�����岻��SCLK���

    USART_ClockInit(USART1, &USART_ClockInitStructure);
    USART_Init(USART, &USART_InitStructure);
    USART_ClearFlag(USART,USART_FLAG_TC);
    //USART_ITConfig(USART, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART, USART_IT_TXE, ENABLE);		
    USART_Cmd(USART, ENABLE);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART, USART_FLAG_TC) == RESET)
  {}
  return ch;
}

/************DMA��ʽ����***************************/
#define SRC_USART1_DR	(&(USART1->DR))		//���ڽ��ռĴ�����ΪԴͷ

//DMAĿ�껺��,����ʹ��˫����
u8 USART1_DMA_Buf1[dma_len] = "ggy";
u8 USART1_DMA_Buf2[dma_len] = "7788";
bool Buf_Ok;	//BUF�Ƿ��Ѿ�����
BUF_NO Free_Buf_No;	//���е�BUF��

DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��
/**********************************************************
** ������:void USART_DMAToBuf1(void)
** ��������: ����DMA��ʼ������
** �������: ��
** �������: �� 
***********************************************************/
void USART_DMAToBuf1(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SRC_USART1_DR;//ԴͷBUF
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_Buf1;//Ŀ��BUF
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			//������Դͷ
	DMA_InitStructure.DMA_BufferSize = dma_len;				    //BUF��С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�����ֽ�Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//�ڴ��ֽ�Ϊ��λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//4���ȼ�֮һ��(������)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);	//DMA5��������ж�
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//ʹ�ܴ��ڽ���DMA	
	//��ʼ��BUF��־
	Free_Buf_No=BUF_NO2;
	Buf_Ok=FALSE;	
	DMA_Cmd(DMA1_Channel5, ENABLE);		// ʹ��DMA
}
