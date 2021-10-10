/****h* Clipboard/Clipboard_L.c [4.2] *
*
*  NAME
*    Clipboard_L.c
*  COPYRIGHT
*    $VER: Clipboard_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Clipboard loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jun.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Mar.2001	V 4.1	most recent version
*    29.Jun.1999	V 1.20	most recent version
*    22.Sep.1998	V 1.10	most recent version
*    02.Jun.1997	V 1.00	initial version
*  NOTES
*
*******
*/

/****u* _loaders/Clipboard_L *
*  Function.eng
*    Loads files from the clipboard. You can exchange data with other programs
*    via the clipboard.<br>
*    Instead of a file requester, a clipboarde requester will appear, where you
*    can choose one of 256 clips.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16)</td></tr>
*    </table>
*  Funktion.ger
*    läd Dateien aus dem Clipboard. Über das Clipboard können Sie Daten mit
*    anderen Programmen austauschen.<br>
*    Anstatt eines Filerequesters erscheint ein Clipboardrequester, in dem Sie
*    eine von 256 Clipdateien wählen können.
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

#define Clipboard_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>
#include <libraries/iffparse.h>
#include <proto/iffparse.h>

//-- Local

#include "project.h"
#include "Clipboard.h"

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

struct Library	 			*IFFParseBase=NULL;
struct IFFHandle			*iff=NULL;

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- GetClipUnit requester

#include "ClipReq.h"

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	SFXMod_OpenGUILibs;
	if(!(IFFParseBase	=OpenLibrary("iffparse.library",37L)))							{ ADosError("iffparse.library >=V37.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; }
	if(!(iff			=AllocIFF())) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(iff)				FreeIFF(iff);
	if(IFFParseBase)	CloseLibrary(IFFParseBase);
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
	WORD unit;
	struct ContextNode *cn;
	struct Voice8Header v8h;
	register ULONG j;
	register UBYTE l;
	ULONG chan,bits=8,curlen;
	UBYTE *workbuf,*wptr,resfail;
	LONG error;
	UBYTE okay=1;
	STRPTR tempbuf;
	SInfo *si=pdata->dst[0];

	if((unit=GetClipUnit())>-1) {
		if((iff->iff_Stream=(ULONG)OpenClipboard(unit))) {
			InitIFFasClip(iff);
			if(!(OpenIFF(iff,IFFF_READ))) {
				while(okay) {
					error=ParseIFF(iff,IFFPARSE_RAWSTEP);
					if(error==IFFERR_EOC) continue;
					else if(error) break;		/* Leave the loop if there is any other error. */
					/* If we get here, error was zero. */
					if((cn=CurrentChunk(iff))) {
						if(cn->cn_Type==IFF_8SVX || cn->cn_Type==IFF_16SV) {
							switch(cn->cn_ID) {
								case IFF_VHDR:
									if((error=ReadChunkBytes(iff,&v8h,sizeof(v8h)))>-1) {
										si->slen=v8h.oneShotHiSamples;
										if(v8h.repeatHiSamples) si->slen+=v8h.repeatHiSamples;
										si->zoomxl=si->slen;
										if(v8h.samplesPerSec) si->srat=v8h.samplesPerSec;
										else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
										if(v8h.repeatHiSamples) {
											si->loop=SFXLOOP_FORWARD;
											si->loopl=v8h.repeatHiSamples;
											si->loops=v8h.oneShotHiSamples;
										}
									}
									else okay=0;
									break;
								case IFF_NAME:
									if((tempbuf=(UBYTE *)AllocVec(cn->cn_Size+1,MEMF_ANY))) {
										if((error=ReadChunkBytes(iff,tempbuf,cn->cn_Size))>-1) {
											tempbuf[cn->cn_Size]=0;TrimString(tempbuf);
											si->title=StringCopy(tempbuf);
											FreeVec((ULONG *)tempbuf);
										}
										else okay=0;
									}
									break;
								case IFF_CHAN:
									if((error=ReadChunkBytes(iff,&chan,4))>-1) {
										if(chan== 6) SET_CHANNELS(si,CH_STEREO);
										if(chan==30) SET_CHANNELS(si,CH_QUADRO);
									}
									else okay=0;
									break;
								case IFF_BITS:
									if((error=ReadChunkBytes(iff,&bits,4))>-1) {
									}
									else okay=0;
									break;
								case IFF_BODY:
									resfail=0;
									if(!(workbuf=(UBYTE *)AllocVec(cn->cn_Size,MEMF_ANY))) resfail=1;
									if(!(AllocSampleMem(si,si->slen,CH_COUNT(si->channels)))) resfail=1;
									if(!resfail) {
										if((error=ReadChunkBytes(iff,workbuf,cn->cn_Size))>-1) {
											wptr=workbuf;
											switch(cn->cn_Type)	{
												case IFF_8SVX:
													switch(bits) {
														case 8:
														//switch(v8h.sCompression) {
															//case COMP8SVX_PCM:
																{
																	PWData * const pwd=NewProWin();
																	if(pwd)	{
																		for(l=0;l<CH_COUNT(si->channels);l++) {
																			SmpAccess * const dstsa=SAInit(si,l,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
																			if(dstsa) {
																				SAFuncPtr const SAWrite=dstsa->safunc;
																				register SAMPLE *dbuf;
																				ProWinTitle title;
																				sprintf(title,GetString(msgProgressLoad),l+1,CH_COUNT(si->channels));
																				StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
																				for(dstsa->seg=0;dstsa->seg<dstsa->seganz;dstsa->seg++) {
																					dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
																					for(j=0;(j<Bytes2Samples(dstsa->blen) && curlen<si->slen);j++,curlen++)	{
																						*(dbuf++)=((WORD)(*(wptr++))<<8);
																					}
																					SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
																				}
																				if(ProWinAborted(curlen)) l=CH_MAX;					 // es wurde abgebrochen
																			}
																			SADone(dstsa,si);					// Samplezugiffshandles freigeben
																		}
																		DelProWin(pwd);
																	}
																}
																//break;
															//}
															break;
														case 16:
															//switch(v8h.sCompression) {
																//case COMP8SVX_PCM:
																{
																	PWData * const pwd=NewProWin();
																	if(pwd)	{
																		for(l=0;l<CH_COUNT(si->channels);l++) {
																			SmpAccess * const dstsa=SAInit(si,l,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
																			if(dstsa) {
																				SAFuncPtr const SAWrite=dstsa->safunc;
																				register SAMPLE *dbuf;
																				ProWinTitle title;
																				sprintf(title,GetString(msgProgressLoad),l+1,CH_COUNT(si->channels));
																				StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
																				for(dstsa->seg=0;dstsa->seg<dstsa->seganz;dstsa->seg++) {
																					dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
																					for(j=0;(j<Bytes2Samples(dstsa->blen) && curlen<si->slen);j++,curlen++) {
																						*(dbuf++)=((WORD)(*(wptr++))<<8)|(*(wptr++));
																					}
																					SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
																				}
																				if(ProWinAborted(curlen)) l=CH_MAX;					 // es wurde abgebrochen
																			}
																			SADone(dstsa,si);					// Samplezugiffshandles freigeben
																		}
																		DelProWin(pwd);
																	}
																}
																//break;
															//}
															break;
													}
													break;
												case IFF_16SV:
													//switch(v8h.sCompression) {
														//case COMP8SVX_PCM:
														{
															PWData * const pwd=NewProWin();
															if(pwd)	{
																for(l=0;l<(si->channels+1);l++)	{
																	SmpAccess * const dstsa=SAInit(si,l,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
																	if(dstsa) {
																		SAFuncPtr const SAWrite=dstsa->safunc;
																		register SAMPLE *dbuf;
																		ProWinTitle title;
																		sprintf(title,GetString(msgProgressLoad),l+1,CH_COUNT(si->channels));
																		StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
																		for(dstsa->seg=0;dstsa->seg<dstsa->seganz;dstsa->seg++) {
																			dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
																			for(j=0;(j<Bytes2Samples(dstsa->blen) && curlen<si->slen);j++,curlen++)	{
																				*(dbuf++)=((WORD)(*(wptr++))<<8)|(*(wptr++));
																			}
																			SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
																		}
																		if(ProWinAborted(curlen)) l=CH_MAX;					 // es wurde abgebrochen
																	}
																	SADone(dstsa,si);					// Samplezugiffshandles freigeben
																}
																DelProWin(pwd);
															}
														}
														//break;
													//}
													break;
											}
										}
										else okay=0;
									}
									else {
										Message(errAllocateMemory,NULL,__FILE__,__LINE__);
										CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
										if(workbuf)	FreeVec((APTR)workbuf);
										return(FALSE);
									}
									if(workbuf)	FreeVec((APTR)workbuf);
									break;
								default:
									MSG4("unknown/unused : %c%c%c%c",(UBYTE)((cn->cn_ID&0xFF000000)>>24),(UBYTE)((cn->cn_ID&0x00FF0000)>>16),(UBYTE)((cn->cn_ID&0x00FF00)>>8),(UBYTE)(cn->cn_ID&0x000000FF));
									break;
							}
						}
						else { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					}
					else MSG("\tCurrentChunk failed!");
				}
				if(!si->title) si->title=StringCopy("Clipboard");
				CloseIFF(iff);
			}
			else { MSG("OpenIFF failed");return(FALSE);	}
			CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
		}
		else { MSG("OpenClipboard failed");return(FALSE); }
	}
	else return(FALSE);
	switch(error) {
		case IFFERR_NOMEM:
			Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE);
		case IFFERR_READ:
		case IFFERR_WRITE:
		case IFFERR_SEEK:
		case IFFERR_MANGLED:
		case IFFERR_SYNTAX:
		case IFFERR_NOTIFF:
			return(FALSE);
		case IFFERR_EOC:
		case IFFERR_EOF:
		default:
			return(TRUE);
	}
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
}

void SaveSettings(struct Instance *instance,char *fname) {
}

void SetDefaultSettings(struct Instance *instance){
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
