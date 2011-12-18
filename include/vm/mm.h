/*
 *
 */
#ifndef MM_H
#define MM_H
extern void mm_init(unsigned long,unsigned long);
extern void mmap_init(unsigned long,unsigned long);
extern unsigned long * create_mm(void);
extern unsigned long get_free_page(void);
#endif
