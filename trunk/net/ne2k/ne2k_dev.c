#include    <ne2k/ne2k.h>
#include    <x86/io.h>
#include    <dgio.h>
#include    <vm/malloc.h>
#include    <x86/system.h>
#include    <string.h>
#include    <glib/list.h>


#define     TRANS_PAGE      0x40        //0x4000~0x4600这段是NIC发送缓存区.
#define     RECV_PAGE       0x46        
#define     RECV_PAGE_END   0x47        //0x4000~0x8000这段是接收缓存区.

#
