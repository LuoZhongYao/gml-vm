#ifndef __ETH_H__
#define __ETH_H__
#define IP_PACKET       0x0800
#define ARP_PACKET      0x0806
#define RARP_PACKET     0x0
#endif
#include    <vm/msg.h>
int ethSend(msgTp ,unsigned long long,unsigned short);
