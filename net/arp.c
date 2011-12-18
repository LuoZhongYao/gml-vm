/*
 *      试试ARP协议
 */
#include    <802/eth.h>
#include    <string.h>
#define     ARP_TYPE    0x0608
#pragma     pack(1)
typedef struct{
    unsigned short hdwType;     	//硬件类型
    unsigned short ptlType;     	//协议类型
    unsigned char  hdwAddrLen;  	//硬件地址长度
    unsigned char  ptlAddrLen;  	//协议地址长度
    unsigned short op;          	//操作
    unsigned char  srcEthAddr[6];	//
    unsigned int   srcIP;           //
    unsigned char  destEthAddr[6];  //
    unsigned int   destIP;          //
}arpHeadT,*arpHeadTp;
#pragma     pack()


unsigned long long IPtoMac(unsigned int ip,unsigned long long mac){
    arpHeadT arp = {
        .hdwType = 0x0100,
        .ptlType = 0x0008,//ARP_TYPE,
        .hdwAddrLen = 6,
        .ptlAddrLen = 4,
        .op = 0x0100,
        .destIP = ip,};
    memcpy(&arp.srcEthAddr,&mac,6);
    arp.srcIP = 0x6601a8c0;
    ethSend(&arp,-1,ARP_TYPE,sizeof(arpHeadT));
    return 0;
}

void replayARP(unsigned int sIP,unsigned int dIP,
        unsigned long long sMAC,unsigned long long dMAC){
    arpHeadT arp = {
        .hdwType = 0x0100,
        .ptlType = 0x0008,
        .hdwAddrLen = 6,
        .ptlAddrLen = 4,
        .op = 0x0200,
        .destIP = dIP,
        .srcIP = sIP,
    };
    memcpy(&arp.srcEthAddr,&sMAC,6);
    memcpy(&arp.destEthAddr,&dMAC,6);
    ethSend(&arp,dMAC,ARP_TYPE,sizeof(arpHeadT));
}
