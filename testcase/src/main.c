#include "../include/printf.h"

int fork()
{
	return 0;
}

void sleep(int t)
{
	while (1);
}

int main()
{
	printf("Now start the testcase!\n");
	if (fork()) {
		while (1) {
			printf("Parent: Ping!\n");
			sleep(100);
		}
	} else {
		while (1) {
			printf("Child: Pong!\n");
			sleep(100);
		}
	}
	return 0;
}
