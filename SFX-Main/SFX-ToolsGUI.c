/****h*SoundFX/SFX-ToolsGUI.c [4.2] *
*
*  NAME
*    SFX-ToolsGUI.c
*  COPYRIGHT
*    $VER: SFX-ToolsGUI.c 4.2 (18.09.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    gui releated helper - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    18.Sep.2001	V 4.2	most recent version
*    30.Aug.2000	V 4.1	most recent version
*    31.May.2000	V 4.00	most recent version
*    30.Jan.2000	V 3.82	most recent version
*    15.Feb.1999	V 3.80	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_ToolsGUI_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void CText(struct RastPort *rp,UWORD x1,UWORD x2,UWORD y,char *text);

void FlushMsgPort(struct MsgPort *mp);

void RefreshGads(ULONG mode);
void ChangeSamples(void);

void ObtainGradientColors(void);
void FreeGradientColors(void);

//-- definitions --------------------------------------------------------------

void CText(struct RastPort *rp,UWORD x1,UWORD x2,UWORD y,char *text) {
	UWORD sl=strlen(text),tl=TextLength(rp,text,sl);
	WORD xp=x1+(((x2-x1)>>1)-(tl>>1));

	if(xp>0) {
		Move(rp,xp,y);Text(rp,text,sl);
	}
}

/** there are sometimes invalid pointers in the queue */
void FlushMsgPort(struct MsgPort *mp) {
	struct Message *msg;

	Forbid();					// alle Messages aus dem Port entfernen
	while(msg=(struct Message *)GetMsg(mp)) ReplyMsg(msg);	// RemHead(&mp->mp_MsgList)
	Permit();
}

void RefreshGads(ULONG mode) {
	if(!RunTime.quiet) {
		if(mode&WIN_OP) 	Refr_ToolbarOperator();
		if(mode&WIN_RXOP)	Refr_ToolbarRexx();
		if(mode&WIN_LO)		Refr_ToolbarLoader();
		if(mode&WIN_SA)		Refr_ToolbarSaver();
		if(mode&WIN_PL)		Refr_ToolbarPlayer();
		if(mode&WIN_ED)		Refr_ToolbarEdit();
		if(mode&WIN_ZO)		Refr_ToolbarZoom();
		if(mode&WIN_RNG) {
			Refr_ToolbarRange();
			Refr_StatusRange();
		}
		if(mode&WIN_BUF) {
			Refr_ToolbarBuffer();
			Refr_GlobalMenu();
			if(RunTime.aktbuf) Refr_ContextMenu();
		}
	}
}

/** call this when the number of sample has been changed */
void ChangeSamples(void) {
	INTRO;
	switch(RunTime.samplestat) {
		case SMPSTAT_UPDATE:					// its just one more or less
			MSG1("ChangeSamples: SMPSTAT_UPDATE %s",(RunTime.aktbuf?RunTime.aktbuf->node.ln_Name:""));
			RefreshGads(WIN_BUF|WIN_RNG);
			break;
		case SMPSTAT_FIRSTNEW:					// its the first
			ASSERT_RESET;
			ASSERT(RunTime.aktbuf);
			if(ASSERT_OKAY) {
				MSG1("ChangeSamples: SMPSTAT_FIRSTNEW %s",RunTime.aktbuf->node.ln_Name);
				RefreshGads(WIN_OP|WIN_RXOP|WIN_SA|WIN_PL|WIN_BUF|WIN_ED|WIN_ZO|WIN_RNG);
				RunTime.samplestat=SMPSTAT_UPDATE;
			}
			break;
		case SMPSTAT_LASTOLD:					// its the last
			MSG("ChangeSamples: SMPSTAT_LASTOLD");
			RefreshGads(WIN_OP|WIN_RXOP|WIN_SA|WIN_PL|WIN_BUF|WIN_ED|WIN_ZO|WIN_RNG);
			RunTime.samplestat=SMPSTAT_FIRSTNEW;
			break;
	}
	OUTRO;
}

void ObtainGradientColors(void) {
	register UBYTE j;
	struct ViewPort *vp;
	struct ColorMap *cm;
	WORD   crs,cre,cgs,cge,cbs,cbe;
	double cr ,crd,cg ,cgd,cb ,cbd;

	if(RunTime.grad_nr) {
		vp=&RunTime.sfxpubscr->ViewPort;
		cm=vp->ColorMap;
		cr=crs=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]][0];cre=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND]][0];crd=((double)cre-crs)/(double)RunTime.grad_nr;
		cg=cgs=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]][1];cge=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND]][1];cgd=((double)cge-cgs)/(double)RunTime.grad_nr;
		cb=cbs=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]][2];cbe=sfxprefs_gui.ScreenColors[sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND]][2];cbd=((double)cbe-cbs)/(double)RunTime.grad_nr;
		for(j=0;j<RunTime.grad_nr;j++) {
			RunTime.grad_cmap[j]=ObtainBestPen(cm,MakeCol32((ULONG)cr),MakeCol32((ULONG)cg),MakeCol32((ULONG)cb),OBP_Precision,PRECISION_EXACT);
			//MSG5("pen[%2d]=%3d  %6.2lf %6.2lf %6.2lf",j,RunTime.grad_cmap[j],cr,cg,cb);
			cr+=crd;cg+=cgd;cb+=cbd;
		}
	}
}

void FreeGradientColors(void) {
	register UBYTE j;
	struct ViewPort *vp;
	struct ColorMap *cm;

	if(RunTime.grad_nr) {
		vp=&RunTime.sfxpubscr->ViewPort;
		cm=vp->ColorMap;
		for(j=0;j<RunTime.grad_nr;j++) ReleasePen(cm,RunTime.grad_cmap[j]);
	}
}

//-- eof ----------------------------------------------------------------------
