/****h* Maestro/Maestro_L.c [4.2] *
*
*  NAME
*    Maestro_L.c
*  COPYRIGHT
*    $VER: Maestro_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Maestro loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    13.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/Maestro_L *
*  Function.eng
*    Loads Maestro Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16)</td></tr>
*    </table>
*    This failrly simple format is saved by the Samplitude Software. The Loader
*    is in an experimental state, because I lack of information about this format.
*  Funktion.ger
*    Läd Maestro Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16)</td></tr>
*    </table>
*    Dieses recht einfache Format wird von der Samplitude Software gespeichert.
*    Dieser Loader befindet sich in einem experimentellen Zustand, da ich
*    leider keine Dokumentation zu diesem Format besitze.
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

#define IFF_Maestro_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Maestro.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

//-- Menu ID's

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

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	return(TRUE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	FILE *infile;
	LoadData ldata;
	char id[8];
	struct MaestroHeader shdr;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;

	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&id,8,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(stricmp(id,"MAESTRO")) { sprintf(errmsg,GetString(errWrongFiletype),"Maestro");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(!fread(&shdr,sizeof(shdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	
	if(shdr.rate) si->srat=shdr.rate;
	else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
	//si->slen=shdr.length;
	if((shdr.format&CHANNEL_MASK)!=shdr.format) {
		ERR1("Channels : %d",shdr.format);
		Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
	}
	else SET_CHANNELS(si,shdr.format);

	ldata.offs=ftell(infile);
	ldata.chmask=CH_ALL;
	ldata.flags=LDF_NONE;
	fclose(infile);
	switch(shdr.format)	{
		case 0: sc_fmt=STR_PCM16;si->slen=ReadData(si,&ldata,(si->slen<<2),CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
		case 1: sc_fmt=STR_PCM16;si->slen=ReadData(si,&ldata,(si->slen<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
		case 2: sc_fmt=STR_PCM8; si->slen=ReadData(si,&ldata,(si->slen<<1),CRUNCH_PCM8 ,CONV(0,0),CHANNEL_STEREOI);break;
		case 3: sc_fmt=STR_PCM8; si->slen=ReadData(si,&ldata, si->slen    ,CRUNCH_PCM8 ,CONV(0,0),CHANNEL_MONO);break;
	}
	if(!si->slen) return(FALSE);
	si->zoomxl=si->slen;

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
}

void SaveSettings(struct Instance *instance,char *fname) {
}

void SetDefaultSettings(struct Instance *instance) {
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
