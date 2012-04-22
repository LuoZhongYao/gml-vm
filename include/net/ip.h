/*
 *  GNU99
 */
#ifndef     __IP_H__
#define     __IP_H__
#include    <vm/malloc.h>
#include    <vm/msg.h>
#include    <glib/list.h>
#define     hostIp()   0xc0a801c7

#pragma     pack(1)
typedef struct{
    unsigned char v:4;            //版本
    unsigned char headLenght:4;   //首部长度,单位4byte
    unsigned char tos;            //服务类型
    unsigned short lenght;        //数据报总长度,单位字节
    unsigned short id;            //标识
    unsigned short flags:3;       //标志
    unsigned short offset:13;     //片偏移
    unsigned char  ttl;           //生存时间
    unsigned char  protocol;      //协议
    unsigned short check;         //校验和
    unsigned int sIP;           //源IP
    unsigned int dIP;           //目的IP
    unsigned char  data[0];       //选项(可选),数据
}*IPDatagram;
#pragma     pack( )
#define     IPPOTO_ICMP     1
#define     IPPOTO_IGMP     2
#define     IPPOTO_TCP      6
#define     IPPOTO_UDP      17
#define     IPPOTO_IGRP     88
void ipSend(unsigned int dip,unsigned int sip,msgTp data,unsigned short type);
void ipRecv(IPDatagram);
#endif
