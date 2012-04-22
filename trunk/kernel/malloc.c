/*
 * 
 *
 */
#include    <vm/mm.h>
#include    <vm/malloc.h>
//#include    "malloc.h"
//extern  unsigned long get_free_page(void);
//#include    <stddef.h>
/* 递归,递归果然好用*/
static mlTp add(mlTp ml,mlTp mm){
    if(ml == NULL){
        mm->root = NULL;
        mm->next = NULL;
        return mm;
    }
    else if(ml->size >= mm->size){
        mm->root = ml->root;
        ml->root = mm;
        mm->next = ml;
        return mm;
    } else{
        ml->next = add(ml->next,mm);
        ml->next->root = ml;
    }
    return ml;
}

static mlTp find(mlTp ml,unsigned long size){
    while(ml != NULL){
        if(ml->size >= size)
            break;
        ml = ml->next;
    }
    return ml;
}

static mlTp del(mlTp mm){
    if(mm != NULL){
        mm->root->next = mm->next;
        if(mm->next != NULL)
            mm->next->root = mm->root;
    }
    return mm;
}

static mlTp unite(mlTp mm){
    if(mm != NULL){
        mlTp nextm = (void *)mm + mm->size + sizeof(mlT);
        //如果mm和nextm在一个页面,并且nextm空闲,则合并mm,nextm
        if(((unsigned long)nextm & (~0xfff)) == ((unsigned long)mm & (~0xfff)))
            if(nextm->state == FREE){
                del(nextm);
                mm->size += nextm->size + sizeof(mlT);
            }
    }
    return mm;
}
static mlTp clean(mlTp ml){
    mlTp mm = ml->next;
    mlTp nextm = NULL;
    while(mm != NULL){
        nextm = mm->root;
        del(mm);
        unsigned long size = 0;
        do{
            size = mm->size;
            mm = unite(mm);
        }while(size != mm->size);
        ml = add(ml,mm);
        if(mm != nextm->next)
            mm = nextm->next;
        else
            mm = nextm->next->next;
    }
    return ml;
}
/* 这两个变量放在这里,很影响程序的结构,但是放在这里比放在开头安全
 * 因为这两个函数只对下面这个两个函数可见
 */
static  mlT  mmlist= {0,NULL,NULL,FREE};
mlTp mmList = &mmlist;
void *malloc(unsigned long size){
    mlTp    mm = NULL;
    //4的倍数分配大小
    size = ((size+3)>>2)<<2;
    if(NULL == (mm = find(mmList,size))){
        mmList = clean(mmList);
        while(NULL == (mm = find(mmList,size))){
            mlTp    nm = NULL;      //new memory
            nm = (void *)get_free_page();
            if(nm == NULL)
                return NULL;
            nm->next = NULL;
            nm->root = NULL;
            nm->state = FREE;
            nm->size = (1<<12) - sizeof(mlT);
            mmList = add(mmList,nm);
        }
    }
    del(mm);
    if(mm->size >=(size + 4 + sizeof(mlT))){
        mlTp    nm = (void *)mm + size + sizeof(mlT);
        nm->state = FREE;
        nm->root = NULL;
        nm->size = mm->size - size -sizeof(mlT);
        mmList = add(mmList,nm);
        mm->size = size;
    }
    mm->state = BUSY;
    return (void *)mm + sizeof(mlT);
}

void free(void *p){
    mlTp    mm = NULL;
    if(p != NULL){
        mm = p - sizeof(mlT);
        mm = unite(mm);
        mm->state = FREE;
        mmList = add(mmList,mm);
    }
}
