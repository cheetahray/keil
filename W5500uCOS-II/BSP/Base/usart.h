#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

#define BaudRate	 115200

#define USART              	 USART1
#define USART_RCC         	 RCC_APB2Periph_USART1

#define USART_GPIO_RCC    		RCC_APB2Periph_GPIOA
#define USART_TX		        	GPIO_Pin_9	// out
#define USART_RX		        	GPIO_Pin_10	// in 
#define USART_GPIO_PORT    		GPIOA   

typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define dma_len 512  //定义串口DMA传输数据长度
extern u8 USART1_DMA_Buf1[dma_len]; //BUF1
extern u8 USART1_DMA_Buf2[dma_len];	//BUF2
typedef enum {BUF_NO1=0,BUF_NO2=1}BUF_NO;
extern BUF_NO Free_Buf_No;
extern bool Buf_Ok;

void USART_DMAToBuf1(void);
void USART1_Config(void);
#endif
