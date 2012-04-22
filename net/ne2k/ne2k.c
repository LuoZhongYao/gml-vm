/*
 *      汇编不给力
 *      :D
 */
#include    <ne2k/ne2k.h>
#include    <x86/io.h>
#include    <dgio.h>
#include    <vm/malloc.h>
#include    <x86/system.h>
#include    <vm/msg.h>
#include    <string.h>
#include    <net/ethernet.h>
//#define     RECV_PACKET     1
//#define     TRANS_PACKET    0xa
#define     TRANS_PAGE      0x40   //0x4000~0x4600该地址是NIC的缓冲区,主机内存
#define     RECV_PAGE       0x46   //0x4600~0x8000,每一页是256字节
#define     RECV_PAGE_END   0x7f

#define     shortNetToHost(num) ({\
        unsigned short _v;          \
        asm("xchgb %%al,%%ah\n\t\t":"=a"(_v):"a"(num):); \
        _v; \
})

static unsigned long long MAC = 0;
static  unsigned char imr = 0x1b;

unsigned long long getNICMac(void){return MAC;};
//static  unsigned char rcr = 0;
//tatic  unsigned char tcr = 0;
//static  unsigned char dcr = 0x58;
#pragma     pack(1)
typedef struct{
    unsigned char state;
    unsigned char next;
    unsigned short len;
}recvHeadT,*recvHeadTp;
#pragma pack()
/*
struct data{
    void *addr;
    unsigned long size;
};
*/
void show(msgTp msg){
    if(msg != NULL){
        for(int i=0;i<msg->len;i++){
            if(!(i%16))
                printk("\n\e ",i);
            if((!(i%8))&&(i%16))
                printk("--");
            else
                printk(" ");
            putByte(msg->msg[i]);
        }
        if(msg->next != NULL){
            printk("\n\n---------NEXT---------");
            show(msg->next);
        }
    }
}
typedef struct queue{
    msgTp  msg;
    struct queue *next;
}queueT,*queueTp;
static queueTp sendQueue = NULL;
static queueTp add(queueTp queue,msgTp buf){
    if(queue == NULL){
        queue = malloc(sizeof(queueT));
        queue->msg = buf;
        queue->next = NULL;
    }else 
        queue->next = add(queue->next,buf);
    return queue;
}
static inline queueTp getLevel(queueTp queue){
    return queue;
}
static queueTp del(queueTp queue){
    queueTp tmp = NULL;
    if(queue != NULL)
        tmp = queue->next;
    free(queue);
    return tmp;
}
void NICtoPC(void *dest,unsigned short dma,int count){
    count++;
    neSetRemoteByteCount((count&-2));
    count >>= 1;
    neSetRemoteAddr(dma);
    neReadDMA();
    asm("0:in   %%dx,%%ax\n\t\t"
            "stosw\n\t\t"
            "loop 0b\n\t\t"
            ::"D"(dest),"c"(count),"d"(IOPORT)
            :"eax");
    while(!(NE_RDC & neGetIntState()));
    neSetIntState(NE_RDC);
}
void PCtoNIC(void *src,unsigned short dma,int count){
    count++;
    neSetRemoteByteCount((count & -2));
    count >>= 1;
    neSetRemoteAddr(dma);
    neWriteDMA();
    asm("0:lodsw\n\t\t"
            "out   %%ax,%%dx\n\t\t"
            "loop 0b\n\t\t"
            ::"S"(src),"c"(count),"d"(IOPORT)
            :"eax");
    //等待写完成
    while(!(NE_RDC & neGetIntState()));
    neSetIntState(NE_RDC);
}
unsigned long long neInit(void){
    unsigned char mac[12] = {0};
    neCmd(NE_DMA_ABORT|NE_STOP);                    		//停止运行
    //字长,高位在15-8,16位DMA传送,回路检测,自动远程操作
    neSetDataConfig(NE_WTS|NE_LS|NE_ARM|NE_BOS); 	//配置数据寄存器
    neSetRemoteByteCount(0);                 		//字节计数归零
    //outb(rcr,RECEIVECONFIGURATION);		    //配置接收寄存器
    neSetRecvConfig(NE_AM|NE_SEP|NE_AB|NE_AR);
    neSetTransferPage(TRANS_PAGE);          //传送页
    neSetTransferConfig(NE_LB00);           //环状模式
    neSetRecvPage(RECV_PAGE,RECV_PAGE_END); //发送缓冲区
    neSetBoundary(RECV_PAGE);               //分界
    neSetCurrnet(RECV_PAGE+1);
    neSetIntState(0xff);                    //中断状态复位
    neSetIntMask(imr);                      //终端掩码,内容是什么我也不知道
    //neSetTransferConfig(NE_LB00);         //进入正常模式,NIC可以工作了
    neCmd(NE_DMA_ABORT|NE_START);          	//停止运行
    //读设置取MAC地址
    NICtoPC(mac,0,12);
    mac[1] = mac[2];
    mac[2] = mac[4];
    mac[3] = mac[6];
    mac[4] = mac[8];
    mac[5] = mac[10];
    neSetMAC((*(unsigned long *)mac),*(unsigned long *)&mac[4]);
    neSetMap(0xffffffff,0xffffffff);
    MAC = *(unsigned long long *)mac;
    return *(unsigned long long *)mac;
}
/*
typedef enum{
    NICtoPC = 1,
    PCtoNIC = 2,
}TransferCmd;
void Transfer(void *buf,unsigned short addr,int count,TransferCmd cmd){
    count++;
    count &= 0xfffffffe;
    neSetRemoteByteCount(count);
    neSetRemoteAddr(addr);
    count >>= 1;
    switch(cmd){
        case    PCtoNIC:
            neWriteDMA();
            asm("0:lodsw\n\t\t"
                    "out   %%ax,%%dx\n\t\t"
                    "loop 0b\n\t\t"
                    ::"S"(buf),"c"(count),"d"(0x10)
                    :"eax");
            break;
        case    NICtoPC:
            printk("recvPacket!");
            neReadDMA();
            unsigned short nextPage = inw(0x10);
            unsigned short len = inw(0x10);
            len += 1;
            len >>= 1;
            asm("0:in   %%dx,%%ax\n\t\t"
                    "stosw\n\t\t"
                    "loop 0b\n\t\t"
                    ::"D"(buf),"c"(len),"d"(10)
                    :"eax");
            neSetCurrnet(nextPage>>8);
    }
    //while(0x40 != inb(INTERRUPTSTATUS));
    outb(0x40,INTERRUPTSTATUS);
    //clc();
}
*/
void neSend(msgTp buf){

    cli();
    //正在发送,则将当前包加入发送队列
    if(buf != NULL){
        if(0x26 == inb(COMMAND)){
            sendQueue = add(sendQueue,buf);
        }else{
            //printk("\nsendPack");
            //show(buf);
            unsigned short count = 0;
            while(buf != NULL){
                PCtoNIC(buf->msg,(TRANS_PAGE<<8) + count,buf->len);
                count += buf->len;
                buf = buf->next;
            }
            neSendPack(TRANS_PAGE,count);
        }
    }
    sti();
}
/*
 *      返回接收的字节数,为-1则表明接收失败.
 */
