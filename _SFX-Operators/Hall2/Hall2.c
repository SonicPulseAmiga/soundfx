/****h* Hall2/Hall2.c [4.1] *
*
*  NAME
*    Hall2.c
*  COPYRIGHT
*    $VER: Hall2.c 4.1 (27.08.00) ? by Stefan Kost 1998-2000
*  FUNCTION
*    Hall2 Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    27.Aug.2000	V 4.1	most recent version
*    02.Aug.2000	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*    old version - just here for comparision
*  IDEAS
*
*******
*/

#define HALL2_C

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

#define GADID_MOD1		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD1		(GADIX_SRC+SRC_IXCT)

#define GADID_ERFC		(GADID_MOD1+MOD_IDCT)
#define GADIX_ERFC		(GADIX_MOD1+MOD_IXCT)

#define GADID_MRFC		(GADID_ERFC+1)
#define GADIX_MRFC		(GADIX_ERFC+1)

#define GADID_DRFC		(GADID_MRFC+1)
#define GADIX_DRFC		(GADIX_MRFC+1)

#define GADID_AMPF		(GADID_DRFC+1)
#define GADIX_AMPF		(GADIX_DRFC+1)

#define GADID_FB		(GADID_AMPF+1)
#define GADIX_FB		(GADIX_AMPF+1)

#define LAST_GADIX		(GADIX_FB+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct
{
	Source			src;		/* Quelle */
	Modulator		mod;		/* Effektanteil */
	Param 			erfc;
	Param 			mrfc;
	Param 			drfc;
	Param 			ampf;
	Param 			fb;
} modparam={0};

PrefsData moddesc[]=
{
	{ PD_SOURCE,		"S1",	&(modparam.src)		},
	{ PD_MODULATOR,		"M1",	&(modparam.mod)		},
	{ PD_END,			0l,		0l					}	
};

RTime 		*RunTime=NULL;
APTR 		cfg=NULL;
char		errmsg[256];

//-- Library-Basisaddrs.

