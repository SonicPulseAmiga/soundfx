/****h* SoundFX/SFX-WinSample.c [0.0] *
*
*  NAME
*    SFX-WinSample.c
*  COPYRIGHT
*    $VER: SFX-WinSample.c 0.0 (17.04.04) © by Stefan Kost 2000-2004
*  FUNCTION                
*    Samplewindows - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Feb.2000
*  MODIFICATION HISTORY
*    17.Apr.2004    V 0.0   most recent version
*    21.Feb.2003	V 4.2	most recent version
*    13.Feb.2001	V 4.1	most recent version
*    19.Jul.2000	V 4.00	most recent version
*    15.Feb.2000	V 4.00	initial version
*  IDEAS
*    * mousebutton-down -> initial area bestimmen
*    * mousemove -> testen ob noch drin ?, sonst imx,imy auf grenzen des initial areas setzen
*  NOTES
*
*******
*/

#define SFX_WinSample_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void Test_SampleWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,SInfo *si,SInfo **clsi,struct IntuiMessage **imsg2);

void Handle_ContextMenu(SInfo *si,ULONG mx,ULONG my);
void Refr_ContextMenu(void);

UBYTE GetMouseChannel(SInfo *si,LONG yh,LONG *ys,LONG *ye,LONG rmy);
UBYTE GetMarkDirection(ULONG markx1,ULONG marky1,ULONG mrx1,ULONG mry1,ULONG mrx2,ULONG mry2);
UBYTE SetMarkPointer(SInfo *si,UBYTE dir,UBYTE *omptr);
UBYTE ClipMark(SInfo *si,UBYTE dir,LONG mxo,LONG myo);
UBYTE GetSampleWinArea(SInfo *si,WORD imx,WORD imy,LONG *rmx,LONG *rmy,UBYTE msel,UBYTE oldarea);

ULONG MouseEvent(ULONG iclass,ULONG icode,WORD imx,WORD imy,SInfo *si);

void OneSampleWin(void);
void MultiSampleWin(void);

//-- defines ------------------------------------------------------------------

//				LeftBorder YRuler Sample RightBorder
//				0          1      2      4
// TopBorder
// 0            0		   1      2      4
// XRuler
// 8            8          9      10     12
// Sample
// 16           16         17     18     20
// BottomBorder
// 32           32         33     34     36
//

#define AREAFX_LeftBorder	 0
#define AREAFX_YRuler		 1
#define AREAFX_Sample		 2
#define AREAFX_RightBorder	 4

#define AREAFY_TopBorder	 0
#define AREAFY_XRuler		 8
#define AREAFY_Sample		 16
#define AREAFY_BottomBorder	 32

#define AREA_Corner	(AREAFX_YRuler|AREAFY_XRuler)
#define AREA_XRuler	(AREAFX_Sample|AREAFY_XRuler)
#define AREA_YRuler (AREAFX_YRuler|AREAFY_Sample)
#define AREA_Sample (AREAFX_Sample|AREAFY_Sample)

//-- globals ------------------------------------------------------------------

extern struct Image imgHidden0,imgHidden1,imgHidden2;

//-- definitions --------------------------------------------------------------

