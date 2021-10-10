/******************************************************************************
** Room.c                                                                    **
**---------------------------------------------------------------------------**
** SFX FX module : Room                                                      **
**---------------------------------------------------------------------------**
** Version : V 0.1                                                           **
** Date    : 01.02.1997                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

/*-- includes & defines -------------------------------------------------------------------------*/

#include "libraries/sfx_mod.h"

#define VERSION 1
#define REVISION 00

#define FXVERS "Room "NUM2STR(VERSION)"."NUM2STR(REVISION)" for SoundFX © by Stefan Kost 1993-1998"

#include <debug/debug_defs.h>
#include <debug/debug_decl.h>

/* Gadget ID's */

#define GADID_GO		0
#define GADIX_GO		0
#define GADID_RESET		1

#define GADID_SRC		(GADID_RESET+1)
#define GADIX_SRC		(GADIX_GO+1)

/* Menu ID's */

#define MENID_LOAD		 0
#define MENID_SAVE		 1
#define MENID_RESET		 2
#define MENID_GO		 3
#define MENID_HELP		 4
#define MENID_ABOUT		 5
#define MENID_QUIT		 6

/*-- protos -------------------------------------------------------------------------------------*/

/* Initialisation and Destruction */

int __asm __UserLibInit(REG(a6) struct Library *SFX_FXBase);
void __asm __UserLibCleanup(REG(a6) struct Library *SFX_FXBase);

/* DataInitialisation */

void __asm LIBClientDataInit(REG(a0) RTime *RunTime_);

/* Modulroutienen */

UBYTE __asm LIBInterface(void);
UBYTE __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn);
void  __asm LIBSetDefaults(void);
void __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret);

/* local stuff */

__inline double angle(double ma,double mw,double aw);

void LoadSettings(char *fname);
void SaveSettings(char *fname);
void RecallSettings(void);
void InitOperator(void);
void DoneOperator(void);

/* private Stuff */

/*-- global variables ---------------------------------------------------------------------------*/

struct
{
	Source			src;		/* Quelle */
} fxparam;

PrefsData fxdesc[]=
{
	{ PD_SOURCE,		PDS_SOURCE,		&(fxparam.src)	},
	{ 0,				0l,				0l				}	
};

RTime 		*RunTime;

/* Library-Basisaddrs. ------------------------------------------------------------------------------------*/

extern struct ExecBase 		*SysBase;
extern struct DosLibrary	*DOSBase;
struct IntuitionBase		*IntuitionBase=0L;
struct GfxBase	 			*GfxBase=0L;
//struct Library			*AslBase=0L,*AmigaGuideBase=0L,*RexxSysBase=0L,*UtilityBase=0L;
struct Library	 			*DiskfontBase=0L,*GadToolsBase=0L,*SFXSupportBase=0L;

/*-- Initialisation and Destruction -------------------------------------------------------------*/

