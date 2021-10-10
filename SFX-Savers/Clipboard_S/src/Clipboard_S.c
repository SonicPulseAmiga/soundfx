/****h* Clipboard/Clipboard_S.c [4.2] *
*
*  NAME
*    Clipboard_S.c
*  COPYRIGHT
*    $VER: Clipboard_S.c 4.2 (19.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Clipboard saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    19.Apr.2002	V 4.2	most recent version
*    27.Aug.2000	V 4.1	most recent version
*    04.Jul.1999	V 1.20	most recent version
*    30.Oct.1998	V 1.10	most recent version
*    02.Jul.1997	V 1.00	initial version
*  NOTES
*
*******
*/

/****u* _savers/Clipboard_S *
*  Function.eng
*    Saves files to the clipboard. You can exchange data with other programms via
*    the clipboard.<br>
*    Instead of a file requester, a clipboarde requester will appear, where you
*    can choose one of 256 clips.
*  Funktion.ger
*    Speichert Dateien in das Clipboard. Über das Clipboard können Sie Daten mit
*    anderen Programmen austauschen.<br>
*    Anstatt eines Filerequesters erscheint ein Clipboardrequester, in dem Sie
*    eine von 256 Clipdateien wählen können.
*  Parameter.eng
*    Type
*      which format (IFF-8SVX,IFF-16SV)
*    Format
*      type of compression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*  Parameter.ger
*    Type
*      welches Format (IFF-8SVX,IFF-16SV)
*    Format
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define Clipboard_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>
#include <libraries/iffparse.h>
#include <proto/iffparse.h>

//-- Local

#include "project.h"
#include "Clipboard.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_TYPE		(GADID_BASE+1)
#define GADIX_TYPE		(GADIX_BASE+1)
#define GADID_FORMAT	(GADID_TYPE+1)
#define GADIX_FORMAT	(GADIX_TYPE+1)

#define GADID_SRC       (GADID_FORMAT+1)
#define GADIX_SRC       (GADIX_FORMAT+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source          src;        /* Quelle */
	UBYTE format,type;
	
	PrefsData moddesc[2];
};

struct Library				*IFFParseBase=NULL;
struct IFFHandle			*iff=NULL;

char *TypeLabels[]={"IFF-8SVX","IFF-16SV",0l};
char *FormLabels[]={STR_PCM8,STR_PCM16,0l};

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
	if(!(cfg			=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	if(!(iff			=AllocIFF())) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(iff)				FreeIFF(iff);
	if(cfg)				CfgIO_RemConfig(cfg);
	if(IFFParseBase)	CloseLibrary(IFFParseBase);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void __saveds __asm *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {
	struct Instance *instance;

	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_END         ,0l  ,0l);	
		SFXMod_ClientDataInitCommon;
	}
	return((void *)instance);
}

