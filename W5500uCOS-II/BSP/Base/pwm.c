
/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：PWM.c
 * 描述    ：初始化TIM2四通道PWM端口 
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2015-10-20
 * 硬件连接: beep->PA1
 * 调试方式：J-Link-OB
**********************************************************************************/	

//头文件
#include "PWM.h"
#include  <ucos_ii.h>
 /**
  * @file   TIM2_PWM_Init
  * @brief  初始化TIM2通道的PWM端口
  * @param  无
  * @retval 无
  */
void TIM1_Config()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 65535-1;   
    TIM_TimeBaseStructure.TIM_Prescaler = 1-1;    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);  

    TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);
    //TIM_InternalClockConfig(TIM3);
    TIM1->SMCR|=0x07;                                  //设置从模式寄存器 
    //TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR0);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);         
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位
 
    TIM_ITConfig(      //使能或者失能指定的TIM中断
        TIM1,            //TIM1
        TIM_IT_Update  | //TIM 更新中断源
        TIM_IT_Trigger,  //TIM 触发中断源 
        ENABLE  	     //使能
    );
	
    //设置优先级
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  	   //从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
		TIM_Cmd(TIM1, ENABLE);
}

/**********************************************************
** 函数名: TIM2_Config
** 功能描述:  定时器2配置为输出比较模式
** 输入参数: 无
** 输出参数: 无
** 说明：
		溢出时间t=36000*8000/72000000=4s
***********************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟和复用功能时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //TIM2二通道PWM波形输出端口PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CWCCW(200);
}

/**********************************************************
** 函数名: TIM3_Config
** 功能描述:  定时器2配置为输出比较模式
** 输入参数: 无
** 输出参数: 无
** 说明：
		溢出时间t=36000*8000/72000000=4s
***********************************************************/
void TIM3_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟和复用功能时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM3二通道PWM波形输出端口PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*预先清除所有中断位*//*
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update); 

	/* 4个通道和溢出都配置中断*//*
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
	*/
}

void CWCCW(u8 ch)
{
	 OS_CPU_SR  cpu_sr;
	 OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
	 OSIntNesting++;	  	  //中断嵌套标志
	 TIM_Cmd(TIM2,DISABLE);		
   if(ch != 127)
   {		 
		 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	   TIM_OCInitTypeDef  TIM_OCInitStructure;
	   /* 基础设置*/
	   TIM_TimeBaseStructure.TIM_Prescaler = 1-1; //此值+1为分频的除数，一次数0.5ms
	   if(ch > 127)
		    TIM_TimeBaseStructure.TIM_Period = 4096/(ch-126)-1;	//计数值   
		 else
			  TIM_TimeBaseStructure.TIM_Period = 4096/(128-ch)-1;	//计数值   
		 
	   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //采样分频
	   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
     //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   	 
	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出比较非主动模式
  	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性为正
		 TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period >> 1);//通道1捕获比较值
						
		 if(ch > 127)
		 {
				/* 比较通道1配置*/
			  TIM_OC3Init(TIM2, &TIM_OCInitStructure);//根据TIM_OCInitStruct中指定的参数初始化TIM2
			  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//禁止OC1重装载,其实可以省掉这句,因为默认是4路都不重装的.
			  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);//禁止OC1重装载,其实可以省掉这句,因为默认是4路都不重装的.
		 }
		 else
		 {
				/*比较通道2 */        
			  TIM_OC4Init(TIM2, &TIM_OCInitStructure);//用指定的参数初始化TIM2 
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
			  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);//禁止OC1重装载,其实可以省掉这句,因为默认是4路都不重装的.
	   }
		 
		 TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
     TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
				
		 /*使能预装载*/
	   TIM_ARRPreloadConfig(TIM2, ENABLE);	
	   	
	   TIM_Cmd(TIM2, ENABLE); //使能定时器2
	 }	
   OS_EXIT_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR	 
	 OSIntExit();
}

void LIGHT(u8 ch)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;	  	  //中断嵌套标志
	TIM_Cmd(TIM3, DISABLE); //使能定时器2
	/* 基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 256-1;	//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 2-1; //此值+1为分频的除数，一次数0.5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性为正
	/* 比较通道1配置*/
	TIM_OCInitStructure.TIM_Pulse = ch; //*100 + 50;//通道1捕获比较值
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//根据TIM_OCInitStruct中指定的参数初始化TIM3
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//禁止OC1重装载,其实可以省掉这句,因为默认是4路都不重装的.
	
	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);	
	
	TIM_Cmd(TIM3, ENABLE); //使能定时器2
	OS_EXIT_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
	OSIntExit();
}
