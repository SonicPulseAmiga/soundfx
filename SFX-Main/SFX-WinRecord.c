/****h* SoundFX/SFX-WinRecord.c [4.3] *
*
*  NAME
*    SFX-WinRecord.c
*  COPYRIGHT
*    $VER: SFX-WinRecord.c 4.3 (13.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    record window - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    01.Jun.2000
*  MODIFICATION HISTORY
*    13.Oct.2003	V 4.3	most recent version
*    06.Jun.2001	V 4.2	most recent version
*    13.Feb.2001	V 4.1	most recent version
*							A : support for mono and for 8-bit sampling
*    01.Jun.2000	V 4.0	initial version
*  NOTES
*    * it seems that a parallel-port sampler always delivers mono and
*      ahi make stereo out of it
*******
*/

#define SFX_WinRecord_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void RecordWin(void);
void Refr_RecordWin(struct Gadget *recGadgets[],struct Window *recwin);

ULONG __asm __interrupt __saveds RecordAhiDevInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHIRecordMessage *chan);

void __asm RecordTask(void);

//-- defines ------------------------------------------------------------------

#define OVERLOAD 0.99994
#define SLIDER_RES 256.0

//-- GadIDs

#define GADID_RECORD		0
#define GADIX_RECORD		0

#define GADID_STOP			(GADID_RECORD+1)
#define GADIX_STOP			(GADIX_RECORD+1)

#define GADID_RESET			(GADID_STOP+1)
#define GADIX_RESET			(GADIX_STOP+1)

#define GADID_AUTO			(GADID_RESET+1)
#define GADIX_AUTO			(GADIX_RESET+1)

#define GADID_AUDIOMODESEL	(GADID_AUTO+1)

#define GADID_AUDIOMODE		(GADID_AUDIOMODESEL+1)
#define GADIX_AUDIOMODE		(GADIX_AUTO+1)

#define GADID_RECSRC		(GADID_AUDIOMODE+1)
#define GADIX_RECSRC		(GADIX_AUDIOMODE+1)

#define GADID_RECGAIN		(GADID_RECSRC+1)
#define GADIX_RECGAIN		(GADIX_RECSRC+1)

#define GADID_RECGAINLAB	(GADID_RECGAIN+1)
#define GADIX_RECGAINLAB	(GADIX_RECGAIN+1)

#define GADID_MONSRC		(GADID_RECGAINLAB+1)
#define GADIX_MONSRC		(GADIX_RECGAINLAB+1)

#define GADID_MONGAIN		(GADID_MONSRC+1)
#define GADIX_MONGAIN		(GADIX_MONSRC+1)

#define GADID_MONGAINLAB	(GADID_MONGAIN+1)
#define GADIX_MONGAINLAB	(GADIX_MONGAIN+1)

#define GADID_LEVLLAB		(GADID_MONGAINLAB+1)
#define GADIX_LEVLLAB		(GADIX_MONGAINLAB+1)

#define GADID_MAXLLAB		(GADID_LEVLLAB+1)
#define GADIX_MAXLLAB		(GADIX_LEVLLAB+1)

#define GADID_LEVRLAB		(GADID_MAXLLAB+1)
#define GADIX_LEVRLAB		(GADIX_MAXLLAB+1)

#define GADID_MAXRLAB		(GADID_LEVRLAB+1)
#define GADIX_MAXRLAB		(GADIX_LEVRLAB+1)

#define GADID_RECINFO		(GADID_MAXRLAB+1)
#define GADIX_RECINFO		(GADIX_MAXRLAB+1)

#define LAST_GADIX			(GADIX_RECINFO+1)

//-- globals ------------------------------------------------------------------

extern struct Library    *AHIBase;
extern struct MsgPort    *AHImp;
extern struct AHIRequest *AHIio;
extern BYTE   AHIDevice;

UBYTE *EmptyLabels[]={ " ",NULL };
UBYTE **RecordLabels,**MonitorLabels;
double rec_lo,rec_up,rec_st;
double mon_lo,mon_up,mon_st;
struct AHIAudioCtrl *raudioctrl=NULL;
struct Hook RecordHook={0};
double aktval,ampenvl,ampenvr,maxenvl,maxenvr,cmaxenv;
double efc1=0.995,efc2=0.005;
ULONG recsize,recchannels,recbits,recfmt;
BYTE *rbuffer8;
WORD *rbuffer16;
UWORD bloop;

BOOL recording=FALSE;
char strFileName[FILENAME_MAX];

struct RecordData
{
	BPTR			fh;
	BYTE			*buffer[2],*ptr;			// WORD
	ULONG			blen[2];
	ULONG			a4,a6;
	BYTE			mainsig,worksig;			// for communication
	struct Process	*maintask,*worktask;
	UBYTE			state,bnum,step;
	char			*fn;
} rd;

struct TagItem ProcTags[]=
{
	NP_Entry,		NULL,
	NP_StackSize,	10000,
	NP_Name,		(ULONG)(SFX_PREFIX"Record"),
	NP_Priority,	1,				/* higher priority than sfx */
	TAG_DONE
};
struct Process *ThatTask;

//-- definitions --------------------------------------------------------------

