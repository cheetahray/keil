#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
void TIM1_Config(void);
void TIM3_Config(void);
void TIM2_Config(void);
void CWCCW(u8 ch);
void LIGHT(u8 ch);
#endif
