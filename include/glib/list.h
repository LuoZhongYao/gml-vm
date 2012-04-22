/*
 */
#ifndef __LIST_H__
#define __LIST_H__

typedef void *Element;

typedef struct node{
    Element e;
    struct node * next;
}*Node;
//无序列表
typedef struct list *List;
struct list{
    Node nodes;
    void (*add)(List this,Element);      //添加元素到列表头.
    void (*addLast)(List this,Element);  //添加元素到列表尾部.
    void (*remove)(List this);           //删除列表头部元素.
    void (*removeLast)(List this);       //删除链表尾部元素.
    Node (*first)(List this);            //获取列表第一个节点.
    Node (*last)(List this);             //获取列表最后一节点.
    Node (*find)(List this,void *);      //查找节点
    int  (*compare)(void *a,void* b);    //节点比较,=0,相等;小于0,b<a;大于0,b>a

    void (*removeNode)(Node);            //节点资源释放函数
    void (*forEach)(List this,void(*)(Node));          //遍历节点
};

extern List newList(void (*remove)(Node),int (*compare)(void*,void*));      //new一个列表对象
extern void delList(List this);                 //删除一个列表对象
#endif