void RecordWin(void)
{
	struct NewGadget ng;
	struct Window *recwin;
	struct Gadget *g;
	struct Gadget *recGList=NULL;
	struct Gadget *recGadgets[LAST_GADIX];
	struct RastPort *recrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=FALSE,res,scroll=FALSE,**dptr;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(2);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD ys5=ys4+1+ANYFRM_HEIGHT(4);
	UWORD ys6=ys5+1+RunTime.scy[FRAME_HEIGHT(1)];
	UWORD winh=ys6+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[420];
	UWORD xh,ys,yh,xp1,xp2,xp3,yp1,yp2,yh2,xh2;
	char strBuffer[100];
	char *xstr;
	UBYTE xw=3*RunTime.scafont->tf_XSize,off=RunTime.scafont->tf_YSize;
	double dbl;
	LONG gain;
	ULONG lev;
	UBYTE fpen=1,autogain=FALSE;
	Param tmp,rectime;

	struct AHIAudioModeRequester *amreq;
	struct TagItem AMReqFiltertags[]={
		AHIDB_Realtime, TRUE,		// Remove the FILESAVE modes (among others?)
		AHIDB_Record,	TRUE,		// support Recoding ?
		TAG_DONE
	};
	struct TagItem AMReqtags[]={
		AHIR_TitleText,			0,
		AHIR_PubScreenName,		(ULONG)SFX_PUBSCRNAME,
		AHIR_InitialLeftEdge,	0,
		AHIR_InitialTopEdge,	15,
		AHIR_InitialWidth,		300,
		AHIR_InitialHeight,		0,
		AHIR_DoMixFreq,			TRUE,
		AHIR_FilterTags,		0,
		TAG_DONE
	};

	recsize=0;recchannels=1;recbits=8;
	recfmt=0;

	AMReqtags[0].ti_Data=(ULONG)GetString(strSelectAudioMd);
	AMReqtags[2].ti_Data=(sfxprefs_gui.xres-300)>>1;
	AMReqtags[5].ti_Data=sfxprefs_gui.yres-30;      
	AMReqtags[7].ti_Data=(ULONG)AMReqFiltertags;
	if(amreq=AHI_AllocAudioRequestA(AMReqtags)) {
		if(g=CreateContext(&recGList)) {
			ng.ng_LeftEdge		=xs1+RunTime.scx[110];
			ng.ng_TopEdge		=ys6+RunTime.scy[2];
			ng.ng_Width			=RunTime.scx[80];
			ng.ng_Height		=RunTime.scy[11];
			ng.ng_GadgetText	="_Record";
			ng.ng_TextAttr		=RunTime.ScrFont;
			ng.ng_GadgetID		=GADID_RECORD;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=RunTime.VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',GA_Disabled,recording,TAG_DONE);
			recGadgets[GADIX_RECORD]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[230];
			ng.ng_GadgetText	="_Stop";
			ng.ng_GadgetID		=GADID_STOP;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',GA_Disabled,!recording,TAG_DONE);
			recGadgets[GADIX_STOP]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* Audiomode */
			ng.ng_TopEdge		=ys1+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width			=RunTime.scx[15];
			ng.ng_Height		=RunTime.scy[11];
			ng.ng_TextAttr		=RunTime.SigFont;
			ng.ng_GadgetText	=IMAGE_AudioReq;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_GadgetID		=GADID_AUDIOMODESEL;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

			ng.ng_LeftEdge		=xs1+RunTime.scx[105];
			ng.ng_Width			=RunTime.scx[240];
			ng.ng_TextAttr		=RunTime.ScrFont;
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=GADID_AUDIOMODE;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,GTTX_CopyText,TRUE,TAG_DONE);
			recGadgets[GADIX_AUDIOMODE]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[53];			/* Record */
			ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width			=RunTime.scx[292];
			ng.ng_GadgetText	="Src";
			ng.ng_GadgetID		=GADID_RECSRC;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EmptyLabels,GTCY_Active,NULL,TAG_DONE);
			recGadgets[GADIX_RECSRC]=g;

			ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width			=RunTime.scx[256];
			ng.ng_GadgetText	="Gain";
			ng.ng_GadgetID		=GADID_RECGAIN;
			g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,(ULONG)SLIDER_RES,GTSL_Level,0,PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
			recGadgets[GADIX_RECGAIN]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[310];
			ng.ng_Width			=RunTime.scx[50];
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=GADID_RECGAINLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,TAG_DONE);
			recGadgets[GADIX_RECGAINLAB]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[385];
			ng.ng_Width			=RunTime.scx[26];
			ng.ng_GadgetText	="Auto";
			ng.ng_GadgetID		=GADID_AUTO;
			g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,autogain,GTCB_Scaled,TRUE,TAG_DONE);
			recGadgets[GADIX_AUTO]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[53];			/* Monitor */
			ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET];
			ng.ng_Width			=RunTime.scx[292];
			ng.ng_GadgetText	="Src";
			ng.ng_GadgetID		=GADID_MONSRC;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EmptyLabels,GTCY_Active,NULL,TAG_DONE);
			recGadgets[GADIX_MONSRC]=g;

			ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width			=RunTime.scx[256];
			ng.ng_GadgetText	="Gain";
			ng.ng_GadgetID		=GADID_MONGAIN;
			g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,(ULONG)SLIDER_RES,GTSL_Level,0,PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
			recGadgets[GADIX_MONGAIN]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[310];
			ng.ng_Width			=RunTime.scx[50];
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=GADID_MONGAINLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,TAG_DONE);
			recGadgets[GADIX_MONGAINLAB]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[310];		/* Levelmeter */
			ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_Width			=RunTime.scx[50];
			ng.ng_GadgetID		=GADID_LEVLLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_FrontPen,fpen,TAG_DONE);
			recGadgets[GADIX_LEVLLAB]=g;

			ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_GadgetID		=GADID_LEVRLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_FrontPen,fpen,TAG_DONE);
			recGadgets[GADIX_LEVRLAB]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[365];
			ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+12];
			ng.ng_GadgetID		=GADID_MAXLLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_FrontPen,fpen,TAG_DONE);
			recGadgets[GADIX_MAXLLAB]=g;

			ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+24];
			ng.ng_GadgetID		=GADID_MAXRLAB;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,FALSE,GTTX_FrontPen,fpen,TAG_DONE);
			recGadgets[GADIX_MAXRLAB]=g;

			ng.ng_TopEdge		=ys4+RunTime.scy[FRMCONTENT_YOFFSET+36];
			ng.ng_GadgetText	="Reset";
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_GadgetID		=GADID_RESET;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
			recGadgets[GADIX_RESET]=g;

			ng.ng_LeftEdge		=xs1+RunTime.scx[35];		/* Recordinfo */
			ng.ng_TopEdge		=ys5+RunTime.scy[2];
			ng.ng_Width			=RunTime.scx[350];
			ng.ng_GadgetText	="Info";
			ng.ng_Flags			=PLACETEXT_LEFT;
			ng.ng_GadgetID		=GADID_RECINFO;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
			recGadgets[GADIX_RECINFO]=g;

			if(g) {
				MemUsage(sfxtitle);
				Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
				Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
				Dialog_Wintags_InnerWidth	=winw;
				Dialog_Wintags_InnerHeight	=winh;
				Dialog_Wintags_IDCMP		=BUTTONIDCMP|LISTVIEWIDCMP|SLIDERIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_INTUITICKS;
				Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
				Dialog_Wintags_Gadgets		=(ULONG)recGList;
				Dialog_Wintags_Title		=(ULONG)GetString(strRecord);
				Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
				if(recwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
					GT_RefreshWindow(recwin,NULL);
					recrp=recwin->RPort;SetFont(recrp,RunTime.scrfont);

					SetAPen(recrp,1);
					RText(recrp,"AudioMode",xs1+RunTime.scx[81],ys1+RunTime.scy[FRMTEXT_YPOS(1)]);

					xh=RunTime.scx[420];
					DrawTitle(recrp,"AHI Record Mode"		,xs1,ys1,xh);
					DrawTitle(recrp,"Record-Source"			,xs1,ys2,xh);
					DrawTitle(recrp,"Monitor-Source"		,xs1,ys3,xh);
					DrawTitle(recrp,"Level Meter & Status"	,xs1,ys4,xh);

					DrawGadWB(recrp,xs1,ys1,xh,ANYFRM_HEIGHT(1));				/* Audio-Mode */
					DrawGadWB(recrp,xs1,ys2,xh,ANYFRM_HEIGHT(2));				/* Rec - Source/Gain */
					DrawGadWB(recrp,xs1,ys3,xh,ANYFRM_HEIGHT(2));				/* Mon - Source/Gainr */
					DrawGadWB(recrp,xs1,ys4,xh,ANYFRM_HEIGHT(4));				/* Levelmeter */
					DrawGadWB(recrp,xs1,ys5,xh,RunTime.scy[FRAME_HEIGHT(1)]);	/* Info */
					DrawGadWB(recrp,xs1,ys6,xh,CMDFRM_HEIGHT);					/* Record / Stop */


					ys=RunTime.scy[FRMCONTENT_YOFFSET+12];							/* Ruler + Scala */
					yh=RunTime.scy[12];												/* Height Levelmeter */
					xh=RunTime.scx[250];
					yp1=ys4+ys-1;
					yp2=ys4+ys+yh+yh+1;
					xp1=xs1+RunTime.scx[53];
					DrawGadBW(recrp,xp1,yp1     ,xh,yh);							/* Level-L */
					DrawGadBW(recrp,xp1,yp1+2+yh,xh,yh);							/* Level-R */
					SetAPen(recrp,1);
					RText(recrp,"Left",xs1+RunTime.scx[45],ys4+RunTime.scy[FRMTEXT_YPOS(2)]);
					RText(recrp,"Right",xs1+RunTime.scx[45],ys4+RunTime.scy[FRMTEXT_YPOS(3)]);
					SetFont(recrp,RunTime.scafont);
					xstr=ToYUnit(RunTime.sfxprefs_sbuf->unity,0);
					Move(recrp,xp1,yp1);Draw(recrp,xp1,yp1-8);Move(recrp,xp1-xw,yp1-18+off);Text(recrp,xstr,6);
					Move(recrp,xp1,yp2);Draw(recrp,xp1,yp2+8);Move(recrp,xp1-xw,yp2+ 9+off);Text(recrp,xstr,6);
					xp1=xs1+RunTime.scx[53]+(xh>>1);
					xstr=ToYUnit(RunTime.sfxprefs_sbuf->unity,16384);
					Move(recrp,xp1,yp1);Draw(recrp,xp1,yp1-8);Move(recrp,xp1-xw,yp1-18+off);Text(recrp,xstr,6);
					Move(recrp,xp1,yp2);Draw(recrp,xp1,yp2+8);Move(recrp,xp1-xw,yp2+ 9+off);Text(recrp,xstr,6);
					xp1=xs1+RunTime.scx[53]+xh;
					xstr=ToYUnit(RunTime.sfxprefs_sbuf->unity,32767);
					Move(recrp,xp1,yp1);Draw(recrp,xp1,yp1-8);Move(recrp,xp1-xw,yp1-18+off);Text(recrp,xstr,6);
					Move(recrp,xp1,yp2);Draw(recrp,xp1,yp2+8);Move(recrp,xp1-xw,yp2+ 9+off);Text(recrp,xstr,6);
					SetFont(recrp,RunTime.scrfont);
					xp1=xs1+RunTime.scx[53]+1;
					xp2=xs1+RunTime.scx[53]+xh-2;
					yp1=ys4+ys;
					yp2=ys4+ys+yh+1+1;
					yh2=yh-2;
					xh2=xh-2;
					
					Refr_RecordWin(recGadgets,recwin);
					sprintf(tmp,"%lf ms",(double)recsize/((double)optp_Ahi.recorate/1000.0));
					UC_ConvertParameter(rectime,tmp,UGRP_ABSTIME,ATME_H_M_S_MS);
					sprintf(strBuffer,"%ld samples = %s",recsize,rectime);
					GT_SetGadgetAttrs(recGadgets[GADIX_RECINFO],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);

					while(!quit) {
						WaitPort(recwin->UserPort);
						while(imsg=GT_GetIMsg(recwin->UserPort)) {
							iclass	=imsg->Class;
							iadr	=imsg->IAddress;
							icode	=imsg->Code;
							GT_ReplyIMsg(imsg);
							if(iclass==IDCMP_GADGETUP) {
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_RECORD:
										recording=TRUE;
										GT_SetGadgetAttrs(recGadgets[GADIX_RECORD],recwin,NULL,GA_Disabled,recording ,TAG_DONE);
										GT_SetGadgetAttrs(recGadgets[GADIX_STOP  ],recwin,NULL,GA_Disabled,!recording,TAG_DONE);
										break;
									case GADID_STOP:
										recording=FALSE;
										GT_SetGadgetAttrs(recGadgets[GADIX_RECORD],recwin,NULL,GA_Disabled,recording,TAG_DONE);
										GT_SetGadgetAttrs(recGadgets[GADIX_STOP  ],recwin,NULL,GA_Disabled,!recording,TAG_DONE);
										break;
									case GADID_RESET:
										maxenvl=maxenvr=0.0;
										break;
									case GADID_AUTO:
										autogain=((recGadgets[GADIX_AUTO]->Flags&GFLG_SELECTED)==GFLG_SELECTED);break;
										break;
									case GADID_AUDIOMODESEL:
										if(res=AHI_AudioRequest(amreq,
																AHIR_TitleText,GetString(strSelectAudioMd),
																AHIR_InitialAudioID,optp_Ahi.recomode,
																AHIR_InitialMixFreq,optp_Ahi.recorate,
																TAG_DONE)) {
											optp_Ahi.recomode=amreq->ahiam_AudioID;
											optp_Ahi.recorate=amreq->ahiam_MixFreq;
											Refr_RecordWin(recGadgets,recwin);
										}
										else {
											switch(IoErr()) {
												case ERROR_NO_FREE_STORE:
													Message(strNoID,"AHI : ERROR_NO_FREE_STORE",NULL,0);
													break;
												case ERROR_NO_MORE_ENTRIES:
													Message(strNoID,"AHI : ERROR_NO_MORE_ENTRIES",NULL,0);
													break;
											}
										}
										break;
									case GADID_RECSRC:
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_Input,icode,TAG_DONE);
										break;
									case GADID_RECGAIN:
										dbl=rec_lo+((double)icode*rec_st);
										sprintf(strBuffer,"%5.2lf",dbl);
										GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
										gain=(LONG)(dbl*(double)SMP_MAX);
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_InputGain,gain,TAG_DONE);
										scroll=FALSE;
										break;
									case GADID_MONSRC:
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_Output,icode,TAG_DONE);
										break;
									case GADID_MONGAIN:
										dbl=mon_lo+((double)icode*mon_st);
										sprintf(strBuffer,"%5.2lf",dbl);
										GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
										gain=(LONG)(dbl*(double)SMP_MAX);
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_MonitorVolume,gain,TAG_DONE);
										scroll=FALSE;
										break;
								}
							}
							if(iclass==IDCMP_GADGETDOWN) {
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_RECGAIN:
									case GADID_MONGAIN:
										scroll=TRUE;
										break;
								}
							}
							if(scroll && iclass==IDCMP_MOUSEMOVE) {
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_RECGAIN:
										dbl=rec_lo+((double)icode*rec_st);
										sprintf(strBuffer,"%5.2lf",dbl);
										GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
										gain=(LONG)(dbl*(double)SMP_MAX);
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_InputGain,gain,TAG_DONE);
										break;
									case GADID_MONGAIN:
										dbl=mon_lo+((double)icode*mon_st);
										sprintf(strBuffer,"%5.2lf",dbl);
										GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
										gain=(LONG)(dbl*(double)SMP_MAX);
										if(raudioctrl) AHI_ControlAudio(raudioctrl,AHIC_MonitorVolume,gain,TAG_DONE);
										break;
								}
							}
							if(iclass==IDCMP_VANILLAKEY) {
								switch(icode) {
									case KEY_ESC:
										quit=TRUE;break;
									case 'R':				/* Record */
									case 'r':
										if(!recording) {
											recording=TRUE;
											GT_SetGadgetAttrs(recGadgets[GADIX_RECORD],recwin,NULL,GA_Disabled,recording ,TAG_DONE);
											GT_SetGadgetAttrs(recGadgets[GADIX_STOP  ],recwin,NULL,GA_Disabled,!recording,TAG_DONE);
										}
										break;
									case 'S':				/* Stop */
									case 's':
										if(recording) {
											recording=FALSE;
											GT_SetGadgetAttrs(recGadgets[GADIX_RECORD],recwin,NULL,GA_Disabled,recording,TAG_DONE);
											GT_SetGadgetAttrs(recGadgets[GADIX_STOP  ],recwin,NULL,GA_Disabled,!recording,TAG_DONE);
										}
										break;
									case KEY_SPACE:			/* Record/Stop */
										recording=!recording;
										GT_SetGadgetAttrs(recGadgets[GADIX_RECORD],recwin,NULL,GA_Disabled,recording,TAG_DONE);
										GT_SetGadgetAttrs(recGadgets[GADIX_STOP  ],recwin,NULL,GA_Disabled,!recording,TAG_DONE);
										break;
								}
							}
							if(iclass==IDCMP_CLOSEWINDOW) {
								recording=FALSE;Delay(10);
								quit=TRUE;
							}
							if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
								ShowHelp("html/nodes/node01.05.09.html");
							if(iclass==IDCMP_INTUITICKS && raudioctrl) {
								if(recording) {
									sprintf(tmp,"%lf ms",(double)recsize/((double)optp_Ahi.recorate/1000.0));
									UC_ConvertParameter(rectime,tmp,UGRP_ABSTIME,ATME_H_M_S_MS);
									sprintf(strBuffer,"%ld samples = %s",recsize,rectime);
									GT_SetGadgetAttrs(recGadgets[GADIX_RECINFO],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
								}
								else {
									// Levelmeter löschen
									SetAPen(recrp,1);
									RectFill(recrp,xp1,yp1,xp2,yp1+yh2);					/* Level-L */
									RectFill(recrp,xp1,yp2,xp2,yp2+yh2);					/* Level-R */
									// Levelmeter malen und Zahlen updaten
									SetAPen(recrp,4);
									xp3=xp1+(UWORD)(ampenvl*(double)xh2);
									RectFill(recrp,xp1,yp1,xp3,yp1+yh2);
									if(ampenvl<OVERLOAD) fpen=1;
									else fpen=10;
									GT_SetGadgetAttrs(recGadgets[GADIX_LEVLLAB],recwin,NULL,GTTX_Text,ToYUnit(RunTime.sfxprefs_sbuf->unity,(LONG)(32767.0*ampenvl)),GTTX_CopyText,TRUE,GTTX_FrontPen,fpen,TAG_DONE);
									xp3=xp1+(UWORD)(ampenvr*(double)xh2);
									RectFill(recrp,xp1,yp2,xp3,yp2+yh2);
									if(ampenvr<OVERLOAD) fpen=1;
									else fpen=10;
									GT_SetGadgetAttrs(recGadgets[GADIX_LEVRLAB],recwin,NULL,GTTX_Text,ToYUnit(RunTime.sfxprefs_sbuf->unity,(LONG)(32767.0*ampenvr)),GTTX_CopyText,TRUE,GTTX_FrontPen,fpen,TAG_DONE);
									SetAPen(recrp,10);
									xp3=xp1+(UWORD)(maxenvl*(double)xh2);
									Move(recrp,xp3,yp1);Draw(recrp,xp3,yp1+yh2);
									if(maxenvl<OVERLOAD) fpen=1;
									else fpen=10;
									GT_SetGadgetAttrs(recGadgets[GADIX_MAXLLAB],recwin,NULL,GTTX_Text,ToYUnit(RunTime.sfxprefs_sbuf->unity,(LONG)(32767.0*maxenvl)),GTTX_CopyText,TRUE,GTTX_FrontPen,fpen,TAG_DONE);
									xp3=xp1+(UWORD)(maxenvr*(double)xh2);
									Move(recrp,xp3,yp2);Draw(recrp,xp3,yp2+yh2);
									if(maxenvr<OVERLOAD) fpen=1;
									else fpen=10;
									GT_SetGadgetAttrs(recGadgets[GADIX_MAXRLAB],recwin,NULL,GTTX_Text,ToYUnit(RunTime.sfxprefs_sbuf->unity,(LONG)(32767.0*maxenvr)),GTTX_CopyText,TRUE,GTTX_FrontPen,fpen,TAG_DONE);
									if((cmaxenv>=OVERLOAD) && autogain) {										/* automatically lower gain */
										cmaxenv=0.0;
										AHI_ControlAudio(raudioctrl,AHIC_InputGain_Query,&gain,TAG_DONE);			/* current gain */
										dbl=((double)gain/(double)SMP_MAX)-rec_st;
										lev=((ULONG)(SLIDER_RES*((dbl-rec_lo)/(rec_up-rec_lo))));						/* reduction */
										GT_SetGadgetAttrs(recGadgets[GADIX_RECGAIN   ],recwin,NULL,GTSL_Level,lev,TAG_DONE);
										sprintf(strBuffer,"%5.2lf",dbl);
										GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
										gain=(LONG)(dbl*(double)SMP_MAX);													/* set new gain */
										AHI_ControlAudio(raudioctrl,AHIC_InputGain,gain,TAG_DONE);
										maxenvl=maxenvr=0.0;														/* reset maxenvs */
									}
									else cmaxenv=0.0;
								}
							}
						}
					}
					CloseWindow(recwin);

					if(raudioctrl) {
						AHI_ControlAudio(raudioctrl,AHIC_Record,FALSE,TAG_DONE);
						AHI_FreeAudio(raudioctrl);raudioctrl=NULL;
					}
					if(RecordLabels) {
						dptr=RecordLabels;
						while(*dptr) { free(*dptr);dptr++; }
						free(RecordLabels);
					}
					if(MonitorLabels) {
						dptr=MonitorLabels;
						while(*dptr) { free(*dptr);dptr++; }
						free(MonitorLabels);
					}
					if(rd.worktask) {
						UBYTE aloader;

						MSG("Finishing worktask");
						rd.state=STATE_FINISH;
						Signal(rd.worktask,(1L<<rd.worksig));				// {M->S:01} start request
						Wait(1L<<rd.mainsig);								// {S->M:04} wait for task to say he's ready to shutdown
						MSG("... worktask finished");
						FreeSignal(rd.mainsig);rd.mainsig=-1;
						FreeVec(rd.buffer[0]);rd.buffer[0]=NULL;
						FreeVec(rd.buffer[1]);rd.buffer[1]=NULL;

						if(recsize>0) {										// only invoke loader if we recorded something
							APTR oldcfg=NULL,newcfg;

							MSG1("generating settings for loader : %ld",recfmt);
							if(!access("env:sfx/loaders/RAW_L.cfg",F_OK|R_OK)) {
								oldcfg=CfgIO_NewConfig("RAW_L","$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s");
								CfgIO_ReadConfig(oldcfg,"env:sfx/loaders/RAW_L.cfg");
							}

							newcfg=CfgIO_NewConfig("RAW_L","$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s");
							CfgIO_WriteString		  (newcfg,"General"	,"Endian"		,"Motorola");
							CfgIO_WriteUnsignedInteger(newcfg,"General"	,"Rate"			,optp_Ahi.recorate);
							CfgIO_WriteUnsignedInteger(newcfg,"General"	,"Offset"		,0);
							CfgIO_WriteBool			  (newcfg,"General"	,"CheckType"	,0);
							switch(recfmt) {
								case AHIST_M8S:
									MSG("  M8S");
									CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"mono");
									CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM8);
									CfgIO_WriteString		  (newcfg,"General"	,"Sign"			,"signed");
									break;
								case AHIST_S8S:
									MSG("  S8S");
									CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"st. interleav.");
									CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM8);
									CfgIO_WriteString		  (newcfg,"General"	,"Sign"			,"signed");
									break;
								case AHIST_M16S:
									MSG("  M16S");
									CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"mono");
									CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM16);
									CfgIO_WriteString		  (newcfg,"General"	,"Sign"			,"signed");
									break;
								case AHIST_S16S:
									MSG("  S16S");
									CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"st. interleav.");
									CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM16);
									CfgIO_WriteString		  (newcfg,"General"	,"Sign"			,"signed");
									break;
							}
