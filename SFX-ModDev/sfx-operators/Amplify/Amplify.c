/****h* Amplify/Amplify.c [4.1] *
*
*  NAME
*    Amplify.c
*  COPYRIGHT
*    $VER: Amplify.c 4.1 (07.10.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Amplify Operator module for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    07.Oct.2000	V 4.1	most recent version
*    02.Aug.2000	V 4.1	most recent version
*    01.Aug.2000	V 1.20	most recent version
*                           * fixed maxvol-bug
*    19.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

/****u* _operators/Amplify *
*  Function.eng
*    Changes the volume of a sample
*  Funktion.ger
*    Ändert die Lautstärke eines Samples
*  Parameter.eng
*    Amplification (<a href="../node03.02.01.html">P1</a>)
*      This value controls the amount of amplification.
*    MaxVol
*      After a clicking this button, the current source will be scanned and the maximum
*      amplification without clipping will be calculated.
*      the result will be entered into Par0 and modulation will be set to "None".
*    Wrap
*      Choose how to handle clipping. The modi below are available :
*        NoClip: don't care
*        Clip : obverdriven values are clipped
*        Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.
*        Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.
*      Just give it a try. Take a long sinewave  and slowly overdrive it.
*  Parameter.ger
*    Amplification (<a href="../node03.02.01.html">P1</a>)
*      Dieser Wert gibt die Lautstärkeänderung an.
*    MaxVol
*      Durch einen Klick auf diesen Knopf, wird das Sample gescannt und die maximale
*      Verstärkung errechnet die möglich ist, ohne das Signal zu übersteuern.
*      Das Ergebnis wird in Par0 eingetragen und die Modulation wird auf "None" gesetzt.
*    Wrap
*      Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
*      Hierbei gibt es 4 Modi : 
*        NoClip: es wird nicht auf Übersteuerte Werte gestestet.
*        Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt
*        Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.
*        Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.
*      Diese Modi sollte man ruhig mal ausprobieren. Dazu nimmt man einen lange Sinus
*      und übersteuert diesen langsam. 
*  Notes.eng
*    Percussion sounds (bassdrums,snare,drums,...) can be lifted a bit
*    (ca. 120 %). This'll produce the typical <a href="../node06.03.00.Overdrive.html">overdrive</a> effect,
*    by clipping sample data (vertically).<br>
*    The amount of amplification without hitting the ceiling hard, can be estimated
*    taking a look at the min-and maxlines in the <a href="../node01.05.01.html">samplewindow</a>.<br>
*    This operator can also be used for amplitudemodulation, creating
*    further possiblities for sound synthesis.
*    For example, make one sine wave with normal period and another
*    with double. Let the one sine be the source and
*    the other sine be the modulation waveform with User/
*    Normal set. Set Par0 to 0 and Par1 to 100. Generate this and
*    take a close look (with zoom maybe?:) at the result.
*  Hinweise.ger
*    Schlagzeuginstrumente ( besonders Basedrums ) können ruhig mal
*    etwas übersteuert werden (ca. 120 %). Eine solche leichte
*    <a href="../node06.03.00.Overdrive.html">Übersteuerung</a> ergibt den typischen Overdriveeffekt, durch
*    die gekappten Samplewerte.<br>
*    Die Verstärkung die ohne Übersteuerung möglich ist, kann an den
*    Min- und Maxlinien im <a href="../node01.05.01.html">Samplefenster</a> abgeschätzt werden.<br>
*    Dieser Operator läßt sich außerdem noch zur Amplitudenmodulation
*    verwenden, wodurch sich weitere Synthesemöglichkeiten ergeben.
*    Erzeugen Sie dazu z.B. einen Sinus mit normaler Periode
*    und einen weiteren mit doppelter. Jetzt wählen Sie ein Sample
*    als Source und stellen als Modulation User/Normal ein. Par0
*    setzen Sie auf 0 und Par1 auf 100. Als Modulator nehmen Sie den
*    anderen Sinus. Lassen Sie das neue Sample erzeugen und schauen
*    Sie es sich an (eventuell vergrößern).
*******
*/

