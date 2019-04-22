#include <pthread.h>
#define  NUM_HILOS 50000

void *printHola(void *num_hilo)
{
	long t;
	t = (long)num_hilo;
	printf("Hola Mundo desde el hilo %ld \n",t);
	pthread_exit(NULL);

}

int main()
{
	int i,j,k,rc;
	long t;

	pthread_t hilos[NUM_HILOS] ;

	for ( t = 0; t < NUM_HILOS; ++t)
	{
		rc= pthread_create(&hilos[t], NULL,printHola,(void*)t);
		if (rc)
		{
			printf("ERROR \n");
				exit(-1);
		}
		
	}


	pthread_exit(NULL);
	return 0;
}