//							switch(recbits) {
//								case 8:			CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM8);break;
//								case 16:		CfgIO_WriteString		  (newcfg,"General"	,"Format"		,STR_PCM16);break;
//							}
//							switch(recchannels) {
//								case CH_MONO:	CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"mono");break;
//								case CH_STEREO:	CfgIO_WriteString		  (newcfg,"General"	,"Channel"		,"st. interleav.");break;
//							}
							CfgIO_WriteConfig(newcfg,"env:sfx/loaders/RAW_L.cfg");

							MSG("invoking loader");
							aloader=RunTime.aloader;		// remember active loader
							RunTime.aloader=GetIndexByName(&LoaderList,"RAW");
							LoadSampleRexx(strFileName);
							MSG("...ready");
							RunTime.aloader=aloader;		// restore active loader
							if(oldcfg) {
								CfgIO_WriteConfig(oldcfg,"env:sfx/loaders/RAW_L.cfg");
								CfgIO_RemConfig(oldcfg);
							}
							else DeleteFile("env:sfx/loaders/RAW_L.cfg");
							if(newcfg) CfgIO_RemConfig(newcfg);
						}
						DeleteFile(strFileName);
					}
				}
				else Message(errOpenWindow,NULL,__FILE__,__LINE__);
				FreeGadgets(recGList);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		AHI_FreeAudioRequest(amreq);
	}
	else Message(errAllocateAudioRequest,NULL,__FILE__,__LINE__);
}

