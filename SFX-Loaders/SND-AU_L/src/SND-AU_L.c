/****h* SND-AU/SND-AU_L.c [4.2] *
*
*  NAME
*    SND-AU_L.c
*  COPYRIGHT
*    $VER: SND-AU_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    SND-AU loader for SoundFX
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

/****u* _loaders/SND-AU_L *
*  Function.eng
*    Loads SND-AU samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
*    </table>
*    These samples come mainly from the SUN, NEXT or DEC computers or in common :
*    most UNIX-based machines are using this format. The format is pretty simple -
*    a small header followed by the sound data. In most cases these are µ-Law packed.
*  Funktion.ger
*    Läd SND-AU Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
*    </table>
*    Diese Samples stammen von SUN-, NEXT- oder DEC-Rechnern bzw. auf Rechnern
*    die unter UNIX arbeiten. Das Formate ist recht einfach aufgebaut - ein
*    einfacher Header und dann die Sounddaten. Diese sind meistens µ-Law gepackt.
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

#define SND_AU_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "SND-AU.h"

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

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	return(TRUE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	struct SunHeader shdr;
	UBYTE *tempbuf;
	FILE *infile;
	LONG namelen;
	UBYTE hdr;
	LoadData ldata;
	SInfo *si=pdata->dst[0];
	BOOL fmtcorr=FALSE;
	char *sc_fmt;

	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&shdr,sizeof(struct SunHeader),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	hdr=0;
	if(shdr.id==SUN__SND || shdr.id==SUN__DEC) hdr=1;
	if(shdr.id==SUN_ISND || shdr.id==SUN_IDEC) hdr=2;
	// DEBUG
	MSG1("loading AU-SND file : %s",pdata->fn);
	//MSG2("id : %x -> hdr : %d",shdr.id,hdr);
	// DEBUG
	if(!hdr) { sprintf(errmsg,GetString(errWrongFiletype),"SND-AU");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(hdr==2) {
		shdr.hdrlen=	LitEnd2BigEnd_ulong(shdr.hdrlen);
		shdr.datalen=	LitEnd2BigEnd_ulong(shdr.datalen);
		shdr.format=	LitEnd2BigEnd_ulong(shdr.format);
		shdr.rate=		LitEnd2BigEnd_ulong(shdr.rate);
		shdr.channels=	LitEnd2BigEnd_ulong(shdr.channels);
	}
	// DEBUG
	MSG1("  hdrlen   : %6d",shdr.hdrlen);
	MSG1("  datalen  : %6d",shdr.datalen);
	MSG1("  format   : %6d",shdr.format);
	MSG1("  rate     : %6d",shdr.rate);
	MSG1("  channels : %6d",shdr.channels);
	// DEBUG
	if(!shdr.datalen || shdr.datalen==SND_LENGHT_ILLEGAL) {
		fmtcorr=TRUE;
		shdr.datalen=GetFileLength(pdata->fn)-shdr.hdrlen;
	}
	if((shdr.channels!=1) && (shdr.channels!=2) && (shdr.channels!=4)) {
		ERR1("Channels : %d",shdr.channels);
		Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
	}
	else SET_CHANNELS(si,(shdr.channels-1));
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	switch(shdr.format) {
		case SND_FORMAT_MULAW_8:
		case SND_FORMAT_LINEAR_8:
		case SND_FORMAT_LINEAR_16:
		case SND_FORMAT_LINEAR_24:
		case SND_FORMAT_LINEAR_32:
		case SND_FORMAT_FLOAT:
		case SND_FORMAT_DOUBLE:
		case SND_FORMAT_ALAW_8:
			break;
		default:
			Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
			break;
	}
	// DEBUG
	MSG("getting name");
	// DEBUG
	namelen=shdr.hdrlen-sizeof(struct SunHeader);
	if(namelen>0) {
		if(tempbuf=(UBYTE *)AllocVec(namelen+1,MEMF_ANY)) {
			if(!fread(tempbuf,namelen,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			tempbuf[namelen]='\0';TrimString(tempbuf);
			si->title=StringCopy(tempbuf);
			FreeVec((ULONG *)tempbuf);
		}
		else fseek(infile,namelen,SEEK_CUR);
	}
	if(shdr.rate) si->srat=shdr.rate;
	else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
	// DEBUG
	MSG("header ready");
	// DEBUG
	ldata.offs=ftell(infile);
	ldata.chmask=CH_ALL;;
	ldata.flags=LDF_NONE;
	switch(shdr.format) {
		case SND_FORMAT_MULAW_8:
			sc_fmt=STR_ULAW;
			switch(si->channels) {
				case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ULAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case SND_FORMAT_LINEAR_8:
			sc_fmt=STR_PCM8;
			switch(si->channels) {
				case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM8,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM8,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM8,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case SND_FORMAT_LINEAR_16:
			sc_fmt=STR_PCM16;
			if(hdr==1) {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case SND_FORMAT_LINEAR_24:
			sc_fmt=STR_PCM24;
			if(hdr==1) {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case SND_FORMAT_LINEAR_32:
			sc_fmt=STR_PCM32;
			if(hdr==1) {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
					case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
					case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case SND_FORMAT_FLOAT:
			sc_fmt=STR_IEEE32;
			switch(si->channels) {
				case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE32,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE32,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE32,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case SND_FORMAT_DOUBLE:
			sc_fmt=STR_IEEE64;
			switch(si->channels) {
				case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE64,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE64,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_IEEE64,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case SND_FORMAT_ALAW_8:
			sc_fmt=STR_ALAW;
			switch(si->channels) {
				case CH_MONO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	si->slen=ReadData(si,&ldata,shdr.datalen,CRUNCH_ALAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
	}
	if(fmtcorr) { sprintf(errmsg,GetString(errDamagedFormat),pdata->fn);Message(errDamagedFormat,errmsg,__FILE__,__LINE__); }
	si->zoomxl=si->slen;
	fclose(infile);
	if(!si->slen) return(FALSE);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
}

void SaveSettings(struct Instance *instance,char *fname)
{ }

void SetDefaultSettings(struct Instance *instance) {
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
