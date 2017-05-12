#include "mmu.h"
#include "string.h"
#include "env.h"
#include "memory/pmap.h"

static void fork_store_pgdir(struct Env *penv)
{
	int i, j;
	pte_t *pte;
	for (i = 0; i < 1024; i ++) {
		if ((curenv->env_pgdir[i] & PTE_P) && !(penv->env_pgdir[i] & PTE_P)) {
			pte = (pte_t *)(KADDR(curenv->env_pgdir[i] & 0xfffff000));
			for (j = 0; j < 1024; j ++) {
				if (pte[j] & PTE_P) {
					struct PageInfo *pp = page_alloc(ALLOC_ZERO);
					page_insert(penv->env_pgdir, pp, (void *)((i << 22) + (j << 12)), pte[j] & 0xfff);
					memcpy((void *)page2kva(pp), (void *)page2kva(pa2page(pte[j])), PGSIZE);
				}
			}
		}
	}
}

int sys_fork()
{
	struct Env *penv;
	printk("\n%s, %d: Start forking(envid = 0x%x)...\n", __FUNCTION__, __LINE__, curenv->env_id);
	int eid = env_alloc(&penv, curenv->env_id);
	penv->env_tf = curenv->env_tf;
	penv->env_tf.eax = 0;
	curenv->env_tf.eax = eid;
	fork_store_pgdir(penv);
	return eid;
}
