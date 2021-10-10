/****h* Duplicate/Duplicate.c [4.1] *
*
*  NAME
*    Duplicate.c
*  COPYRIGHT
*    $VER: Duplicate.c 4.1 (11.10.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Duplicate Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    11.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*  IDEAS
*    new parameters
*    - value    : overlap (of time)
*    - checkbox : crossfade
*******
*/

/****u* _operators/Duplicate *
*  Function.eng
*    Doubles a sample multiple times
*  Funktion.ger
*    Doppelt ein Sample mehrfach
*  Parameter.eng
*    Rep.
*      Repetitions. How many copies of the sound do you want to have.
*  Parameter.ger
*    Rep.
*      Wiederholungen. Wie viele Kopien des Sounds sie haben möchten.
*  Notes.eng
*    If you got only one period of a waveform (such as most chipsounds) or only
*    cycle of a drum-loop, you can make it longer, by duplicating it several
*    times. This could be neccessary if you want to generate an effect with this sample.
*  Hinweise.ger
*    Wenn sie von einer Wellenform nur eine Periode haben (z.B. ein Chipsound) oder
*    nur einen Durchlauf eines Drumloops, so können Sie	dieses Sample verlängern,
*    indem Sie es mehrfach duplizieren. Dies ist z.B. notwendig, wenn Sie auf das
*    Sample einen Effekt berechnen möchten.
*******
*/

#define DUPLICATE_C

//-- includes -----------------------------------------------------------------

#include <libraries/sfx_mod.h>

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

//-- Initialisation and Destruction

int __asm __UserLibInit(REG(a6,struct Library *SFX_FXBase));
void __asm __UserLibCleanup(REG(a6,struct Library *SFX_FXBase));

//-- DataInitialisation and Destruction

void __asm LIBClientDataInit(REG(a0,RTime *RunTime_));
void __asm LIBClientDataDone(void);

//-- Modulroutienen

UBYTE __asm LIBInterface(void);
UBYTE __asm LIBProcess(REG(d0,UBYTE callmd),REG(a0,SInfo *si),REG(a1,char *fn));
void __asm LIBHandleARexxCmd(REG(a0,char *params),REG(a1,char *ret));

//-- local stuff

void LoadFXParams		(char *fname);
void SaveFXParams		(char *fname);
void DefaultFXParams	(void);
void RecallLastFXParams	(void);
void RecallDefFXParams	(void);

void RefreshGUI(struct Gadget *modGadgets[],struct Window *modWin);

//-- private Stuff

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_GO		0
#define GADIX_GO		0

#define GADID_CFGSEL	(GADID_GO+1)
#define GADIX_CFGSEL	(GADIX_GO+1)
#define GADID_CFGLIST	(GADID_CFGSEL)
#define GADIX_CFGLIST	(GADIX_CFGSEL)
#define GADID_CFGNAME	(GADID_CFGLIST+1)
#define GADIX_CFGNAME	(GADIX_CFGLIST+1)
#define GADID_CFGADD	(GADID_CFGNAME+1)
#define GADIX_CFGADD	(GADIX_CFGNAME+1)
#define GADID_CFGDEL	(GADID_CFGADD+1)
#define GADIX_CFGDEL	(GADIX_CFGADD+1)

#define GADID_SRC		(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_SRC		(GADIX_CFGSEL+CFGSEL_IXCT)

#define GADID_TIMES		(GADID_SRC+SRC_IDCT)
#define GADIX_TIMES		(GADIX_SRC+SRC_IXCT)

#define LAST_GADIX		(GADIX_TIMES+1)

//-- globals ------------------------------------------------------------------

//-- tag for version command

UBYTE *VersTag="\0$VER: "PRJ_VERS;

//-- these are the parameters the operator uses

struct
{
	Source			src;		/* Quelle */
	Param			times;
} modparam={0};

//-- this allows us to automatically process some parts of the settings

PrefsData moddesc[]=
{
	{ PD_SOURCE,		"S1",	&(modparam.src)		},
	{ PD_END,			0l,		0l					}	
};

//-- general global data

RTime 		*RunTime=NULL;		// the shared memory structure
APTR 		cfg=NULL;			// a handle for a preset
char		errmsg[256];		// for error messages

//-- Library-Basisaddrs.