int __saveds __asm __UserLibInit(REG(a6) struct Library *SFX_FXBase)
{
	int fail=0;

//	if(!(AmigaGuideBase=OpenLibrary("amigaguide.library",34L))) fail=1;		/* Libraries */
//	if(!fail && !(AslBase=OpenLibrary("asl.library",0L))) fail=1;
	if(!fail && !(DiskfontBase=OpenLibrary("diskfont.library",37))) fail=1;
	if(!fail && !(DOSBase=(struct DosLibrary *)OpenLibrary("dos.library",0L))) fail=1;
	if(!fail && !(GadToolsBase=OpenLibrary("gadtools.library",37L))) fail=1;
	if(!fail && !(GfxBase=(struct GfxBase *)OpenLibrary("graphics.library",37))) fail=1;
	if(!fail && !(IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",37))) fail=1;
//	if(!fail && !(RexxSysBase=OpenLibrary("rexxsyslib.library",0L))) fail=1;
	if(!fail && !(SFXSupportBase=OpenLibrary("progdir:libs/sfxsupport.library",0L))) fail=1;
//	if(!fail && !(UtilityBase=OpenLibrary("utility.library",37))) fail=1;

	return(fail);
}

void __saveds __asm __UserLibCleanup(REG(a6) struct Library *SFX_FXBase)
{
//	if(UtilityBase)		CloseLibrary(UtilityBase);
	if(SFXSupportBase)	CloseLibrary(SFXSupportBase);
//	if(RexxSysBase)		CloseLibrary(RexxSysBase);
	if(IntuitionBase)	CloseLibrary((struct Library *)IntuitionBase);
	if(GfxBase)			CloseLibrary((struct Library *)GfxBase);
	if(GadToolsBase)	CloseLibrary(GadToolsBase);
	if(DOSBase)			CloseLibrary((struct Library *)DOSBase);
	if(DiskfontBase)	CloseLibrary(DiskfontBase);
//	if(AslBase)			CloseLibrary(AslBase);
//	if(AmigaGuideBase)	CloseLibrary(AmigaGuideBase);
}

/*-- DataInitialisation -------------------------------------------------------------------------*/

void __saveds __asm LIBClientDataInit(REG(a0) RTime *RunTime_)
{
	FILE *prefs;

	RunTime=RunTime_;
	
	RecallSettings();			// can't do that earlier, because RunTime is not initialized before

	UnLock(CreateDir("progdir:_operators/Room"));

	if(prefs=fopen("env:sfx/operators/Room.prefs","rb"))
	{
		fclose(prefs);
		LoadSettings("env:sfx/operators/Room.prefs");	/* Try to load last used settings */
	}
}

/*-- Modulroutinen ------------------------------------------------------------------------------*/

UBYTE __saveds __asm LIBInterface(void)
{
	struct NewGadget ng;
	struct Window *fxWin;
	struct Gadget *g;
	struct Gadget *fxGList=0l;
	struct Gadget *fxGadgets[GADIX_SRC+SRC_IXCT];
	struct Menu *fxMenu;
	struct MenuItem *item;
	struct RastPort *fxrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	ULONG gnr;
	UBYTE quit=0,gh,enabled;
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime->scy[FRAME_HEIGHT(1)];
	UWORD winh=ys2+2+RunTime->scy[FRAME_HEIGHT(1)];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime->scx[308];
	UWORD xh;
	struct TagItem wintags[]=
	{
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)"Room",
		WA_ScreenTitle,		(ULONG)FXVERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	struct NewMenu FXMenu[]=
	{
		{ NM_TITLE, "Project",		0,		0,					0,	(APTR)0},
		{  NM_ITEM, "Load...",		"L",	0,					0,	(APTR)MENID_LOAD},
		{  NM_ITEM, "Save...",		"S",	0,					0,	(APTR)MENID_SAVE},
		{  NM_ITEM, "Reset",		"R",	0,					0,	(APTR)MENID_RESET},
		{  NM_ITEM, NM_BARLABEL,	0,		0,					0,	(APTR)0},
		{  NM_ITEM, "Go",			"G",	0,					0,	(APTR)MENID_GO},
		{  NM_ITEM, NM_BARLABEL,	0,		0,					0,	(APTR)0},
		{  NM_ITEM, "Help...",		"Help",	NM_COMMANDSTRING,	0,	(APTR)MENID_HELP},
		{  NM_ITEM, "About...",		"?",	NM_COMMANDSTRING,	0,	(APTR)MENID_ABOUT},
		{  NM_ITEM, NM_BARLABEL,	0,		0,					0,	(APTR)0},
		{  NM_ITEM, "Quit",			"Q",	0,					0,	(APTR)MENID_QUIT},

		{   NM_END, 0,				0,		0,					0,	(APTR)0}
	};

	InitOperator();
	enabled=(RunTime->aktsamples>0);

	if(g=CreateContext(&fxGList))
	{
		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_TopEdge		=ys2+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[80];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	="_Go";
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		fxGadgets[GADIX_GO]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[164];
		ng.ng_GadgetText	="_Reset";
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_RESET;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		g=AddSource(&fxparam.src,fxGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		if(g)
		{
			if((fxMenu=CreateMenus(FXMenu,GTMN_FrontPen,1,GTMN_NewLookMenus,TRUE,GTMN_FullMenu,TRUE,TAG_DONE)))
			{
				if(LayoutMenus(fxMenu,RunTime->VisualInfo,GTMN_TextAttr,RunTime->ScrFont,TAG_DONE))
				{
					wintags[0].ti_Data=(RunTime->xres-winw)>>1;
					wintags[1].ti_Data=(RunTime->yres-winh)>>1;
					wintags[2].ti_Data=winw;
					wintags[3].ti_Data=winh;
					wintags[6].ti_Data=(ULONG)fxGList;
					if(fxWin=OpenWindowTagList(0l,wintags))
					{
						GT_RefreshWindow(fxWin,0l);
						SetMenuStrip(fxWin,fxMenu);
						fxrp=fxWin->RPort;SetFont(fxrp,RunTime->scrfont);

						SetAPen(fxrp,1);
						RText(fxrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[TEXT_Y_POS(1)]);

						xh=RunTime->scx[308];
						DrawGadWB(fxrp,xs1,ys1,xh,RunTime->scy[FRAME_HEIGHT(1)]);		/* Source */
						DrawGadWB(fxrp,xs1,ys2,xh,RunTime->scy[FRAME_HEIGHT(1)]);		/* Go */

						SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);

						if(!enabled) GT_SetGadgetAttrs(fxGadgets[GADIX_GO],fxWin,0l,GA_Disabled,TRUE,TAG_DONE);

						while(!quit)
						{
							WaitPort(fxWin->UserPort);
							while(imsg=GT_GetIMsg(fxWin->UserPort))
							{
								iclass	=imsg->Class;
								iadr	=imsg->IAddress;
								icode	=imsg->Code;
								GT_ReplyIMsg(imsg);
								switch(iclass)
								{
									case IDCMP_ACTIVEWINDOW:
										if(!enabled && RunTime->aktsamples)
										{
											enabled=1;
											GT_SetGadgetAttrs(fxGadgets[GADIX_GO],fxWin,0l,GA_Disabled,FALSE,TAG_DONE);
											fxparam.src.srcbuf=LockBuffer(RunTime->aktbuf);
											fxparam.src.rmode=SetRngMode(fxparam.src.srcbuf);
											SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);
										}
										else if(enabled && !RunTime->aktsamples)
										{
											enabled=0;
											GT_SetGadgetAttrs(fxGadgets[GADIX_GO],fxWin,0l,GA_Disabled,TRUE,TAG_DONE);
										}
										break;
									case IDCMP_GADGETUP:
										gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
										gh=HandleSource(&fxparam.src,fxWin,fxGadgets,GADID_SRC,GADIX_SRC,gnr,icode);
										if(!gh)
										{
											switch(gnr)
											{
												case GADID_GO:
													SaveSettings("env:sfx/operators/Room.prefs");		/* Store actual settings */
													quit=1;
													break;
												case GADID_RESET:
													DoneOperator();
													RecallSettings();
													InitOperator();
													SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);
													break;
											}
										}
										break;
									case IDCMP_VANILLAKEY:
										switch(icode)
										{
											case 0x0A:
											case 0X0D:
											case 'G':			/* Go */
											case 'g':
												SaveSettings("env:sfx/operators/Room.prefs");		/* Store actual settings */
												quit=1;
												break;
											case 'R':			/* Reset */
											case 'r':
												DoneOperator();
												RecallSettings();
												InitOperator();
												SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);
												break;
											case 0x1B:			/* ESC */
												quit=2;
												break;
											case '?':
												Message(FXVERS);
												break;
										}
										break;
									case IDCMP_CLOSEWINDOW:
										quit=2;
										break;
									case IDCMP_RAWKEY:
										if(icode==0x5F) Help("/_operators/Room.guide/main",RunTime->fxoff,RunTime->anzfx);
										break;
									case IDCMP_MENUPICK:
										while(icode!=MENUNULL)
										{
											if(item=ItemAddress(fxMenu,icode))
											{
												switch((ULONG)MENU_USERDATA(item))
												{
													case MENID_LOAD:
														DoneOperator();
														LoadSettings(0l);
														InitOperator();
														SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);
														break;
													case MENID_SAVE:
														SaveSettings(0l);
														break;
													case MENID_RESET:
														DoneOperator();
														RecallSettings();
														InitOperator();
														SetSource   (&fxparam.src,fxWin,fxGadgets,GADIX_SRC);
														break;
													case MENID_GO:
														SaveSettings("env:sfx/operators/Room.prefs");		/* Store actual settings */
														quit=1;
														break;
													case MENID_HELP:
														Help("/_operators/Room.guide/main",RunTime->fxoff,RunTime->anzfx);
														break;
													case MENID_ABOUT:
														Message(FXVERS);
														break;
													case MENID_QUIT:
														quit=2;
														break;
												}
												icode=item->NextSelect;
											}
										}
										break;
								}
							}
						}
						ClearMenuStrip(fxWin);
						CloseWindow(fxWin);
					}
					else Message(RunTime->mess[43]);
				}
				else Message(RunTime->mess[42]);
				FreeMenus(fxMenu);
			}
			else Message(RunTime->mess[41]);
		}
		else Message(RunTime->mess[8]);
		FreeGadgets(fxGList);
	}
	else Message(RunTime->mess[9]);
	if(quit!=1) DoneOperator();
	return((UBYTE)(2-quit));
}

