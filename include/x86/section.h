/*
 *      这本来是个汇编宏常量的定义,但是出于某些原因,我用了C语言的头文件,C语言的
 *  的确要比汇编本身的号用点.
 *      (c) LuoZH   2011-06-12
 */
#ifndef SECTION_H
#define SECTION_H
/*
 *      数据段和代码段描述符的创建宏,利用它可很直观的创建GDT,IDT表项`
 */
/*
 *      这个宏表面上很漂亮干净,但是,不等不承认,这里有一点问题,很差的可读性,
 *  不信?你看.short  (attr)|((limit>>8)&0x0f00);这行,为什么limit是右移8位,而不是
 *  16位呢.你自己去观察吧,我也是在开启分页机制,映射高内存的时候才发现这里是8,
 *  而不是16,:-)
 */
#define CreateSection(base,limit,attr) \
    .short  limit&0xffff;\
    .short  base&0xffff; \
    .byte   (base>>16)&0xff;\
    .short  (attr)|((limit>>8)&0x0f00);\
    .byte   (base>>24)&0xff;\
//代码段相关属性宏定义
#define CODE_READ   0x009a      //代码段可读
#define CODE_C      0x009c      //一致代码段
#define CODE_G      0x8098      //使用4GB的界限
#define CODE_D      0x4098      //默认使用32位地址
//数据段属性
#define DATA_WRITE  0x0092      //数据段可写
#define DATA_E      0x0094      //向下扩展
#define DATA_G      0x8090      //使用32界限
#define DATA_B      0x4090      //堆栈段使用ESP等32位指针,向下扩展,指明栈上限4GB
//TSS描述符
#define TSS_G       0x8089      //TSS使用4GB界限,通常这个是用不到的
#define TSS         0x0089      //说明这是一个TSS段
//权限
#define R0          0x0000      //ring 0
#define R1          0x0020      //
#define R2          0x0040      //
#define R3          0x0060      //ring 3
#endif
