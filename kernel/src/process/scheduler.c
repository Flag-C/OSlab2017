#include "env.h"

void schedule_process()
{
	int index, i, x;
	if (curenv)
		index = curenv - envs;
	else
		index = 0;
	for (i = 1; i <= NENV; i ++) {
		x = (index + i) % NENV;
		if (envs[x].env_status == ENV_NOT_RUNNABLE) {
			if (++ envs[x].env_runs == 0)
				envs[x].env_status = ENV_RUNNABLE;
		} else if (envs[x].env_status == ENV_RUNNABLE && x != 0) {
			envs[x].env_runs ++;
			env_run(&(envs[x]));
		}
	}
	envs[0].env_runs ++;
	env_run(&(envs[0]));
	printk("%s, %d: Kernel should not reach here!\n", __FUNCTION__, __LINE__);
}