void Test_SampleWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,SInfo *si,SInfo **clsi,struct IntuiMessage **imsg2) {
	register UBYTE i;
	ULONG tempul;
	LONG lpos,llen,poss,posl,hx;
	LONG rmx,rmy;									/* real mouse coordinates */
	static UBYTE loops=FALSE;						/* LoopSet ? */
	static UBYTE marks=FALSE;						/* MarkSet ? */
	static UBYTE draws=FALSE;						/* DrawStart ? */
	static UBYTE scroll=FALSE;						/* Scroller gedrückt */
	static UBYTE msel=FALSE;						/* MouseLeft = DOWN ? */
	static UBYTE omptr=MPTR_STANDARD;				/* old pointer (so we don't change it if we don't need to) */
	UBYTE mptr=FALSE;								/* Standartptr ? (0=ja) */
	UBYTE ch;										/* active Channel for */
	static UBYTE och;								/*   for Trace, Mark */
	static UBYTE dir=0;								/* in which part of the mark we clicking */
	UBYTE clip;										/* do we move the mark beyond some borders ? */
	static ULONG markx1=0,marky1=0;					/* old markpositions */
	ULONG markx2,marky2;							/* new markpositions */
	ULONG mrx1,mrx2,mry1,mry2;						/* for modifying a mark */
	LONG mxo,myo;
	static LONG yh=0,ys=0,ye=0;						/* Channelheight,~strt,~end */
	static UBYTE area=0,oldarea;					/* which part of the window are we in (curve, x-axis, y-axis) */
	struct NewToolbar *ntb;

	switch(iclass) {
		case IDCMP_ACTIVEWINDOW:
//			MSG2("  *** act sample is titled 0x%08ld::\"%s\"",si->title,si->title);
//			MSG3("main: IDCMP_ACTIVEWINDOW %d (%s,%s)",RunTime.swallowactivate,RunTime.aktbuf->node.ln_Name,si->node.ln_Name);
//			MSG1("si->win->Flags&WFLG_WINDOWACTIVE : 0x%08lx",si->win->Flags&WFLG_WINDOWACTIVE);
			if(!RunTime.swallowactivate) {
				if((RunTime.aktbuf!=si) && (si->win->Flags&WFLG_WINDOWACTIVE)) {
					RunTime.aktbuf=si;
					Refr_ToolbarBuffer();
					Refr_ToolbarRange();
					Refr_GlobalMenu();
					Refr_ContextMenu();
				}
				WindowToFront(si->win);
				//MSG("SampleWin: IDCMP_ACTIVEWINDOW -> TO FRONT");
			}
			else {
				RunTime.swallowactivate=0;ActivateWindow(RunTime.aktbuf->win);
			}
			yh=(si->aktch ? ((si->yres-si->aktch)/si->aktch) : 0);
			break;
		case IDCMP_CLOSEWINDOW:
			if(RunTime.play && si==RunTime.plsi) StopSample();
			RunTime.rngmove=RNGMOVE_NONE;
			if(!si->lockct) {
				UBYTE cont=1;
				if(si->safemode==1 && !si->saved) cont=Question(reqCloseSample,GetString(strOkay),GetString(strCancel));
				else if(si->safemode==2) cont=Question(reqCloseSample,GetString(strOkay),GetString(strCancel));
				if(cont) *clsi=si;
			}
			else Message(errCloseSample,NULL,NULL,0);
			break;
		case IDCMP_NEWSIZE:
		    MSG2("SampleWin: IDCMP_NEWSIZE (%s,%s)",RunTime.aktbuf->node.ln_Name,si->node.ln_Name);
//			while((*imsg2=GT_GetIMsg(swin_up)) && ((*imsg2)->Class==IDCMP_NEWSIZE)) GT_ReplyIMsg(*imsg2);		// skip newsizes
			LayoutSample(si,0);
			yh=(si->aktch ? ((si->yres-si->aktch)/si->aktch) : 0);
			break;
		case IDCMP_MOUSEBUTTONS:
			if(!si->aktch) break;	/* only care about mouse-moves when there are aktive channels */
			area=GetSampleWinArea(si,imx,imy,&rmx,&rmy,FALSE,0);
			////rmx=imx-si->xo;
			////rmy=imy-si->yo;
//			MSG4("SampleWin: IDCMP_MOUSEBUTTONS im[%4d,%4d] rm[%4d,%4d]",imx,imy,rmx,rmy);
			switch(icode) {
				case SELECTDOWN:
					msel=TRUE;
					////area=0;												/* get the part of the winodw we're over */
					////if(rmx>=0 && rmx<si->xres) area|=AREAFX_Sample;
					////else area|=AREAFX_YRuler;
					////if(rmy>=0 && rmy<si->yres) area|=AREAFY_Sample;
					////else area|=AREAFY_XRuler;
					oldarea=area;									/* to track where we started to select */
					switch(area) {
						case AREA_XRuler:					// we are on the x-axis ruler
							RunTime.rngmode=RNGMD_MARK;
							if(si->mark) DrawMark(si);
							markx1=si->markxs=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
							marky1=si->markys=si->zoomys;si->markyl=si->zoomyl;
							si->markxl=0;si->mark=1;marks=1;
							DrawMark(si);
							SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize64->Pointer,TAG_DONE);omptr=MPTR_SIZE64;
							Refr_ToolbarRange();
							break;
						case AREA_YRuler:					// we are on the y-axis ruler
							RunTime.rngmode=RNGMD_MARK;
							if(si->mark) DrawMark(si);
							markx1=si->markxs=si->zoomxs;si->markxl=si->zoomxl;
							marky1=si->markys=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
							si->markyl=0;si->mark=1;marks=1;
							DrawMark(si);
							SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize82->Pointer,TAG_DONE);omptr=MPTR_SIZE82;
							Refr_ToolbarRange();
							break;
						case AREA_Sample:						// we are inside the window (without axis rules)
							//-- determine channel mouse is in
							ch=GetMouseChannel(si,yh,&ys,&ye,rmy);
							och=ch;
							if(RunTime.rngmode==RNGMD_TRACE) {		/* Trace */
								if(si->aktch && si->zoomxl<si->xres) { draws=1;SetAPen(si->rp,sfxprefs_gui.Pens[PEN_SMP_LINE]); }
							}
							else {
								if(RunTime.aktbuf->loop && rmy<6) { 	/* Loop */
									hx=si->loops-si->zoomxs;
									poss=(LONG)(((double)hx*(double)si->xres)/(double)si->zoomxl);
									hx=(si->loops+si->loopl)-si->zoomxs;
									posl=(LONG)(((double)hx*(double)si->xres)/(double)si->zoomxl);
									if(poss<=rmx && (poss+6)>=rmx) { RunTime.loopp=0;loops=1; }
									if((posl-6)<=rmx && posl>=rmx) { RunTime.loopp=1;loops=1; }
									RunTime.rngmode=RNGMD_LOOP;
								}
								else {									/* Mark */
									RunTime.rngmode=RNGMD_MARK;
									if(si->mark) {
										markx1=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
										marky1=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
										if(markx1>=si->markxs && markx1<(si->markxs+si->markxl) && marky1>=si->markys && marky1<(si->markys+si->markyl)) {
											mrx1=si->markxs+(ULONG)(0.15*(double)si->markxl);if(mrx1==si->markxs) mrx1++;
											mrx2=si->markxs+(ULONG)(0.85*(double)si->markxl);if(mrx2==si->markxs+si->markxl) mrx2--;
											mry1=si->markys+(ULONG)(0.15*(double)si->markyl);if(mry1==si->markys) mry1++;
											mry2=si->markys+(ULONG)(0.85*(double)si->markyl);if(mry2==si->markys+si->markxl) mry2--;
											dir=GetMarkDirection(markx1,marky1,mrx1,mry1,mrx2,mry2);
											marks=2;
										}
										else {
											DrawMark(si);
											markx1=si->markxs=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
											marky1=si->markys=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
											si->markxl=si->markyl=0;si->mark=1;marks=1;
											DrawMark(si);
											SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrRange->Pointer,TAG_DONE);omptr=MPTR_RANGE;
										}
									}
									else {
										markx1=si->markxs=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
										marky1=si->markys=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
										si->markxl=si->markyl=0;si->mark=1;marks=1;
										DrawMark(si);
										SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrRange->Pointer,TAG_DONE);omptr=MPTR_RANGE;
									}
								}
							}
							Refr_ToolbarRange();
							break;
					}
					break;
				case SELECTUP:
					msel=FALSE;
					if(draws) DrawSample(si,0);
					if(marks && (!si->markxl) && (!si->markyl)) {		/* if user just clicked somewhere ... */
						DrawMark(si);
						si->markxl=1;si->markys=0;si->markyl=SMP_MAX;	/* ... make it a one pixel, but full height mark */
						DrawMark(si);
					}
					loops=0;marks=0;draws=0;
					break;
				/* handle popup-menus separately (see below) */
			}									 /* No break !!! */
		case IDCMP_MOUSEMOVE:
			while((*imsg2=GT_GetIMsg(swin_up)) && ((*imsg2)->Class==IDCMP_MOUSEMOVE)) {		// skip mousemoves
				imx=(*imsg2)->MouseX;
				imy=(*imsg2)->MouseY;
				GT_ReplyIMsg(*imsg2);
			}
			/* only care about mouse-moves when there are aktive channels and if there is no menu processing going on */
			if(!si->aktch || (si->win->Flags&WFLG_MENUSTATE)) {
				if(omptr!=MPTR_STANDARD) { SetWindowPointer(si->win,TAG_DONE);omptr=MPTR_STANDARD; }
				break;
			}
			area=GetSampleWinArea(si,imx,imy,&rmx,&rmy,msel,oldarea);
			//-- update big cross
			if(area==AREA_Sample) {
				//-- draw cross
				if(si->crs) DrawCross(si,si->cx,si->cy);
				else si->crs=TRUE;
				DrawCross(si,(si->cx=rmx),(si->cy=rmy));
			}
			else {
				//-- remove cross
				if(si->crs) {
					DrawCross(si,si->cx,si->cy);
					si->crs=FALSE;
				}
			}
			switch(area) {
				case AREA_XRuler:					// we are on the x-axis ruler
					//-- set mouse pointers
					if(!msel) {																	// if mousebutton is not pressed
						if(omptr!=MPTR_SIZE64) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrCrossHair->Pointer,TAG_DONE);omptr=MPTR_CROSSHAIR; }
					}
					else {																		// mousebutton is pressed
						DrawMark(si);
						markx2=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
						si->markxs=min(markx1,markx2);si->markxl=max(markx1,markx2)-si->markxs;
						DrawMark(si);
						Refr_StatusRange();
					}
					break;
				case AREA_YRuler:					// we are on the y-axis ruler
					//-- set mouse pointers
					if(!msel) {																	// if mousebutton is not pressed
						if(omptr!=MPTR_SIZE82) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrCrossHair->Pointer,TAG_DONE);omptr=MPTR_CROSSHAIR; }
					}
					else {																		// mousebutton is pressed
						DrawMark(si);
						marky2=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
						si->markys=min(marky1,marky2);si->markyl=max(marky1,marky2)-si->markys;
						DrawMark(si);
						Refr_StatusRange();
					}
					break;
				case AREA_Sample:					// are we inside the window (without axis rulers)
					//-- determine channel mouse is in
					ch=GetMouseChannel(si,yh,&ys,&ye,rmy);
					//-- get data for Statuswindow
					RunTime.mx1= si->zoomxs          +(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
					RunTime.my1=(si->zoomys+SMP_MAXN)+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
					if(si->storage==VM_MEMORY) RunTime.my2=si->buf[si->chnr[ch]][RunTime.mx1];
					else RunTime.my2=0;
					//-- set mouse pointers
					mptr=FALSE;
					if(!msel) {																	// if mousebutton is not pressed
						if(RunTime.aktbuf->loop && rmy<6) {										// Loop
							hx=si->loops-si->zoomxs;
							poss=(LONG)(((double)hx*(double)si->xres)/(double)si->zoomxl);
							hx=(si->loops+si->loopl)-si->zoomxs;
							posl=(LONG)(((double)hx*(double)si->xres)/(double)si->zoomxl);
							if((poss<=rmx && (poss+6)>=rmx) || ((posl-6)<=rmx && posl>=rmx)) {
								if(omptr!=MPTR_SIZE64) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize64->Pointer,TAG_DONE);omptr=MPTR_SIZE64; }
								mptr=TRUE;
							}
							RunTime.rngmode=RNGMD_LOOP;
						}
						else {																	// Mark
							if(si->mark) {
								markx1=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
								marky1=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
								if(markx1>=si->markxs && markx1<(si->markxs+si->markxl) && marky1>=si->markys && marky1<(si->markys+si->markyl)) {
									mrx1=si->markxs+(ULONG)(0.15*(double)si->markxl);if(mrx1==si->markxs) mrx1++;
									mrx2=si->markxs+(ULONG)(0.85*(double)si->markxl);if(mrx2==si->markxs+si->markxl) mrx2--;
									mry1=si->markys+(ULONG)(0.15*(double)si->markyl);if(mry1==si->markys) mry1++;
									mry2=si->markys+(ULONG)(0.85*(double)si->markyl);if(mry2==si->markys+si->markxl) mry2--;
									dir=GetMarkDirection(markx1,marky1,mrx1,mry1,mrx2,mry2);
									mptr|=SetMarkPointer(si,dir,&omptr);
								}
							}
						}
					}
					else {
						if(omptr==MPTR_STANDARD) {
							switch(RunTime.rngmode) {
								case RNGMD_LOOP:
									if(loops && RunTime.aktbuf->loop) {
										if(omptr!=MPTR_SIZE64) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize64->Pointer,TAG_DONE);omptr=MPTR_SIZE64; }
									}
									break;
								case RNGMD_MARK:
									dir=GetMarkDirection(markx1,marky1,mrx1,mry1,mrx2,mry2);
									mptr|=SetMarkPointer(si,dir,&omptr);
									break;
							}
						}
					}
					if(!mptr) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrCrossHair->Pointer,TAG_DONE);omptr=MPTR_CROSSHAIR; }
					//-- action, depending on the current rangemode
					switch(RunTime.rngmode) {
						case RNGMD_TRACE:
							if(si->zoomxl<si->xres && draws) {
								si->buf[si->chnr[ch]][RunTime.mx1]=RunTime.my1;
								SetAPen(si->rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
								WritePixel(si->rp,si->xo+rmx,si->yo+rmy);
							}
							break;
						case RNGMD_LOOP:
							if(loops && RunTime.aktbuf->loop) {
								if(RunTime.loopp) {		/* LoopL */
									DrawLoop(si);
									llen=(si->zoomxs+(LONG)(((double)rmx*(double)si->zoomxl)/(double)si->xres))-si->loops;
									si->loopl=max(0,llen);
									DrawLoop(si);
								}
								else {					/* LoopS */
									DrawLoop(si);
									lpos=si->loops;
									si->loops=si->zoomxs+(ULONG)(((double)rmx*(double)si->zoomxl)/(double)si->xres);
									llen=(LONG)si->loopl+(lpos-si->loops);
									si->loopl=max(0,llen);
									DrawLoop(si);
								}
								RecalcLoopPlayData();
							}
							break;
						case RNGMD_MARK:
							switch(marks) {
								case 1:		// new mark
									//if(och==ch) {
										DrawMark(si);
										markx2=si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres);
										marky2=si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh);
										si->markxs=min(markx1,markx2);si->markxl=max(markx1,markx2)-si->markxs;
										si->markys=min(marky1,marky2);si->markyl=max(marky1,marky2)-si->markys;
										DrawMark(si);
									//}
									break;
								case 2:		// modify mark
									//if(och==ch) {
										DrawMark(si);
										markx2=(si->zoomxs+(ULONG)(((double)    rmx *(double)si->zoomxl)/(double)si->xres));
										mxo=markx1-markx2;
										marky2=(si->zoomys+(ULONG)(((double)(ye-rmy)*(double)si->zoomyl)/(double)yh));
										myo=marky1-marky2;
										clip=ClipMark(si,dir,mxo,myo);
										DrawMark(si);
										if(!clip) { markx1=markx2;marky1=marky2; }
									//}
									break;
							}
							break;
					}
					Refr_StatusRange();
					break;
				default:
					if(omptr!=MPTR_STANDARD) {
						SetWindowPointer(si->win,TAG_DONE);omptr=MPTR_STANDARD;
					}
					break;
			}
			if(scroll) {
				switch((int)(((struct Gadget *)iadr)->GadgetID)) {
					case 0:
						if(si->slen<65536) si->zoomxs=icode;
						else si->zoomxs=(ULONG)icode<<4;
						DrawSample(si,1);
						break;
				}
				Refr_StatusRange();
			}
			break;
		case IDCMP_IDCMPUPDATE:
			switch(GetTagData(GA_ID, 0,(struct TagItem *)iadr)) {
				case GADID_SWB_HORIZ:
					GetAttr(PGA_TOP,si->scrobj[GADID_SWB_HORIZ],&tempul);
					i=0;while((si->slen>>i)>65535L) i++;
					posl=(tempul<<i);
					if(posl!=si->zoomxs) { si->zoomxs=posl;DrawSample(si,0);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange(); }
					break;
				case GADID_SWB_VERT:
					GetAttr(PGA_TOP,si->scrobj[GADID_SWB_VERT],&tempul);
					posl=65535-(tempul+si->zoomyl);
					if(posl!=si->zoomys) { si->zoomys=posl;DrawSample(si,0);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange(); }
					break;
				case GADID_SWB_LEFT:
					if(si->zoomxs) {
						tempul=si->zoomxl>>4;tempul=max(tempul,1);
						lpos=si->zoomxs-tempul;
						if(lpos>=0) si->zoomxs=lpos;
						else si->zoomxs=0;
						DrawSample(si,0);SetSlider(si,1);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange();
					}
					break;
				case GADID_SWB_RIGHT:
					if(si->zoomxs+si->zoomxl<si->slen) {
						tempul=si->zoomxl>>4;tempul=max(tempul,1);
						lpos=si->zoomxs+tempul;
						if(lpos+si->zoomxl<si->slen) si->zoomxs=lpos;
						else si->zoomxs=si->slen-si->zoomxl;
						DrawSample(si,0);SetSlider(si,1);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange();
					}
					break;
				case GADID_SWB_UP:
					if(si->zoomys+si->zoomyl<65535) {
						tempul=si->zoomyl>>4;tempul=max(tempul,1);
						lpos=si->zoomys+tempul;
						if(lpos+si->zoomyl<65535) si->zoomys=lpos;
						else si->zoomys=65535-si->zoomyl;
						DrawSample(si,0);SetSlider(si,2);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange();
					}
					break;
				case GADID_SWB_DOWN:
					if(si->zoomys) {
						tempul=si->zoomyl>>4;tempul=max(tempul,1);
						lpos=si->zoomys-tempul;
						if(lpos>=0) si->zoomys=lpos;
						else si->zoomys=0;
						DrawSample(si,0);SetSlider(si,2);RunTime.rngmode=RNGMD_ZOOM;Refr_ToolbarRange();
					}
					break;
				case GADID_SWB_ICONIFY:
					if(!(((struct Gadget *)(si->scrobj[GADID_SWB_ICONIFY]))->Flags&GFLG_SELECTED)) {
//						MSG1("  btn->Flags : 0x%08lx",((struct Gadget *)(si->scrobj[GADID_SWB_ICONIFY]))->Flags);
						HideSample();
						ntb=&SFXToolbar[GADIX_TB_HIDESHOW];
						ntb->nt_Image=&imgHidden0;ntb->nt_HighImage=&imgHidden1;   ntb->nt_ActiveImage=&imgHidden2;
						RemoveGadget(RunTime.winToolbar,ntb->nt_Gadget);
						ntb->nt_Gadget->GadgetRender=ntb->nt_Image;
						ntb->nt_Gadget->SelectRender=ntb->nt_ActiveImage;
						AddGadget(RunTime.winToolbar,ntb->nt_Gadget,0);
						RefreshGList(ntb->nt_Gadget,RunTime.winToolbar,NULL,1);
					}
					break;
				case GADID_SWB_POPUP:
					if(!(((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->Flags&GFLG_SELECTED)) {
//						MSG1("  btn->Flags : 0x%08lx",((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->Flags);
//						MSG2("  gad.x/y : %ld,%ld",((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->LeftEdge,((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->TopEdge);
//						MSG2("  gad.w/h : %ld,%ld",((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->Width,((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->Height);
						Handle_ContextMenu(si,(si->win->Width-2)+((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->LeftEdge,((struct Gadget *)(si->scrobj[GADID_SWB_POPUP]))->Height);
						yh=(si->aktch ? ((si->yres-si->aktch)/si->aktch) : 0);
					}
					break;
			}
			break;
		case IDCMP_RAWKEY:
			switch(icode) {
				case RKEY_ENTER:		/* Enter/Return */
				case RKEY_RETURN:
					SwitchIDCMPOFF();
					ProcessSample();
					SwitchIDCMPON();
					break;
				case RKEY_HELP:
					ShowHelp("html/nodes/node01.05.01.html");
					break;
				default:
					Test_RKeys(icode,iqual,si);
					break;
			}
			break;
		case IDCMP_MENUPICK:
			Test_Menu(icode,iqual);
			break;
	}
	/* handle popup-menus */
	if(	(iclass==IDCMP_MOUSEBUTTONS)
	&&	(((icode==MIDDLEUP) && (sfxprefs_misc.contextbutton==0))
		|| ((icode==MENUUP) && (sfxprefs_misc.contextbutton==1)))
	&&	(imx>si->win->BorderLeft)
	&&	(imy>si->win->BorderTop)
	&&	(imx<(si->win->Width-si->win->BorderRight))
	&&	(imy<(si->win->Height-si->win->BorderBottom))) {
		Handle_ContextMenu(si,imx,imy);
		yh=(si->aktch ? ((si->yres-si->aktch)/si->aktch) : 0); // because of possible enable / disable channel
	}
}

void Handle_ContextMenu(SInfo *si,ULONG mx,ULONG my) {
	register UBYTE i;
	WORD pmennum;									/* Popupmenunumber */
	struct NewToolbar *ntb;

	if(PopupMenuBase && pmenSampleWin) {
		pmennum=PM_OpenPopupMenu(si->win,PM_Menu,pmenSampleWin,PM_Left,mx,PM_Top,my,TAG_DONE);
		switch(pmennum) {
			case PMENID_DRMD_LINES:		si->drawmode=DM_LINES;		DrawSample(si,0);break;
			case PMENID_DRMD_DOTS:		si->drawmode=DM_DOTS;		DrawSample(si,0);break;
			case PMENID_DRMD_DOTSABS:	si->drawmode=DM_DOTSABS;	DrawSample(si,0);break;
			case PMENID_DRMD_DOTSHQ:	si->drawmode=DM_DOTSHQ;		DrawSample(si,0);break;
			case PMENID_DRMD_FILLED:	si->drawmode=DM_FILLED;		DrawSample(si,0);break;
			case PMENID_DRMD_FILLEDABS:	si->drawmode=DM_FILLEDABS;	DrawSample(si,0);break;
			case PMENID_DRMD_FILLEDHQ:	si->drawmode=DM_FILLEDHQ;	DrawSample(si,0);break;

			case PMENID_LOOP:
				si->loop=PM_ItemChecked(pmenSampleWin,PMENID_LOOP);
				if(si->loop) {
					if(si->loopl==0) {
						si->loops=0;
						si->loopl=si->slen-1;
					}
					RunTime.rngmode=RNGMD_LOOP;
				}
				DrawLoop(si);
				Refr_ToolbarRange();
				break;

			case PMENID_CH_0:
				if(PM_ItemChecked(pmenSampleWin,PMENID_CH_0)) { si->channelmask|=1;si->aktch++; }
				else { si->channelmask&=254;si->aktch--; }
				i=0;
				if(si->channelmask&1) si->chnr[i++]=0;
				if(si->channelmask&2) si->chnr[i++]=1;
				if(si->channelmask&4) si->chnr[i++]=2;
				if(si->channelmask&8) si->chnr[i  ]=3;
				DrawSample(si,0);
				break;
			case PMENID_CH_1:
				if(PM_ItemChecked(pmenSampleWin,PMENID_CH_1)) { si->channelmask|=2;si->aktch++; }
				else { si->channelmask&=253;si->aktch--; }
				i=0;
				if(si->channelmask&1) si->chnr[i++]=0;
				if(si->channelmask&2) si->chnr[i++]=1;
				if(si->channelmask&4) si->chnr[i++]=2;
				if(si->channelmask&8) si->chnr[i  ]=3;
				DrawSample(si,0);
				break;
			case PMENID_CH_2:
				if(PM_ItemChecked(pmenSampleWin,PMENID_CH_2)) { si->channelmask|=4;si->aktch++; }
				else { si->channelmask&=251;si->aktch--; }
				i=0;
				if(si->channelmask&1) si->chnr[i++]=0;
				if(si->channelmask&2) si->chnr[i++]=1;
				if(si->channelmask&4) si->chnr[i++]=2;
				if(si->channelmask&8) si->chnr[i  ]=3;
				DrawSample(si,0);
				break;
			case PMENID_CH_3:
				if(PM_ItemChecked(pmenSampleWin,PMENID_CH_3)) { si->channelmask|=8;si->aktch++; }
				else { si->channelmask&=247;si->aktch--; }
				i=0;
				if(si->channelmask&1) si->chnr[i++]=0;
				if(si->channelmask&2) si->chnr[i++]=1;
				if(si->channelmask&4) si->chnr[i++]=2;
				if(si->channelmask&8) si->chnr[i  ]=3;
				DrawSample(si,0);
				break;
			case PMENID_HIDE:
				HideSample();
				ntb=&SFXToolbar[GADIX_TB_HIDESHOW];
				ntb->nt_Image=&imgHidden0;ntb->nt_HighImage=&imgHidden1;   ntb->nt_ActiveImage=&imgHidden2;
				RemoveGadget(RunTime.winToolbar,ntb->nt_Gadget);
				ntb->nt_Gadget->GadgetRender=ntb->nt_Image;
				ntb->nt_Gadget->SelectRender=ntb->nt_ActiveImage;
				AddGadget(RunTime.winToolbar,ntb->nt_Gadget,0);
				RefreshGList(ntb->nt_Gadget,RunTime.winToolbar,NULL,1);
				break;
		}
	}
}

void Refr_ContextMenu(void) {
	INTRO;
	if(PopupMenuBase && pmenSampleWin) {
		if(RunTime.aktbuf->drawmode==DM_LINES)		PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_LINES		),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_LINES		),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_DOTS)		PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTS		),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTS		),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_DOTSABS)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTSABS	),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTSABS	),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_DOTSHQ)		PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTSHQ	),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_DOTSHQ	),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_FILLED)		PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLED	),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLED	),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_FILLEDABS)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLEDABS	),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLEDABS	),PM_Checked,FALSE,TAG_DONE);
		if(RunTime.aktbuf->drawmode==DM_FILLEDHQ)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLEDHQ	),PM_Checked,TRUE ,TAG_DONE);
		else 										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_DRMD_FILLEDHQ	),PM_Checked,FALSE,TAG_DONE);

		PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_LOOP),PM_Checked,RunTime.aktbuf->loop,TAG_DONE);

		if(RunTime.aktbuf->channels<3)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_3),PM_Disabled,TRUE ,PM_Checked,FALSE,TAG_DONE);
		else							PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_3),PM_Disabled,FALSE,PM_Checked,((RunTime.aktbuf->channelmask&8)?TRUE:FALSE),TAG_DONE);
		if(RunTime.aktbuf->channels<2)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_2),PM_Disabled,TRUE ,PM_Checked,FALSE,TAG_DONE);
		else							PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_2),PM_Disabled,FALSE,PM_Checked,((RunTime.aktbuf->channelmask&4)?TRUE:FALSE),TAG_DONE);
		if(RunTime.aktbuf->channels<1)	PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_1),PM_Disabled,TRUE ,PM_Checked,FALSE,TAG_DONE);
		else							PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_1),PM_Disabled,FALSE,PM_Checked,((RunTime.aktbuf->channelmask&2)?TRUE:FALSE),TAG_DONE);
										PM_SetItemAttrs(PM_FindItem(pmenSampleWin,PMENID_CH_0),PM_Disabled,FALSE,PM_Checked,((RunTime.aktbuf->channelmask&1)?TRUE:FALSE),TAG_DONE);
	}
	OUTRO;
}

