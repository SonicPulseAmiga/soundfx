// assigntest.c

#include <stdio.h>

#include <exec/types.h>
#include <dos/dos.h>

#include <proto/dos.h>

extern struct DosLibrary	*DOSBase;

void main(void)
{
	BPTR lock;

	if((lock=Lock("progdir:fonts",ACCESS_READ)))
	{
		if(AssignAdd("fonts",lock))
		{
			printf("Assign added\n");
			getch();
			if(!RemAssignList("fonts",lock)) printf("Can't remove assign\n");
			
		}
		else { printf("Can't add assign\n");UnLock(lock); }
	}
	else printf("Can't lock progdir:fonts\n");
}
