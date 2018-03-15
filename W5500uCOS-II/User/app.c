#define GLOBALS
#include "includes.h"
#include "ucos_ii.h"

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
    while(1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);    //on
        OSTimeDly(50);                    //half second
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);    //off
        OSTimeDly(50);
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
* ��    �ƣ�static  void Task_Com1(void *p_arg)
* ��    �ܣ�����1����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�������
****************************************************************************/
void Task_Com1(void *p_arg) {
    u16 x1=0,x2=0;
    (void)p_arg;
    while(1) {
        if(Buf_Ok==TRUE)//BUF����
        {
            Buf_Ok=FALSE;
            x1=0;
            x2=0;
            if(Free_Buf_No==BUF_NO1)//���BUF1����
            {
                while(x1<dma_len)
                {
                    printf("%c",USART1_DMA_Buf1[x1++]);	//�ô���1��BUF1�����ݷ��ͳ�ȥ
                }
            }
            else //���BUF2����
            {
                while(x2<dma_len)
                {
                    printf("%c",USART1_DMA_Buf2[x2++]);	//�ô���1��BUF2�����ݷ��ͳ�ȥ
                }
            }
        }
    }
}
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

OS_STK LED0TaskStk[LED0STKSIZE];
OS_STK LED1TaskStk[LED1STKSIZE];
OS_STK Task_Com1Stk[Task_Com1_STK_SIZE];
int main()
{
    RCC_DeInit();
    LED_GPIO_Init();    //initialize the gpio
    USART1_Config();
    OSInit();                    //initialize the os
    NVIC_Config();//�жϵȼ�����
    USART_DMAToBuf1();//����DMA����
    OS_CPU_SysTickInit();    //initialze the system clock
    //create two task LED0 and LED1
    OSTaskCreate(Task_LED0, (void *)0, &LED0TaskStk[LED0STKSIZE - 1], 5);
    OSTaskCreate(Task_LED1, (void *)0, &LED1TaskStk[LED1STKSIZE - 1], 6);
    //����1���ռ���������---------------------------------------------------------
    OSTaskCreate(Task_Com1,(void *)0,&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO);

    OSStart();    //start the os
    return 0;
}