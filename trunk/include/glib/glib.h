/*
 *  2012-01
 *  luozhongyao@gmail.com
 *  GNU99
 */
#ifndef __GLIB_H__
#define __GLIB_H__

typedef struct list{
    void *data;
    struct list *next;
}listT,*listTp;

extern  listTp createList(void);
extern listTp addNode(listTp h,void *data);
extern listTp removeNode(listTp h);
extern listTp getNode(listTp h);
#endif
