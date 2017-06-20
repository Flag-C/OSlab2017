#include "common.h"
#include "x86/x86.h"
#include "device/timer.h"
#include "process.h"
#include "env.h"

/* 8253输入频率为1.193182MHz */
#define TIMER_PORT 0x40
#define FREQ_8253 1193182

extern struct Env *curenv;
volatile int tick = 0;


void
init_timer(void)
{
	int counter = FREQ_8253 / HZ;
	assert(counter < 65536);
	out_byte(TIMER_PORT + 3, 0x34);
	out_byte(TIMER_PORT + 0, counter % 256);
	out_byte(TIMER_PORT + 0, counter / 256);
}

void
timer_event(void)
{
	tick ++;
	/*if (tick % 10 == 0) {
		curenv->env_status = ENV_RUNNABLE;
		schedule_process();
	}*/
}

int
get_tick()
{
	return tick;
}