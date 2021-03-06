/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：exti.c
 * 描述    ：按键引脚配置
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2015-11-11
 * 硬件连接: 无
 * 调试方式：J-Link-OB
**********************************************************************************/
//头文件  
#include "exti.h"
#include "includes.h"

//函数声明
//void Delay(u32 n);

 /**
  * @file   Exti_Config
  * @brief  调用函数库，初始化中断配置
  * @param  无
  * @retval 无
  */
void Exti_Config(void)
{	
	/*查看外部中断/事件线路映像
  PA0、PB0、PC0、PD0、PE0、PF0、PG0 ————>映像于 EXTI0
  PA1、PB1、PC1、PD1、PE1、PF1、PG1 ————>映像于 EXTI1
  PA2、PB2、PC2、PD2、PE2、PF2、PG2 ————>映像于 EXTI2
  PA3、PB3、PC3、PD3、PE3、PF3、PG3 ————>映像于 EXTI3
  .......依次类推，每个外部中断都有一个事件线路映射*/
  //定义一个EXTI_InitTypeDef 类型的结构体
  EXTI_InitTypeDef EXTI_InitStructure;
  //使能AFIO功能的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	
  //设知中断线路
	/*
  GPIO_EXTILineConfig(KEY1_BUTTON_EXTI_PORT_SOURCE, KEY1_BUTTON_EXTI_PIN_SOURCE);
  GPIO_EXTILineConfig(KEY2_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE);*/
  GPIO_EXTILineConfig(KEY3_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE);
  //选择中断线路
  EXTI_InitStructure.EXTI_Line = /*KEY1_BUTTON_EXTI_LINE | KEY2_BUTTON_EXTI_LINE | */KEY3_BUTTON_EXTI_LINE; 
  //设置为中断请求触发模式
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //设置为下降沿触发
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  //设置为使能中断线
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //调用库函数，初始化中断
  EXTI_Init(&EXTI_InitStructure);
}
/**
  * @file   nvic_config
  * @brief  调用库函数，初始化中断向量
  * @param  无
  * @retval 无
  */
void Nvic_Config_Key()
{
 
  NVIC_InitTypeDef NVIC_InitStructure; //定义一个NVIC_InitTypeDef 类型的结构体
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //选择中断分组0	
	
  NVIC_InitStructure.NVIC_IRQChannel = /*KEY1_BUTTON_EXTI_IRQn|KEY2_BUTTON_EXTI_IRQn|*/KEY3_BUTTON_EXTI_IRQn;//配置选中的中断向量,指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0; //配置抢占优先级:抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //配置响应优先级:响应式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断向量
  NVIC_Init(&NVIC_InitStructure);  //初始化NVIC
}

/**
  * @file   EXTI9_5_IRQHandler
  * @brief  中断处理函数
  * @param  无
  * @retval 无
  */
void EXTI9_5_IRQHandler(void)
{  
  OS_CPU_SR  cpu_sr;
  
	OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
  	OSIntNesting++;	  	  //中断嵌套标志
  	OS_EXIT_CRITICAL();	  //恢复全局中断标志		 		  
/*检查指定的EXTI线路出发请求发生与否*//*
  if (EXTI_GetITStatus(KEY1_BUTTON_EXTI_LINE) != RESET)
  { 
    /*清除EXTI线路挂起位*//*
    EXTI_ClearITPendingBit(KEY1_BUTTON_EXTI_LINE); 
   /*D1状态翻转*//*
   LEDXToggle(LED1);
  }
  /*检查指定的EXTI线路出发请求发生与否*//*
  if (EXTI_GetITStatus(KEY2_BUTTON_EXTI_LINE) != RESET)
  {
    /*清除EXTI线路挂起位*//*
    EXTI_ClearITPendingBit(KEY2_BUTTON_EXTI_LINE);
   /*D2状态翻转*//*
   LEDXToggle(LED2);
  }	
  /*检查指定的EXTI线路出发请求发生与否*/
  if (EXTI_GetITStatus(KEY3_BUTTON_EXTI_LINE) != RESET)
  { 
    /*清除EXTI线路挂起位*/
		unsigned char * msg;
	  int msgsize = 3;  	
		TIM_SetCounter(TIM1,0);
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);
		TIM_SetCounter(TIM2, 0);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		msgsize *= sizeof(char);
		msg = malloc(msgsize);
    memcpy(msg,"Fa",msgsize);
    OSMboxPost(Com1_MBOX,msg); 	        //将接收到的数据通过消息邮箱传递给串口1接收解析任务   
		stepray = 0;
    EXTI_ClearITPendingBit(KEY3_BUTTON_EXTI_LINE); 
		/*D3状态翻转*/
    //LEDXToggle(LED3);
  }
  OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换  	
}









