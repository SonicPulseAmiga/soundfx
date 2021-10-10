/****h* htmldoctools/prevnext.c [1.00] *
*
*  NAME
*    prevnext.c
*  COPYRIGHT
*    $VER: prevnext.c 1.00 (07.05.00) © by Stefan Kost 2000-2000
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    07.May.2000
*  MODIFICATION HISTORY
*    07.May.2000	V 1.00	most recent version
*    07.May.2000	V 0.00	initial version
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

#define PRJ_SUBNAME		"prevnext"
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

static const char *ARG_TEMPLATE="[-p] [-n] <index> <node>";

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

int main(int argc,char *argv[])
{
  extern char *optarg;
  extern int optind;
  int c;
  int numRetVal=RETURN_OK;
  char cline[256],oline[256];
  signed char mode=0,err=0;
  FILE *ixf;

  while ((c = getopt(argc, argv, "pn")) != EOF) {
	switch (c) {
	case 'p':
	  if(!mode) mode=-1;
	  else err=1;
	  break;
	case 'n':
	  if(!mode) mode= 1;
	  else err=1;
	  break;
	case '?':
	  err=1;break;
	}
  }
  if(argc-optind<2) err=1;
  if(err) { fprintf(stderr,"usage : "PRJ_SUBNAME" %s\n",ARG_TEMPLATE);numRetVal=RETURN_WARN; }
  else {
	if((ixf=fopen((char *)argv[optind],"rt"))) {
	  if(mode==-1) {
		fgets(cline,256,ixf);cline[strlen(cline)-1]='\0';
		strcpy(oline,cline);
		while(!feof(ixf)) {
		  fgets(cline,256,ixf);cline[strlen(cline)-1]='\0';
		  if(!strcmp(cline,(char *)argv[optind+1])) { printf(oline);break; }
		  else strcpy(oline,cline);
		}
	  }
	  if(mode==1) {
		while(!feof(ixf)) {
		  fgets(cline,256,ixf);cline[strlen(cline)-1]='\0';
		  if(!strcmp(cline,(char *)argv[optind+1])) {
			fgets(oline,256,ixf);cline[strlen(oline)-1]='\0';
			printf(oline);break;
		  }
		}
	  }
	  fclose(ixf);
	}
	else { Error("open index file : %s\n",argv[optind]);numRetVal=RETURN_ERROR; }
  }
  return(numRetVal);
}

//-- eof ----------------------------------------------------------------------
