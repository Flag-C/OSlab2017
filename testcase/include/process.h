int fork();
void sleep(int);
void exit(int);
void thread_create(void*(*start_rtn)(void*));