/****h* sfxsupport.library/misc.c [4.2] *
*
*  NAME
*    misc.c
*  COPYRIGHT
*    $VER: misc.c 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    misc routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Jul.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*							C: no use of amigaguide.libary, asl.library, diskfont.library anymore
*							C: doesn't *requires* locale.library, openurl.library anymore, just uses them if available
*    27.Aug.2000	V 4.1	most recent version
*    02.Aug.2000	V 4.0	most recent version
*    01.Jul.2000	V 1.3	most recent version
*    20.Sep.1998	V 1.2	most recent version
*    15.Jul.1998	V 1.2	initial version
*  NOTES
*
*******
*/

#define MISC_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"
//-- DEBUG
//#include <proto/casasupport.h>
//-- DEBUG

//-- prototypes ---------------------------------------------------------------

//-- DataInitialisation

long			ASM LIB_EXPORT(ServerDataInit(REG(a0,RTime *RunTime_)));
void			ASM LIB_EXPORT(ServerDataDone(void));

//-- SampleListVerwaltung

SInfo *			ASM LIB_EXPORT(GetBufferPtr(REG(d0,ULONG id),REG(d1,WORD nr)));
UBYTE  			ASM LIB_EXPORT(GetBufferNr(REG(d0,ULONG id)));

//-- Version testing

ULONG			ASM LIB_EXPORT(GetSFXVersion(void));

//-- Miscellaneous

double			INLINE bound(double val,double lower,double upper);

//-- private Stuff

//-- globals ------------------------------------------------------------------

RTime 	*RunTime=NULL;
char sfxmsg[256];
struct Library *AHIBase=NULL;
ULONG msgid2hlpid[strSoundFXMessage];	// 'strSoundFXMessage' is the first id after the messages
UBYTE os_version;

// set some params for init code

extern long __oslibversion=38;

//-- Library-Basisaddrs.

extern struct ExecBase	*SysBase;
struct Library	 		*ConfigIOBase=NULL;
struct DosLibrary		*DOSBase=NULL;
struct Library	 		*GadToolsBase=NULL;
struct GfxBase	 		*GfxBase=NULL;
struct IntuitionBase	*IntuitionBase=NULL;
struct Library			*ListToolsBase=NULL;
struct LocaleBase		*LocaleBase=NULL;
struct Library			*OpenURLBase=NULL;
struct Library			*UnitConvBase=NULL;

//-- DEBUG
//struct CSBase *CSBase;
//-- DEBUG


//-- Locale

struct Catalog *sfxcat=NULL;

//-- definitions --------------------------------------------------------------

//-- DataInitialisation

long SAVEDS ASM LIB_EXPORT(ServerDataInit(REG(a0,RTime *RunTime_))) {
	if(!RunTime) {
		RunTime=RunTime_;
		GetRunTime(os_version)=os_version;
		return(TRUE);
	}
	else return(FALSE);
}

void SAVEDS ASM LIB_EXPORT(ServerDataDone(void)) {
	RunTime=NULL;
}

//-- SampleListVerwaltung

SInfo * SAVEDS ASM LIB_EXPORT(GetBufferPtr(REG(d0,ULONG id),REG(d1,WORD nr))) {
	UBYTE found=0,i=0;
	SInfo *si=(SInfo *)GetRunTime(blist)->lh_Head;

	if(nr==-1) {
		while(!EndOfList(GetRunTime(blist),(struct Node *)si) && !found) {
			if(si->id==id) found=1;
			else si=(SInfo *)(((struct Node *)si)->ln_Succ);
		}
		if(!found) si=0l;
	}
	else {
		while(!EndOfList(GetRunTime(blist),(struct Node *)si) && i<nr) {
			si=(SInfo *)(((struct Node *)si)->ln_Succ);
			i++;
		}
		if(i<nr) si=0l;
	}
	return(si);
}

//-- GetBufferIndex
UBYTE SAVEDS ASM LIB_EXPORT(GetBufferNr(REG(d0,ULONG id))) {
	UBYTE found=0,i=0;
	SInfo *si=(SInfo *)GetRunTime(blist)->lh_Head;

	while(!EndOfList(GetRunTime(blist),(struct Node *)si) && !found) {
		if(si->id==id) found=1;
		else { si=(SInfo *)(((struct Node *)si)->ln_Succ);i++; }
	}
	return(i);
}

//-- Version testing

ULONG SAVEDS ASM LIB_EXPORT(GetSFXVersion(void)) {
	return(MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION));
}

//-- Miscellaneous

double INLINE bound(double val,double lower,double upper) {
	if		(val>upper) return(upper);
	else if	(val<lower) return(lower);
	return(val);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
