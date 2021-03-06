/****h* Delay/Delay.c [1.20] *
*
*  NAME
*    Delay.c
*  COPYRIGHT
*    $VER: Delay.c 1.20 (11.03.00) ? by Stefan Kost 1998-2000
*  FUNCTION
*    Delay Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    11.Mar.2000	V 1.20	most recent version
*							* renaming
*								curlen -> curpos
*								curlen2 -> curpos2
*							* precalc
*								blen=Byte2Samples(srcsa->blen)
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define DELAY_C

//-- includes -----------------------------------------------------------------

#include <libraries/sfx_mod.h>

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

//-- Initialisation and Destruction

int __asm __UserLibInit(REG(a6) struct Library *SFX_FXBase);
void __asm __UserLibCleanup(REG(a6) struct Library *SFX_FXBase);

//-- DataInitialisation and Destruction

void __asm LIBClientDataInit(REG(a0) RTime *RunTime_);
void __asm LIBClientDataDone(void);

//-- Modulroutienen

UBYTE __asm LIBInterface(void);
UBYTE __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn);
void __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret);

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

#define GADID_MOD3		(GADID_MOD2+MOD_IDCT)
#define GADIX_MOD3		(GADIX_MOD2+MOD_IXCT)

#define GADID_AMPF		(GADID_MOD3+MOD_IDCT)
#define GADIX_AMPF		(GADIX_MOD3+MOD_IXCT)

#define GADID_DRY		(GADID_AMPF+1)
#define GADIX_DRY		(GADIX_AMPF+1)

#define GADID_INTER		(GADID_DRY+1)
#define GADIX_INTER		(GADIX_DRY+1)

#define LAST_GADIX		(GADIX_INTER+INT_IXCT)

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
	Modulator		mod2;		/* Feedback */
	Modulator		mod3;		/* Delay */
	Interpolator	int1;		/*?Interpolator */
	Param 			ampf;		/* Verst?rkung/Abschw?chung */
	UBYTE			dry;		/* Modus */
} modparam={0};

PrefsData moddesc[]=
{
	{ PD_SOURCE,		"S1",	&(modparam.src)		},
	{ PD_MODULATOR,		"M1",	&(modparam.mod1)	},
	{ PD_MODULATOR,		"M2",	&(modparam.mod2)	},
	{ PD_MODULATOR,		"M3",	&(modparam.mod3)	},
	{ PD_INTERPOLATOR,	"I1",	&(modparam.int1)	},
	{ PD_END,			0l,		0l					}	
};

RTime 		*RunTime=NULL;
APTR 		cfg=NULL;

char *DryLabels[]={ "Dry=1-Eff","Dry=-Eff",0l };

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

int __saveds __asm __UserLibInit(REG(a6) struct Library *SFX_FXBase)
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
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) ? by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFX_FXBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6) struct Library *SFX_FXBase)
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

void __saveds __asm LIBClientDataInit(REG(a0) RTime *RunTime_)
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

