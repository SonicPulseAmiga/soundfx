/******************************************************************************
** ct.c                                                                      **
**---------------------------------------------------------------------------**
** generates a tab which tell what char is recognised by which is#? function **
**---------------------------------------------------------------------------**
** Project : <Projectname>                                                   **
** Version : V 0.0                                                           **
** Date    : Donnerstag 09-Jul-98                                            **
** Author  : Stefan Kost                                                     **
******************************************************************************/

//-- includes -----------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>

//-- prototypes ---------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- externals ----------------------------------------------------------------

//-- definitions --------------------------------------------------------------

void main(void)
{
	register short i;

	printf("      alnum alpha ascii cntrl digit graph lower print punct space upper xdigit\n");
	for(i=0;i<256;i++)
	{
		if(i>31) printf("%3d %c ",i,i);
		else printf("%3d   ",i);
		if(isalnum(i)) printf("  x   "); else printf("      ");
		if(isalpha(i)) printf("  x   "); else printf("      ");
		if(isascii(i)) printf("  x   "); else printf("      ");
		if(iscntrl(i)) printf("  x   "); else printf("      ");
		if(isdigit(i)) printf("  x   "); else printf("      ");
		if(isgraph(i)) printf("  x   "); else printf("      ");
		if(islower(i)) printf("  x   "); else printf("      ");
		if(isprint(i)) printf("  x   "); else printf("      ");
		if(ispunct(i)) printf("  x   "); else printf("      ");
		if(isspace(i)) printf("  x   "); else printf("      ");
		if(isupper(i)) printf("  x   "); else printf("      ");
		if(isxdigit(i)) printf("  x   \n"); else printf("      \n");
	}
}

//-- eof ----------------------------------------------------------------------
