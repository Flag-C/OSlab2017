#include "env.h"

#define NR_SEM 32

enum SemaphoreType {
	SEM_TYPE_FREE = 0,
	SEM_TYPE_GENERAL,
	SEM_TYPE_BINARY
};

typedef struct Semaphore {
	char sem_name[32];
	int sem_val;
	unsigned sem_type;
	struct Env *sem_blocklist;
	struct Semaphore *sem_next;
} semaphore;

semaphore sems[NR_SEM];
extern semaphore *sem_free_list;

bool sem_open(char *name, int val, int type);
bool sem_close(char *name);
bool sem_wait(char *name);
bool sem_post(char *name);
