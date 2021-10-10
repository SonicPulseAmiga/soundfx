/****h* htmldoctools/htmlinfo.c [1.00] *
*
*  NAME
*    htmlinfo.c
*  COPYRIGHT
*    $VER: htmlinfo.c 1.00 (07.05.00) © by Stefan Kost 2000-2000
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    07.May.2000
*  MODIFICATION HISTORY
*    07.May.2000	V 1.00	most recent version
*    07.May.2000	V 1.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

//-- Ansi

#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef AMIGA
#include <cmacros.h>
#endif

//-- Local

#define PRJ_SUBNAME		"htmlinfo"
#include "project.h"
#include "config.h"

//-- DebugTools

//#ifdef AMIGA
//#  define DEBUG_SRCPATH "data:source/sfx/sfx-main/html/"
//#  include <debug/dt_decl.h>
//#endif

//-- prototypes ---------------------------------------------------------------

#if defined(AMIGA) && defined(__SASC)
char *_stroserror(int errornum);
#endif
void Error(char *fmt,...);

char ReadHTMLTag(char *buffer,FILE *htmlf);

//-- defines ------------------------------------------------------------------

#define RETURN_OK		0
#define RETURN_WARN		1
#define RETURN_ERROR   -1
#define FALSE			0
#define TRUE			1

#ifdef HAVE_STRCASECMP
#define mystricmp strcasecmp
#endif
#ifdef HAVE_STRICMP
#define mystricmp stricmp
#endif

//-- globals ------------------------------------------------------------------

#ifdef AMIGA
char *VersTag="\0$VER:"PRJ_VERS;
#endif

static const char *ARG_TEMPLATE="<file> <tag> <opt>";
enum ArgsEnum {
  ARGID_FILE=1,
  ARGID_TAG,
  ARGID_OPT,
  ARG_COUNT
};

//-- definitions --------------------------------------------------------------

#if defined(AMIGA) && defined(__SASC)
char *stroserror(int errornum)
{
  static char *deferr="unknown",*err;
  long i=0;
  
  err=deferr;
  while(i!=-1 && __os_errlist[i].errnum) {
	if(__os_errlist[i].errnum==errornum) {
	  err=__os_errlist[i].msg;i=-1;
	}
	else i++;
  }
  return(err);
}
#endif

void Error(char *fmt,...)
{
  va_list arglist;
  char str[256];
  int C_Error=errno;
#ifdef AMIGA
  long IOError=IoErr();
#ifdef __SASC
  int OSError=_OSERR;
#endif
#endif

  va_start(arglist,fmt);
  printf(PRJ_NAME" : ");
  vprintf(fmt,arglist);
  if(C_Error) printf("\terrno : %s\n",strerror(C_Error));
#ifdef AMIGA
#ifdef __SASC
  if(OSError) printf("\toserr : %s\n",stroserror(OSError));
#endif
  if(IOError) { Fault(IOError,"\tioerr ",str,255);printf("%s\n",str); }
#endif
  va_end(arglist);
}

char ReadHTMLTag(char *buffer,FILE *htmlf)
{
  char boolRet=FALSE;
  int c;
	
  while(!feof(htmlf)) {
	c=getc(htmlf);
	if((char)c=='<') {								/* begin of tag found */
	  c=getc(htmlf);
	  while(((char)c!='>') && !feof(htmlf)) {				/* read until end of tag */
		*(buffer++)=(char)c;
		c=getc(htmlf);
	  }
	  *buffer='\0';
	  if(!feof(htmlf)) { boolRet=TRUE;break; }
	}
  }
  return(boolRet);
}

int main(int argc,char *argv[])
{
  int numRetVal=RETURN_OK,i;
  char cline[256],dline[256],c,*clineptr,*dlineptr;
  FILE *htmlf;
  char optQuoteM4=FALSE;
  
  if(argc>2) {
	if(argc>ARGID_OPT) {
	  for(i=ARGID_OPT;i<argc;i++) {
//		printf("option : %s",argv[i]);
		if(!mystricmp(argv[i],"QUOTEM4")) { optQuoteM4=TRUE;/*puts("  enabled");*/ }
	  }
	}
// DEBUG
//	printf("processing ..\n");
// DEBUG
	if((htmlf=fopen(argv[ARGID_FILE],"rt"))) {
	  if(!mystricmp(argv[ARGID_TAG],"TITLE")) {
		while(!feof(htmlf)) {
		  if(ReadHTMLTag(cline,htmlf)) {							/* read next html-tag */
// DEBUG
//			printf(":: %s\n",cline);
// DEBUG
			if(!mystricmp(cline,"TITLE")) {
			  clineptr=cline;
			  c=getc(htmlf);
			  while(((char)c!='<') && !feof(htmlf)) {				/* read until begin of next tag */
				*(clineptr++)=(char)c;
				c=getc(htmlf);
			  }
			  *clineptr='\0';
			  if(!feof(htmlf)) {
				if(optQuoteM4) {
				  clineptr=cline;dlineptr=dline;
				  while(*clineptr) {
					if((*clineptr=='#') || (*clineptr==',')) {
					  *(dlineptr++)='`';
					  *(dlineptr++)=*(clineptr++);
					  *(dlineptr++)='\'';
					}
					else {
					  *(dlineptr++)=*(clineptr++);
					}
				  }
				  *dlineptr='\0';
				  printf(dline);
				}
				else printf(cline);
				break;
			  }
			}
		  }
		}
	  }
	  else { numRetVal=RETURN_ERROR; }			// invalid Tag
	  fclose(htmlf);
	}
	else { Error("open input file : %s\n",argv[ARGID_FILE]);numRetVal=RETURN_ERROR; }
  }
  else { fprintf(stderr,"usage : "PRJ_SUBNAME" %s\n",ARG_TEMPLATE);numRetVal=RETURN_WARN; }
  
  return(numRetVal);
}

//-- eof ----------------------------------------------------------------------
