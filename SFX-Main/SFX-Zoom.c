/****h* SoundFX/SFX-Zoom.c [4.3] *
*
*  NAME
*    SFX-Zoom.c
*  COPYRIGHT
*    $VER: SFX-Zoom.c 4.3 (03.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Zoom functions - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    03.Mar.2004	V 4.3	most recent version
*    08.Jul.2003	V 4.2	most recent version
*    25.Jun.2000	V 4.00	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_Zoom_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void ZoomIn(SInfo *entry,UBYTE mode);
void ZoomOut(SInfo *entry,UBYTE mode);
void ZoomPixel(SInfo *entry,UBYTE mode);
void ZoomFull(SInfo *entry,UBYTE mode);

//-- private

void __inline AfterZoom(SInfo *entry,UBYTE mode,UBYTE change);

//-- definitions --------------------------------------------------------------

void ZoomIn(SInfo *entry,UBYTE mode)
{
	UBYTE change=FALSE;

	if(mode&ZOOMMD_X) {
		if(entry->mark && entry->markxl>1) {
			entry->zoomxs=entry->markxs;entry->zoomxl=entry->markxl;
			entry->markxs=entry->markxl=0L;
			change=TRUE;
		}
		else {
			if(entry->zoomxl>1) {
				entry->zoomxs+=(entry->zoomxl>>2);
				entry->zoomxl=(entry->zoomxl>>1);
				change=TRUE;
			}
		}
	}
	if(mode&ZOOMMD_Y) {
		if(entry->mark && entry->markyl>1) {
			entry->zoomys=entry->markys;entry->zoomyl=entry->markyl;
			entry->markys=entry->markyl=0L;
			change=TRUE;
		}
		else {
			if(entry->zoomyl>1) {
				entry->zoomys+=(entry->zoomyl>>2);
				entry->zoomyl=(entry->zoomyl>>1);
				change=TRUE;
			}
		}
	}
	AfterZoom(entry,mode,change);
}

void ZoomOut(SInfo *entry,UBYTE mode)
{
	UBYTE change=FALSE;
	ULONG h;

	if(mode&ZOOMMD_X) {
		if(entry->zoomxs>0 || entry->zoomxl<entry->slen) {
			h=entry->zoomxl>>1;
			if(entry->zoomxs>=h) entry->zoomxs-=h;
			else entry->zoomxs=0;
			entry->zoomxl<<=1;
			if(entry->zoomxs+entry->zoomxl>entry->slen) {
				h=(entry->zoomxs+entry->zoomxl)-entry->slen;
				if(entry->zoomxs>h) entry->zoomxs-=h;
				else {
					entry->zoomxs=0;
					entry->zoomxl=entry->slen;
				}
			}
			entry->markxs=0;
			entry->markxl=0;
			change=TRUE;
		}
	}
	if(mode&ZOOMMD_Y) {
		if(entry->zoomys>0 || entry->zoomyl<65535) {
			h=entry->zoomyl>>1;
			if(entry->zoomys>=h) entry->zoomys-=h;
			else entry->zoomys=0;
			entry->zoomyl<<=1;
			if(entry->zoomys+entry->zoomyl>65535) {
				h=(entry->zoomys+entry->zoomyl)-65355;
				if(entry->zoomys>h) entry->zoomys-=h;
				else {
					entry->zoomys=0;
					entry->zoomyl=SMP_MAX-1;			/* 65535 for 16bit */
				}
			}
			entry->markys=0;
			entry->markyl=0;
			change=TRUE;
		}
	}
	AfterZoom(entry,mode,change);
}

void ZoomPixel(SInfo *entry,UBYTE mode)
{
	UBYTE change=FALSE;

	if(mode&ZOOMMD_X) {
		if(entry->slen>=entry->xres) {
			entry->zoomxl=entry->xres;
			if(entry->zoomxs+entry->zoomxl>entry->slen) entry->zoomxs-=(entry->slen-(entry->zoomxs+entry->zoomxl));
			change=TRUE;
		}
		else {
			if(entry->zoomxs!=0 || entry->zoomxl!=entry->slen) {
				entry->zoomxs=0;
				entry->zoomxl=entry->slen;
				change=TRUE;
			}
		}
	}
	if(mode&ZOOMMD_Y) {
		if(entry->zoomyl!=entry->yres) {
			entry->zoomyl=entry->yres;
			change=TRUE;
		}
	}
	AfterZoom(entry,mode,change);
}

void ZoomFull(SInfo *entry,UBYTE mode)
{
	UBYTE change=FALSE;

	if(mode&ZOOMMD_X) {
		if(entry->zoomxs!=0 || entry->zoomxl!=entry->slen) {
			entry->zoomxs=0;
			entry->zoomxl=entry->slen;
			change=TRUE;
		}
	}
	if(mode&ZOOMMD_Y) {
		if(entry->zoomys!=0 || entry->zoomyl!=SMP_MAX-1) {
			entry->zoomys=0;
			entry->zoomyl=SMP_MAX-1;			/* 65535 for 16bit */
			change=TRUE;
		}
	}
	AfterZoom(entry,mode,change);
}

//-- privat

void __inline AfterZoom(SInfo *entry,UBYTE mode,UBYTE change) {
	if(change) {
		entry->mark=FALSE;
		DrawSample(entry,0);
		SetSlider(entry,mode);
	}
	if(RunTime.rngmode!=RNGMD_ZOOM) {
		RunTime.rngmode=RNGMD_ZOOM;
		Refr_ToolbarRange();
		//Refr_StatusRange();	// will be called from Refr_ToolbarRange as well
	}
	else {
		Refr_StatusRange();
	}
}

//-- eof ----------------------------------------------------------------------
