/****h* FutureSound/FutureSound_L.c [4.2] *
*
*  NAME
*    FutureSound_L.c
*  COPYRIGHT
*    $VER: FutureSound_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    FutureSound loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jun.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    18.Sep.1998	V 1.1	most recent version
*    02.Jun.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/FutureSound_L *
*  Function.eng
*    Loads FutureSound files. The FutureSound format is a very old format featuring
*    little. Basically it is a RAW sample with a small chunk of data in front of
*    it, in which length and sampling rate are stored.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>no (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-8)</td></tr>
*    </table>
*  Funktion.ger
*    Läd FutureSound Dateien. Das FutureSound Format ist ein sehr altes Format mit
*    wenig Möglichkeiten (und wenig Bedeutung). Im Prinzip ist es ein RAW-Sample
*    mit einem winzigen Datenblock davor, indem die Länge und die Samplingrate steht.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>nein (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>nein (PCM-8)</td></tr>
*    </table>
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define FutureSound_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	UBYTE dummy;
};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

SFXMod_DoClientDataInit
SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance))
{
	return(TRUE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata))
{
	FILE *infile;
	UWORD rate;
	ULONG slen;
	LoadData ldata;
	SInfo *si=pdata->dst[0];

	if((si->slen=GetFileLength(pdata->fn)-6)>0) {
		if(infile=fopen(pdata->fn,"rb")) {
			strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
			if(!fread(&slen,4,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(slen!=si->slen) { sprintf(errmsg,GetString(errWrongFiletype),"FutureSound");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(!fread(&rate,2,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			si->srat=(ULONG)rate;
			if(!si->srat) { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
			si->zoomxl=si->slen;
			ldata.offs=ftell(infile);
			ldata.chmask=CH_ALL;
			ldata.flags=LDF_NONE;
			si->slen=ReadData(si,&ldata,si->slen,CRUNCH_PCM8,CONV(0,0),CHANNEL_MONO);
			if(!si->slen) return(FALSE);
			fclose(infile);
		}
		else { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	}
	else { StdCError(pdata->fn,errAccessFile,errno,__FILE__,__LINE__);return(FALSE); }

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM8);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret))
{
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname)
{
}

void SaveSettings(struct Instance *instance,char *fname)
{
}

void SetDefaultSettings(struct Instance *instance)
{
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
