#include "semaphore.h"
#include "process.h"
#include "string.h"

semaphore sems[NR_SEM];
semaphore *sem_free_list = NULL;

void init_sem()
{
	int i;
	for (i = 0; i < NR_SEM; i ++) {
		sems[i].sem_val = 0;
		sems[i].sem_type = SEM_TYPE_FREE;
		sems[i].sem_blocklist = NULL;
		sems[i].sem_next = sems + i + 1;
	}
	sems[NR_SEM - 1].sem_next = NULL;
	sem_free_list = &sems[0];
}

static int sem_name2index(char *name)
{
	int i;
	for (i = 0; i < NR_SEM; i ++)
		if (sems[i].sem_type != SEM_TYPE_FREE)
			if (strcmp(sems[i].sem_name, name) == 0)
				return i;
	return -1;
}

bool sem_init(char *name, int val, int type)
{
	if (sem_name2index(name) != -1) {
		printk("Semaphore %s already exists!\n", name);
		return false;
	}
	if (sem_free_list == NULL) {
		printk("No space for semaphore %s! sem_open failed!\n", name);
		return false;
	}
	semaphore *s = sem_free_list;
	strcpy(s->sem_name, name);
	s->sem_val = val;
	s->sem_type = type;
	s->sem_blocklist = NULL;
	sem_free_list = s->sem_next;
	printk("Semaphore %s created!\n", name);
	return true;
}

bool sem_destory(char *name)
{
	int i = sem_name2index(name);
	if (i != -1) {
		sems[i].sem_type = SEM_TYPE_FREE;
		sems[i].sem_next = sem_free_list;
		sem_free_list = &(sems[i]);
		return true;
	} else {
		printk("Semaphore %s not found!\n", name);
		return false;
	}
}

bool sem_wait(char *name)
{
	int i = sem_name2index(name);
	if (i == -1) {
		printk("Semaphore %s not found!\n", name);
		return false;
	} else if (sems[i].sem_val > 0) {
		sems[i].sem_val --;
		return true;
	} else {
		curenv->env_status = ENV_NOT_RUNNABLE;
		curenv->env_link = sems[i].sem_blocklist;
		sems[i].sem_blocklist = curenv;
		schedule_process();
		return true;
	}
}

bool sem_post(char *name)
{
	int i = sem_name2index(name);
	if (i == -1) {
		printk("Semaphore %s not found!\n", name);
		return false;
	} else if (sems[i].sem_val > 0) {
		if (sems[i].sem_type == SEM_TYPE_GENERAL)
			sems[i].sem_val ++;
		return true;
	} else {
		if (sems[i].sem_blocklist == NULL) {
			sems[i].sem_val ++;
			return true;
		} else {
			struct Env *e = sems[i].sem_blocklist;
			e->env_status = ENV_RUNNABLE;
			sems[i].sem_blocklist = e->env_link;
			return true;
		}
	}
}
