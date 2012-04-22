/*
 *  2012-01
 *  luozhongyao@gmail.com
 *  该源码使用GNU99特性
 */
/*
#include    <vm/msg.h>
#include    <glib.h>
#include    <stddef.h>
static listTp msgList = NULL;
void sendMSG(msgTp msg){
    msgList = listInsertEnd(msgList,msg);
}

msgTp recvMSG(void){
    listTp ls = NULL;
    msgTp msg = NULL;
    ls = listGetFrist(msgList);
    msg = ls->data;
    msgList = listDelFrist(msgList);
    return msg;
}
*/
