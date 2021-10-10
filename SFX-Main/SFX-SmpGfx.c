/****h* SoundFX/SFX-SmpGfx.c [4.3] *
*
*  NAME
*    SFX-SmpGfx.c
*  COPYRIGHT
*    $VER: SFX-SmpGfx.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    sample graphic routines - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Feb.2003	V 4.2	most recent version
*    27.Aug.2000	V 4.1	most recent version
*    12.Jun.2000	V 4.0	most recent version
*    03.Dec.1999	V 3.81	most recent version
*                             F: drawing of very small windows (switch off scalas, at least one scala number)
*    21.Apr.1999	V 3.80	most recent version
*    08.Nov.1998	V 3.71	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    19.Jul.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_SmpGfx_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- sample drawing

void LayoutSample(SInfo *si,UBYTE mode);
void DrawSample(SInfo *si,UBYTE mode);
void Layout(SInfo *si);

//-- helplines drawing

void DrawSampleExtra(SInfo *si,UBYTE channel,ULONG y,ULONG h);

//-- waveform drawing

void DrawSampleL    (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleD    (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleDA   (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleDHQ  (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleF    (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleFA   (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleFHQ  (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);

void DrawSampleFHQF (SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);
void DrawSampleSpect(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h);

//-- Markers, Ranges, etc.

void DrawLoop (SInfo *si);
void DrawMark (SInfo *si);
void DrawCross(SInfo *si,UWORD xpos,UWORD ypos);
void DrawPPos (SInfo *si,ULONG pos);

//-- Axis drawing

void DrawAxisX(SInfo *si);
void DrawAxisY(SInfo *si);

//-- Event Tester

char TestForEvent(struct Window *win,ULONG mask);

//-- globals ------------------------------------------------------------------

UWORD hlines[7][2]={
	{ 8192,0xCCCC},
	{16384,0xAAAA},
	{24576,0xCCCC},
	{32768,0xFFFF},
	{40960,0xCCCC},
	{49152,0xAAAA},
	{57344,0xCCCC}
};

// we can break on IDCMP_IDCMPUPDATE as we don't know which is the last one
//#define ABORTDRAW_IDCMPS IDCMP_IDCMPUPDATE|IDCMP_RAWKEY|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW
#define ABORTDRAW_IDCMPS IDCMP_RAWKEY|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW

//-- definitions --------------------------------------------------------------

//-- sample drawing

void LayoutSample(SInfo *si,UBYTE mode) {
	UWORD oldxres,oldyres;

	INTRO;

	oldxres=si->xres;oldyres=si->yres;
	Layout(si);
	//-- if sample has been resized
	if((oldxres!=si->xres || oldyres!=si->yres)) DrawSample(si,0);

	OUTRO;
}

/** @todo call this a a new process, so that in case of a redraw, we can simply kill and restart this process */

