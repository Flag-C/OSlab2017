#include "../../include/types.h"

bool sem_open(char *name, int val, int type)
{
	bool ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(13), "b"(name), "c"(val), "d"(type));
	return ret;
}

bool sem_close(char *name)
{
	bool ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(14), "b"(name));
	return ret;
}

bool sem_wait(char *name)
{
	bool ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(15), "b"(name));
	return ret;
}

bool sem_post(char *name)
{
	bool ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(16), "b"(name));
	return ret;
}
