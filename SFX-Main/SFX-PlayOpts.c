/****h* SoundFX/SFX-PlayOpts.c [4.2] *
*
*  NAME
*    SFX-PlayOpts.c
*  COPYRIGHT
*    $VER: SFX-PlayOpts.c 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    options windows for player modules - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    20.Aug.1994
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    07.Jun.2000	V 4.00	most recent version
*    13.Aug.1998	V 3.70	most recent version
*    20.Jun.1994	V 3.00	initial version
*  NOTES
*
*******
*/

#define SFX_PlayOpts_C

/* Includes und Defines */

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void OPT_Player(void);

void OptP_AHI(void);
void OptP_Amiga8bit(void);
void OptP_Amiga14bit(void);
void OptP_Amiga14cbit(void);

//-- definitions --------------------------------------------------------------

void OPT_Player(void)
{
	switch(RunTime.aplayer) {
		case 0: OptP_AHI();break;
		case 1: OptP_Amiga14bit();break;
		case 2: OptP_Amiga14cbit();break;
		case 3: OptP_Amiga8bit();break;
	}
}

void OptP_AHI(void)
{
	struct NewGadget ng;
	struct Window *smwin;
	struct Gadget *g;
	struct Gadget *fxGList=NULL;
	struct Gadget *fxGadgets[1];
	struct RastPort *fxrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0,res;
	char smname[80],title[80];
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime.scy[14];
	UWORD winh=ys2+2+RunTime.scy[14];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[286];
	UWORD xh;

	extern struct Library    *AHIBase;
	extern struct MsgPort    *AHImp;
	extern struct AHIRequest *AHIio;
	extern BYTE   AHIDevice;
	
	struct AHIAudioModeRequester *amreq;
	struct TagItem AMReqFiltertags[]={
		AHIDB_Realtime, TRUE,		// Remove the FILESAVE modes (among others?)
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

	AMReqtags[0].ti_Data=(ULONG)GetString(strSelectAudioMd);
	AMReqtags[2].ti_Data=(sfxprefs_gui.xres-300)>>1;
	AMReqtags[5].ti_Data=sfxprefs_gui.yres-30;      
	AMReqtags[7].ti_Data=(ULONG)AMReqFiltertags;
	if(amreq=AHI_AllocAudioRequestA(AMReqtags)) {
		if(!(res=AHI_GetAudioAttrs(optp_Ahi.playmode,NULL,AHIDB_BufferLen,75,AHIDB_Name,smname,TAG_DONE))) sprintf(smname,"Invalid Mode");

		if(g=CreateContext(&fxGList)) {
			ng.ng_LeftEdge		=xs1+RunTime.scx[58];
			ng.ng_TopEdge		=ys2+RunTime.scy[2];
			ng.ng_Width			=RunTime.scx[80];
			ng.ng_Height		=RunTime.scy[11];
			ng.ng_GadgetText	="_Okay";
			ng.ng_TextAttr		=RunTime.ScrFont;
			ng.ng_GadgetID		=0;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=RunTime.VisualInfo;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			ng.ng_LeftEdge		=xs1+RunTime.scx[148];
			ng.ng_GadgetText	="_Reset";
			ng.ng_GadgetID		=1;
			g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

			ng.ng_LeftEdge		=xs1+RunTime.scx[89];					/* Audiomode */
			ng.ng_TopEdge		=ys1+RunTime.scy[2];
			ng.ng_Width			=RunTime.scx[15];
			ng.ng_Height		=RunTime.scy[11];
			ng.ng_TextAttr		=RunTime.SigFont;
			ng.ng_GadgetText	=IMAGE_AudioReq;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_GadgetID		=2;
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

			ng.ng_LeftEdge		=xs1+RunTime.scx[105];
			ng.ng_Width			=RunTime.scx[175];
			ng.ng_TextAttr		=RunTime.ScrFont;
			ng.ng_GadgetText	="";
			ng.ng_GadgetID		=3;
			g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,smname,GTTX_CopyText,TRUE,TAG_DONE);
			fxGadgets[0]=g;

			if(g) {
				sprintf(title,GetString(strPlayerOptions),"AHI");
				MemUsage(sfxtitle);
				Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
				Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
				Dialog_Wintags_InnerWidth	=winw;
				Dialog_Wintags_InnerHeight	=winh;
				Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK;
				Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
				Dialog_Wintags_Gadgets		=(ULONG)fxGList;
				Dialog_Wintags_Title		=(ULONG)title;
				Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
				if(smwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
					GT_RefreshWindow(smwin,NULL);
					fxrp=smwin->RPort;SetFont(fxrp,RunTime.scrfont);

					SetAPen(fxrp,1);
					RText(fxrp,"AudioMode",xs1+RunTime.scx[81],ys1+RunTime.scy[9]);

					xh=RunTime.scx[286];
					DrawGadWB(fxrp,xs1,ys1,xh,RunTime.scy[14]);		/* Source */
					DrawGadWB(fxrp,xs1,ys2,xh,RunTime.scy[14]);		/* Go */

					while(!quit) {
						WaitPort(smwin->UserPort);
						while(imsg=GT_GetIMsg(smwin->UserPort)) {
							iclass	=imsg->Class;
							iadr	=imsg->IAddress;
							icode	=imsg->Code;
							GT_ReplyIMsg(imsg);
							if(iclass==IDCMP_GADGETUP) {
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case 0:			/* Okay */
										quit=1;break;
									case 1:			/* Reset */
										SetDefPOptions(3);
										if(!(res=AHI_GetAudioAttrs(optp_Ahi.playmode,NULL,AHIDB_BufferLen,75,AHIDB_Name,smname,TAG_DONE))) sprintf(smname,"Invalid Mode");
										GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTTX_Text,smname,GTTX_CopyText,TRUE,TAG_DONE);
										break;
									case 2:
										if(res=AHI_AudioRequest(amreq,
																AHIR_TitleText,GetString(strSelectAudioMd),
																AHIR_InitialAudioID,optp_Ahi.playmode,
																AHIR_InitialMixFreq,optp_Ahi.playrate,
																TAG_DONE)) {
											optp_Ahi.playmode=amreq->ahiam_AudioID;
											optp_Ahi.playrate=amreq->ahiam_MixFreq;
											if(!(res=AHI_GetAudioAttrs(optp_Ahi.playmode,NULL,AHIDB_BufferLen,75,AHIDB_Name,smname,TAG_DONE))) sprintf(smname,"Invalid Mode");
											GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTTX_Text,smname,GTTX_CopyText,TRUE,TAG_DONE);

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
								}
							}
							if(iclass==IDCMP_VANILLAKEY) {
								switch(icode) {
									case 'O':			/* Okay */
									case 'o':
										quit=1;break;
									case 'R':			/* Reset */
									case 'r':
										SetDefPOptions(3);
										if(!(res=AHI_GetAudioAttrs(optp_Ahi.playmode,NULL,AHIDB_BufferLen,75,AHIDB_Name,smname,TAG_DONE))) sprintf(smname,"Invalid Mode");
										GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTTX_Text,smname,GTTX_CopyText,TRUE,TAG_DONE);
										break;
								}
							}
							if(iclass==IDCMP_CLOSEWINDOW) quit=2;
							if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)	ShowHelp("html/nodes/"HELPBASE_PL".Ahi.html");
						}
					}
					CloseWindow(smwin);
				}
				else Message(errOpenWindow,NULL,__FILE__,__LINE__);
				FreeGadgets(fxGList);
			}
			else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateContext,NULL,__FILE__,__LINE__);
		AHI_FreeAudioRequest(amreq);
	}
	else Message(errAllocateAudioRequest,NULL,__FILE__,__LINE__);
}