UBYTE GetMouseChannel(SInfo *si,LONG yh,LONG *ys,LONG *ye,LONG rmy) {
	register UBYTE j;
	UBYTE ch=0;
	
	//-- determine channel mouse is in
	*ys=0;*ye=*ys+yh;
	for(j=0;j<si->aktch;j++) {
		if(rmy>=*ys && rmy<=*ye) { ch=j;break; }
		else { *ys=*ye+1;*ye=*ys+yh; }
	}
	//if(rmy==*ye) *ye++;
	return(ch);
}
                
UBYTE GetMarkDirection(ULONG markx1,ULONG marky1,ULONG mrx1,ULONG mry1,ULONG mrx2,ULONG mry2) {
	UBYTE dir=0;

	if(markx1<=mrx1) dir|=1;
	if(markx1>=mrx2) dir|=4;
	if(marky1<=mry1) dir|=2;
	if(marky1>=mry2) dir|=8;
	return(dir);
}


UBYTE SetMarkPointer(SInfo *si,UBYTE dir,UBYTE *omptr) {
	UBYTE mptr=0;
                                                    
	switch(dir) {
		case 0:
			if(*omptr!=MPTR_MOVE) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrMove->Pointer,TAG_DONE);*omptr=MPTR_MOVE; }
			mptr=1;break;
		case 1:
		case 4:
			if(*omptr!=MPTR_SIZE64) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize64->Pointer,TAG_DONE);*omptr=MPTR_SIZE64; }
			mptr=1;break;
		case 2:
		case 8:                        
			if(*omptr!=MPTR_SIZE82) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize82->Pointer,TAG_DONE);*omptr=MPTR_SIZE82; }
			mptr=1;break;
		case (1|2):
		case (4|8):
			if(*omptr!=MPTR_SIZE91) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize91->Pointer,TAG_DONE);*omptr=MPTR_SIZE91; }
			mptr=1;break;
		case (2|4):
		case (1|8):
			if(*omptr!=MPTR_SIZE73) { SetWindowPointer(si->win,WA_Pointer,RunTime.MPtrSize73->Pointer,TAG_DONE);*omptr=MPTR_SIZE73; }
			mptr=1;break;
	}
	return(mptr);
}

