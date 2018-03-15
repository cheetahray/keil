#define GLOBALS
#include "includes.h"
#include "ucos_ii.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "udp_demo.h"
#include "socket.h"

void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void Task_LED0(void *p_arg)
{
    //OSTimeDly(150);
    WizW5500_Init(IP_FROM_DEFINE);
    while(1)
    {   /*
        GPIO_SetBits(GPIOB, GPIO_Pin_0);    //on
        OSTimeDly(50);                    //half second
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);    //off
        OSTimeDly(50);*/
        loopback_udp(SOCK_UDPS, local_port);/*UDP 数据回环测试*/
    }
}
void Task_LED1(void *p_arg)
{
    while(1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);    //on
        OSTimeDly(100);    //one second
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);    //off
        OSTimeDly(100);
    }
}
/****************************************************************************
* 名    称：static  void Task_Com1(void *p_arg)
* 功    能：串口1任务
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void Task_Com1(void *p_arg) {
    u16 x1=0,x2=0;
    (void)p_arg;
    while(1) {
        if(Buf_Ok==TRUE)//BUF可用
        {
            Buf_Ok=FALSE;
            x1=0;
            x2=0;
            if(Free_Buf_No==BUF_NO1)//如果BUF1空闲
            {
                while(x1<dma_len)
                {
                    printf("%c",USART1_DMA_Buf1[x1++]);	//用串口1将BUF1中数据发送出去
                }
            }
            else //如果BUF2空闲
            {
                while(x2<dma_len)
                {
                    printf("%c",USART1_DMA_Buf2[x2++]);	//用串口1将BUF2中数据发送出去
                }
            }
        }
    }
}
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

OS_STK LED0TaskStk[LED0STKSIZE];
OS_STK LED1TaskStk[LED1STKSIZE];
OS_STK Task_Com1Stk[Task_Com1_STK_SIZE];
int main()
{
    RCC_DeInit();
    LED_GPIO_Init();    //initialize the gpio
    USART1_Config();
    OSInit();                    //initialize the os
    NVIC_Config();//中断等级配置
    USART_DMAToBuf1();//串口DMA配置
    OS_CPU_SysTickInit();    //initialze the system clock
    //create two task LED0 and LED1
    OSTaskCreate(Task_LED0, (void *)0, &LED0TaskStk[LED0STKSIZE - 1], 5);
    OSTaskCreate(Task_LED1, (void *)0, &LED1TaskStk[LED1STKSIZE - 1], 6);
    //串口1接收及发送任务---------------------------------------------------------
    OSTaskCreate(Task_Com1,(void *)0,&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO);

    OSStart();    //start the os
    return 0;
}
