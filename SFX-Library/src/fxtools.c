/****h* sfxsupport.library/fxtools.c [4.2] *
*
*  NAME
*    fxtools.c
*  COPYRIGHT
*    $VER: fxtools.c 4.2 (18.02.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    some fx helpers
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    18.Feb.2003    V 4.2   most recent version
*    25.Apr.2001    V 4.1   most recent version
*                           added DrawGadWB,DrawGadBW,DrawTitle
*    30.Jul.2000    V 1.30  most recent version
*    23.Feb.1999    V 1.20  most recent version
*    18.Sep.1998    V 1.20  initial version
*  NOTES
*
*******
*/

#define FXTOOLS_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"
#define CHIPREG_DEFS
#include "SFX-ChipRegDefs.h"
#undef CHIPREG_DEFS

//-- prototypes ---------------------------------------------------------------

//-- FXTools

UBYTE           ASM LIB_EXPORT(SetRngMode(REG(a0,SInfo *si)));
void            ASM LIB_EXPORT(SetRngs(REG(a0,SInfo *si),REG(a1,ULONG *rm_start),REG(a2,ULONG *rm_len),REG(d0,UBYTE rmode)));
ULONG           ASM LIB_EXPORT(ZeroSearch(REG(a0,SInfo *si),REG(d0,ULONG aktpos),REG(d1,BYTE dir),REG(d2,BYTE dir2),REG(d3,UBYTE chan)));
ULONG           ASM LIB_EXPORT(PeakSearch(REG(a0,SInfo *si),REG(d0,ULONG beg),REG(d1,ULONG len),REG(d2,BYTE dir)));
void            ASM LIB_EXPORT(SetNewPlaybackRate(void));

SInfo *         SetValidBuffer(ULONG *modid);

//-- private Stuff

BYTE			INLINE Signum(WORD value);

//-- definitions --------------------------------------------------------------

//-- FXTools

UBYTE SAVEDS ASM LIB_EXPORT(SetRngMode(REG(a0,SInfo *si))) {
	UBYTE rmode=RNGMD_ALL;

	if(si) {
		ASSERT_RESET;
		ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

		if(si->zoomxl<si->slen) rmode=RNGMD_WINDOW;
		if(si->markxl>1) rmode=RNGMD_RANGE;              /* markxl==1 -> Marker */
	}
	return(rmode);
}

void SAVEDS ASM LIB_EXPORT(SetRngs(REG(a0,SInfo *si),REG(a1,ULONG *rm_start),REG(a2,ULONG *rm_len),REG(d0,UBYTE rmode))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(rm_start,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(rm_len,__FUNC__,"3.par",4));

	switch(rmode) {
		case RNGMD_ALL:     *rm_start=0;         *rm_len=si->slen;    break;
		case RNGMD_WINDOW:  *rm_start=si->zoomxs;*rm_len=si->zoomxl;  break;
		case RNGMD_RANGE:   *rm_start=si->markxs;*rm_len=si->markxl;  break;
	}
	//if(*rm_start<0) *rm_start=0;			// can't be - all ULONGS
	if(*rm_len==0) *rm_len=si->slen;		// in case no mark/no zoom
	if(*rm_start+*rm_len>si->slen) {		// sanity check #1
		if(*rm_start<si->slen) *rm_len=si->slen-*rm_start;
		else { //-- sanity check #2 - just in case
			*rm_start=0;*rm_len=si->slen;
		}
	}
}

