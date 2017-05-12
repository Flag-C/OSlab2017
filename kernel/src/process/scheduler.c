#include "env.h"

void schedule_process()
{
	int index, i;
	if (curenv) {
		index = curenv - envs;
		for (i = index + 1; i < NENV; i ++)
			if (envs[i].env_status == ENV_RUNNABLE) {
				envs[i].env_runs ++;
				env_run(&(envs[i]));
			}
		for (i = 0; i < index; i ++)
			if (envs[i].env_status == ENV_RUNNABLE) {
				envs[i].env_runs ++;
				env_run(&(envs[i]));
			}
	}
}
