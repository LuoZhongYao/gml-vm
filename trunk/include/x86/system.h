#define move_user_mode()    \
    asm(    \
        "pushl  $0x23\n\t"  \
        "pushl  $0x7FFF00\n\t"  \
        "pushfl\n\t"    \
        "mov    (%%esp),%%eax\n\t"  \
        "or     $0x200,%%eax\n\t"    \
        "mov    %%eax,(%%esp)\n\t"  \
        "pushl  $0x1b\n\t"  \
        "pushl  $0f\n\t"    \
        "iret\n\t"  \
        "0:\n\t"    \
        "mov    $0x23,%%eax\n\t"  \
        "mov    %%eax,%%ds\n\t"  \
        "mov    %%eax,%%es\n\t"  \
        "mov    %%eax,%%gs\n\t"  \
        "mov    %%eax,%%fs\n\t"  \
        :::"%eax" \
       )
#define sti()   asm("sti")
#define cli()   asm("cli")
#define wait()  asm("wait")
#define refpage()   \
    asm("mov    %%cr3,%%eax\n\t"    \
        "mov    %%eax,%%cr3\n\t"    \
        :::"%eax")