ULONG SAVEDS ASM LIB_EXPORT(ZeroSearch(REG(a0,SInfo *si),REG(d0,ULONG aktpos),REG(d1,BYTE dir),REG(d2,BYTE dir2),REG(d3,UBYTE chan))) {
	register LONG spos=aktpos;
	UBYTE search=TRUE;
	BYTE sign1,sign2;
	WORD *buf;
	/** see below
	register ULONG i,j;
	SAFuncPtr SARead;
	SmpAccess *sa;
	UBYTE init=FALSE;
	*/

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	switch(si->storage) {
		case VM_MEMORY:
			if(dir==1) {                                            // search forward
				buf=&si->buf[chan][spos];
				sign1=Signum(*buf);
				while(search) {
					buf++;spos++;
					if(spos==si->slen) break;
					else {
						sign2=Signum(*buf);
						if(dir2== 0 && sign1!=sign2) search=FALSE;          // for what transition do we search
						if(dir2==-1 && sign1>sign2) search=FALSE;
						if(dir2== 1 && sign1<sign2) search=FALSE;
						sign1=sign2;
					}
				}
			}
			else {                                                  // search backward
				spos--;
				buf=&si->buf[chan][spos];
				sign1=Signum(*buf);
				while(!search) {
					buf--;spos--;
					if(spos<0) break;
					else {
						sign2=Signum(*buf);
						if(dir2== 0 && sign1!=sign2) search=FALSE;          // for what transition do we search
						if(dir2==-1 && sign1>sign2) search=FALSE;
						if(dir2== 1 && sign1<sign2) search=FALSE;
						sign1=sign2;
					}
				}
				spos++;
			}
			break;
		case VM_DRIVE:
			/** @TODO implement ZeroSearch for virtual memory */
			/*
			if(dir==1) {
				ERR("not fully implemented");
				if((sa=LIB_INTCALL(SAInit(si,chan,spos,(si->slen-spos),VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN)))) {
					SARead=sa->safunc;
					for(sa->seg=1;((sa->seg<sa->seganz) && search);sa->seg++) {
						buf=(WORD *)sa->buffer1;if(!SARead(sa,sa->blen)) break;
						for(i=0;((i<(sa->blen>>1)) && search);i++) {
							if(init) sign2=Signum(*buf);
							else { sign1=Signum(*buf);init=TRUE; }
							buf++;spos++;
							if(dir2== 0 && sign1!=sign2) search=FALSE;          // for what transition do we search
							if(dir2==-1 && sign1>sign2) search=FALSE;
							if(dir2== 1 && sign1<sign2) search=FALSE;
							sign1=sign2;
						}
					}
					buf=(WORD *)sa->buffer1;SARead(sa,sa->blen);
					for(i=0;i<(si->slen-((sa->seganz-1)<<(sa->blensh-1)));i++) {
						sign2=Signum(*buf);
						buf++;spos++;
						if(dir2== 0 && sign1!=sign2) search=FALSE;          // for what transition do we search
						if(dir2==-1 && sign1>sign2) search=FALSE;
						if(dir2== 1 && sign1<sign2) search=FALSE;
						sign1=sign2;
					}
					LIB_INTCALL(SADone(sa,si));
				}
				//else MSG("      SAInit failed");
			}
			else {
				ERR("not yet implemented");
			}
			*/
			break;
	}
	if(!search) return((ULONG)spos);
	else return(aktpos);
}

ULONG SAVEDS ASM LIB_EXPORT(PeakSearch(REG(a0,SInfo *si),REG(d0,ULONG beg),REG(d1,ULONG len),REG(d2,BYTE dir))) {
	register ULONG spos;
	register UBYTE l;
	WORD peak=0,*buf;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	switch(si->storage) {
		case VM_MEMORY:
			if(dir==1) {
				peak=SMP_MAXN;
				for(l=0;l<(si->channels+1);l++) {
					if(si->channelmask&(1<<l)) {
						buf=&si->buf[l][beg];
						for(spos=0;spos<len;spos++) {
							if(*buf>peak) peak=*buf;
							buf++;
						}
					}
				}
			}
			else {
				peak=SMP_MAXP;
				for(l=0;l<(si->channels+1);l++) {
					if(si->channelmask&(1<<l)) {
						buf=&si->buf[l][beg];
						for(spos=0;spos<len;spos++) {
							if(*buf<peak) peak=*buf;
							buf++;
						}
					}
				}
			}
			break;
		case VM_DRIVE:
			ERR("not yet implemented");
			/** @TODO implement PeakSearch for virtual memory */
			break;
	}
	return(peak);
}