UBYTE ClipMark(SInfo *si,UBYTE dir,LONG mxo,LONG myo) {
	UBYTE clip=FALSE;

	if(!dir) {		// move
		if(mxo>0) {
			if(mxo<si->markxs) si->markxs-=mxo;
			else si->markxs=0;						// clip left smpborder
		}
		else {
			if((si->markxs-mxo)+si->markxl<si->slen) si->markxs-=mxo;
			else si->markxs=si->slen-si->markxl;	// clip right smpborder
		}
		if(myo>0) {
			if(myo<si->markys) si->markys-=myo;
			else si->markys=0;						// clip top smpborder
		}
		else {
			if((si->markys-myo)+si->markyl<SMP_MAX) si->markys-=myo;
			else si->markys=SMP_MAX-si->markyl;		// clip bottom smpborder
		}
	}
	if(dir&1) {		// modify left
		if(mxo>0) {
			if(mxo<si->markxs) { si->markxl+=mxo;si->markxs-=mxo;}
			else { clip=TRUE;si->markxs=0; }						   // clip left smpborder
		}
		else {
			if((-mxo)<si->markxl) { si->markxl+=mxo;si->markxs-=mxo; }
			else { clip=TRUE;si->markxs+=(si->markxl-1);si->markxl=1; }// clip right markborder
		}
	}
	if(dir&2) {		// modify top
		if(myo>0) {
			if(myo<si->markys) { si->markyl+=myo;si->markys-=myo; }
			else { clip=TRUE;si->markys=0; }						   // clip top smpborder
		}
		else {
			if((-myo)<si->markyl) { si->markyl+=myo;si->markys-=myo; }
			else { clip=TRUE;si->markys+=(si->markyl-1);si->markyl=1; }// clip bottom markborder
		}
	}
	if(dir&4) {		// modify right
		if(mxo>0) {
			if(mxo<si->markxl) si->markxl-=mxo;
			else { clip=TRUE;si->markxl=1; }						   // clip left markborder
		}
		else {
			if(si->markxs+(si->markxl-mxo)<si->slen) si->markxl-=mxo;
			else { clip=TRUE;si->markxl=si->slen-si->markxs; }		   // clip right smpborder
		}
	}
	if(dir&8) {		// modify bottom
		if(myo>0) {
			if(myo<si->markyl) si->markyl-=myo;
			else { clip=TRUE;si->markyl=1; }						   // clip top markborder
		}
		else {
			if(si->markys+(si->markyl-myo)<SMP_MAX) si->markyl-=myo;
			else { clip=TRUE;si->markyl=SMP_MAX-si->markys; }			   // clip bottom smpborder
		}
	}
	return(clip);
}

