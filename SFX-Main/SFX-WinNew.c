/****h* SoundFX/SFX-WinNew.c [4.2] *
*
*  NAME
*    SFX-WinNew.c
*  COPYRIGHT
*    $VER: SFX-WinNew.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    new empty sample - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    24.Nov.2001
*  MODIFICATION HISTORY
*    30.Oct.2002    V 4.2   most recent version
*    24.Nov.2001    V 4.2   initial version
*  NOTES
*
*******
*/

#define SFX_WinNew_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void InfoWin(void);

//-- defines ------------------------------------------------------------------

#define GADID_SLEN          (0)
#define GADIX_SLEN          (0)

#define GADID_SRATPOPUP     (GADID_SLEN+1)

#define GADID_SRAT          (GADID_SRATPOPUP+1)
#define GADIX_SRAT          (GADIX_SLEN+1)

#define GADID_CHANNELS      (GADID_SRAT+1)
#define GADIX_CHANNELS      (GADIX_SRAT+1)

#define GADID_OKAY          (GADID_CHANNELS+1)

#define LAST_GADIX          (GADIX_CHANNELS+1)

//-- definitions --------------------------------------------------------------

void NewSampleWin(void)
{
	struct NewGadget ng;
	struct Window *win;
	struct Gadget *g;
	struct Gadget *glist=NULL;
	struct Gadget *gadgets[LAST_GADIX];
	struct RastPort *rp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[208];
	UWORD xh;
	UBYTE *ChannelLabels[]={ "Mono","Stereo","Quadro",NULL };
	Param param,slenstr="1.0 s",sratstr="44100 hz";
	UBYTE channels=0;
	ULONG slen,srat;
	LONG rate;
	SInfo *si;

	if(g=CreateContext(&glist)) {
		ng.ng_LeftEdge      =xs1+RunTime.scx[(208-80)>>1];
		ng.ng_TopEdge       =ys2+RunTime.scy[2];
		ng.ng_Width         =RunTime.scx[80];
		ng.ng_Height        =RunTime.scy[11];
		ng.ng_GadgetText    ="_Okay";
		ng.ng_TextAttr      =RunTime.ScrFontB;
		ng.ng_GadgetID      =GADID_OKAY;
		ng.ng_Flags         =PLACETEXT_IN;
		ng.ng_VisualInfo    =RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge      =xs1+RunTime.scx[65];                   /* Sample Options */
		ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_Width         =RunTime.scx[100];
		ng.ng_GadgetText    ="SLen";
		ng.ng_TextAttr      =RunTime.ScrFont;
		ng.ng_GadgetID      =GADID_SLEN;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,slenstr,TAG_DONE);
		gadgets[GADIX_SLEN]=g;

		ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width         =RunTime.scx[15];
		ng.ng_TextAttr      =RunTime.SigFont;
		ng.ng_GadgetText    =IMAGE_PopUp;
		ng.ng_GadgetID      =GADID_SRATPOPUP;
		ng.ng_Flags         =PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge      =xs1+RunTime.scx[81];
		ng.ng_Width         =RunTime.scx[75];
		ng.ng_TextAttr      =RunTime.ScrFont;
		ng.ng_GadgetText    ="";
		ng.ng_GadgetID      =GADID_SRAT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,sratstr,TAG_DONE);
		gadgets[GADIX_SRAT]=g;

		ng.ng_LeftEdge      =xs1+RunTime.scx[65];
		ng.ng_TopEdge       =ys1+RunTime.scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width         =RunTime.scx[100];
		ng.ng_GadgetText    ="Channels";
		ng.ng_GadgetID      =GADID_CHANNELS;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ChannelLabels,GTCY_Active,channels,TAG_DONE);
		gadgets[GADIX_CHANNELS]=g;

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)glist;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrjNew);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(win=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(win,NULL);
				rp=win->RPort;SetFont(rp,RunTime.scrfont);

				SetAPen(rp,1);
				RText(rp,"SRat",xs1+RunTime.scx[ 57],ys1+RunTime.scy[FRMTEXT_YPOS(2)]);

				xh=RunTime.scx[208];
				DrawTitle(rp,"Options"                    ,xs1,ys1,xh);

				DrawGadWB(rp,xs1,ys1,xh,ANYFRM_HEIGHT(3));                    /* Options */
				DrawGadWB(rp,xs1,ys2,xh,CMDFRM_HEIGHT);                       /* Okay */

				while(!quit) {
					WaitPort(win->UserPort);
					while(imsg=GT_GetIMsg(win->UserPort)) {
						iclass  =imsg->Class;
						iadr    =imsg->IAddress;
						icode   =imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_OKAY:
									quit=2;break;
								case GADID_SLEN:
									HandleParameter(slenstr,win,gadgets[GADIX_SLEN]);
									break;
								case GADID_SRATPOPUP:
									UC_ConvertParameter(param,sratstr,UGRP_ABSFREQUENCY,AFRQ_HZ);
									srat=atof(param);
									if(rate=GetPeriode(srat,NULL)) {
										sprintf(param,"%ld hz",rate);
										UC_ConvertParameter(sratstr,param,UGRP_ABSFREQUENCY,UC_GetParameterUnit(sratstr,UGRP_ABSFREQUENCY));
										GT_SetGadgetAttrs(gadgets[GADIX_SRAT],win,NULL,GTST_String,sratstr,TAG_DONE);
									}
									break;
								case GADID_SRAT:
									HandleParameter(sratstr,win,gadgets[GADIX_SRAT]);
									break;
								case GADID_CHANNELS:
									channels=icode;
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ESC:               /* Cancel */
									quit=1;break;
								case KEY_ENTER:             /* Okay */
								case KEY_RETURN:
								case 'O':
								case 'o':
									quit=2;break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) quit=1;
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.05.XX.html");
					}
				}
				CloseWindow(win);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(glist);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	if(quit==2) {
		//-- create new sample buffer
		UC_ConvertParameter(param,sratstr,UGRP_ABSFREQUENCY,AFRQ_HZ);
		srat=atof(param);
		if(UC_GetParameterUnit(slenstr,UGRP_ABSTIME)==ATME_SAMPLES) slen=atof(slenstr);
		else { UC_ConvertParameter(param,slenstr,UGRP_ABSTIME,ATME_MS);slen=atof(param)*((double)srat/1000.0); }
		if(slen<2) slen=2;
		MSG3("create new sample buffer with [slen=%ld,srat=%ld,channels=%ld]",slen,srat,channels+1);
		if(si=NewSample()) {
			si->channels=(1L<<channels)-1;
			si->channelmask=(1L<<(si->channels+1))-1;
			si->aktch=si->channels+1;
			if(AllocSampleMem(si,slen,(si->channels+1))) {
				si->srat=srat;
				si->slen=slen;
				si->zoomxl=slen;
				AddSample(si,"New");
			}
			else { RemSample(si);Message(errAllocateMemory,NULL,__FILE__,__LINE__); }
		}
		else Message(errAddSampleBuffer,NULL,__FILE__,__LINE__);
	}
}

//-- eof ----------------------------------------------------------------------
