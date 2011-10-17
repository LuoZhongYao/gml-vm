/*
 *      粗略的stdarg.h,供当前阶段的printk函数使用,当然,运行效果我不敢保证
 */
#ifndef     STDARG_H
#define     STDARG_H
typedef     void    *va_list;
#define     __va_rounded_size(type) \
    (((sizeof(type))+sizeof(int)-1)/sizeof(int)*sizeof(int))
#define     va_arg(ap,type) \
    (ap+=__va_rounded_size(type),   \
     *((type *)(ap-__va_rounded_size(type))))
#define     va_start(ap,lastarg)    \
    (ap=((va_list)&lastarg+__va_rounded_size(lastarg)))
#define     va_end(ap)  (ap=(va_list)0)
#endif
