/****h* SoundFX/SFX-Play.c [4.3] *
*
*  NAME
*    SFX-Play.c
*  COPYRIGHT
*    $VER: SFX-Play.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    sample player interface - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    16.Jul.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    30.Oct.2002	V 4.2	most recent version
*    14.Jun.2000	V 4.00	most recent version
*    25.Jan.2000	V 3.82	most recent version
*    13.Aug.1998	V 3.70	most recent version
*    16.Jul.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_Play_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#define CHIPREG_DEFS
#include "SFX-ChipRegDefs.h"
#undef  CHIPREG_DEFS

//-- prototypes ---------------------------------------------------------------

void PlaySample_All(void);
void PlaySample_Rng(void);
void StopSample(void);

//-- helper for internal paula players

UBYTE InitPaulaPlayers(void);
void FreePaulaPlayers(void);

//-- helper for all players

void SetPlayRange(void);
void SetPlayStatus(void);

//-- globals ------------------------------------------------------------------

struct Interrupt *audint=NULL,*oldaudint=NULL;
char *intname="SFX-SamplePlayer";
UWORD *plbufs0,*plbufs1,*plbufs2,*plbufs3;		// obsolete soon
UWORD sv,v2,s2;
UWORD *sbuf0l=NULL,*sbuf1l=NULL,*sbuf0r=NULL,*sbuf1r=NULL;
UWORD *vbuf0l=NULL,*vbuf1l=NULL,*vbuf0r=NULL,*vbuf1r=NULL;
UWORD *ebuflr=NULL;
UBYTE plct,plst,plst2;
UWORD plbuflen;
ULONG oplayer=0;

UWORD *sptr0,*sptr1,*vptr0,*vptr1;
UWORD *plbuf0,*plbuf1,*plbuf2,*plbuf3;
SmpAccess *plsa0=NULL,*plsa1=NULL,*plsa2=NULL,*plsa3=NULL;
SmpAccess *plsa[CH_MAX]={NULL};
SAFuncPtr SARead;

//-- definitions --------------------------------------------------------------

void PlaySample_All(void) {
	if(RunTime.aktbuf->srat<100000 && RunTime.aktbuf->srat>100 && RunTime.aktbuf->slen>256) {
		if(RunTime.play) { StopSample();Delay(2); }
		RunTime.plsi=RunTime.aktbuf;
		ModifyIDCMP(RunTime.winBackDrop,IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_MENUVERIFY|IDCMP_INTUITICKS);
		if(RunTime.aplayer) RunTime.hfilter=(*PRA)&2;							/* Status der PowerLED retten */
		oplayer=RunTime.aplayer;
		switch(RunTime.aplayer) {
			case 0: if(RunTime.boolAHIOkay							) PlayAhiASample_All();break;
			case 1: if(RunTime.boolPaulaOkay						) PlayCascSample_All();break;
			case 2: if(RunTime.boolPaulaOkay && RunTime.boolCaCaOkay) PlayCaCaSample_All();break;
			case 3: if(RunTime.boolPaulaOkay						) PlayNormSample_All();break;
		}
	}
}

void PlaySample_Rng(void) {
	if(RunTime.aktbuf->srat<100000 && RunTime.aktbuf->srat>100 && RunTime.aktbuf->slen>256) {
		if(RunTime.play) { StopSample();Delay(2); }
		RunTime.plsi=RunTime.aktbuf;
		ModifyIDCMP(RunTime.winBackDrop,IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_MENUPICK|IDCMP_MENUVERIFY|IDCMP_INTUITICKS);
		if(RunTime.aplayer) RunTime.hfilter=(*PRA)&2;						/* Status der PowerLED retten */
		oplayer=RunTime.aplayer;
		switch(RunTime.aplayer) {
			case 0: if(RunTime.boolAHIOkay							) PlayAhiASample_Rng();break;
			case 1: if(RunTime.boolPaulaOkay						) PlayCascSample_Rng();break;
			case 2: if(RunTime.boolPaulaOkay && RunTime.boolCaCaOkay) PlayCaCaSample_Rng();break;
			case 3: if(RunTime.boolPaulaOkay						) PlayNormSample_Rng();break;
		}
	}
}

void StopSample(void) {
	switch(oplayer) {
		case 0: StopAhiASample();break;
		case 1: StopCascSample();break;
		case 2: StopCaCaSample();break;
		case 3: StopNormSample();break;
	}
	if(oplayer) {
		if(RunTime.hfilter) *PRA|=(UBYTE)2;					/* Status der PowerLED restaurieren */
		else *PRA&=(UBYTE)65533;
	}

}

//-- helper for internal paula players

UBYTE InitPaulaPlayers(void) {
	if((sbuf0l=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (sbuf1l=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (sbuf0r=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (sbuf1r=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (vbuf0l=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (vbuf1l=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (vbuf0r=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (vbuf1r=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (ebuflr=(UWORD *)AllocVec(MIN_PLAYBUF_BYTES,MEMF_PUBLIC|MEMF_CHIP|MEMF_CLEAR))
	&& (audint=(struct Interrupt *)AllocVec(sizeof(struct Interrupt),MEMF_PUBLIC|MEMF_CLEAR))) {
		return(TRUE);
	}
	return(FALSE);
}

void FreePaulaPlayers(void) {
	if(sbuf0l) { FreeVec(sbuf0l);sbuf0l=NULL; }
	if(sbuf1l) { FreeVec(sbuf1l);sbuf1l=NULL; }
	if(sbuf0r) { FreeVec(sbuf0r);sbuf0r=NULL; }
	if(sbuf1r) { FreeVec(sbuf1r);sbuf1r=NULL; }
	if(vbuf0l) { FreeVec(vbuf0l);vbuf0l=NULL; }
	if(vbuf1l) { FreeVec(vbuf1l);vbuf1l=NULL; }
	if(vbuf0r) { FreeVec(vbuf0r);vbuf0r=NULL; }
	if(vbuf1r) { FreeVec(vbuf1r);vbuf1r=NULL; }
	if(ebuflr) { FreeVec(ebuflr);ebuflr=NULL; }
	if(audint) { FreeVec(audint);audint=NULL; }
}

//-- helper for all players

void SetPlayRange(void) {
	switch(SetRngMode(RunTime.plsi)) {
		case RNGMD_ALL:
			RunTime.plptr=0;RunTime.plslen=RunTime.plsi->slen;
			break;
		case RNGMD_WINDOW:
			RunTime.plptr=RunTime.plsi->zoomxs;RunTime.plslen=RunTime.plsi->zoomxs+RunTime.plsi->zoomxl;
			break;
		case RNGMD_RANGE:
			RunTime.plptr=RunTime.plsi->markxs;RunTime.plslen=RunTime.plsi->markxs+RunTime.plsi->markxl;
			break;
	}	
}

void SetPlayStatus(void) {
	static char titlestr[256];

	DrawPPos(RunTime.plsi,RunTime.oldptr);           /* Delete Pos */
	RunTime.oldptr=RunTime.plptr;
	DrawPPos(RunTime.plsi,RunTime.oldptr);           /* Draw Pos */
	sprintf(titlestr,"%s: playing ... %s",((struct Node *)(RunTime.plsi))->ln_Name,ToXUnit(RunTime.plsi->unitx,RunTime.oldptr,RunTime.plsi->srat));
	SetWindowTitles(RunTime.plsi->win,titlestr,(UBYTE *)-1);
}

//-- eof ----------------------------------------------------------------------
