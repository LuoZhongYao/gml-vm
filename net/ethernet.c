/*
 */
#include    <net/ethernet.h>
#include    <net/arp.h>
#include    <net/ip.h>
#include    <glib/list.h>
#include    <stddef.h>
#include    <vm/malloc.h>
#include    <string.h>
#include    <ne2k/ne2k.h>
#include    <net/in.h>

unsigned char localMac[ETH_ALEN] = {0xb0,0xc4,0x20,0x00,0x00,0x00};

extern int ethSend(msgTp data,unsigned char *dHost,unsigned char *sHost,unsigned short type){
    msgTp msg = NULL;
    EthernetHeader *eth;
    msg = malloc(sizeof(*msg) + sizeof(EthernetHeader));
    msg->len = sizeof(EthernetHeader);
    msg->next = data;

    eth = (EthernetHeader *)msg->msg;
    memcpy(eth->dHost,dHost,ETH_ALEN);
    memcpy(eth->sHost,sHost,ETH_ALEN);
    eth->type = htons(type);

    neSend(msg);
    return 0;
} 

extern void ethRecv(msgTp msg){
    EthernetHeader *eth = (EthernetHeader *)msg->msg;
    switch(ntohs(eth->type)){
        case ETHERTYPE_IP: ipRecv((IPDatagram)((void*)eth+sizeof(EthernetHeader)));break;
        case ETHERTYPE_ARP: ;
        case ETHERTYPE_REVARP: arpRecv((ARPHeader *)((void*)eth+sizeof(EthernetHeader)));break;
        default:printk("TYPE:\e\n",eth->type);
    }
}
