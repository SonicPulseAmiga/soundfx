/****h* IFF-MAUD/IFF-MAUD_L.c [4.2] *
*
*  NAME
*    IFF-MAUD_L.c
*  COPYRIGHT
*    $VER: IFF-MAUD_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    IFF-MAUD loader for SoundFX
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

/****u* _loaders/IFF-MAUD_L *
*  Function.eng
*    Loads IFF-MAUD Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,A-LAW,µ-LAW)</td></tr>
*    </table>
*    This is an IFF-type format, which was introduced by MacroSystems (the producer
*    of the Toccata and Maestro-boards). Is supports multichannel samples, several
*    bit resolutions and compression.
*  Funktion.ger
*    Läd IFF-MAUD Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dies ist ein dem IFF-Standart entsprechendes Dateiformat,welches von der Firma
*    MacroSystems (die Hersteller der Toccata und Maestro-Karten) eingeführt wurde.
*    Diese Format unterstützt mehrkanalige Samples, unterschiedliche Bitauflösungen
*    und Kompression der Audiodaten.
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

#define IFF_MAUD_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-MAUD.h"

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
	struct MaudHeader mhdr={0};
	FILE *infile;
	LONG formsize,subtype,diff=0;
	UBYTE bits=8;
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
	if(subtype!=IFF_MAUD) { sprintf(errmsg,GetString(errWrongFiletype),"IFF_MAUD");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
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
				case IFF_MHDR:
					if(!fread(&mhdr,sizeof(mhdr),1,infile))	{ Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					//printf("\n\tMAUD\n");
					//printf("\t\tSamples       %5d\n",mhdr.Samples);			/* Slen */
					//printf("\t\tSampleSizeC   %5d\n",mhdr.SampleSizeC);		/* Bits */
					//printf("\t\tSampleSizeU   %5d\n",mhdr.SampleSizeU);		/* Bits */
					//printf("\t\tRateSource    %5d\n",mhdr.RateSource);		/* Rate */
					//printf("\t\tRateDevide    %5d\n",mhdr.RateDevide);		/* Ratenteteiler */
					//printf("\t\tChannelInfo   %5d\n",mhdr.ChannelInfo);		/* Channels */
					//printf("\t\tChannels      %5d\n",mhdr.Channels);		/* Channels */
					//printf("\t\tCompression   %5d\n",mhdr.Compression);		/* Compression */
					if(mhdr.RateSource && mhdr.RateDevide) si->srat=mhdr.RateSource/mhdr.RateDevide;
					else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
					bits=mhdr.SampleSizeU;
					if(mhdr.ChannelInfo!=MCI_MONO && mhdr.ChannelInfo!=MCI_STEREO) { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					else {
						if((mhdr.ChannelInfo==MCI_MONO && mhdr.Channels==1) || (mhdr.ChannelInfo==MCI_STEREO && mhdr.Channels==2)) {
							SET_CHANNELS(si,(mhdr.Channels-1));
						}
					}
					//if((mhdr.Compression!=COMPMAUD_NONE) && (mhdr.Compression!=COMPMAUD_FIBDELTA)) { Message(GetRunTime(mess[34]));fclose(infile);return(FALSE); }
					diff=ch.chunksize-sizeof(mhdr);
					break;
				case IFF_NAME:			// non-standard
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
				case IFF_COPY:			// non-standard
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
				case IFF_AUTH:			// non-standard
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
				case IFF_ANNO:			// non-standard
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
				case IFF_MDAT:
					ldata.offs=ftell(infile);
					ldata.chmask=CH_ALL;
					ldata.flags=LDF_NONE;
					switch(bits) {
						case 8:
							switch(mhdr.Compression) {
								case COMPMAUD_PCM:
									sc_fmt=STR_PCM8;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
									}
									break;
								case COMPMAUD_FDPCM:
									sc_fmt=STR_FDPCM8_4;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
							}
							break;
						case 16:
							switch(mhdr.Compression) {
								case COMPMAUD_PCM:
									sc_fmt=STR_PCM16;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
								case COMPMAUD_ULAW:
									sc_fmt=STR_ULAW;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
								case COMPMAUD_ALAW:
									sc_fmt=STR_ALAW;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
							}
							break;
						case 24:
							sc_fmt=STR_PCM24;
							switch(mhdr.Compression) {
								case COMPMAUD_PCM:
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
							}
							break;
						case 32:
							sc_fmt=STR_PCM32;
							switch(mhdr.Compression) {
								case COMPMAUD_PCM:
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREOI);break;
									}
									break;
							}
							break;
					}
					if(!si->slen) { fclose(infile);return(FALSE); }
					fseek(infile,ch.chunksize,SEEK_CUR);
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

//-- private Stuff

//-- eof ----------------------------------------------------------------------
