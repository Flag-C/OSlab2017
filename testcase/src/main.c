#include "../include/printf.h"
#include "../include/process.h"

int main()
{
	printf("Now start the testcase!\n");
	if (fork()) {
		while (1) {
			printf("Parent: Ping!\n");
			sleep(2);
		}
	} else {
		while (1) {
			printf("Child: Pong!\n");
			sleep(1);
		}
	}
	return 0;
}
