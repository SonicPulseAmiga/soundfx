/****h* Datatypes/DataTypes_L.c [4.2] *
*
*  NAME
*    DataTypes_L.c
*  COPYRIGHT
*    $VER: DataTypes_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Datatype loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jun.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    29.Jun.1999	V 1.20	most recent version
*    19.Jan.1999	V 1.10	most recent version
*    02.Jun.1997	V 1.00	initial version
*  NOTES
*
*******
*/

/****u* _loaders/DataTypes_L *
*  Function.eng
*    Loads Sample-Files via AMIGA OS DataTypes. This loader would load every
*    sample, if you have a datatype for its format installed. You can try this,
*    when the Universal-Loader fails.<br>
*    The main disadvantag of the system shipped with OS3.x, is that it only
*    supports 8bit mono samples. Fortunately __SFX can use the extensions
*    introduced by the sounddt41 (which can be found on aminet).
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16)</td></tr>
*    </table>
*  Funktion.ger
*    Läd Sampledateien über die AMIGA OS Datentypen. Damit kann man jedes Sample
*    laden, für welches man den zugehörigen Datatype installiert hat. Sie können
*    diesen Loader versuchen, wenn der Universal-Loader fehlschlug.<br>
*    Der Hauptnachteil des dem OS3.x beiliegenden Systems, ist daß es nur 8bit
*    mono samples unterstützt. Glücklicherweise kann __SFX die Erweiterungen
*    des sounddt41 (welcher auf dem aminet zu finden ist) nutzen.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16)</td></tr>
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

#define Datatypes_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>
#include <datatypes/datatypes.h>
#include <datatypes/datatypesclass.h>
#include <datatypes/soundclass.h>
#include <datatypes/soundclassext.h>
#include <proto/datatypes.h>

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


struct Library	 			*DataTypesBase=NULL;

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(DataTypesBase	=OpenLibrary("datatypes.library",39)))							{ ADosError("datatypes.library >=V39.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; }
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(DataTypesBase)	CloseLibrary(DataTypesBase);
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
	struct VoiceHeader *v8h;
	register ULONG j;
	register UBYTE l;
	ULONG slen,curlen,format,bits;
	Object *Sound;
	APTR sbuf;
	BYTE *sbufb;
	WORD *sbufw;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;

	if(Sound=NewDTObject(pdata->fn,DTA_GroupID,GID_SOUND,TAG_DONE)) {
		if((GetDTAttrs(Sound,SDTA_Sample,&sbuf,SDTA_SampleLength,&slen,SDTA_VoiceHeader,&v8h,TAG_DONE)==3)) {
			if(!(GetDTAttrs(Sound,SDTA_SampleType,&format,TAG_DONE))) format=0;		// will require sounddt >=V41
			switch(format) {
				case SDTST_M8S:		bits= 8;sbufb=(BYTE *)sbuf;SET_CHANNELS(si,CH_MONO);break;
				case SDTST_S8S:		bits= 8;sbufb=(BYTE *)sbuf;SET_CHANNELS(si,CH_MONO);break;
				case SDTST_M16S:	bits=16;sbufw=(WORD *)sbuf;SET_CHANNELS(si,CH_STEREO);break;
				case SDTST_S16S:	bits=16;sbufw=(WORD *)sbuf;SET_CHANNELS(si,CH_STEREO);break;
				default:
					Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);
					DisposeDTObject(Sound);return(FALSE);
			}

			if((AllocSampleMem(si,slen,CH_COUNT(si->channels)))) {
				PWData * const pwd=NewProWin();

				si->zoomxl=si->slen=slen;
				if(v8h->vh_SamplesPerSec) si->srat=v8h->vh_SamplesPerSec;
				else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
				if(v8h->vh_RepeatHiSamples) {
					si->loop=SFXLOOP_FORWARD;
					si->loopl=v8h->vh_RepeatHiSamples;
					si->loops=v8h->vh_OneShotHiSamples;
				}
				if(pwd) {
					for(l=0;l<CH_COUNT(si->channels);l++) {
						SmpAccess * const dstsa=SAInit(si,l,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
						if(dstsa) {
							SAFuncPtr const SAWrite=dstsa->safunc;
							register SAMPLE *dbuf;
							ProWinTitle title;
							sprintf(title,GetString(msgProgressLoad),l+1,CH_COUNT(si->channels));
							StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
							switch(bits) {
								case 8:
									sc_fmt=STR_PCM8;
									for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (curlen<si->slen));dstsa->seg++) {
										dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
										for(j=0;((j<Bytes2Samples(dstsa->blen)) && (curlen<si->slen));j++,curlen++) {
											*(dbuf++)=((WORD)(*(sbufb++))<<8);
										}
										SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
									}
									break;
								case 16:
									sc_fmt=STR_PCM16;
									for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && (curlen<si->slen));dstsa->seg++) {
										dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
										for(j=0;((j<Bytes2Samples(dstsa->blen)) && (curlen<si->slen));j++,curlen++) {
											*(dbuf++)=*(sbufw++);
										}
										SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
									}
									break;
							}
							if(ProWinAborted(curlen)) l=CH_MAX;					 // es wurde abgebrochen
						}
						SADone(dstsa,si);					// Samplezugiffshandles freigeben
					}
					DelProWin(pwd);
				}
			}
			else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);DisposeDTObject(Sound);return(FALSE); }
		}
		else { Message(errSampleAttributes,NULL,__FILE__,__LINE__);DisposeDTObject(Sound);return(FALSE); }
		DisposeDTObject(Sound);
	}
	else { Message(errSampleObject,NULL,__FILE__,__LINE__);return(FALSE); }

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