extern struct ExecBase 		*SysBase;
struct Library	 			*AslBase=NULL;
struct Library	 			*ConfigIOBase=NULL;
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
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) goto Error;
	if(!(SFXSupportBase	=OpenLibrary("progdir:libs/"SFXSupportName,SFXSupportVersion))) { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open progdir:libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; }
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "progdir:libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; }
	if(!(AslBase		=OpenLibrary("asl.library",37)))								{ sprintf(errmsg,GetString(errOpenLibrary),"asl.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),ConfigIOName" >=V"NUM2STR(ConfigIOVersion));	Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error;?}
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"dos.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L)))							{ sprintf(errmsg,GetString(errOpenLibrary),"gadtools.library >=V37.0");					Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"graphics.library >=V37.0");					Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),UnitConvName" >=V"NUM2STR(UnitConvVersion)); Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
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
	if(ConfigIOBase)	CloseLibrary(ConfigIOBase);
	if(AslBase)			CloseLibrary(AslBase);
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void __saveds __asm LIBClientDataInit(REG(a0) RTime *RunTime_)
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

#define FXCore do {																					\
	in=(double)*sbuf;																				\
/*	in=((double)*sbuf+lin)/2.0;lin=(double)*sbuf; */		/* simpler LowPass */					\
																									\
	out1=in;												/* Vorverz?gerung */					\
	for(k=1;k<VANZ;k++) { out1+=(faccv[k]*bufv[delv[k]]);delv[k]=(delv[k]+1)%mdelv; }				\
	out1+=(faccv[0]*bufv[delv[0]]);																	\
	out1/=(double)VANZ;																				\
	bufv[delv[0]]=(in*0.95+out1*0.05);						/* feedback main multitap-delayline	*/	\
/*	bufv[delv[0]]=in; */																			\
	delv[0]=(delv[0]+1)%mdelv;																		\
	in=out1;												/* Eingang f?r n?chsten Schritt */		\
																									\
	out2=0.0;												/* Kammfilter */						\
	for(k=0;k<KANZ;k++) {																			\
		out=bufk[k][delk[k]];									/* Kammfilterdelayausgang */		\
		dout=fbbk[k]+out;										/* Ausgang Tiefpass dazu */			\
		bufk[k][delk[k]]=(facck*in+facckd*dout);				/* Feedback (GUI-Param) */			\
		fbbk[k]=facbk[k]*dout;																		\
		out2+=out;																					\
		delk[k]=(delk[k]+1)%mdelk[k];																\
	}																								\
	out2/=4.0;												/* really out2/=8.0; */					\
																									\
	lout=out2;												/* Allpass1 */							\
	out2=(out2*(-facda1)+bufa[0][dela[0]]*facdaa1);													\
	out=lout+(bufa[0][dela[0]]*facda1);																\
	bufa[0][dela[0]]=out;																			\
	dela[0]=(dela[0]+1)%mdela[0];																	\
																									\
	lout=out2;												/* Allpass2 */							\
	out2=(out2*(-facda2)+bufa[1][dela[1]]*facdaa2);													\
	out=lout+(bufa[1][dela[1]]*facda2);																\
	bufa[1][dela[1]]=out;																			\
	dela[1]=(dela[1]+1)%mdela[1];																	\
																									\
	out=(out1+out2)*ampf;									/* Early+Main,Diff */					\
	CLIPFLT(out);											/* check here for overflow */			\
	*(dbuf++)=(WORD)((double)*(sbuf++)*dry+out*eff);												\
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
	UBYTE ret=FALSE,runing=TRUE,gh,enabled,nr;
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+RunTime->scy[FRAME_HEIGHT(3)];
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
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
			ng.ng_TopEdge		=ys4+RunTime->scy[2];
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

			g=AddModulator(&modparam.mod,modGadgets,g,GADID_MOD1,GADIX_MOD1,xs1,ys2);

			ng.ng_LeftEdge		=xs1+RunTime->scx[53];
			ng.ng_TopEdge		=ys3+RunTime->scy[2];
			ng.ng_Width			=RunTime->scx[90];
			ng.ng_GadgetText	="Early";
			ng.ng_TextAttr		=RunTime->ScrFont;
			ng.ng_GadgetID		=GADID_ERFC;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.erfc,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_ERFC]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[214];
			ng.ng_GadgetText	="Main";
			ng.ng_GadgetID		=GADID_MRFC;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.mrfc,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_MRFC]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[53];
			ng.ng_TopEdge		=ys3+RunTime->scy[14];
			ng.ng_GadgetText	="Diff";
			ng.ng_GadgetID		=GADID_DRFC;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.drfc,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_DRFC]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[214];
			ng.ng_GadgetText	="Ampf";
			ng.ng_GadgetID		=GADID_AMPF;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.ampf,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_AMPF]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[53];
			ng.ng_TopEdge		=ys3+RunTime->scy[26];
			ng.ng_GadgetText	="Fb.";
			ng.ng_GadgetID		=GADID_FB;
			g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,modparam.fb,TAG_DONE);
			((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
			modGadgets[GADIX_FB]=g;

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

						SetAPen(modrp,7);
						Move(modrp,xs1+RunTime->scx[5],ys2+RunTime->scy[6]);Text(modrp,"Effect",6);

						xh=STDFRM_WIDTH;
						DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
						DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Eff */
						DrawGadWB(modrp,xs1,ys3,xh,RunTime->scy[FRAME_HEIGHT(3)]);	/* Par */
						DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
						DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

						SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
						SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);
						SetModulator(&modparam.mod,modWin,modGadgets,GADIX_MOD1);

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
											if(!gh) gh=HandleModulator(&modparam.mod,&modparam.src,0l,modWin,modGadgets,GADID_MOD1,GADIX_MOD1,gnr,icode);
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
													case GADID_ERFC:
														HandleParameter(modparam.erfc,modWin,modGadgets[GADIX_ERFC]);
														break;
													case GADID_MRFC:
														HandleParameter(modparam.mrfc,modWin,modGadgets[GADIX_MRFC]);
														break;
													case GADID_DRFC:
														HandleParameter(modparam.drfc,modWin,modGadgets[GADIX_DRFC]);
														break;
													case GADID_AMPF:
														HandleParameter(modparam.ampf,modWin,modGadgets[GADIX_AMPF]);
														break;
													case GADID_FB:
														HandleParameter(modparam.fb,modWin,modGadgets[GADIX_FB]);
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

UBYTE __saveds __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn)
{
	ULONG curlen;
	register ULONG j,curlen2;
	register UBYTE l,k;
	PWData *pwd;
	double effs,effe,effd,eff,dry;
	double out,out1,out2,in,lin;
	double erfc,mrfc,drfc,ampf,rtfc;
	char title[80];
	SInfo *dstbuf,*srcbuf;
	ULONG rm_start,rm_len,blen;
	register SAMPLE *sbuf,*dbuf;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	UBYTE ret=1;
	char tstr[PARBUFLEN+1];
	UBYTE resfail=0;
#define VANZ 9
	ULONG delv[VANZ];											/* Vorverz?gerung */
	ULONG mdelv;
	double faccv[VANZ]={0.9803,1.0,0.8017,0.6225,0.7049,0.2618,0.2373,0.1988,0.0851 };
	double ffcv;
#define KANZ 8
	ULONG delk[KANZ],mdelk[KANZ];								/* Kammfilter */
	double facck,facckd;
//	double facck[KANZ],facckd[KANZ];
	double facbk[KANZ];
	double fbbk[KANZ];
	double ffck,dout;
#define AANZ 2
	ULONG dela[AANZ],mdela[AANZ];								/* Allpass */
	double lout;
	double facda1=0.72,facdaa1=1.0-(facda1*facda1);
	double facda2=0.63,facdaa2=1.0-(facda2*facda2);
	double ffca;

	double *bufv,*bufk[KANZ],*bufa[AANZ];

	if(callmd==CM_REXX) {
		UnlockBuffer(modparam.src.srcbuf);
		ret&=((modparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret) return(ret);
	}

	srcbuf=modparam.src.srcbuf;

	moddata1=(void *)(modparam.mod.bshpdata);
	modfunc1=modparam.mod.modfunc;
	UC_ConvertParameter(tstr,modparam.mod.vals,UGRP_PROPOTION,PROP_FC);
	effs=atof(tstr);
	UC_ConvertParameter(tstr,modparam.mod.vale,UGRP_PROPOTION,PROP_FC);
	effe=atof(tstr);
	effd=effe-effs;

	UC_ConvertParameter(tstr,modparam.erfc,UGRP_PROPOTION,PROP_FC);
	erfc=atof(tstr);
	UC_ConvertParameter(tstr,modparam.mrfc,UGRP_PROPOTION,PROP_FC);
	mrfc=atof(tstr);
	UC_ConvertParameter(tstr,modparam.drfc,UGRP_PROPOTION,PROP_FC);
	drfc=atof(tstr);

	UC_ConvertParameter(tstr,modparam.ampf,UGRP_AMPLITUDE,AMP_FC);
	ampf=atof(tstr);

	UC_ConvertParameter(tstr,modparam.fb,UGRP_PROPOTION,PROP_FC);
	facckd=atof(tstr);facck=1.0-facckd;

	SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);

	if(dstbuf=NewSample()) {
		rtfc=(double)(srcbuf->srat)/48000.0;
		ffcv=erfc*rtfc;
		mdelv=(ULONG)(5200.0*ffcv);					// Maxdelay f?r Multitapdelay
		MSG1("max erdel : %ld",mdelv);
		ffck=mrfc*rtfc;
		mdelk[0]=(ULONG)(2400.0*ffck);				// Delayzeiten f?r Kammfilter
		mdelk[1]=(ULONG)(2688.0*ffck);
		mdelk[2]=(ULONG)(2928.0*ffck);
		mdelk[3]=(ULONG)(3264.0*ffck);
		mdelk[4]=(ULONG)(3456.0*ffck);
		mdelk[5]=(ULONG)(3744.0*ffck);
		mdelk[6]=(ULONG)(4013.0*ffck);
		mdelk[7]=(ULONG)(4407.0*ffck);

//		facckd[0]=0.12;facck[0]=1.0-facckd[0];
//		facckd[1]=0.14;facck[1]=1.0-facckd[1];
//		facckd[2]=0.17;facck[2]=1.0-facckd[2];
//		facckd[3]=0.13;facck[3]=1.0-facckd[3];
//		facckd[4]=0.21;facck[4]=1.0-facckd[4];
//		facckd[5]=0.26;facck[5]=1.0-facckd[5];
//		facckd[6]=0.37;facck[6]=1.0-facckd[6];
//		facckd[7]=0.31;facck[7]=1.0-facckd[7];
//
//		for(k=0;k<KANZ;k++) {
//			facckd[k]=mypow_nn(10.0,(-900.0/mdelk[k]));
//			facck[k]=1.0-facckd[k];
//		}
//		facbk[0]=0.4512*rtfc;					// Koeffizienten f?r Tiefp?sse im Kammfilter 
//		facbk[1]=0.4712*rtfc;
//		facbk[2]=0.4912*rtfc;
//		facbk[3]=0.5108*rtfc;
//		facbk[4]=0.5208*rtfc;
//		facbk[5]=0.5408*rtfc;
//		facbk[6]=0.5621*rtfc;
//		facbk[7]=0.5924*rtfc;

		facbk[0]=0.4512;						// Koeffizienten f?r Tiefp?sse im Kammfilter 
		facbk[1]=0.4712;						// the are not sampling-rate dependent
		facbk[2]=0.4912;
		facbk[3]=0.5108;
		facbk[4]=0.5208;
		facbk[5]=0.5408;
		facbk[6]=0.5621;
		facbk[7]=0.5924;

		ffca=drfc*rtfc;
		mdela[0]=(ULONG)(6.0*ffca);				// Delays f?r Allpasse
		mdela[1]=(ULONG)(11.3*ffca);

		if(!(AllocSampleMem(dstbuf,srcbuf->slen,(srcbuf->channels+1)))) resfail=1;
		if(!(bufv=(double *)MyAllocMem((mdelv*sizeof(double)),MEMF_ANY))) resfail=1;
		for(k=0;k<KANZ;k++) { if(!(bufk[k]=(double *)MyAllocMem((mdelk[k]*sizeof(double)),MEMF_ANY))) resfail=1; }
		for(k=0;k<AANZ;k++) { if(!(bufa[k]=(double *)MyAllocMem((mdela[k]*sizeof(double)),MEMF_ANY))) resfail=1; }
		if(!resfail) {
			BufferFill(srcbuf,dstbuf);
			if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster ?ffnen
				for(l=0;l<(srcbuf->channels+1);l++) {
					AssignBShape(&modparam.mod);
					if(srcbuf->channelmask&(1<<l)) {
						memset(bufv,0,mdelv*sizeof(double));					/* Clear Delays */
						for(k=0;k<KANZ;k++) memset(bufk[k],0,mdelk[k]*sizeof(double));
						for(k=0;k<AANZ;k++) memset(bufa[k],0,mdela[k]*sizeof(double));
						delv[0]=0;
						delv[1]=(ULONG)( 955.0*ffcv);		// Delayzeiten f?r Earlyreflections
						delv[2]=(ULONG)(1699.0*ffcv);
						delv[3]=(ULONG)(1867.0*ffcv);
						delv[4]=(ULONG)(1987.0*ffcv);
						delv[5]=(ULONG)(3355.0*ffcv);
						delv[6]=(ULONG)(3821.0*ffcv);
						delv[7]=(ULONG)(4492.0*ffcv);
						delv[8]=(ULONG)(5107.0*ffcv);
// DEBUG
						for(k=0;k<KANZ;k++) {
							MSG2("mr-delay[%2d]=%ld ",k,mdelk[k]);
						}
						for(k=0;k<VANZ;k++) {
							MSG3("er-delay[%2d]=%ld %6.4lf",k,delv[k],faccv[k]);
						}
// DEBUG
						for(k=0;k<KANZ;k++) { delk[k]=0;fbbk[k]=0.0; }
						dela[0]=dela[1]=0;
						lin=0;

						if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);
						srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e lesen
						dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Bl?cke konstanter Gr??e schreiben
						if(srcsa && dstsa) {
							SARead =srcsa->safunc;
							SAWrite=dstsa->safunc;
							blen=Bytes2Samples(srcsa->blen);
							sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);
							StartProWin(pwd,&curlen,title,rm_len);					// den Statusbalken starten
							curlen2=0;
							for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++,dstsa->seg++) {
								sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & n?chstes Read starten
								dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
								j=0;
								while((j<blen) && (curlen<rm_len)) {		// die Blockl?nge durchgehen
									eff=effs+effd*modfunc1(curlen2,rm_len,moddata1);dry=1.0-eff;
									curlen2=(curlen+=4);
									FXCore;FXCore;FXCore;FXCore;j+=4;
								}
								SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
							}
							if(ProWinAborted(curlen)) l=4;					// es wurde abgebrochen
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
		if(bufv) MyFreeMem((ULONG *)bufv,(mdelv*sizeof(double)));
		for(k=0;k<KANZ;k++) { if(bufk[k]) MyFreeMem((ULONG *)bufk[k],(mdelk[k]*sizeof(double))); }
		for(k=0;k<AANZ;k++) { if(bufa[k]) MyFreeMem((ULONG *)bufa[k],(mdela[k]*sizeof(double))); }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);ret=1; }
	return(ret);
}

