/****h* IFF-16SV/IFF-16SV_L.c [4.2] *
*
*  NAME
*    IFF-16SV_L.c
*  COPYRIGHT
*    $VER: IFF-16SV_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    IFF-16SV loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jun.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*               			handles corrupt form- and chunksizes better
*    07.Mar.2001	V 4.1	most recent version
*							C : now better accepts broken samples
*    18.Jun.2000	V 1.2	most recent version
*    18.Sep.1998	V 1.1	most recent version
*    02.Jun.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/IFF-16SV_L *
*  Function.eng
*    Loads IFF-16SV Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-16,FDPCM-16:6,EDPCM-16:5)</td></tr>
*    </table>
*    Again I got the info on this format from Richard Körbners freeware	program <b>SoundBox</b>.
*    It is basically the 8SVX format,except that it carries the "16SV" mark and stores 16bit
*    sample data in the "BODY" chunk.
*  Funktion.ger
*    Läd IFF-16SV Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-16,FDPCM-16:6,EDPCM-16:5)</td></tr>
*    </table>
*    Dieses Format entnahm ich ebenfalls Richard Körbners Freewareprogramms <b>SoundBox</b>.
*    Es entspricht im Prinzip dem normalen 8SVX-Format, nur hat es die Kennung "16SV"
*    und im "BODY"-Chunk werden 16bit Samples gespeichert.
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

#define IFF_16SV_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-16SV.h"

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
	struct ChunkHeader ch;
	struct Voice8Header v8h={0};
	FILE *infile;
	LONG formsize,subtype,diff=0;
	ULONG chan;
	LoadData ldata;
	char *tempbuf;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;
	// DEBUG
	//ULONG slen;
	// DEBUG

	BOOL fmtcorr=FALSE,dataokay=FALSE;		/* support for mangled files (offset and length errors) */
	ULONG filesize=GetFileLength(pdata->fn);


	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(ch.TYPE!=IFF_FORM) { sprintf(errmsg,GetString(errWrongFiletype),"IFF");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(!fread(&subtype, sizeof(subtype),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	formsize=ch.chunksize-sizeof(subtype);
	//-- check for wrong formsize (bigger than file)
	if(formsize>filesize) {
		formsize=filesize-(sizeof(struct ChunkHeader)+sizeof(subtype));
		fmtcorr=TRUE;
	}
	if(subtype!=IFF_16SV) { sprintf(errmsg,GetString(errWrongFiletype),"IFF-16SV");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	while(formsize>0) {		// as long as there is anything left to read
		if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) {
			if(dataokay) { fmtcorr=TRUE;formsize=diff=0; }
			else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
		}
		if(diff) {
			if(!ValidChunkID(ch.STRTYPE)) {
				diff-=sizeof(struct ChunkHeader);
				fseek(infile,diff,SEEK_CUR);
				if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				if(!ValidChunkID(ch.STRTYPE)) formsize=0;	// seem to be totally scrambled
			}
			fmtcorr=TRUE;
			diff=0;
		}
		// DEBUG
		MSG5("Chunk : %c%c%c%c %6ld",ch.STRTYPE[0],ch.STRTYPE[1],ch.STRTYPE[2],ch.STRTYPE[3],ch.chunksize);
		// DEBUG
		formsize-=sizeof(struct ChunkHeader);
		//-- check for wrong chunksize (bigger than remaining file)
		if(ch.chunksize>formsize) {
			ch.chunksize=formsize;
			fmtcorr=TRUE;
		}
		if(formsize) {
			switch(ch.TYPE) {												/* je nach Chunktyp */
				case IFF_VHDR:
					if(!fread(&v8h,sizeof(v8h),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);	}
					// DEBUG
					//slen=v8h.oneShotHiSamples;
					//if(v8h.repeatHiSamples) slen+=v8h.repeatHiSamples;
					// DEBUG
					if(v8h.samplesPerSec) si->srat=v8h.samplesPerSec;
					else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
					if(v8h.repeatHiSamples) {
						si->loop=SFXLOOP_FORWARD;
						si->loopl=v8h.repeatHiSamples;
						si->loops=v8h.oneShotHiSamples;
					}
					diff=ch.chunksize-sizeof(v8h);
					break;
				case IFF_NAME:
					if((tempbuf=(UBYTE *)AllocVec(ch.chunksize+1,MEMF_ANY))) {
						if(!fread(tempbuf,ch.chunksize,1,infile)) {
							FreeVec((APTR)tempbuf);
							if(dataokay) { fmtcorr=TRUE;formsize=0; }
							else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
						}
						else {
							tempbuf[ch.chunksize]=0;TrimString(tempbuf);
							si->title=StringCopy(tempbuf);
							FreeVec((APTR)tempbuf);
						}
					}
					else fseek(infile,ch.chunksize,SEEK_CUR);
					break;
				case IFF_COPY:
					if(si->cpyr) FreeVec((ULONG *)si->cpyr);
					if((tempbuf=(UBYTE *)AllocVec(ch.chunksize+1,MEMF_ANY))) {
						if(!fread(tempbuf,ch.chunksize,1,infile)) {
							FreeVec((APTR)tempbuf);
							if(dataokay) { fmtcorr=TRUE;formsize=0; }
							else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
						}
						else {
							tempbuf[ch.chunksize]=0;TrimString(tempbuf);
							si->cpyr=StringCopy(tempbuf);
							FreeVec((APTR)tempbuf);
						}
					}
					else fseek(infile,ch.chunksize,SEEK_CUR);
					break;
				case IFF_AUTH:
					if(si->auth) FreeVec((ULONG *)si->auth);
					if((tempbuf=(UBYTE *)AllocVec(ch.chunksize+1,MEMF_ANY))) {
						if(!fread(tempbuf,ch.chunksize,1,infile)) {
							FreeVec((APTR)tempbuf);
							if(dataokay) { fmtcorr=TRUE;formsize=0; }
							else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
						}
						else {
							tempbuf[ch.chunksize]=0;TrimString(tempbuf);
							si->auth=StringCopy(tempbuf);
							FreeVec((APTR)tempbuf);
						}
					}
					else fseek(infile,ch.chunksize,SEEK_CUR);
					break;
				case IFF_ANNO:
					if(si->anno) FreeVec((ULONG *)si->anno);
					if((tempbuf=(UBYTE *)AllocVec(ch.chunksize+1,MEMF_ANY))) {
						if(!fread(tempbuf,ch.chunksize,1,infile)) {
							FreeVec((APTR)tempbuf);
							if(dataokay) { fmtcorr=TRUE;formsize=0; }
							else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
						}
						else {
							tempbuf[ch.chunksize]=0;TrimString(tempbuf);
							si->anno=StringCopy(tempbuf);
							FreeVec((APTR)tempbuf);
						}
					}
					else fseek(infile,ch.chunksize,SEEK_CUR);
					break;
				case IFF_CHAN:
					if(!fread(&chan,4,1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						if(chan== 6) SET_CHANNELS(si,CH_STEREO);
						if(chan==30) SET_CHANNELS(si,CH_QUADRO);
						diff=ch.chunksize-4;
					}
					break;
				case IFF_BODY:
					ldata.offs=ftell(infile);
					ldata.chmask=CH_ALL;
					ldata.flags=LDF_NONE;
					switch(v8h.sCompression) {
						case COMP16SV_PCM:
							sc_fmt=STR_PCM16;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);break;
							}
							break;
						case COMP16SV_FDPCM:
							sc_fmt=STR_FDPCM16_6;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_STEREO);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_QUADRO);break;
							}
							break;
						case COMP16SV_EDPCM:
							sc_fmt=STR_EDPCM16_5;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_STEREO);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_QUADRO);break;
							}
							break;
					}
					if(!si->slen) { fclose(infile);return(FALSE); }
					fseek(infile,ch.chunksize,SEEK_CUR);
					si->zoomxl=si->slen;
					//MSG2("hdr.slen=%ld  data.slen=%ld",slen,si->slen);
					dataokay=TRUE;
					break;
				default:
					fseek(infile,ch.chunksize,SEEK_CUR);
					break;
			}
			formsize-=ch.chunksize;
			if(ch.chunksize&1) { formsize--;fseek(infile,1L,SEEK_CUR); }			/* odd-length chunk */
		}
	}
	if(fmtcorr) { sprintf(errmsg,GetString(errDamagedFormat),pdata->fn);Message(errDamagedFormat,errmsg,__FILE__,__LINE__); }
	fclose(infile);

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
