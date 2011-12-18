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
int ethSend(void *src,unsigned long long mac,unsigned short type,const int count){
    ethPacketTp eth = NULL;    
    int len = sizeof(ethPakcetT) + count;
    if(len >1492)
        return -1;
    else if(len < 60)
        len = 60;

    eth = malloc(len);
    memcpy(eth->destMac,&mac,6);
    mac = getNICMac();
    memcpy(eth->srcMac,&mac,6);
    eth->len = count;
    //eth->DSAP = 0xaa;
    //eth->SSAP = 0xaa;
    //eth->cntl = 0x03;
    //memset(eth->orgCode,0,3);
    eth->type = type;
    memcpy((void *)eth + sizeof(ethPakcetT),src,count);
    neSend(eth,len);
    return 0;
}
