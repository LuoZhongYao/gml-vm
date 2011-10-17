#	setup模块的生成,并写入映像
#
#
.PHONY:clear all
AS = as
CC = cc
AS_OPT = --32
CC_OPT = -m32 -c -std=gnu99 -fno-stack-protector -I ../include -Wall
LD_OPT = --oformat binary -m elf_i386 
LD = ld
HEAD= 
vpath	%.h ../include/ 
#all:setup.o init.o trap.o int.o dg.o sys_call.o sys.o sched.o mm.o fork.o thread.o
%.o:%.S
	$(CC) $< -o $@ $(CC_OPT)
%.o:%.c
	$(CC) $< -o $@ $(CC_OPT)
%.o:%.s
	$(AS) $< -o $@ $(AS_OPT)
clear:
	-rm -rf *.o
