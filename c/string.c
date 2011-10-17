#include    <string.h>
void *memcpy(void *dest,const void *src,int n){
    asm("cld\n\t"
            "rep\n\t"
            "movsb\n\t"
            :
            :"c"(n),"S"(src),"D"(dest)
            :
           );
    return dest;
}
void *memset(void *dest,int ch,int n){
    asm("0:movb  %%al,(%1)\n\t"
        "inc %1\n\t"
        "loop 0b\n\t"
        :
        :"a"(ch),"D"(dest),"c"(n)
        :);
    return dest;
}
