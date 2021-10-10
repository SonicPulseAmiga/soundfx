/****h* sfxsupport.library/fxtools-gui.c [4.2] *
*
*  NAME
*    fxtools-gui.c
*  COPYRIGHT
*    $VER: fxtools-gui.c 4.2 (18.02.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    some fx gui helpers
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    24.Mar.2003
*  MODIFICATION HISTORY
*    24.Mar.2003    V 4.2   most recent version
*    24.Mar.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#define FXTOOLS_GUI_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- GUI-Tools / Gadgetabfrage,-verwaltung

void            ASM LIB_EXPORT(DrawGadWB(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h)));
void            ASM LIB_EXPORT(DrawGadBW(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h)));
void            ASM LIB_EXPORT(DrawTitle(REG(a0,struct RastPort *rp),REG(a1,STRPTR title),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w)));
void            ASM LIB_EXPORT(RText(REG(a0,struct RastPort *rp),REG(a1,STRPTR text),REG(d0,UWORD x),REG(d1,UWORD y)));
void            ASM LIB_EXPORT(CheckGad_int(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,LONG   *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv)));
void            ASM LIB_EXPORT(CheckGad_dbl(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,double *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv)));
void            ASM LIB_EXPORT(HandleParameter(REG(a0,STRPTR src),REG(a1,struct Window *win),REG(a2,struct Gadget *g)));

//-- definitions --------------------------------------------------------------

//-- GUI-Tools / Gadgetabfrage,-verwaltung

void SAVEDS ASM LIB_EXPORT(DrawGadWB(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(rp,__FUNC__,"1.par",4));

	Move(rp,x,y+h);
	SetAPen(rp,2);Draw(rp,x,y);Draw(rp,x+w,y);
	SetAPen(rp,6);Draw(rp,x+w,y+h);Draw(rp,x,y+h);
}

void SAVEDS ASM LIB_EXPORT(DrawGadBW(REG(a0,struct RastPort *rp),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w),REG(d3,UWORD h))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(rp,__FUNC__,"1.par",4));

	Move(rp,x,y+h);
	SetAPen(rp,6);Draw(rp,x,y);Draw(rp,x+w,y);
	SetAPen(rp,2);Draw(rp,x+w,y+h);Draw(rp,x,y+h);
}

void SAVEDS ASM LIB_EXPORT(DrawTitle(REG(a0,struct RastPort *rp),REG(a1,STRPTR title),REG(d0,UWORD x),REG(d1,UWORD y),REG(d2,UWORD w))) {
	UWORD sl=strlen(title);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(rp,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(title,__FUNC__,"2.par",4));

	if(GetRunTime(sfxpubscr)->RastPort.BitMap->Depth<4) {                  /* draw one color bar */
		SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_GUI_GRADIENTSTART]);RectFill(rp,x+1,y+1,x+w-2,y+GetRunTime(scy[8]));
	}
	else {                                                              /* draw gradient color bar */
		double xp,xd;
		UBYTE i;
		UWORD *optn,y1,y2;
		UWORD nptn1[2]={ 0xAAAA,0x0000 },nptn2[2]={ 0xAAAA,0x5555 },nptn3[2]={ 0xAAAA,0xFFFF };
		BYTE opsize;

		optn  =rp->AreaPtrn;opsize=rp->AreaPtSz;
		xd=(double)((x+w-1)-(x+1))/(double)(((GetRunTime(grad_nr)-1)<<2)+1);xp=(x+1);
		y1=y+1;y2=y+GetRunTime(scy[8]);
		SetAPen(rp,GetRunTime(grad_cmap[0]));
		RectFill(rp,(UWORD)xp,y1,(UWORD)(xp+xd),y2);xp+=xd;
		for(i=0;i<(GetRunTime(grad_nr)-1);i++) {
			SetABPenDrMd(rp,GetRunTime(grad_cmap[i+1]),GetRunTime(grad_cmap[i]),JAM2);
			rp->AreaPtrn=nptn1;rp->AreaPtSz=1;
			RectFill(rp,(UWORD)xp,y1,(UWORD)(xp+xd),y2);xp+=xd;
			rp->AreaPtrn=nptn2;
			RectFill(rp,(UWORD)xp,y1,(UWORD)(xp+xd),y2);xp+=xd;
			rp->AreaPtrn=nptn3;
			RectFill(rp,(UWORD)xp,y1,(UWORD)(xp+xd),y2);xp+=xd;
			rp->AreaPtrn=optn;rp->AreaPtSz=opsize;
			RectFill(rp,(UWORD)xp,y1,(UWORD)(xp+xd),y2);xp+=xd;
		}
	}

	SetABPenDrMd(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_GUI_COLORTEXTGLOW],0,JAM1); /* print title in 'glow-color' first */
	Move(rp,x+GetRunTime(scx[5])-1,y+GetRunTime(scy[6]));Text(rp,title,sl);
	Move(rp,x+GetRunTime(scx[5])+1,y+GetRunTime(scy[6]));Text(rp,title,sl);
	SetAPen(rp,GetRunTime(sfxprefs_gui)->Pens[PEN_GUI_COLORTEXT]);                 /* print title 'color' now */
	Move(rp,x+GetRunTime(scx[5])  ,y+GetRunTime(scy[6]));Text(rp,title,sl);
}

