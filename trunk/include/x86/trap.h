/*
 *  中断安装,处理程序
 */
#ifndef TRAP_H
#define TRAP_H
//IDT表的逻辑地址
#define IDT_ADDR        0x100000
//中断属性
#define INT_GATE        0x8E00      //中断门
#define TRA_GATE        0x8F00      //陷阱门
#define TR_GATE         0x8500      //任务门
#define IDT_R0          0x0000
#define IDT_R1          0x2000
#define IDT_R2          0x4000
#define IDT_R3          0x6000
//中断函数类型
typedef void (*int_codeTp)(void);
/*
 *  将指定函数加入到IDT中,并设置相应属性
 *  由于这段代码是内核的一部分,使用的是内核数据段,所以没有设置段也是正确的
 */
typedef struct{
    unsigned    short   addr1;
    unsigned    short   section;
    unsigned    short   attr;
    unsigned    short   addr2;
}idtT,*idtTp;
//extern void    nop(void);
extern  void    int_clock(void);
extern  void    print_error(int error);
extern  void    null_int(void);
extern  void    ts_int(void);
extern void install_int(unsigned int num,int_codeTp func,unsigned int section,unsigned short attr); 
extern  void    int_0(void);
extern  void    int_1(void);
extern  void    int_2(void);
extern  void    int_3(void);
extern  void    int_4(void);
extern  void    int_5(void);
extern  void    int_6(void);
extern  void    int_7(void);
extern  void    int_8(void);
extern  void    int_9(void);
extern  void    int_11(void);
extern  void    int_12(void);
extern  void    int_13(void);
extern  void    int_15(void);
extern  void    int_16(void);
extern  void    int_17(void);
extern  void    int_18(void);
extern  void    int_19(void);
extern  void    page_fault(void);
extern  void    trap_init(void);
extern  void    NCI_int(void);
#endif
