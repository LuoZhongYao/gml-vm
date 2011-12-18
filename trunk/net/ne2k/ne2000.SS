/*
 *  (c)LuoZhongYao
 *  2011-12-?
 *  Network.Come on,Network!
 *  代码可以说是完全照抄<>
 */
#include    <ne2000/ne2000.h>
.data
    rcr: .byte   0       #接收配置寄存器的值
    tcr: .byte   0       #传送配置寄存器的值
    dcr: .byte  0x58     #数据配置寄存器的值
    imr: .byte  0xb      #终端掩码值
.globl  neInit,neSend,neISR
.text
/*
 *  初始化ne3000系列网卡
 */
neInit:
    push    %eax
    push    %edx

    mov     $0x21,%al               #停止工作
    mov     $COMMAND,%dx
    out     %al,%dx

    mov     dcr,%al                 #数据配置寄存器 
    mov     $DATACONFIGURATION,%dx
    out     %al,%dx

    mov     $REMOTEBYTECOUNT0,%dx
    xor     %al,%al
    out     %al,%dx                 #低位远端计算字节
    mov     $REMOTEBYTECOUNT1,%dx
    out     %al,%dx                 #高位远端计数字节

    mov     rcr,%al                 #接收配置
    mov     $RECEIVECONFIGURATION,%dx
    out     %al,%dx

    mov     $0x20,%al               #开始传送页
    mov     $TRANSMITPAGE,%dx
    out     %al,%dx

    mov     $0x2,%al                #暂时加入回送模式
    mov     $TRANSMITCONFIGURATION,%dx
    out     %al,%dx

    mov     $0x26,%al               #页开始
    mov     $PAGESTART,%dx
    out     %al,%dx

    mov     $BOUNDARY,%dx           #分界
    out     %al,%dx

    mov     $0x40,%al               #页结束
    mov     $PAGESTOP,%dx
    out     %al,%dx

    mov     $0x61,%al               #去页1的寄存器
    mov     $COMMAND,%dx
    out     %al,%dx

    mov     $0x26,%al               #当前页
    mov     $CURRENT,%dx
    out     %al,%dx

    mov     $0x22,%al
    mov     $COMMAND,%dx
    out     %al,%dx                 #返回页0,开始模式

    mov     $0xff,%al
    mov     $INTERRUPTSTATUS,%dx
    out     %al,%dx                 #中断状态

    mov     imr,%al
    mov     $INTERRUPTMASK,%dx
    out     %al,%dx                 #中断掩码

    mov     $TRANSMITCONFIGURATION,%dx
    mov     tcr,%al
    out     %al,%dx                 #TCR 在正常模式,NIC准备接收

    pop     %edx
    pop     %eax
    ret


/*
 *      NE3000发送
 */
/*
 *  neSend(void *src,int count);
 *  src 发送的数据,count 发送字节数
 */
neSend:
    push    %ebp
    mov     %esp,%ebp
    pushal

    cli
    mov     $COMMAND,%dx
    in      %dx,%al
    cmp     $0x26,%al                   #正在传送中?
    je      queueIt                 #加入到传送队列

    mov     8(%ebp),%eax
    mov     12(%ebp),%ecx
    call    PCtoNIC
    mov     $TRANSMITPAGE,%dx
    out     %al,%dx
    mov     $TRANSMITBYTECOUNT0,%dx
    mov     %cl,%al
    out     %al,%dx
    mov     $TRANSMITBYTECOUNT1,%dx
    mov     %ch,%al
    out     %al,%dx
    mov     $COMMAND,%dx
    mov     $0x26,%al
    out     %al,%dx
    jmp     0f
queueIt:
    call    queuePacket
0:  
    sti
    popal
    leave
    ret

/*
 *      PCtoNIC
 *      DS:ESI = 待发送的包
 *      ECX    = 发送字节数
 *      EAX    = 网卡缓存页
 */
PCtoNIC:
    pushal
    push    %eax
    inc     %ecx
    and     $0xFFFFFFFE,%ecx            #二倍数字节,优化用
    mov     $REMOTEBYTECOUNT0,%dx
    mov     %cl,%al
    out     %al,%dx
    mov     $REMOTEBYTECOUNT1,%dx
    mov     %ch,%al
    out     %al,%dx                     #设置发送字节寄存器
    pop     %eax
    mov     $REMOTESTARTADDRESS0,%dx
    out     %al,%dx
    mov     $REMOTESTARTADDRESS1,%dx
    mov     %ah,%al
    out     %al,%dx                     #DMA地址
    mov     $COMMAND,%dx
    mov     $0x12,%al
    out     %al,%dx                     #开始写
    mov     $IOPORT,%dx
    shr     $1,%ecx
0:
    lodsw
    out     %ax,%dx
    loop    0b
    mov     $0x0,%ecx
    mov     $INTERRUPTSTATUS,%dx
0:
    in      %dx,%al
    test    $0x40,%al
    jnz     1f
    jmp     0b                          #DMA完成?
1:
    mov     $INTERRUPTSTATUS,%dx
    mov     $0x40,%al
    out     %al,%dx
    clc
    popal
    ret



/*
 *  NICtoPC
 *  es:edi = 包接收地址
 *  ecx    = 接收字节数
 *  eax    = 网卡DMA缓存页
 */
NICtoPC:
    pushal
    push    %eax
    inc     %ecx
    and     $0xFFFFFFFE,%ecx
    mov     $REMOTEBYTECOUNT0,%dx
    mov     %cl,%al
    out     %al,%dx
    mov     $REMOTEBYTECOUNT1,%dx
    mov     %ch,%al
    out     %al,%dx
    pop     %eax
    mov     $REMOTESTARTADDRESS0,%dx
    out     %al,%dx
    mov     $REMOTESTARTADDRESS1,%dx
    mov     %ah,%al
    out     %al,%dx
    mov     $COMMAND,%dx
    mov     $0xa,%al
    out     %al,%dx
    mov     $IOPORT,%dx
    shr     $1,%ecx
0:
    in      %dx,%ax
    stosw
    loop    0b
    mov     $INTERRUPTSTATUS,%dx
0:
    in      %dx,%al
    test    $0x40,%al
    jnz     1f
    jmp     0b
1:
    out     %al,%dx
    popal
    ret


neISR:
    
