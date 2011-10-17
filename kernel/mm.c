/*
 *  (c) 2011-10
 *  Email:LuoZhongYao@gmail.com
 *      再次,写mm.
 *  前1GB是系统内核部分,由内核用来安排内核数据,1GB以后归虚拟机所有.
 *  物理内存小于1GB部分对等映射.不使用Linux 3GB 以后归内核使用的原因是,我比较喜
 *  欢从前面安排内存.
 */
//
#include    <multiboot.h>
#include    <vm/kernel.h>
#include    <x86/trap.h>
#include    <string.h>
#include    <dgio.h>
#include    <stddef.h>
#include    <x86/system.h>
#define     MMAP_ADDR       0x200000
#define     KERNEL_SPACE    0x400000
unsigned long HMEM = 0;             //内存顶端大小
unsigned long LMEM = 0x100000;     //低端内存
unsigned long KMEM = KER_MM_SIZE;            //对等映射的内存大小
unsigned char * const mmap = (unsigned char *)MMAP_ADDR;

/*  获取一页空闲内存  内核空间申请时使用  */
unsigned long get_free_page(void){
    while(1){
        for(int i=KERNEL_SPACE>>12;i<KMEM>>12;i++){
            if(mmap[i]==0){
                mmap[i] = 1;
                return (i<<12);
            }
        }
        //没有空闲空间就对内存进行处理,还未实现
    }
    return 0;
}
/* 获取一页空闲内存,用户空间申请专用 */
unsigned long uget_free_page(void){
    while(1){
        for(int i=KMEM>>12;i<HMEM>>12;i++){
            if(mmap[i]==0){
                mmap[i] = 1;
                return (i<<12);
            }
        }
        //没有用户空间没有空闲内存页了,则向内核空间申请
        return get_free_page();
    }
    return 0;
}
/* 释放内存页 */
int free_page(unsigned long addr){
    if(addr>HMEM){
        //painc("oop:Free high memory!");
        return -1;
    }
    /* 由于内核空间不在申请范围内,所以不需要判断,释放了也行 */
    if(mmap[addr>>12] == 0){
        //painc("oop:Free free memory!");
        return -1;
    }
    mmap[addr>>12]--;
    return 0;
}
/* 启动分页机制 */
unsigned long open_pagination(void){
    unsigned long *pde;
    unsigned long *pte;
    pde = (unsigned long *)get_free_page();
    for(int i=0;i<1024;i++){
        if(i<=KMEM>>22){
            pte = (unsigned long *)get_free_page();
            pde[i] = (unsigned long)pte|3;
            for(int j=0;((i<<10)+j)<KMEM>>12&&j<1024;j++)
                pte[j]=(i<<22)|(j<<12)|3;
        }
        else
            pde[i] = 0;
    }
    __asm__("mov    %0,%%cr3\n\t\t"
            "mov    %%cr0,%%eax\n\t\t"
            "or     $0x80010000,%%eax\n\t"
            "mov    %%eax,%%cr0\n\t\t"
            :
            :"a"(pde));
    return  (unsigned long)pde;
}
/* 将物理映射到线性地址 vd = virtual address pd = physics address*/
int put_page(unsigned vd,unsigned pd){
    unsigned long *pde;
    unsigned long *pte;
    if(mmap[pd>>12]==0)
        return -1;        
    /* 获得PDE */
    __asm__("mov    %%cr3,%0\n\t"
            :"=a"(pde));
    /* 未分配页表则分配 */
    /* 
     * wait.如何保证申请的空间在内核空间内呢?当内存小于等于内核空间时这不是问题
     * 但是,如果内存大于内核空间时呢?
     */
    if(!(pde[vd>>22]&1)){
        pde[vd>>22] = (unsigned long)(get_free_page())|7;
    }
    pte = (unsigned long *)(pde[vd>>22]&(~0xfff));
    pte[(vd>>12)&0x3ff] = pd|7;
    return 0;
}
/* 传建一个虚拟内存,供新的虚拟机使用 */
/* 在考虑要不要共享内存,然后实现写时复制 */
unsigned long *create_mm(void){
    unsigned long *new_mm;
    unsigned long *old_mm;
    unsigned long *pte;
    new_mm = (unsigned long *)get_free_page();
    //获得老指针,供操作
    __asm__("mov    %%cr3,%0\n\t\t":"=a"(old_mm));
    for(int i=KMEM>>22;i<1024;i++){
        if(old_mm[i]&1){
            pte = (unsigned long *)(old_mm[i]&(~0x3ff));
            for(int j=0;j<1024;j++){
                if(pte[j]&1){
                    pte[j] &= 0xfffffffd;
                    mmap[pte[j]>>12]++;
                }
            }
            old_mm[i] &= 0xfffffffd;
            mmap[old_mm[i]>>12]++;
        }
    }
    memcpy(new_mm,old_mm,1024);
    refpage();
    return new_mm;
}
/* 取消页表的共享,返回新的页 */
unsigned long un_wpte_page(unsigned long page){
    unsigned long new_page;
    if(mmap[(page)>>12]>1){
        mmap[(page)>>12]--;
        new_page = get_free_page();
        memcpy((void *)new_page,(void *)page,1024);
        page = new_page;
    }
    return new_page;
}
unsigned long  un_wp_page(unsigned long page){
    unsigned long new_page;
    if(mmap[(page)>>12]>1){
        mmap[(page)>>12]--;
        new_page = uget_free_page();
        memcpy((void *)new_page,(void *)page,1024);
        page = new_page;
    }
    return new_page;
}
/* 页写保护处理 */
void do_wp_page(unsigned error,unsigned long addr){
    unsigned long * pde;
    unsigned long * pte;
    __asm__("mov %%cr3,%0\n\t":"=a"(pde));
    //如果页表也被共享,取消其共享
    if(!(pde[addr>>22]&2)){
        pde[addr>>22] = (un_wpte_page(pde[addr>>22]))|7;
    } 
    pte = (unsigned long *)(pde[addr>>22]&(~0x3ff));
    pte[addr>>12&0x3ff] = un_wp_page(pte[addr>>12&0x3ff])|7;
    refpage();
}
/* 缺页处理 */
void do_no_page(unsigned error,unsigned long addr){
    unsigned long pd;
    pd = uget_free_page();
    put_page(addr,pd);
}
void mmap_init(unsigned long addr,unsigned long length){
    /* multiboot 提供的内存可用分布表 */
    typedef memory_map_t *mmapTp;
    mmapTp mmap_p = (mmapTp)(addr);
    unsigned char busy;
    length+=addr;
    for(int i=0;i<0x100000;i++)
        mmap[i] = 100;
    /* 初始化内存可用表,希望4KB对齐的 */
    for(;addr<length;addr+=(mmap_p->size+4)){
        mmap_p = (mmapTp)addr;
        if(mmap_p->type ==1 ){
            HMEM = mmap_p->base_addr_low+mmap_p->length_low;
            busy = 0;
        }
        else
            busy = 100;
        for(int i=0;i<mmap_p->length_low>>12;i++)
            mmap[i] = busy;
    }
    /*
    //前面4MB内核,GDT,IDT等固定数据占据了
    for(int i=0;i<0x400;i++)
        mmap[i] = 100;
        */
    if(HMEM<KMEM)
        KMEM = HMEM;
    install_int(14,page_fault,KER_CODE_SEC,TRA_GATE|IDT_R0);
    //开启分页
    open_pagination();
}
void mm_init(unsigned long lower,unsigned long upper){
    for(int i=0;i<0x100000;i++)
        mmap[i] = 100;
    for(int i=0x10;i<(upper>>2);i++)
        mmap[i] = 0;
    /*
    //前面4MB内核,GDT,IDT等固定数据占据了
    for(int i=0;i<0x400;i++)
        mmap[i] = 100;
        */
    HMEM = upper;
    if(HMEM<KMEM)
        KMEM = HMEM;
    install_int(14,page_fault,KER_CODE_SEC,TRA_GATE|IDT_R0);
    //开启分页
    open_pagination();
}
