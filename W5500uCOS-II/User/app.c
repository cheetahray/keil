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
    
    KEY_GPIO_Config();//按键引脚配置
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
        loopback_artnet(SOCK_UDPS, remote_port, &lastch, &ch, &lastlight, &watt, &lastchangle, &changle);/*UDP 数据回环测试*/   
    }
}
void Task_Key(void *p_arg)
{
    INT8U err;
    unsigned char * msg;
    while(1)
    {
        msg=(unsigned char *)OSMboxPend(Com1_MBOX,0,&err); 		  //等待串口接收指令的消息邮箱 
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
        msg=(unsigned char *)OSMboxPend(CWCCW_MBOX,0,&err); 		  //等待串口接收指令的消息邮箱 
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
						OSMboxPost(CWCCW_MBOX, msg); 	        //将接收到的数据通过消息邮箱传递给串口1接收解析任务   
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
				OSTimeDly(1);
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//采用组2

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//DMA1通道5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占式优先级设为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//副优先级设为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//中断向量使能
    NVIC_Init(&NVIC_InitStructure);	//初始化结构体
	  /*
	  NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;//TIM3中断
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为1
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //副优先级设置为0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	  NVIC_Init(&NVIC_InitStructure);//中断初始化
    */
    NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;//TIM3中断
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为1
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //副优先级设置为0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	  NVIC_Init(&NVIC_InitStructure);//中断初始化
	  
		Exti_Config();//初始化中断配置	
    Nvic_Config_Key();//初始化中断向量
  
}

OS_STK NetworkTaskStk[NetworkSTKSIZE];
OS_STK KeyTaskStk[KeySTKSIZE];
OS_STK Task_Com1Stk[Task_Com1_STK_SIZE];
OS_STK Task_CWCCWStk[Task_CWCCW_STK_SIZE];
int main()
{
    OSInit();                    //initialize the os
    RCC_DeInit();
    NVIC_Config();//中断等级配置
    LED_GPIO_Init();    //initialize the gpio
    USART1_Config();
    USART_DMAToBuf1();//串口DMA配置
    //TIM2_PWM_Init();//初始化通用定时器TIM2
    TIM2_Config();
    TIM3_Config();
    OS_CPU_SysTickInit();    //initialze the system clock
    Com1_MBOX=OSMboxCreate((void *) 0);		 //建立串口1中断的邮箱
	  CWCCW_MBOX=OSMboxCreate((void *) 0);		 //建立串口1中断的邮箱
    //create two task LED0 and LED1
    OSTaskCreate(Task_Network, (void *)0, &NetworkTaskStk[NetworkSTKSIZE - 1], Task_Network_PRIO);
    OSTaskCreate(Task_Key, (void *)0, &KeyTaskStk[KeySTKSIZE - 1], Task_Key_PRIO);
    //串口1接收及发送任务---------------------------------------------------------
    OSTaskCreate(Task_Com1,(void *)0,&Task_Com1Stk[Task_Com1_STK_SIZE-1],Task_Com1_PRIO);
    OSTaskCreate(Task_CWCCW,(void *)0,&Task_CWCCWStk[Task_CWCCW_STK_SIZE-1],Task_CWCCW_PRIO);
    OSStart();    //start the os
    return 0;
}