#define FXCore do {																					\
	rbufr=(double)Bytes2Samples((ULONG)rbufw&rbufmask)-del;		/* only Offset to baseaddr */		\
	if(rbufr<0.0) rbufr+=(double)Bytes2Samples(rbufsize);											\
	val2=InterpolFunc(rbufbase,&rbufr,rbufmask,irange);			/* get Value */						\
																									\
	val1=(val2*eff+(double)*sbuf*dry)*ampf;															\
	CLIPINT(val1);																					\
	*(dbuf++)=(SAMPLE)val1;																			\
	*rbufw=(SAMPLE)(val2*fb+(double)*(sbuf++)*dr);													\
	rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));									\
} while(0)


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
	UWORD ys5=ys4+1+MODSEL_HEIGHT;
	UWORD ys6=ys5+1+RunTime->scy[FRAME_HEIGHT(2)];
	UWORD winh=ys6+2+CMDFRM_HEIGHT;
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
			ng.ng_TopEdge		=ys6+RunTime->scy[2];
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
			g=AddModulator(&modparam.mod3,modGadgets,g,GADID_MOD3,GADIX_MOD3,xs1,ys4);

			ng.ng_LeftEdge		=xs1+RunTime->scx[53];
			ng.ng_TopEdge		=ys5+RunTime->scy[2];
			ng.ng_Width			=RunTime->scx[60];
			ng.ng_GadgetText	="Ampf";
			ng.ng_TextAttr		=RunTime->ScrFont;
			ng.ng_GadgetID		=GADID_AMPF;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.ampf,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_AMPF]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[214];
			ng.ng_Width			=RunTime->scx[90];
			ng.ng_GadgetText	="Dry";
			ng.ng_GadgetID		=GADID_DRY;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,DryLabels,GTCY_Active,modparam.dry,TAG_DONE);
			modGadgets[GADIX_DRY]=g;

			g=AddInterpolator(&modparam.int1,modGadgets,g,GADID_INTER,GADIX_INTER,xs1,ys5+RunTime->scy[14]);

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
						RText(modrp,"Src",xs1+RunTime->scx[29],ys1+  RunTime->scy[TEXT_Y_POS(1)]);
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+3+RunTime->scy[TEXT_Y_POS(2)+7]);
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys3+3+RunTime->scy[TEXT_Y_POS(2)+7]);
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys4+3+RunTime->scy[TEXT_Y_POS(2)+7]);

						SetAPen(modrp,7);
						Move(modrp,xs1+RunTime->scx[5],ys2+RunTime->scy[6]);Text(modrp,"Effect",6);
						Move(modrp,xs1+RunTime->scx[5],ys3+RunTime->scy[6]);Text(modrp,"Feedback",8);
						Move(modrp,xs1+RunTime->scx[5],ys4+RunTime->scy[6]);Text(modrp,"Delay",5);

						xh=STDFRM_WIDTH;
						DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
						DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
						DrawGadWB(modrp,xs1,ys3,xh,MODSEL_HEIGHT);					/* Fb */
						DrawGadWB(modrp,xs1,ys4,xh,MODSEL_HEIGHT);					/* Delay */
						DrawGadWB(modrp,xs1,ys5,xh,RunTime->scy[FRAME_HEIGHT(2)]);	/* Par */
						DrawGadWB(modrp,xs1,ys6,xh,CMDFRM_HEIGHT);					/* Go */
						DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,winh-2);				/* CfgSel */

						SetCfgSel      (&cfgsel       ,modWin,modGadgets,GADIX_CFGSEL);
						SetSource      (&modparam.src ,modWin,modGadgets,GADIX_SRC);
						SetModulator   (&modparam.mod1,modWin,modGadgets,GADIX_MOD1);
						SetModulator   (&modparam.mod2,modWin,modGadgets,GADIX_MOD2);
						SetModulator   (&modparam.mod3,modWin,modGadgets,GADIX_MOD3);
						SetInterpolator(&modparam.int1,modWin,modGadgets,GADIX_INTER);

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
											if(!gh) gh=HandleModulator   (&modparam.mod1,&modparam.src,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode);
											if(!gh) gh=HandleModulator   (&modparam.mod2,&modparam.src,0l,modWin,modGadgets,GADID_MOD2,GADIX_MOD2,gnr,icode);
											if(!gh) gh=HandleModulator   (&modparam.mod3,&modparam.src,0l,modWin,modGadgets,GADID_MOD3,GADIX_MOD3,gnr,icode);
											if(!gh) gh=HandleInterpolator(&modparam.int1,modWin,modGadgets,GADID_INTER ,GADIX_INTER ,gnr,icode);
											if(!gh) gh=HandleCfgSel		 (&cfgsel       ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic);
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
													case GADID_AMPF:
														HandleParameter(modparam.ampf,modWin,modGadgets[GADIX_AMPF]);
														break;
													case GADID_DRY:
														modparam.dry=icode;
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
													ActivateGadget(modGadgets[GADIX_MOD1],modWin,0l);
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