void SAVEDS ASM LIB_EXPORT(RText(REG(a0,struct RastPort *rp),REG(a1,STRPTR text),REG(d0,UWORD x),REG(d1,UWORD y))) {
	UWORD sl=strlen(text);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(rp,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(text,__FUNC__,"2.par",4));

	Move(rp,x-TextLength(rp,text,sl),y);Text(rp,text,sl);
}

void SAVEDS ASM LIB_EXPORT(CheckGad_int(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,LONG *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(val,__FUNC__,"3.par",4));

	*val=atoi(((struct StringInfo *)(g->SpecialInfo))->Buffer);
	if(chkrng) {
		if(*val>maxv) {
			*val=maxv;GT_SetGadgetAttrs(g,win,NULL,GTIN_Number,*val,TAG_DONE);
		}
		if(*val<minv) {
			*val=minv;GT_SetGadgetAttrs(g,win,NULL,GTIN_Number,*val,TAG_DONE);
		}
	}
}

void SAVEDS ASM LIB_EXPORT(CheckGad_dbl(REG(a0,struct Gadget *g),REG(a1,struct Window *win),REG(a2,double *val),REG(d0,UBYTE chkrng),REG(d1,LONG minv),REG(d2,LONG maxv))) {
	char str[25];

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(val,__FUNC__,"3.par",4));

	*val=atof(((struct StringInfo *)(g->SpecialInfo))->Buffer);
	if(chkrng) {
		if(*val>(double)maxv) {
			*val=(double)maxv;
			sprintf(str,"%20.15lf",*val);LIB_INTCALL(FilterString(str));
			GT_SetGadgetAttrs(g,win,NULL,GTST_String,str,TAG_DONE);
		}
		if(*val<(double)minv) {
			*val=(double)minv;
			sprintf(str,"%20.15lf",*val);LIB_INTCALL(FilterString(str));
			GT_SetGadgetAttrs(g,win,NULL,GTST_String,str,TAG_DONE);
		}
	}
}

void SAVEDS ASM LIB_EXPORT(HandleParameter(REG(a0,STRPTR src),REG(a1,struct Window *win),REG(a2,struct Gadget *g))) {
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(g,__FUNC__,"3.par",4));

	if(((struct StringInfo *)(g->SpecialInfo))->Buffer) {
		strncpy(src,((struct StringInfo *)(g->SpecialInfo))->Buffer,PARBUFLEN);
		src[PARBUFLEN]='\0';
	}
	else GT_SetGadgetAttrs(g,win,NULL,GTST_String,src,TAG_DONE);
}

//-- eof ----------------------------------------------------------------------
