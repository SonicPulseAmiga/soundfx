//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void main(void)
{
	char strBuffer[100];

	puts("Bitte geben Sie einen String ein : ");fflush(stdout);
	gets(strBuffer);
	printf("Die Zahl lautet : %lf\n",atof(strBuffer));
}
