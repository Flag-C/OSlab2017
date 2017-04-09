#ifndef __X86_CPU_H__
#define __X86_CPU_H__

#include "common.h"

typedef union CR0 {
	struct {
		uint32_t protect_enable      : 1;
		uint32_t monitor_coprocessor : 1;
		uint32_t emulation           : 1;
		uint32_t task_switched       : 1;
		uint32_t extension_type      : 1;
		uint32_t numeric_error       : 1;
		uint32_t pad0                : 10;
		uint32_t write_protect       : 1;
		uint32_t pad1                : 1;
		uint32_t alignment_mask      : 1;
		uint32_t pad2                : 10;
		uint32_t no_write_through    : 1;
		uint32_t cache_disable       : 1;
		uint32_t paging              : 1;
	};
	uint32_t val;
} CR0;

/* the Control Register 3 (physical address of page directory) */
typedef union CR3 {
	struct {
		uint32_t pad0                : 3;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t pad1                : 7;
		uint32_t page_directory_base : 20;
	};
	uint32_t val;
} CR3;

/* 将CPU置入休眠状态直到下次中断到来 */
static inline void
wait_for_interrupt()
{
	asm volatile("hlt");
}

/* 修改IDRT */
static inline void
save_idt(void *addr, uint32_t size)
{
	static volatile uint16_t data[3];
	data[0] = size - 1;
	data[1] = (uint32_t)addr;
	data[2] = ((uint32_t)addr) >> 16;
	asm volatile("lidt (%0)" : : "r"(data));
}

/* 打开外部中断 */
static inline void
enable_interrupt(void)
{
	asm volatile("sti");
}

/* 关闭外部中断 */
static inline void
disable_interrupt(void)
{
	asm volatile("cli");
}

static __inline void
hlt(void)
{
	asm volatile("hlt");
}
#define NR_IRQ    256

#endif
