#ifndef __IN_H__
#define __IN_H__
/*
 */
#define shortSwap(num) ({\
    unsigned short _v;\
    __asm__ __volatile__("xchg %%ah,%%al;":"=a"(_v):"a"(num));\
    _v; \
})

#define longSwap(num) ({    \
    unsigned long _v;   \
    __asm__ __volatile__("xchg %%ah,%%al\n\t\t" \
        "mov %%eax,%0\n\t\t"        \
        "shl $16,%0\n\t\t"          \
        "shr $16,%%eax\n\t\t"       \
        "xchg %%ah,%%al\n\t\t"      \
        "or %%eax,%0\n\t\t"         \
        :"=b"(_v)                   \
        :"a"(num));                 \
    _v;                             \
})
/*
#define longSwap(num) ({\
        unsigned long _v;                           \
        __asm __volatile__("xchg %%ah,%%al\n\t\t"   \
            "mov %%eax,%0\n\t\t"                    \
            :"=b"(_v)                               \
            :"a"(num));                             \
        _v;                                         \
    )}
    */
#define htonl(num) longSwap(num)
#define ntohl(num) longSwap(num)
#define htons(num) shortSwap(num)
#define ntohs(num) shortSwap(num)
#endif