void Refr_RecordWin(struct Gadget *recGadgets[],struct Window *recwin)
{
	register ULONG i;
	UBYTE res,**dptr;
	char strBuffer[80];
	ULONG inputs,outputs,lev,port,bufsize,recchannels;
	LONG lo,up,gain;
	double dbl;
	struct TagItem audiotags[]={
		AHIA_AudioID,	0,
		AHIA_MixFreq,	0,
		AHIA_Channels,	0,
		AHIA_Sounds,	2,
		AHIA_RecordFunc,NULL,
		TAG_DONE
	};

	//-- free old resources (if allocated)

	if(raudioctrl) {
		AHI_ControlAudio(raudioctrl,AHIC_Record,FALSE,TAG_DONE);
		AHI_FreeAudio(raudioctrl);raudioctrl=NULL;
	}
	if(RecordLabels) {
//		MSG1("Freeing old RecordLabels : 0x%08lx",*RecordLabels);
		dptr=RecordLabels;
		while(*dptr) { free(*dptr);dptr++; }
		free(RecordLabels);
	}
	if(MonitorLabels) {
//		MSG1("Freeing old MonitorLabels : 0x%08lx",*MonitorLabels);
		dptr=MonitorLabels;
		while(*dptr) { free(*dptr);dptr++; }
		free(MonitorLabels);
	}

	if(rd.worktask) {
		rd.state=STATE_FINISH;
		Signal(rd.worktask,(1L<<rd.worksig));				// {M->S:01} start request
		Wait(1L<<rd.mainsig);								// {S->M:04} wait for task to say he's ready to shutdown
		FreeSignal(rd.mainsig);rd.mainsig=-1;
		FreeVec(rd.buffer[0]);rd.buffer[0]=NULL;
		FreeVec(rd.buffer[1]);rd.buffer[1]=NULL;
	}


	//-- allocate new resources

	if(!(res=AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_BufferLen,75,AHIDB_Name,strBuffer,TAG_DONE))) {
		sprintf(strBuffer,"Invalid Mode");
		GT_SetGadgetAttrs(recGadgets[GADIX_AUDIOMODE],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(recGadgets[GADIX_RECSRC],recwin,NULL,GTCY_Labels,EmptyLabels,GTCY_Active,0,TAG_DONE);
		GT_SetGadgetAttrs(recGadgets[GADIX_MONSRC],recwin,NULL,GTCY_Labels,EmptyLabels,GTCY_Active,0,TAG_DONE);
		rec_lo=rec_up=mon_lo=mon_up=1.0;
		rec_st=mon_st=0.0;
		GT_SetGadgetAttrs(recGadgets[GADIX_RECGAIN   ],recwin,NULL,GTSL_Level,0,TAG_DONE);
		sprintf(strBuffer,"%5.2lf",rec_lo);
		GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(recGadgets[GADIX_MONGAIN   ],recwin,NULL,GTSL_Level,0,TAG_DONE);
		sprintf(strBuffer,"%5.2lf",mon_lo);
		GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
	}
	else {
		AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_Stereo,&recchannels,TAG_DONE);
		MSG1("Record Stereo ? %2ld",recchannels);
		AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_Bits,&recbits,TAG_DONE);
		MSG1("Record Bits   ? %2ld",recbits);
		GT_SetGadgetAttrs(recGadgets[GADIX_AUDIOMODE],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,TAG_DONE);
		RecordHook.h_Entry=(ULONG(*)())RecordAhiDevInt;
		audiotags[0].ti_Data=optp_Ahi.recomode;
		audiotags[1].ti_Data=(ULONG)optp_Ahi.recorate;
		audiotags[2].ti_Data=(ULONG)recchannels+1;
		audiotags[4].ti_Data=(ULONG)(&RecordHook);
		if(raudioctrl=AHI_AllocAudioA(audiotags)) {
			AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_Inputs,&inputs,TAG_DONE);
			MSG1("Inputs  : %2ld",inputs);
			if(inputs) {
				dptr=RecordLabels=malloc((1+inputs)*sizeof(UBYTE *));
				for(i=0;i<inputs;i++) {
					AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_BufferLen,75,AHIDB_InputArg,i,AHIDB_Input,strBuffer,TAG_DONE);
					*dptr=strdup(strBuffer);
					dptr++;
				}
				*dptr=NULL;
				AHI_ControlAudio(raudioctrl,AHIC_Input_Query,&port,TAG_DONE);
				MSG1("  Act.  : %2ld",port);
				GT_SetGadgetAttrs(recGadgets[GADIX_RECSRC],recwin,NULL,GTCY_Labels,RecordLabels,GTCY_Active,port,GA_Disabled,FALSE,TAG_DONE);
				AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_MinInputGain,&lo,AHIDB_MaxInputGain,&up,TAG_DONE);
				if(up!=lo) {
					rec_lo=(double)lo/(double)SMP_MAX;
					rec_up=(double)up/(double)SMP_MAX;
					rec_st=(rec_up-rec_lo)/SLIDER_RES;
					AHI_ControlAudio(raudioctrl,AHIC_InputGain_Query,&gain,TAG_DONE);
					dbl=(double)gain/(double)SMP_MAX;
					MSG3("gain : 0x%08lx .. 0x%08lx .. 0x%08lx",lo,gain,up);
					MSG3("gain : %08.4lf .. %08.4lf .. %08.4lf",rec_lo,dbl,rec_up);
					lev=(ULONG)(SLIDER_RES*((dbl-rec_lo)/(rec_up-rec_lo)));
					GT_SetGadgetAttrs(recGadgets[GADIX_RECGAIN   ],recwin,NULL,GTSL_Level,lev,GA_Disabled,FALSE,TAG_DONE);
					sprintf(strBuffer,"%5.2lf",dbl);
					GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,GA_Disabled,FALSE,TAG_DONE);
					GT_SetGadgetAttrs(recGadgets[GADIX_AUTO      ],recwin,NULL,GA_Disabled,FALSE,TAG_DONE);
				}
				else {
					GT_SetGadgetAttrs(recGadgets[GADIX_RECGAIN   ],recwin,NULL,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(recGadgets[GADIX_AUTO      ],recwin,NULL,GA_Disabled,TRUE,TAG_DONE);
				}
			}
			else {
				GT_SetGadgetAttrs(recGadgets[GADIX_RECSRC    ],recwin,NULL,GTCY_Labels,RecordLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(recGadgets[GADIX_RECGAIN   ],recwin,NULL,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(recGadgets[GADIX_RECGAINLAB],recwin,NULL,GTTX_Text,0l,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(recGadgets[GADIX_AUTO      ],recwin,NULL,GA_Disabled,TRUE,TAG_DONE);
			}
			AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_Outputs,&outputs,TAG_DONE);
			MSG1("Outputs : %2ld",outputs);
			if(outputs) {
				dptr=MonitorLabels=malloc((1+outputs)*sizeof(UBYTE *));
				for(i=0;i<outputs;i++) {
					AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_BufferLen,75,AHIDB_OutputArg,i,AHIDB_Output,strBuffer,TAG_DONE);
					*dptr=strdup(strBuffer);
					dptr++;
				}
				*dptr=NULL;
				AHI_ControlAudio(raudioctrl,AHIC_Output_Query,&port,TAG_DONE);
				MSG1("  Act.  : %2ld",port);
				GT_SetGadgetAttrs(recGadgets[GADIX_MONSRC],recwin,NULL,GTCY_Labels,MonitorLabels,GTCY_Active,port,GA_Disabled,FALSE,TAG_DONE);
				AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_MinMonitorVolume,&lo,AHIDB_MaxMonitorVolume,&up,TAG_DONE);
				if(up!=lo) {
					mon_lo=(double)lo/(double)SMP_MAX;
					mon_up=(double)up/(double)SMP_MAX;
					mon_st=(mon_up-mon_lo)/SLIDER_RES;
					AHI_ControlAudio(raudioctrl,AHIC_MonitorVolume_Query,&gain,TAG_DONE);
					dbl=(double)gain/(double)SMP_MAX;
					MSG3("gain : 0x%08lx .. 0x%08lx .. 0x%08lx",lo,gain,up);
					MSG3("gain : %08.4lf .. %08.4lf .. %08.4lf",mon_lo,dbl,mon_up);
					lev=(ULONG)(SLIDER_RES*((dbl-mon_lo)/(rec_up-mon_lo)));
					GT_SetGadgetAttrs(recGadgets[GADIX_MONGAIN   ],recwin,NULL,GTSL_Level,lev,GA_Disabled,FALSE,TAG_DONE);
					sprintf(strBuffer,"%5.2lf",dbl);
					GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,strBuffer,GTTX_CopyText,TRUE,GA_Disabled,FALSE,TAG_DONE);
				}
				else {
					GT_SetGadgetAttrs(recGadgets[GADIX_MONGAIN   ],recwin,NULL,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,0l,GA_Disabled,TRUE,TAG_DONE);
				}
			}
			else {
				GT_SetGadgetAttrs(recGadgets[GADIX_MONSRC    ],recwin,NULL,GTCY_Labels,MonitorLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(recGadgets[GADIX_MONGAIN   ],recwin,NULL,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
				GT_SetGadgetAttrs(recGadgets[GADIX_MONGAINLAB],recwin,NULL,GTTX_Text,0l,GA_Disabled,TRUE,TAG_DONE);
			}
			ampenvl=ampenvr=maxenvl=maxenvr=cmaxenv=0.0;

			// new record task
			strmfp(strFileName,RunTime.sfxprefs_vmem->swap_path,"sfx_record");
			AHI_GetAudioAttrs(optp_Ahi.recomode,NULL,AHIDB_MaxRecordSamples,&bufsize,TAG_DONE);
			MSG1("recsmpl   : %4d",bufsize);

			recsize			=0;
			rd.a4			=__builtin_getreg(REG_A4);
			rd.a6			=__builtin_getreg(REG_A6);
			rd.maintask		=(struct Process *)FindTask(NULL);		// set up parameters for IO Tasks
			rd.mainsig		=AllocSignal(-1);
			rd.worktask		=NULL;
			rd.worksig		=-1;
			rd.state		=STATE_READY;
			rd.fn			=strFileName;
			rd.buffer[0]	=AllocVec((bufsize<<6),MEMF_PUBLIC);	// num_samples *4 (because stereo,16bit) *16 (because 16 frames)
			rd.buffer[1]	=AllocVec((bufsize<<6),MEMF_PUBLIC);
			rd.ptr			=rd.buffer[0];
			rd.bnum			=0;
			rd.step			=0;
			rd.blen[0]		=0;
			rd.blen[1]		=0;
			rd.fh			=NULL;
			ProcTags[0].ti_Data=(ULONG)RecordTask;

			MSG("lauching RecordTask");
			Forbid();
			if(ThatTask=CreateNewProc(ProcTags)) ThatTask->pr_Task.tc_UserData=&rd;		// create Task & pass parameters
			Permit();
			MSG("  done");
			
			Wait(1L<<rd.mainsig);														// wait for task to say hello
			if(rd.worksig==-1 || !rd.worktask) {										// is task okay ?
				MSG("  and failed");
			}
			else {
				MSG("  and okay");
				// enable recording
				AHI_ControlAudio(raudioctrl,AHIC_Record,TRUE,TAG_DONE);
			}
		}
	}
}

