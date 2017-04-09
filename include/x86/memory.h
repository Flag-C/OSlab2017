#ifndef __X86_MEMORY_H__
#define __X86_MEMORY_H__

#define DPL_KERNEL              0
#define DPL_USER                3

struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
};

struct TrapFrame {
	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t error_code;
	uint32_t eip, cs, eflags;
	uint32_t esp;
	uint32_t ss;
};

#endif