/* get the part of the winodw we're over */
UBYTE GetSampleWinArea(SInfo *si,WORD imx,WORD imy,LONG *rmx,LONG *rmy,UBYTE msel,UBYTE oldarea) {
	UBYTE area=0;

	if(imx<si->win->BorderLeft) {
		area|=AREAFX_LeftBorder;
		*rmx=si->win->BorderLeft-(LONG)si->xo;
	}
	else {
		*rmx=imx-(LONG)si->xo;							 /* subtract border (winborder+rulers) */
		if(*rmx<0) area|=AREAFX_YRuler;
		else if(*rmx<si->xres) area|=AREAFX_Sample;
		else { area|=AREAFX_RightBorder;*rmx=si->xres; }
	}
	if(imy<si->win->BorderTop) {
		area|=AREAFY_TopBorder;
		*rmy=si->win->BorderTop-(LONG)si->yo;
	}
	else {
		*rmy=imy-(LONG)si->yo;
		if(*rmy<0) area|=AREAFY_XRuler;
		else if(*rmy<si->yres) area|=AREAFY_Sample;
		else { area|=AREAFY_BottomBorder;*rmy=(si->yres-1); }
	}
	if(msel) {										/* mouse-button is pressed */
		if(area!=oldarea) {								/* we are somewhere else, than were we've started */
			switch(area) {
				case AREA_Corner:
					switch(oldarea) {
						case AREA_Sample:	*rmx=0;*rmy=0;break;
					}
					break;
				case AREA_XRuler:
					switch(oldarea) {
						case AREA_Sample:	*rmy=0;break;
					}
					break;
				case AREA_YRuler:
					switch(oldarea) {
						case AREA_Sample:	*rmx=0;break;
					}
					break;
				case AREA_Sample:
					switch(oldarea) {
						case AREA_XRuler:	*rmy=-1;break;
						case AREA_YRuler:	*rmx=-1;break;
					}
					break;
			}
			area=oldarea;
		}
		//MSG5("imx=%d  rmx=%d  bl=%d  xo=%d xres=%d",imx,*rmx,si->win->BorderLeft,si->xo,si->xres);
		//MSG5("imy=%d  rmy=%d  bt=%d  yo=%d yres=%d",imy,*rmy,si->win->BorderTop ,si->yo,si->yres);
		//MSG2("area = %d 0x%06x",area,area);
	}
	return(area);
}

