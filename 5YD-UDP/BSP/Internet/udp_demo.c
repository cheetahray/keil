/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP��ʾ����
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "utility.h"
#include "udp_demo.h"
#include "SYSTICK.h"
#include "tinyosc.h"
/**
*@brief		loopback_udp
*@param		s���˿�ѡ��  port���˿ں�
*@return	��
*/

void loopback_udp(SOCKET s, uint16 port)
{
  uint16 len=0;
	uint8 buff[2048];                                                          /*����һ��2KB�Ļ���*/	
	switch(getSn_SR(s))                                                /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:                                                        /*socket���ڹر�״̬*/
			socket(s,Sn_MR_UDP,port,0);                              /*��ʼ��socket*/
		  break;
		
		case SOCK_UDP:                                                           /*socket��ʼ�����*/
			delay_ms(10);
			if(getSn_IR(s) & Sn_IR_RECV)
			{
				setSn_IR(s, Sn_IR_RECV);                                     /*������ж�*/
			}
			if((len=getSn_RX_RSR(s))>0)                                    /*���յ�����*/
			{
				recvfrom(s,buff, len, remote_ip,&remote_port);               /*W5500���ռ����������������*/
                buff[len-8]=0x00;                                                    /*����ַ���������*/
                printf("%s\r\n",buff);                                               /*��ӡ���ջ���*/ 
				
                len = tosc_writeMessage(buff, sizeof(buff), "/Hit", // the address
                            "iii",   // the format; 'f':32-bit float, 's':ascii string, 'i':32-bit integer
                            ray_remote_ip[3], 3, 122);
	
                sendto(s, buff, len, ray_remote_ip, ray_remote_port);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
			}
			break;
	}
}

void dma_udp(SOCKET s, uint16 port, int distance)
{
    uint16 len=0;
	uint8 buff[32];                                                          /*����һ��2KB�Ļ���*/	
				
    len = tosc_writeMessage(buff, sizeof(buff), "/Hit", // the address
                            "iii",   // the format; 'f':32-bit float, 's':ascii string, 'i':32-bit integer
                            ray_remote_ip[3], 3, distance);
				
    sendto(s, buff, len, ray_remote_ip, ray_remote_port);                /*W5500�ѽ��յ������ݷ��͸�Remote*/

}



