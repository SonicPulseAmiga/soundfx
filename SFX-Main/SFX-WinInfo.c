/****h* SoundFX/SFX-WinInfo.c [4.3] *
*
*  NAME
*    SFX-WinInfo.c
*  COPYRIGHT
*    $VER: SFX-WinInfo.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    about window + project infos - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    06.Jun.2001	V 4.2	most recent version
*    24.Mar.2001	V 4.1	most recent version
*    22.Jul.2000	V 4.00	most recent version
*							* now uses table-lv hook
*    02.Mar.1999	V 3.80	most recent version
*    13.Aug.1998	V 3.70	most recent version
*    16.Jul.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_WinInfo_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void InfoWin(void);

//-- defines ------------------------------------------------------------------

#define GADID_SMPLIST		0
#define GADIX_SMPLIST		0

#define GADID_COPY			(GADID_SMPLIST+1)
#define GADIX_COPY			(GADIX_SMPLIST+1)

#define GADID_AUTH			(GADID_COPY+1)
#define GADIX_AUTH			(GADIX_COPY+1)

#define GADID_ANNO			(GADID_AUTH+1)
#define GADIX_ANNO			(GADIX_AUTH+1)

#define GADID_REGNR			(GADID_ANNO+1)
#define GADIX_REGNR			(GADIX_ANNO+1)

#define GADID_REGNAME		(GADID_REGNR+1)
#define GADIX_REGNAME		(GADIX_REGNR+1)

#define GADID_OKAY			(GADID_REGNAME+1)

#define LAST_GADIX			(GADIX_REGNAME+1)

//-- definitions --------------------------------------------------------------

void InfoWin(void)
{
	register ULONG i;
	struct NewGadget ng;
	struct Window *siwin;
	struct Gadget *g;
	struct Gadget *inGList=NULL;
	struct Gadget *inGadgets[LAST_GADIX];
	struct RastPort *inrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	UBYTE quit=0;
	struct Node *node;
	SInfo *entry;
	char infostr[255];
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(2);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(3)+RunTime.scy[63];
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(2);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+RunTime.scx[354];
	UWORD xh;
	UBYTE scrfyoff=RunTime.scrfont->tf_YSize;
	struct List sampleList;
	struct ExtNode {
		struct Node node;
		SInfo *si;
	};
	APTR sampleNodes=NULL;
	struct ExtNode *sampleNode;
	ULONG anzsamples,aktsample=0;

	NewList(&sampleList);
	if(RunTime.aktsamples) {
		anzsamples=GetNumberOfNodes(&BufferList);
		if(sampleNodes=(struct ExtNode **)AllocVec(sizeof(struct ExtNode)*anzsamples,MEMF_ANY|MEMF_CLEAR)) {
			SInfo *si;
			sampleNode=(struct ExtNode *)sampleNodes;i=0;
			foreach(node,&BufferList) {
				si=(SInfo *)node;
				sprintf(infostr,"%-20s \t22 %1d \t26 %8s \t37 %6d",node->ln_Name,(si->channels+1),ToXUnit(si->unitx,si->slen,si->srat),si->srat);
				sampleNode->node.ln_Name=StringCopy(infostr);
				sampleNode->si=si;
				AddTail(&sampleList,sampleNode);
				if(((SInfo *)node)==RunTime.aktbuf) aktsample=i;
//				MSG2("%2d  0x%08lx",i,node);
				i++;sampleNode++;
			}
//			MSG1("%2d",aktsample);
		}
	}
	if(g=CreateContext(&inGList)) {
		ng.ng_LeftEdge		=xs1+RunTime.scx[137];
		ng.ng_TopEdge		=ys4+RunTime.scy[2];
		ng.ng_Width			=RunTime.scx[80];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="_Okay";
		ng.ng_TextAttr		=RunTime.ScrFontB;
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime.VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);

		ng.ng_LeftEdge		=xs1+RunTime.scx[8];
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+10];
		ng.ng_Width			=RunTime.scx[336];
		ng.ng_Height		=RunTime.scy[52];
		ng.ng_GadgetText	="";
		ng.ng_TextAttr		=RunTime.ScrFont;
		ng.ng_GadgetID		=GADID_SMPLIST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
			GTLV_Labels,&sampleList,
			GTLV_Selected,aktsample,
			GTLV_Top,aktsample,
			GTLV_ShowSelected,0,
			GTLV_CallBack,RunTime.tablelvhook,
			GTLV_MaxPen,7,
		TAG_DONE);
		inGadgets[GADIX_SMPLIST]=g;

		ng.ng_LeftEdge		=xs1+RunTime.scx[58];
		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+62];
		ng.ng_Width			=RunTime.scx[285];
		ng.ng_Height		=RunTime.scy[11];
		ng.ng_GadgetText	="(C)";
		ng.ng_GadgetID		=GADID_COPY;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		inGadgets[GADIX_COPY]=g;

		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+74];
		ng.ng_GadgetText	="Author";
		ng.ng_GadgetID		=GADID_AUTH;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		inGadgets[GADIX_AUTH]=g;

		ng.ng_TopEdge		=ys2+RunTime.scy[FRMCONTENT_YOFFSET+86];
		ng.ng_GadgetText	="Anno.";
		ng.ng_GadgetID		=GADID_ANNO;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		inGadgets[GADIX_ANNO]=g;

		ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET];
		ng.ng_GadgetText	="Reg.Nr.";
		ng.ng_GadgetID		=GADID_REGNR;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		inGadgets[GADIX_REGNR]=g;

		ng.ng_TopEdge		=ys3+RunTime.scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="User";
		ng.ng_GadgetID		=GADID_REGNAME;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,TAG_DONE);
		inGadgets[GADIX_REGNAME]=g;

		if(g) {
			MemUsage(sfxtitle);
			Dialog_Wintags_Left			=(RunTime.xres-winw)>>1;
			Dialog_Wintags_Top			=(RunTime.yres-winh)>>1;
			Dialog_Wintags_InnerWidth	=winw;
			Dialog_Wintags_InnerHeight	=winh;
			Dialog_Wintags_IDCMP		=BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW;
			Dialog_Wintags_Flags		=WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS;
			Dialog_Wintags_Gadgets		=(ULONG)inGList;
			Dialog_Wintags_Title		=(ULONG)GetString(menPrjInfo);
			Dialog_Wintags_ScreenTitle	=(ULONG)sfxtitle;
			if(siwin=OpenWindowTagList(NULL,Dialog_Wintags)) {
				GT_RefreshWindow(siwin,NULL);
				inrp=siwin->RPort;SetFont(inrp,RunTime.scrfont);

				SetAPen(inrp,1);xh=RunTime.scx[6];
				Move(inrp,xs1+2+RunTime.scx[9]      ,ys2+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(inrp,"Name"  ,4);
				Move(inrp,xs1+2+RunTime.scx[9]+22*xh,ys2+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(inrp,"Ch."   ,3);
				Move(inrp,xs1+2+RunTime.scx[9]+26*xh,ys2+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(inrp,"Length",6);
				Move(inrp,xs1+2+RunTime.scx[9]+37*xh,ys2+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET]);Text(inrp,"Rate"  ,4);

				xh=RunTime.scx[354];
				SetAPen(inrp,1);
				sprintf(infostr,"SoundFX %d.%d",PRJ_VERSION,PRJ_REVISION);
				CText(inrp,xs1,xs1+xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET],infostr);

				sprintf(infostr,"(c) 1993-2004 by Stefan Kost");
				CText(inrp,xs1,xs1+xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET+ 6],infostr);

				sprintf(infostr,VERS_BITS"/"VERS_PREC" version compiled on "__DATE__" for "_CPUNAME"/"_FPUNAME);
				CText(inrp,xs1,xs1+xh,ys1+scrfyoff+RunTime.scy[FRMCONTENT_YOFFSET+15],infostr);

				if(RunTime.status) sprintf(infostr,"%s",Key.line0);				// keynumber
				else sprintf(infostr,"-DEMO-");
//				else sprintf(infostr,"non upgradable version");
				GT_SetGadgetAttrs(inGadgets[GADIX_REGNR  ],siwin,NULL,GTTX_Text,infostr,GTTX_CopyText,TRUE,TAG_DONE);				// key number
				GT_SetGadgetAttrs(inGadgets[GADIX_REGNAME],siwin,NULL,GTTX_Text,RunTime.username,GTTX_CopyText,TRUE,TAG_DONE);	// user name
				if(RunTime.aktsamples) {
					entry=((struct ExtNode *)GetNodeByIndex(&sampleList,aktsample))->si;
					if(entry) {
						GT_SetGadgetAttrs(inGadgets[GADIX_COPY],siwin,NULL,GTTX_Text,(entry->cpyr?entry->cpyr:NULL),GTTX_CopyText,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(inGadgets[GADIX_AUTH],siwin,NULL,GTTX_Text,(entry->auth?entry->auth:NULL),GTTX_CopyText,TRUE,TAG_DONE);
						GT_SetGadgetAttrs(inGadgets[GADIX_ANNO],siwin,NULL,GTTX_Text,(entry->anno?entry->anno:NULL),GTTX_CopyText,TRUE,TAG_DONE);
					}
				}
				else GT_SetGadgetAttrs(inGadgets[GADIX_SMPLIST],siwin,NULL,GA_Disabled,TRUE,TAG_DONE);

				DrawTitle(inrp,"Copyright"					,xs1,ys1,xh);
				DrawTitle(inrp,"Project Information"		,xs1,ys2,xh);
				DrawTitle(inrp,"Registration Information"	,xs1,ys3,xh);

				DrawGadWB(inrp,xs1,ys1,xh,ANYFRM_HEIGHT(2));					/* Copyright */
				DrawGadWB(inrp,xs1,ys2,xh,ANYFRM_HEIGHT(3)+RunTime.scy[63]);	/* Liste */
				DrawGadWB(inrp,xs1,ys3,xh,ANYFRM_HEIGHT(2));					/* Registration */
				DrawGadWB(inrp,xs1,ys4,xh,CMDFRM_HEIGHT);						/* Okay */

				while(!quit) {
					WaitPort(siwin->UserPort);
					while(imsg=GT_GetIMsg(siwin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						if(iclass==IDCMP_GADGETUP) {
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_OKAY:
									quit=1;break;
								case GADID_SMPLIST:
									entry=((struct ExtNode *)GetNodeByIndex(&sampleList,icode))->si;
									MSG2("%2d  0x%08lx",icode,entry);
									GT_SetGadgetAttrs(inGadgets[GADIX_COPY],siwin,NULL,GTTX_Text,(entry->cpyr?entry->cpyr:NULL),GTTX_CopyText,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(inGadgets[GADIX_AUTH],siwin,NULL,GTTX_Text,(entry->auth?entry->auth:NULL),GTTX_CopyText,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(inGadgets[GADIX_ANNO],siwin,NULL,GTTX_Text,(entry->anno?entry->anno:NULL),GTTX_CopyText,TRUE,TAG_DONE);
									break;
							}
						}
						if(iclass==IDCMP_VANILLAKEY) {
							switch(icode) {
								case KEY_ESC:				/* Okay */
								case KEY_ENTER:
								case KEY_RETURN:
								case 'O':
								case 'o':
									quit=1;break;
							}
						}
						if(iclass==IDCMP_CLOSEWINDOW) quit=1;
						if(iclass==IDCMP_RAWKEY && icode==RKEY_HELP)
							ShowHelp("html/nodes/node01.05.02.html");
					}
				}
				CloseWindow(siwin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
			FreeGadgets(inGList);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	if(sampleNodes) {
		sampleNode=(struct ExtNode *)sampleNodes;
		for(i=0;i<anzsamples;i++,sampleNode++)
			if(sampleNode->node.ln_Name) FreeVec(sampleNode->node.ln_Name);
		FreeVec(sampleNodes);
	}
}

//-- eof ----------------------------------------------------------------------
