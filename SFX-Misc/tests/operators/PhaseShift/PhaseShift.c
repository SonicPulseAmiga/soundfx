/****h* PhaseShift/PhaseShift.c [1.00] *
*
*  NAME
*    PhaseShift.c
*  COPYRIGHT
*    $VER: PhaseShift.c 1.00 201.12.99) © by Stefan Kost 1998-1999
*  FUNCTION
*    PhaseShift Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    27.Nov.1998
*  MODIFICATION HISTORY
*    01.Dec.1999	V 1.00	most recent version
*    27.Nov.1999	V 1.00	initial version
*  NOTES
*
*******
*/

#define PHASESHIFT_C

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

void LoadFXParams	(char *fname);
void SaveFXParams	(char *fname);
void DefaultFXParams(void);
void RecallFXParams	(void);

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

#define GADID_MOD1		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC+SRC_IXCT)

#define GADID_MOD2		(GADID_MOD1+MOD_IDCT)
#define GADIX_MOD2		(GADIX_MOD1+MOD_IXCT)

#define GADID_ANZ		(GADID_MOD2+MOD_IDCT)
#define GADIX_ANZ		(GADIX_MOD2+MOD_IXCT)

#define GADID_WFKT		(GADID_ANZ+1)
#define GADIX_WFKT		(GADIX_ANZ+1)

#define LAST_GADIX		(GADIX_WFKT+WFKT_IXCT)

//-- Menu ID's

#define MENID_LOAD		 0
#define MENID_SAVE		 1
#define MENID_RESET		 2
#define MENID_GO		 3
#define MENID_HELP		 4
#define MENID_ABOUT		 5
#define MENID_QUIT		 6

//-- globals ------------------------------------------------------------------

struct
{
	Source			src;		/* Quelle */
	Modulator		mod1;		/* Effektanteil */
	Modulator		mod2;		/* Phasenwinkel */
	WinFunction		wfkt1;		/* Fensterfunktion */
	Param			anz;		/* Anzahl Filtercoeffizienten */
} modparam={0};

PrefsData moddesc[]=
{
	{ PD_SOURCE,		"S1",	&(modparam.src)		},
	{ PD_MODULATOR,		"M1",	&(modparam.mod1)	},
	{ PD_MODULATOR,		"M2",	&(modparam.mod2)	},
	{ PD_WINFUNCTION,	"W1",	&(modparam.wfkt1)	},
	{ PD_END,			0l,		0l					}	
};