void OptP_Amiga8bit(void)
{
	struct NewGadget ng;
	struct Window *smwin;
	struct Gadget *g;
	struct Gadget *fxGList;
	struct Gadget *fxGadgets[2];
	struct RastPort *fxrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0;
	char title[80];
	char *HFilterLabels[]={"On","Off",NULL};
	char *RatClipLabels[]={"28867","57734","None",NULL};
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime.scy[26];
	UWORD winh=ys2+2+RunTime.scy[14];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[286];
	UWORD xh;
	
	if(g=CreateContext(&fxGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[58];
		ng.ng_TopEdge		=ys2+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=0;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[148];
		ng.ng_GadgetText	="_Reset";
		ng.ng_GadgetID		=1;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[74];
		ng.ng_TopEdge		=ys1+RunTime.scy[2];
		ng.ng_GadgetText	="HFilter";
		ng.ng_GadgetID		=2;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,HFilterLabels,GTCY_Active,optp_Amiga_8bit.hfilter,TAG_DONE);
		fxGadgets[0]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[14];
		ng.ng_GadgetText	="RateClip";
		ng.ng_GadgetID		=3;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,RatClipLabels,GTCY_Active,optp_Amiga_8bit.rateclip,TAG_DONE);
		fxGadgets[1]=g;

		if(g) {
			sprintf(title,GetString(strPlayerOptions),"Amiga-8bit");
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)fxGList;
			Dialog_Wintags_Title		=(ULONG)title;
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(smwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(smwin,NULL);
				fxrp=smwin->RPort;SetFont(fxrp,RunTime.scrfont);

				xh=RunTime.scx[286];
				DrawGadWB(fxrp,xs1,ys1,xh,RunTime.scy[26]);		/* Source */
				DrawGadWB(fxrp,xs1,ys2,xh,RunTime.scy[14]);		/* Go */

				while(!quit)
				{
					WaitPort(smwin->UserPort);
					while(imsg=GT_GetIMsg(smwin->UserPort))
					{
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP)
						{
							switch((int)(((struct Gadget *)iadr)->GadgetID))
							{
								case 0:			/* Okay */
									quit=1;break;
								case 1:			/* Reset */
									SetDefPOptions(0);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_8bit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_8bit.rateclip,TAG_DONE);
									break;
								case 2:
									optp_Amiga_8bit.hfilter=icode;
									break;
								case 3:
									optp_Amiga_8bit.rateclip=icode;
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY)
						{
							switch(icode)
							{
								case 'O':			/* Okay */
								case 'o':
									quit=1;break;
								case 'R':			/* Reset */
								case 'r':
									SetDefPOptions(0);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_8bit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_8bit.rateclip,TAG_DONE);
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) quit=2;
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)	ShowHelp("html/nodes/"HELPBASE_PL".Amiga-8bit.html");
					}
				}
				CloseWindow(smwin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(fxGList);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
}

