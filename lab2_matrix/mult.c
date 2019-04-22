#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUM_THREADS 4


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
			//m[i][j]=rand()%5;
			m[i][j]=1;
		}
	}
}

void mult(int** a, int** b, int** c, int ra, int ca,int rb, int cb){

	for (int i = 0; i <ra ; ++i)
	{
		for (int j = 0; j < cb; ++j)
		{
			c[i][j]=0;
			for (int k = 0; k < ca; ++k)
			{
				c[i][j] = c[i][j] + a[i][k]*b[k][j];
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




	time_t t;
	srand((unsigned) time(&t));

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

	//pmat(a,rowsa,colsa);
	//pmat(b,rowsb,colsb);

	//---------------------------------------------------
	gettimeofday(&start, 0);	
	mult(a,b,c,rowsa,colsa,rowsb,colsb);
	gettimeofday(&finish, 0);

	//---------------------------------------------------

	//pmat(c,rowsa,colsb);
	compTime = (finish.tv_sec - start.tv_sec) * 1000000;
	compTime = compTime + (finish.tv_usec - start.tv_usec);
	Time = (double)compTime;

	printf("Application time: %f Micro secs\n",(double)Time);


	
	return 0;
}