//-- not marking, looping etc.
#define SMPWIN_STATE_IDLE				0
//-- mouse-move over loop-start			-> Mousepointer SIZE_64							-> SMPWIN_STATE_IDLE
//-- mouse-move over loop-end			-> Mousepointer SIZE_64							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-1		-> Mousepointer SIZE_91							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-2		-> Mousepointer SIZE_82							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-..		-> Mousepointer SIZE_XX							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-5		-> Mousepointer MOVE							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-..		-> Mousepointer SIZE_XX							-> SMPWIN_STATE_IDLE
//-- mouse-move over mark-area-9		-> Mousepointer SIZE_91							-> SMPWIN_STATE_IDLE
//-- mouse-move over ruler x			-> Mousepointer SIZE_64							-> SMPWIN_STATE_IDLE
//-- mouse-move over ruler y			-> Mousepointer SIZE_82							-> SMPWIN_STATE_IDLE
//-- mouse-move over sample				-> Mousepointer CROSS, TraceInfo, CrossHair		-> SMPWIN_STATE_IDLE
//-- mouse-btn-down over loop-start		-> RngMode(Loop)								-> SMPWIN_STATE_MOVE_LOOP_START
//-- mouse-btn-down over loop-end		-> RngMode(Loop)								-> SMPWIN_STATE_MOVE_LOOP_END
//-- mouse-btn-down over mark-area-1	-> RngMode(Mark)								-> SMPWIN_STATE_RESIZE_MARK_1
//-- mouse-btn-down over mark-area-2	-> RngMode(Mark)								-> SMPWIN_STATE_RESIZE_MARK_2
//-- mouse-btn-down over mark-area-..
//-- mouse-btn-down over mark-area-5	-> RngMode(Mark)								-> SMPWIN_STATE_MOVE_MARK
//-- mouse-btn-down over mark-area-..
//-- mouse-btn-down over mark-area-9	-> RngMode(Mark)								-> SMPWIN_STATE_RESIZE_MARK_9
//-- mouse-btn-down over ruler x		-> RngMode(Mark),SetNewMark						-> SMPWIN_STATE_CREATE_MARK_FULLY
//-- mouse-btn-down over ruler y		-> RngMode(Mark),SetNewMark						-> SMPWIN_STATE_CREATE_MARK_FULLX
//-- mouse-btn-down over sample 		-> RngMode(Mark),SetNewMark						-> SMPWIN_STATE_CREATE_MARK

