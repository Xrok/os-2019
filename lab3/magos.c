#include <stdio.h>
#include <pthread.h>
#define NUM_MAGOS 5

struct mesa
{
	int mago;
	int** palo;
	int** plato;

};



void * cena (void *arg){


}





int main(int argc, char const *argv[])
{	
	pthread_t magos[NUM_MAGOS];

	//int palo[5];
	//int plato[5];
	
	pthread_mutex_t lock;

	int** palo = malloc(5*sizeof(int*));
	int** plato = malloc(5*sizeof(int*));
	

	for (int i = 0; i < 5; ++i)
	{
		palo[i] = malloc(2*sizeof(int));
		plato[i] = malloc(2*sizeof(int));
	}


	


	for (int i = 0; i < NUM_MAGOS; ++i)
	{
		mesa *data = malloc(sizeof(mesa));

		data->palo=palo;
		data->plato=plato;
		data->mago=i;
		
		pthread_created(magos[i],NULL,cena,&data);
	}


//power of the crowd
	return 0;
}