void __saveds __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret)
{
	UBYTE cf=0,pf;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam"))
	{
		cf=1;pf=ParseModulatorSet  (&modparam.mod,"P1",params);
		if(!pf && !stricmp(RexxPar2,"Erfc"))			{ strncpy(modparam.erfc,RexxPar3,PARBUFLEN);modparam.erfc[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Mrfc"))			{ strncpy(modparam.mrfc,RexxPar3,PARBUFLEN);modparam.mrfc[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Drfc"))			{ strncpy(modparam.drfc,RexxPar3,PARBUFLEN);modparam.drfc[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strncpy(modparam.ampf,RexxPar3,PARBUFLEN);modparam.ampf[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Fb"))				{ strncpy(modparam.fb  ,RexxPar3,PARBUFLEN);modparam.fb  [PARBUFLEN]=0;pf=1; }
		if(pf) SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam"))
	{
		cf=1;pf=ParseModulatorGet  (&modparam.mod,"P1",params,ret);
		if(!pf && !stricmp(RexxPar2,"Erfc"))			{ strncpy(ret,modparam.erfc,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Mrfc"))			{ strncpy(ret,modparam.mrfc,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Drfc"))			{ strncpy(ret,modparam.drfc,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Ampf"))			{ strncpy(ret,modparam.ampf,PARBUFLEN);ret[PARBUFLEN]=0;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Fb"))				{ strncpy(ret,modparam.fb  ,PARBUFLEN);ret[PARBUFLEN]=0;/*pf=1;*/ }
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
	strcpy(modparam.erfc,CfgIO_ReadString	(cfg,"General"	,"Erfc"	,"100 %"));
	strcpy(modparam.mrfc,CfgIO_ReadString	(cfg,"General"	,"Mrfc"	,"100 %"));
	strcpy(modparam.drfc,CfgIO_ReadString	(cfg,"General"	,"Drfc"	,"100 %"));
	strcpy(modparam.ampf,CfgIO_ReadString	(cfg,"General"	,"Ampf"	,"1.35"));
	strcpy(modparam.fb  ,CfgIO_ReadString	(cfg,"General"	,"Fb"	,"15 %"));
}

void SaveFXParams(char *fname)
{
	SaveModSettings(cfg,moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Erfc"		,modparam.erfc);
	CfgIO_WriteString		  (cfg,"General"	,"Mrfc"		,modparam.mrfc);
	CfgIO_WriteString		  (cfg,"General"	,"Drfc"		,modparam.drfc);
	CfgIO_WriteString		  (cfg,"General"	,"Ampf"		,modparam.ampf);
	CfgIO_WriteString		  (cfg,"General"	,"Fb"		,modparam.fb);

	CfgIO_WriteConfig(cfg,fname);
}

void DefaultFXParams(void)
{
	modparam.src.srcbuf=0L;
	modparam.src.rmode=RNGMD_ALL;

	UC_ConvertParameter(modparam.mod.vals,"1.0",UGRP_PROPOTION,PROP_FC);
	UC_ConvertParameter(modparam.mod.vale,"1.0",UGRP_PROPOTION,PROP_FC);
	modparam.mod.bshape=BSHP_NONE;
	modparam.mod.desc[0]='\0';

	UC_ConvertParameter(modparam.erfc,"100 %" ,UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(modparam.mrfc,"100 %" ,UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(modparam.drfc,"100 %" ,UGRP_PROPOTION,PROP_PROZ);
	UC_ConvertParameter(modparam.ampf,"1.35"  ,UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(modparam.fb  ,"15 %"  ,UGRP_PROPOTION,PROP_PROZ);
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
	SetModulator(&modparam.mod,modWin,modGadgets,GADIX_MOD1);
	GT_SetGadgetAttrs(modGadgets[GADIX_ERFC],modWin,0l,GTST_String,modparam.erfc,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MRFC],modWin,0l,GTST_String,modparam.mrfc,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_DRFC],modWin,0l,GTST_String,modparam.drfc,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_AMPF],modWin,0l,GTST_String,modparam.ampf,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_FB  ],modWin,0l,GTST_String,modparam.fb  ,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
