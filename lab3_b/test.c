#include<stdio.h>

#include "hrtimer_x86.h"

int main(int argc, char const *argv[])
{
	double start,end,time;
	start=gethrtime_x86();
	end=gethrtime_x86();

	printf("%f\n %f\n",start,end );

	return 0;
}