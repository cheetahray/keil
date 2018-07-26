/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__
#define STEPAROUND 38400
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#define angle2step(changle)  ( ( (changle) * 768 ) / 5 ) //STEPAROUND / 250
#include  <ucos_ii.h>
/*
#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_mem.h>
#include  <lib_str.h>
*/
#include  <stm32f10x_conf.h>
//#include  <stm32f10x_lib.h>

#include  <app_cfg.h>
#include  "usart.h"
#include  "W5500_conf.h"
#include  "W5500.h"
#include  "udp_demo.h"
#include  "socket.h"
#include  "key.h"
#include  "exti.h"
#include  "pwm.h"
/*
#include  <lcd.h>
#include  <bsp.h>
*/
/*
#if (APP_OS_PROBE_EN == DEF_ENABLED)
#include  <os_probe.h>
#endif

#if (APP_PROBE_COM_EN == DEF_ENABLED)
#include  <probe_com.h>

#if (PROBE_COM_METHOD_RS232 == DEF_ENABLED)
#include  <probe_rs232.h>
#endif
#endif
*/
#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif
EXT INT16U stepray, hereray;
EXT OS_EVENT* Com1_MBOX;  
EXT OS_EVENT* CWCCW_MBOX;  
/*
EXT unsigned char TxBuffer1[400]; 
 
EXT unsigned char RxBuffer1[400];
EXT unsigned char TxCounter1;
EXT unsigned int TxCounter2;
EXT volatile unsigned int RxCounter1; 
EXT volatile unsigned int RxCounter2;

EXT volatile unsigned char rec_f,tx_flag;
*/
#endif
