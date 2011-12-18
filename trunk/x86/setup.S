/*
   *    (C) 2011-10
   *    Email:LuoZhongYao@gmail.com
   *    每天都对自己说:"Good luck!"
   *    该程序负责初始化内核工作环境,是内核执行的第一个程序.
   *    初始化内容包括,重新安排GDT(由multiboot规范知道,现在已经有一个GDT了),并且
   *    对8259进行初始化,本质上来说,该程序只是原gmL的setup去除进入保护模式和添加
   *    multiboot支持而已.
   *        使用multiboot的好处是可以使用现存的文件系统,而不用去研究它,可以被其
   *    引导器引导.内核大小没有限制.反正很多啦,所以我个人建议,有写内核的同学,必
   *    要参考multiboot,并遵守它.
   */
/*  汇编代码    */
#define ASM
/*  multiboot规范头文件,该文件直接拷贝于grub_0.98/docs/multiboot.h */
#include    <multiboot.h>
#include    <x86/section.h>
GDT_LEN = gdt_end-gdt_start
/*
#define     io_delay    \
    jmp .+2             \
    jmp .+2
    */
/* IO延时宏 */
.macro  io_delay
    jmp .+2
    jmp .+2
.endm
.text
.globl  _start
_start:
    jmp start
    /*  multiboot   要求4字节对齐   */
    .align  4
multiboot_header:
    /*  multiboot header magic  详情请查阅multiboot */
    .long   MULTIBOOT_HEADER_MAGIC
    /*  multiboot header flags multiboot为你做什么服务,色情服务不在这个范围内*/
    /*  同时也告诉了multiboot你是什么款式的人,oh,程序   */
    .long   MULTIBOOT_HEADER_FLAGS
    /*  multiboot   header checksum ,详情请看multiboot  */
    /* 我猜测引导器就是靠checksum和magic来识别multiboot header的. */
    .long   -(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)
/*  如果不是ELF文件格式,下面的信息就是必须的,这个__ELF__不知道是在哪里定义的 */
/* 不过我的二进制,就算下面这些提供了,grub还是不鸟我,所以我也不鸟二进制 */
#ifndef __ELF__
    /* multiboot header addr,hi哥们,你在哪? */
    .long   multiboot_header
    /* Load addr .text在OSimage中的偏移 */
    .long   _start
    /* Load end addr 引导到OSimage中那个位置,如果为零,引导整个OS Image */
    .long   _edata
    /*  bss段的结束地址.Boot loader将其清零,并且保证不被覆盖 */
    .long   _end
    /*  Boot loader 引导完后将跳转到这里执行 */
    .long   start
#endif
start:
    cli
    /*
    mov     $0x3FFFF0,%esp
    push    %ebx
    */
    /* 保存由Boot loader提供的信息地址 */
    /* mov     %ebx,multiboot_envp  */
    /* 8259,写过很多次了,懒得在写,不懂的可以google,或者Email我 */
    /* 唯一说一点的就是外部中断0x20~0x30 */
    /* 高电平触发,为NE2000专门修改的 */
    mov     $0x11,%al
    out     %al,$0x20
    io_delay
    out     %al,$0xa0    
    io_delay

    mov     $0x20,%al
    out     %al,$0x21
    io_delay
    mov     $0x28,%al
    out     %al,$0xa1
    io_delay   

    mov     $0x04,%al
    out     %al,$0x21
    io_delay
    mov     $0x2,%al
    out     %al,$0xa1
    io_delay

    mov     $0x1,%al
    out     %al,$0x21
    io_delay
    out     %al,$0xa1
    io_delay

/* 屏蔽外部中断 */
    mov     $0xFF,%al
    out     %al,$0x21
    io_delay
    out     %al,$0xa1
    io_delay
/* 重新设置GDT,GDT被放置到0x10100处 */
    mov     $0x101000,%edi
    mov     $gdt_start,%esi
    mov     $GDT_LEN,%ecx
    shr     $2,%ecx
    cld
    rep     movsl
    lgdt    gdt
    lidt    idt
/* 刷新环境 */
    ljmp    $0x8,$new_env
new_env:
    mov     $0x10,%eax
    mov     %eax,%ds
    mov     %eax,%es
    mov     %eax,%ss
    mov     %eax,%fs
    mov     %eax,%gs
    mov     $0x3FFFF0,%esp  
    /* 传递multiboot信息给cmain,转移到cmain去执行 */
    /* 这里是我保证里之前都不会使用ebx,本来这样做不利于以后的发展,但比多一个glob
       强一点 */
    push    %ebx
    call    cmain

over:
#hlt
    jmp  over
    /* 设置临时的分页 */
    
.data
/*
multiboot_envp:
    .long   0x0
    */
gdt:
    .word   0xFFFF
    .long   0x101000
idt:
    .word   256*8-1
    .long   0x100000
gdt_start:
CreateSection(0xAA,0,0XBB);
CreateSection(0x00000,0xFFFFF,CODE_READ|CODE_D|CODE_G)
CreateSection(0x0,0xFFFFF,DATA_WRITE|DATA_G|R0|DATA_B);
CreateSection(0x00000,0xFFFFF,CODE_READ|CODE_D|CODE_G|R3);
CreateSection(0x0,0xFFFFF,DATA_WRITE|DATA_G|R3|DATA_B);
CreateSection(0x100800,0x100,TSS|R3);
gdt_end:
