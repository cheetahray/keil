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

/**
*@brief		loopback_udp
*@param		s���˿�ѡ��  port���˿ں�
*@return	��
*/

void loopback_udp(SOCKET s, uint16 port)
{
  uint16 len=0;
	uint8 buff[2048];                                                  /*����һ��2KB�Ļ���*/	

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
				sendto(s,buff,len-8, remote_ip, remote_port);                /*W5500�ѽ��յ������ݷ��͸�Remote*/
			}
			break;
		}
}

void loopback_artnet(SOCKET s, uint16 port)
{
  uint16 len=0;
	uint8 buff[2048];																							/*����һ��2KB�Ļ���*/	
	
	uint8 packet_length=18;																				//artnet header length
  int dmx_ch;                                                        /*counter to print DMX data*/
	int opcode;
	int protocal_ver;
	int sequence;
	int physical;
	int sub_net;
	int universe;
	int net;
	int dmx_length;																								

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
				recvfrom(s,buff, len, remote_ip,& remote_port);               /*W5500���ռ����������������*/				
				if(len>18 && 0 == memcmp(buff, "Art-Net\x00", 8) ){																									 /*unpack artnet data*/
					opcode = buff[8]+(buff[9]<<8);
					if(opcode==0x5000){
						protocal_ver = buff[10]+(buff[11]);
						sequence = buff[12];
						physical = buff[13];
						sub_net = (buff[14]&0XF0)>>4;
						universe = buff[14]&0X0F;
						net = buff[15];
						dmx_length = (buff[16]<<8)+buff[17];
						printf("\r\n %s opc: %d, pver: %d, seq: %d, phy: %d, "
										"sub_net: %d, uni: %d, net: %d, length: %d || "
										,buff,opcode,protocal_ver,sequence,physical,		/*print artnet header data*/
											sub_net,universe,net, dmx_length);						
						for(dmx_ch=0;dmx_ch<10;dmx_ch++){
							printf("%d.",buff[dmx_ch+packet_length]);							/*print dmx data*/
						}
					}
				}
				sendto(s,buff,len-8, remote_ip, remote_port);                /*send artnet data to remote port*/
			}
			break;
		}
}



