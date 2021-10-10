#include <stdio.h>

void main(void)
{
	char *str="123456789abcdefghijklmnopqrstuvwxyz",*sptr;
	long *ptr=(long *)str;

	printf("0x%08lx\n",str);
	printf("0x%08lx <%s>\n\n",ptr,ptr);

	sptr=(ptr=(char *)ptr+3,(char *)ptr-3);printf("0x%08lx <%s>\n",sptr,sptr);
	sptr=(ptr=(char *)ptr+3,(char *)ptr-3);printf("0x%08lx <%s>\n",sptr,sptr);
	sptr=(ptr=(char *)ptr+3,(char *)ptr-3);printf("0x%08lx <%s>\n",sptr,sptr);
	(char *)ptr='#';printf("0x%08lx <%s>\n",str,str);
}
