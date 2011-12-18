VMgmL = vmgmL
VMDev = a.img
.PHONY:clear install make start
vpath %.h include/
AS = as
CC = cc
#网络部分
NET_DIR = net
NET_CC = -I include/net/
NET_LD = 
NET_OBJ = $(NET_DIR)/ne2k/ne2k.o $(NET_DIR)/802/eth.o $(NET_DIR)/arp.o
#内核部分
KN_CC = -m32 -c -std=gnu99 -fno-stack-protector -I include/ -Wall
KN_LD = -m elf_i386 -Ttext 0x300000
KN_OBJ = x86/setup.o kernel/init.o kernel/int.o kernel/trap.o kernel/mm.o c/string.o kernel/dg.o kernel/malloc.o 
#总编译
AS_OPT = --32
CC_OPT =$(KN_CC) $(NET_CC) #-m32 -c -std=gnu99 -fno-stack-protector -I include/ -Wall
LD_OPT = $(KN_LD) $(NET_LD) #-m elf_i386 -Ttext 0x300000
LD = ld
OBJ = $(KN_OBJ) $(NET_OBJ) #x86/setup.o kernel/init.o kernel/int.o kernel/trap.o kernel/mm.o c/string.o kernel/dg.o kernel/malloc.o ne2000/ne2000.o net802/eth.o
$(VMgmL):$(OBJ)
	$(LD) $^ -o $@ $(LD_OPT)
%.o:%.S
	$(CC) $< -o $@ $(CC_OPT)
%.o:%.c
	$(CC) $< -o $@ $(CC_OPT)
%.o:%.s
	$(AS) $< -o $@ $(AS_OPT)
install:
	mount -t ext2 -o loop $(VMDev) mnt/
	cp $(VMgmL) mnt/
	umount mnt/
clear:
	(cd kernel;rm -f *.o);
	(cd x86;rm -f *.o);
	(cd $(NET_DIR)/;rm -f *.o);
	(cd $(NET_DIR)/ne2k;rm -f *.o);
	(cd $(NET_DIR)/802;rm -f *.o);
