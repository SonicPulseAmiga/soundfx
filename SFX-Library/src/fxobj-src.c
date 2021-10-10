/****h* sfxsupport.library/fxobj-src.c [4.1] *
*
*  NAME
*    fxobj-src.c
*  COPYRIGHT
*    $VER: fxobj-src.c 4.1 (29.10.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    source object - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    29.Oct.2000	V 4.1	most recent version
*    29.Apr.1999	V 1.20	most recent version
*                           A: using a cycle-gadget as alternitive for
*                              selecting sources
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*  TODO
*
*******
*/

#define FXOBJ_SRC_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

struct Gadget * ASM LIB_EXPORT(AddSource(REG(a0,Source *src),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
void			ASM LIB_EXPORT(SetSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
UBYTE			ASM LIB_EXPORT(HandleSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

//-- defines ------------------------------------------------------------------

#define GADID_SRCSEL	0
#define GADIX_SRCSEL	0
#define GADID_SRC		(GADID_SRCSEL+1)
#define GADIX_SRC		(GADIX_SRCSEL+1)
#define GADID_RMODE		(GADID_SRC+1)
#define GADIX_RMODE		(GADIX_SRC+1)

//-- globals ------------------------------------------------------------------

UBYTE *RModeLabels[]={ "All","Window","Range",0l };
UBYTE *EmptyLabels[]={ " ",0l };

//-- definitions --------------------------------------------------------------

struct Gadget * SAVEDS ASM LIB_EXPORT(AddSource(REG(a0,Source *src),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y))) {
	struct NewGadget ng;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	if(GetRunTime(sfxprefs_misc)->srcselect) {
		ng.ng_LeftEdge		=x+GetRunTime(scx[37]);
		ng.ng_TopEdge		=y+GetRunTime(scy[10]);
		ng.ng_Width			=GetRunTime(scx[170]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=firstid+GADID_SRCSEL;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EmptyLabels,GTCY_Active,0,TAG_DONE);
		gad[firstix+GADIX_SRCSEL]=g;
		gad[firstix+GADIX_SRC]=0;
	}
	else {
		ng.ng_LeftEdge		=x+GetRunTime(scx[37]);
		ng.ng_TopEdge		=y+GetRunTime(scy[10]);
		ng.ng_Width			=GetRunTime(scx[15]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_TextAttr		=GetRunTime(SigFont);
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=firstid+GADID_SRCSEL;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		gad[firstix+GADIX_SRCSEL]=g;

		ng.ng_LeftEdge		=x+GetRunTime(scx[53]);
		ng.ng_Width			=GetRunTime(scx[155]);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=firstid+GADID_SRC;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,0l,GTTX_CopyText,TRUE,TAG_DONE);
		gad[firstix+GADIX_SRC]=g;
	}

	ng.ng_LeftEdge		=x+GetRunTime(scx[214]);
	ng.ng_Width			=GetRunTime(scx[90]);
	ng.ng_GadgetText	="";
	ng.ng_GadgetID		=firstid+GADID_RMODE;
	ng.ng_Flags			=PLACETEXT_LEFT;
	g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,RModeLabels,GTCY_Active,src->rmode,TAG_DONE);
	gad[firstix+GADIX_RMODE]=g;

	return(g);
}

void SAVEDS ASM LIB_EXPORT(SetSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	if(src->srcbuf) {		// is there any source available
		if(!gad[firstix+GADIX_SRC]) {
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRCSEL],win,0l,GTCY_Labels,GetRunTime(barray),GTCY_Active,GetIndexByNode(GetRunTime(blist),(struct Node *)(src->srcbuf)),GA_Disabled,FALSE,TAG_DONE);
		}
		else {
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRCSEL],win,0l,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRC],win,0l,GTTX_Text,((struct Node *)(src->srcbuf))->ln_Name,TAG_DONE);
		}
		src->rmode=LIB_INTCALL(SetRngMode(src->srcbuf));
		GT_SetGadgetAttrs(gad[firstix+GADIX_RMODE],win,0l,GTCY_Active,src->rmode,GA_Disabled,FALSE,TAG_DONE);
	}
	else {
		if(!gad[firstix+GADIX_SRC]) {
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRCSEL],win,0l,GTCY_Labels,EmptyLabels,GTCY_Active,0,GA_Disabled,TRUE,TAG_DONE);
		}
		else {
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRCSEL],win,0l,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_SRC],win,0l,GTTX_Text,0l,TAG_DONE);
		}
		GT_SetGadgetAttrs(gad[firstix+GADIX_RMODE],win,0l,GTCY_Active,src->rmode,GA_Disabled,TRUE,TAG_DONE);
	}
}

UBYTE SAVEDS ASM LIB_EXPORT(HandleSource(REG(a0,Source *src),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode))) {
	SInfo *tbuf;
	UBYTE ret=FALSE;
	struct Requester NullReq;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	switch(id-firstid) {
		case GADID_SRCSEL:
			ret=TRUE;
			if(!gad[firstix+GADIX_SRC]) {
				if(icode!=GetIndexByNode(GetRunTime(blist),(struct Node *)(src->srcbuf))) {
					if(tbuf=(SInfo *)GetNodeByIndex(GetRunTime(blist),icode)) {
						LIB_INTCALL(UnlockBuffer(src->srcbuf));
						src->srcbuf=LIB_INTCALL(LockBuffer(tbuf));
						src->rmode=LIB_INTCALL(SetRngMode(src->srcbuf));
						GT_SetGadgetAttrs(gad[firstix+GADIX_RMODE],win,0l,GTCY_Active,src->rmode,TAG_DONE);
					}
				}
			}
			else {
				InitRequester(&NullReq);Request(&NullReq,win);SetWindowPointer(win,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
				if(tbuf=LIB_INTCALL(GetListEntry())) {
					LIB_INTCALL(UnlockBuffer(src->srcbuf));
					src->srcbuf=LIB_INTCALL(LockBuffer(tbuf));
					src->rmode=LIB_INTCALL(SetRngMode(src->srcbuf));
					GT_SetGadgetAttrs(gad[firstix+GADIX_SRC],win,0l,GTTX_Text,((struct Node *)(src->srcbuf))->ln_Name,TAG_DONE);
					GT_SetGadgetAttrs(gad[firstix+GADIX_RMODE],win,0l,GTCY_Active,src->rmode,TAG_DONE);
				}
				EndRequest(&NullReq,win);SetWindowPointer(win,TAG_DONE);
			}
			break;
		case GADID_RMODE:
			ret=TRUE;
			src->rmode=icode;
			break;
	}
	return(ret);
}

//-- eof ----------------------------------------------------------------------
