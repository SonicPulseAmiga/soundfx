/****h* sfxsupport.library/fxobj-wfkt.c [4.2] *
*
*  NAME
*    fxobj-wfkt.c
*  COPYRIGHT
*    $VER: fxobj-wfkt.c 4.2 (04.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    windowfunction object - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    04.Jun.2002	V 4.2	most recent version
*    04.Nov.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define FXOBJ_WFKT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- GUI Handling

struct Gadget * ASM LIB_EXPORT(AddWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
void			ASM LIB_EXPORT(SetWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
UBYTE			ASM LIB_EXPORT(HandleWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

//-- ARexx Parsing

UBYTE			ASM LIB_EXPORT(ParseWinFunctionSet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
UBYTE			ASM LIB_EXPORT(ParseWinFunctionGet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));

//-- defines ------------------------------------------------------------------

#define GADID_WINSEL	0
#define GADIX_WINSEL	0
#define GADID_WININFO	(GADID_WINSEL+1)
#define GADIX_WININFO	(GADIX_WINSEL+1)

//-- globals ------------------------------------------------------------------

UBYTE *WFktLabels[]={
	"Bartlett",
	"Blackman",
	"Fejer",
	"HalfSine",
	"HalfSineQ",
	"HalfSineS",
	"Hamming",
	"Hanning",
	"Kaiser",
	"Rectangle",
	"Welch",
	NULL
};

struct Node wfktNodes[WIN_ANZ];
struct List wfktList;

//-- definitions --------------------------------------------------------------

//-- GUI Handling

struct Gadget * SAVEDS ASM LIB_EXPORT(AddWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y))) {
	struct NewGadget ng;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wfkt,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	ng.ng_LeftEdge		=x+GetRunTime(scx[53]);
	ng.ng_TopEdge		=y;
	ng.ng_Width			=GetRunTime(scx[15]);
	ng.ng_Height		=GetRunTime(scy[11]);
	ng.ng_TextAttr		=GetRunTime(SigFont);
	ng.ng_GadgetText	=IMAGE_PopUp;
	ng.ng_GadgetID		=firstid+GADID_WINSEL;
	ng.ng_Flags			=PLACETEXT_IN;
	ng.ng_VisualInfo	=GetRunTime(VisualInfo);
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
	gad[firstix+GADIX_WINSEL]=g;

	ng.ng_LeftEdge		=x+GetRunTime(scx[69]);
	ng.ng_Width			=GetRunTime(scx[139]);
	ng.ng_TextAttr		=GetRunTime(ScrFont);
	ng.ng_GadgetText	="";
	ng.ng_GadgetID		=firstid+GADID_WININFO;
	g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,wfkt->desc,TAG_DONE);
	gad[firstix+GADIX_WININFO]=g;

	return(g);
}

void SAVEDS ASM LIB_EXPORT(SetWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wfkt,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	wfkt->desc[0]=0;GetWFkt(wfkt);
	GT_SetGadgetAttrs(gad[firstix+GADIX_WININFO],win,0l,GTTX_Text,wfkt->desc,TAG_DONE);
}

UBYTE SAVEDS ASM LIB_EXPORT(HandleWinFunction(REG(a0,WinFunction *wfkt),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode))) {
	UBYTE ret=0;
	struct Requester NullReq;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wfkt,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	switch(id-firstid) {
		case GADID_WINSEL:
			ret=1;
			InitRequester(&NullReq);Request(&NullReq,win);SetWindowPointer(win,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
			GetWFkt(wfkt);
			EndRequest(&NullReq,win);SetWindowPointer(win,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_WININFO],win,0l,GTTX_Text,wfkt->desc,TAG_DONE);
			break;
	}
	return(ret);
}

//-- ARexx Parsing

UBYTE SAVEDS ASM LIB_EXPORT(ParseWinFunctionSet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params))) {
	UBYTE pf=FALSE;
	ULONG sl=strlen(prefix);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wfkt,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));

	if(!sl || (sl && !strnicmp(RexxPar2,prefix,sl))) {
		if(       !stricmp(&params[MaxParLen2+sl],"WinPar"))	{ wfkt->par=atof(RexxPar3);pf=TRUE; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"WinType"))	{ wfkt->win=CfgIO_MatchString(RexxPar3,WFktLabels);pf=TRUE; }
	}
	return(pf);
}

UBYTE SAVEDS ASM LIB_EXPORT(ParseWinFunctionGet(REG(a0,WinFunction *wfkt),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret))) {
	UBYTE pf=FALSE;
	ULONG sl=strlen(prefix);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wfkt,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ret,__FUNC__,"4.par",4));

	if(!sl || (sl && !strnicmp(&params[MaxParLen2],prefix,sl)))	{
		if(       !stricmp(&params[MaxParLen2+sl],"WinPar"))	{ sprintf(ret,"%7.4lf",wfkt->par);pf=TRUE; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"WinType"))	{ strcpy(ret,WFktLabels[wfkt->win]);pf=TRUE; }
	}
	return(pf);
}

//-- eof ----------------------------------------------------------------------
