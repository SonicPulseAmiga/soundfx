/****h* sfxsupport.library/fxobj-inter.c [1.20] *
*
*  NAME
*    fxobj-inter.c
*  COPYRIGHT
*    $VER: fxobj-inter.c 1.20 (20.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    interpolations object - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    20.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define FXOBJ_INTER_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- GUI Handling

struct Gadget * ASM LIB_EXPORT(AddInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y)));
void			ASM LIB_EXPORT(SetInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix)));
UBYTE			ASM LIB_EXPORT(HandleInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode)));

//-- ARexx Parsing

UBYTE			ASM LIB_EXPORT(ParseInterpolatorSet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params)));
UBYTE			ASM LIB_EXPORT(ParseInterpolatorGet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret)));

//-- defines ------------------------------------------------------------------

#define GADID_INTSEL	0
#define GADIX_INTSEL	0
#define GADID_INTINFO	(GADID_INTSEL+1)
#define GADIX_INTINFO	(GADIX_INTSEL+1)

//-- globals ------------------------------------------------------------------

UBYTE *InterpolLabels[]={
	"None",
	"Linear",
	"Si",
	"Lagrange",
	NULL
};

struct Node interpolNodes[INT_TYPE_ANZ];
struct List interpolList;

//-- definitions --------------------------------------------------------------

//-- GUI Handling

struct Gadget * SAVEDS ASM LIB_EXPORT(AddInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Gadget *gad[]),REG(a2,struct Gadget *g),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,UWORD x),REG(d3,UWORD y))) {
	struct NewGadget ng;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(inter,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	ng.ng_LeftEdge		=x+GetRunTime(scx[53]);
	ng.ng_TopEdge		=y;
	ng.ng_Width			=GetRunTime(scx[15]);
	ng.ng_Height		=GetRunTime(scy[11]);
	ng.ng_TextAttr		=GetRunTime(SigFont);
	ng.ng_GadgetText	=IMAGE_PopUp;
	ng.ng_GadgetID		=firstid+GADID_INTSEL;
	ng.ng_Flags			=PLACETEXT_IN;
	ng.ng_VisualInfo	=GetRunTime(VisualInfo);
	g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
	gad[firstix+GADIX_INTSEL]=g;

	ng.ng_LeftEdge		=x+GetRunTime(scx[69]);
	ng.ng_Width			=GetRunTime(scx[139]);
	ng.ng_TextAttr		=GetRunTime(ScrFont);
	ng.ng_GadgetText	="";
	ng.ng_GadgetID		=firstid+GADID_INTINFO;
	g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,inter->desc,TAG_DONE);
	gad[firstix+GADIX_INTINFO]=g;

	return(g);
}

void SAVEDS ASM LIB_EXPORT(SetInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstix))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(inter,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	inter->desc[0]=0;GetInterpol(inter);
	GT_SetGadgetAttrs(gad[firstix+GADIX_INTINFO],win,0l,GTTX_Text,inter->desc,TAG_DONE);
}

UBYTE SAVEDS ASM LIB_EXPORT(HandleInterpolator(REG(a0,Interpolator *inter),REG(a1,struct Window *win),REG(a2,struct Gadget *gad[]),REG(d0,ULONG firstid),REG(d1,ULONG firstix),REG(d2,ULONG id),REG(d3,USHORT icode))) {
	UBYTE ret=0;
	struct Requester NullReq;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(inter,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(gad,__FUNC__,"3.par",4));

	switch(id-firstid) {
		case GADID_INTSEL:
			ret=1;
			InitRequester(&NullReq);Request(&NullReq,win);SetWindowPointer(win,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
			GetInterpol(inter);
			EndRequest(&NullReq,win);SetWindowPointer(win,TAG_DONE);
			GT_SetGadgetAttrs(gad[firstix+GADIX_INTINFO],win,0l,GTTX_Text,inter->desc,TAG_DONE);
			break;
	}
	return(ret);
}

//-- ARexx Parsing

UBYTE SAVEDS ASM LIB_EXPORT(ParseInterpolatorSet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params))) {
	UBYTE pf=FALSE;
	ULONG sl=strlen(prefix);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(inter,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));

	if(!sl || (sl && !strnicmp(RexxPar2,prefix,sl))) {
		if(       !stricmp(&params[MaxParLen2+sl],"IntRange"))	{ inter->range=atof(RexxPar3);pf=TRUE; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"IntType"))	{ inter->type=CfgIO_MatchString(RexxPar3,InterpolLabels);pf=TRUE; }
	}
	return(pf);
}

UBYTE SAVEDS ASM LIB_EXPORT(ParseInterpolatorGet(REG(a0,Interpolator *inter),REG(a1,STRPTR prefix),REG(a2,STRPTR params),REG(a3,STRPTR ret))) {
	UBYTE pf=FALSE;
	ULONG sl=strlen(prefix);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(inter,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(prefix,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(params,__FUNC__,"3.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ret,__FUNC__,"4.par",4));

	if(!sl || (sl && !strnicmp(RexxPar2,prefix,sl))) {
		if(       !stricmp(&params[MaxParLen2+sl],"IntRange"))	{ sprintf(ret,"%7.4lf",inter->range);pf=TRUE; }
		if(!pf && !stricmp(&params[MaxParLen2+sl],"IntType"))	{ strcpy(ret,InterpolLabels[inter->type]);pf=TRUE; }
	}
	return(pf);
}

//-- eof ----------------------------------------------------------------------
