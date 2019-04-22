#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>

void not(void){};

int main(){
	double time,timef,times,timec;
	pid_t id;
	long compTime;
	struct timeval start;
	struct timeval finish;
	
	int op[2];
	int p = pipe(op);
for(int i =0;i<20;i++){

	gettimeofday(&start,0);
	not();//function
	gettimeofday(&finish,0);
	compTime = (finish.tv_sec - start.tv_sec)*1000000;
	compTime = compTime + (finish.tv_usec-start.tv_usec);
	time = (double)compTime;
	timef=timef+time;

	gettimeofday(&start,0);
	id = getpid();//syscall
	gettimeofday(&finish,0);
	compTime = (finish.tv_sec - start.tv_sec)*1000000;

	compTime = (finish.tv_usec-start.tv_usec);
	time = (double)compTime;
	times=times+time;

	int child = fork();

	gettimeofday(&start,0);

	if(!child){
		write(op[1],"a",1);
		exit(0);	
	}else if(child>0){
		write(op[1],"b",1);
		
		wait(NULL);
	}else{
		printf("No se pudo crear el hijo");
	}
	gettimeofday(&finish,0);
	compTime = (finish.tv_sec - start.tv_sec)*1000000;

	compTime = (finish.tv_usec-start.tv_usec);
	time = (double)compTime;
	timec=timec+time;
}


	printf("Time of calling a function: %f microseconds \n",timef/20);
	printf("Time of calling a syscall: %f microseconds \n",times/20);
	printf("Time of making a context swicth: %f microseconds \n",timec/20);




	return 0;
}
