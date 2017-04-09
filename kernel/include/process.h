#include "mmu.h"
#include "memory/memlayout.h"

#define KSTACK_SIZE 4096
#define NR_PCB 16

typedef struct PCB {
	struct TrapFrame tf;
	uint8_t kstack[KSTACK_SIZE];
	pde_t updir[NPDENTRIES] __attribute__((__aligned__(PGSIZE)));
} PCB;
