#include "../include/printf.h"
#include "../include/process.h"

void sleep(int t)
{
	while (1);
}

int main0()
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

int main()
{
	int i;
	for (i = 0; i < 2; i ++) {
		if (fork())
			printf("[Parent: Ping!]\n");
		else
			printf("[Child: Pong!]\n");
	}
	while (1);
}
