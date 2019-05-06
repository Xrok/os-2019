#include<stdio.h>
#include<pthread.h>
#include"hrtimer_x86.h"
#include"functions.h"

#define NUM_THREAD 2 //2 4 8
#define TIMES 10 //10 100 10000 10000

volatile unsigned long lock;

//pthread_barrier_t mutex;


void reverse_barrier(int cant){

	static int counter = 0;
	static int flag = 0;
	int local_sense = flag;

	local_sense = !local_sense;

	tatas_lock_backoff(&lock);

	counter++;

	if (counter== cant)
	{
		unlock(&lock);
		counter=0;
		flag=local_sense;
	}else{
		unlock(&lock);
		while(flag != local_sense){};
	}
}

void local_barrier(int cant){
	int static counter = 0;
	int static flag = 0;
	int mycount =0;

	tatas_lock_backoff(&lock);
	if (counter==0)
	{
		flag=0;
	}
	mycount = ++counter;
	unlock(&lock);

	if (mycount==cant)
	{
		counter=0;
		flag=1;
	}
	while(flag==0){}
}

void* func_pthread(void* arg)
{
	int* id = (int*)arg;

	for (int i = 0; i < TIMES; ++i)
	{
		pthread_barrier_wait(&mutex);;
	}
}

void* func_reverse(void* arg)
{
	int* id = (int*)arg;

	for (int i = 0; i < TIMES; ++i)
	{
		reverse_barrier(NUM_THREAD);
	}
}

void* func_local(void* arg)
{
	int* id = (int*)arg;

	for (int i = 0; i < TIMES; ++i)
	{
		local_barrier(NUM_THREAD);
		
	}
}



int main(int argc, char const *argv[])
{
	double end,start;

	pthread_t threads[NUM_THREAD];
	//pthread_barrier_init(&mutex,NULL,NUM_THREAD);
	lock=0;
	int ids[NUM_THREAD];

	start=gethrtime_x86();

	for (int i = 0; i < NUM_THREAD; ++i)
	{
		ids[i]=i;
		pthread_create(&threads[i],NULL,func_local,&ids[i]);
	}

	end=gethrtime_x86();

	for (int i = 0; i < NUM_THREAD; ++i)
	{
		pthread_join(threads[i],NULL);
	}

	printf("%f\n",end-start);
	
	return 0;
}