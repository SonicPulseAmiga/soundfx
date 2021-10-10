/****h* RIFF-WAV/RIFF-WAV_L.c [4.3] *
*
*  NAME
*    RIFF-WAV_L.c
*  COPYRIGHT
*    $VER: RIFF-WAV_L.c 4.3 (16.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    RIFF-WAV loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    16.Apr.2004    V 4.3   most recent version
*							A: support for IEEE formats
*                           C: better handling of files with broken fmt_ size (wSize)
*    14.Sep.2002	V 4.2	most recent version
*               			C: handles corrupt form- and chunksizes better
*    08.Feb.2001	V 4.1	most recent version
*							C: now better accepts broken samples (havemost)
*    18.Jun.2000	V 1.2	most recent version
*    15.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/RIFF-WAV_L *
*  Function.eng
*    Loads RIFF-WAV Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    This format was introduced by Windows on the PC and borrows heavily from
*    the IFF standard. The WAV format represents one of the most used formats on
*    the PC.
*  Funktion.ger
*    Läd RIFF-WAV Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dieses Format wurde mit Window auf dem PC eingeführt und ist stark an den
*    IFF-Standart angelehnt. Das WAV-Format ist Eines der am häufigsten genutzten
*    Sampleformate auf dem PC.
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

#define RIFF_WAV_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "RIFF-WAV.h"

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
	struct ChunkHeader ch,chi;
	struct WaveFmt				wavefmt;
	struct WaveFmt_ADPCM		wavefmtadpcm;
	struct WaveFmt_ADPCM_Coeff	wavefmtadpcmcoeff;
	struct WaveFmt_DVIIMA_ADPCM	wavefmtdviimaadpcm;
	struct WaveFact				wavefact;
	struct WaveSmpl				wavesmpl;
	struct WaveSmpl_Loop		wavesmplloop;
	FILE *infile;
	LONG formsize,sformsize,subtype,fmtextrasize=0;
	LONG diff=0;
	UBYTE bits=8;
	//UBYTE lok;
	ULONG l;
	char ibuf[256];
	UBYTE *tempbuf;
	WORD wSize;
	LoadData ldata;
	APTR codecdata=NULL;
	ULONG codecdatasize;
	WORD *ptr;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;

	BOOL fmtcorr=FALSE,dataokay=FALSE;		/* support for mangled files (offset and length errors) */
	ULONG filesize=GetFileLength(pdata->fn);

	/** @todo handle size=0 ? */
	
	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) { fclose(infile);return(FALSE); }
	ch.chunksize=LitEnd2BigEnd_ulong(ch.chunksize);
	if(ch.TYPE!=WAV_RIFF) { sprintf(errmsg,GetString(errWrongFiletype),"RIFF");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	if(!fread(&subtype, sizeof(subtype),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	formsize=ch.chunksize-sizeof(subtype);
	//-- check for wrong formsize (bigger than file)
	if(formsize>filesize) {
		formsize=filesize-(sizeof(struct ChunkHeader)+sizeof(subtype));
		fmtcorr=TRUE;
	}
	if(subtype!=WAV_WAVE) { sprintf(errmsg,GetString(errWrongFiletype),"RIFF-WAV");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	while(formsize>0) {		// as long as there is anything left to read
		if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) {
			if(dataokay) { 
				ERR1("!FormatErrorCorrection required! : formsize=%d",formsize);
				fmtcorr=TRUE;formsize=diff=0;
			}
			else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
		}
		if(diff) {
			ERR1("!FormatErrorCorrection required! : diff=%d",diff);
			if(!ValidChunkID(ch.STRTYPE)) {
				diff-=sizeof(struct ChunkHeader);
				fseek(infile,diff,SEEK_CUR);
				if(!fread(&ch,sizeof(struct ChunkHeader),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				if(!ValidChunkID(ch.STRTYPE)) formsize=0;	// seems to be totally scrambled
			}
			fmtcorr=TRUE;
			diff=0;
		}
		ch.chunksize=LitEnd2BigEnd_ulong(ch.chunksize);
		// DEBUG
		MSG5("Chunk : %c%c%c%c %6ld",ch.STRTYPE[0],ch.STRTYPE[1],ch.STRTYPE[2],ch.STRTYPE[3],ch.chunksize);
		// DEBUG
		formsize-=sizeof(struct ChunkHeader);
		//-- check for wrong chunksize (bigger than remaining file)
		if(ch.chunksize>formsize) {
			ch.chunksize=formsize;
			fmtcorr=TRUE;
			ERR1("!FormatErrorCorrection required! : chunck longer than rest of file : %d",(ch.chunksize-formsize));
		}
		if(formsize>0) {
			switch(ch.TYPE) {												/* je nach Chunktyp */
				case WAV_fmt_:
					if(!fread(&wavefmt,sizeof(wavefmt),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					wavefmt.wFormatTag		=LitEnd2BigEnd_uword(wavefmt.wFormatTag);
					wavefmt.wChannels		=LitEnd2BigEnd_uword(wavefmt.wChannels);
					wavefmt.dwSamplesPerSec	=LitEnd2BigEnd_ulong(wavefmt.dwSamplesPerSec);
					wavefmt.dwAvgBytesPerSec=LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);
					wavefmt.wBlockAlign		=LitEnd2BigEnd_uword(wavefmt.wBlockAlign);
					wavefmt.wBitsPerSample	=LitEnd2BigEnd_uword(wavefmt.wBitsPerSample);
					// DEBUG
					MSG1("\tFormatTag       %5d",wavefmt.wFormatTag);
					MSG1("\tChannels        %5d",wavefmt.wChannels);
					MSG1("\tSamplesPerSec   %5d",wavefmt.dwSamplesPerSec);
					MSG1("\tAvgBytesPerSec  %5d",wavefmt.dwAvgBytesPerSec);
					MSG1("\tBlockAlign      %5d",wavefmt.wBlockAlign);
					MSG1("\tBitsPerSample   %5d",wavefmt.wBitsPerSample);
					// DEBUG
					if(wavefmt.dwSamplesPerSec) si->srat=wavefmt.dwSamplesPerSec;
					else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
					if((wavefmt.wChannels!=1) && (wavefmt.wChannels!=2) && (wavefmt.wChannels!=4)) {
						Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
					}
					else SET_CHANNELS(si,(wavefmt.wChannels-1));
					// read fmt extension for compressed samples
					if(wavefmt.wFormatTag!=WAVE_FORMAT_PCM) {
						if(ch.chunksize>sizeof(wavefmt)) {
							if(!fread(&wSize,sizeof(wSize),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
							wSize=LitEnd2BigEnd_uword(wSize);
							MSG1("\tSize            %5d",wSize);
							fmtextrasize=sizeof(wSize);
						}
						else {
							wSize=0;
							ERR1("\tSize            %5d",wSize);
							ERR3("%d != %d + %d",ch.chunksize,sizeof(wavefmt),sizeof(wSize));
						}
					}
					switch(wavefmt.wFormatTag) {
						case WAVE_FORMAT_PCM:
							if(wavefmt.wBitsPerSample<9) bits=8;
							else if(wavefmt.wBitsPerSample<17) bits=16;
							else if(wavefmt.wBitsPerSample<25) bits=24;
							else if(wavefmt.wBitsPerSample<33) bits=32;
							else { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
							diff=ch.chunksize-sizeof(wavefmt);
							break;
						case WAVE_FORMAT_IEEE_FLOAT:
							MSG("WAVE_FORMAT_IEEE_FLOAT <----------");
							if(wavefmt.wBitsPerSample<33) bits=32;
							else if(wavefmt.wBitsPerSample<65) bits=64;
							else { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
							diff=ch.chunksize-(sizeof(wavefmt)+fmtextrasize);
							break;
						case WAVE_FORMAT_ADPCM:
							MSG("WAVE_FORMAT_ADPCM <--------------");
							if(!fread(&wavefmtadpcm,sizeof(wavefmtadpcm),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
							wavefmtadpcm.wSamplesPerBlock=LitEnd2BigEnd_uword(wavefmtadpcm.wSamplesPerBlock);
							wavefmtadpcm.wNumCoef=LitEnd2BigEnd_uword(wavefmtadpcm.wNumCoef);
							// DEBUG
							MSG1("\tSamplesPerBlock %5d",wavefmtadpcm.wSamplesPerBlock);
							MSG1("\tNumCoef         %5d",wavefmtadpcm.wNumCoef);
							// DEBUG
							diff=ch.chunksize-(sizeof(wavefmt)+sizeof(wavefmtadpcm)+fmtextrasize);
							codecdatasize=(1+wavefmtadpcm.wNumCoef)*(sizeof(WORD)<<1);
							if(codecdata=AllocVec(codecdatasize,MEMF_ANY)) {
								ptr=(WORD *)codecdata;
								*(ptr++)=wavefmtadpcm.wSamplesPerBlock;
								*(ptr++)=wavefmtadpcm.wNumCoef;
								for(l=0;l<wavefmtadpcm.wNumCoef;l++) {
									if(!fread(&wavefmtadpcmcoeff,sizeof(wavefmtadpcmcoeff),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
									wavefmtadpcmcoeff.iCoef1=LitEnd2BigEnd_uword(wavefmtadpcmcoeff.iCoef1);
									wavefmtadpcmcoeff.iCoef2=LitEnd2BigEnd_uword(wavefmtadpcmcoeff.iCoef2);
									// DEBUG
									MSG3("\tCoeff[%2d]        %5d %5d",l,wavefmtadpcmcoeff.iCoef1,wavefmtadpcmcoeff.iCoef2);
									// DEBUG
									*(ptr++)=wavefmtadpcmcoeff.iCoef1;
									*(ptr++)=wavefmtadpcmcoeff.iCoef2;
									diff-=sizeof(wavefmtadpcmcoeff);
								}
								ldata.codecdata=codecdata;
							}
							break;
						case WAVE_FORMAT_MULAW:
							diff=ch.chunksize-(sizeof(wavefmt)+fmtextrasize);
							break;
						case WAVE_FORMAT_ALAW:
							diff=ch.chunksize-(sizeof(wavefmt)+fmtextrasize);
							break;
						case WAVE_FORMAT_IMA_ADPCM:		// the same as WAVE_FORMAT_DVI_ADPCM
							MSG("WAVE_FORMAT_IMA_ADPCM <----------");
							if(!fread(&wavefmtdviimaadpcm,sizeof(wavefmtdviimaadpcm),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
							wavefmtdviimaadpcm.wSamplesPerBlock=LitEnd2BigEnd_uword(wavefmtdviimaadpcm.wSamplesPerBlock);
							// DEBUG
							MSG1("\tSamplesPerBlock %5d",wavefmtdviimaadpcm.wSamplesPerBlock);
							// DEBUG
							diff=ch.chunksize-(sizeof(wavefmt)+sizeof(wavefmtdviimaadpcm)+fmtextrasize);
							break;
						default:
							Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);
							break;
					}
					break;
				case WAV_fact:
					if(!fread(&wavefact,sizeof(wavefact),1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						wavefact.dwFileSize		=LitEnd2BigEnd_ulong(wavefact.dwFileSize);
						// DEBUG
						MSG1("\tFileSize       %5d",wavefact.dwFileSize);
						// DEBUG
						diff=ch.chunksize-sizeof(wavefact);
					}
					break;
				case WAV_smpl:
					if(!fread(&wavesmpl,sizeof(wavesmpl),1,infile)) {
						if(dataokay) { fmtcorr=TRUE;formsize=0; }
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						wavesmpl.dwIMAManufacturer	=LitEnd2BigEnd_ulong(wavesmpl.dwIMAManufacturer);
						wavesmpl.dwIMAProduct		=LitEnd2BigEnd_ulong(wavesmpl.dwIMAProduct);
						wavesmpl.dwSamplePeriod		=LitEnd2BigEnd_ulong(wavesmpl.dwSamplePeriod);
						wavesmpl.dwUnityMIDINote	=LitEnd2BigEnd_ulong(wavesmpl.dwUnityMIDINote);
						wavesmpl.dwPitchFraction	=LitEnd2BigEnd_ulong(wavesmpl.dwPitchFraction);
						wavesmpl.dwSMPTEType		=LitEnd2BigEnd_ulong(wavesmpl.dwSMPTEType);
						wavesmpl.dwSMPTEOffset		=LitEnd2BigEnd_ulong(wavesmpl.dwSMPTEOffset);
						wavesmpl.dwNumLoops			=LitEnd2BigEnd_ulong(wavesmpl.dwNumLoops);
						// DEBUG
						//kprintf("\tManufacturer   %5d\n",wavesmpl.dwIMAManufacturer);
						//kprintf("\tProduct        %5d\n",wavesmpl.dwIMAProduct);
						//kprintf("\tSamplePeriod   %5d\n",wavesmpl.dwSamplePeriod);
						//kprintf("\tUnityMIDINote  %5d\n",wavesmpl.dwUnityMIDINote);
						//kprintf("\tPitchFraction  %5d\n",wavesmpl.dwPitchFraction);
						//kprintf("\tSMPTEType      %5d\n",wavesmpl.dwSMPTEType);
						//kprintf("\tSMPTEOffset    %5d\n",wavesmpl.dwSMPTEOffset);
						//kprintf("\tNumLoops       %5d\n",wavesmpl.dwNumLoops);
						// DEBUG
						//lok=0;
						for(l=0;l<wavesmpl.dwNumLoops;l++) {
							if(!fread(&wavesmplloop,sizeof(wavesmplloop),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);}
							//if(!lok) {		// ?
								wavesmplloop.dwType	=LitEnd2BigEnd_ulong(wavesmplloop.dwType);
								if(wavesmplloop.dwType==WAV_SMPL_LOOPFWD) {
									wavesmplloop.dwStart	=LitEnd2BigEnd_ulong(wavesmplloop.dwStart);
									wavesmplloop.dwEnd		=LitEnd2BigEnd_ulong(wavesmplloop.dwEnd);
									// DEBUG
									//MSG3("%2d %6d %6d",l,wavesmplloop.dwStart,wavesmplloop.dwEnd);
									// DEBUG
									si->loop=SFXLOOP_FORWARD;
									si->loops=wavesmplloop.dwStart;
									si->loopl=wavesmplloop.dwEnd-wavesmplloop.dwStart;
									if(si->loops+si->loopl>si->slen && si->slen>0) si->loopl=si->slen-si->loops;
								}
							//}
						}
						fseek(infile,ch.chunksize-(sizeof(wavesmpl)+wavesmpl.dwNumLoops*sizeof(wavesmplloop)),SEEK_CUR);
					}
					break;
				case WAV_data:
					ldata.offs=ftell(infile);
					ldata.chmask=CH_ALL;
					ldata.flags=LDF_NONE;
					switch(wavefmt.wFormatTag) {
						case WAVE_FORMAT_PCM:
							switch(bits) {
								case 8:
									sc_fmt=STR_PCM8;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
									}
									break;
								case 16:
									sc_fmt=STR_PCM16;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
									}
									break;
								case 24:
									sc_fmt=STR_PCM24;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM24,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
									}
									break;
								case 32:
									sc_fmt=STR_PCM32;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_PCM32,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
									}
									break;
							}
							break;
						case WAVE_FORMAT_IEEE_FLOAT:
							//MSG("\tIEEE ReadData");
							switch(bits) {
								case 32:
									sc_fmt=STR_IEEE32;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE32,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE32,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE32,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
									}
									break;
								case 64:
									sc_fmt=STR_IEEE64;
									switch(si->channels) {
										case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE64,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
										case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE64,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
										case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_IEEE64,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
									}
									break;
							}
							break;
						case WAVE_FORMAT_ADPCM:
							MSG("\tADPCM ReadData");
							sc_fmt=STR_MS_ADPCM;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_MS_ADPCM16_4,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_MS_ADPCM16_4,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_MS_ADPCM16_4,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case WAVE_FORMAT_MULAW:
							//MSG("\tµ-LAW ReadData");
							sc_fmt=STR_ULAW;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ULAW,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
						case WAVE_FORMAT_ALAW:
							//MSG("\tA-LAW ReadData");
							sc_fmt=STR_ALAW;
							switch(si->channels) {
								case CH_MONO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
								case CH_STEREO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	si->slen=ReadData(si,&ldata,ch.chunksize,CRUNCH_ALAW,CONV(0,0),CHANNEL_QUADROI);break;
							}
							break;
					}
					if(!si->slen) { fclose(infile);return(FALSE); }
					fseek(infile,ch.chunksize,SEEK_CUR);
					si->zoomxl=si->slen;
					dataokay=TRUE;
					break;
				case WAV_LIST:
					if(!fread(&subtype, sizeof(subtype),1,infile)) {
						if(dataokay) {
							ERR1("!FormatErrorCorrection required! : formsize=%d",formsize);
							fmtcorr=TRUE;formsize=0;
						}
						else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
					}
					else {
						if(subtype==WAV_INFO) {
							sformsize=ch.chunksize-sizeof(subtype);
							while(sformsize>0) {		// as long as there is anything left to read
								if(!fread(&chi,sizeof(struct ChunkHeader),1,infile)) {
									if(dataokay) { fmtcorr=TRUE;sformsize=0; }
									else { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
								}
								chi.chunksize=LitEnd2BigEnd_ulong(chi.chunksize);
								sformsize-=sizeof(struct ChunkHeader);
								// DEBUG
								MSG5("Chunk : %c%c%c%c %6ld",chi.STRTYPE[0],chi.STRTYPE[1],chi.STRTYPE[2],chi.STRTYPE[3],chi.chunksize);
								// DEBUG
								switch(chi.TYPE) {												/* je nach Chunktyp */
									case WAV_IART:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;
									case WAV_ICOP:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;
									case WAV_ICRD:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;
									case WAV_INAM:
										if(tempbuf=(UBYTE *)AllocVec(chi.chunksize+1,MEMF_ANY)) {
											if(!fread(tempbuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE);	}
											tempbuf[chi.chunksize]=0;
											TrimString(tempbuf);
											si->title=StringCopy(tempbuf);
											FreeVec((ULONG *)tempbuf);
										}
										else fseek(infile,ch.chunksize,SEEK_CUR);
										break;
									case WAV_IPRD:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;
									case WAV_ISFT:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;										
									case WAV_ISRC:
										if(!fread(ibuf,chi.chunksize,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
										//kprintf("\t\t%s\n",ibuf);
										break;
									default:
										fseek(infile,chi.chunksize,SEEK_CUR);
										break;
								}
								sformsize-=chi.chunksize;
								if(chi.chunksize&1) { fseek(infile,1L,SEEK_CUR);sformsize--; }			/* odd-length chunk */
								// DEBUG	
								//MSG1("Data left : %6ld",sformsize);
								// DEBUG
							}
						}
						else fseek(infile,ch.chunksize,SEEK_CUR);
					}
					break;
				default:
					//MSG("unknown");
					fseek(infile,ch.chunksize,SEEK_CUR);
					break;
			}
			formsize-=ch.chunksize;
			if(ch.chunksize&1) { formsize--;fseek(infile,1L,SEEK_CUR); }			/* odd-length chunk */
			//MSG1("\tData left : %6ld",formsize);
		}
	}
	if(fmtcorr) { sprintf(errmsg,GetString(errDamagedFormat),pdata->fn);Message(errDamagedFormat,errmsg,__FILE__,__LINE__); }
	if(codecdata) FreeVec(codecdata);
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
