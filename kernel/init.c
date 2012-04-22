/*
 *  (c) 2011-10 ~   ?
 *  Email:LuoZhongYao@gmail.com
 *      初始化环境,这里的初始化其实只是一个调用,具体的由相应函数去完成,跟x86体系
 *  相关的部分在x86目录下,以此类推.
 */
#include    <multiboot.h>
#include    <vm/malloc.h>
#include    <vm/mm.h>
#include    <x86/trap.h>
#include    <dgio.h>
#include    <x86/system.h>
#include    <string.h>
#include    <stddef.h>
#include    <ne2k/ne2k.h>
#include    <net/ethernet.h>
#include    <net/ip.h>
#include    <vm/msg.h>
#include    <net/in.h>
#include    <net/udp.h>

int cmain(multiboot_info_t *env){
    //中断初始化
    trap_init();
    //内存初始化
    if(env->flags&0x40)
        mmap_init(env->mmap_addr,env->mmap_length);
    else if(env->flags&0x1)
        mm_init(env->mem_lower,env->mem_upper);
    else
        return -1;
    //初始化网卡
    neInit();
    msgTp msg = malloc(sizeof(msgT) + 100);
    msg->len = 100;
    msg->next = NULL;
    memcpy(msg->msg,"Hello IP potocol!",17);
    udpSend(msg,htonl(0xc0a80165),htons(8000),htons(1024));
    //ipSend(htonl(0xc0a80165),htonl(hostIp()),msg,IPPOTO_UDP);
    for(int i=0;i<0xFFFFF;i++);
    /*
    ipSend(htonl(0xc0a80165),htonl(hostIp()),msg,IPPOTO_UDP);
    ipSend(htonl(0xc0a80165),htonl(hostIp()),msg,IPPOTO_UDP);
    */
    while(1){
        ;//ipSend(htonl(0xc0a80165),htonl(hostIp()),msg,IPPOTO_UDP);
    }
    return 0;
}
