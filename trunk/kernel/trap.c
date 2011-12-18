/*
 *      LuoZH/init/init.c
 *      如kernel.h文件里描述,我不擅长处理文件名,这个文件名就是借用linux的,但是
 *  内容没有用linux的,原因很简单,我看不懂linux的,:-)
 *      (c) 2011    LuoZH
 */
#include    <x86/trap.h>
#include    <x86/io.h>
#include    <vm/kernel.h>
#define     LATCH   1193180
//IDT表指针
const idtTp idt =   (idtTp) IDT_ADDR;
/*
 *  默认中断C处理函数
 */
void    nop(void){
    ;
}
void    print_error(int error){
}
/*
 *  时钟中断处理函数
 */
inline void install_int(unsigned int num,int_codeTp  func,unsigned int section,unsigned short attr){
    idt[num].addr1=(unsigned short)((unsigned int)func)&0xFFFF;
    idt[num].addr2=(unsigned short)(((unsigned int)func)>>16)&0xFFFF;
    idt[num].section=section;
    idt[num].attr=attr;
}
void trap_init(void){
    /*
     *  安装默认中断
     */
    for(int i=0;i<256;i++){
        install_int(i,null_int,KER_CODE_SEC,INT_GATE|IDT_R0);
    }
    install_int(0,int_0,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(1,int_1,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(2,int_2,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(3,int_3,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(4,int_4,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(5,int_5,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(6,int_6,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(7,int_7,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(8,int_8,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(9,int_9,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(10,ts_int,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(11,int_11,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(12,int_12,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(13,int_13,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(15,int_15,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(16,int_16,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(17,int_17,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(18,int_18,KER_CODE_SEC,INT_GATE|IDT_R0);
    install_int(19,int_19,KER_CODE_SEC,INT_GATE|IDT_R0);
    /*
     *  将时钟中断的设置为10ms一次
     */
    outb_p(0x36,0x43);
    outb_p(LATCH&0xFF,0x40);
    outb_p(LATCH>>8,0x40);
    /*
     *  安装时钟中断,并开启中断允许
     */
    //install_int(0x20,int_clock,KER_CODE_SEC,INT_GATE|IDT_R0);
    //outb_p(inb_p(0x21)&~0x1,0x21);
    //outb_p(inb_p(0x21)&~0x1,0x21);
    install_int(0x23,NCI_int,KER_CODE_SEC,INT_GATE|IDT_R0);
    outb_p(inb_p(0x21)&~0x8,0x21);
    //outb_p(inb_p(0xa1)&~0x2,0xa1);
    //安装系统调用中断
    //install_int(0x80,sys_call,KER_CODE_SEC,TRA_GATE|IDT_R3);
}