msgTp neRecv(void){
    unsigned char bound;
    unsigned char currnet;
    msgTp   buf = NULL;
    recvHeadT recv;

    bound = neGetBoundary() + 1;
    currnet = neGetCurrnet();
    if(bound > RECV_PAGE_END)
        bound = RECV_PAGE;

    //有数据在NIC DMA中 
    if(bound != currnet){
        NICtoPC(&recv,bound<<8,sizeof(recvHeadT));
        //由于网络顺序是和本地相反的,所以要进行一次转换
        //printk("len:\e next:\e  \n",recv.len,recv.next);
        //recv.len = shortNetToHost(recv.len);
        //去除4字节的CRC
        recv.len -= 4;
        //接收状态错误,下一页指针错误,长度错误都丢弃所有的包
        if(recv.next>RECV_PAGE_END||recv.next<RECV_PAGE||recv.len>0x600){
            neSetCurrnet(RECV_PAGE);
            neSetBoundary(RECV_PAGE_END);
            return NULL;
        }else{
            buf = malloc(sizeof(msgT) + recv.len);
            if(buf != NULL){
                buf->next = NULL;
                buf->len = recv.len;
                NICtoPC(buf->msg,(bound<<8)|sizeof(recvHeadT),buf->len);
                neSetBoundary((recv.next-1));
            }
        }
    }
    return buf;
}
void neISR(void){
    cli();
    int state = 0;
    queueTp     queue = NULL;
    //该部分需添加禁止该中断重入
#define BUG_NO_RE_INT
    sti();
    state = neGetIntState();
    if(NE_ISR_PRX & state){
        msgTp msg = neRecv();
        //show(msg);
        ethRecv(msg);
        //sendMSG(msg);
    }else if(NE_ISR_PTX & state){
        if(0x38 != inb(TRANSMITSTATUS)){
        }
        if(NULL != (queue = getLevel(sendQueue))){
            neSend(queue->msg);
            sendQueue = del(sendQueue);
        }
    }
    neSetIntState(state);
    neSetIntMask(0);
    cli();
    //该部分添加允许本中断
#define BUG_RE_INT
    outb(0x20,0xa0);
    outb(0x20,0x20);
    neSetIntMask(imr);
    neSetIntState(state);
    sti();
}

