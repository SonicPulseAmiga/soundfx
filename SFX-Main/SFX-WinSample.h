/****h*SoundFX/SFX-WinSample.h [4.2] *
*
*  NAME
*    SFX-WinSample.h
*  COPYRIGHT
*    $VER: SFX-WinSample.h 4.2 (19.02.03) © by Stefan Kost 2000-2003
*  FUNCTION
*    Samplewindows - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Feb.2000
*  MODIFICATION HISTORY
*    19.Feb.2003	V 4.2	most recent version
*    18.Feb.2000	V 4.0   most recent version
*    15.Feb.2000	V 4.0   initial version
*  NOTES
*
*******
*/

#ifndef SFX_WinSample_H
#define SFX_WinSample_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_WinSample_C
	extern void Test_SampleWin (ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr,SInfo *si,SInfo **clsi,struct IntuiMessage **imsg2);

	extern void Refr_ContextMenu(void);

	extern void OneSampleWin(void);
	extern void MultiSampleWin(void);
#endif

#endif /* SFX_WinSample_H */

//-- eof ----------------------------------------------------------------------
