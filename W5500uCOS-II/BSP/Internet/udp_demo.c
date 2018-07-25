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
#include "includes.h"
//#include "SYSTICK.h"

/**
*@brief		loopback_udp
*@param		s：端口选择  port：端口号
*@return	无
*/

void loopback_udp(SOCKET s, uint16 port)
{
    uint16 len=0;
    uint8 buff[2048];                                                  /*定义一个2KB的缓存*/

    switch(getSn_SR(s))                                                /*获取socket的状态*/
    {
    case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
        socket(s,Sn_MR_UDP,port,0);                              /*初始化socket*/
        break;

    case SOCK_UDP:                                                           /*socket初始化完成*/
        //delay_ms(10);
        if(getSn_IR(s) & Sn_IR_RECV)
        {
            setSn_IR(s, Sn_IR_RECV);                                     /*清接收中断*/
        }
        if((len=getSn_RX_RSR(s))>0)                                    /*接收到数据*/
        {
            recvfrom(s,buff, len, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据*/
            buff[len-8]=0x00;                                                    /*添加字符串结束符*/
            if(memcmp(buff,"p1on",4) == 0)
                TIM_Cmd(TIM2, ENABLE);//使能定时器
            else if(memcmp(buff,"p1off",5) == 0)
                TIM_Cmd(TIM2, DISABLE);//使能定时器
            printf("%s\r\n",buff);                                               /*打印接收缓存*/
            //sendto(s,buff,len-8, remote_ip, remote_port);                /*W5500把接收到的数据发送给Remote*/
        }
        break;
    }
}

void loopback_artnet(SOCKET s, uint16 port, u8 * lastch, u8 * ch, u8 * lastlight, u8 * watt, u8 * lastchangle, u8 * changle)
{
    uint16 len=0;

    switch(getSn_SR(s))                                                /*获取socket的状态*/
    {
    case SOCK_CLOSED:                                                        /*socket处于关闭状态*/
        socket(s,Sn_MR_UDP,port,0);                              /*初始化socket*/
        break;

    case SOCK_UDP:                                                           /*socket初始化完成*/
        //delay_ms(10);
        if(getSn_IR(s) & Sn_IR_RECV)
        {
            setSn_IR(s, Sn_IR_RECV);                                     /*清接收中断*/
        }
        if((len=getSn_RX_RSR(s))>0)                                    /*接收到数据*/
        {
            static uint8 buff[513];																							/*定义一个2KB的缓存*/

            recvfrom(s,buff, len, remote_ip,& remote_port);               /*W5500接收计算机发送来的数据*/
            if(len>18 && 0 == memcmp(buff, "Art-Net\x00", 8) ) {																									 /*unpack artnet data*/
                //int dmx_ch;                                                        /*counter to print DMX data*/
                int opcode;
                //int protocal_ver;
                //int sequence;
                //int physical;
                //int sub_net;
                int universe;
                //int net;
                int dmx_length;

                opcode = buff[8]+(buff[9]<<8);
                if(opcode==0x5000) {
                    //protocal_ver = buff[10]+(buff[11]);
                    //sequence = buff[12];
                    //physical = buff[13];
                    //sub_net = (buff[14]&0XF0)>>4;
                    universe = buff[14]&0X0F;
                    //net = buff[15];
                    dmx_length = (buff[16]<<8)+buff[17];/*
                    printf("\r\n %s opc: %d, pver: %d, seq: %d, phy: %d, "
                           "sub_net: %d, uni: %d, net: %d, length: %d || "
                           ,buff,opcode,protocal_ver,sequence,physical,		/*print artnet header data*//*
                           sub_net,universe,net, dmx_length);
                    for(dmx_ch=0; dmx_ch<3; dmx_ch++) {
                        printf("%d.\t",buff[dmx_ch+packet_length]);							/*print dmx data*/
                    //}
                    if(dmx_length>=2)
                    {
                        static uint8 Vacc[5];
                        uint8 packet_length=18;																				//artnet header length
                        uint8 boolangle;
                        *ch = buff[packet_length];
                        *watt = buff[packet_length+1];
                        *changle = buff[packet_length+2];
                        boolangle = (*changle < 250);
                        if(*lastch == 127 && Vacc[2] != 2 && Vacc[3] != 2)
                        {
                            Vacc[1] = *lastch;
                            Vacc[2] = 2;
                            Vacc[3] = 2;
                            stepray = angle2step(*changle);
                        }
                        else if(0 == boolangle && *lastch != *ch)
                        {
                            printf("ch1= %d.\t", *ch);							/*print dmx data*/
                            Vacc[0] = *ch;
                            *lastch = *ch;
                            OSMboxPost(CWCCW_MBOX, Vacc);
                        }
                        else if(*lastchangle != *changle)
                        {
                            Vacc[4] = *changle;
                            if(stepray == hereray)
                            {
                                Vacc[1] = 127;
                                *lastch = 127;
                            }
                            if(boolangle)
                            {
                                Vacc[0] = *ch;
                                OSMboxPost(CWCCW_MBOX, Vacc);
                            }
                            *lastchangle = *changle;
                        }
                        else if(*lastlight != *watt)
                        {
                            printf("ch2= %d.\t", *watt);							/*print dmx data*/
                            *lastlight = *watt;
                            LIGHT(*watt);
                        }
                    }
                }
            }
            //sendto(s,buff,len-8, remote_ip, remote_port);                /*send artnet data to remote port*/
        }
        break;
    }
}