UBYTE __saveds __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn)
{
	ULONG curlen;
	register UBYTE l;
	register ULONG j,curlen2;
	PWData *pwd;
	double effs,effe,effd,eff,dry;				// for Modulation parameters
	double fbs,fbe,fbd,fb,dr;
	double dels,dele,deld,del;
	double ampf;
	LONG val1;
	double val2;
	char title[80];
	SInfo *dstbuf,*srcbuf;						// sampleaccess
	ULONG rm_start,rm_len,irange;
	register SAMPLE *sbuf,*dbuf,*rbufw;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;	// for BShapes
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *rbuf=0l,*rbufbase;
	ULONG delay,rbufmask,rbufsize;
	double rbufr;
	InterpolFuncPtr InterpolFunc;
	UBYTE ret=1,pu;
	Param param;
	UBYTE resfail=0;

// TIMER VAR
	char t1[8],t2[8];
	char tmsg[256];
// TIMER VAR

	if(callmd==CM_REXX)
	{
		UnlockBuffer(modparam.src.srcbuf);
		ret&=((modparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret) return(ret);
	}

	srcbuf=modparam.src.srcbuf;

	moddata1=(void *)(modparam.mod1.bshpdata);
	modfunc1=modparam.mod1.modfunc;
	UC_ConvertParameter(param,modparam.mod1.vals,UGRP_PROPOTION,PROP_FC);
	effs=atof(param);
	UC_ConvertParameter(param,modparam.mod1.vale,UGRP_PROPOTION,PROP_FC);
	effe=atof(param);
	effd=effe-effs;

	moddata2=(void *)(modparam.mod2.bshpdata);
	modfunc2=modparam.mod2.modfunc;
	UC_ConvertParameter(param,modparam.mod2.vals,UGRP_PROPOTION,PROP_FC);
	fbs=atof(param);
	UC_ConvertParameter(param,modparam.mod2.vale,UGRP_PROPOTION,PROP_FC);
	fbe=atof(param);
	fbd=fbe-fbs;

	moddata3=(void *)(modparam.mod3.bshpdata);
	modfunc3=modparam.mod3.modfunc;
	if(UC_GetParameterUnit(modparam.mod3.vals,UGRP_ABSFREQUENCY)!=INVALID_UNIT)
	{
		UC_ConvertParameter(param,modparam.mod3.vals,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		dels=(double)srcbuf->srat/del;
	}
	else
	{
		if((pu=UC_GetParameterUnit(modparam.mod3.vals,UGRP_ABSTIME))!=INVALID_UNIT)
		{
			if(pu==ATME_SAMPLES) dels=atof(modparam.mod3.vals);
			else { UC_ConvertParameter(param,modparam.mod3.vals,UGRP_ABSTIME,ATME_MS);dels=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,modparam.mod3.vals,UGRP_RELTIME,RTME_FC);dels=(double)srcbuf->slen*atof(param); }
	}
	if(dels<1.0) dels=1.0;
	if(UC_GetParameterUnit(modparam.mod3.vale,UGRP_ABSFREQUENCY)!=INVALID_UNIT)
	{
		UC_ConvertParameter(param,modparam.mod3.vale,UGRP_ABSFREQUENCY,AFRQ_HZ);if((del=atof(param))<1.0) del=1.0;
		dele=(double)srcbuf->srat/del;
	}
	else
	{
		if((pu=UC_GetParameterUnit(modparam.mod3.vale,UGRP_ABSTIME))!=INVALID_UNIT)
		{
			if(pu==ATME_SAMPLES) dele=atof(modparam.mod3.vale);
			else { UC_ConvertParameter(param,modparam.mod3.vale,UGRP_ABSTIME,ATME_MS);dele=atof(param)*((double)srcbuf->srat/1000.0); }
		}
		else { UC_ConvertParameter(param,modparam.mod3.vale,UGRP_RELTIME,RTME_FC);dele=(double)srcbuf->slen*atof(param); }
	}
	if(dele<1.0) dele=1.0;
	deld=dele-dels;

	UC_ConvertParameter(param,modparam.ampf,UGRP_AMPLITUDE,AMP_FC);
	ampf=atof(param);
	irange=(ULONG)(modparam.int1.range);
	delay=max((ULONG)dele,(ULONG)dels)+irange;

	SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);

	AssignInterpolFunc(modparam.int1.type,&InterpolFunc);

	if(dstbuf=NewSample())
	{
		if(!(AllocSampleMem(dstbuf,srcbuf->slen,(srcbuf->channels+1)))) resfail=1;
		if(!resfail) if(!(rbuf=NewRingBuf(delay,&rbufbase,&rbufmask,&rbufsize))) resfail=1;
		if(!resfail)
		{
			BufferFill(srcbuf,dstbuf);
			if((pwd=NewProWin()))										// ein neues asynchrones Statusfenster ?ffnen
			{
				for(l=0;l<(srcbuf->channels+1);l++)
				{
					AssignBShape(&modparam.mod1);							// reset moddata
					AssignBShape(&modparam.mod2);
					AssignBShape(&modparam.mod3);
					if(srcbuf->channelmask&(1<<l))
					{
						if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
						srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e lesen
						dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e schreiben
						if(srcsa && dstsa)
						{
							SARead =srcsa->safunc;
							SAWrite=dstsa->safunc;
							memset(rbuf,0,(rbufsize<<1));rbufw=rbufbase;
							sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);
							StartProWin(pwd,&curlen,title,rm_len);					// den Statusbalken starten
							curlen2=0;
// TIMER START
							getclk(t1);
// TIMER START
							if(modparam.dry) {
								for(srcsa->seg=dstsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++,dstsa->seg++) {
									sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
									j=0;
									while((j<Bytes2Samples(srcsa->blen)) && (curlen<rm_len)) {		// die Blockl?nge durchgehen
										eff=effs+effd*modfunc1(curlen2,rm_len,moddata1);dry=-eff;
										fb =fbs +fbd *modfunc2(curlen2,rm_len,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
										del=dels+deld*modfunc3(curlen2,rm_len,moddata3);
										curlen2=(curlen+=4);

										FXCore;FXCore;FXCore;FXCore;j+=4;
									}
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
							else {
								for(srcsa->seg=dstsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++,dstsa->seg++) {
									sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & n?chstes Read starten
									dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
									j=0;
									while((j<Bytes2Samples(srcsa->blen)) && (curlen<rm_len)) {		// die Blockl?nge durchgehen
										eff=effs+effd*modfunc1(curlen2,rm_len,moddata1);dry=(1.0-eff);
										fb =fbs +fbd *modfunc2(curlen2,rm_len,moddata2);dr =(fb>=0.0)    ? (1.0-fb) : (1.0+fb);
										del=dels+deld*modfunc3(curlen2,rm_len,moddata3);
										curlen2=(curlen+=4);

										FXCore;FXCore;FXCore;FXCore;j+=4;
									}
									SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
								}
							}
// TIMER STOP
							getclk(t2);
							if(t2[5]<t1[5]) t2[5]+=60;
							sprintf(tmsg,"Zeit : %d msek",(t2[5]*60*100+t2[6]*100+t2[7])-(t1[5]*60*100+t1[6]*100+t1[7]));
							Message(strNoID,tmsg,__FILE__,__LINE__);
// TIMER STOP
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
		else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);ret=TRUE; }
		if(rbuf)	DelRingBuf(rbuf,rbufsize);
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);ret=TRUE; }
	return(ret);
}

void __saveds __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret)
{
	UBYTE cf=FALSE,pf;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam"))
	{
		cf=TRUE;	pf=ParseModulatorSet   (&modparam.mod1,"P1",params);
		if(!pf) pf=ParseModulatorSet   (&modparam.mod2,"P2",params);
		if(!pf) pf=ParseModulatorSet   (&modparam.mod3,"P3",params);
		if(!pf) pf=ParseInterpolatorSet(&modparam.int1,"I1",params);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strncpy(modparam.ampf ,RexxPar3,PARBUFLEN);modparam.ampf[PARBUFLEN]=0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry"))			{ modparam.dry=CfgIO_MatchString(RexxPar3,DryLabels);pf=TRUE; }
		if(pf) SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam"))
	{
		cf=TRUE;	pf=ParseModulatorGet   (&modparam.mod1,"P1",params,ret);
		if(!pf) pf=ParseModulatorGet   (&modparam.mod2,"P2",params,ret);
		if(!pf) pf=ParseModulatorGet   (&modparam.mod3,"P3",params,ret);
		if(!pf) pf=ParseInterpolatorGet(&modparam.int1,"I1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Ampf"))		{ strncpy(ret,modparam.ampf ,PARBUFLEN);ret[PARBUFLEN]=0;pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Dry"))			{ strcpy(ret,DryLabels[modparam.dry]);/*pf=TRUE;*/ }
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
	strcpy(modparam.ampf,CfgIO_ReadString	(cfg,"General"	,"Ampf"	,modparam.ampf));
	modparam.dry=	CfgIO_MatchString(
					CfgIO_ReadString		(cfg,"General"	,"Dry"	,DryLabels[modparam.dry]),DryLabels);
}

void SaveFXParams(char *fname)
{
	SaveModSettings(cfg,moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,modparam.ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Dry"		,DryLabels[modparam.dry]);

	CfgIO_WriteConfig(cfg,fname);
}

void DefaultFXParams(void)
{
//	modparam.src.srcbuf=0L;
	modparam.src.rmode=RNGMD_ALL;

	UC_ConvertParameter(modparam.mod1.vals,"0.4",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(modparam.mod1.vale,"0.4",UGRP_PROPOTION,PROP_FC);
	modparam.mod1.bshape=BSHP_NONE;

	UC_ConvertParameter(modparam.mod2.vals,"0.4",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(modparam.mod2.vale,"0.4",UGRP_PROPOTION,PROP_FC);
	modparam.mod2.bshape=BSHP_NONE;

	UC_ConvertParameter(modparam.mod3.vals,"100 ms",UGRP_ABSTIME,ATME_MS);
	UC_ConvertParameter(modparam.mod3.vale,"100 ms",UGRP_ABSTIME,ATME_MS);
	modparam.mod3.bshape=BSHP_NONE;

	modparam.int1.range=2.0;
	modparam.int1.type=INT_TYPE_LIN;

	UC_ConvertParameter(modparam.ampf ,"1.25",UGRP_AMPLITUDE,AMP_FC);
	modparam.dry=0;
}

void RecallFXParams(void)
{
	if		(!access( "env:sfx/operators/"PRJ_NAME".cfg",F_OK|R_OK))		 LoadFXParams( "env:sfx/operators/"PRJ_NAME".cfg");
	else if	(!access("progdir:_operators/"PRJ_NAME"/default.cfg",F_OK|R_OK)) LoadFXParams("progdir:_operators/"PRJ_NAME"/default.cfg");
	else																	 DefaultFXParams();
}

void RefreshGUI(struct Gadget *modGadgets[],struct Window *modWin)
{
	SetSource      (&modparam.src ,modWin,modGadgets,GADIX_SRC);
	SetModulator   (&modparam.mod1,modWin,modGadgets,GADIX_MOD1);
	SetModulator   (&modparam.mod2,modWin,modGadgets,GADIX_MOD2);
	SetModulator   (&modparam.mod3,modWin,modGadgets,GADIX_MOD3);
	SetInterpolator(&modparam.int1,modWin,modGadgets,GADIX_INTER);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,modparam.ampf,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_DRY ],modWin,0l,GTCY_Active,modparam.dry ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
