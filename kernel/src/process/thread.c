#define TH_DEBUG
#include "mmu.h"
#include "string.h"
#include "env.h"
#include "memory/pmap.h"

static struct Env *find_thread_root(struct Env *penv)
{
	if (penv->env_parent_id == -1 || (penv->env_parent_id >> 8) != (penv->env_id >> 8))
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
#ifdef TH_DEBUG
	printk("\n%s, %d: creating thread(envid = 0x%x)...\n", __FUNCTION__, __LINE__, curenv->env_id);
#endif
	struct Env *penv;
	struct Env *parent = find_thread_root(curenv);
	env_alloc(&penv, curenv->env_id);
	penv->env_id = (curenv->env_id & 0xffffff00) + parent->env_nr_thread;
#ifdef TH_DEBUG
	printk("e->env_id: 0x%x)\n", penv->env_id);
#endif
	penv->env_tf = curenv->env_tf;
	penv->env_tf.eip = (uintptr_t)start;
	penv->env_pgdir = curenv->env_pgdir;
	penv->env_tf.esp = USTACKTOP - (parent->env_nr_thread << 17);
	parent->env_nr_thread ++;
}
