/****h* IFF-AIFF/IFF-AIFF_L.c [4.2] *
*
*  NAME
*    IFF-AIFF_L.c
*  COPYRIGHT
*    $VER: IFF-AIFF_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    IFF-AIFF loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*               			handles corrupt form- and chunksizes better
*    07.Mar.2001	V 4.1	most recent version
*							C : now better accepts broken samples
*    18.Jun.2000	V 1.2	most recent version
*    13.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/IFF-AIFF_L *
*  Function.eng
*    Loads IFF-AIFF Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    You will find this fileformat mainly on Apple-Macintosh computers. The AIFF
*    format supports multichannel samples and several bit resolutions.
*  Funktion.ger
*    Läd IFF-AIFF Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    Dieses Dateiformat findet man überwiegend auf AppleMacintosh-Rechnern. Das
*    AIFF-Format unterstützt mehrkanalige Samples und unterschiedliche Bitauflösungen.
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

#define IFF_AIFF_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-AIFF.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

static LONG FromIEEE(UBYTE *bytes);

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
	struct ChunkHeader ch;
	struct CommonChunk cohdr;
	struct SampledSoundHeader sshdr;
	FILE *infile;
	LONG formsize,subtype,diff=0;
	UBYTE bits=8;
	ULONG slen;
	LoadData ldata;
	char *tempbuf;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;

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
	if(subtype!=IFF_AIFF) { sprintf(errmsg,GetString(errWrongFiletype),"IFF-AIFF");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE);}
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
		if(formsize>0) {
			switch(ch.TYPE) {												/* je nach Chunktyp */
				case IFF_COMM:
					if(!fread(&cohdr,sizeof(cohdr),1,infile))	{ Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					MSG1("      Channels      %5d",cohdr.numChannels);
					MSG1("      SampleFrames  %5d",cohdr.numSampleFrames);		/* Slen */
					MSG1("      SampleSize    %5d",cohdr.sampleSize);			/* Bits */
					MSG1("      SampleRate    %5d",FromIEEE(cohdr.sampleRate));	/* Rate */
					si->srat=FromIEEE(cohdr.sampleRate);
					if(!si->srat) { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
					if(cohdr.sampleSize<9) bits=8;
					else if(cohdr.sampleSize<17) bits=16;
					else if(cohdr.sampleSize<25) bits=24;
					else if(cohdr.sampleSize<33) bits=32;
					else {
						ERR1("Bits : %d",cohdr.sampleSize);
						Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					//si->zoomxl=si->slen=cohdr.numSampleFrames;
					if(cohdr.numChannels!=1 && cohdr.numChannels!=2 && cohdr.numChannels!=4) {
						ERR1("Channels : %d",cohdr.numChannels);
						Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					else SET_CHANNELS(si,(cohdr.numChannels-1));
					diff=ch.chunksize-sizeof(cohdr);
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
				case IFF_SSND:
					if(!fread(&sshdr,sizeof(sshdr),1,infile))	{ Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					if(sshdr.offset) {
						//printf("\n offset : %d\n",sshdr.offset);
						fseek(infile,sshdr.offset,SEEK_CUR);
					}
					slen=ch.chunksize-(sizeof(sshdr)+sshdr.offset);
					ldata.offs=ftell(infile);
					ldata.chmask=CH_ALL;
					ldata.flags=LDF_NONE;
					switch(bits) {
						case 8:
							sc_fmt=STR_PCM8;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM8,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM8,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM8,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case 16:
							sc_fmt=STR_PCM16;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case 24:
							sc_fmt=STR_PCM24;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM24,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case 32:
							sc_fmt=STR_PCM32;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_PCM32,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
					}
					if(!si->slen) { fclose(infile);return(FALSE); }
					fseek(infile,slen,SEEK_CUR);
					si->zoomxl=si->slen;
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

static LONG FromIEEE(UBYTE *bytes) {
	signed int	exp;
	LONG		hiMant;
	LONG		ret;

	exp=bytes[1];
	hiMant=(LONG)(bytes[2]<<8)|(LONG)(bytes[3]);
	if(exp==0 && hiMant==0) ret=0;
	else { exp=1<<(14-exp);ret=hiMant/exp; }
	return(ret);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
