#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define NUM_MAGOS 5


int palos[NUM_MAGOS];
	
pthread_mutex_t lock;

struct persona
{
	int id;
};

void * cena (void *arg){

	struct persona* mago;

	mago = (struct persona*) arg ;

	int id = mago->id;

	int cant_palos=0;
	int num_palos[2]={0,0};
	int comida = 20;
	
	while(comida){

		//BARRERAAAA----------------
		for (int i = 0; i < NUM_MAGOS; ++i)
		{
			
			if (cant_palos<2){
				pthread_mutex_lock(&lock);
				if (palos[i]==0)
				{

					num_palos[cant_palos]=i;
					palos[i]= id;
					cant_palos++;
				}
				pthread_mutex_unlock( &lock);		
			}
		}
		if (cant_palos==2)
		{
			comida--;
			sleep(1);
			printf("Comiendo filosofo %d\n", id);
		}else{
			printf("Hablando filosofo %d\n", id);
		}
		//BARRERAAAA----------------
		for (int i = 0; i < cant_palos; ++i)
		{
			palos[num_palos[i]]=0;
		}
		for (int i = 0; i < 2; ++i)
		{
			num_palos[i]=0;
		}
		cant_palos=0;
	}
	


}




int main(int argc, char const *argv[])
{	
	struct timeval start;
	struct timeval finish;
	long compTime;
	double Time;

	pthread_t magos[NUM_MAGOS];
	
	pthread_mutex_init(&lock,NULL);



	gettimeofday(&start, 0);

	for (int i = 0; i < NUM_MAGOS; ++i)
	{
		struct persona *data = (struct persona*)malloc(sizeof(struct persona));


		data->id=i;
		
		pthread_create(&magos[i],NULL,cena,(void*)data);

	}



	for (int i = 0; i < NUM_MAGOS; ++i)
	{
		pthread_join(magos[i],NULL);
	}
	
	gettimeofday(&finish, 0);


	compTime = (finish.tv_sec - start.tv_sec) * 1000000;
	compTime = compTime + (finish.tv_usec - start.tv_usec);
	Time = (double)compTime/1000000;

	printf("Tiempo de la cena %f\n", Time);
//power of the crowd
	return 0;
}