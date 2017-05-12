#include "env.h"
#include "process.h"

void sys_sleep(int t)
{
	if (t <= 0)
		return;
	else {
		curenv->env_status = ENV_NOT_RUNNABLE;
		curenv->env_runs = -t;
		schedule_process();
	}
}
