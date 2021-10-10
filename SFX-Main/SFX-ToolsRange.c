/****h*SoundFX/SFX-ToolsRange.c [4.3] *
*
*  NAME
*    SFX-ToolsRange.c
*  COPYRIGHT
*    $VER: SFX-ToolsRange.c 4.3 (03.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    range procession tools - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    03.Mar.2004	V 4.3	most recent version
*    01.Aug.2002	V 4.2	most recent version
*    12.Jun.2000	V 4.00	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_ToolsRange_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void NewRangeStart (ULONG numNewStart ,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);
void NewRangeEnd   (ULONG numNewEnd   ,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);
void NewRangeLength(ULONG numNewLength,ULONG numUpperBound,ULONG *numStart,ULONG *numLength);

void RangeMoveHorizontal(void);
void RangeMoveVertical(void);

//-- definitions --------------------------------------------------------------

void NewRangeStart(ULONG numNewStart,ULONG numUpperBound,ULONG *numStart,ULONG *numLength) {
	MSG2("newStart=%ld : upperBound=%ld",numNewStart,numUpperBound);
	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);

	if(numNewStart>=numUpperBound) { *numStart=numUpperBound-1;*numLength=1; }
	else {
		if(RunTime.rnglock) {	// keep other end at fixed position
			if(numNewStart>=*numStart+*numLength) { *numLength=1;*numStart=numNewStart; }
			else { *numLength-=numNewStart-*numStart;*numStart=numNewStart; }
		}
		else {              	// keep length constant
			*numStart=numNewStart;
			if(*numStart+*numLength>=numUpperBound) *numStart=numUpperBound-*numLength;
		}
	}

	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);
	ASSERT(*numStart+*numLength<=numUpperBound);
}

void NewRangeEnd(ULONG numNewEnd,ULONG numUpperBound,ULONG *numStart,ULONG *numLength) {
	MSG2("newEnd=%ld : upperBound=%ld",numNewEnd,numUpperBound);
	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);

	if(numNewEnd>=numUpperBound) numNewEnd=numUpperBound;
	if(RunTime.rnglock) {	 // keep other end at fixed position
		if(numNewEnd<=*numStart) { *numLength=1;*numStart=numNewEnd; }
		else *numLength=numNewEnd-*numStart;
	}
	else {				  	// keep length constant
		if(numNewEnd>=*numLength) *numStart=numNewEnd-*numLength;
		else { *numStart=0;*numLength=numNewEnd; }
	}

	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);
	ASSERT(*numStart+*numLength<=numUpperBound);
}

void NewRangeLength(ULONG numNewLength,ULONG numUpperBound,ULONG *numStart,ULONG *numLength) {
	MSG2("newLength=%ld : upperBound=%ld",numNewLength,numUpperBound);
	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);

	if(*numStart+numNewLength>=numUpperBound) *numLength=numUpperBound-*numStart;
	else *numLength=numNewLength;

	MSG2("Start=%ld : Length=%ld",*numStart,*numLength);
	ASSERT(*numStart+*numLength<=numUpperBound);
}

void RangeMoveHorizontal(void)
{
	LONG lpos,llen;

	if(RunTime.movemode) {
		if(RunTime.rnglock) {
			switch(RunTime.rngmode) {
				case RNGMD_LOOP:
					if(RunTime.aktbuf->loop) {
						llen=(LONG)RunTime.aktbuf->loopl+RunTime.rngdiff;if(llen<0) llen=0;
						if(RunTime.aktbuf->loops+llen>RunTime.aktbuf->slen) llen=RunTime.aktbuf->slen-RunTime.aktbuf->loops;
						DrawLoop(RunTime.aktbuf);
						RunTime.aktbuf->loopl=llen;
						DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
					}
					break;
				case RNGMD_MARK:
					llen=(LONG)RunTime.aktbuf->markxl+RunTime.rngdiff;if(llen<0) llen=0;
					if(RunTime.aktbuf->markxs+llen>RunTime.aktbuf->slen) llen=RunTime.aktbuf->slen-RunTime.aktbuf->markxs;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markxl=llen;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					llen=(LONG)RunTime.aktbuf->zoomxl+RunTime.rngdiff;if(llen<0) llen=0;
					if(RunTime.aktbuf->zoomxs+llen>RunTime.aktbuf->slen) llen=RunTime.aktbuf->slen-RunTime.aktbuf->zoomxs;
					RunTime.aktbuf->zoomxl=llen;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
		else {
			switch(RunTime.rngmode)	{
				case RNGMD_LOOP:
					if(RunTime.aktbuf->loop) {
						lpos=(LONG)RunTime.aktbuf->loops+RunTime.rngdiff;if(lpos<0) lpos=0;
						if(lpos+RunTime.aktbuf->loopl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->loopl;
						DrawLoop(RunTime.aktbuf);
						RunTime.aktbuf->loops=lpos;
						DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
					}
					break;
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->markxl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->markxl;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markxs=lpos;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->zoomxl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->zoomxl;
					RunTime.aktbuf->zoomxs=lpos;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
	}
	else {
		if(RunTime.rnglock)	{
			switch(RunTime.rngmode)
			{
				case RNGMD_LOOP:
					if(RunTime.aktbuf->loop) {
						lpos=(LONG)RunTime.aktbuf->loops+RunTime.rngdiff;if(lpos<0) lpos=0;
						llen=RunTime.aktbuf->loops-lpos;if((LONG)RunTime.aktbuf->loopl+llen<0) { lpos+=llen;llen=0; }
						DrawLoop(RunTime.aktbuf);
						RunTime.aktbuf->loopl+=llen;RunTime.aktbuf->loops=lpos;
						DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
					}
					break;
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					llen=RunTime.aktbuf->markxs-lpos;if((LONG)RunTime.aktbuf->markxl+llen<0) { lpos+=llen;llen=0; }
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markxs=lpos;RunTime.aktbuf->markxl+=llen;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					llen=RunTime.aktbuf->zoomxs-lpos;if((LONG)RunTime.aktbuf->zoomxl+llen<0) { lpos+=llen;llen=0; }
					RunTime.aktbuf->zoomxs=lpos;RunTime.aktbuf->zoomxl+=llen;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
				break;
			}
		}
		else {
			switch(RunTime.rngmode)	{
				case RNGMD_LOOP:
					if(RunTime.aktbuf->loop) {
						lpos=(LONG)RunTime.aktbuf->loops+RunTime.rngdiff;if(lpos<0) lpos=0;
						if(lpos+RunTime.aktbuf->loopl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->loopl;
						DrawLoop(RunTime.aktbuf);
						RunTime.aktbuf->loops=lpos;
						DrawLoop(RunTime.aktbuf);RecalcLoopPlayData();
					}
					break;
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->markxl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->markxl;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markxs=lpos;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomxs+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->zoomxl>RunTime.aktbuf->slen) lpos=RunTime.aktbuf->slen-RunTime.aktbuf->zoomxl;
					RunTime.aktbuf->zoomxs=lpos;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
	}
	Refr_StatusRange();
}

void RangeMoveVertical(void)
{
	LONG lpos,llen;

	if(RunTime.movemode) {
		if(RunTime.rnglock) {
			switch(RunTime.rngmode) {
				case RNGMD_MARK:
					llen=(LONG)RunTime.aktbuf->markyl+RunTime.rngdiff;if(llen<0) llen=0;
					if(RunTime.aktbuf->markys+llen>SMP_MAX) llen=SMP_MAX-RunTime.aktbuf->markys;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markyl=llen;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					llen=(LONG)RunTime.aktbuf->zoomyl+RunTime.rngdiff;if(llen<0) llen=0;
					if(RunTime.aktbuf->zoomys+llen>SMP_MAX) llen=SMP_MAX-RunTime.aktbuf->zoomys;
					RunTime.aktbuf->zoomyl=llen;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
		else {
			switch(RunTime.rngmode)	{
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markys+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->markyl>SMP_MAX) lpos=SMP_MAX-RunTime.aktbuf->markyl;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markys=lpos;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomys+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->zoomyl>SMP_MAX) lpos=SMP_MAX-RunTime.aktbuf->zoomyl;
					RunTime.aktbuf->zoomys=lpos;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
	}
	else {
		if(RunTime.rnglock)	{
			switch(RunTime.rngmode)	{
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markys+RunTime.rngdiff;if(lpos<0) lpos=0;
					llen=RunTime.aktbuf->markys-lpos;if((LONG)RunTime.aktbuf->markyl+llen<0) { lpos+=llen;llen=0; }
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markys=lpos;RunTime.aktbuf->markyl+=llen;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomys+RunTime.rngdiff;if(lpos<0) lpos=0;
					llen=RunTime.aktbuf->zoomys-lpos;if((LONG)RunTime.aktbuf->zoomyl+llen<0) { lpos+=llen;llen=0; }
					RunTime.aktbuf->zoomys=lpos;RunTime.aktbuf->zoomyl+=llen;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
				break;
			}
		}
		else {
			switch(RunTime.rngmode)	{
				case RNGMD_MARK:
					lpos=(LONG)RunTime.aktbuf->markys+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->markyl>SMP_MAX) lpos=SMP_MAX-RunTime.aktbuf->markyl;
					DrawMark(RunTime.aktbuf);
					RunTime.aktbuf->markys=lpos;
					DrawMark(RunTime.aktbuf);
					break;
				case RNGMD_ZOOM:
					lpos=(LONG)RunTime.aktbuf->zoomys+RunTime.rngdiff;if(lpos<0) lpos=0;
					if(lpos+RunTime.aktbuf->zoomyl>SMP_MAX) lpos=SMP_MAX-RunTime.aktbuf->zoomyl;
					RunTime.aktbuf->zoomys=lpos;
					DrawSample(RunTime.aktbuf,1);SetSlider(RunTime.aktbuf,1);
					break;
			}
		}
	}
	Refr_StatusRange();
}

//-- eof ----------------------------------------------------------------------
