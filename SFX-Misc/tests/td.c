// typedef-test

#include <stdio.h>

typedef char PARAM[20];

void main(void)
{
	PARAM a;

	printf("%d\n",sizeof(a));
}