void OptP_Amiga14bit(void)
{
	struct NewGadget ng;
	struct Window *smwin;
	struct Gadget *g;
	struct Gadget *fxGList=NULL;
	struct Gadget *fxGadgets[2];
	struct RastPort *fxrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0;
	char title[80];
	char *HFilterLabels[]={"On","Off",NULL};
	char *RatClipLabels[]={"28867","57734","None",NULL};
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime.scy[26];
	UWORD winh=ys2+2+RunTime.scy[14];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[286];
	UWORD xh;

	if(g=CreateContext(&fxGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[58];
		ng.ng_TopEdge		=ys2+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=0;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[148];
		ng.ng_GadgetText	="_Reset";
		ng.ng_GadgetID		=1;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[74];
		ng.ng_TopEdge		=ys1+RunTime.scy[2];
		ng.ng_GadgetText	="HFilter";
		ng.ng_GadgetID		=2;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,HFilterLabels,GTCY_Active,optp_Amiga_14bit.hfilter,TAG_DONE);
		fxGadgets[0]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[14];
		ng.ng_GadgetText	="RateClip";
		ng.ng_GadgetID		=3;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,RatClipLabels,GTCY_Active,optp_Amiga_14bit.rateclip,TAG_DONE);
		fxGadgets[1]=g;

		if(g) {
			sprintf(title,GetString(strPlayerOptions),"Amiga-14bit");
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)fxGList;
			Dialog_Wintags_Title		=(ULONG)title;
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(smwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(smwin,NULL);
				fxrp=smwin->RPort;SetFont(fxrp,RunTime.scrfont);

				xh=RunTime.scx[286];
				DrawGadWB(fxrp,xs1,ys1,xh,RunTime.scy[26]);		/* Source */
				DrawGadWB(fxrp,xs1,ys2,xh,RunTime.scy[14]);		/* Go */

				while(!quit)
				{
					WaitPort(smwin->UserPort);
					while(imsg=GT_GetIMsg(smwin->UserPort))
					{
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP)
						{
							switch((int)(((struct Gadget *)iadr)->GadgetID))
							{
								case 0:			/* Okay */
									quit=1;break;
								case 1:			/* Reset */
									SetDefPOptions(1);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_14bit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_14bit.rateclip,TAG_DONE);
									break;
								case 2:
									optp_Amiga_14bit.hfilter=icode;
									break;
								case 3:
									optp_Amiga_14bit.rateclip=icode;
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY)
						{
							switch(icode)
							{
								case 'O':			/* Okay */
								case 'o':
									quit=1;break;
								case 'R':			/* Reset */
								case 'r':
									SetDefPOptions(1);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_14bit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_14bit.rateclip,TAG_DONE);
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) quit=2;
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)	ShowHelp("html/nodes/"HELPBASE_PL".Amiga-14bit.html");
					}
				}
				CloseWindow(smwin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(fxGList);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
}