UBYTE __saveds __asm LIBProcess(REG(d0) UBYTE callmd,REG(a0) SInfo *si,REG(a1) char *fn)
{
	register ULONG i;
	register UBYTE l,k;
	PWInfo *pwi;
	char title[80];
	SInfo *dstbuf,*srcbuf;
	
	double rx,ry,rz;			/* Roomsize */
	double mx,my,mz;			/* Microphonepos */
	double sx,sy,sz;			/* Speakerpos */
	double ax,ay,az;			/* Audiopos (reflected) */
	double wx,wy,wz;			/* Wallpos (echo to calc) */ 
	double ms,mw,aw,ma;			/* Micro->Speaker, Micro->Wall, Audio->Wall, Micro->Audio */
	double pid2=2.0/PI,delf;	/* Umfang, some factors */
	double vol;					/* Volume */
	double wxl,wyl,ws,we,wd;	/* Wallloop */
	double xd,yd,zd;			/* Distances */
	double val,ampf;
	double v1,v2,v3,v4,v5,v6;	/* Volume */
	ULONG anz,depth;			/* Echos pro Koordinatenachse -> anzges=anz*anz*6, Echotracingdepth */
	ULONG del;					/* Delay */

	ULONG rm_start,rm_len;
	UBYTE ret=1;
//	UBYTE resfail=0;

	if(callmd==CM_REXX)
	{
		InitOperator();
		UnlockBuffer(fxparam.src.srcbuf);
		ret&=((fxparam.src.srcbuf=LockBuffer(GetBufferPtr(RunTime->srclist[0],-1)))!=0);
		if(!ret)
		{
			DoneOperator();
			return(ret);
		}
	}

	srcbuf=fxparam.src.srcbuf;
	SetRngs(srcbuf,&rm_start,&rm_len,fxparam.src.rmode);

// hardcoded defaults

	anz=4;
	depth=1;
	rx=100.0;ry=10.0;rz=40.0;
	mx= 0.5;my= 0.5;mz= 0.3;
	mx*= rx;my*= ry;mz*= rz;
	wd=1.0/(double)anz;
	ws=wd/2.0;we=1.0-ws;
	delf=(double)srcbuf->srat/340.0;			/* Schallgeschwindigkeit : 340 m/s */
	ampf=30.0/(double)(anz*anz*6);
	v1=0.6*ampf*pid2;v6=0.2*ampf*pid2;			/* Lautstärken : vorne, hinten */
	v2=0.4*ampf*pid2;v3=0.4*ampf*pid2;			/* Lautstärken : links, rechts */
	v4=0.4*ampf*pid2;v5=0.4*ampf*pid2;			/* Lautstärken : vorne, hinten */

// hardcoded defaults

	if(dstbuf=NewSample())
	{
		if(AllocSampleMem(dstbuf,srcbuf->slen,(srcbuf->channels+1)))
		{
			BufferFill(srcbuf,dstbuf);
			for(l=0;l<(srcbuf->channels+1);l++)
			{
				if(srcbuf->channelmask&(1<<l))
				{
					sprintf(title,"Working on Room[%d]",l+1);
					if(rm_start>0) memcpy(&dstbuf->buf[l][0],&srcbuf->buf[l][0],(rm_start<<1));
					if(rm_start+rm_len<srcbuf->slen) memcpy(&dstbuf->buf[l][rm_start+rm_len],&srcbuf->buf[l][rm_start+rm_len],((srcbuf->slen-(rm_start+rm_len))<<1));
					if(pwi=InitProWin(rm_len,title))
					{
						for(i=rm_start;i<rm_start+rm_len;i++)
						{
							if(RefreshProWin(pwi,i-rm_start)) { i=rm_start+rm_len;l=4; }
							else
							{
								sx= 0.5;sy= 0.5;sz= 0.6;	// später modulieren */
								sx*= rx;sy*= ry;sz*= rz;	

								val=(double)srcbuf->buf[l][i];

								xd=mx-sx;yd=my-sy;zd=mz-sz;ms=sqrt(xd*xd+yd*yd+zd*zd);	// direct
								vol=ampf/sqrt(ms);
								del=i+(ULONG)(ms*delf);
								if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);

								wz=0.0;az=sz;											// back wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wx=wxl*rx;ax=wx+(wx-sx);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wy=wyl*ry;ay=wy+(wy-sy);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v6*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
								wz=rz;az=sz;											// front wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wx=wxl*rx;ax=wx+(wx-sx);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wy=wyl*ry;ay=wy+(wy-sy);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v1*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
								wx=0.0;ax=sx;											// left wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wz=wxl*rz;az=wz+(wz-sz);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wy=wyl*ry;ay=wy+(wy-sy);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v2*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
								wx=rx;ax=sx;											// right wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wz=wxl*rz;az=wz+(wz-sz);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wy=wyl*ry;ay=wy+(wy-sy);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v3*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
								wy=0.0;ay=sy;											// top wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wx=wxl*rx;ax=wx+(wx-sx);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wz=wyl*rz;az=wz+(wz-sz);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v4*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
								wy=ry;ay=sy;											// bottom wall
								for(wxl=ws;wxl<we;wxl+=wd)
								{
									wx=wxl*rx;ax=wx+(wx-sx);
									for(wyl=ws;wyl<we;wyl+=wd)
									{
										wz=wyl*rz;az=wz+(wz-sz);
										xd=mx-ax;yd=my-ay;zd=mz-az;ma=sqrt(xd*xd+yd*yd+zd*zd);
										xd=mx-wx;yd=my-wy;zd=mz-wz;mw=sqrt(xd*xd+yd*yd+zd*zd);
										xd=ax-wx;yd=ay-wy;zd=az-wz;aw=sqrt(xd*xd+yd*yd+zd*zd);
										vol=v5*angle(ma,mw,aw);
										del=i+(ULONG)((mw+aw)*delf);
										if(del<dstbuf->slen) dstbuf->buf[l][del]+=(WORD)(vol*val);
									}
								}
							}
						}
						RemoveProWin(pwi);
					}
				}
				else memcpy(dstbuf->buf[l],srcbuf->buf[l],(srcbuf->slen<<1));
			}
			AddSample(dstbuf,0l);
		}
		else { RemSample(dstbuf);Message(RunTime->mess[6]);ret=1; }
	}
	else { Message(RunTime->mess[11]);ret=1; }
	DoneOperator();
	return(ret);
}

