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
    
    KEY_GPIO_Config();//������������
}
void Task_Network(void *p_arg)
{
    u8 lastch = 127,ch, lastlight, watt, lastchangle, changle;
    OSTimeDly(150);
    WizW5500_Init(IP_FROM_DEFINE);
    stepray = 0;
    while(1)
    {   
        OSTimeDly(5);
        loopback_artnet(SOCK_UDPS, remote_port, &lastch, &ch, &lastlight, &watt, &lastchangle, &changle);/*UDP ���ݻػ�����*/   
    }
}
void Task_Key(void *p_arg)
{
    INT8U err;
    unsigned char * msg;
    while(1)
    {
        msg=(unsigned char *)OSMboxPend(Com1_MBOX,0,&err); 		  //�ȴ����ڽ���ָ�����Ϣ���� 
        if(msg)
				{
					  if(memcmp(msg,"Fa",3) == 0)
						{
								GPIO_SetBits(GPIOB, GPIO_Pin_0);    //on
								OSTimeDly(5);                    //half second
								GPIO_ResetBits(GPIOB, GPIO_Pin_0);    //off
								OSTimeDly(5);
                GPIO_SetBits(GPIOB, GPIO_Pin_1);    //on
								OSTimeDly(5);    //one second
								GPIO_ResetBits(GPIOB, GPIO_Pin_1);    //off
								OSTimeDly(5);
            }		    
            free(msg);
				}
    }
}
void Task_CWCCW(void *p_arg)
{
    INT8U err, V0, a2, t3, V, changle;
    unsigned char * msg;
	  
    while(1)
    {
        msg=(unsigned char *)OSMboxPend(CWCCW_MBOX,0,&err); 		  //�ȴ����ڽ���ָ�����Ϣ���� 
			  if(msg)
				{
					V = *msg;
					V0 = *(msg+1);
					a2 = *(msg+2);
					t3 = *(msg+3);
					changle = *(msg+4);
					if( V < V0 ) 
					{
						 if(V0 - V < a2 * t3)
								 V0 = V;
						 else
								 V0 = V0 - a2 * t3;
					}
					else if(V > V0)
					{
						 if(V - V0 < a2 * t3)
								 V0 = V;
						 else
								 V0 = V0 + a2 * t3;
					}
					CWCCW(V0);				
        	if( V != V0 )
					{
						*(msg+1) = V0;
						OSMboxPost(CWCCW_MBOX, msg); 	        //�����յ�������ͨ����Ϣ���䴫�ݸ�����1���ս�������   
					}
					else
				  {
					  hereray = angle2step(changle);
					}
					OSTimeDly(t3 << 3);                    //half second
				}
				//OSTimeDly(1);                    //half second
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
				OSTimeDly(1);
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//������2

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1ͨ��5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ���Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//�ж�����ʹ��
    NVIC_Init(&NVIC_InitStructure);	//��ʼ���ṹ��
	  /*
	  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;//TIM3�ж�
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ1
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�����Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	  NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
    */
    NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;//TIM3�ж�
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//ռ��ʽ���ȼ�����Ϊ1
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�����Ϊ0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ж�ʹ��
	  NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	  
		Exti_Config();//��ʼ���ж�����	
    Nvic_Config_Key();//��ʼ���ж�����
  
}

OS_STK NetworkTaskStk[NetworkSTKSIZE];
OS_STK KeyTaskStk[KeySTKSIZE];
OS_STK Task_Com1Stk[Task_Com1_STK_SIZE];
OS_STK Task_CWCCWStk[Task_CWCCW_STK_SIZE];
int main()
{
    OSInit();                    //initialize the os
    RCC_DeInit();
    NVIC_Config();//�жϵȼ�����
    LED_GPIO_Init();    //initialize the gpio
    USART1_Config();
    USART_DMAToBuf1();//����DMA����
    //TIM2_PWM_Init();//��ʼ��ͨ�ö�ʱ��TIM2
    TIM2_Config();
    TIM3_Config();
    OS_CPU_SysTickInit();    //initialze the system clock
    Com1_MBOX=OSMboxCreate((void *) 0);		 //��������1�жϵ�����
	  CWCCW_MBOX=OSMboxCreate((void *) 0);		 //��������1�жϵ�����
    //create two task LED0 and LED1
    OSTaskCreate(Task_Network, (void *)0, &NetworkTaskStk[NetworkSTKSIZE - 1], Task_Network_PRIO);
    OSTaskCreate(Task_Key, (void *)0, &KeyTaskStk[KeySTKSIZE - 1], Task_Key_PRIO);
    //����1���ռ���������---------------------------------------------------------
    OSTaskCreate(Task_Com1,(void *)0,&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO);
    OSTaskCreate(Task_CWCCW,(void *)0,&Task_CWCCWStk[Task_CWCCW_STK_SIZE-1],Task_CWCCW_PRIO);
    OSStart();    //start the os
    return 0;
}
