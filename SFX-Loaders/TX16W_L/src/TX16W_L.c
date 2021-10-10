/****h* TX16W/TX16W_L.c [4.2] *
*
*  NAME
*    TX16W_L.c
*  COPYRIGHT
*    $VER: TX16W_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    TX16W loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.20	most recent version
*    13.Sep.1998	V 1.10	most recent version
*    02.Jul.1997	V 1.00	initial version
*  NOTES
*
*******
*/

/****u* _loaders/TX16W_L *
*  Function.eng
*    Loads samples from the Yamaha TX16W.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>no (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-12)</td></tr>
*    </table>
*    These samples are always 12-bit, are limited in length to 262144 samples
*    (attack- and sustainpart) and supporting only three different rates
*    (16 kHz, 33 kHz, 50 kHz).
*  Funktion.ger
*    Läd Samples vom Yamaha TX16W.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>nein (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>nein (PCM-12)</td></tr>
*    </table>
*    Diese Samples sind immer 12-bit, in Ihrer Länge begrenzt auf 262144 Samples
*    (Attack- und Sustainpart) and unterstützen nur drei verschiedene Raten
*    (16 kHz, 33 kHz, 50 kHz).
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

#define TX16W_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "TX16W.h"

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
	struct SampleHeader shdr;
	FILE *infile;
	char id[6];
	ULONG len1,len2,slen;
	LoadData ldata;
	SInfo *si=pdata->dst[0];

	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&id,6,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(strnicmp(id,"LM8953",6)) { sprintf(errmsg,GetString(errWrongFiletype),"TX16W");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(!fread(&shdr, sizeof(shdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;

	// DEBUG
	//printf("rate : %ld\n",(ULONG)shdr.sample_rate);
	// DEBUG
	switch(shdr.sample_rate) {
		case RATE_16000: si->srat=16000;break;
		case RATE_33000: si->srat=33000;break;
		case RATE_50000: si->srat=50000;break;
		default:
			if((shdr.atc_length[2]&0xFE)==RATE_16000_0 && (shdr.rpt_length[2]&0xFE)==RATE_16000_1) si->srat=16000;
			else if((shdr.atc_length[2]&0xFE)==RATE_33000_0 && (shdr.rpt_length[2]&0xFE)==RATE_33000_1) si->srat=33000;
			else if((shdr.atc_length[2]&0xFE)==RATE_50000_0 && (shdr.rpt_length[2]&0xFE)==RATE_50000_1) si->srat=50000;
			else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=33000; }
			break;
	}
	len1=((ULONG)shdr.atc_length[0])|(((ULONG)shdr.atc_length[1])<<8)|((((ULONG)shdr.atc_length[2])&0x1)<<16);
	len2=((ULONG)shdr.rpt_length[0])|(((ULONG)shdr.rpt_length[1])<<8)|((((ULONG)shdr.rpt_length[2])&0x1)<<16);
	// DEBUG
	//kprintf("len1 : %ld\n",len1);
	//kprintf("len2 : %ld\n",len2);
	// DEBUG
	slen=len1;
	if(shdr.format==FORMAT_LOOPED) {
		si->loop=SFXLOOP_FORWARD;
		si->loops=len1;
		si->loopl=len2;
		slen+=len2;
	}
	
	ldata.offs=ftell(infile);
	ldata.chmask=CH_ALL;
	ldata.flags=LDF_NONE;
	fclose(infile);
	si->zoomxl=si->slen=ReadData(si,&ldata,(ULONG)(slen*1.5),CRUNCH_PCM12,CONV(0,0),CHANNEL_MONO);
	if(!si->slen) return(FALSE);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM12);

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
