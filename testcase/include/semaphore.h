enum SemaphoreType {
	SEM_TYPE_FREE = 0,
	SEM_TYPE_GENERAL,
	SEM_TYPE_BINARY
};

bool sem_init(char *name, int val, int type);
bool sem_destory(char *name);
bool sem_wait(char *name);
bool sem_post(char *name);
