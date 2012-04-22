/*
 */

//#include    <glib/list.h>
#include    <vm/msg.h>
#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#define ETH_ALEN    6       //以太网物理地址长度
//以太网地址
#define     hostMac()  localMac 
unsigned char localMac[ETH_ALEN];
#pragma     pack(1)
// 10Mb/s 以太网头部
typedef struct{
    unsigned char dHost[ETH_ALEN];  //目的主机
    unsigned char sHost[ETH_ALEN];  //源主机
    unsigned short type;            //包类型
}EthernetHeader; 
#pragma     pack()

//以太网协议类型
#define ETHERTYPE_PUP       0x0200  /* Xerox PUP */
#define ETHERTYPE_SPRITE    0x0500  /* Sprite */
#define ETHERTYPE_IP        0x0800  /* IP */
#define ETHERTYPE_ARP       0x0806  /* Address resolution */
#define ETHERTYPE_REVARP    0x8035  /* Reverse ARP */
#define ETHERTYPE_AT        0x809B  /* AppleTalk protocol */
#define ETHERTYPE_AARP      0x80F3  /* AppleTalk ARP */
#define ETHERTYPE_VLAN      0x8100  /* IEEE 802.1Q VLAN tagging */
#define ETHERTYPE_IPX       0x8137  /* IPX */
#define ETHERTYPE_IPV6      0x86dd  /* IP protocol version 6 */
#define ETHERTYPE_LOOPBACK  0x9000  /* used to test interfaces */

extern  int ethSend(msgTp data,unsigned char *dHost,unsigned char *sHost,unsigned short type);
extern  void ethRecv(msgTp);
#endif