void SAVEDS ASM LIB_EXPORT(SetNewPlaybackRate(void)) {
	UWORD playperi;
	UBYTE tplst=1;

	// gets only called from
	//   SFX-Main.c::SFX-WinSmpOpts.c
	//   SFX-Library.c::req-period.c
	// both already do the check below
	//if(GetRunTime(play) && GetRunTime(plsi)) {
		switch(GetRunTime(aplayer)) {
			case 0:
				if(GetRunTime(optp_Amiga_8bit)->rateclip==0 && GetRunTime(plsi)->srat>28867) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>28867) tplst<<=1;
				}
				if(GetRunTime(optp_Amiga_8bit)->rateclip==1 && GetRunTime(plsi)->srat>57734) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>57734) tplst<<=1;
				}
				//playperi=(UWORD)(1.0/((double)(GetRunTime(plsi)->srat/tplst)*0.000000279356));
				playperi=(*(struct ExecBase**)(4))->ex_EClockFrequency*5*tplst/GetRunTime(plsi)->srat;
				*aud1per=*aud0per=(UWORD)playperi;*GetRunTime(plst)=tplst;*GetRunTime(plst2)=tplst+tplst;
				break;
			case 1:
				if(GetRunTime(optp_Amiga_14bit)->rateclip==0 && GetRunTime(plsi)->srat>28867) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>28867) tplst<<=1;
				}
				if(GetRunTime(optp_Amiga_14bit)->rateclip==1 && GetRunTime(plsi)->srat>57734) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>57734) tplst<<=1;
				}
				//playperi=(UWORD)(1.0/((double)(GetRunTime(plsi)->srat/tplst)*0.000000279356));
				playperi=(*(struct ExecBase**)(4))->ex_EClockFrequency*5*tplst/GetRunTime(plsi)->srat;
				*aud3per=*aud2per=*aud1per=*aud0per=(UWORD)playperi;*GetRunTime(plst)=tplst;*GetRunTime(plst2)=tplst+tplst;
				break;
			case 2:
				if(GetRunTime(optp_Amiga_14cbit)->rateclip==0 && GetRunTime(plsi)->srat>28867) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>28867) tplst<<=1;
				}
				if(GetRunTime(optp_Amiga_14cbit)->rateclip==1 && GetRunTime(plsi)->srat>57734) {
					tplst=2;
					while((GetRunTime(plsi)->srat/tplst)>57734) tplst<<=1;
				}
				//playperi=(UWORD)(1.0/((double)(GetRunTime(plsi)->srat/tplst)*0.000000279356));
				playperi=(*(struct ExecBase**)(4))->ex_EClockFrequency*5*tplst/GetRunTime(plsi)->srat;
				*aud3per=*aud2per=*aud1per=*aud0per=(UWORD)playperi;*GetRunTime(plst)=tplst;*GetRunTime(plst2)=tplst+tplst;
				break;
			case 3:
				AHIBase=GetRunTime(AHIBase);
				if(AHIBase) {
					switch(GetRunTime(plsi)->channels) {
						case CH_MONO:
							AHI_SetFreq(0,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							break;
						case CH_STEREO:
							AHI_SetFreq(0,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							AHI_SetFreq(1,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							break;
						case CH_QUADRO:
							AHI_SetFreq(0,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							AHI_SetFreq(1,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							AHI_SetFreq(2,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							AHI_SetFreq(3,GetRunTime(plsi)->srat,GetRunTime(audioctrl),AHISF_IMM);
							break;
					}
				}
				break;
		}
	//}
}

SInfo * SetValidBuffer(ULONG *modid) {
	SInfo *buf;

	if((*modid) && (buf=LIB_INTCALL(GetBufferPtr(*modid,-1)))) *modid=buf->id;
	else {
		if(GetRunTime(aktbuf)) {
			buf=GetRunTime(aktbuf);*modid=buf->id;
		}
		else {
			buf=NULL; *modid=NULL;
		}
	}
	return(buf);
}

//-- private Stuff

BYTE INLINE Signum(WORD value) {
	if(value>0) return(1);
	else if(value==0) return(0);
	return(-1);
}

//-- eof ----------------------------------------------------------------------
