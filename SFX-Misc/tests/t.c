#include <stdio.h>

void main(void)
{
	short i,j;

	for(i=j=0;i<5;j=++i)
	{
		printf("i=%d  j=%d\n",i,j);
	}
}