ULONG __asm __interrupt __saveds RecordAhiDevInt(register __a0 struct Hook *hook,register __a2 struct AHIAudioCtrl *actrl,register __a1 struct AHIRecordMessage *chan)
{
	recfmt=chan->ahirm_Type;
	switch(chan->ahirm_Type) {
		case AHIST_M8S:
			if(recording) {
				if(rd.worktask) {
					recsize+=chan->ahirm_Length;
					memcpy(rd.ptr,chan->ahirm_Buffer,chan->ahirm_Length);
					rd.ptr+=chan->ahirm_Length;
					rd.blen[rd.bnum]+=chan->ahirm_Length;
					rd.step++;
					if(rd.step==16) {									/* collect 16 frames before saving */
						rd.bnum=1-rd.bnum;
						rd.ptr=rd.buffer[rd.bnum];
						rd.blen[rd.bnum]=0;
						rd.step=0;
						Signal(rd.worktask,1L<<rd.worksig);
					}
				}
			}
			else {
				rbuffer8=chan->ahirm_Buffer;
				for(bloop=0;bloop<chan->ahirm_Length;bloop++) {
					aktval=((double)abs(*rbuffer8))/128.0;
					if(aktval>ampenvl) ampenvl=aktval;
					else ampenvl=efc1*ampenvl+efc2*aktval;
					ampenvr=ampenvl;
					if(aktval>maxenvl) maxenvl=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer8++;
				}
			}
			break;
		case AHIST_S8S:
			if(recording) {
				if(rd.worktask) {
					recsize+=chan->ahirm_Length;
					memcpy(rd.ptr,chan->ahirm_Buffer,(chan->ahirm_Length<<1));
					rd.ptr+=(chan->ahirm_Length<<1);
					rd.blen[rd.bnum]+=(chan->ahirm_Length<<1);
					rd.step++;
					if(rd.step==16) {									/* collect 16 frames before saving */
						rd.bnum=1-rd.bnum;
						rd.ptr=rd.buffer[rd.bnum];
						rd.blen[rd.bnum]=0;
						rd.step=0;
						Signal(rd.worktask,1L<<rd.worksig);
					}
				}
			}
			else {
				rbuffer8=chan->ahirm_Buffer;
				for(bloop=0;bloop<chan->ahirm_Length;bloop++) {
					aktval=((double)abs(*rbuffer8))/128.0;
					if(aktval>ampenvl) ampenvl=aktval;
					else ampenvl=efc1*ampenvl+efc2*aktval;
					if(aktval>maxenvl) maxenvl=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer8++;
					aktval=((double)abs(*rbuffer8))/128.0;
					if(aktval>ampenvr) ampenvr=aktval;
					else ampenvr=efc1*ampenvr+efc2*aktval;
					if(aktval>maxenvr) maxenvr=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer8++;
				}
			}
			break;
		case AHIST_M16S:
			if(recording) {
				if(rd.worktask) {
					recsize+=chan->ahirm_Length;
					memcpy(rd.ptr,chan->ahirm_Buffer,(chan->ahirm_Length<<1));
					rd.ptr+=(chan->ahirm_Length<<1);
					rd.blen[rd.bnum]+=(chan->ahirm_Length<<1);
					rd.step++;
					if(rd.step==16) {									/* collect 16 frames before saving */
						rd.bnum=1-rd.bnum;
						rd.ptr=rd.buffer[rd.bnum];
						rd.blen[rd.bnum]=0;
						rd.step=0;
						Signal(rd.worktask,1L<<rd.worksig);
					}
				}
			}
			else {
				rbuffer16=chan->ahirm_Buffer;
				for(bloop=0;bloop<chan->ahirm_Length;bloop++) {
					aktval=((double)abs(*rbuffer16))/(double)(SMP_MAX>>1);
					if(aktval>ampenvl) ampenvl=aktval;
					else ampenvl=efc1*ampenvl+efc2*aktval;
					ampenvr=ampenvl;
					if(aktval>maxenvl) maxenvl=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer16++;
				}
			}
			break;
		case AHIST_S16S:
			if(recording) {
				if(rd.worktask) {
					recsize+=chan->ahirm_Length;
					memcpy(rd.ptr,chan->ahirm_Buffer,(chan->ahirm_Length<<2));
					rd.ptr+=(chan->ahirm_Length<<2);
					rd.blen[rd.bnum]+=(chan->ahirm_Length<<2);
					rd.step++;
					if(rd.step==16) {									/* collect 16 frames before saving */
						rd.bnum=1-rd.bnum;
						rd.ptr=rd.buffer[rd.bnum];
						rd.blen[rd.bnum]=0;
						rd.step=0;
						Signal(rd.worktask,1L<<rd.worksig);
					}
				}
			}
			else {
				rbuffer16=chan->ahirm_Buffer;
				for(bloop=0;bloop<chan->ahirm_Length;bloop++) {
					aktval=((double)abs(*rbuffer16))/(double)(SMP_MAX>>1);
					if(aktval>ampenvl) ampenvl=aktval;
					else ampenvl=efc1*ampenvl+efc2*aktval;
					if(aktval>maxenvl) maxenvl=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer16++;
					aktval=((double)abs(*rbuffer16))/(double)(SMP_MAX>>1);
					if(aktval>ampenvr) ampenvr=aktval;
					else ampenvr=efc1*ampenvr+efc2*aktval;
					if(aktval>maxenvr) maxenvr=aktval;
					if(aktval>cmaxenv) cmaxenv=aktval;
					rbuffer16++;
				}
			}
			break;
	}
	return(NULL);
}

