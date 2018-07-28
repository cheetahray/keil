
/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��PWM.c
 * ����    ����ʼ��TIM2��ͨ��PWM�˿� 
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-10-20
 * Ӳ������: beep->PA1
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	

//ͷ�ļ�
#include "PWM.h"
#include  <ucos_ii.h>
 /**
  * @file   TIM2_PWM_Init
  * @brief  ��ʼ��TIM2ͨ����PWM�˿�
  * @param  ��
  * @retval ��
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
    TIM1->SMCR|=0x07;                                  //���ô�ģʽ�Ĵ��� 
    //TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR0);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);         
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���жϱ�־λ
 
    TIM_ITConfig(      //ʹ�ܻ���ʧ��ָ����TIM�ж�
        TIM1,            //TIM1
        TIM_IT_Update  | //TIM �����ж�Դ
        TIM_IT_Trigger,  //TIM �����ж�Դ 
        ENABLE  	     //ʹ��
    );
	
    //�������ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  	   //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
		TIM_Cmd(TIM1, ENABLE);
}

/**********************************************************
** ������: TIM2_Config
** ��������:  ��ʱ��2����Ϊ����Ƚ�ģʽ
** �������: ��
** �������: ��
** ˵����
		���ʱ��t=36000*8000/72000000=4s
***********************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //TIM2��ͨ��PWM��������˿�PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//CWCCW(200);
}

/**********************************************************
** ������: TIM3_Config
** ��������:  ��ʱ��2����Ϊ����Ƚ�ģʽ
** �������: ��
** �������: ��
** ˵����
		���ʱ��t=36000*8000/72000000=4s
***********************************************************/
void TIM3_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM3��ͨ��PWM��������˿�PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*Ԥ����������ж�λ*//*
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update); 

	/* 4��ͨ��������������ж�*//*
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
	*/
}

void CWCCW(u8 ch)
{
	 OS_CPU_SR  cpu_sr;
	 OS_ENTER_CRITICAL();  //����ȫ���жϱ�־,�����ж�// Tell uC/OS-II that we are starting an ISR
	 OSIntNesting++;	  	  //�ж�Ƕ�ױ�־
	 TIM_Cmd(TIM2,DISABLE);		
   if(ch != 127)
   {		 
		 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	   TIM_OCInitTypeDef  TIM_OCInitStructure;
	   /* ��������*/
	   TIM_TimeBaseStructure.TIM_Prescaler = 1-1; //��ֵ+1Ϊ��Ƶ�ĳ�����һ����0.5ms
	   if(ch > 127)
		    TIM_TimeBaseStructure.TIM_Period = 4096/(ch-126)-1;	//����ֵ   
		 else
			  TIM_TimeBaseStructure.TIM_Period = 4096/(128-ch)-1;	//����ֵ   
		 
	   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
	   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
     //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   	 
	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //����ȽϷ�����ģʽ
  	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ���Ϊ��
		 TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period >> 1);//ͨ��1����Ƚ�ֵ
						
		 if(ch > 127)
		 {
				/* �Ƚ�ͨ��1����*/
			  TIM_OC3Init(TIM2, &TIM_OCInitStructure);//����TIM_OCInitStruct��ָ���Ĳ�����ʼ��TIM2
			  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
			  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
		 }
		 else
		 {
				/*�Ƚ�ͨ��2 */        
			  TIM_OC4Init(TIM2, &TIM_OCInitStructure);//��ָ���Ĳ�����ʼ��TIM2 
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
			  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
	   }
		 
		 TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
     TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
				
		 /*ʹ��Ԥװ��*/
	   TIM_ARRPreloadConfig(TIM2, ENABLE);	
	   	
	   TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
	 }	
   OS_EXIT_CRITICAL();  //����ȫ���жϱ�־,�����ж�// Tell uC/OS-II that we are starting an ISR	 
	 OSIntExit();
}

void LIGHT(u8 ch)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();  //����ȫ���жϱ�־,�����ж�// Tell uC/OS-II that we are starting an ISR
	OSIntNesting++;	  	  //�ж�Ƕ�ױ�־
	TIM_Cmd(TIM3, DISABLE); //ʹ�ܶ�ʱ��2
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 256-1;	//����ֵ   
	TIM_TimeBaseStructure.TIM_Prescaler = 2-1; //��ֵ+1Ϊ��Ƶ�ĳ�����һ����0.5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ���Ϊ��
	/* �Ƚ�ͨ��1����*/
	TIM_OCInitStructure.TIM_Pulse = ch; //*100 + 50;//ͨ��1����Ƚ�ֵ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//����TIM_OCInitStruct��ָ���Ĳ�����ʼ��TIM3
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
	
	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);	
	
	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��2
	OS_EXIT_CRITICAL();  //����ȫ���жϱ�־,�����ж�// Tell uC/OS-II that we are starting an ISR
	OSIntExit();
}
