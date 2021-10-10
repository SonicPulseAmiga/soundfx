/****h* SoundFX/SFX-MRU.c [4.3] *
*
*  NAME
*    SFX-MRU.c
*  COPYRIGHT
*    $VER: SFX-MRU.c 4.3 (14.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    MostRecentUsed files  - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Sep.2000
*  MODIFICATION HISTORY
*    14.Oct.2003	V 4.3	most recent version
*							C: don't add 'sfx_record'
*    23.May.2002	V 4.2	most recent version
*    02.Oct.2000	V 4.1	most recent version
*    11.Sep.2000	V 4.1	initial version
*  NOTES
*
*******
*/

#define SFX_MRU_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

BOOL	MRU_Init(void);
void	MRU_Done(void);
void	MRU_Add(STRPTR fn);
STRPTR	MRU_GetFileNode(UBYTE nr);
STRPTR	MRU_GetFileName(UBYTE nr);

//-- globals ------------------------------------------------------------------

APTR mrucfg=NULL;
char MRU_MenuStr[5][FILENAME_MAX];

//-- definitions --------------------------------------------------------------

BOOL MRU_Init(void) {
	BOOL ret=TRUE;
	register UBYTE i;
	STRPTR name;
	
	if(mrucfg=CfgIO_NewConfig("SoundFX-MRU"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		CfgIO_ReadConfig(mrucfg,"progdir:data/mru.cfg");
		for(i=0;i<5;i++) {
			/** @TODO check for non-existing files here ?
			 removable media: we can use Info() to see if there is a disk in a drive
			*/
			if(name=MRU_GetFileNode(i+1)) strcpy(MRU_MenuStr[i],name);
			else MRU_MenuStr[i][0]='\0';
		}
	}
	else ret=FALSE;
	return(ret);
}

void MRU_Done(void) {
	if(mrucfg) {
		CfgIO_WriteConfig(mrucfg,"progdir:data/mru.cfg");
		CfgIO_RemConfig(mrucfg);
	}
}

void MRU_Add(STRPTR fn) {
	register UBYTE i;
	char fnode[FILENAME_MAX],fname[FILENAME_MAX];
	STRPTR name;
	UBYTE found=0;

	// should we check for non-existent files too ?
	if(IsStringEmpty(fn)) return;

	stcgfn(fnode,fn);						// get file node part from fullname
	if(!strcmp(fnode,"sfx_record")) return;	// don't add if name is "sfx_record"

	strcpy(fname,fn);						// copy it, so that following calls to MRU_GetFileName() not detroying it
	for(i=1;i<=5;i++) {						// test, if it is already in menu
		if(!strcmp(fname,MRU_GetFileName(i))) {
			found=i;
			break;
		}
	}
	if(found!=1) {							// if it's not the alread at the first position
		if(!found) found=5;					// now shift entries to make space for the new one
		if(found>4) {
			CfgIO_WriteString(mrucfg,"MRU5","FileNode",CfgIO_ReadString(mrucfg,"MRU4","FileNode",""));
			CfgIO_WriteString(mrucfg,"MRU5","FileName",CfgIO_ReadString(mrucfg,"MRU4","FileName",""));
		}
		if(found>3) {
			CfgIO_WriteString(mrucfg,"MRU4","FileNode",CfgIO_ReadString(mrucfg,"MRU3","FileNode",""));
			CfgIO_WriteString(mrucfg,"MRU4","FileName",CfgIO_ReadString(mrucfg,"MRU3","FileName",""));
		}
		if(found>2) {
			CfgIO_WriteString(mrucfg,"MRU3","FileNode",CfgIO_ReadString(mrucfg,"MRU2","FileNode",""));
			CfgIO_WriteString(mrucfg,"MRU3","FileName",CfgIO_ReadString(mrucfg,"MRU2","FileName",""));
		}
		if(found>1) {
			CfgIO_WriteString(mrucfg,"MRU2","FileNode",CfgIO_ReadString(mrucfg,"MRU1","FileNode",""));
			CfgIO_WriteString(mrucfg,"MRU2","FileName",CfgIO_ReadString(mrucfg,"MRU1","FileName",""));
		}
		CfgIO_WriteString(mrucfg,"MRU1","FileNode",fnode);
		CfgIO_WriteString(mrucfg,"MRU1","FileName",fname);

		for(i=0;i<5;i++) {		// rebuild menu
			if(name=MRU_GetFileNode(i+1)) strcpy(MRU_MenuStr[i],name);
			else MRU_MenuStr[i][0]='\0';
		}
		if(!LayoutMenus(RunTime.Menu,RunTime.VisualInfo,GTMN_TextAttr,RunTime.ScrFont,TAG_DONE)) Message(errLayoutMenus,NULL,__FILE__,__LINE__);
		Refr_GlobalMRUMenu();
	}
	OUTRO;
}

STRPTR	MRU_GetFileNode(UBYTE nr) {
	static char res[FILENAME_MAX];
	char sect[10];

	sprintf(sect,"MRU%1d",nr);
	strcpy(res,CfgIO_ReadString(mrucfg,sect,"FileNode",""));
	return(res);
}

STRPTR	MRU_GetFileName(UBYTE nr) {
	static char res[FILENAME_MAX];
	char sect[10];

	sprintf(sect,"MRU%1d",nr);
	strcpy(res,CfgIO_ReadString(mrucfg,sect,"FileName",""));
	return(res);
}

//-- eof ----------------------------------------------------------------------