void __saveds __asm RecordTask(void)
{
	struct Process *ThisTask;
	struct RecordData *rd;
	UBYTE fail,running;

	ThisTask=(struct Process *)FindTask(NULL);
	if(rd=ThisTask->pr_Task.tc_UserData) {
		__builtin_putreg(REG_A4,rd->a4);
		__builtin_putreg(REG_A6,rd->a6);

		fail=FALSE;running=TRUE;
		rd->worktask=ThisTask;							// maintask will check those fields to see if we're okay
		if((rd->worksig=AllocSignal(-1))==-1) fail=TRUE;
		if((rd->fh=Open(rd->fn,MODE_NEWFILE))==-1) fail=TRUE;
		MSG1("WT : filename %s",rd->fn);
		Signal(rd->maintask,(1L<<rd->mainsig));			// say hello
		if(!fail) {
			MSG("WT : running");
			while(running) {
				Wait(1L<<rd->worksig);					// wait for request
//				MSG("WT : signal");
				if(rd->state==STATE_READY) Write(rd->fh,rd->buffer[1-rd->bnum],rd->blen[1-rd->bnum]);	// write block
				else running=FALSE;
/*				Forbid();
				if(rd->state==STATE_FINISH) { running=FALSE;Permit(); }		// we're ready
				else {													// we've got to work
					Permit();
					Write(rd->fh,rd->buffer,rd->blen);	// write block
				}
*/
			}
			
		}
		MSG("WT : finishing");
		rd->worktask=NULL;
		if(rd->worksig!=-1) { FreeSignal(rd->worksig);rd->worksig=-1; }
		MSG("WT : before close()");
		if(rd->fh) {
			while(!Close(rd->fh)) {
				MSG2("WT : file closing error : %ld %s\n",errno,strerror(errno));
				Delay(10);
			}
			rd->fh=NULL;
		}
		MSG("WT : after close()");
		if(!fail) Signal(rd->maintask,(1L<<rd->mainsig));			// {S->M:04} tell main that we're ready
	}
	else Signal(rd->maintask,(1L<<rd->mainsig));
}
//-- eof ----------------------------------------------------------------------
