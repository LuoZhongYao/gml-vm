/*
 *  2011-12-?
 *  以太网封装
 */
#include    <802/eth.h>
#include    <ne2k/ne2k.h>
#include    <string.h>
#include    <vm/malloc.h>
#pragma     pack(1)
/*
typedef struct {
    unsigned char destMac[6];
    unsigned char srcMac[6];
    unsigned short len;
    unsigned char DSAP;
    unsigned char SSAP;
    unsigned char cntl;
    unsigned char orgCode[3];
    unsigned short type;
}ethPakcetT,*ethPacketTp;
*/
typedef struct{
    unsigned char destMac[6];
    unsigned char srcMac[6];
    union{
        unsigned short type;
        unsigned short len;
    };
}ethPakcetT,*ethPacketTp;
#pragma     pack()
int ethSend(msgTp src,unsigned long long mac,unsigned short type){
    msgTp   msg = NULL;
    ethPacketTp eth = NULL;

    msg = malloc(sizeof(msgT) + sizeof(ethPakcetT));
    msg->len = sizeof(ethPakcetT);
    msg->next = src;

    eth = (ethPacketTp)msg->msg;
    memcpy(eth->destMac,&mac,6);
    mac = getNICMac();
    memcpy(eth->srcMac,&mac,6);
    eth->len = src->len;
    eth->type = type;

    neSend(msg);

    return 0;
}
