/****h* SoundFX/SFX-Range.c [4.2] *
*
*  NAME
*    SFX-Range.c
*  COPYRIGHT
*    $VER: SFX-Range.c 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Range functions - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*    02.Sep.2000	V 4.1	most recent version
*							* added Range_Mark_#? functions
*    16.Jul.2000	V 4.00	most recent version
*							* added Range_#?2#? functions
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_Range_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void RangeX1Beg(SInfo *entry);
void RangeX2End(SInfo *entry);
void RangeXZero(SInfo *entry,BYTE dir);

void RangeY2Top(SInfo *entry);
void RangeY1Bot(SInfo *entry);
void RangeYPeak(SInfo *entry,BYTE dir);

void Range_Loop2Mark(SInfo *entry);
void Range_Mark2Loop(SInfo *entry);
void Range_Zoom2Mark(SInfo *entry);
void Range_Zoom2Loop(SInfo *entry);
void Range_MarkOff(SInfo *entry);
void Range_MarkAll(SInfo *entry);

//-- definitions --------------------------------------------------------------

void RangeX1Beg(SInfo *entry) {
	if(RunTime.rnglock) {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP: if(entry->loop) { DrawLoop(entry);entry->loopl +=entry->loops ;entry->loops =0;DrawLoop(entry);RecalcLoopPlayData(); } break;
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markxl+=entry->markxs;entry->markxs=0;DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomxl+=entry->zoomxs;entry->zoomxs=0;DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	else {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP: if(entry->loop) { DrawLoop(entry);entry->loops =0;DrawLoop(entry);RecalcLoopPlayData(); } break;
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markxs=0;DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomxs=0;DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	Refr_StatusRange();
}

void RangeX2End(SInfo *entry) {
	if(RunTime.rnglock) {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP: if(entry->loop) { DrawLoop(entry);entry->loopl +=entry->slen-(entry->loops +entry->loopl );DrawLoop(entry);RecalcLoopPlayData(); } break;
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markxl+=entry->slen-(entry->markxs+entry->markxl);DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomxl+=entry->slen-(entry->zoomxs+entry->zoomxl);DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	else {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP: if(entry->loop) { DrawLoop(entry);entry->loops =(entry->slen-entry->loopl );DrawLoop(entry);RecalcLoopPlayData(); } break;
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markxs=(entry->slen-entry->markxl);DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomxs=(entry->slen-entry->zoomxl);DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	Refr_StatusRange();
}

void RangeXZero(SInfo *entry,BYTE dir) {
	LONG lpos;
	
	if(RunTime.rnglock) {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP:
				if(entry->loop) {
					if(RunTime.movemode) {
						lpos=ZeroSearch(entry,entry->loops+entry->loopl,dir,0,0)-(LONG)entry->loops;
						if(lpos>=0) { DrawLoop(entry);entry->loopl=lpos;DrawLoop(entry);RecalcLoopPlayData(); }
					}
					else {
						lpos=ZeroSearch(entry,entry->loops,dir,0,0);
						if((LONG)entry->loopl+((LONG)entry->loops-lpos)>=0) {
							DrawLoop(entry);entry->loopl+=(entry->loops-lpos);entry->loops=lpos;DrawLoop(entry);RecalcLoopPlayData();
						}
					}
				}
				break;
			case RNGMD_MARK:
				if(entry->mark) {
					if(RunTime.movemode) {
						lpos=ZeroSearch(entry,entry->markxs+entry->markxl,dir,0,0)-(LONG)entry->markxs;
						if(lpos>=0) { DrawMark(entry);entry->markxl=lpos;DrawMark(entry); }
					}
					else {
						lpos=ZeroSearch(entry,entry->markxs,dir,0,0);
						if((LONG)entry->markxl+((LONG)entry->markxs-lpos)>=0) {
							DrawMark(entry);entry->markxl+=(entry->markxs-lpos);entry->markxs=lpos;DrawMark(entry);
						}
					}
				}
				break;
			case RNGMD_ZOOM:
				if(RunTime.movemode) {
					lpos=ZeroSearch(entry,entry->zoomxs+entry->zoomxl,dir,0,0)-(LONG)entry->zoomxs;
					if(lpos>=0) { entry->zoomxl=lpos;DrawSample(entry,0);SetSlider(entry,1); }
				}
				else {
					lpos=ZeroSearch(entry,entry->zoomxs,dir,0,0);
					if((LONG)entry->zoomxl+((LONG)entry->zoomxs-lpos)>=0) {
						entry->zoomxl+=(entry->zoomxs-lpos);entry->zoomxs=lpos;DrawSample(entry,0);SetSlider(entry,1);
					}
				}
				break;
		}
	}
	else {
		switch(RunTime.rngmode) {
			case RNGMD_LOOP:
				if(entry->loop) {
					DrawLoop(entry);
					if(RunTime.movemode) { lpos=ZeroSearch(entry,entry->loops+entry->loopl,dir,0,0);entry->loops+=(lpos-(entry->loops+entry->loopl)); }
					else { entry->loops=ZeroSearch(entry,entry->loops,dir,0,0); }
					DrawLoop(entry);RecalcLoopPlayData();
				}
				break;
			case RNGMD_MARK:
				if(entry->mark) {
					DrawMark(entry);
					if(RunTime.movemode) { lpos=ZeroSearch(entry,entry->markxs+entry->markxl,dir,0,0);entry->markxs+=(lpos-(entry->markxs+entry->markxl)); }
					else { entry->markxs=ZeroSearch(entry,entry->markxs,dir,0,0); }
					DrawMark(entry);
				}
				break;
			case RNGMD_ZOOM:
				if(RunTime.movemode) { lpos=ZeroSearch(entry,entry->zoomxs+entry->zoomxl,dir,0,0);entry->zoomxs+=(lpos-(entry->zoomxs+entry->zoomxl)); }
				else { entry->zoomxs=ZeroSearch(entry,entry->zoomxs,dir,0,0); }
				DrawSample(entry,0);SetSlider(entry,1);
				break;
		}
	}
	Refr_StatusRange();
}

void RangeY2Top(SInfo *entry) {
	if(RunTime.rnglock) {
		switch(RunTime.rngmode) {
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markyl+=SMP_MAX-(entry->markys+entry->markyl);DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomyl+=SMP_MAX-(entry->zoomys+entry->zoomyl);DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	else {
		switch(RunTime.rngmode) {
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markys=(SMP_MAX-entry->markyl);DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomys=(SMP_MAX-entry->zoomyl);DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	Refr_StatusRange();
}

void RangeY1Bot(SInfo *entry) {
	if(RunTime.rnglock) {
		switch(RunTime.rngmode) {
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markyl+=entry->markys;entry->markys=0;DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomyl+=entry->zoomys;entry->zoomys=0;DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	else {
		switch(RunTime.rngmode) {
			case RNGMD_MARK: if(entry->mark) { DrawMark(entry);entry->markys=0;DrawMark(entry); } break;
			case RNGMD_ZOOM:								   entry->zoomys=0;DrawSample(entry,0);SetSlider(entry,1);break;
		}
	}
	Refr_StatusRange();
}

void RangeYPeak(SInfo *entry,BYTE dir) {
	LONG lpos;
	
	if(RunTime.rnglock) {							// keep the other end fixed
		switch(RunTime.rngmode) {
			case RNGMD_MARK:
				if(entry->mark) {
					lpos=PeakSearch(entry,entry->markxs,entry->markxl,dir)-SMP_MAXN;
					DrawMark(entry);
					if(dir==1) entry->markyl+=(lpos-(LONG)(entry->markys+entry->markyl));
					else { entry->markyl+=((LONG)entry->markys-lpos);entry->markys=lpos; }
					DrawMark(entry);
				}
				break;
			case RNGMD_ZOOM:
				lpos=PeakSearch(entry,entry->zoomxs,entry->zoomxl,dir)-SMP_MAXN;
				if(dir==1) entry->zoomyl+=(lpos-(LONG)(entry->zoomys+entry->zoomyl));
				else { entry->zoomyl+=((LONG)entry->zoomys-lpos);entry->zoomys=lpos; }
				DrawSample(entry,0);SetSlider(entry,1);
				break;
		}
	}
	else {											// keep the length fixed
		switch(RunTime.rngmode) {
			case RNGMD_MARK:
				if(entry->mark) {
					lpos=PeakSearch(entry,entry->markxs,entry->markxl,dir)-SMP_MAXN;
					DrawMark(entry);
					if(dir==1) entry->markys+=(lpos-(LONG)(entry->markys+entry->markyl));
					else entry->markys=lpos;
					DrawMark(entry);
				}
				break;
			case RNGMD_ZOOM:
				lpos=PeakSearch(entry,entry->zoomxs,entry->zoomxl,dir)-SMP_MAXN;
				if(dir==1) entry->zoomys+=(lpos-(LONG)(entry->zoomys+entry->zoomyl));
				else entry->zoomys=lpos;
				DrawSample(entry,0);SetSlider(entry,1);
				break;
		}
	}
	Refr_StatusRange();
}

void Range_Loop2Mark(SInfo *entry)
{
	if(entry->mark) DrawMark(entry);
	entry->markxs=entry->loops;
	entry->markxl=entry->loopl;
	entry->mark=1;
	DrawMark(entry);
}

void Range_Mark2Loop(SInfo *entry)
{
	if(entry->loop) DrawLoop(entry);
	entry->loops=entry->markxs;
	entry->loopl=entry->markxl;
	entry->loop=1;
	DrawLoop(entry);RecalcLoopPlayData();
	Refr_ContextMenu();

}

void Range_Zoom2Loop(SInfo *entry)
{
	if(entry->loop) DrawLoop(entry);
	entry->loops=entry->zoomxs;
	entry->loopl=entry->zoomxl;
	entry->loop=1;
	DrawLoop(entry);RecalcLoopPlayData();
	Refr_ContextMenu();
}

void Range_Zoom2Mark(SInfo *entry)
{
	if(entry->mark) DrawMark(entry);
	entry->markxs=entry->zoomxs;
	entry->markxl=entry->zoomxl;
	entry->mark=1;
	DrawMark(entry);
}

void Range_MarkOff(SInfo *entry)
{
	DrawMark(entry);
	entry->markxl=entry->markyl=0;
	entry->mark=0;
}

void Range_MarkAll(SInfo *entry)
{
	if(entry->mark) DrawMark(entry);
	entry->markxs=0;entry->markxl=entry->slen;
	entry->markys=0;entry->markyl=SMP_MAX;
	entry->mark=1;
	DrawMark(entry);
}

//-- eof ----------------------------------------------------------------------