#define SMPWIN_STATE_MOVE_LOOP_START	 1
#define SMPWIN_STATE_MOVE_LOOP_END		 2

#define SMPWIN_STATE_RESIZE_MARK_1		 3
#define SMPWIN_STATE_RESIZE_MARK_2		 4
#define SMPWIN_STATE_RESIZE_MARK_3		 5
#define SMPWIN_STATE_RESIZE_MARK_4		 6
#define SMPWIN_STATE_RESIZE_MARK_6		 7
#define SMPWIN_STATE_RESIZE_MARK_7		 8
#define SMPWIN_STATE_RESIZE_MARK_8		 9
#define SMPWIN_STATE_RESIZE_MARK_9		10

#define SMPWIN_STATE_MOVE_MARK			11

#define SMPWIN_STATE_CREATE_MARK_FULLY	12
#define SMPWIN_STATE_CREATE_MARK_FULLX	13
#define SMPWIN_STATE_CREATE_MARK		14
//-- mouse-btn-up						-> 												-> SMPWIN_STATE_IDLE
//-- mouse-move							-> EraseOldMark, DrawNewMark					-> SMPWIN_STATE_CREATE_MARK

// !! requires si->me_state (mouse-event-state)
// returns 

ULONG MouseEvent(ULONG iclass,ULONG icode,WORD imx,WORD imy,SInfo *si) {

	//-- generate event-code
	switch(iclass) {
		case IDCMP_MOUSEBUTTONS:
			switch(icode) {
				case SELECTDOWN:
					break;
				case SELECTUP:
					break;
			}
			break;
		case IDCMP_MOUSEMOVE:
			break;
	}
	//-- depending on state, invoke action and switch to new state
//	switch(si->me_state) {
//	}
	return(0);
}

