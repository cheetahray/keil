/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    �������MCUͨ��SPI2��w5500�Ķ�д��������ɶ�IP,mac��gateway�ȵĲ�����
 *						�������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ
 *            ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP���Ա�����
 *            �뽫w5500_conf.c�ļ��е�local_port����Ϊ����ʹ�õļ����˿�,Ĭ��Ϊ5000
 * ��汾  ��V3.50
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-07-14
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

//ͷ�ļ�
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
** ������: NVIC_Config
** ��������: �ж��������ȼ�������
** �������: ��
** �������: ��
***********************************************************/
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//������2

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1ͨ��5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//�ж�����ʹ��
    NVIC_Init(&NVIC_InitStructure);	//��ʼ���ṹ��
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
    NVIC_Config();//�жϵȼ�����
    USART_DMAToBuf1();//����DMA����
    printf("  Դ�������豸----UDP Demo V1.0 \r\n");
    printf(" W5500���Ժ͵��Ե�UDP�˿�ͨѶ \r\n");
    printf(" W5500�ı��ض˿�Ϊ:%d \r\n",local_port);
    printf(" Զ�˶˿�Ϊ:%d \r\n",remote_port);
    printf(" ���ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
    while (1)
    {
        if(Buf_Ok==TRUE)//BUF����
        {
            Buf_Ok=FALSE;
            x1=0;
            x2=0;
            if(Free_Buf_No==BUF_NO1)//���BUF1����
            {
                while(x1<dma_len)
                {
                    //USART1_Putc(USART1_DMA_Buf1[x1++]);	//�ô���1��BUF1�����ݷ��ͳ�ȥ
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
            else //���BUF2����
            {
                while(x2<dma_len)
                {
                    //USART1_Putc(USART1_DMA_Buf2[x2++]);	//�ô���1��BUF2�����ݷ��ͳ�ȥ
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
            loopback_udp(SOCK_UDPS, local_port);/*UDP ���ݻػ�����*/
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





