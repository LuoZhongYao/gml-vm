/*
 *      (c)LuoZhongYao
 *      2011-12-?
 *      尝试写写NE2000兼容网卡,玩点刺激
 */
#ifndef __NE3000_H__
#define __NE3000_H__
#include    <x86/io.h>
#include    <vm/msg.h>

//寄存器端口
#define	COMMAND	                0x300
#define	PAGESTART	        	COMMAND + 0x1
#define	PAGESTOP	        	COMMAND + 0x2
#define	BOUNDARY	        	COMMAND + 0x3
#define	TRANSMITSTATUS	    	COMMAND + 0x4
#define	TRANSMITPAGE	    	COMMAND + 0x4
#define	TRANSMITBYTECOUNT0		COMMAND + 0x5
#define	NCR	                	COMMAND + 0x5
#define	TRANSMITBYTECOUNT1		COMMAND + 0x6
#define	INTERRUPTSTATUS	    	COMMAND + 0x7
#define	CURRENT	            	COMMAND + 0x7
#define	REMOTESTARTADDRESS0		COMMAND + 0x8
#define	CRDMA0	            	COMMAND + 0x8
#define	REMOTESTARTADDRESS1		COMMAND + 0x9
#define	CRDMA1	            	COMMAND + 0x9
#define	REMOTEBYTECOUNT0		COMMAND + 0xa
#define	REMOTEBYTECOUNT1		COMMAND + 0xb
#define	RECEIVESTATUS	    	COMMAND + 0xc
#define	RECEIVECONFIGURATION	COMMAND + 0xc
#define	TRANSMITCONFIGURATION	COMMAND + 0xd
#define	FAE_TALLY	        	COMMAND + 0xd
#define	DATACONFIGURATION   	COMMAND + 0xe
#define	CRC_TALLY	        	COMMAND + 0xe
#define	INTERRUPTMASK	    	COMMAND + 0xf
#define	MISS_PKT_TALLY	    	COMMAND + 0xf
#define IOPORT                  COMMAND + 0x10

//PAGE1寄存器o
#define PAR0                    COMMAND + 0x01
#define PAR1                    COMMAND + 0x02
#define PAR2                    COMMAND + 0x03
#define PAR3                    COMMAND + 0x04
#define PAR4                    COMMAND + 0x05
#define PAR5                    COMMAND + 0x06
#define MAR0                    COMMAND + 0x08
#define MAR1                    COMMAND + 0x09
#define MAR2                    COMMAND + 0x0a
#define MAR3                    COMMAND + 0x0b
#define MAR4                    COMMAND + 0x0c
#define MAR5                    COMMAND + 0x0d
#define MAR6                    COMMAND + 0x0e
#define MAR7                    COMMAND + 0x0f

//设置网卡地址
#define neSetMAC(macl,mach)   {\
    neCmd(NE_PAGE01|NE_STOP|NE_DMA_ABORT);        \
    outb((macl)&0xff,PAR0);                        \
    outb((macl>>8)&0xff,PAR1);                     \
    outb((macl>>16)&0xff,PAR2);                    \
    outb((macl>>24)&0xff,PAR3);                        \
    outb((mach)&0xff,PAR4);                        \
    outb((mach>>8)&0xff,PAR5);                        \
    neCmd(NE_PAGE00|NE_START|NE_DMA_ABORT);       \
}
#define neSetMap(mapl,maph){\
    neCmd(NE_PAGE01|NE_STOP|NE_DMA_ABORT);        \
    outb((mapl)&0xff,MAR0);                        \
    outb((mapl>>8)&0xff,MAR1);                     \
    outb((mapl>>16)&0xff,MAR2);                    \
    outb((mapl>>24)&0xff,MAR3);                        \
    outb((maph)&0xff,MAR4);                        \
    outb((maph>>8)&0xff,MAR5);                        \
    outb((maph>>16)&0xff,MAR6);                        \
    outb((maph>>24)&0xff,MAR7);                        \
    neCmd(NE_PAGE00|NE_START|NE_DMA_ABORT);       \
}
//网卡命令集
#define NE_STOP         0x01        //停止接收或发送数据
#define NE_START        0x02        //启动
#define NE_SEND         0x04        //发送数据
#define NE_NO_DMA       0x00        //禁止DMA
#define NE_READ_DMA     0x08        //运行远程DMA读
#define NE_WRITE_DMA    0x10        //允许远程DMA写
#define NE_SEND_PCK     0x18        //发送数据
#define NE_DMA_ABORT    0x20        //中断DMA操作
#define NE_PAGE00        0x00        //页1
#define NE_PAGE01        0x40
#define NE_PAGE02        0x80
#define NE_PAGE03        0xc0
//给网卡发送命令宏定义
#define neCmd(cmd) outb((cmd),COMMAND)
//允许写
#define neWriteDMA()    neCmd(NE_START|NE_WRITE_DMA)
#define neReadDMA()    neCmd(NE_START|NE_READ_DMA)
//发送数据包
#define neSendPack(page,count)   {\
    neSetTransferPage((page));      \
    neSetTransferByteCount((count));  \
    neCmd(NE_DMA_ABORT|NE_SEND|NE_START);   \
}

