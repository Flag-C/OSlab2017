#include "process.h"

int sys_getpid()
{
	return curenv->env_id;
}
