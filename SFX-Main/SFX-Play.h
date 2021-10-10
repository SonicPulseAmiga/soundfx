/****h* SoundFX/SFX-Play.h [4.3] *
*
*  NAME
*    SFX-Play.h
*  COPYRIGHT
*    $VER: SFX-Play.h 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    sample player interface - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    15.May.2002	V 4.2	most recent version
*    13.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_Play_H
#define SFX_Play_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_Play_C
	extern void PlaySample_All(void);
	extern void PlaySample_Rng(void);
	extern void StopSample(void);

	//-- helper for internal paula players

	extern UBYTE InitPaulaPlayers(void);
	extern void FreePaulaPlayers(void);

	//-- helper for all players
	
	extern void SetPlayRange(void);
	extern void SetPlayStatus(void);
#endif

//-- globals ------------------------------------------------------------------

#ifndef SFX_Play_C
	extern struct Interrupt *audint,*oldaudint;
	extern char *intname;
	extern UWORD sv,v2,s2;
	extern UWORD *sbuf0l,*sbuf1l,*sbuf0r,*sbuf1r;
	extern UWORD *vbuf0l,*vbuf1l,*vbuf0r,*vbuf1r;
	extern UWORD *ebuflr;
	extern UBYTE plct,plst,plst2;
	extern UWORD plbuflen;

	extern UWORD *sptr0,*sptr1,*vptr0,*vptr1;
	extern UWORD *plbuf0,*plbuf1,*plbuf2,*plbuf3;
	extern SmpAccess *plsa0,*plsa1,*plsa2,*plsa3;
	extern SmpAccess *plsa[CH_MAX];
	extern SAFuncPtr SARead;
#endif

#endif /* SFX_Play_H */

//-- eof ----------------------------------------------------------------------