#define AMPLIFY_C

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

#define GADID_MOD		(GADID_SRC+SRC_IDCT)
#define GADIX_MOD		(GADIX_SRC+SRC_IXCT)

#define GADID_MAXVOL	(GADID_MOD+MOD_IDCT)
#define GADIX_MAXVOL	(GADIX_MOD+MOD_IXCT)

#define GADID_WRAPMODE	(GADID_MAXVOL+1)
#define GADIX_WRAPMODE	(GADIX_MAXVOL+1)

#define LAST_GADIX		(GADIX_WRAPMODE+1)

//-- globals ------------------------------------------------------------------

//-- tag for version command

UBYTE *VersTag="\0$VER: "PRJ_VERS;

//-- these are the parameters the operator uses

struct
{
	Source			src;		/* Quelle */
	Modulator		mod;		/* Effektanteil */
	UBYTE			wrap;
	UBYTE			maxvol;
} modparam={0};

#define WRAP_NOCLIP	0
#define WRAP_CLIP	1
#define WRAP_WRAP1	2
#define WRAP_WRAP2	3
#define WRAP_ANZ	4

//-- this allows us to automatically process some parts of the settings

PrefsData moddesc[]=
{
	{ PD_SOURCE,		"S1",		&(modparam.src)	},
	{ PD_MODULATOR,		"M1",		&(modparam.mod)	},
	{ PD_END,			0l,			0l				}	
};

//-- general global data

RTime 		*RunTime=NULL;		// the shared memory structure
APTR 		cfg=NULL;			// a handle for a preset
char		errmsg[256];		// for error messages

UBYTE *WrapLabels[]={ "None","Clip","Wrap1","Wrap2",0l };

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

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFX_FXBase))
{
	INITDEBUG;
	SysBase=(struct ExecBase *)*((ULONG *)4L);
	if(!(IntuitionBase	=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) goto Error;
	if(!(SFXSupportBase	=OpenLibrary("progdir:libs/"SFXSupportName,SFXSupportVersion))) { DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "Can't open progdir:libs/" SFXSupportName " >=V" NUM2STR(SFXSupportVersion) "\x00\x00",44);goto Error; }
	if(GetSFXVersion()!=MAKE_VERSION_STAMP(PRJ_VERSION,PRJ_REVISION))					{ DisplayAlert(RECOVERY_ALERT,"\x00\xF0\x14" "progdir:libs/"SFXSupportName" has not the requested version V"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"!" "\x00\x00",44);goto Error; }
	if(!(AslBase		=OpenLibrary("asl.library",37)))								{ sprintf(errmsg,GetString(errOpenLibrary),"asl.library >=V37.0");							Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),ConfigIOName" >=V"NUM2STR(ConfigIOVersion));		Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"dos.library >=V37.0");							Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GadToolsBase	=OpenLibrary("gadtools.library",37L)))							{ sprintf(errmsg,GetString(errOpenLibrary),"gadtools.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(GfxBase		=(struct GfxBase *)OpenLibrary("graphics.library",37)))			{ sprintf(errmsg,GetString(errOpenLibrary),"graphics.library >=V37.0");						Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
	if(!(UnitConvBase	=OpenLibrary(UnitConvName,UnitConvVersion)))					{ sprintf(errmsg,GetString(errOpenLibrary),UnitConvName" >=V"NUM2STR(UnitConvVersion));		Message(errOpenLibrary,errmsg,__FILE__,__LINE__);goto Error; }
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

#define FXCore_NoClip do {																			\
	*(dbuf++)=(SAMPLE)(((double)(*(sbuf++)))*amp);													\
} while(0)

#define FXCore_Clip do {																			\
	curval=(LONG)(((double)(*(sbuf++)))*amp);														\
	if		(curval>SMP_MAXP) curval=SMP_MAXP;														\
	else if	(curval<SMP_MAXN) curval=SMP_MAXN;														\
	*(dbuf++)=(SAMPLE)curval;																		\
} while(0)

