/*
 *      内核临时调试用打印函数
 */
#include    <stdarg.h>
#include    <string.h>
static int x=0,y=10;
void write_ch(int ch){
    unsigned int addr=y*80+x;
    addr<<=1;
    switch(ch){
        case    '\n':y++;x=0;break;
        default:
            asm("add    $0xb8000,%1\n\t"
                "or     $0x0F00,%0\n\t"
                "mov    %%ax,(%1)\n\t"
                :
                :"a"(ch),"b"(addr)
                );
        x++;
    }
    y+=x/80;
    x%=80;
    addr=(y*80+x)*2;
    if(addr>80*25*2){
        memcpy((void *)0xb8000,(void *)(0xb8000+160),25*80*2);
        y--;
    }
}
void puthex(unsigned int num){
    if(num){
        puthex(num>>4);
        write_ch("0123456789ABCDEF"[num&0xF]);
    }else
        write_ch('0');
}
void putByte(unsigned char num){
        write_ch("0123456789ABCDEF"[(num>>4)&0xF]);
        write_ch("0123456789ABCDEF"[num&0xF]);
}
int printk(char *str,...){
    va_list ap;
    va_start(ap,str);
    while(*str){
        switch(*str){
            case    '\e':puthex(va_arg(ap,unsigned int));str++;break;
            case '%':
            default:write_ch(*str++); 
        }
    }
    return  0;
}
void clear(void){
    x=y=0;
    memset((void *)0xb8000,0,80*25*2);
}
