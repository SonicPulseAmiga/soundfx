#include <math.h>
#include <stdio.h>
#include <time.h>
#include <m68881.h>

#define mypow(x,y) exp(y*log(x))

#define ITER 500000

void main(void)
{
	register unsigned long i;
	time_t t1;
	double d1=1.1,d2;

	printf("rechne ...\n");
	d2=0.0;
	t1=time(0l);
	for(i=0;i<ITER;i++)
	{
		d2+=pow((double)i,d1);
		d2-=pow((double)i,d1);
	}
	t1=time(0l)-t1;
	printf("t = %ld\n",t1,d2);
	printf("rechne ...\n");
	d2=0.0;
	t1=time(NULL);
	for(i=0;i<ITER;i++)
	{
		d2+=mypow((double)i,d1);
		d2-=mypow((double)i,d1);
	}
	t1=time(NULL)-t1;
	printf("t = %ld\n",t1,d2);
}
