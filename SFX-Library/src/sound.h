/****h* sfxsupport.library/sound.h [4.2] *
*
*  NAME
*    sound.h
*  COPYRIGHT
*    $VER: sound.h 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    sound project tools - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef SOUND_H
#define SOUND_H

//-- prototypes ---------------------------------------------------------------

#ifndef SOUND_C
	//-- BufferTools

	extern void			ASM LIB_EXPORT(BufferFill(REG(a0,SInfo *srcbuf),REG(a1,SInfo *dstbuf)));
	extern SInfo *		ASM LIB_EXPORT(NewSample(void));

	extern void			ASM LIB_EXPORT(AddSample(REG(a0,SInfo *si),REG(a1,STRPTR name)));
	extern void			ASM LIB_EXPORT(RemSample(REG(a0,SInfo *si)));

	extern void			ASM LIB_EXPORT(SetSlider(REG(a0,SInfo *si),REG(d0,UBYTE mode)));
	extern void			ASM LIB_EXPORT(SetTitle(REG(a0,SInfo *si)));
	extern void			ASM LIB_EXPORT(SetName(REG(a0,SInfo *si),REG(a1,STRPTR name)));
	extern STRPTR 		ASM LIB_EXPORT(JoinSampleNames(REG(a0,STRPTR name1),REG(a1,STRPTR name2)));

	extern void			ASM LIB_EXPORT(ShowSample(void));
	extern void			ASM LIB_EXPORT(HideSample(void));

	extern void			ASM LIB_EXPORT(SetOptWinBounds(REG(a0,UWORD *x),REG(a1,UWORD *y),REG(d0,UWORD w),REG(d1,UWORD h)));
	extern UBYTE		WinHit(UWORD *windata,UWORD *xc,UWORD *yc,UWORD winanz,UWORD x,UWORD y);

	extern SInfo *		ASM LIB_EXPORT(LockBuffer(REG(a0,SInfo *si)));
	extern void			ASM LIB_EXPORT(UnlockBuffer(REG(a0,SInfo *si)));

	extern void			ASM LIB_EXPORT(RecalcSampleLines(REG(a0,SInfo *si)));

	//-- helper for scroller windows

	extern UBYTE		ASM LIB_EXPORT(InitScrollerWinStuff(void));
	extern void			ASM LIB_EXPORT(FreeScrollerWinStuff(void));

#endif

#endif /* SOUND_H */

//-- eof ----------------------------------------------------------------------
