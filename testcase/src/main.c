#include "../include/printf.h"
#include "../include/process.h"

int main()
{
	int i;
	printf("Now start the testcase!\n");
	if (fork()) {
		for (i = 0; i < 3; i ++) {
			printf("process %x:Parent: Ping!\n", getpid());
			sleep(3);
		}
	} else {
		for (i = 0; i < 3; i ++) {
			printf("process %x:Child: Pong!\n", getpid());
			sleep(1);
		}
	}
	exit(0);
}
