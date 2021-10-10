/****h* IFF-AIFC/IFF-AIFC_L.c [4.2] *
*
*  NAME
*    IFF-AIFC_L.c
*  COPYRIGHT
*    $VER: IFF-AIFC_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    IFF-AIFC loader for SoundFX
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

/****u* _loaders/IFF-AIFC_L *
*  Function.eng
*    Loads IFF-AIFC Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    You will find this fileformat mainly on Apple-Macintosh computers. The AIFC
*    format is an extension of the AIFF format. It now supports multichannel samples,
*    several bit resolutions and compression.
*  Funktion.ger
*    Läd IFF-AIFC Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dieses Dateiformat findet man überwiegend auf AppleMacintosh-Rechnern. Das
*    AIFC-Format stellt eine Erweiterung des AIFF-Formates dar. Es unterstützt
*    mehrkanalige Samples, unterschiedliche Bitauflösungen und Kompression.
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

#define IFF_AIFC_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-AIFC.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

static LONG FromIEEE(UBYTE *bytes);

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
	register UWORD i,j;
	struct ChunkHeader ch;
	struct ExtCommonChunk extcohdr;
	struct SampledSoundHeader sshdr;
	struct Marker *marker=0l;
	struct Instrument inst;
	FILE *infile;
	LONG formsize,subtype,diff=0;
	UBYTE bits=8;
	ULONG slen,temp_l;
	UWORD anzm;
	UBYTE mlen;
	char mname[258],*tempbuf;
	LoadData ldata;
	UBYTE binst=0,bmark=0,mf1,mf2;
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
	if(subtype!=IFF_AIFC) { sprintf(errmsg,GetString(errWrongFiletype),"IFF-AIFC");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE);}
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	while(formsize>0) {		// as long as there is anything left to read
		if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) {
			if(dataokay) { fmtcorr=TRUE;formsize=sizeof(struct ChunkHeader); }
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
					if(!fread(&extcohdr,sizeof(extcohdr),1,infile))	{ Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					MSG1("\t\tChannels      %5ld",extcohdr.numChannels);				/* Channels */
					MSG1("\t\tSampleFrames  %5ld",extcohdr.numSampleFrames);			/* Slen */
					MSG1("\t\tSampleSize    %5ld",extcohdr.sampleSize);					/* Bits */
					MSG1("\t\tSampleRate    %5ld",FromIEEE(extcohdr.sampleRate));		/* Rate */
					MSG1("\t\tCompression   %5ld",extcohdr.compressionType);			/* Crunch */
					si->srat=FromIEEE(extcohdr.sampleRate);
					if(!si->srat) { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
					if(extcohdr.sampleSize<9) bits=8;
					else if(extcohdr.sampleSize<17) bits=16;
					else if(extcohdr.sampleSize<25) bits=24;
					else if(extcohdr.sampleSize<33) bits=32;
					else {
						ERR1("Bits : %d",extcohdr.sampleSize);
						Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					if(extcohdr.numChannels!=1 && extcohdr.numChannels!=2 && extcohdr.numChannels!=4) {
						ERR1("Channels : %d",extcohdr.numChannels);
						Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					else SET_CHANNELS(si,(extcohdr.numChannels-1));
					switch(extcohdr.compressionType) {
						case NO_COMPRESSION:
						case COMP_ALAW:
						case COMP_ULAW:
						    break;
						default:
							Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					diff=ch.chunksize-sizeof(extcohdr);
					fseek(infile,diff,SEEK_CUR);		// seek alway - it's the textual compression description
					diff=0;
					break;
				case IFF_FVER:
					if(!fread(&temp_l,4,1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						if(temp_l!=AIFCVersion1) { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
						diff=ch.chunksize-4;
					}
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
				case IFF_MARK:
					if(!fread(&anzm,2,1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						if((marker=(struct Marker *)AllocVec(anzm*sizeof(struct Marker),MEMF_ANY))) {
							for(i=0;i<anzm;i++) {
								if(!fread(&marker[i].id ,2,1,infile)) {
									if(dataokay) { fmtcorr=TRUE;formsize=0;i=anzm; }
									else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
								}
								if(!fread(&marker[i].pos,4,1,infile)) {
									if(dataokay) { fmtcorr=TRUE;formsize=0;i=anzm; }
									else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
								}
								if(fread(&mlen,1,1,infile)) {
									if(!fread(mname,mlen,1,infile)) {
										if(dataokay) { fmtcorr=TRUE;formsize=0;i=anzm; }
										else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
									}
									mname[mlen]='\0';
									marker[i].name=StringCopy(mname);
								}
								else {
									if(dataokay) { fmtcorr=TRUE;formsize=0;i=anzm; }
									else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
									marker[i].name=NULL;
								}
							}
						}
						bmark=1;
						//diff=ch.chunksize-(2+anzm*sizeof(struct Marker));
					}
					break;
				case IFF_INST:
					if(!fread(&inst,sizeof(struct Instrument),1,infile))
					{
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						diff=ch.chunksize-sizeof(struct Instrument);
						binst=1;
					}
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
					switch(extcohdr.compressionType) {
						case NO_COMPRESSION:
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
							break;
						case COMP_ULAW:
							sc_fmt=STR_ULAW;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ULAW,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case COMP_ALAW:
							sc_fmt=STR_ALAW;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,slen,CRUNCH_ALAW,CONV(0,0),CHANNEL_QUADROI);break;
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
	if(binst && bmark) {
		if(inst.susloopmode==LOOP_FORWARD) {
			mf1=mf2=0;
			for(j=0;j<anzm;j++)	if(marker[j].id==inst.susloopmark1) { si->loops=marker[j].pos;mf1=1; }
			if(mf1) for(j=0;j<anzm;j++) if(marker[j].id==inst.susloopmark2) { si->loopl=marker[j].pos-si->loops;mf2=1; }
			if(mf1 && mf2) si->loop=SFXLOOP_FORWARD;
		}
	}
	if(marker) {
		for(i=0;i<anzm;i++) if(marker[i].name) FreeVec(marker[i].name);
		FreeVec((ULONG *)marker);
	}

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
