#include    <vm/malloc.h>
#include    <net/ip.h>
#include    <net/ethernet.h>
#include    <net/arp.h>
#include    <glib/list.h>
#include    <vm/msg.h>
#include    <net/in.h>
#include    <string.h>
unsigned short checkSum(unsigned short *src,int lenght){
    unsigned long sum = 0;
    for(sum = 0; lenght > 0;lenght--)
        sum += *src++;
    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum>>16);
    return ~sum;
}
void ipSend(unsigned int dip,unsigned int sip,msgTp data,unsigned short type){
    msgTp   msg = NULL;
    unsigned short len = 0;
    msgTp   tmp = NULL;
    IPDatagram ipDatagram = NULL;
    unsigned char *dHost = NULL;
    unsigned char *sHost = NULL;
    //数据流处理
    msg = malloc(sizeof(msgT) + sizeof(*ipDatagram));
    if(NULL == msg)
        exit(1);
    msg->len = sizeof(*ipDatagram);
    msg->next = data;
    //计算数据长度
    tmp = msg;
    while(tmp != NULL){
        len += tmp->len;
        tmp = tmp->next;
    }
    //ip数据报头处理
    ipDatagram = (IPDatagram)msg->msg;
    memset((void *)ipDatagram,20,0);
    ipDatagram->v = 5;
    ipDatagram->headLenght = 4;
    ipDatagram->lenght = htons(len);
    ipDatagram->id = htons(1);
    ipDatagram->offset = 0;
    ipDatagram->ttl = 60;
    ipDatagram->protocol = type;
    ipDatagram->sIP = sip;
    ipDatagram->dIP = dip;
    ipDatagram->check = checkSum((unsigned short *)ipDatagram,10);
    
    //获取发送以太网地址
    dHost = ipToMac(dip);
    sHost = hostMac();
    ethSend(msg,dHost,sHost,ETHERTYPE_IP);
}

void ipRecv(IPDatagram ipDatagram){
    //printk("IPType:\e\n",ipDatagram->protocol);
}

