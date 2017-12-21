/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    ：完成是MCU通过SPI2对w5500的读写操作，完成对IP,mac，gateway等的操作，
 *						内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
 *            如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP测试本程序，
 *            请将w5500_conf.c文件中的local_port定义为测试使用的监听端口,默认为5000
 * 库版本  ：V3.50
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2017-07-14
 * 调试方式：J-Link-OB
**********************************************************************************/

//头文件
#include "stm32f10x.h"
#include "led.h"
#include "SYSTICK.h"
#include "usart.h"
#include "i2c.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include <stdio.h>
#include "tcp_demo.h"
#include "udp_demo.h"
/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    SYSTICK_Init();
    USART1_Config();
    //LED_GPIO_Config();	
    I2C_Configuration();
    printf("  源地仪器设备----UDP Demo V1.0 \r\n");		
    WizW5500_Init(IP_FROM_DEFINE);	
    printf(" W5500可以和电脑的UDP端口通讯 \r\n");
    printf(" W5500的本地端口为:%d \r\n",local_port);
    printf(" 远端端口为:%d \r\n",remote_port);
    printf(" 连接成功后，PC机发送数据给W5500，W5500将返回对应数据 \r\n");
    while (1)
    {
      loopback_udp(SOCK_UDPS, local_port);/*UDP 数据回环测试*/
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





