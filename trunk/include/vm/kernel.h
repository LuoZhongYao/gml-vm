/*
 *  (c) 2011-10
 *  Email:LuoZhongYao@gmail.com
 */
//内核代码段描述符
#define KER_CODE_SEC    0x8
#define KER_DATA_SEC    0x10
//期望对等映射内存的大小,一般是1GB,如果实际物理内存没有那么大,那么就是实际物理内存大小
#define KER_MM_SIZE      0x40000000