void __saveds __asm LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) {
	if(instance) {
		SFXMod_ClientDataDoneCommon;
		FreeVec((void *)instance);
	}
}

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables
	SFXMod_Interface_CfgSelVariables
	SFXMod_Interface_NotifyVariables

	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
	UWORD xh;

	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+RunTime->scx[(308>>1)-110];
		ng.ng_TopEdge		=ys3+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	=GetString(btnSave);
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[(308>>1)+10];
		ng.ng_GadgetText	=GetString(menPrefsUse);
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		g=AddSource(&instance->src,modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[95];
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_TYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,TypeLabels,GTCY_Active,instance->type,TAG_DONE);
		modGadgets[GADIX_TYPE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[205];
		ng.ng_GadgetText	="Format";
		ng.ng_GadgetID		=GADID_FORMAT;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_FORMAT]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous" ,xs1,ys2,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));               /* Miscellaneous */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);                  /* Go */

			SetSource   (&instance->src,modWin,modGadgets,GADIX_SRC);
			if(instance->type==1) GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&instance->src,modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource(&instance->src,modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_TYPE:
											instance->type=icode;
											if(instance->type==1) GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,GA_Disabled,TRUE,TAG_DONE);
											else GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,GA_Disabled,FALSE,TAG_DONE);
											break;
										case GADID_FORMAT:
											instance->format=icode;
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									//case KEY_TAB: ActivateGadget(modGadgets[GADIX_],modWin,0l);break;
								}
								break;
							SFXMod_Interface_Events
						}
					}
				}
			}
			SFXMod_Interface_WinDone
			SFXMod_Interface_FReqDone
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	SFXMod_Interface_NotifyDone
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	WORD unit;
	struct Voice8Header v8h;
	register ULONG j;
	register UBYTE l;
	ULONG chan,bits,size,bufl,sl,curlen,blen;
	UBYTE *workbuf,*wptr;
	SaveData sdata;
	SInfo *si;
	UBYTE ret=1;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:             // wir wurden e.g. per ARexx gestartet
			UnlockBuffer(instance->src.srcbuf);
			ret&=((instance->src.srcbuf=LockBuffer(pdata->src[0]))!=0);
			if(!ret) return(ret);
			instance->src.rmode=RNGMD_ALL;
			break;
	}
	si=instance->src.srcbuf;

	SetRngs(si,&sdata.savestart,&sdata.savelength,instance->src.rmode);
	strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
	sdata.chmask=CH_ALL;

	size=4L;
	size+=(long)(sizeof(v8h)+8L);							/* VHDR */
	sl=strlen(((struct Node *)si)->ln_Name);if(sl&1) sl++;
	size+=(long)(sl+8L);									/* NAME */
	if(si->channels>0) size+=12L;							/* CHAN */

	bufl=Samples2Bytes(sdata.savelength*(si->channels+1));
	MSG1("bufl vor  crunch=%d",bufl);
	switch(instance->type) {
		case TYPE_8SVX:
			switch(instance->format) {
				case FORMAT_8SVX_PCM8:	bufl=GetCrunchedLength(CRUNCH_PCM8 ,bufl);v8h.sCompression=COMP8SVX_PCM;		  break;
				case FORMAT_8SVX_PCM16:	bufl=GetCrunchedLength(CRUNCH_PCM16,bufl);v8h.sCompression=COMP8SVX_PCM;size+=12L;break;
			}
			break;
		case TYPE_16SV:	bufl=GetCrunchedLength(CRUNCH_PCM16,bufl);v8h.sCompression=COMP16SV_PCM;break;
	}
	MSG1("bufl nach crunch=%d",bufl);
	size+=(bufl+8L);										/* BODY */

	if((unit=GetClipUnit())!=-1) {
		if((iff->iff_Stream=(ULONG)OpenClipboard(unit))) {
			InitIFFasClip(iff);
			if(!(OpenIFF(iff,IFFF_WRITE))) {
				switch(instance->type) {
					case TYPE_8SVX:
						if(PushChunk(iff,IFF_8SVX,IFF_FORM,size)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
						break;
					case TYPE_16SV:
						if(PushChunk(iff,IFF_16SV,IFF_FORM,size)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
						break;
				}
				PopChunk(iff);
				if(PushChunk(iff,0,IFF_VHDR,sizeof(struct Voice8Header))) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }

				v8h.samplesPerHiCycle=0;
				v8h.samplesPerSec=si->srat;
				v8h.ctOctave=1;
				if((instance->src.rmode==RNGMD_ALL && si->loop==SFXLOOP_FORWARD) && si->loopl) {
					v8h.oneShotHiSamples=si->loops;
					v8h.repeatHiSamples=si->loopl;
				}
				else {
					v8h.oneShotHiSamples=sdata.savelength;
					v8h.repeatHiSamples=0;
				}
				if(!WriteChunkBytes(iff,&v8h,sizeof(struct Voice8Header))) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
				PopChunk(iff);

				sl=strlen(((struct Node *)si)->ln_Name);if(sl&1) sl++;
				if(PushChunk(iff,0,IFF_NAME,sl)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
				if(!WriteChunkBytes(iff,((struct Node *)si)->ln_Name,sl)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
				PopChunk(iff);

				if(si->channels>0)										/* >Mono ? */
				{
					if(PushChunk(iff,0,IFF_CHAN,4L)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					if(si->channels==1) chan=6;
					if(si->channels==3) chan=30;
					if(!WriteChunkBytes(iff,&chan,4L)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					PopChunk(iff);
				}

				if(instance->type==TYPE_8SVX && instance->format==FORMAT_8SVX_PCM16)
				{
					if(PushChunk(iff,0,IFF_BITS,4L)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					bits=16;
					if(!WriteChunkBytes(iff,&bits,4L)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					PopChunk(iff);
				}

				if(PushChunk(iff,0,IFF_BODY,bufl)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
				if(workbuf=(UBYTE *)AllocVec(bufl,MEMF_ANY))
				{
					PWData * const pwd=NewProWin();
					if(pwd)
					{
						wptr=workbuf;
						//MSG1("  workbuf=0x%08lx",workbuf);
						switch(instance->type)
						{
							case TYPE_8SVX:
								switch(instance->format)
								{
									case FORMAT_8SVX_PCM8:
										for(l=0;l<CH_COUNT(si->channels);l++) {
											SmpAccess * const srcsa=SAInit(si,l,sdata.savestart,sdata.savelength,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
											if(srcsa) {
												SAFuncPtr const SARead=srcsa->safunc;
												register SAMPLE *sbuf;
												ProWinTitle title;
												blen=Bytes2Samples(srcsa->blen);
												sprintf(title,GetString(msgProgressSave),l+1,CH_COUNT(si->channels));
												StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
												for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<si->slen));srcsa->seg++) {
													sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);						// Pointer auf dst-Block
													for(j=0;((j<blen) && (curlen<si->slen));j++,curlen++) {
														*(wptr++)=(UBYTE)((*(sbuf++))>>8);
													}
												}
												if(ProWinAborted(curlen)) l=4;		// es wurde abgebrochen
												SADone(srcsa,si);					// Samplezugiffshandles freigeben
											}
										}
										break;
									case FORMAT_8SVX_PCM16:
										for(l=0;l<CH_COUNT(si->channels);l++) {
											SmpAccess * const srcsa=SAInit(si,l,sdata.savestart,sdata.savelength,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
											if(srcsa) {
												SAFuncPtr const SARead=srcsa->safunc;
												register SAMPLE *sbuf;
												ProWinTitle title;
												blen=Bytes2Samples(srcsa->blen);
												sprintf(title,GetString(msgProgressSave),l+1,CH_COUNT(si->channels));
												StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
												for(srcsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++) {
													sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);						// Pointer auf dst-Block
													for(j=0;((j<blen) && (curlen<si->slen));j++,curlen++) {
														*(wptr++)=(UBYTE)((* sbuf   )>>8);
														*(wptr++)=(UBYTE)((*(sbuf++))&0xFF);
													}
												}
												if(ProWinAborted(curlen)) l=4;					// es wurde abgebrochen
												SADone(srcsa,si);					// Samplezugiffshandles freigeben
											}
										}
										break;
								}
								break;
							case TYPE_16SV:
								for(l=0;l<CH_COUNT(si->channels);l++) {
									SmpAccess * const srcsa=SAInit(si,l,sdata.savestart,sdata.savelength,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
									if(srcsa) {
										SAFuncPtr const SARead=srcsa->safunc;
										register SAMPLE *sbuf;
										ProWinTitle title;
										blen=Bytes2Samples(srcsa->blen);
										sprintf(title,GetString(msgProgressSave),l+1,CH_COUNT(si->channels));
										StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
										for(srcsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++) {
											sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);						// Pointer auf dst-Block
											for(j=0;((j<blen) && curlen<si->slen);j++,curlen++) {
												*(wptr++)=(UBYTE)((* sbuf   )>>8);
												*(wptr++)=(UBYTE)((*(sbuf++))&0xFF);
											}
										}
										if(ProWinAborted(curlen)) l=4;					// es wurde abgebrochen
										SADone(srcsa,si);					// Samplezugiffshandles freigeben
									}
								}
								break;
						}
						DelProWin(pwd);
					}
					if(!WriteChunkBytes(iff,workbuf,bufl)) { Message(errWriteData,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
					PopChunk(iff);
					FreeVec((ULONG *)workbuf);
				}
				else { Message(errAllocateMemory,NULL,__FILE__,__LINE__);CloseIFF(iff);CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);return(FALSE); }
				CloseIFF(iff);
			}
			else MSG("OpenIFF failed");
			CloseClipboard((struct ClipboardHandle *)iff->iff_Stream);
		}
		else MSG("OpenClipboard failed");
		return(TRUE);
	}
	else return(FALSE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ instance->format=CfgIO_MatchString(RexxPar3,FormLabels);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Type"))	{ instance->type  =CfgIO_MatchString(RexxPar3,TypeLabels);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ sprintf(ret,"%s",FormLabels[instance->format]);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Type"))	{ sprintf(ret,"%s",TypeLabels[instance->type  ]);/*pf=1;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetSaverPreset")) {
		if(!stricmp(RexxPar2,"default"))  { RecallDefaultSettings(instance); }
		else { sprintf(fn,PROGDIR"_savers/"PRJ_NAME"/%s.cfg",RexxPar2);LoadSettings(instance,fn); }
		SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	
}

/*-- local Stuff --------------------------------------------------------------------------------*/

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	LoadModSettings(cfg,instance->moddesc);
	instance->format	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_8SVX_PCM8]),FormLabels);
	instance->type	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Type"			,TypeLabels[TYPE_8SVX]),TypeLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);
	CfgIO_WriteString		  (cfg,"General"	,"Type"			,TypeLabels[instance->type]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->type	=TYPE_8SVX;
	instance->format	=FORMAT_8SVX_PCM8;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_TYPE  ],modWin,0l,GTCY_Active,instance->type,TAG_DONE);
	if(instance->type==1) GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,GA_Disabled,TRUE,TAG_DONE);
	else GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,GA_Disabled,FALSE,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