#define FXCore_Wrap1 do {																			\
	curval=(LONG)(((double)(*(sbuf++)))*amp);														\
	if		(curval>SMP_MAXP) curval=((curval-SMP_MAXN)&SMP_MAXMASK)+SMP_MAXN;						\
	else if	(curval<SMP_MAXN) curval=((curval-SMP_MAXP)&SMP_MAXMASK)+SMP_MAXP;						\
	*(dbuf++)=(SAMPLE)curval;																		\
} while(0)

#define FXCore_Wrap2 do {																			\
	curval=(LONG)(((double)(*(sbuf++)))*amp);														\
	while(curval>SMP_MAXP || curval<SMP_MAXN) {														\
		if		(curval>SMP_MAXP) curval=SMP_MAXP-(curval-SMP_MAXP);								\
		else if	(curval<SMP_MAXN) curval=SMP_MAXN-(curval-SMP_MAXN);								\
	}																								\
	*(dbuf++)=(SAMPLE)curval;																		\
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
	UBYTE ret=FALSE,runing=TRUE,gh,srctyp,enabled,nr;
	LONG maxvalp,maxvaln;
	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+MODSEL_HEIGHT;
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
	UWORD xh;
	register ULONG j,curlen;
	register UBYTE l;
	SInfo *srcbuf;
	SAMPLE *sbuf;
	SmpAccess *srcsa;
	SAFuncPtr SARead;
	ULONG rm_start,rm_len;
	Param param;
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

			g=AddModulator(&modparam.mod,modGadgets,g,GADID_MOD,GADIX_MOD,xs1,ys2);

			ng.ng_LeftEdge		=xs1+RunTime->scx[37];
			ng.ng_TopEdge		=ys3+RunTime->scy[FRMCONTENT_YOFFSET];
			ng.ng_Width			=RunTime->scx[80];
			ng.ng_GadgetText	="MaxVol";
			ng.ng_TextAttr		=RunTime->ScrFont;
			ng.ng_GadgetID		=GADID_MAXVOL;
			ng.ng_Flags			=PLACETEXT_IN;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			modGadgets[GADIX_MAXVOL]=g;

			ng.ng_LeftEdge		=xs1+RunTime->scx[214];
			ng.ng_Width			=RunTime->scx[90];
			ng.ng_GadgetText	="Wrap";
			ng.ng_GadgetID		=GADID_WRAPMODE;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,WrapLabels,GTCY_Active,modparam.wrap,TAG_DONE);
			modGadgets[GADIX_WRAPMODE]=g;

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
						RText(modrp,"Mod",xs1+RunTime->scx[29],ys2+RunTime->scy[FRMTEXT_YPOS(2)]);

						DrawTitle(modrp,"Source"		,xs1,ys1,STDFRM_WIDTH);
						DrawTitle(modrp,"Amplification"	,xs1,ys2,STDFRM_WIDTH);
						DrawTitle(modrp,"Miscellaneous"	,xs1,ys3,STDFRM_WIDTH);
						DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

						xh=STDFRM_WIDTH;
						DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);					/* Source */
						DrawGadWB(modrp,xs1,ys2,xh,MODSEL_HEIGHT);					/* Par */
						DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(1));				/* Par */
						DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);					/* Go */
						DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

						SetCfgSel   (&cfgsel      ,modWin,modGadgets,GADIX_CFGSEL);
						SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);
						SetModulator(&modparam.mod,modWin,modGadgets,GADIX_MOD);

						if(!enabled) {
							GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
							GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
						}

						sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

						while(runing) {
							sigout=Wait(sigin);
							if(sigout&(1L<<nt.sig)) {
								if(!enabled && RunTime->aktsamples) {
									enabled=1;
									GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
									modparam.src.srcbuf=LockBuffer(RunTime->aktbuf);
									modparam.src.rmode=SetRngMode(modparam.src.srcbuf);
								}
								else if(enabled && !RunTime->aktsamples) {
									enabled=0;
									GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_MAXVOL],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
								}
								SetSource   (&modparam.src,modWin,modGadgets,GADIX_SRC);
							}
							if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
								while(imsg=GT_GetIMsg(modWin->UserPort)) {
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
											if(!gh) gh=HandleModulator(&modparam.mod,&modparam.src,0l,modWin,modGadgets,GADID_MOD,GADIX_MOD,gnr,icode);
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
													case GADID_MAXVOL:
														maxvalp=maxvaln=0;
														if(modparam.src.srcbuf) {
															srcbuf=modparam.src.srcbuf;
															SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);
															for(l=0;l<(srcbuf->channels+1);l++) {
																if(srcbuf->channelmask&(1<<l)) {
																	if(srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))	{ // sequentiell Blöcke konstanter Größe lesen
																		SARead =srcsa->safunc;curlen=0;
																		for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++) {
																			sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
																			for(j=0;(j<Bytes2Samples(srcsa->blen) && curlen<rm_len);j++,curlen++) {
																				if(*sbuf>maxvalp) maxvalp=*sbuf;
																				else if(*sbuf<maxvaln) maxvaln=*sbuf;
																				sbuf++;
																			}
																		}
																		SADone(srcsa,srcbuf);
																	}
																}
															}
															if(maxvalp || maxvaln) {
																maxvaln=-maxvaln;
																if(maxvaln==32768 || maxvalp==32767) strcpy(param,"1.0");
																else sprintf(param,"%6.4lf",32767.0/(double)(max(maxvalp,maxvaln)));
																srctyp=UC_GetParameterUnit(modparam.mod.vals,UGRP_AMPLITUDE);
																UC_ConvertParameter(modparam.mod.vals,param,UGRP_AMPLITUDE,srctyp);
																UC_ConvertParameter(modparam.mod.vale,param,UGRP_AMPLITUDE,srctyp);
																modparam.mod.bshape=BSHP_NONE;strcpy(modparam.mod.desc,"None");
																SetModulator(&modparam.mod,modWin,modGadgets,GADIX_MOD);
															}
														}
														break;
													case GADID_WRAPMODE:
														modparam.wrap=icode;
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
													ActivateGadget(modGadgets[GADIX_MOD],modWin,0l);
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
//											if(icode==RKEY_HELP) Help("/_operators/"PRJ_NAME".guide/main",RunTime->fxoff,RunTime->anzfx);
//											if(icode==RKEY_HELP) Help("_operators/"PRJ_NAME".html",0,0);
											if(icode==RKEY_HELP) Help("html/nodes/"HELPBASENAME_OP"."PRJ_NAME".html",0,0);
											break;
										case IDCMP_MENUPICK:
											while(icode!=MENUNULL) {
												if(item=ItemAddress(RunTime->FXMenu,icode)) {
													switch((ULONG)MENU_USERDATA(item)) {
														case MENID_LOAD:
															MODFReqtags[9].ti_Data=FALSE;
															if(AslRequest(modFRequest,MODFReqtags)) {
																strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
																DoneFXParams(moddesc);FreeFXParams(moddesc);				// free old settings
																LoadFXParams(fn);InitFXParams(moddesc);						// init new settings
																RefreshGUI(modGadgets,modWin);
															}
															break;
														case MENID_SAVE:
															MODFReqtags[9].ti_Data=TRUE;
															if(AslRequest(modFRequest,MODFReqtags)) {
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
	LONG maxvalp,maxvaln,curval;
	double amps,ampe,ampd,amp;
	char title[80];
	ULONG rm_start,rm_len,blen;
	SInfo *dstbuf,*srcbuf;
	register SAMPLE *sbuf,*dbuf;
	SmpAccess *srcsa,*dstsa;
	SAFuncPtr SARead,SAWrite;
	BShapeFuncPtr modfunc;
	void *moddata;
	UBYTE ret=1,srctyp;
	Param param;
//	UBYTE resfail=0;

	if(callmd==CM_REXX) {				// wir wurden per ARexx gestartet
		UnlockBuffer(modparam.src.srcbuf);
		ret&=((modparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret) return(ret);
	}

	srcbuf=modparam.src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,modparam.src.rmode);		// gibt den zu bearbeitenden Bereich zurück

	if(modparam.maxvol) {				// maxvol suchen (wenn von rexx aus gestartet)
		maxvalp=maxvaln=0;
		if((pwd=NewProWin())) {					// ein neues asynchrones Statusfenster öffnen
			for(l=0;l<(srcbuf->channels+1);l++) {
				if(srcbuf->channelmask&(1<<l)) {
					if(srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN)) {	// sequentiell Blöcke konstanter Größe lesen
						SARead =srcsa->safunc;
						sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);
						StartProWin(pwd,&curlen,title,rm_len);
						for(srcsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++) {
							sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
							for(j=0;(j<Bytes2Samples(srcsa->blen) && curlen<rm_len);j++,curlen++) {
								if(*sbuf>maxvalp) maxvalp=*sbuf;
								else if(*sbuf<maxvaln) maxvaln=*sbuf;
								sbuf++;
							}
						}
						if(ProWinAborted(curlen)) l=5;					// es wurde abgebrochen
						SADone(srcsa,srcbuf);
					}
				}
			}
			DelProWin(pwd);
			if(l==5) return(1TRUE;
		}
		if(maxvalp || maxvaln) {
			maxvaln=-maxvaln;
			if(maxvaln==32768 || maxvalp==32767) strcpy(param,"1.0");
			else sprintf(param,"%6.4lf",32767.0/(double)(max(maxvalp,maxvaln)));
			srctyp=UC_GetParameterUnit(modparam.mod.vals,UGRP_AMPLITUDE);
			UC_ConvertParameter(modparam.mod.vals,param,UGRP_AMPLITUDE,srctyp);
			UC_ConvertParameter(modparam.mod.vale,param,UGRP_AMPLITUDE,srctyp);
			modparam.mod.bshape=BSHP_NONE;
		}
	}

	moddata=(void *)(modparam.mod.bshpdata);							// einige pointer cachen und die Parameter aus den Parameterstring in
	modfunc=modparam.mod.modfunc;										// numerische Basiseinheiten umwandeln
	UC_ConvertParameter(param,modparam.mod.vals,UGRP_AMPLITUDE,AMP_FC);	// e.g. Lautstärke von was-auch-immer (db/%/%%/Faktor/...) nach Faktor
	amps=atof(param);
	UC_ConvertParameter(param,modparam.mod.vale,UGRP_AMPLITUDE,AMP_FC);
	ampe=atof(param);
	ampd=ampe-amps;													// Modulationsdiffernz

	if(dstbuf=NewSample()) {										// wie benötigen einen neuen Samplepuffer für das Resultat
		if(AllocSampleMem(dstbuf,srcbuf->slen,(srcbuf->channels+1))) {	// dann noch etwas wo das Ergebnis hingespeichert werden kann
																		// !! dies kann auch Festplattenplatz sein
			BufferFill(srcbuf,dstbuf);									// kopieren die Buffereinstellungen des Quellsamples
			if((pwd=NewProWin())) {										// ein neues asynchrones Statusfenster öffnen
				for(l=0;l<(srcbuf->channels+1);l++)	{						// bearbeite jeden Kanal ...
					AssignBShape(&modparam.mod);
					if(srcbuf->channelmask&(1<<l)) {							// ... der aktive ist
						if(rm_start>0) CopyDataCh(srcbuf,dstbuf,l,0,0,rm_start);	// Bereich von 0...Bereichsstart kopieren
																					// Die Samplezugriffshandles besorgen
						srcsa=SAInit(srcbuf,l,rm_start,rm_len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe lesen
						dstsa=SAInit(dstbuf,l,rm_start,rm_len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
						if(srcsa && dstsa) {										// hat's geklappt ?
							SARead =srcsa->safunc;									// die Zugriffsfunktionspointer cachen
							SAWrite=dstsa->safunc;
							blen=Bytes2Samples(srcsa->blen);
							sprintf(title,GetString(msgProgress),PRJ_NAME,l+1);		// Titel für Statusfenster
							StartProWin(pwd,&curlen,title,rm_len);					// den Statusbalken starten
							curlen2=0;
							switch(modparam.wrap) {									// je nach wrap-mode
								case WRAP_NOCLIP:										// und jetzt berechnen
									for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++,dstsa->seg++) { // für jedes Segment
										sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);	// Pointer auf src-Block holen & nächstes Read starten
										dbuf=(SAMPLE *)dstsa->buffer1;								// Pointer auf dst-Block
										j=0;
										while((j<blen) && (curlen<rm_len)) {		// die Blocklänge durchgehen
											amp=amps+ampd*modfunc(curlen2,rm_len,moddata);						// Modulator aufrufen, liefert immer Wert zwischen  0.0 und 1.0 zurück
											curlen2=(curlen+=4);
											FXCore_NoClip;FXCore_NoClip;FXCore_NoClip;FXCore_NoClip;j+=4;
										}
										SAWrite(dstsa,dstsa->blen);									// akt Block schreiben
									}
									break;
								case WRAP_CLIP:
									for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++,dstsa->seg++) {
										sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
										dbuf=(SAMPLE *)dstsa->buffer1;
										j=0;
										while((j<blen) && (curlen<rm_len)) {		// die Blocklänge durchgehen
											amp=amps+ampd*modfunc(curlen2,rm_len,moddata);						// Modulator aufrufen, liefert immer Wert zwischen  0.0 und 1.0 zurück
											curlen2=(curlen+=4);
											FXCore_Clip;FXCore_Clip;FXCore_Clip;FXCore_Clip;j+=4;
										}
										SAWrite(dstsa,dstsa->blen);
									}
									break;
								case WRAP_WRAP1:
									for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++,dstsa->seg++) {
										sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
										dbuf=(SAMPLE *)dstsa->buffer1;
										j=0;
										while((j<blen) && (curlen<rm_len)) {		// die Blocklänge durchgehen
											amp=amps+ampd*modfunc(curlen2,rm_len,moddata);						// Modulator aufrufen, liefert immer Wert zwischen  0.0 und 1.0 zurück
											curlen2=(curlen+=4);
											FXCore_Wrap1;FXCore_Wrap1;FXCore_Wrap1;FXCore_Wrap1;j+=4;
										}
										SAWrite(dstsa,dstsa->blen);
									}
									break;
								case WRAP_WRAP2:
									for(srcsa->seg=dstsa->seg=0;((srcsa->seg<srcsa->seganz) && (curlen<rm_len));srcsa->seg++,dstsa->seg++) {
										sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);
										dbuf=(SAMPLE *)dstsa->buffer1;
										j=0;
										while((j<blen) && (curlen<rm_len)) {		// die Blocklänge durchgehen
											amp=amps+ampd*modfunc(curlen2,rm_len,moddata);						// Modulator aufrufen, liefert immer Wert zwischen  0.0 und 1.0 zurück
											curlen2=(curlen+=4);
											FXCore_Wrap2;FXCore_Wrap2;FXCore_Wrap2;FXCore_Wrap2;j+=4;
										}
										SAWrite(dstsa,dstsa->blen);
									}
									break;
							}
							if(ProWinAborted(curlen)) l=4;					// es wurde abgebrochen
						}
						if(srcsa) SADone(srcsa,srcbuf);					// Samplezugiffshandles freigeben
						if(dstsa) SADone(dstsa,dstbuf);
																		// Bereich von Bereichsende..Samplenende kopieren
						if(rm_start+rm_len<srcbuf->slen) CopyDataCh(srcbuf,dstbuf,l,(rm_start+rm_len),(rm_start+rm_len),(srcbuf->slen-(rm_start+rm_len)));
					}
					else CopyDataCh(srcbuf,dstbuf,l,0,0,srcbuf->slen);	// den kompletten kanal unverändert übernehmen
				}
				AddSample(dstbuf,((struct Node *)srcbuf)->ln_Name);		// das Sample öffentlich zugänglich machen (Fenster öffnen, in Liste aufnehmen)
				DelProWin(pwd);											// das Statusbalkenfenster wieder freigeben
			}
		}
		else { RemSample(dstbuf);Message(errAllocateMemory,NULL,__FILE__,__LINE__);ret=1; }
	}
	else { Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);ret=1; }
	return(ret);													// Errorflag zurückgeben
}

