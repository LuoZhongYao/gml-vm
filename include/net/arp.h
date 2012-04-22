/*
 */
#include    <vm/msg.h>
#include    <net/ethernet.h>

#ifndef     __ARP_H__
#define     __ARP_H__
#pragma     pack(1)
//ARP头部
typedef struct{
    unsigned short hwdType;         //硬件类型
    unsigned short protocolType;    //协议类型
    unsigned char  hwdLenght;       //硬件地址长度
    unsigned char  protocolLenght;  //协议地址长度
    unsigned short op;              //操作
    unsigned char  sHost[ETH_ALEN]; //发送端主机以太网地址
    unsigned int   sIP;             //发送端IP地址
    unsigned char  dHost[ETH_ALEN]; //接收端以太网地址
    unsigned int   dIP;             //接收端IP地址
}ARPHeader;
#pragma     pack()

//硬件类型
#define ARPHRD_NETROM       0  /* From KA9Q: NET/ROM pseudo. */
#define ARPHRD_ETHER        1  /* Ethernet 10/100Mbps.  */
#define ARPHRD_EETHER       2  /* Experimental Ethernet.  */
#define ARPHRD_AX25         3  /* AX.25 Level 2.  */
#define ARPHRD_PRONET       4  /* PROnet token ring.  */
#define ARPHRD_CHAOS        5  /* Chaosnet.  */
#define ARPHRD_IEEE802      6  /* IEEE 802.2 Ethernet/TR/TB.  */
#define ARPHRD_ARCNET       7  /* ARCnet.  */
#define ARPHRD_APPLETLK     8  /* APPLEtalk.  */
#define ARPHRD_DLCI         15 /* Frame Relay DLCI.  */
#define ARPHRD_ATM          19 /* ATM.  */
#define ARPHRD_METRICOM     23 /* Metricom STRIP (new IANA id).  */
#define ARPHRD_IEEE1394     24 /* IEEE 1394 IPv4 - RFC 2734.  */
#define ARPHRD_EUI64        27 /* EUI-64.  */
#define ARPHRD_INFINIBAND   32 /* InfiniBand.  */

//操作
#define ARPOP_REQUEST   1  /* ARP request.  */
#define ARPOP_REPLY     2  /* ARP reply.  */
#define ARPOP_RREQUEST  3  /* RARP request.  */
#define ARPOP_RREPLY    4  /* RARP reply.  */
#define ARPOP_InREQUEST 8  /* InARP request.  */
#define ARPOP_InREPLY   9  /* InARP reply.  */
#define ARPOP_NAK       10  /* (ATM)ARP NAK.  */

/*
extern void arpRequest();
extern void arpReply(void);
*/
//获取IP地址对应的mac,成功返回地址指针,否则返回NULL
extern unsigned char *ipToMac(unsigned int ip);
void arpRecv(ARPHeader *arpHeader);
#endif