extern struct ExecBase 		*SysBase;
struct Library	 			*AslBase=NULL;
struct Library	 			*ConfigIOBase=NULL;
struct Library	 			*DiskfontBase=NULL;
struct DosLibrary			*DOSBase=NULL;
struct Library				*GadToolsBase=NULL;
struct GfxBase	 			*GfxBase=NULL;
struct IntuitionBase		*IntuitionBase=NULL;
struct Library				*SFXSupportBase=NULL;
struct Library				*UnitConvBase=NULL;

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFX_FXBase))
{
	INITDEBUG;
	SysBase=(struct ExecBase *)*((ULONG *)4L);
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) goto Error;
	if(!(SFXSupportBase	=OpenLibrary("progdir:libs/"SFXSupportName,SFXSupportVersion))) { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open progdir:libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; }
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "progdir:libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; }
	if(!(AslBase		=OpenLibrary("asl.library",37)))								{ sprintf(errmsg,GetString(errOpenLibrary),"asl.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),ConfigIOName" >=V"NUM2STR(ConfigIOVersion));	Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(DiskfontBase	=OpenLibrary("diskfont.library",37)))							{ sprintf(errmsg,GetString(errOpenLibrary),"diskfont.library >=V37.0");					Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"dos.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L)))							{ sprintf(errmsg,GetString(errOpenLibrary),"gadtools.library >=V37.0");					Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"graphics.library >=V37.0");					Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),UnitConvName" >=V"NUM2STR(UnitConvVersion)); Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFX_FXBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFX_FXBase))
{
	if(cfg)				CfgIO_RemConfig(cfg);
	if(UnitConvBase)	CloseLibrary(UnitConvBase);
	if(SFXSupportBase)	{ FreeFXParams(moddesc);CloseLibrary(SFXSupportBase); }
	if(IntuitionBase)	CloseLibrary((struct Library *)IntuitionBase);
	if(GfxBase)			CloseLibrary((struct Library *)GfxBase);
	if(GadToolsBase)	CloseLibrary(GadToolsBase);
	if(DOSBase)			CloseLibrary((struct Library *)DOSBase);
	if(DiskfontBase)	CloseLibrary(DiskfontBase);
	if(ConfigIOBase)	CloseLibrary(ConfigIOBase);
	if(AslBase)			CloseLibrary(AslBase);
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void __saveds __asm LIBClientDataInit(REG(a0,RTime *RunTime_))
{
	RunTime=RunTime_;

	UnLock(CreateDir("progdir:_operators/"PRJ_NAME));
	RecallLastFXParams();			// Default oder LastUsed values setzen
	InitFXParams(moddesc);			// Buffers locken
}

void __saveds __asm LIBClientDataDone(void)
{
	DoneFXParams(moddesc);			// Buffers unlocken
}

//-- Modulroutinen

UBYTE __saveds __asm LIBInterface(void)
{
	struct NewGadget ng;
	struct Window *modWin;
	struct Gadget *g;
	struct Gadget *modGList=0l;
	struct Gadget *modGadgets[LAST_GADIX];
	struct FileRequester *modFRequest;
	struct MenuItem *item;
	struct RastPort *modrp;
	struct IntuiMessage *imsg;
	ULONG iclass,isec,imic;
	APTR iadr;
	USHORT icode;
	struct Node *node;
	CfgSel cfgsel;
	NotifyTask nt;
	LONG sigin,sigout;
	ULONG gnr;
	UBYTE ret=FALSE,runing=TRUE,gh,enabled,nr;
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	char fn[FMSIZE],cn[FNSIZE];
	struct TagItem wintags[]=
	{
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)PRJ_NAME,
		WA_ScreenTitle,		(ULONG)PRJ_VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	struct TagItem MODFReqtags[]=
	{
		ASLFR_TitleText,		NULL,
		ASLFR_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		ASLFR_InitialLeftEdge,	180,
		ASLFR_InitialTopEdge,	15,
		ASLFR_InitialWidth,		300,
		ASLFR_InitialHeight,	184,
		ASLFR_InitialDrawer,	(ULONG)"progdir:_operators/"PRJ_NAME,
		ASLFR_InitialFile,		(ULONG)"default.cfg",
		ASLFR_RejectIcons,		TRUE,
		ASLFR_DoSaveMode,		FALSE,
		TAG_DONE
	};

	enabled=(RunTime->aktsamples>0);

	nt.task=FindTask(NULL);
	if((nt.sig=AllocSignal(-1))!=-1)
	{
		AddTail(&RunTime->gblist,&nt);
		if(g=CreateContext(&modGList))
		{
			ng.ng_LeftEdge		=xs1+RunTime->scx[104];
			ng.ng_TopEdge		=ys3+RunTime->scy[2];
			ng.ng_Width			=RunTime->scx[100];
			ng.ng_Height		=RunTime->scy[11];
			ng.ng_GadgetText	=GetString(menStart);
			ng.ng_TextAttr		=RunTime->ScrFontB;
			ng.ng_GadgetID		=GADID_GO;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=RunTime->VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			modGadgets[GADIX_GO]=g;

			InitCfgSel(&cfgsel,"progdir:_operators/"PRJ_NAME);
			g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

			g=AddSource(&modparam.src,modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

			ng.ng_LeftEdge		=xs1+RunTime->scx[53];
			ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
			ng.ng_Width			=RunTime->scx[80];
			ng.ng_Height		=RunTime->scy[11];
			ng.ng_GadgetText	="Rep.";
			ng.ng_TextAttr		=RunTime->ScrFont;
			ng.ng_GadgetID		=GADID_TIMES;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.times,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_TIMES]=g;

			if(g)
			{
				MODFReqtags[0].ti_Data=(ULONG)GetString(strSelectPreset);
				MODFReqtags[2].ti_Data=(RunTime->xres-300)>>1;
				MODFReqtags[5].ti_Data=RunTime->yres-30;
				if(modFRequest=AllocAslRequest(ASL_FileRequest,MODFReqtags))
				{
					wintags[0].ti_Data=(RunTime->xres-winw)>>1;
					wintags[1].ti_Data=(RunTime->yres-winh)>>1;
					wintags[2].ti_Data=winw;
					wintags[3].ti_Data=winh;
					wintags[6].ti_Data=(ULONG)modGList;
					if(modWin=OpenWindowTagList(0l,wintags))
					{
						GT_RefreshWindow(modWin,0l);
						SetMenuStrip(modWin,RunTime->FXMenu);
						modrp=modWin->RPort;SetFont(modrp,RunTime->scrfont);

						SetAPen(modrp,1);
						RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

						DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
						DrawTitle(modrp,"Repetitions"	,xs1,ys2,STDFRM_WIDTH);
						DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

						xh=STDFRM_WIDTH;
						DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
						DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));				/* Par */
						DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
						DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

						SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
						SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);

						if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

						sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

						while(runing)
						{
							sigout=Wait(sigin);
							if(sigout&(1L<<nt.sig))
							{
								if(!enabled && RunTime->aktsamples)
								{
									enabled=1;
									GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
									modparam.src.srcbuf=LockBuffer(RunTime->aktbuf);
									modparam.src.rmode=SetRngMode(modparam.src.srcbuf);
								}
								else if(enabled && !RunTime->aktsamples)
								{
									enabled=0;
									GT_SetGadgetAttrs(modGadgets[GADIX_GO],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
								}
								SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);
							}
							if(sigout&(1L<<modWin->UserPort->mp_SigBit))
							{
								while(imsg=GT_GetIMsg(modWin->UserPort))
								{
									iclass	=imsg->Class;
									iadr	=imsg->IAddress;
									icode	=imsg->Code;
									isec	=imsg->Seconds;
									imic	=imsg->Micros;
									GT_ReplyIMsg(imsg);
									switch(iclass)
									{
										case IDCMP_GADGETUP:
											gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
											gh=HandleSource(&modparam.src,modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode);
											if(!gh) gh=HandleCfgSel   (&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
											if(!gh)
											{
												switch(gnr)
												{
													case GADID_GO:
														SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");		/* Store actual settings */
														ret=TRUE;runing=FALSE;
														break;
													case GADID_CFGLIST:
													case GADID_CFGNAME:
													case GADID_CFGDEL:
														switch(cfgsel.action) {
															case CFGSEL_ACTION_LOAD:
																DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
																if(!strnicmp(cfgsel.aktcfg->ln_Name," default",8)) RecallDefFXParams();
																else {
																	if(!strnicmp(cfgsel.aktcfg->ln_Name," current",8)) RecallLastFXParams();
																	else { sprintf(fn,"%s/%s.cfg",cfgsel.path,cfgsel.aktcfg->ln_Name);LoadFXParams(fn); }
																}
																InitFXParams(moddesc);						// init new settings
																RefreshGUI(modGadgets,modWin);
																break;
															case CFGSEL_ACTION_GO:
																SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");		/* Store actual settings */
																ret=TRUE;runing=FALSE;
																break;
														}
														break;
													case GADID_CFGADD:
														nr=0;sprintf(cn,"New%02d.cfg",nr);
														while((FindName(&(cfgsel.cfgs),cn)) && (nr<100)) { nr++;sprintf(cn,"New%02d.cfg",nr); }
														if(nr<100) {
															strmfp(fn,cfgsel.path,cn);SaveFXParams(fn);
															strsfn(fn,NULL,NULL,cn,NULL);
															if(node=AddCfgSelConfig(&cfgsel,cn)) {
																cfgsel.aktcfg=node;SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL);
																ActivateGadget(modGadgets[GADIX_CFGNAME],modWin,0l);
															}
															else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
														}
														break;
													case GADID_TIMES:
														HandleParameter(modparam.times,modWin,modGadgets[[GADIX_TIMES]);
														break;
												}
											}
											break;
										case IDCMP_VANILLAKEY:
											switch(icode)
											{
												case KEY_ENTER:
												case KEY_RETURN:
													SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");		/* Store actual settings */
													ret=TRUE;runing=FALSE;
													break;
												case KEY_ESC:
													runing=FALSE;
													break;
												case KEY_TAB:
													ActivateGadget(modGadgets[GADIX_TIMES],modWin,0l);
													break;
												case '?':
													Message(strNoID,PRJ_VERS,NULL,0l);
													break;
											}
											break;
										case IDCMP_CLOSEWINDOW:
											runing=FALSE;
											break;
										case IDCMP_RAWKEY:
											if(icode==RKEY_HELP) Help("html/nodes/"HELPBASENAME_OP"."PRJ_NAME".html",0,0);
											break;
										case IDCMP_MENUPICK:
											while(icode!=MENUNULL)
											{
												if(item=ItemAddress(RunTime->FXMenu,icode))
												{
													switch((ULONG)MENU_USERDATA(item))
													{
														case MENID_LOAD:
															MODFReqtags[9].ti_Data=FALSE;
															if(AslRequest(modFRequest,MODFReqtags))
															{
																strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
																DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
																LoadFXParams(fn);InitFXParams(moddesc);						// init new settings
																RefreshGUI(modGadgets,modWin);
															}
															break;
														case MENID_SAVE:
															MODFReqtags[9].ti_Data=TRUE;
															if(AslRequest(modFRequest,MODFReqtags))
															{
																strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
																SaveFXParams(fn);strsfn(fn,NULL,NULL,cn,NULL);
																if(!strcmp(modFRequest->fr_Drawer,cfgsel.path)) {
																	if(!FindName(&(cfgsel.cfgs),cn)) {
																		if(node=AddCfgSelConfig(&cfgsel,cn)) {
																			cfgsel.aktcfg=node;SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL);
																		}
																		else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
																	}
																}
															}
															break;
														case MENID_RESET:
															DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
															RecallLastFXParams();InitFXParams(moddesc);					// init new settings
															RefreshGUI(modGadgets,modWin);
															break;
														case MENID_DEFAULT:
															DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
															RecallDefFXParams();InitFXParams(moddesc);					// init new settings
															RefreshGUI(modGadgets,modWin);
															break;
														case MENID_GO:
															SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");		/* Store actual settings */
															ret=TRUE;runing=FALSE;
															break;
														case MENID_HELP:
															Help("/_operators/"PRJ_NAME".guide/main",RunTime->fxoff,RunTime->anzfx);
															break;
														case MENID_ABOUT:
															Message(strNoID,PRJ_VERS,NULL,0l);
															break;
														case MENID_QUIT:
															runing=FALSE;
															break;
													}
													icode=item->NextSelect;
												}
											}
											break;
									}
								}
							}
						}
						ClearMenuStrip(modWin);
						CloseWindow(modWin);
					}
					else Message(errOpenWindow,NULL,__FILE__,__LINE__);
					FreeAslRequest(modFRequest);
				}
				else Message(errAllocateFileRequest,NULL,__FILE__,__LINE__);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
			FreeGadgets(modGList);
			DoneCfgSel(&cfgsel);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		if(((struct Node *)&nt)==RunTime->gblist.lh_Head) RemHead(&RunTime->gblist);
		else if(((struct Node *)&nt)==RunTime->gblist.lh_Tail) RemTail(&RunTime->gblist);
		else Remove((struct Node *)&nt);
		FreeSignal(nt.sig);
	}
	else Message(errAllocateSignal,NULL,__FILE__,__LINE__);
	return(ret);
}

