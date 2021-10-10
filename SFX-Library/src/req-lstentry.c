/****h* sfxsupport.library/req-lstentry.c [4.2] *
*
*  NAME
*    req-lstentry.c
*  COPYRIGHT
*    $VER: req-lstentry.c 4.2 (26.09.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    get list entry requester - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    26.Sep.2001	V 4.2	most recent version
*    27.Aug.2000	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define REQ_LSTENTRY_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

SInfo * 		ASM LIB_EXPORT(GetListEntry(void));
struct Node * 	ASM LIB_EXPORT(GetNodeInteractive(REG(a0,struct List *list),REG(d0,ULONG index)));

//-- defines ------------------------------------------------------------------

#define GADID_OKAY	0

#define GADID_LIST	(GADID_OKAY+1)
#define GADIX_LIST	0

//-- definitions --------------------------------------------------------------

SInfo * SAVEDS ASM LIB_EXPORT(GetListEntry(void)) {
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE akttest=0;
	SInfo *si=GetRunTime(aktbuf),*tsi;
	struct NewGadget ng;
	struct Window *gswin;
	struct Gadget *g;
	struct Gadget *gsGadgets[GADIX_LIST+1];
	struct Gadget *gsGList=0l;
	struct RastPort *gsrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	NotifyTask nt;
	LONG sigin,sigout;
	UBYTE akt=LIB_INTCALL(GetBufferNr(GetRunTime(aktbuf)->id));
	UBYTE quit=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[91]);
	UWORD winh=ys2+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[254]);
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};

	nt.task=FindTask(NULL);
	if((nt.sig=AllocSignal(-1))!=-1) {
		AddTail(&GetRunTime(gblist),&nt);

		if(g=CreateContext(&gsGList)) {
			ng.ng_LeftEdge		=xs1+GetRunTime(scx[87]);
			ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
			ng.ng_Width			=GetRunTime(scx[80]);
			ng.ng_Height		=GetRunTime(scy[11]);
			ng.ng_GadgetText	=LIB_INTCALL(GetString(strOkay));
			ng.ng_TextAttr		=GetRunTime(ScrFontB);
			ng.ng_GadgetID		=GADID_OKAY;
			ng.ng_Flags			=PLACETEXT_IN;
			ng.ng_VisualInfo	=GetRunTime(VisualInfo);
			g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

			ng.ng_LeftEdge		=xs1+GetRunTime(scx[8]);
			ng.ng_TopEdge		=ys1+GetRunTime(scy[2]);
			ng.ng_Width			=GetRunTime(scx[238]);
			ng.ng_Height		=GetRunTime(scy[88]);
			ng.ng_GadgetText	="";
			ng.ng_TextAttr		=GetRunTime(ScrFont);
			ng.ng_GadgetID		=GADID_LIST;
			ng.ng_Flags			=PLACETEXT_LEFT;
			g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,GetRunTime(blist),GTLV_Selected,akt,GTLV_Top,akt,GTLV_ShowSelected,0l,TAG_DONE);
			gsGadgets[GADIX_LIST]=g;

			if(g) {
				wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
				wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
				wintags[2].ti_Data=winw;
				wintags[3].ti_Data=winh;
				wintags[6].ti_Data=(ULONG)gsGList;
				wintags[7].ti_Data=(ULONG)LIB_INTCALL(GetString(strSelectBuffer));
				if(gswin=OpenWindowTagList(0l,wintags)) {
					GT_RefreshWindow(gswin,0l);
					gsrp=gswin->RPort;SetFont(gsrp,GetRunTime(scrfont));

					xh=GetRunTime(scx[254]);
					LIB_INTCALL(DrawGadWB(gsrp,xs1,ys1,xh,GetRunTime(scy[91])));					/* Par */
					LIB_INTCALL(DrawGadWB(gsrp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)])));		/* Okay */

					sigin=(1L<<gswin->UserPort->mp_SigBit)|(1L<<nt.sig);

					while(!quit) {
						sigout=Wait(sigin);
						if(sigout&(1L<<nt.sig)) {
							akt=LIB_INTCALL(GetBufferNr(GetRunTime(aktbuf)->id));
							si=GetRunTime(aktbuf);
							GT_SetGadgetAttrs(gsGadgets[0],gswin,0l,GTLV_Labels,GetRunTime(blist),GTLV_Selected,akt,GTLV_Top,akt,GTLV_ShowSelected,0l,TAG_DONE);
						}
						if(sigout&(1L<<gswin->UserPort->mp_SigBit)) {
							while(imsg=GT_GetIMsg(gswin->UserPort)) {
								iclass	=imsg->Class;
								iadr	=imsg->IAddress;
								icode	=imsg->Code;
								GT_ReplyIMsg(imsg);
								switch(iclass) {
									case IDCMP_GADGETUP:
										switch((int)(((struct Gadget *)iadr)->GadgetID)) {
											case GADID_OKAY:
												quit=1;break;
											case GADID_LIST:
												tsi=LIB_INTCALL(GetBufferPtr(0,icode));
												if(!akttest) { CurrentTime(&css,&csm);akttest=1; }
												else {
													CurrentTime(&ccs,&ccm);
													if(DoubleClick(css,csm,ccs,ccm) && si==tsi) {
														quit=1;
														css=0;csm=0;ccs=500;ccm=500;akttest=0;
													}
													else { css=ccs;csm=ccm;ccs=500;ccm=500; }
												}
												si=tsi;
												break;
										}
										break;
									case IDCMP_VANILLAKEY:
										switch(icode) {
											case KEY_RETURN:
											case KEY_ENTER:
												quit=1;break;
											case KEY_ESC:
												quit=2;break;
										}
										break;
									case IDCMP_RAWKEY:
										if(icode==RKEY_HELP)
											LIB_INTCALL(ShowHelp("html/nodes/node01.05.08.html"));
										break;
									case IDCMP_CLOSEWINDOW:
										quit=2;
										break;
								}
							}
						}
					}
					CloseWindow(gswin);
				}
				else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
				FreeGadgets(gsGList);
			}
			else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
		}
		else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));
		if(((struct Node *)&nt)==GetRunTime(gblist).lh_Head) RemHead(&GetRunTime(gblist));
		else if(((struct Node *)&nt)==GetRunTime(gblist).lh_Tail) RemTail(&GetRunTime(gblist));
		else Remove((struct Node *)&nt);
		FreeSignal(nt.sig);
	}
	else LIB_INTCALL(Message(errAllocateSignal,NULL,__FILE__,__LINE__));

	return((quit==1) ? si : 0);
}

