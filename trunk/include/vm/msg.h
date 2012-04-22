/*
 *  2012-01
 *  luozhongyao@gmail.com
 *  :
 *      本源码使用GNU99特性.
 *  网络层
 */
#ifndef __NET_H__
#define __NET_H__
//缓存表
typedef struct{
    void    *next;
    unsigned short len;
    unsigned char  msg[0];
}msgT,*msgTp;

extern void sendMSG(msgTp msg);
extern msgTp recvMSG(void);
#endif
