#include "../include/printf.h"
#include "../../include/types.h"
#include "../include/semaphore.h"
#include "../include/process.h"

#define MAX_N 6
int num_of_product = 0;
int buffer[MAX_N] = {0};
int current = 0;

void *producer()
{
	int i;
	while (1) {
		sem_wait("empty");
		sem_wait("mutex");
		buffer[current ++] = ++ num_of_product;
		printf("Producer works: ");
		for (i = 0; i < MAX_N; i ++)
			printf("%d\t", buffer[i]);
		printf("\n");
		sem_post("mutex");
		sem_post("full");
	}
}

void *consumer()
{
	int i;
	while (1) {
		sem_wait("full");
		sem_wait("mutex");
		buffer[-- current] = 0;
		printf("Consumer works: ");
		for (i = 0; i < MAX_N; i ++)
			printf("%d\t", buffer[i]);
		printf("\n");
		sem_post("mutex");
		sem_post("empty");
	}
}

int main()
{
	sem_init("mutex", 1, SEM_TYPE_BINARY);
	sem_init("full", 0, SEM_TYPE_GENERAL);
	sem_init("empty", MAX_N, SEM_TYPE_GENERAL);
	thread_create(producer);
	thread_create(consumer);
	while (1);
}
