/*
 *  (c) 2011-10 ~   ?
 *  Email:LuoZhongYao@gmail.com
 *      初始化环境,这里的初始化其实只是一个调用,具体的由相应函数去完成,跟x86体系
 *  相关的部分在x86目录下,以此类推.
 */
#include    <multiboot.h>
#include    <vm/mm.h>
#include    <x86/trap.h>
#include    <dgio.h>
#include    <x86/system.h>
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
    //move_user_mode();
    *(unsigned *)0x10000000 = 0xABCEDEF;
    create_mm();
    *(unsigned *)0x10000000 = 0x1111111;
    return 0;
}
