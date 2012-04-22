/*
 */
#ifndef __UDP_H__
#define __UDP_H__
typedef struct{
    unsigned short sPort;
    unsigned short dPort;
    unsigned short length;
    unsigned short checkSum;
}UDPHeader;
void udpSend(msgTp data,unsigned int dIP,unsigned short dPort,unsigned short sPort);

#endif
