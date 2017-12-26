/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：usart.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据打
 *           印到PC上的超级终端或串口调试助手。     
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2015-11-11
 * 硬件连接: TX->PA9;RX->PA10
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
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
  * @brief  USART1 GPIO 配置,工作模式配置。9600-8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    USART_ClockInitTypeDef USART_ClockInitStructure;
    RCC_APB2PeriphClockCmd( USART_RCC | USART_GPIO_RCC,ENABLE);

    /*USART1_TX ->PA9*/			
    GPIO_InitStructure.GPIO_Pin = USART_TX;	       //选中串口默认输出管脚         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
    /*USART1_RX ->PA10*/
    GPIO_InitStructure.GPIO_Pin = USART_RX;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
    /*串口通讯参数设置*/
    USART_InitStructure.USART_BaudRate = BaudRate; //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;//空闲时钟为低电平
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;//时钟第二个边沿进行数据捕获
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;//最后一位数据的时钟脉冲不从SCLK输出

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

/************DMA方式传输***************************/
#define SRC_USART1_DR	(&(USART1->DR))		//串口接收寄存器作为源头

//DMA目标缓冲,这里使用双缓冲
u8 USART1_DMA_Buf1[dma_len] = "ggy";
u8 USART1_DMA_Buf2[dma_len] = "7788";
bool Buf_Ok;	//BUF是否已经可用
BUF_NO Free_Buf_No;	//空闲的BUF号

DMA_InitTypeDef DMA_InitStructure;//定义DMA结构体
/**********************************************************
** 函数名:void USART_DMAToBuf1(void)
** 功能描述: 串口DMA初始化配置
** 输入参数: 无
** 输出参数: 无 
***********************************************************/
void USART_DMAToBuf1(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SRC_USART1_DR;//源头BUF
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_Buf1;//目标BUF
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;			//外设作源头
	DMA_InitStructure.DMA_BufferSize = dma_len;				    //BUF大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设字节为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//内存字节为单位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//4优先级之一的(高优先)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//非内存到内存
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);	//DMA5传输完成中断
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//使能串口接收DMA	
	//初始化BUF标志
	Free_Buf_No=BUF_NO2;
	Buf_Ok=FALSE;	
	DMA_Cmd(DMA1_Channel5, ENABLE);		// 使能DMA
}
