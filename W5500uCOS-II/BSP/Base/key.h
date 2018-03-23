#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"
/*
#define KEY1_BUTTON_PIN                   GPIO_Pin_5
#define KEY1_BUTTON_GPIO_PORT             GPIOB
#define KEY1_BUTTON_GPIO_RCC              RCC_APB2Periph_GPIOB
#define KEY1_Status_Read()								GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define KEY2_BUTTON_PIN                   GPIO_Pin_8
#define KEY2_BUTTON_GPIO_PORT             GPIOB
#define KEY2_BUTTON_GPIO_RCC              RCC_APB2Periph_GPIOB
#define KEY2_Status_Read()								GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
*/
#define KEY3_BUTTON_PIN                   GPIO_Pin_7
#define KEY3_BUTTON_GPIO_PORT             GPIOA
#define KEY3_BUTTON_GPIO_RCC              RCC_APB2Periph_GPIOA
#define KEY3_Status_Read()								GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)


void KEY_GPIO_Config(void);

#endif
