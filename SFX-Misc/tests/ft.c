/******************************************************************************
** ft.c                                                                      **
**---------------------------------------------------------------------------**
** <Description>                                                             **
**---------------------------------------------------------------------------**
** Project : <Projectname>                                                   **
** Version : V 0.0                                                           **
** Date    : Montag 20-Jul-98                                                **
** Author  : Stefan Kost                                                     **
******************************************************************************/

//-- includes -----------------------------------------------------------------

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <dos/dos.h>

//-- prototypes ---------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- externals ----------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int main(void)
{
	int fh;
	long apos;
	char *data1="0123456789012345";
	char *data2="abcdefghijklmnop";

	if((fh=open("ram:test.txt",O_RDWR|O_CREAT))>-1)
	{
		if(write(fh,data1,15)<0) perror("ft/write");
		close(fh);
	}
	else { perror("ft/open");return(RETURN_WARN); }
	
	printf("1 Block is written !\n");getch();

	if((fh=open("ram:test.txt",O_RDWR|O_CREAT))>-1)
	{
		if((apos=lseek(fh,10,SEEK_SET))<0) perror("ft/lseek");
		else printf("aktuelle position : %ld\n",apos);
		if(write(fh,data2,15)<0) perror("ft/write");
		if((apos=lseek(fh,0,SEEK_CUR))<0) perror("ft/lseek");
		else printf("aktuelle position : %ld\n",apos);
		close(fh);
	}
	else { perror("ft/open");return(RETURN_WARN); }

	printf("2 Block is written !\n");

	return(RETURN_OK);
}

//-- eof ----------------------------------------------------------------------