void OneSampleWin(void) {
	SInfo *oldaktbuf=RunTime.aktbuf;

	if(RunTime.aktsamples) {
		TemporaryHideAllSounds();
		RunTime.onewindow=TRUE;
		ShowAllTemporaryHiddenSounds();

		RunTime.aktbuf=oldaktbuf;
		if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
	}
	else RunTime.onewindow=TRUE;
}

void MultiSampleWin(void) {
	SInfo *oldaktbuf=RunTime.aktbuf;
	struct Node *si;

	if(RunTime.aktsamples) {
		TemporaryHideAllSounds();
		RunTime.onewindow=FALSE;
		foreach(si,&BufferList) {								/* reshow all temporary hidden samples */
			if(SFX_SOUND_IS_TEMPHIDDEN((SInfo *)si)) {
				RunTime.aktbuf=(SInfo *)si;
				RunTime.aktbuf->xres=sfxprefs_sbuf.xs1;
				RunTime.aktbuf->yres=sfxprefs_sbuf.ys1;
				SetOptWinBounds(&RunTime.aktbuf->xpos,&RunTime.aktbuf->ypos,RunTime.aktbuf->xres,RunTime.aktbuf->yres);
				ShowSample();
			}
		}
		ShowAllTemporaryHiddenSounds();

		RunTime.aktbuf=oldaktbuf;
		if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
	}
	else RunTime.onewindow=FALSE;

}

//-- eof ----------------------------------------------------------------------
