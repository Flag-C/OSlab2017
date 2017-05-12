#include "../include/printf.h"
#include "../include/process.h"

int main()
{
	int i;
	printf("Now start the testcase!\n");
	if (fork()) {
		for (i = 0; i < 3; i ++) {
			printf("Parent: Ping!\n");
			sleep(3);
		}
	} else {
		for (i = 0; i < 3; i ++) {
			printf("Child: Pong!\n");
			sleep(1);
		}
	}
	exit(0);
}
