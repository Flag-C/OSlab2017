#include "../include/printf.h"
#include "../../include/types.h"
#include "../include/semaphore.h"
#include "../include/process.h"

int main_folk()
{
	int i;
	printf("Now start the testcase!\n");
	if (fork()) {
		for (i = 0; i < 5; i ++) {
			printf("process %x:Parent: Ping!\n", getpid());
			sleep(3);
		}
	} else {
		for (i = 0; i < 5; i ++) {
			printf("process %x:Child: Pong!\n", getpid());
			sleep(1);
		}
	}
	exit(0);
}

int main()
{
	sem_open("mdzz", 2, SEM_TYPE_GENERAL);
	printf("%s, %d: hehe!\n", __FUNCTION__, __LINE__);
	sem_wait("mdzz");
	printf("%s, %d: hehe!\n", __FUNCTION__, __LINE__);
	sem_wait("mdzz");
	printf("%s, %d: hehe!\n", __FUNCTION__, __LINE__);
	sem_wait("mdzz");
	printf("%s, %d: hehe!\n", __FUNCTION__, __LINE__);
	printf("haha");

}