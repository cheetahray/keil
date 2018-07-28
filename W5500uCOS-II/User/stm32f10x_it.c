/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "w5500_conf.h"
#include "usart.h"
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
u16 nTime;
void SysTick_Handler(void)
{
    if(nTime)
        nTime--;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void TIM1_UP_IRQHandler(void) 
{ 	    	  	     
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源 
/***************在此处添加中断执行内容******************/
    stepray += 65536 ;  
	}	     
} 
/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;	  	  //中断嵌套标志
    OS_EXIT_CRITICAL();	  //恢复全局中断标志

    /*
    if ( TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET )
    {
        timer2_isr(); W5500
        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
    }*/
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//判断TIM2的比较1通道是否有中断
    {
        /*必须清空标志位*/
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);//清除比较1中断标志位
        if(stepray != hereray)
        {
            GPIOA->BRR=GPIO_Pin_4;//PB0=0
            if(STEPAROUND == stepray)
                stepray = 0;
            else
                stepray++;
        }
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)//判断TIM2的比较2通道是否有中断
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);//清除比较2中断标志位
        if(stepray != hereray)
        {
            GPIOA->BRR=GPIO_Pin_5;//PB1=0
            if(0 == stepray)
                stepray = STEPAROUND;
            else
                stepray--;
        }
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//溢出中断
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除溢出中断标志位
        GPIOA->BSRR=GPIO_Pin_4;//PB0=1
        GPIOA->BSRR=GPIO_Pin_5;//PB1=1
    }
    OSIntExit();
}
/*
void TIM3_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
  	OSIntNesting++;	  	  //中断嵌套标志
  	OS_EXIT_CRITICAL();	  //恢复全局中断标志

  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//判断TIM3的比较1通道是否有中断
	{
		/*必须清空标志位*//*
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);//清除比较1中断标志位
		GPIOA->BSRR=GPIO_Pin_6;//PB0=1
	}
	else if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//溢出中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除溢出中断标志位
		GPIOA->BRR=GPIO_Pin_6;//PB0=0
	}
	OSIntExit();
}
*/
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

extern DMA_InitTypeDef DMA_InitStructure;
/**********************************************************
** 函数名:void DMA1_Channel5_IRQHandler(void)
** 功能描述: DMA中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void DMA1_Channel5_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //保存全局中断标志,关总中断// Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;		  //用于中断嵌套
    OS_EXIT_CRITICAL();	  //恢复全局中断标志

    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        //DataCounter = DMA_GetCurrDataCounter(DMA1_Channel5);//获取剩余长度,一般都为0,调试用
        DMA_ClearITPendingBit(DMA1_IT_GL5);	//清除全部中断标志
        //转换可操作BUF
        if(Free_Buf_No==BUF_NO1) //如果BUF1空闲，将DMA接收数据赋值给BUF1
        {
            DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_Buf1;
            DMA_Init(DMA1_Channel5, &DMA_InitStructure);
            Free_Buf_No=BUF_NO2;
        }
        else  //如果BUF2空闲，将DMA接收数据赋值给BUF2
        {
            DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_Buf2;
            DMA_Init(DMA1_Channel5, &DMA_InitStructure);
            Free_Buf_No=BUF_NO1;
        }
        Buf_Ok=TRUE;
    }

    OSIntExit();  //在os_core.c文件里定义,如果有更高优先级的任务就绪了,则执行一次任务切换
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
