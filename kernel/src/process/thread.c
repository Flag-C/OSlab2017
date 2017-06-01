#define TH_DEBUG
#include "mmu.h"
#include "string.h"
#include "env.h"
#include "memory/pmap.h"

static struct Env *find_thread_root(struct Env *penv)
{
	if (penv->env_parent_id == 0)
		return penv;
	else {
		struct Env *parent;
		if (envid2env(penv->env_parent_id, &parent, false))
			printk("%s, %d: envid2env error!\n", __FUNCTION__, __LINE__);
		return find_thread_root(parent);
	}
}

void sys_create_thread(void *(*start)(void *))
{
	struct Env *penv;
#ifdef TH_DEBUG
	printk("\n%s, %d: creating thread(envid = 0x%x)...\n", __FUNCTION__, __LINE__, curenv->env_id);
#endif
	env_alloc(&penv, curenv->env_id);
	penv->env_tf = curenv->env_tf;
	penv->env_tf.eip = (uintptr_t)start;
	penv->env_pgdir = curenv->env_pgdir;
	struct Env *parent = find_thread_root(curenv);
	penv->env_tf.esp = USTACKTOP - (parent->env_nr_thread << 17);
	parent->env_nr_thread ++;
}