void DrawSample(SInfo *si,UBYTE mode) {
	INTRO;
	if(si && SFX_SOUND_IS_SHOWN(si)) {
		struct RastPort *rp=si->rp;

		si->crs=FALSE;		 // if there was a cross active, we discard it
		if(si->aktch) {
			/** @TODO: run this as a process, if there is already one for this buffer kill it (RemTask) */
			ULONG yh=1+((si->yres-si->aktch)/si->aktch),ys=0;

			if(si->scalax && si->scays) DrawAxisX(si); // !! leave this here, we can't put it above, because we
			if(si->scalay && si->scaxs) DrawAxisY(si); // need x and y-res' calculated

			SetABPenDrMd(rp,sfxprefs_gui.Pens[PEN_SMP_BACK],0,JAM1);
			RectFill(rp,si->xo,si->yo,si->xo+si->xres,si->yo+si->yres);
			switch(si->drawmode) {
				case DM_LINES:
					if(si->channelmask&1) {	 DrawSampleL(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleL(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleL(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleL(si,mode,3,ys,yh);
					break;
				case DM_DOTS:
					if(si->channelmask&1) {	 DrawSampleD(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleD(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleD(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleD(si,mode,3,ys,yh);
					break;
				case DM_DOTSABS:
					if(si->channelmask&1) {	 DrawSampleDA(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleDA(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleDA(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleDA(si,mode,3,ys,yh);
					break;
				case DM_DOTSHQ:
					if(si->zoomxl>si->xres) {
						if(si->channelmask&1) {	 DrawSampleDHQ(si,mode,0,ys,yh);ys+=yh; }
						if(si->channelmask&2) {	 DrawSampleDHQ(si,mode,1,ys,yh);ys+=yh; }
						if(si->channelmask&4) {	 DrawSampleDHQ(si,mode,2,ys,yh);ys+=yh; }
						if(si->channelmask&8)   DrawSampleDHQ(si,mode,3,ys,yh);
					}
					else {
						if(si->channelmask&1) {	 DrawSampleD(si,mode,0,ys,yh);ys+=yh; }
						if(si->channelmask&2) {	 DrawSampleD(si,mode,1,ys,yh);ys+=yh; }
						if(si->channelmask&4) {	 DrawSampleD(si,mode,2,ys,yh);ys+=yh; }
						if(si->channelmask&8)   DrawSampleD(si,mode,3,ys,yh);
					}
					break;
				case DM_FILLED:
					if(si->channelmask&1) {	 DrawSampleF(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleF(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleF(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleF(si,mode,3,ys,yh);
					break;
				case DM_FILLEDABS:
					if(si->channelmask&1) {	 DrawSampleFA(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleFA(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleFA(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleFA(si,mode,3,ys,yh);
					break;
				case DM_FILLEDHQ:
					if(si->zoomxl>si->xres) {
						if(si->channelmask&1) {	 DrawSampleFHQ(si,mode,0,ys,yh);ys+=yh; }
						if(si->channelmask&2) {	 DrawSampleFHQ(si,mode,1,ys,yh);ys+=yh; }
						if(si->channelmask&4) {	 DrawSampleFHQ(si,mode,2,ys,yh);ys+=yh; }
						if(si->channelmask&8)   DrawSampleFHQ(si,mode,3,ys,yh);
					}
					else {
						if(si->channelmask&1) {	 DrawSampleF(si,mode,0,ys,yh);ys+=yh; }
						if(si->channelmask&2) {	 DrawSampleF(si,mode,1,ys,yh);ys+=yh; }
						if(si->channelmask&4) {	 DrawSampleF(si,mode,2,ys,yh);ys+=yh; }
						if(si->channelmask&8)   DrawSampleF(si,mode,3,ys,yh);
					}
					break;
				/*
				case DM_FILLEDHQF:
					if(si->channelmask&1) {	 DrawSampleFHQF(si,mode,0,ys,yh);ys+=yh; }
					if(si->channelmask&2) {	 DrawSampleFHQF(si,mode,1,ys,yh);ys+=yh; }
					if(si->channelmask&4) {	 DrawSampleFHQF(si,mode,2,ys,yh);ys+=yh; }
					if(si->channelmask&8)   DrawSampleFHQF(si,mode,3,ys,yh);
					break;
				case DM_SPECT:
					if(	(flbuf_r=(double *)AllocVec(size,MEMF_ANY))
					&&	(flbuf_i=(double *)AllocVec(size,MEMF_ANY))
					&&	(win_fkt=NewWindowFkt((mm+2),win,&par))
					&&           NewFFTTabs(&bitrevtab,&sintab1,NULL,&costab,m)
					&&	(rbuf	=NewRingBuf(mm,&rbufbase,&rbufmask,&rbufsize))) {
						DrawSampleSpect(...,0,...);
						DrawSampleSpect(...,1,...);
						...
					}
					if(flbuf_r)		FreeVec((ULONG *)flbuf_r);
					if(flbuf_i)		FreeVec((ULONG *)flbuf_i);
									DelFFTTabs(bitrevtab,sintab1,NULL,costab);
					if(win_fkt)		DelWindowFkt(win_fkt);
					if(rbuf)		DelRingBuf(rbuf);
					break;
				*/
				UNEXPECTED_DEFAULT
			}
			if(!mode || !si->quickdraw) {
				if(si->loop && si->loopl) DrawLoop(si);
				if(si->mark) DrawMark(si);
			}
			if(RunTime.play) {
				if(RunTime.plptr<=RunTime.plslen || RunTime.plsi->loop) DrawPPos(RunTime.plsi,RunTime.oldptr);	  /* Draw Pos */
			}
		}
		else {
			STRPTR msg=GetString(msgNoChannelSelected);
			SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_BACK]);
			RectFill(rp,si->xo,si->yo,si->xo+si->xres,si->yo+si->yres);
			SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_TEXT]);
			CText(rp,0,si->xres,((si->yres>>1)+RunTime.scy[3]),msg);
		}
	}
	else ERR("DrawSample called for invalid sinfo");
	OUTRO;
}

void Layout(SInfo *si) {
	//-- adapt y-axis
	if(si->aktch && si->scalax) {
		si->scays=RunTime.scafont->tf_YSize+6;
		si->yo   =si->win->BorderTop+si->scays;
		si->yres =si->yo+si->win->BorderBottom+1;
		if(si->win->Height>si->yres) si->yres=si->win->Height-si->yres;
		else {
			si->scays=0;
			si->yo   =si->win->BorderTop;
			si->yres =si->win->Height-(si->win->BorderTop+si->win->BorderBottom+1);
		}
	}
	else {
		si->scays=0;
		si->yo   =si->win->BorderTop;
		si->yres =si->win->Height-(si->win->BorderTop+si->win->BorderBottom+1);
	}
	//-- adapt x-axis
	if(si->aktch && si->scalay) {
		si->scaxs=RunTime.scafont->tf_XSize*6+3+5;
		si->xo   =si->win->BorderLeft+si->scaxs;
		si->xres =si->xo+si->win->BorderRight+1;
		if(si->win->Width>si->xres) si->xres=si->win->Width-si->xres;
		else {
			si->scaxs=0;
			si->xo   =si->win->BorderLeft;
			si->xres =si->win->Width-(si->win->BorderLeft+si->win->BorderRight+1);
		}
	}
	else {
		si->scaxs=0;
		si->xo   =si->win->BorderLeft;
		si->xres =si->win->Width-(si->win->BorderLeft+si->win->BorderRight+1);
	}
}

//-- helplines drawing

void DrawSampleExtra(SInfo *si,UBYTE channel,ULONG y,ULONG h) {
	register ULONG i;
	LONG smpline;
	ULONG yh,xs,xe,ys,ye;
	// ULONG ys1,ye1;
	double vptr,vptrs,vstep,vstart,yscl;
	struct RastPort *rp=si->rp;

	//INTRO;
	yscl=(double)h/(double)(si->zoomyl+1);

	xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;

	//-- channel divider
	SetAPen(rp,0);
	/** @todo skip for first visible channel */
	//if((channel!=0) || (!(si->channelmask&(1<<(channel-1))))) {
		Move(rp,xs,ys);Draw(rp,xe,ys);
	//}
	/** @todo skip for last visible channel */
	//if((channel!=3) || (!(si->channelmask&(1<<(channel+1))))) {
		Move(rp,xs,ye);Draw(rp,xe,ye);
	//}

	//-- max volume lines
	if(si->maxlines) {
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_MAXL]);
		smpline=si->smax[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ys1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ys;
		smpline=si->smin[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ye1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ye;
		//RectFill(rp,xs,ys1,xe,ye1);
	}
	// rms volume lines
	if(si->rmslines) {
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_RMSL]);
		smpline= si->srms[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ys1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ys;
		smpline=-si->srms[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ye1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ye;
		//RectFill(rp,xs,ys1,xe,ye1);
	}
	//-- avg volumne lines
	if(si->avglines) {
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_AVGL]);
		smpline= si->savg[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ys1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ys;
		smpline=-si->savg[channel]-SMP_MAXN;
		if(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) { yh=ye-(LONG)(yscl*(smpline-si->zoomys));Move(rp,xs,yh);Draw(rp,xe,yh); }
		//ye1=(smpline>si->zoomys && smpline<(si->zoomys+si->zoomyl)) ? ye-(LONG)(yscl*(smpline-si->zoomys)) : ye;
		//RectFill(rp,xs,ys1,xe,ye1);
	}

	//-- horizontal guides
	SetABPenDrMd(rp,sfxprefs_gui.Pens[PEN_SMP_GRID],sfxprefs_gui.Pens[PEN_SMP_BACK],JAM1);
	if(si->rastery) {
		for(i=0;i<7;i++) {
			if(hlines[i][0]>si->zoomys && hlines[i][0]<(si->zoomys+si->zoomyl)) {
				SetDrPt(rp,hlines[i][1]);
				yh=ye-(LONG)(yscl*(hlines[i][0]-si->zoomys));
				Move(rp,xs,yh);Draw(rp,xe,yh);
			}
		}
	}

	//-- vertical guides
	if(si->rasterx) {
		if(si->zoomxl>10000000) vstep=10000000.0;
		else if(si->zoomxl>1000000) vstep=1000000.0;
		else if(si->zoomxl>100000) vstep=100000.0;
		else if(si->zoomxl>10000) vstep=10000.0;
		else if(si->zoomxl>1000) vstep=1000.0;
		else if(si->zoomxl>100) vstep=100.0;
		else if(si->zoomxl>10) vstep=10.0;
		else vstep=1.0;
		
		vptrs=(double)si->xres/((double)si->zoomxl/vstep);
		SetDrPt(rp,0xAAAA);
		vstart=(double)si->zoomxs/vstep;
		vstart=(1.0-(vstart-(double)((LONG)vstart)))*vptrs;
		for(vptr=(double)(xs+vstart);vptr<((double)(xs+si->xres));vptr+=vptrs) {
			Move(rp,(int)vptr,ys);Draw(rp,(int)vptr,ye);
		}
	}
	SetDrPt(rp,65535);
	//OUTRO;
}

//-- waveform drawing

void DrawSampleL(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register UWORD xh;
	register ULONG yp,pos;
	register LONG ptr;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	INTRO;
	xh=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		xscl=(double)si->zoomxl/(double)si->xres;								      // Samples per Pixel
		yscl=(double)h/(double)(si->zoomyl+1);
		pos=ptr=off=0;																		// pos ist gezeichneter Pixel
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);

		//if((SMP_MAX<<1)<si->zoomys) Move(rp,xh,ye);
		//else if((SMP_MAX<<1)>zoomye) Move(rp,xh,ys);
		//else { yp=ye-(LONG)(yscl*(si->zoomys-SMP_MAXN));Move(rp,xh,yp); }
		buf=(SAMPLE *)sa->buffer1;
		yp=(ULONG)(buf[0]-SMP_MAXN);
		if(yp<si->zoomys) Move(rp,xh,ye);
		else if(yp>zoomye) Move(rp,xh,ys);
		else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Move(rp,xh,yp); }

		for(sa->seg=1;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++) {				// weil wir letztes Segment extra zeichnen
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			while((ptr-off)<Bytes2Samples(sa->blen)) {
				ERRC1(((ptr-off)<0),"floating point under-run : %ld",-(ptr-off));
				ERRC1(((ptr-off)>Bytes2Samples(sa->blen)),"floating point over-run : %ld",((ptr-off)-Bytes2Samples(sa->blen)));

				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);									// do not subtract off here, as it may change below
			}
			off+=Bytes2Samples(sa->blen);						// how many blocks did we have already drawn
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running && (xh<xe)) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					// just fetch the last block
			while(xh<xe) {
				ERRC1(((ptr-off)<0),"floating point under-run : %ld",-(ptr-off));
				ERRC1(((ptr-off)>Bytes2Samples(sa->blen)),"floating point over-run : %ld",((ptr-off)-Bytes2Samples(sa->blen)));

				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
		}
		SADone(sa,si);
	}
	else MSG("SAInit failed");
	OUTRO;
}

void DrawSampleD(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register UWORD xh;
	register ULONG yp,pos,ptr;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	xh=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		xscl=(double)si->zoomxl/(double)si->xres;								      // Samples per Pixel
		yscl=(double)h/(double)(si->zoomyl+1);
		pos=ptr=off=0;
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		for(sa->seg=1;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			while((ptr-off)<Bytes2Samples(sa->blen)) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
			off+=Bytes2Samples(sa->blen);
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running && (xh<xe)) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					   // just fetch the last block
			while(xh<xe) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
			SADone(sa,si);
		}
	}
}

void DrawSampleDA(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register UWORD xh;
	register ULONG yp,pos,ptr;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	xh=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		xscl=(double)si->zoomxl/(double)si->xres;								      // Samples per Pixel
		yscl=(double)h/(double)(si->zoomyl+1);
		pos=ptr=off=0;
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		for(sa->seg=1;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			while((ptr-off)<Bytes2Samples(sa->blen)) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				yp=(ULONG)(SMP_MAX-(buf[ptr-off]-SMP_MAXN));
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
			off+=Bytes2Samples(sa->blen);
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running && (xh<xe)) {
			buf=(WORD *)sa->buffer1;SARead(sa,0);					 // just fetch the last block
			while(xh<xe) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				yp=(ULONG)(SMP_MAX-(buf[ptr-off]-SMP_MAXN));
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
		}
		SADone(sa,si);
	}
}

void DrawSampleDHQ(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register ULONG i;
	register UWORD optr,ptr=0;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	LONG yp;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	//MSG3("zys/e/l %6d %6d %6d",si->zoomys,zoomye,si->zoomyl);

	optr=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		yscl=(double)h/(double)(si->zoomyl+1);
		xscl=(double)si->xres/(double)si->zoomxl;
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		for(sa->seg=1,off=0;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++,off+=Bytes2Samples(sa->blen)) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			for(i=0;i<Bytes2Samples(sa->blen);i++) {
				yp=(ULONG)(*(buf++)-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,optr,yp); }
				ptr=xs+(UWORD)((double)(off+i)*xscl);			// pixel coord.
				if(optr<ptr) optr=ptr;
			}
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					   // just fetch the last block
			for(i=0;ptr<(xe-1);i++)	{
				yp=(ULONG)(*(buf++)-SMP_MAXN);
				if(yp>=si->zoomys && yp<=zoomye) { yp=ye-(LONG)(yscl*(yp-si->zoomys));WritePixel(rp,optr,yp); }
				ptr=xs+(UWORD)((double)(off+i)*xscl);			// pixel coord.
				if(optr<ptr) optr=ptr;
			}
		}
		SADone(sa,si);
	}
}


void DrawSampleF(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register UWORD yrh,xh;
	register ULONG yp,pos,ptr;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	xh=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		xscl=(double)si->zoomxl/(double)si->xres;								      // Samples per Pixel
		yscl=(double)h/(double)(si->zoomyl+1);
		yrh=(ULONG)(SMP_MAX>>1);
		pos=ptr=off=0;
		if(yrh<si->zoomys) yrh=si->zoomys;
		else if(yrh>si->zoomys+si->zoomyl) yrh=si->zoomys+si->zoomyl;
		yrh=ye-(LONG)(yscl*(yrh-si->zoomys));
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		for(sa->seg=1;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			while((ptr-off)<Bytes2Samples(sa->blen)) {
				Move(rp,xh,yrh);
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
			off+=Bytes2Samples(sa->blen);
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running && (xh<xe)) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					   // just fetch the last block
			while(xh<xe) {
				Move(rp,xh,yrh);
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
		}
		SADone(sa,si);
	}
}

void DrawSampleFA(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register UWORD xh;
	register ULONG yp,pos,ptr;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	xh=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		xscl=(double)si->zoomxl/(double)si->xres;									      // Samples per Pixel
		yscl=(double)h/(double)(si->zoomyl+1);
		pos=ptr=off=0;
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		for(sa->seg=1;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			while((ptr-off)<Bytes2Samples(sa->blen)) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Move(rp,xh,ye);
				else if(yp>zoomye) Move(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Move(rp,xh,yp); }
				yp=(ULONG)(SMP_MAX-(buf[ptr-off]-SMP_MAXN));
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
			off+=Bytes2Samples(sa->blen);
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running && (xh<xe)) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					   // just fetch the last block
			while(xh<xe) {
				yp=(ULONG)(buf[ptr-off]-SMP_MAXN);
				if(yp<si->zoomys) Move(rp,xh,ye);
				else if(yp>zoomye) Move(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Move(rp,xh,yp); }
				yp=(ULONG)(SMP_MAX-(buf[ptr-off]-SMP_MAXN));
				if(yp<si->zoomys) Draw(rp,xh,ye);
				else if(yp>zoomye) Draw(rp,xh,ys);
				else { yp=ye-(LONG)(yscl*(yp-si->zoomys));Draw(rp,xh,yp); }
				xh++;pos++;ptr=(ULONG)((double)pos*xscl);
			}
		}
		SADone(sa,si);
	}
}

void DrawSampleFHQ(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
	register ULONG i;
	register UWORD optr,ptr=0;
	double yscl,xscl;
	ULONG xs,xe,ys,ye,off,zoomye=si->zoomys+si->zoomyl;
	SAMPLE *buf;
	SAFuncPtr SARead;
	SmpAccess *sa;
	LONG mi,ma,yp1,yp2;
	struct RastPort *rp=si->rp;
	char running=TRUE;

	//MSG3("zys/e/l %6d %6d %6d",si->zoomys,zoomye,si->zoomyl);

	optr=xs=si->xo;xe=xs+si->xres;
	ys=si->yo+y;ye=ys+h;
	if(!mode  || !si->quickdraw) DrawSampleExtra(si,channel,y,h);
	if((sa=SAInit(si,channel,si->zoomxs,si->zoomxl,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN))) {
		SARead=sa->safunc;
		yscl=(double)h/(double)(si->zoomyl+1);
		xscl=(double)si->xres/(double)si->zoomxl;
		SetAPen(rp,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		mi=ma=0;
		for(sa->seg=1,off=0;((sa->seg<sa->seganz) && (running==TRUE));sa->seg++,off+=Bytes2Samples(sa->blen)) {
			buf=(SAMPLE *)sa->buffer1;
			if(!SARead(sa,sa->blen)) { running=FALSE;break; }
			for(i=0;i<Bytes2Samples(sa->blen);i++) {
				if(*buf>ma) ma=*buf;
				else if(*buf<mi) mi=*buf;
				buf++;
				ptr=xs+(UWORD)((double)(off+i)*xscl);			// pixel coord.
				if(optr<ptr) {
					yp1=mi-SMP_MAXN;
					if(yp1<si->zoomys) yp1=ye;
					else yp1=ye-(LONG)(yscl*(yp1-si->zoomys));
				
					yp2=ma-SMP_MAXN;
					if(yp2>zoomye) yp2=ys;
					else yp2=ye-(LONG)(yscl*(yp2-si->zoomys));

					//if(yp1<yp2) MSG("!!! Barf !!!");

					RectFill(rp,optr,yp2,optr,yp1);
					mi=ma=0;
					optr=ptr;
				}
			}
			running=!TestForEvent(si->win,ABORTDRAW_IDCMPS);
		}
		if(running) {
			buf=(SAMPLE *)sa->buffer1;SARead(sa,0);					   // just fetch the last block
			for(i=0;ptr<(xe-1);i++)	{
				if(*buf>ma) ma=*buf;
				else if(*buf<mi) mi=*buf;
				buf++;
				ptr=xs+(UWORD)((double)(off+i)*xscl);			// pixel coord.
				if(optr<ptr) {
					yp1=mi-SMP_MAXN;
					if(yp1<si->zoomys) yp1=ye;
					else yp1=ye-(LONG)(yscl*(yp1-si->zoomys));
			
					yp2=ma-SMP_MAXN;
					if(yp2>zoomye) yp2=ys;
					else yp2=ye-(LONG)(yscl*(yp2-si->zoomys));
	
					RectFill(rp,optr,yp2,optr,yp1);
					mi=ma=0;
					optr=ptr;
				}
			}
		}
		SADone(sa,si);
	}
}

void DrawSampleFHQF(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
/* @TODO:
	wie FHQ
	+ diff zwischen cur und last
	+ für jeden block avg diff
	+ hohe diff -> hell, wenig diff dunkel : PEN_SMP_LINE_BRIGHT,PEN_SMP_LINE,PEN_SMP_LINE_BRIGHT
*/
}

void DrawSampleSpect(SInfo *si,UBYTE mode,UBYTE channel,ULONG y,ULONG h) {
/* Should I use WritePixelArray8 ? */
/*
	foreach position {
		*rbufw=*(sbuf++);										// get current value
		rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));

		fill source buffer

		rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+(Bytes2Samples(rbufsize)-mm))&rbufmask));
		flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt;
		for(o=0;o<=mm;o++) {
			*(flp1_r++)=(*(winfp++))*((double)*rbufr/32768.0);
			*(flp1_i++)=0.0;
			rbufr=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr+1)&rbufmask));
		}

		Transform(flbuf_r,flbuf_i,bitrevtab,sintab1,costab,-1,m);	// in Frequenzdarstellung transformieren

		flp1_r=&flbuf_r[1];flp1_i=&flbuf_i[1];						// 0 is DC-Offset
		for(o=0;o<m;o++) {
			//*(rasbuf++)=sqrt((*flp1_r)*(*(flp1_r++))+(*flp1_i)*(*(flp1_i++)));	// Bug in SAS ??
			*(rasbuf++)=sqrt((*flp1_r)*(*flp1_r)+(*flp1_i)*(*flp1_i));		// really for(o=1;o<=m;o++)
			flp1_r++;flp1_i++;
		}

		draw resampled line (to match height)
	}
*/	
}

//-- Markers, Ranges, etc.

void DrawLoop(SInfo *si) {
	if(si && SFX_SOUND_IS_SHOWN(si) && si->aktch) {
		register ULONG xp;
		UWORD x1,x2;
		ULONG xs,ys,xe,ye;
		UBYTE color;
		struct RastPort *rp=si->rp;

		xs=si->xo  ;xe=si->xo+si->xres;
		ys=si->yo+1;ye=si->yo+si->yres;
		color=sfxprefs_gui.Pens[PEN_SMP_LOOP]^sfxprefs_gui.Pens[PEN_SMP_BACK];
		SetABPenDrMd(rp,color,color,COMPLEMENT);
		SetWriteMask(rp,color);
		if(si->loops>=si->zoomxs && si->loops<=(si->zoomxs+si->zoomxl)) {
			xp=xs+(((si->loops-si->zoomxs)*si->xres)/si->zoomxl);
			Move(rp,xp,ys+5);Draw(rp,xp,ye);
			x1=xp;x1=max(xs,x1);x2=xp+6;x2=min(xe,x2);
			RectFill(rp,x1,ys,x2,ys+4);
		}
		if((si->loops+si->loopl)>=si->zoomxs && (si->loops+si->loopl)<=(si->zoomxs+si->zoomxl)) {
			xp=xs+((((si->loops+si->loopl)-si->zoomxs)*si->xres)/si->zoomxl);
			Move(rp,xp,ys+5);Draw(rp,xp,ye);
			x1=xp-6;x1=max(xs,x1);x2=xp;x2=min(xe,x2);
			RectFill(rp,x1,ys,x2,ys+4);
		}
		SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
	}
}

void DrawMark(SInfo *si) {
	if(si && SFX_SOUND_IS_SHOWN(si)) {
		register UBYTE l;
		UWORD x1,x2;
		UWORD y1,y2;
		double yscl;
		ULONG ys,yh;
		LONG zxs,zxe,zys,zye;
		LONG mxs,mxe,mys,mye;
		UBYTE color;
		struct RastPort *rp;

		yh=(si->yres-si->aktch)/si->aktch;

		zxs=si->zoomxs;zxe=zxs+si->zoomxl;
		zys=si->zoomys;zye=zys+si->zoomyl;
		mxs=si->markxs;mxe=mxs+si->markxl;
		mys=si->markys;mye=mys+si->markyl;

		yscl=(double)yh/(double)(si->zoomyl+1);

		if(mxs<=zxe && mxe>=zxs && mys<=zye && mye>=zys) {
			rp=si->rp;
			color=sfxprefs_gui.Pens[PEN_SMP_MARK]^sfxprefs_gui.Pens[PEN_SMP_BACK];
			SetABPenDrMd(rp,color,color,COMPLEMENT);
			SetWriteMask(rp,color);

			if(mxs>zxs)	x1=si->xo+(int)(((double)(mxs-zxs)*(double)si->xres)/(double)si->zoomxl);
			else x1=si->xo;
			if(mys>zys) y1=yh-(LONG)(yscl*((mys-zys)));
			else y1=yh;

			if(mxe<zxe)	x2=si->xo+(int)(((double)(mxe-zxs)*(double)si->xres)/(double)si->zoomxl);
			else x2=si->xo+si->xres;
			if(mye<zye) y2=yh-(LONG)(yscl*((mye-zys)));
			else y2=0;

			if(x1==x2) {
				if(x1>0) x1--;
				else x2++;
			}
			if(y1==y2) {
				if(y2>y1) y2--;
				else y1++;
			}

			ys=si->yo;
			for(l=0;l<si->aktch;l++,ys+=yh) RectFill(rp,x1,ys+y2,x2,ys+y1);		 // Draw Mark for each channel
			SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
		}
	}
}

void DrawCross(SInfo *si,UWORD xpos,UWORD ypos) {
	//it is only called from SFX-WinSample.c/Test_SampleWin() and this already takes care
	//if(si && SFX_SOUND_IS_SHOWN(si) && !(si->win->Flags&WFLG_MENUSTATE)) {
		struct RastPort *rp=si->rp;
		UBYTE color;
		color=sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]^sfxprefs_gui.Pens[PEN_SMP_BACK];
		SetABPenDrMd(rp,color,color,COMPLEMENT);SetWriteMask(rp,color);
		Move(rp,si->xo,si->yo+ypos);Draw(rp,si->xo+si->xres,si->yo+ypos);
		Move(rp,si->xo+xpos,si->yo);Draw(rp,si->xo+xpos,si->yo+si->yres);
		SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
	//}
}

void DrawPPos(SInfo *si,ULONG pos) {
	if(si && SFX_SOUND_IS_SHOWN(si) && si->aktch) {
		register LONG xp=pos-si->zoomxs;
		if(xp>0 && xp<=si->zoomxl) {
			struct RastPort *rp=si->rp;
			UBYTE color=sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]^sfxprefs_gui.Pens[PEN_SMP_BACK];

			xp=si->xo+(LONG)(((double)xp*(double)si->xres)/(double)si->zoomxl);
			SetABPenDrMd(rp,color,color,COMPLEMENT);SetWriteMask(rp,color);
			Move(rp,xp,si->yo);Draw(rp,xp,si->yo+si->yres);
			SetWriteMask(rp,0xFF);SetDrMd(rp,JAM1);
		}
	}
}

//-- Axis drawing

void DrawAxisX(SInfo *si) {
	struct RastPort *rp=si->rp;
	register UBYTE i;
	double xsta,xste,xscl,srat;
	WORD xp,xps,xpl,xe,xpe;
	UBYTE anz,ranz,off,xw,sl;
	ULONG val1,val2,sh,skip,rskip,start,xval;
	UBYTE sks[]={ 1,2,5 };		// Teiler von 10
	char *xstr;

	//INTRO;

	off=RunTime.scafont->tf_Baseline+1;
	xw=RunTime.scafont->tf_XSize;
	srat=(double)si->srat;
	//SHOW_FLT(srat);
	//SHOW_WORD(off);
	anz=(UBYTE)((si->xres/RunTime.scafont->tf_XSize)/(strlen(ToXUnit(si->unitx,si->zoomxs+si->zoomxl,si->srat))+1)); // wieviele Texte von der Länge der Endmarke würden hinpassen
	if(!anz) anz=1;
	switch(si->unitx) {				 // Bereichslänge in der kleinsten Einheit
		case ATME_H_M_S_MS:
			val1=(ULONG)((double)si->zoomxl/(srat/1000.0));
			val2=(ULONG)((double)si->zoomxs/(srat/1000.0));
			break;
		case ATME_H_M_S_MF:
			val1=(ULONG)((double)si->zoomxl/(srat/24.0));
			val2=(ULONG)((double)si->zoomxs/(srat/24.0));
			break;
		case ATME_H_M_S_PF:
			val1=(ULONG)((double)si->zoomxl/(srat/25.0));
			val2=(ULONG)((double)si->zoomxs/(srat/25.0));
			break;
		case ATME_H_M_S_NF:
			val1=(ULONG)((double)si->zoomxl/(srat/30.0));
			val2=(ULONG)((double)si->zoomxs/(srat/30.0));
			break;
		case ATME_SAMPLES:
			val1=si->zoomxl;
			val2=si->zoomxs;
			break;
		UNEXPECTED_DEFAULT
	}

	//SHOW_LONG(val1);SHOW_LONG(val2);
	
	skip=val1/anz;
	sh=1;rskip=0;					// Sprungweite ermitteln
	while(rskip<=skip) {
		for(i=0;i<3;i++) {
			rskip=sh*sks[i];
			if(rskip>skip) i=4;
		}
		sh*=10;
	}
	ranz=val1/rskip;

	//SHOW_BYTE(anz);SHOW_BYTE(ranz);
	//SHOW_LONG(skip);SHOW_LONG(rskip);

	start=val2/rskip;
	if(start*rskip<val2) start++;
	start*=rskip;

	//SHOW_LONG(start);

	switch(si->unitx) {				 // kleinsten Einheit in Sample umrechnen
		case ATME_SAMPLES:
			xsta=(double)start;
			xste=(double)rskip;
			break;
		case ATME_H_M_S_MS:
			xsta=(double)start*(srat/1000.0);
			xste=(double)rskip*(srat/1000.0);
			break;
		case ATME_H_M_S_MF:
			xsta=(double)start*(srat/24.0);
			xste=(double)rskip*(srat/24.0);
			break;
		case ATME_H_M_S_PF:
			xsta=(double)start*(srat/25.0);
			xste=(double)rskip*(srat/25.0);
			break;
		case ATME_H_M_S_NF:
			xsta=(double)start*(srat/30.0);
			xste=(double)rskip*(srat/30.0);
			break;
		UNEXPECTED_DEFAULT
	}

	//SHOW_FLT(xsta);
	//SHOW_FLT(xste);

	if((xsta+(ranz*xste))<(si->zoomxs+si->zoomxl)) ranz++;

	xscl=(double)si->xres/(double)si->zoomxl;	      // calculate to avoid overflows

	SetABPenDrMd(rp,0,0,JAM1);
	RectFill(rp,si->win->BorderLeft,si->win->BorderTop,si->xo+si->xres,si->yo-1);
	SetAPen(rp,1);SetFont(rp,RunTime.scafont);
	xe=si->xo+si->xres;xpe=0;
	for(i=0;i<ranz;i++) {
		xval=(ULONG)(xsta+((double)i*xste));
		xp=si->xo+(UWORD)((double)(xval-si->zoomxs)*xscl);
		Move(rp,xp,si->yo-5);Draw(rp,xp,si->yo-1);

		xstr=ToXUnit(si->unitx,(ULONG)xval,si->srat);sl=strlen(xstr);
		xpl=sl*xw;xps=xp-(xpl>>1);
		if(xps<si->win->BorderLeft) {
			Move(rp,si->win->BorderLeft,si->win->BorderTop+off);
			Text(rp,xstr,sl);
		}
		else if((xps+xpl)>xe) {
			if(xe-xpl>xpe) {
				Move(rp,xe-xpl,si->win->BorderTop+off);
				Text(rp,xstr,sl);
			}
		}
		else {
			Move(rp,xps,si->win->BorderTop+off);xpe=xps+xpl+2;
			Text(rp,xstr,sl);
		}
	}
	SetFont(rp,RunTime.scrfont);

	//OUTRO;
}

void DrawAxisY(SInfo *si) {
	struct RastPort *rp=si->rp;
	register UBYTE i,l;
	UWORD ml,nl,off;
	LONG yval;
	ULONG yp,ys,ye,yd,yl,yt,h;
	double yscl;
	char *str;

	h=1+((si->yres-si->aktch)/si->aktch);
	ys=si->yo;
	yscl=(double)h/(double)(si->zoomyl+1);

	off=RunTime.scafont->tf_Baseline>>1;
	// maxlines displayable (with some space inbetween)
	ml=1+(h/(3+RunTime.scafont->tf_YSize)); //ml=1+(h/(3+RunTime.scy[6]));
	//MSG1("AxisY :     ml=%d",ml);
	yd=yl=((LONG)si->zoomyl+1);
	//MSG1("AxisY : yval=%6d",yval);
	//MSG1("AxisY :     yd=%d",yd);
	//MSG1("AxisY : zoomys=%d",si->zoomys);
	//MSG1("AxisY : zoomyl=%d",si->zoomyl);
	if(ml>1) {
		//MSG("stepsize loop 1 ---------");
		nl=1+(yl/yd);			// needed lines
		//MSG1("AxisY :     yd=%d",yd);
		//MSG1("AxisY :     nl=%d",nl);
		while((nl<=ml) && yd>1) {
			yd>>=1;nl=1+(yl/yd);
			//MSG1("AxisY :     yd=%d",yd);
			//MSG1("AxisY :     nl=%d",nl);
		}
		if(nl>ml) { yd<<=1;nl=1+(yl/yd); }
		//MSG1("AxisY :     yd=%d",yd);
		//MSG1("AxisY :     nl=%d",nl);
	}
	else nl=1;
	//MSG("done---------------------");

	SetAPen(rp,0);RectFill(rp,si->win->BorderLeft,si->yo,si->xo-1,si->yo+si->yres);
	SetAPen(rp,1);SetFont(rp,RunTime.scafont);
	for(l=0;l<si->aktch;l++) {
		yval=(LONG)(si->zoomys+SMP_MAXN);
		ye=ys+h;
		for(i=0;i<nl;i++) {
			yp=ye-(LONG)(yscl*(((ULONG)(yval-SMP_MAXN))-si->zoomys));
			Move(rp,si->xo-5,yp);Draw(rp,si->xo-1,yp);
			//MSG3("%d %d %d",yp,ys,ye);
			//MSG1("%d",off);
			/*
			if((yp+off)>=(ye-1)) Move(rp,si->win->BorderLeft+2,(ye-1));
			else if(yp<=(ys+off+3)) Move(rp,si->win->BorderLeft+2,(ys+off+3));
			else Move(rp,si->win->BorderLeft+2,yp+off);
			Text(rp,ToYUnit(si->unity,yval),6);
			*/
			if((yp+off)>=(ye-1)) yt=ye-1;
			else if(yp<=(ys+off+3)) yt=ys+off+3;
			else yt=yp+off;
			str=ToYUnit(si->unity,yval);str[6]='\0';
			RText(rp,str,si->xo-6,yt);
			yval+=yd;
		}
		//ys=ye+1;
		ys=ye;
	}
	SetFont(rp,RunTime.scrfont);
}

//-- Event Tester

char TestForEvent(struct Window *win,ULONG mask) {
	char found=FALSE;
	struct List *list=&(win->UserPort->mp_MsgList);
	struct Node *imsg;

	Forbid();
	foreach(imsg,list) {
		if((((struct IntuiMessage *)imsg)->IDCMPWindow==win) && (((struct IntuiMessage *)imsg)->Class&mask)) {
			if(((struct IntuiMessage *)imsg)->Class==IDCMP_RAWKEY) {  // only check for key-down events
				if(((struct IntuiMessage *)imsg)->Code<128) { found=TRUE;break; }
			}
			else { found=TRUE;break; }
		}
	}
	Permit();
	return(found);
}

//-- eof ----------------------------------------------------------------------
