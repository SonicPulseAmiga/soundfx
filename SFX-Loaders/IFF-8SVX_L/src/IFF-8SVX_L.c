/****h* IFF-8SVX/IFF-8SVX_L.c [4.2] *
*
*  NAME
*    IFF-8SVX_L.c
*  COPYRIGHT
*    $VER: IFF-8SVX_L.c 4.2 (22.07.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    IFF-8SVX loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jun.1997
*  MODIFICATION HISTORY
*    22.Jul.2002	V 4.2	most recent version
*               			handles corrupt form- and chunksizes better
*    07.Mar.2001	V 4.1	most recent version
*							C : now better accepts broken samples
*    18.Jun.2000	V 1.2	most recent version
*    13.Sep.1998	V 1.1	most recent version
*    02.Jun.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/IFF-8SVX_L *
*  Function.eng
*    Loads IFF-8SVX Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,FDPCM-16:6,EDPCM-8:4,EDPCM-16:5)</td></tr>
*    </table>
*    This is the most wide spread sound-file format on the Amiga. It is build
*    like any other IFF file making it a very flexible format whilst retaining
*    compatibility. The IFF-8SV format is one of the few that saves loops.<br>
*    __SFX also supports quadrosamples, 16-bit and combined samples. I have got
*    the description of the combined samples from the freeware program <b>SoundBox</b>
*    by Richard Körber. This format saves the full 16-bit data of a sample.
*    Is this sample loaded into a standard program (supporting only plain IFF-8SVX
*    files) then it loads as a standard 8-bit sample. If a program however knows
*    this format it loads it as a 16-bit sample.
*  Funktion.ger
*    Läd IFF-8SVX Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,FDPCM-16:6,EDPCM-8:4,EDPCM-16:5)</td></tr>
*    </table>
*    Dies ist das am meisten verbreitetste Sound-Dateiformat auf dem Amiga. Es
*    ist nach dem IFF-Standart aufgebaut und ist so leicht den eigenen Wünschen
*    anzupassen, ohne das die Kompatibilität beeinträchtigt wird. Das IFF-8SVX
*    Format gehört zu den wenigen Formattypen die Loops mit abspeichern.<br>
*    __SFX unterstützt auch Quadrosamples und 16-bit bzw. combined Samples.
*    Den Aufbau der Combined-Samples habe ich aus der Dokumentation des Freeware-
*    programmes <b>SoundBox</b> von Richard Körber entnommen. Dieses Format
*    speichert die vollen 16bit-Daten eines Samples. Wenn man dieses Sample in
*    ein herkömliches Programm (das nur normale IFF-8SVX-Samples kennt) einläd,
*    so wird das Sample automatisch als 8-bit Sample geladen. Ein Programm das
*    den Aufbau kennt, läd es als 16bit-Sample.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    When __SFX saves a sample in the 16-bit format it creates a "BITS" chunk of
*    the following structure :<br>
*    <code>struct chunk_bits {</code><br>
*    <code>  char  id[4];  // "BITS"</code><br>
*    <code>  ULONG len;    // 4L</code><br>
*    <code>  ULONG bits;   // 8/16 bit so far supported</code><br>
*    <code>};</code><br>
*    In addition the "CHAN" chunk has been extended. With a data value of 30, it
*    is a quadrosample.
*  Hinweise.ger
*    Bei Speicherung als 16-bit-Sample legt __SFX einen "BITS"-Chunk an. Dieser
*    ist wie folgt aufgebaut :<br>
*    <code>struct chunk_bits {</code><br>
*    <code>  char  id[4];  // "BITS"</code><br>
*    <code>  ULONG len;    // 4L</code><br>
*    <code>  ULONG bits;   // 8/16 bit so far supported</code><br>
*    <code>};</code><br>
*    Außerdem wurde der "CHAN"-Chunk erweitert. Bei einem Datenwert von 30, handelt
*    es sich um ein Quadrosample.
*******
*/

#define IFF_8SVX_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-8SVX.h"

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
	struct Voice8Header v8h;
	register ULONG i,j;
	register UBYTE l;
	FILE *infile;
	LONG formsize,subtype,diff=0;
	ULONG chan,bits=8;
	UBYTE *workbuf;
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
	if(subtype!=IFF_8SVX) { sprintf(errmsg,GetString(errWrongFiletype),"IFF-8SVX");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
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
				case IFF_VHDR:
					if(!fread(&v8h,sizeof(v8h),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);	}
	//				slen=v8h.oneShotHiSamples;
	//				if(v8h.repeatHiSamples) slen+=v8h.repeatHiSamples;
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
				case IFF_BITS:
					if(!fread(&bits,4,1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else diff=ch.chunksize-4;
					break;
				case IFF_BODY:
					ldata.offs=ftell(infile);
					ldata.chmask=CH_ALL;;
					ldata.flags=LDF_NONE;
					switch(bits) {
						case 8:
							switch(v8h.sCompression) {
								case COMP8SVX_PCM:
									sc_fmt=STR_PCM8;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
								case COMP8SVX_FDPCM:
									sc_fmt=STR_FDPCM8_4;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
								case COMP8SVX_EDPCM:
									sc_fmt=STR_EDPCM8_4;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM8_4,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM8_4,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM8_4,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
							}
							break;
						case 16:
							switch(v8h.sCompression) {
								case COMP8SVX_PCM:
									sc_fmt=STR_PCM16;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
								case COMP8SVX_FDPCM:
									sc_fmt=STR_FDPCM16_6;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_FDPCM16_6,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
								case COMP8SVX_EDPCM:
									sc_fmt=STR_EDPCM16_5;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_EDPCM16_5,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
							}
							break;
						case 24:
							switch(v8h.sCompression) {
								case COMP8SVX_PCM:
									sc_fmt=STR_PCM24;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
							}
							break;
						case 32:
							switch(v8h.sCompression) {
								case COMP8SVX_PCM:
									sc_fmt=STR_PCM32;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREO);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(0,0),CHANNEL_QUADRO);break;
									}
									break;
							}
							break;
					}
					if(!si->slen) { fclose(infile);return(0); }
					fseek(infile,ch.chunksize,SEEK_CUR);
					si->zoomxl=si->slen;
					dataokay=TRUE;
					break;
				case IFF_LBDY:
					sc_fmt=STR_PCM16;
					if(workbuf=(UBYTE *)AllocVec(ch.chunksize,MEMF_ANY)) {
						if(fread(workbuf,ch.chunksize,1,infile)) {
							j=0;
							for(l=0;l<(si->channels+1);l++) 
								for(i=0;i<si->slen;i++)
									si->buf[l][i]|=(WORD)workbuf[j++];
						}
						else Message(errReadData,NULL,__FILE__,__LINE__);
						FreeVec((ULONG *)workbuf);
					}
					else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
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