void __saveds __asm LIBHandleARexxCmd(REG(a0,char *params),REG(a1,char *ret))
{
	UBYTE cf=0,pf;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetOperatorParam"))
	{
		cf=1;pf=ParseModulatorSet(&modparam.mod,"P1",params);
		if(!pf && !stricmp(RexxPar2,"MaxVol"))	{ modparam.maxvol=atoi(RexxPar3)&1;pf=1; }
		if(!pf && !stricmp(RexxPar2,"Wrap"))	{ modparam.wrap  =CfgIO_MatchString(RexxPar3,WrapLabels);pf=1; }
		if(pf) SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetOperatorParam"))
	{
		cf=1;pf=ParseModulatorGet(&modparam.mod,"P1",params,ret);
		if(!pf && !stricmp(RexxPar2,"MaxVol"))	{ sprintf(ret,"%d",modparam.maxvol);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Wrap"))	{ sprintf(ret,"%s",WrapLabels[modparam.wrap]);/*pf=1;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetOperatorPreset"))
	{
		if(!stricmp(RexxPar2,"default"))  { DoneFXParams(moddesc);FreeFXParams(moddesc);RecallDefFXParams();InitFXParams(moddesc); }
		else { sprintf(fn,"progdir:_operators/"PRJ_NAME"/%s.cfg",RexxPar2);DoneFXParams(moddesc);FreeFXParams(moddesc);LoadFXParams(fn);InitFXParams(moddesc); }
		SaveFXParams("env:sfx/operators/"PRJ_NAME".cfg");
	}
}

//-- local Stuff

void LoadFXParams(char *fname)
{
	CfgIO_ReadConfig(cfg,fname);
	
	LoadModSettings(cfg,moddesc);
	modparam.wrap	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Wrap"		,WrapLabels[WRAP_NOCLIP]),WrapLabels);
	modparam.maxvol	=CfgIO_ReadBool				(cfg,"General"	,"MaxVol"	,TRUE);
}

void SaveFXParams(char *fname)
{
	SaveModSettings(cfg,moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Wrap"		,WrapLabels[modparam.wrap]);
	CfgIO_WriteBool			  (cfg,"General"	,"MaxVol"	,modparam.maxvol);

	CfgIO_WriteConfig(cfg,fname);
}

void DefaultFXParams(void)
{
	modparam.src.srcbuf=0L;
	modparam.src.rmode=RNGMD_ALL;

	UC_ConvertParameter(modparam.mod.vals,"1.0",UGRP_AMPLITUDE,AMP_FC);
	UC_ConvertParameter(modparam.mod.vale,"1.0",UGRP_AMPLITUDE,AMP_FC);
	modparam.mod.bshape=BSHP_NONE;
	modparam.mod.desc[0]='\0';

	modparam.wrap=WRAP_NOCLIP;
	modparam.maxvol=0;
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
	SetModulator(&modparam.mod,modWin,modGadgets,GADIX_MOD);
	GT_SetGadgetAttrs(modGadgets[GADIX_WRAPMODE],modWin,0l,GTCY_Active,modparam.wrap,TAG_DONE);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
