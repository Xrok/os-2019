#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define NUM_MAGOS 5


int palos[NUM_MAGOS];
int comida_acum = 0;
int alimento = 20;
int total_alimento = 20*5;//alimento*cant_magos
	
pthread_mutex_t lock;
pthread_barrier_t barrera;

void set_total_to_0(){
	comida_acum =0;
}

struct persona
{
	int id;
};


void * cena (void *arg){
	set_total_to_0();

	struct persona* mago;

	mago = (struct persona*) arg ;

	int id = mago->id;

	int cant_palos=0;
	int num_palos[2]={0,0};
	int comida = alimento;

	while(1){
		pthread_barrier_wait(&barrera);
		if (comida_acum==total_alimento)
		{
			break;
		}

		for (int i = 0; i < NUM_MAGOS; ++i)
		{
			if (comida)
			{
				if (cant_palos<2)
				{
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
		}
		if (cant_palos==2)
		{
			comida--;
			pthread_mutex_lock(&lock);
			comida_acum++;
			pthread_mutex_unlock( &lock);

			sleep(4);
			printf("Comiendo filosofo %d\n", id);
		}else{
			sleep(4);
			printf("Hablando filosofo %d\n", id);
		}
		pthread_barrier_wait(&barrera);
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
	double tiempo_prom=0;

	pthread_t magos[NUM_MAGOS];
	
	pthread_mutex_init(&lock,NULL);

	pthread_barrier_init(&barrera,NULL,NUM_MAGOS);


for (int j = 0; j < 20; ++j)
{
	
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
	tiempo_prom=tiempo_prom+Time;
	printf("Tiempo de la cena N# %d: %f\n", j+1,Time);

}

printf("Tiempo en promedio de la cena: %f\n",tiempo_prom/20 );


	return 0;
}
