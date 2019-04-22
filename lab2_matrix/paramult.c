#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#define NUM_THREADS 4


struct args
{
	int** a;
	int** b;
	int** c;
	int ira_a;
	int fra;
	int nn;
	int icb_a;
	int fcb;

};
void pmat(int** m, int r, int c){

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			printf("%d\t",m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void llenarmatrix(int** m, int r, int c){

	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			m[i][j]=rand()%5;
		}
	}
}

void *multpar(void *input){

	for( int ira=((struct args*)input)->ira_a ; ira < ((struct args*)input)->fra ; ++ira)
	{

		for ( int icb= ((struct args*)input)->icb_a ; icb < ((struct args*)input)->fcb; ++icb)
		{
			((struct args*)input)->c[ira][icb]=0;
			for (int k = 0; k < ((struct args*)input)->nn; ++k)
			{
				((struct args*)input)->c[ira][icb] = ((struct args*)input)->c[ira][icb] + ((struct args*)input)->a[ira][k]* ((struct args*)input)->b[k][icb];
			}
		}
	}
}


int main(int argc, char const *argv[])
{
	struct timeval start;
	struct timeval finish;
	long compTime;
	double Time;

	int rowsa,colsa,rowsb,colsb;

	time_t t;
	srand((unsigned) time(&t));

	do{

	printf("Ingrese medidas de la primera matriz \n");
	scanf("%d %d", &rowsa,&colsa);

	printf("Ingrese medidas de la segunda matriz \n");
	scanf("%d %d", &rowsb,&colsb);
	if (colsa!= rowsb)
	{
		printf("No se puede continuar con esas medidas. Intente de nuevo. \n");
	}

	}while(colsa!=rowsb);


	

	int** a = malloc(rowsa*sizeof(int*));
	int** b = malloc(rowsb*sizeof(int*));
	int** c = malloc(rowsa*sizeof(int*));
	

	for (int i = 0; i < rowsa; ++i)
	{
		a[i] = malloc(colsa*sizeof(int));
	}

	for (int i = 0; i < rowsb; ++i)
	{
		b[i] = malloc(colsb*sizeof(int));
	}

	for (int i = 0; i < rowsa; ++i)
	{
		c[i] = malloc(colsb*sizeof(int));
	}

	llenarmatrix(a,rowsa,colsa);
	llenarmatrix(b,rowsb,colsb);

	
	pthread_t threads[NUM_THREADS];
		
	
	int cant, casillas, pointer =0,endpointer;
	int cantidades[NUM_THREADS];





	gettimeofday(&start, 0);
	if (colsb>rowsa)
	{
		casillas=colsb;

		int divTemp = NUM_THREADS;

		for (int i = 0; i < NUM_THREADS; ++i)
		{
			cant = ((casillas)+(divTemp-1))/divTemp;
			cantidades[i]=cant;
			casillas = casillas - cant;

			if (divTemp>1)
			 {
			 	divTemp--;
			 }
			
		}


		for (int i = 0; i < NUM_THREADS; ++i)
		{
			if (cantidades[i])
			{
				endpointer= pointer+cantidades[i];

				struct args *data = (struct args*)malloc(sizeof(struct args));

				data->a=a;
				data->b=b;
				data->c=c;
				data->ira_a=0;
				data->fra=rowsa;
				data->nn=colsa;
				data->icb_a=pointer;
				data->fcb=endpointer;

				pthread_create(&threads[i], 0, multpar, (void*)data);

				pointer = pointer + cantidades[i];
				
			}
		}


	}else{

		casillas=rowsa;

		int divTemp = NUM_THREADS;

		for (int i = 0; i < NUM_THREADS; ++i)
		{
			cant = ((casillas)+(divTemp-1))/divTemp;
			cantidades[i]=cant;
			casillas = casillas - cant;

			if (divTemp>1)
			 {
			 	divTemp--;
			 }
			
		}


		for (int i = 0; i < NUM_THREADS; ++i)
		{
			if (cantidades[i])
			{
				endpointer= pointer+cantidades[i];

				struct args *data = (struct args*)malloc(sizeof(struct args));

				data->a=a;
				data->b=b;
				data->c=c;
				data->ira_a=pointer;
				data->fra=endpointer;
				data->nn=colsa;
				data->icb_a=0;
				data->fcb=colsb;

				pthread_create(&threads[i], 0, multpar, (void*)data);
				pointer = pointer + cantidades[i];
				
			}
		}

	}

	//---------------------------------------------------
	//-------------JOIN ALL THE THREADS------------------
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		pthread_join(threads[i],NULL);
	}
	gettimeofday(&finish, 0);
	
	//---------------------------------------------------


	compTime = (finish.tv_sec - start.tv_sec) * 1000000;
	compTime = compTime + (finish.tv_usec - start.tv_usec);
	Time = (double)compTime;

	//pmat(a,rowsa,colsa);
	//pmat(b,rowsb,colsb);
	//pmat(c,rowsa,colsb);
	printf("Application time: %f Micro secs\n",(double)Time);


	
	return 0;
}