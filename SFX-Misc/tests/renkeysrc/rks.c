// renkeysrc.c

#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void makelinkfor(char *pre,char *post)
{
	char cmdln[256];
	int res;

	sprintf(cmdln,"copy TO %s%c-%s FROM %s%s_",pre,65+rand()%26,post,pre,post);
	if(res=system(cmdln)) printf("cmd <%s> failed %d\n",cmdln,res);
}

void main(void)
{
	srand(time(NULL));

	makelinkfor("SFX-","CheckKey1.c");
	makelinkfor("SFX-","CheckKey2.c");
	makelinkfor("SFX-","CheckKey3.c");
	makelinkfor("SFX-","CheckKey4.c");

	makelinkfor("SFX-","CheckOldKey1.c");
	makelinkfor("SFX-","CheckOldKey2.c");
	makelinkfor("SFX-","CheckSum1.c");

	makelinkfor("SFX-","CheckKeyContents.c");
	makelinkfor("SFX-","CheckKeyList.c");
}