UBYTE __saveds __asm LIBProcess(REG(d0,UBYTE callmd),REG(a0,SInfo *si),REG(a1,char *fn))
{
	ULONG curlen;
	register UBYTE l;
	register ULONG j,curlen2;
	PWData *pwd;
	ULONG rlen,anz;
	char title[80];
	SInfo *dstbuf,*srcbuf;
	ULONG rm_start,rm_len;
	UBYTE ret=1;
	Param param;
//	UBYTE resfail=0;

	if(callmd==CM_REXX)
	{
		UnlockBuffer(modparam.src.srcbuf);
		ret&=((modparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret) return(ret);
	}

	srcbuf=modparam.src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);

	UC_ConvertParameter(param,modparam.times,UGRP_COUNT,COUN_ABS);
	anz=atoi(param);
	rlen=rm_len*anz;

	if(dstbuf=NewSample())
	{
		BufferFill(srcbuf,dstbuf);
		dstbuf->slen=(srcbuf->slen-rm_len)+rlen;
		dstbuf->zoomxl=dstbuf->slen;
		if(AllocSampleMem(dstbuf,dstbuf->slen,(srcbuf->channels+1)))
		{
			if((pwd=NewProWin()))										// ein neues asynchrones Statusfenster öffnen
			{
				for(l=0;l<(srcbuf->channels+1);l++)
				{
					if(srcbuf->channelmask&(1<<l))
					{
						if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);
						StartProWin(pwd,&curlen,title,rm_len);					// den Statusbalken starten
						curlen2=0;
						for(j=0;((j<anz) && (l<4));j++)
						{
							CopyDataCh(srcbuf,dstbuf,l,rm_start,(rm_start+curlen2),rm_len);
							Forbid();
							if(ProWinAborted(curlen)) l=4;					// es wurde abgebrochen
							else curlen2=(curlen+=rm_len);
							Permit();
						}
						if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rlen),(srcbuf->slen-(rm_start+rm_len)));
					}
					else CopyDataCh(srcbuf,dstbuf,l,0,0,srcbuf->slen);
				}
				AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
				DelProWin(pwd);
			}
		}
		else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);ret=1; }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);ret=1; }
	return(ret);
}

