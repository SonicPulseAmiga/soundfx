/****h* sfxsupport.library/guiext.c [4.2] *
*
*  NAME
*    guiext.c
*  COPYRIGHT
*    $VER: guiext.c 4.2 (06.02.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    gui extensions - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    25.Jan.2000
*  MODIFICATION HISTORY
*    06.Feb.2002	V 4.2	most recent version
*    30.Oct.2000	V 4.1	most recent version
*    18.Feb.2000	V 1.30	most recent version
*    25.Jan.2000	V 1.30	initial version
*  NOTES
*
*******
*/

#define GUIEXT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- extra information

void		ASM LIB_EXPORT(SetStatus(REG(a0,STRPTR msg)));

//-- extended event handling

void		ASM LIB_EXPORT(NG_HandleEvents(REG(d0,ULONG iclass),REG(d1,UWORD icode),REG(d2,WORD imx),REG(d3,WORD imy),REG(a0,APTR iadr),REG(a1,struct Window *iwin),REG(a2,BOOL *closedd)));
void		HighLightGadgets(UWORD smx,UWORD smy);

//--globals -------------------------------------------------------------------

char sfxtitle[256];

//-- definitions --------------------------------------------------------------

//-- extra information

void SAVEDS ASM LIB_EXPORT(SetStatus(REG(a0,STRPTR msg))) {
	GT_SetGadgetAttrs(GetRunTime(winStatus)->FirstGadget->NextGadget,GetRunTime(winStatus),0l,GTTX_Text,msg,GTTX_CopyText,TRUE,TAG_DONE);
}

//-- extended event handling

void SAVEDS ASM LIB_EXPORT(NG_HandleEvents(REG(d0,ULONG iclass),REG(d1,UWORD icode),REG(d2,WORD imx),REG(d3,WORD imy),REG(a0,APTR iadr),REG(a1,struct Window *iwin),REG(a2,BOOL *closedd))) {
	UWORD x1,y1,x2,y2,w,h;
	BOOL change=FALSE;

	ASSERT_RESET;
	//ASSERTQ(IS_VALID_POINTER_N0(iadr,__FUNC__,"5.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(iwin,__FUNC__,"6.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(closedd,__FUNC__,"7.par",4));

	switch(iclass) {
		case IDCMP_MOUSEMOVE:
			if(!(iwin->Flags&WFLG_MENUSTATE)) HighLightGadgets(imx+iwin->LeftEdge,imy+iwin->TopEdge);
			break;
		case IDCMP_CHANGEWINDOW:
			w=iwin->Width;	x1=iwin->LeftEdge;	x2=x1+(w-1);
			h=iwin->Height;	y1=iwin->TopEdge;	y2=y1+(h-1);
			if(w >GetRunTime(clientrect).Width)		{ change=TRUE;w =GetRunTime(clientrect).Width;x2=x1+(w-1); }
			if(h >GetRunTime(clientrect).Height)		{ change=TRUE;h =GetRunTime(clientrect).Height;y2=y1+(h-1); }
			if(x1<GetRunTime(clientrect).LeftEdge)		{ change=TRUE;x1=GetRunTime(clientrect).LeftEdge; }
			if(y1<GetRunTime(clientrect).TopEdge)		{ change=TRUE;y1=GetRunTime(clientrect).TopEdge; }
			if(x2>GetRunTime(clientrect).RightEdge)	{ change=TRUE;x2=GetRunTime(clientrect).RightEdge;x1=x2-(w-1); }
			if(y2>GetRunTime(clientrect).BottomEdge)	{ change=TRUE;y2=GetRunTime(clientrect).BottomEdge;y1=y2-(h-1); }
			if(change) ChangeWindowBox(iwin,x1,y1,w,h);
			break;
		case IDCMP_ACTIVEWINDOW:
			LIB_INTCALL(MemUsage(sfxtitle));
			SetWindowTitles(iwin,(UBYTE *)~0,sfxtitle);
			break;
	}
}

/**
 * @todo consider gadtools gadgets as well (e.g. status bar)
 * @todo dynamic texts, e.g. rangemode & zoommode could displa the current mode
 */
void HighLightGadgets(UWORD smx,UWORD smy) {
	BOOL found;
	UWORD wmx,wmy,wix=0;
	//APTR tmp;
	WORD ngp;
	struct Gadget *ngad;
	struct Window *w,*wins[3];
	static WORD ogp=0;
	static struct Gadget *ogad=NULL;
	static struct Window *owin=NULL;

	if(GetRunTime(winDropDown)) { wins[0]=GetRunTime(winDropDown);wins[1]=GetRunTime(winToolbar);wins[2]=0l; }
	else	  				    { wins[0]=GetRunTime(winToolbar) ;wins[1]=0l; }
	if(owin && (owin!=GetRunTime(winToolbar)) && (owin!=GetRunTime(winDropDown))) { owin=NULL;ogad=NULL;MSG("oops -> window is gone"); }

	found=FALSE;
	w=wins[wix];
	while(w) {									/* check all toolbar windows */
		if((smx>w->LeftEdge) && (smy>w->TopEdge) && (smx<w->LeftEdge+w->Width) && (smy<w->TopEdge+w->Height)) {	/* is mouse inside this window ? */
			wmx=smx-w->LeftEdge;wmy=smy-w->TopEdge;
			ngad=w->FirstGadget;ngp=0;
			while(ngad) {
				if((ngad->GadgetType==GTYP_BOOLGADGET) && (!(ngad->Flags&GFLG_DISABLED))) {
					if((wmx>ngad->LeftEdge) && (wmy>ngad->TopEdge) && (wmx<ngad->LeftEdge+ngad->Width) && (wmy<ngad->TopEdge+ngad->Height)) {
						if(ngad!=ogad) {
							if(ogad && owin) {		// altes Gadget 'de'highlighten
								RemoveGadget(owin,ogad);
								ogad->GadgetRender=((struct NewToolbar *)ogad->UserData)->nt_Image;
								AddGadget(owin,ogad,ogp);
								RefreshGList(ogad,owin,0l,1);
							}
							RemoveGadget(w,ngad);
							ngad->GadgetRender=((struct NewToolbar *)ngad->UserData)->nt_HighImage;
							AddGadget(w,ngad,ngp);
							RefreshGList(ngad,w,0l,1);
							LIB_INTCALL(SetStatus(((struct NewToolbar *)ngad->UserData)->nt_Label));
							ogad=ngad;ogp=ngp;owin=w;
						}
						found=TRUE;ngad=NULL;
					}
				}
				if(!found) ngad=ngad->NextGadget;
				ngp++;
			}
			w=NULL;		// wenn Mouse in diesem Fenster war ab nicht über Button, brauchen wir trotzdem nicht weiter suchen
		}
		else {
			wix++;
			w=wins[wix];
		}
	}
	if((!found) && ogad && owin) {
		RemoveGadget(owin,ogad);
		ogad->GadgetRender=((struct NewToolbar *)ogad->UserData)->nt_Image;
		AddGadget(owin,ogad,ogp);
		RefreshGList(ogad,owin,0l,1);
		LIB_INTCALL(SetStatus(NULL));
		ogad=NULL;ogp=0;owin=0;
	}
}

//-- eof ----------------------------------------------------------------------
