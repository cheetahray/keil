/**
******************************************************************************
* @file   		udp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		UDP演示函数
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
*@param		s：端口选择  port：端口号
*@return	无
*/

void loopback_udp(SOCKET s, uint16 port)
{
  uint16 len=0;
	uint8 buff[2048];                                                          /*定义一个2KB的缓存*/	
	switch(getSn_SR(s))                                                /*获取socket的状态*/
	{
		case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
			socket(s,Sn_MR_UDP,port,0);                              /*初始化socket*/
		  break;
		
		case SOCK_UDP:                                                           /*socket初始化完成*/
			delay_ms(10);
			if(getSn_IR(s) & Sn_IR_RECV)
			{
				setSn_IR(s, Sn_IR_RECV);                                     /*清接收中断*/
			}
			if((len=getSn_RX_RSR(s))>0)                                    /*接收到数据*/
			{
				recvfrom(s,buff, len, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据*/
                buff[len-8]=0x00;                                                    /*添加字符串结束符*/
                printf("%s\r\n",buff);                                               /*打印接收缓存*/ 
				
                len = tosc_writeMessage(buff, sizeof(buff), "/Hit", // the address
                            "iii",   // the format; 'f':32-bit float, 's':ascii string, 'i':32-bit integer
                            ray_remote_ip[3], 3, 122);
	
                sendto(s, buff, len, ray_remote_ip, ray_remote_port);                /*W5500把接收到的数据发送给Remote*/
			}
			break;
	}
}

void dma_udp(SOCKET s, uint16 port, int distance)
{
    uint16 len=0;
	uint8 buff[32];                                                          /*定义一个2KB的缓存*/	
				
    len = tosc_writeMessage(buff, sizeof(buff), "/Hit", // the address
                            "iii",   // the format; 'f':32-bit float, 's':ascii string, 'i':32-bit integer
                            ray_remote_ip[3], 3, distance);
				
    sendto(s, buff, len, ray_remote_ip, ray_remote_port);                /*W5500把接收到的数据发送给Remote*/

}