//中断状态
#define neGetIntState()    inb(INTERRUPTSTATUS)
#define neSetIntState(state)    outb((state),INTERRUPTSTATUS)
#define NE_ISR_PRX      0x01        //接收数据正常
#define NE_ISR_PTX      0x02        //发送数据正常
#define NE_RXE          0x04        //接收数据有:CRC,丢包,频队列错误
#define NE_TXE          0x08        //过分冲突导致传送错误
#define NE_OVW          0x10        //接收缓冲区溢出
#define NE_CNT          0x20        //当一个或多个网络计数器的MSB设置完成时该位被设置
#define NE_RDC          0x40        //远端DMA操作完成
#define NE_REST         0x80        //复位


//DCR数据结构寄存器
#define neSetDataConfig(mode)       outb((mode)|0x80,DATACONFIGURATION)
#define NE_WTS          0x01        //字长传送DMA数据
#define NE_BOS          0x02        //高位在MD15-8,低位在MD7-0
#define NE_LAS          0x04        //16位DMA传送模式
#define NE_LS           0x08        //回路检测,正常操作
#define NE_ARM          0x10        //自动初始化远程操作,发送包命令
#define NE_FT0          0x00
#define NE_FT1          0x20
#define NE_FT8          0x40
#define NE_FT12         0x60

//传送配置
#define neSetTransferConfig(mode)   outb(mode|0xE0,TRANSMITCONFIGURATION)
#define NE_TCR_COMFIG   0x00        //设置将要配置TCR寄存器
#define NE_TCR_CRC      0x00        //CRC正常操作
#define NE_LB00         0x00        //正常操作
#define NE_LB01         0x02        //内部LookBack
#define NE_LB10         0x04        //外部LookBack
#define NE_LB11         0x06        //外部LookBack
#define NE_ATB          0x08        //多播地址混合接收
#define NE_OFST         0x10        //冲突补偿

//传送状态寄存器
#define neGetTransferState()        inb(TRANSMITSTATUS)
#define NE_TSR_PTX      0x01        //无错误
#define NE_COK          0x04        //数据传送过程中出现冲突
#define NE_ABT          0x08        ///超频导致传送失败
#define NE_CRS          0x10        //传送数据包有丢包
#define NE_CDH          0x40        //
#define NE_OWC          0x80        //

//RCR接收配置寄存器
#define neSetRecvConfig(mode)       outb(mode,RECEIVECONFIGURATION)
#define NE_RCR_CONFIG   0x04        //设置将要配置RCR寄存器
#define NE_SEP          0x01        //接收错误的数据包
#define NE_AR           0x02        //接收小于4byte的数据包
#define NE_AB           0x04        //接收广播数据包
#define NE_AM           0x08        //接收多播地址数据包
#define NE_PRO          0x10        //接收发送给本机的数据包
#define NE_MON          0x20

    //RSR接收状态寄存器
#define neGetRecvState()            inb(RECEIVESTATUS)
#define NE_RSR_PRX      0x01        //无错误
#define NE_RSR_CRC      0x02        //CRC错误
#define NE_FAE          0x04        //频同步错误
#define NE_MPA          0x10        //数据丢包
#define NE_PHY          0x20        //多播或广播数据
#define NE_DIS          0x40        //禁止接收
#define NE_DFF          0x80        //冲突

//设置传送字节计数器
#define neSetRemoteByteCount(count)   {\
            outb((count)&0xff,REMOTEBYTECOUNT0);\
            outb(((count)>>8)&0xff,REMOTEBYTECOUNT1);\
        }
//设置传送地址
#define neSetRemoteAddr(addr)   {\
        outb((addr) & 0xff,REMOTESTARTADDRESS0);      \
        outb((addr >> 8) & 0xff,REMOTESTARTADDRESS1);   \
    }
//设置传送页
#define neSetTransferPage(page)     outb(page,TRANSMITPAGE)
#define neSetTransferByteCount(count)   {\
            outb((count)&0xff,TRANSMITBYTECOUNT0);\
            outb(((count)>>8)&0xff,TRANSMITBYTECOUNT1);\
        }
//设置发送页
#define neSetRecvPage(start,stop)   {\
        outb(start,PAGESTART);             \
        outb(stop,PAGESTOP);             \
    }
//设置中断允许位
#define neSetIntMask(mask)  outb(mask,INTERRUPTMASK)
//设置BOUNDARY
#define neSetBoundary(page) outb(page,BOUNDARY)
#define neGetBoundary()     inb(BOUNDARY)
//设置CURRENT
#define neSetCurrnet(page)  {\
        neCmd(NE_PAGE01|NE_STOP|NE_DMA_ABORT);       \
        outb(page,CURRENT);     \
        neCmd(NE_PAGE00|NE_START|NE_DMA_ABORT);        \
    }
#define neGetCurrnet()  ({\
        unsigned char _v;\
        neCmd(NE_PAGE01|NE_START|NE_DMA_ABORT);       \
        _v = inb(CURRENT);     \
        neCmd(NE_PAGE00|NE_START|NE_DMA_ABORT);        \
        _v;     \
    })
unsigned long long neInit(void);
void neSend(msgTp);
msgTp neRecv(void);
void neISR(void);
unsigned long long getNICMac(void);
#endif
