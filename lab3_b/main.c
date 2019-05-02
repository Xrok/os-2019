#include<stdio.h>
#include<pthread.h>


#include "functions.h"
#define NUM_THREADS 2

int cuenta=0;
pthread_mutex_t lock;

volatile unsigned long mutex;
ticket_lock_t mutex_ticket;

void*func(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		cuenta++;
	}

}

void*func_mutex(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		pthread_mutex_lock(&lock);
		cuenta++;
		pthread_mutex_unlock(&lock);
	}

}

void*func_tas(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		tas_lock(&mutex);
		cuenta++;
		unlock(&mutex);
	}

}

void*func_tatas(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		tatas_lock(&mutex);
		cuenta++;
		unlock(&mutex);
	}
}

void*func_tatas_backoff(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		tatas_lock_backoff(&mutex);
		cuenta++;
		unlock(&mutex);
	}
}
void*func_ticket(void* arg)
{
	for (int i = 0; i < 50000; ++i)
	{
		ticket_lock(&mutex_ticket);
		cuenta++;
		ticket_unlock(&mutex_ticket);
	}
}





int main(int argc, char const *argv[])
{
	pthread_t threads[NUM_THREADS];

	pthread_mutex_init(&lock,NULL);
	mutex=0;
	mutex_ticket.next_ticket=0;
	mutex_ticket.now_serving=0;


	for (int i = 0; i < NUM_THREADS; ++i)
	{
		pthread_create(&threads[i],NULL,func_ticket,NULL);
	}




	for (int i = 0; i < NUM_THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	printf("%d\n",cuenta );
	return 0;
}