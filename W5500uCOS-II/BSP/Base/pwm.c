
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

 /**
  * @file   TIM2_PWM_Init
  * @brief  ��ʼ��TIM2ͨ����PWM�˿�
  * @param  ��
  * @retval ��
  */
void TIM2_PWM_Init(void)
{	
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//ʹ��TIM2ʱ�ӣ�GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM2��ͨ��PWM��������˿�PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(3600*10/72M)s=0.0005s����2KƵ��
	TIM_TimeBaseStructure.TIM_Period = 3600-1;//�Զ���װ��ֵ��ȡֵ������0x0000~0xFFFF֮��
	TIM_TimeBaseStructure.TIM_Prescaler =10-1;//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ����ȡֵ������0x0000~0xFFFF֮��							 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ���ģʽ	 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
		
	//����������  
	TIM_OCInitStructure.TIM_Pulse = 900;               //���ô�װ�벶��ȽϼĴ���������ֵ,ȡֵ������0x0000~0xFFFF֮�䣬ռ�ձ�900/3600
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);          //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);               //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���                     
	TIM_Cmd(TIM2, DISABLE);	//��ֹTIM2ʹ��
  //TIM_Cmd(TIM2, ENABLE);//ʹ�ܶ�ʱ��	
}

/**********************************************************
** ������: TIM2_Config
** ��������:  ��ʱ��2����Ϊ����Ƚ�ģʽ
** �������: ��
** �������: ��
** ˵����
		���ʱ��t=36000*8000/72000000=4s
***********************************************************/
void TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5; //TIM2��ͨ��PWM��������˿�PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 40000-1;	//����ֵ   
	TIM_TimeBaseStructure.TIM_Prescaler = 200-1; //��ֵ+1Ϊ��Ƶ�ĳ�����һ����0.5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;//����ȽϷ�����ģʽ
	/* �Ƚ�ͨ��1����*/
	TIM_OCInitStructure.TIM_Pulse = 39999;//ͨ��1����Ƚ�ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ���Ϊ��
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//����TIM_OCInitStruct��ָ���Ĳ�����ʼ��TIM3
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
	
	/*�Ƚ�ͨ��2 */        
	TIM_OCInitStructure.TIM_Pulse = 20000;//ͨ��2����Ƚ�ֵ  
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//��ָ���Ĳ�����ʼ��TIM3 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	/* �Ƚ�ͨ��3 *//*         
	TIM_OCInitStructure.TIM_Pulse = 2000; //ͨ��3����Ƚ�ֵ 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
	
	/* �Ƚ�ͨ��4 *//*       
	TIM_OCInitStructure.TIM_Pulse = 4000;//ͨ��4����Ƚ�ֵ  
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);	
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | /*TIM_IT_CC3 | TIM_IT_CC4|*/TIM_IT_Update); 

	/* 4��ͨ��������������ж�*/
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | /*TIM_IT_CC3 | TIM_IT_CC4|*/TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��2
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
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;
	/* TIM2 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ�Ӻ͸��ù���ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM2��ͨ��PWM��������˿�PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ��������*/
	TIM_TimeBaseStructure.TIM_Period = 4000-1;	//����ֵ   
	TIM_TimeBaseStructure.TIM_Prescaler = 200-1; //��ֵ+1Ϊ��Ƶ�ĳ�����һ����0.5ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  //������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;//����ȽϷ�����ģʽ
	/* �Ƚ�ͨ��1����*/
	TIM_OCInitStructure.TIM_Pulse = 2000;//ͨ��1����Ƚ�ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM����Ƚϼ���Ϊ��
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);//����TIM_OCInitStruct��ָ���Ĳ�����ʼ��TIM2
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);//��ֹOC1��װ��,��ʵ����ʡ�����,��ΪĬ����4·������װ��.
	
	/*�Ƚ�ͨ��2 *//*
	TIM_OCInitStructure.TIM_Pulse = 20000;//ͨ��2����Ƚ�ֵ  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//��ָ���Ĳ�����ʼ��TIM2 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	/* �Ƚ�ͨ��3 *//*         
	TIM_OCInitStructure.TIM_Pulse = 2000; //ͨ��3����Ƚ�ֵ 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	/* �Ƚ�ͨ��4 *//*       
	TIM_OCInitStructure.TIM_Pulse = 4000;//ͨ��4����Ƚ�ֵ  
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);	
	/*Ԥ����������ж�λ*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | /*TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|*/TIM_IT_Update); 

	/* 4��ͨ��������������ж�*/
	TIM_ITConfig(TIM2, TIM_IT_CC1 | /*TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|*/TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��2
}