__inline double angle(double ma,double mw,double aw)
{
	double um,vol;		/* Umfang,Lautstärke */

	um=0.5*(ma+mw+aw);
	vol=(acos(sqrt((um*(um-ma))/(mw*aw))))/sqrt(mw+aw);
	return(vol);
}

void __saveds __asm LIBSetDefaults(void)
{
//	fxparam.src.srcbuf=0L;
	fxparam.src.rmode=0;
}

void __saveds __asm LIBHandleARexxCmd(REG(a0) char *params,REG(a1) char *ret)
{
	UBYTE cf=0;/*,pf;*/
	char fn[256];

	if(!stricmp(&params[0],"SetOperatorParam"))
	{
		cf=1;/*pf=0;*/
//		if(pf) SaveSettings("env:sfx/operators/Room.prefs");
	}
	if(!cf && !stricmp(&params[0],"GetOperatorParam"))
	{
		cf=1;/*pf=0;*/
	}
	if(!cf && !stricmp(&params[0],"SetOperatorPreset"))
	{
		DoneOperator();
		if(!stricmp(&params[MaxParLen2],"default")) RecallSettings();
		if( stricmp(&params[MaxParLen2],"current"))
		{
			sprintf(fn,"Room/%s.prefs",&params[MaxParLen2]);
			LoadSettings(fn);
		}
		InitOperator();
	}
}

/*-- local Stuff --------------------------------------------------------------------------------*/

void LoadSettings(char *fname) { LoadModSettings(fname,"Room",fxdesc,VERSION,REVISION); }
void SaveSettings(char *fname) { SaveModSettings(fname,"Room",fxdesc,VERSION,REVISION); }

void RecallSettings(void)
{
	FILE *prefs;

	LIBSetDefaults();
	if(prefs=fopen("Room/default.prefs","rb"))
	{
		fclose(prefs);
		LoadSettings("Room/default.prefs");			/* Try to read saved defaults */
	}
}

void InitOperator(void) { InitFXParams(fxdesc); }
void DoneOperator(void) { DoneFXParams(fxdesc); }

/*-- private Stuff ------------------------------------------------------------------------------*/

/*-- EOF ----------------------------------------------------------------------------------------*/
