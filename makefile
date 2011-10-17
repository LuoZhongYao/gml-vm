VMgmL = vmgmL
VMDev = a.img
.PHONY:clear install make start
vpath %.h include/
AS = as
CC = cc
AS_OPT = --32
CC_OPT = -m32 -c -std=gnu99 -fno-stack-protector -I include/ -Wall
LD_OPT = -m elf_i386 -Ttext 0x300000
LD = ld
OBJ = x86/setup.o kernel/init.o kernel/int.o kernel/trap.o kernel/mm.o c/string.o kernel/dg.o
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