RTime 		*RunTime=NULL;
APTR 		cfg=NULL;

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
	if(!(AslBase		=OpenLibrary("asl.library",37))) goto Error;
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion))) goto Error;
	if(!(DiskfontBase	=OpenLibrary("diskfont.library",37))) goto Error;
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37))) goto Error;
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L))) goto Error;
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37))) goto Error;
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) goto Error;
	if(!(SFXSupportBase	=OpenLibrary("progdir:libs/"SFXSupportName,SFXSupportVersion))) goto Error;
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion))) goto Error;
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
	RecallFXParams();				// Default oder LastUsed values setzen
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
	UBYTE ret=FALSE,runing=TRUE,gh,enabled;
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+MODSEL_HEIGHT;
	UWORD ys5=ys4+1+RunTime->scy[FRAME_HEIGHT(1)];
	UWORD winh=ys5+2+CMDFRM_HEIGHT;
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
			ng.ng_TopEdge		=ys5+RunTime->scy[2];
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

			g=AddModulator(&modparam.mod1,modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys2);
			g=AddModulator(&modparam.mod2,modGadgets,g,GADID_MOD2,GADIX_MOD2,xs1,ys3);

			g=AddWinFunction(&modparam.wfkt1,modGadgets,g,GADID_WFKT,GADIX_WFKT,xs1,ys4+RunTime->scy[2]);

			ng.ng_LeftEdge		=xs1+RunTime->scx[254];
			ng.ng_TopEdge		=ys4+RunTime->scy[2];
			ng.ng_Width			=RunTime->scx[50];
			ng.ng_GadgetText	="Num";
			ng.ng_TextAttr		=RunTime->ScrFont;
			ng.ng_GadgetID		=GADID_ANZ;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.anz,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_ANZ]=g;

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
						RText(modrp,"Src"),xs1+RunTime->scx[29],ys1+  RunTime->scy[TEXT_Y_POS(1)];
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+3+RunTime->scy[TEXT_Y_POS(2)+7]);
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+3+RunTime->scy[TEXT_Y_POS(2)+7]);
						RText(modrp,"Window",xs1+RunTime->scx[45],ys4+  RunTime->scy[TEXT_Y_POS(1)]);

						SetAPen(modrp,7);
						Move(modrp,xs1+RunTime->scx[5],ys2+RunTime->scy[6]);Text(modrp,"Effect",6);
						Move(modrp,xs1+RunTime->scx[5],ys3+RunTime->scy[6]);Text(modrp,"Phase",5);

						xh=STDFRM_WIDTH;
						DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
						DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
						DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Par */
						DrawGadWB(modrp,xs1,ys4,xh,RunTime->scy[FRAME_HEIGHT(1)]);	/* Par */
						DrawGadWB(modrp,xs1,ys5,xh,CMDFRM_HEIGHT);					/* Go */
						DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,winh-2);				/* CfgSel */

						SetCfgSel     (&cfgsel        ,modWin,modGadgets,GADIX_CFGSEL);
						SetSource     (&modparam.src  ,modWin,modGadgets,GADIX_SRC);
						SetModulator  (&modparam.mod1 ,modWin,modGadgets,GADIX_MOD1);
						SetModulator  (&modparam.mod2 ,modWin,modGadgets,GADIX_MOD2);
						SetWinFunction(&modparam.wfkt1,modWin,modGadgets,GADIX_WFKT);

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
													gh=HandleSource     (&modparam.src,modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode);
											if(!gh) gh=HandleModulator  (&modparam.mod1,&modparam.src,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode);
											if(!gh) gh=HandleModulator  (&modparam.mod2,&modparam.src,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode);
											if(!gh) gh=HandleWinFunction(&modparam.wfkt1,modWin,modGadgets,GADID_WFKT,GADIX_WFKT,gnr,icode);
											if(!gh) gh=HandleCfgSel     (&cfgsel        ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
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
														switch(cfgsel.action)
														{
															case CFGSEL_ACTION_LOAD:
																DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
																if(!strnicmp(cfgsel.aktcfg->ln_Name,"_default",8)) RecallFXParams();
																else
																{
																	if(!strnicmp(cfgsel.aktcfg->ln_Name,"_current",8)) strcpy(fn,"env:sfx/operators/"PRJ_NAME".cfg");
																	else sprintf(fn,"progdir:_operators/"PRJ_NAME"/%s.cfg",cfgsel.aktcfg->ln_Name);
																	LoadFXParams(fn);
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
														MODFReqtags[9].ti_Data=TRUE;
														if(AslRequest(modFRequest,MODFReqtags))
														{
															strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
															SaveFXParams(fn);strsfn(fn,NULL,NULL,cn,NULL);
															if(!FindName(&(cfgsel.cfgs),cn))
															{
																if(node=(struct Node *)MyAllocMem(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR))
																{
																	node->ln_Name=strdup(cn);SortIn(&(cfgsel.cfgs),node);
																	cfgsel.aktcfg=node;SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL);
																}
																else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
															}
														}
														break;
													case GADID_ANZ:
														HandleParameter(modparam.anz,modWin,modGadgets[GADIX_ANZ]);
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
												case '?':
													Message(strNoID,PRJ_VERS,NULL,0l);
													break;
											}
											break;
										case IDCMP_CLOSEWINDOW:
											runing=FALSE;
											break;
										case IDCMP_RAWKEY:
											if(icode==0x5F) Help("/_operators/"PRJ_NAME".guide/main",RunTime->fxoff,RunTime->anzfx);
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
																if(!FindName(&(cfgsel.cfgs),cn))
																{
																	if(node=(struct Node *)MyAllocMem(sizeof(struct Node),MEMF_ANY|MEMF_CLEAR))
																	{
																		node->ln_Name=strdup(cn);SortIn(&(cfgsel.cfgs),node);
																		cfgsel.aktcfg=node;SetCfgSel(&cfgsel,modWin,modGadgets,GADIX_CFGSEL);
																	}
																	else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
																}
															}
															break;
														case MENID_RESET:
															DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
															RecallFXParams();InitFXParams(moddesc);						// init new settings
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
	register ULONG j,curlen2,m;
	register UBYTE l;
	PWData *pwd;
	double effs,effe,effd,eff,dry;
	double frqs,frqe,frqd,frq;
	double sample;
	LONG val1;
	double *matrix=0l,*mat;
	double fg,ff,f;
	double si0,si1,fc;
	UWORD anz,anz2;
	double *win_fkt=0l,*wptr;
	char title[80];
	SInfo *dstbuf,*srcbuf;
	ULONG rm_start,rm_len,dpos;
	register SAMPLE *sbuf,*dbuf,*rbufw,*rbufr;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *rbuf,*rbufbase;
	ULONG rbufmask,rbufsize;
	UBYTE ret=1;
	char tstr[PARBUFLEN+1];
	UBYTE resfail=0;

	if(callmd==CM_REXX)
	{
		UnlockBuffer(modparam.src.srcbuf);
		ret&=((modparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret) return(ret);
	}

	srcbuf=modparam.src.srcbuf;

	moddata1=(void *)(modparam.mod1.bshpdata);
	modfunc1=modparam.mod1.modfunc;
	UC_ConvertParameter(tstr,modparam.mod1.vals,UGRP_PROPOTION,PROP_FC);
	effs=atof(tstr);
	UC_ConvertParameter(tstr,modparam.mod1.vale,UGRP_PROPOTION,PROP_FC);
	effe=atof(tstr);
	effd=effe-effs;

	moddata2=(void *)(modparam.mod2.bshpdata);
	modfunc2=modparam.mod2.modfunc;
	UC_ConvertParameter(tstr,modparam.mod2.vals,UGRP_PHASE,PHA_RAD);
	frqs=atof(tstr);
	UC_ConvertParameter(tstr,modparam.mod2.vale,UGRP_PHASE,PHA_RAD);
	frqe=atof(tstr);
	frqd=frqe-frqs;

	UC_ConvertParameter(tstr,modparam.anz,UGRP_COUNT,COUN_ABS);
	anz=atoi(tstr);
	if(!(anz&1)) anz++;		// make it an odd number
	if(anz<3) anz=3;
	if(anz>1023) anz=1023;
	
	SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);

	if(dstbuf=NewSample())
	{
		if(!(AllocSampleMem(dstbuf,srcbuf->slen,(srcbuf->channels+1)))) resfail=1;
		if(!resfail) if(!(win_fkt=(double *)MyAllocMem((anz<<1)*sizeof(double),MEMF_ANY))) resfail=1;
		if(!resfail) if(!(matrix =(double *)MyAllocMem( anz    *sizeof(double),MEMF_ANY))) resfail=1;
		if(!resfail) if(!(rbuf=NewRingBuf(anz,&rbufbase,&rbufmask,&rbufsize))) resfail=1;
		if(!resfail)
		{
			BufferFill(srcbuf,dstbuf);
			GenWindow((anz<<1),modparam.wfkt1.win,&modparam.wfkt1.par,win_fkt);
			m=(anz-1)>>1;matrix[m]=0.0;									// design hilbert filter
			for(j=1;j<=m;j++)
			{
//				f=(j&1)?2.0/(j*PI):0.0;
				f=(j&1)?1.0/j:0.0;
				matrix[m+j]= f*win_fkt[m+j];
       			matrix[m-j]=-f*win_fkt[m-j];
			}
			if((pwd=NewProWin()))										// ein neues asynchrones Statusfenster öffnen
			{
				for(l=0;l<(srcbuf->channels+1);l++)
				{
					AssignBShape(&modparam.mod1);
					AssignBShape(&modparam.mod2);
					if(srcbuf->channelmask&(1<<l))
					{
						if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
						srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
						dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
						if(srcsa && dstsa)
						{
							SARead =srcsa->safunc;
							SAWrite=dstsa->safunc;
							memset(rbuf,0,(rbufsize<<1));
							rbufw=rbufbase;
							dbuf=(SAMPLE *)dstsa->buffer1;dpos=0;						// Pointer auf dst-Block
							sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);
							StartProWin(pwd,&curlen,title,rm_len);					// den Statusbalken starten
							curlen2=0;
							for(srcsa->seg=dstsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++,dstsa->seg++)
							{
								sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
								for(j=0;(j<Bytes2Samples(srcsa->blen) && curlen<rm_len);j++,curlen2=++curlen)	// die Blocklänge durchgehen
								{
									*rbufw=*(sbuf++);											// get current value
									rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
									if(curlen>m)
									{
										eff=effs+effd*modfunc1((curlen2-m),rm_len,moddata1);dry=1.0-eff;
//										frq=frqs+frqd*modfunc2((curlen2-m),rm_len,moddata2);
																							// build matrix
										sample=0.0;anz2=anz;mat=matrix;						// apply filter
										rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-anz)&rbufmask));
										while(anz2--)
										{
											sample+=(*(++mat)*(double)*rbufr);
											rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
										}
										rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-(m+1))&rbufmask));
										val1=(LONG)((double)*rbufr*dry+sample*eff);
										CLIPINT(val1);
										*(dbuf++)=(WORD)val1;
										if(dpos==Bytes2Samples(dstsa->blen))
										{
											SAWrite(dstsa,dstsa->blen);dstsa->seg++;					// akt Block schreiben
											dbuf=(SAMPLE *)dstsa->buffer1;dpos=0;						// Pointer auf dst-Block
										}
									}
								}
							}
							for(j=0;j<m;j++,curlen2++)	// den Rest berechnen
							{
								*rbufw=0.0;											// pad with 0.0
								rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
								eff=effs+effd*modfunc1((curlen2-m),rm_len,moddata1);dry=1.0-eff;
//								frq=frqs+frqd*modfunc2((curlen2-m),rm_len,moddata2);
																							// build matrix
								sample=0.0;anz2=anz;mat=matrix;						// apply filter
								rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-anz)&rbufmask));
								while(anz2--)
								{
									sample+=(*(++mat)*(double)*rbufr);
									rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
								}
								rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-(m+1))&rbufmask));
								val1=(LONG)((double)*rbufr*dry+sample*eff);
								CLIPINT(val1);
								*(dbuf++)=(WORD)val1;
								if(dpos==Bytes2Samples(dstsa->blen))
								{
									SAWrite(dstsa,dstsa->blen);dstsa->seg++;					// akt Block schreiben
									dbuf=(SAMPLE *)dstsa->buffer1;dpos=0;						// Pointer auf dst-Block
								}
							}
							if(curlen>=0xFFFFFFF0) l=4;					// es wurde abgebrochen
						}
						if(srcsa) SADone(srcsa,srcbuf);
						if(dstsa) SADone(dstsa,dstbuf);
						if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					}
					else CopyDataCh(srcbuf,dstbuf,l,0,0,srcbuf->slen);
				}
				AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);
				DelProWin(pwd);
			}
		}
		else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);ret=1; }
		if(win_fkt) MyFreeMem((ULONG *)win_fkt,((anz<<1)*sizeof(double)));
		if(matrix)	MyFreeMem((ULONG *)matrix ,( anz    *sizeof(double)));
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
		cf=1;	pf=ParseModulatorSet  (&modparam.mod1,"P1",params);
		if(!pf) pf=ParseModulatorSet  (&modparam.mod2,"P2",params);
		if(!pf) pf=ParseWinFunctionSet(&modparam.wfkt1,"W1",params);
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strncpy(modparam.anz  ,RexxPar3,PARBUFLEN);modparam.anz  [PARBUFLEN]=0;pf=1; }
		if(pf) SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam"))
	{
		cf=1;	pf=ParseModulatorGet  (&modparam.mod1,"P1",params,ret);
		if(!pf) pf=ParseModulatorGet  (&modparam.mod2,"P2",params,ret);
		if(!pf) pf=ParseWinFunctionGet(&modparam.wfkt1,"W1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Length"))		{ strncpy(ret,modparam.anz  ,PARBUFLEN);ret[PARBUFLEN]=0;/*pf=1;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetOperatorPreset"))
	{
		if(!stricmp(RexxPar2,"default")) { DoneFXParams(moddesc);FreeFXParams(moddesc);RecallFXParams();InitFXParams(moddesc); }
		else { sprintf(fn,"progdir:_operators/"PRJ_NAME"/%s.cfg",RexxPar2);DoneFXParams(moddesc);FreeFXParams(moddesc);LoadFXParams(fn);InitFXParams(moddesc); }
		SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
}

//-- local Stuff --------------------------------------------------------------

void LoadFXParams(char *fname)
{
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,moddesc);
	strcpy(modparam.anz,CfgIO_ReadString	(cfg,"General"	,"Length"	,"16"));
}

void SaveFXParams(char *fname)
{
	SaveModSettings(cfg,moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Length"	,modparam.anz);

	CfgIO_WriteConfig(cfg,fname);
}

void DefaultFXParams(void)
{
	modparam.src.srcbuf=0L;
	modparam.src.rmode=RNGMD_ALL;

	UC_ConvertParameter(modparam.mod1.vals,"1.0",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(modparam.mod1.vale,"1.0",UGRP_PROPOTION,PROP_FC);
	modparam.mod1.bshape=BSHP_NONE;

	UC_ConvertParameter(modparam.mod2.vals,"90 °",UGRP_PHASE,PHA_D);
	UC_ConvertParameter(modparam.mod2.vale,"90 °",UGRP_PHASE,PHA_D);
	modparam.mod2.bshape=BSHP_NONE;

	modparam.wfkt1.win=WIN_HAMMING;
	modparam.wfkt1.par=0.54;

	UC_ConvertParameter(modparam.anz,"16" ,UGRP_COUNT,COUN_ABS);
}

void RecallFXParams(void)
{
	if		(!access( "env:sfx/operators/"PRJ_NAME".cfg",F_OK|R_OK))		 LoadFXParams( "env:sfx/operators/"PRJ_NAME".cfg");
	else if	(!access("progdir:_operators/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadFXParams("progdir:_operators/"PRJ_NAME"/default.cfg");
	else																	 DefaultFXParams();
}

void RefreshGUI(struct Gadget *modGadgets[],struct Window *modWin)
{
	SetSource     (&modparam.src,  modWin,modGadgets,GADIX_SRC);
	SetModulator  (&modparam.mod1, modWin,modGadgets,GADIX_MOD1);
	SetModulator  (&modparam.mod2, modWin,modGadgets,GADIX_MOD2);
	SetWinFunction(&modparam.wfkt1,modWin,modGadgets,GADIX_WFKT);
	GT_SetGadgetAttrs(modGadgets[GADIX_ANZ],modWin,0l,GTST_String,modparam.anz,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
