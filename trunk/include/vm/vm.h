/*
 *  (c) 2011-10-11
 *  Email:LuoZhongYao@gmail.com
 *
 */
#ifndef __VM_H__
#define __VM_H__
/* 虚拟CPU */
typedef struct{
    unsigned long ip;
    unsigned long sp;
    unsigned long bp;
    unsigned long mm;
}cpuT,*cpuTp;
/* 指令 */
typedef struct{
    unsigned long op;
    unsigned long src;
    unsigned long des;
}cmdT,*cmdTp;
#endif
