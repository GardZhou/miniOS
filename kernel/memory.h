#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"

#define PG_P_1 1        //页表项或页目录项存在属性位
#define PG_P_0 0
#define PG_RW_R 0       //R/W属性位值，读/执行
#define PG_RW_W 2       //读/写/执行
#define PG_US_S 0       //U/S属性位值,系统级
#define PG_US_U 4       //用户级

/* 虚拟地址池，用于虚拟地址管理 */
typedef struct virtual_addr
{
    bitmap vaddr_bitmap;        //虚拟地址用到的位图结构
    uint32_t vaddr_start;       //虚拟地址起始地址
}virtual_addr;

enum pool_flags {
    PF_KERNEL = 1,
    PF_USER = 2,
};


extern struct pool kernel_pool, user_pool;
void mem_init(void);
void* get_kernel_pages(uint32_t pg_cnt);
void* malloc_page(enum pool_flags pf, uint32_t pg_cnt);
uint32_t* pde_ptr(uint32_t vaddr);
uint32_t* pte_ptr(uint32_t vaddr);

#endif