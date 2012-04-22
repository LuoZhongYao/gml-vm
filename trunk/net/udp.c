#include    <vm/malloc.h>
#include    <string.h>
#include    <net/in.h>
#include    <net/ip.h>
#include    <net/udp.h>

void udpSend(msgTp data,unsigned int dIP,
        unsigned short dPort,unsigned short sPort){
    msgTp   msg = NULL;
    UDPHeader *udpHeader = NULL;
    msg = malloc(sizeof(msgT) + sizeof(UDPHeader));
    msg->next = data;
    msg->len = sizeof(UDPHeader);
    udpHeader = (UDPHeader *)msg->msg;
    udpHeader->sPort = sPort;
    udpHeader->dPort = dPort;
    udpHeader->length = htons((sizeof(UDPHeader) + data->len));
    udpHeader->checkSum = 0x00;

    ipSend(dIP,htonl(hostIp()),msg,IPPOTO_UDP);
}
