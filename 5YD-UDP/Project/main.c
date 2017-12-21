/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    �������MCUͨ��SPI2��w5500�Ķ�д��������ɶ�IP,mac��gateway�ȵĲ�����
 *						�������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ
 *            ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP���Ա�����
 *            �뽫w5500_conf.c�ļ��е�local_port����Ϊ����ʹ�õļ����˿�,Ĭ��Ϊ5000
 * ��汾  ��V3.50
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2017-07-14
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/

//ͷ�ļ�
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
    printf("  Դ�������豸----UDP Demo V1.0 \r\n");		
    WizW5500_Init(IP_FROM_DEFINE);	
    printf(" W5500���Ժ͵��Ե�UDP�˿�ͨѶ \r\n");
    printf(" W5500�ı��ض˿�Ϊ:%d \r\n",local_port);
    printf(" Զ�˶˿�Ϊ:%d \r\n",remote_port);
    printf(" ���ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
    while (1)
    {
      loopback_udp(SOCK_UDPS, local_port);/*UDP ���ݻػ�����*/
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





