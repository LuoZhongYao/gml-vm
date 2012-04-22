/*
 */
#include    <vm/msg.h>
#include    <vm/malloc.h>
#include    <glib/list.h>
#include    <net/in.h>
#include    <net/arp.h>
#include    <net/ethernet.h>
#include    <string.h>
#define     hostIp()   0xc0a801c7

//arp缓存
typedef struct{
    unsigned int  ip;       //IP
    unsigned char mac[ETH_ALEN];    //以太网地址
}*ARPTable,ARPTableT;
//arp缓存表
static List arpTable = NULL;

//arp查询
static void arpRequest(unsigned int ip){
    msgTp msg = NULL;
    ARPHeader *arpHeader;
    unsigned char *sHost = hostMac();
    unsigned char dHost[ETH_ALEN] = {0xff,0xff,0xff,0xff,0xff,0xff};
    msg = malloc(sizeof(msgT) + 60);
    msg->len = 60;//sizeof(ARPHeader);
    msg->next = NULL;
    arpHeader = (ARPHeader *)msg->msg;
    *arpHeader = (ARPHeader){
        .hwdType = htons(ARPHRD_ETHER),
        .protocolType = htons(ETHERTYPE_IP),
        .hwdLenght = ETH_ALEN,
        .protocolLenght = 4,
        .op = htons(ARPOP_REQUEST),
        .dIP =  ip,
    };
    memcpy(arpHeader->sHost,sHost,ETH_ALEN);
    arpHeader->sIP = htonl(hostIp());
    ethSend(msg,dHost,sHost,ETHERTYPE_ARP);
}
//arp回应
static void arpReply(ARPHeader *arp){
    msgTp msg = NULL;
    ARPHeader *arpHeader;
    unsigned char *sHost = hostMac();
    unsigned char dHost[ETH_ALEN] = {0xff,0xff,0xff,0xff,0xff,0xff};
    msg = malloc(sizeof(msgT) + 60);
    msg->len = 60;//sizeof(ARPHeader);
    msg->next = NULL;
    arpHeader = (ARPHeader*)msg->msg;
    *arpHeader = (ARPHeader){
        .hwdType = htons(ARPHRD_ETHER),
        .protocolType = htons(ETHERTYPE_IP),
        .hwdLenght = ETH_ALEN,
        .protocolLenght = 4,
        .op = htons(ARPOP_REPLY),
        .dIP =  arp->sIP,
    };
    memcpy(arpHeader->dHost,arp->dHost,ETH_ALEN);
    memcpy(arpHeader->sHost,sHost,ETH_ALEN);
    arpHeader->sIP = htons(hostIp());
    ethSend(msg,dHost,sHost,ETHERTYPE_ARP);

}
//接收到的arp包处理
void arpRecv(ARPHeader *arpHeader){
    Node node = NULL;
    ARPTable item = NULL;
    if(NULL == arpHeader)
        return;
    //如果arpTable还没有初始化,则查询本次arp请求的IP,进而初始化arpTable
    if(arpTable == NULL)
        ipToMac(arpHeader->sIP);
    //更新IP
    node = arpTable->find(arpTable,&(ARPTableT){arpHeader->sIP,{0}});
    if(NULL == node){
        item = malloc(sizeof(*item));
        if(item == NULL)
            exit(1);
        item->ip = arpHeader->sIP;
    }
    memcpy(item->mac,arpHeader->sHost,ETH_ALEN);
    printk("ip: \e\nmac:\e",item->ip,*(short*)item->mac);
    arpTable->add(arpTable,item);
    //如果是arp查询,并且ip地址是本机,则回复
    if(hostIp() == arpHeader->dIP)
        if(ARPOP_REPLY == arpHeader->protocolType)
            arpReply(arpHeader);
}

//释放arp表项
static void freeArpTable(Node item){
    if(NULL != item)
        free(item->e);
}
static int ipCompare(ARPTable d,ARPTable s){
    if((NULL != s)&&(NULL != d))
        return d->ip - s->ip;
    return 1;
}
//查询IP对应的mac地址,成功返回地址指针,否则返回NULL
extern  unsigned char *ipToMac(unsigned int ip){
    //如果表项为空,则初始化表项,并发送一个arp查询
    Node node;
    //unsigned char *mac = NULL;
    if(arpTable == NULL){
        arpTable = newList(freeArpTable,ipCompare);
        if(arpTable == NULL)
            exit(1);
    }
    //查找表项,如果没有找到,则发送一个arp查询
    node = arpTable->find(arpTable,&(ARPTableT){ip,{0}});
    if(NULL == node){
        arpRequest(ip);
        //等待1秒后,继续查询
        //sleep(1000);
        for(int i=0;i<0xffff;i++);
        node = arpTable->find(arpTable,&(ARPTableT){ip,{0}});
    }
    return ((ARPTable)node->e)->mac;
}
