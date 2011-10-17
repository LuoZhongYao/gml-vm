/*
   *    这个文件处理大部分中断,参考了下linux的asm.s,发现它的设计很优秀,很有技巧
   *    性,我很欣赏,所以结构很像linux的asm.s
   *    (c) 2011    LuoZH
   */
#include    <vm/kernel.h>
.globl  page_fault,print_error,do_wp_page,do_no_page,do_sys_page
.globl  null_int,ts_int,int_clock
.globl  int_0,int_1,int_2,int_3,int_4,int_5,int_6,int_7,int_8,int_9
.globl  int_11,int_12,int_13,int_15,int_16,int_17,int_18,int_19
.globl  
.data
KEN_MODE:   .int  0 
/*
   *    脉搏
   */
.text
/*
   *    时钟中断本来可以一起处理,但是考虑到这里面可能的任务切换,单独处理可能效果
   *    更好一些.
   */
int_clock:
    pushal
    push    %ds
    push    %es
    push    %fs
    push    %gs
    mov     $KER_DATA_SEC,%ebx
    mov     %ebx,%ds
    mov     %ebx,%es
    mov     %ebx,%fs
    mov     %ebx,%gs
#    call    do_time     #处理时间相关
/*
   *    这里说明一下,调用sched的时候保存了当前的栈到进程的状态信息里面,在函数返
   *    后这个新的进程栈将被加载,所以我们函数返回后不需要加上esp,而程序能够正常
   *    运行.
   */
    mov     $0x1,%ebx
    cmp     KEN_MODE,%ebx
    je      0f
    push    %esp
#    call    sched       #进程调度
#下面却换任务
    mov     %eax,%esp   #空间切换
    mov     %cr3,%eax
#cmp     %eax,run_space
    je      0f
#    mov     run_space,%eax
    mov     %eax,%cr3
0:
    mov     $0x20,%eax
    outb    %al,$0x20
    pop     %gs
    pop     %fs
    pop     %es
    pop     %ds
    popal
    iret
    
/*
   *    这是所有中断的入口形式,她的目的很明确,就是保存环境,初始化段寄存器,并
   *    调用相应的中断处理函数,在trap.c里面实现
   */
ts_int:
    push    $print_error
    jmp     error_code
int_0:
    push    $print_error
    push    $0  
    jmp     dg_error_code
int_1:
    push    $print_error
    push    $1
    jmp     dg_error_code
int_2:
    push    $print_error
    push    $2
    jmp     dg_error_code
int_3:
    push    $print_error
    push    $3
    jmp     dg_error_code
int_4:
    push    $print_error
    push    $4
    jmp     dg_error_code
int_5:
    push    $print_error
    push    $5
    jmp     dg_error_code
int_6:
    push    $print_error
    push    $6
    jmp     dg_error_code
int_7:
    push    $print_error
    push    $7
    jmp     dg_error_code
int_8:
    push    $print_error
    jmp     error_code
int_9:
    push    $print_error
    push    $9
    jmp     dg_error_code
int_11:
    push    $print_error
    jmp     error_code
int_12:
    push    $print_error
    jmp     error_code
int_13:
    push    $print_error
    jmp     error_code
int_15:
    push    $print_error
    push    $15
    jmp     dg_error_code
int_16:
    push    $print_error
    push    $16
    jmp     dg_error_code
int_17:
    push    $print_error
    jmp     error_code
int_18:
    push    $print_error
    push    $9
    jmp     dg_error_code
int_19:
    push    $print_error
    push    $19
    jmp     dg_error_code
null_int:
    push    $nop
no_error_code:
    xchgl   %eax,(%esp)
/*
   *    下面我就犯傻了,由于gcc编译器调用函数保存了除eax,ecx,edx以外的寄存器,
   *    所以我们只用保存这几个就可以了.
   *   pusha
   */
    push    %ecx
    push    %edx
    push   %ds
    push   %es
    push   %gs
    push   %fs
/*
   *    加载内核数据段
   */
    mov     $KER_DATA_SEC,%edx 
    mov     %edx,%ds
    mov     %edx,%es
    mov     %edx,%gs
    mov     %edx,%fs
    call    *%eax
    pop     %fs
    pop     %gs
    pop     %es
    pop     %ds
    pop     %edx
    pop     %ecx
    pop     %eax
    iret
error_code:
    xchg    %eax,4(%esp)
    xchg    %ebx,(%esp)
    push    %ecx
    push    %edx
    push    %edi
    push    %esi
    push    %ebp
    push    %ds
    push    %es
    push    %fs
    push    %eax
    lea     44(%esp),%eax
    push    %eax
    mov     $0x8,%eax
    mov     %eax,%ds
    mov     %eax,%es
    mov     %eax,%fs
    call    *%ebx
    addl    $8,%esp
    pop     %fs
    pop     %es
    pop     %ds
    pop     %ebp
    pop     %esi
    pop     %edi
    pop     %edx
    pop     %ecx
    pop     %ebx
    pop     %eax
    iret
page_fault:
    xchgl   %eax,(%esp)
    pushl   %ecx
    pushl    %edx
    pushl    %ds
    pushl    %es
    pushl    %fs
    mov     $0x1,%ecx
    mov     %ecx,KEN_MODE
    mov     $KER_DATA_SEC,%edx 
    mov     %dx,%ds
    mov     %dx,%es
    mov     %dx,%fs
    mov     %cr2,%edx
    push    %edx
    push    %eax
    /*
       *    判断是否是页面不存在错误
       */
    test    $0x1,%eax
    jne     0f
    call    do_no_page    
    jmp     1f
0:
    call    do_wp_page    
1:
    add     $0x8,%esp
    mov     $0x0,%ecx
    mov     %ecx,KEN_MODE
    mov     $KER_DATA_SEC,%edx 
    pop     %fs
    pop     %es
    pop     %ds
    pop     %edx
    pop     %ecx
    pop     %eax
    iret
/*
   *    调试用
   */
dg_error_code:
    xchgl   %eax,4(%esp)
    xchgl   %ecx,(%esp)    /*调试用*/
/*
   *    下面我就犯傻了,由于gcc编译器调用函数保存了除eax,ecx,edx以外的寄存器,
   *    所以我们只用保存这几个就可以了.
   *   pusha
   */
    push    %edx
    push   %ds
    push   %es
    push   %gs
    push   %fs
/*
   *    加载内核数据段
   */
    mov     $KER_DATA_SEC,%edx 
    mov     %edx,%ds
    mov     %edx,%es
    mov     %edx,%gs
    mov     %edx,%fs
    push    %ecx
    call    *%eax
    pop     %ecx
    pop     %fs
    pop     %gs
    pop     %es
    pop     %ds
    pop     %edx
    pop     %ecx
    pop     %eax
    iret
