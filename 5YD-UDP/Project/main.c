/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    ：完成是MCU通过SPI2对w5500的读写操作，完成对IP,mac，gateway等的操作，
 *						内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
 *            如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP测试本程序，
 *            请将w5500_conf.c文件中的local_port定义为测试使用的监听端口,默认为5000
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-07-14
 * 调试方式：J-Link-OB
**********************************************************************************/

//头文件
#include "stm32f10x.h"
#include "led.h"
#include "SYSTICK.h"
#include "usart.h"
#include "i2c.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include <stdio.h>
#include "tcp_demo.h"
#include "udp_demo.h"
#include "usart.h"
#include "radarsensor.h"
/**********************************************************
** 函数名: NVIC_Config
** 功能描述: 中断向量优先级等配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//采用组2

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1通道5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占式优先级设为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//副优先级设为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//中断向量使能
    NVIC_Init(&NVIC_InitStructure);	//初始化结构体
}
/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    u16 x1=0,x2=0, dist = 0;
    SYSTICK_Init();
    USART1_Config();
    LED_GPIO_Config();
    //I2C_Configuration();
    delay_ms(1500);
    WizW5500_Init(IP_FROM_DEFINE);
    NVIC_Config();//中断等级配置
    USART_DMAToBuf1();//串口DMA配置
    printf("  源地仪器设备----UDP Demo V1.0 \r\n");
    printf(" W5500可以和电脑的UDP端口通讯 \r\n");
    printf(" W5500的本地端口为:%d \r\n",local_port);
    printf(" 远端端口为:%d \r\n",remote_port);
    printf(" 连接成功后，PC机发送数据给W5500，W5500将返回对应数据 \r\n");
    while (1)
    {
        if(Buf_Ok==TRUE)//BUF可用
        {
            Buf_Ok=FALSE;
            x1=0;
            x2=0;
            if(Free_Buf_No==BUF_NO1)//如果BUF1空闲
            {
                while(x1<dma_len)
                {
                    //USART1_Putc(USART1_DMA_Buf1[x1++]);	//用串口1将BUF1中数据发送出去
                    if( (0 == (USART1_DMA_Buf1[x1]&0x80)) && (0 == (USART1_DMA_Buf1[x1+1]&0x80)) && (128 == (USART1_DMA_Buf1[x1+2]&0x80)) )
                    {
                        //printf("%c%c%c", USART1_DMA_Buf1[x1], USART1_DMA_Buf1[x1+1], USART1_DMA_Buf1[x1+2]);
                        //printf("%d ", DecodeLaseData3Byte(USART1_DMA_Buf1+x1));
                        dist = DecodeLaseData3Byte(USART1_DMA_Buf1+x1);
                        if ( dist > 0 && dist < 170)
                            dma_udp(SOCK_UDPS, remote_port, dist);
                        x1+=3;
                    }
                    else
                        x1++;
                }
            }
            else //如果BUF2空闲
            {
                while(x2<dma_len)
                {
                    //USART1_Putc(USART1_DMA_Buf2[x2++]);	//用串口1将BUF2中数据发送出去
                    if( (0 == (USART1_DMA_Buf2[x2]&0x80)) && (0 == (USART1_DMA_Buf2[x2+1]&0x80)) && (128 == (USART1_DMA_Buf2[x2+2]&0x80)) )
                    {
                        //printf("%c%c%c", USART1_DMA_Buf2[x2], USART1_DMA_Buf2[x2+1], USART1_DMA_Buf2[x2+2]);
                        //printf("%d ", DecodeLaseData3Byte(USART1_DMA_Buf2+x2));
                        dist = DecodeLaseData3Byte(USART1_DMA_Buf2+x2);
                        if ( dist > 0 && dist < 170)
                            dma_udp(SOCK_UDPS, remote_port, dist);
                        x2+=3;
                    }
                    else
                        x2++;
                }
            }
        }
        else
            loopback_udp(SOCK_UDPS, local_port);/*UDP 数据回环测试*/
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