struct Node * SAVEDS ASM LIB_EXPORT(GetNodeInteractive(REG(a0,struct List *list),REG(d0,ULONG index))) {
	ULONG css=0,csm=0,ccs=500,ccm=500;
	UBYTE akttest=0;
	struct NewGadget ng;
	struct Window *gswin;
	struct Gadget *g;
	struct Gadget *gsGList=0l;
	struct RastPort *gsrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	UWORD icode;
	ULONG nindex;
	UBYTE quit=0;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[91]);
	UWORD winh=ys2+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[254]);
	UWORD xh;
	struct TagItem wintags[]={
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_RMBTRAP|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			0l,
		WA_ScreenTitle,		(ULONG)VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(list,__FUNC__,"1.par",4));

	if(g=CreateContext(&gsGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[87]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[80]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	=LIB_INTCALL(GetString(strOkay));
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_OKAY;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[8]);
		ng.ng_TopEdge		=ys1+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[238]);
		ng.ng_Height		=GetRunTime(scy[88]);
		ng.ng_GadgetText	="";
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_LIST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,list,GTLV_Selected,index,GTLV_Top,index,GTLV_ShowSelected,0l,TAG_DONE);

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)gsGList;
			wintags[7].ti_Data=(ULONG)LIB_INTCALL(GetString(strSelectEntry));
			if(gswin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(gswin,0l);
				gsrp=gswin->RPort;SetFont(gsrp,GetRunTime(scrfont));

				xh=GetRunTime(scx[254]);
				LIB_INTCALL(DrawGadWB(gsrp,xs1,ys1,xh,GetRunTime(scy[91])));						/* Par */
				LIB_INTCALL(DrawGadWB(gsrp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)])));		/* Okay */

				while(!quit) {
					WaitPort(gswin->UserPort);
					while(imsg=GT_GetIMsg(gswin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_GADGETUP:
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_OKAY:
										quit=1;break;
									case GADID_LIST:
										nindex=icode;
										if(!akttest) { CurrentTime(&css,&csm);akttest=1; }
										else {
											CurrentTime(&ccs,&ccm);
											if(DoubleClick(css,csm,ccs,ccm) && index==nindex) {
												quit=1;
												css=0;csm=0;ccs=500;ccm=500;akttest=0;
											}
											else { css=ccs;csm=ccm;ccs=500;ccm=500; }
										}
										index=nindex;
										break;
									}
									break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									case KEY_RETURN:
									case KEY_ENTER:
										quit=1;break;
									case KEY_ESC:
										quit=2;break;
								}
								break;
							case IDCMP_RAWKEY:
								if(icode==RKEY_HELP)
									LIB_INTCALL(ShowHelp("html/nodes/node01.05.07.html"));
								break;
							case IDCMP_CLOSEWINDOW:
								quit=2;
								break;
						}
					}
				}
				CloseWindow(gswin);
			}
			else LIB_INTCALL(Message(errOpenWindow,NULL,__FILE__,__LINE__));
			FreeGadgets(gsGList);
		}
		else LIB_INTCALL(Message(errCreateGadgets,NULL,__FILE__,__LINE__));
	}
	else LIB_INTCALL(Message(errCreateContext,NULL,__FILE__,__LINE__));

	return((quit==1) ? GetNodeByIndex(list,index) : NULL);
}

//-- eof ----------------------------------------------------------------------
