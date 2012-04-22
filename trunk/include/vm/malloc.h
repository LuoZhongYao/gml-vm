/*
 *  2011-12-9
 */
#ifndef __MALLOC_H__
#define __MALLOC_H__
#include    <stddef.h>
#include    <dgio.h>
typedef struct mnode{
    unsigned long   size;
    struct mnode    *next;
    struct mnode    *root;
    enum{
        FREE = 0xFE,
        BUSY = 0xFB,
    }state;
}mlT,*mlTp;
extern  void *malloc(unsigned long size);
extern  void free(void *p);

#define exit(num) ({\
        printk("1");\
        while(1);   \
    })

#define perror(str) ({\
        printk(str);\
        exit(1);\
    )}
#endif