void OptP_Amiga14cbit(void)
{
	struct NewGadget ng;
	struct Window *smwin;
	struct Gadget *g;
	struct Gadget *fxGList=NULL;
	struct Gadget *fxGadgets[2];
	struct RastPort *fxrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE quit=0;
	char title[80];
	char *HFilterLabels[]={"On","Off",NULL};
	char *RatClipLabels[]={"28867","57734","None",NULL};
	UWORD ys1=2;
	UWORD ys2=ys1+1+RunTime.scy[26];
	UWORD winh=ys2+2+RunTime.scy[14];
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[286];
	UWORD xh;

	if(g=CreateContext(&fxGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[58];
		ng.ng_TopEdge		=ys2+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=0;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[148];
		ng.ng_GadgetText	="_Reset";
		ng.ng_GadgetID		=1;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[74];
		ng.ng_TopEdge		=ys1+RunTime.scy[2];
		ng.ng_GadgetText	="HFilter";
		ng.ng_GadgetID		=2;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,HFilterLabels,GTCY_Active,optp_Amiga_14cbit.hfilter,TAG_DONE);
		fxGadgets[0]=g;

		ng.ng_TopEdge		=ys1+RunTime.scy[14];
		ng.ng_GadgetText	="RateClip";
		ng.ng_GadgetID		=3;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,RatClipLabels,GTCY_Active,optp_Amiga_14cbit.rateclip,TAG_DONE);
		fxGadgets[1]=g;

		if(g) {
			sprintf(title,GetString(strPlayerOptions),"Amiga-14cbit");
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)fxGList;
			Dialog_Wintags_Title		=(ULONG)title;
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(smwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(smwin,NULL);
				fxrp=smwin->RPort;SetFont(fxrp,RunTime.scrfont);

				xh=RunTime.scx[286];
				DrawGadWB(fxrp,xs1,ys1,xh,RunTime.scy[26]);		/* Source */
				DrawGadWB(fxrp,xs1,ys2,xh,RunTime.scy[14]);		/* Go */

				while(!quit)
				{
					WaitPort(smwin->UserPort);
					while(imsg=GT_GetIMsg(smwin->UserPort))
					{
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP)
						{
							switch((int)(((struct Gadget *)iadr)->GadgetID))
							{
								case 0:			/* Okay */
									quit=1;break;
								case 1:			/* Reset */
									SetDefPOptions(2);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_14cbit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_14cbit.rateclip,TAG_DONE);
									break;
								case 2:
									optp_Amiga_14cbit.hfilter=icode;
									break;
								case 3:
									optp_Amiga_14cbit.rateclip=icode;
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY)
						{
							switch(icode)
							{
								case 'O':			/* Okay */
								case 'o':
									quit=1;break;
								case 'R':			/* Reset */
								case 'r':
									SetDefPOptions(2);
									GT_SetGadgetAttrs(fxGadgets[0],smwin,NULL,GTCY_Active,optp_Amiga_14cbit.hfilter,TAG_DONE);
									GT_SetGadgetAttrs(fxGadgets[1],smwin,NULL,GTCY_Active,optp_Amiga_14cbit.rateclip,TAG_DONE);
									break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) quit=2;
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)	ShowHelp("html/nodes/"HELPBASE_PL".Amiga-14cbit.html");
					}
				}
				CloseWindow(smwin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(fxGList);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
}

//-- eof ----------------------------------------------------------------------
