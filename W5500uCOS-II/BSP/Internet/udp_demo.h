#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H

#include "stm32f10x.h"
#include "Types.h"
void loopback_udp(SOCKET s, uint16 port);
void loopback_artnet(SOCKET s, uint16 port, u8 * lastch, u8 * ch, u8 * lastlight, u8 * watt);/*UDP 数据回环测试*/
#endif 


