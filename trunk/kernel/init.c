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
#include    <802/eth.h>

#define     NET     1

#if NET 
#define     MAC 0x0000000020c4b0
#define     MSG "Net Host"
#else
#define     MAC 0x0001000020c4b0
#define     MSG "Host 2 !D:"
#endif
#define     IP 0x0101a8c0
#define     HOST_IP 0x6501a8c0
unsigned int IPtoMac(unsigned int ip,unsigned long long mac);
void replayARP(unsigned int ip,unsigned int,unsigned long long ,unsigned long long mac);

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
    char *p = malloc(1024);
    unsigned long long s_mac = neInit(p);
    unsigned long long d_mac = MAC;
    //ethSend(MSG,d_mac,0x0608,0x200);
    //IPtoMac(IP,d_mac);
    extern int T;
    //printk("Apply Route IP to MAC!\n");
    //IPtoMac(IP,s_mac);
    //printk("Apply Host IP to MAC!\n");
    IPtoMac(HOST_IP,s_mac);
    int flags = 1;
    while(1){
        if(T){
            if(flags){
                replayARP(HOST_IP,IP,MAC,0xc773b2272100);
                printk(p+34);
                flags = 0;
            }
            T = 0;
        }
    }
    for(int i =0xffe;i;i--){
        IPtoMac(IP,s_mac);
        /*
        if(!(i%2000))
            ethSend(MSG,d_mac,0x200);
        else if(!(i%4001))
            ethSend("OKTHYU",d_mac,0x200);
        //for(int i = 0xfffe;i--;);
        //printk("DMAC:\e\e\n",*((unsigned int *)(&d_mac)+1),(unsigned int)d_mac);
        printk(&p[14]);
        */
    }
    //move_user_mode();
    /*
    *(unsigned *)0x10000000 = 0xABCEDEF;
    create_mm();
    *(unsigned *)0x10000000 = 0x1111111;
    */
    return 0;
}
