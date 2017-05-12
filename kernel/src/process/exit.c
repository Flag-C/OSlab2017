#include "process.h"

void sys_exit(int stat)
{
	printk("%s, %d: env(envid = 0x%x) exits with parameter %d\n", __FUNCTION__, __LINE__, curenv->env_id, stat);
	env_free(curenv);
	schedule_process();
}