void __saveds __asm LIBHandleARexxCmd(REG(a0,char *params),REG(a1,char *ret))
{
	UBYTE cf=0,pf;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam"))
	{
		cf=1;pf=0;
		if(       !stricmp(RexxPar2,"Repeat"))		{ strncpy(modparam.times,RexxPar3,PARBUFLEN);modparam.times[PARBUFLEN]=0;pf=1; }
		if(pf) SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam"))
	{
		cf=1;/*pf=0;*/
		if(       !stricmp(RexxPar2,"Repeat"))		{ strncpy(ret,modparam.times,PARBUFLEN);ret[PARBUFLEN]=0;/*pf=1;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetOperatorPreset"))
	{
		if(!stricmp(RexxPar2,"default")) { DoneFXParams(moddesc);FreeFXParams(moddesc);RecallDefFXParams();InitFXParams(moddesc); }
		else { sprintf(fn,"progdir:_operators/"PRJ_NAME"/%s.cfg",RexxPar2);DoneFXParams(moddesc);FreeFXParams(moddesc);LoadFXParams(fn);InitFXParams(moddesc); }
		SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
}

//-- local Stuff --------------------------------------------------------------

void LoadFXParams(char *fname)
{
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,moddesc);
	strcpy(modparam.times,CfgIO_ReadString	(cfg,"General"	,"Repeat"	,"10"));
}

void SaveFXParams(char *fname)
{
	SaveModSettings(cfg,moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Repeat"	,modparam.times);

	CfgIO_WriteConfig(cfg,fname);
}

void DefaultFXParams(void)
{
	modparam.src.srcbuf=0L;
	modparam.src.rmode=RNGMD_ALL;

	UC_ConvertParameter(modparam.times,"10",UGRP_COUNT,COUN_ABS);
}

void RecallLastFXParams(void)
{
	if		(!access( "env:sfx/operators/"PRJ_NAME".cfg",F_OK|R_OK))		 LoadFXParams( "env:sfx/operators/"PRJ_NAME".cfg");
	else if	(!access("progdir:_operators/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadFXParams("progdir:_operators/"PRJ_NAME"/default.cfg");
	else																	 DefaultFXParams();
}

void RecallDefFXParams(void)
{
	if		(!access("progdir:_operators/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadFXParams("progdir:_operators/"PRJ_NAME"/default.cfg");
	else																	 DefaultFXParams();
}

void RefreshGUI(struct Gadget *modGadgets[],struct Window *modWin)
{
	SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_TIMES],modWin,0l,GTST_String,modparam.times,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
