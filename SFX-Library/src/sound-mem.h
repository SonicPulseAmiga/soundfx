/****h* sfxsupport.library/sound-mem.h [4.2] *
*
*  NAME
*    sound-mem.h
*  COPYRIGHT
*    $VER: sound-mem.h 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    sound memory allocation - declarations
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

#ifndef SOUND_MEM_H
#define SOUND_MEM_H

//-- prototypes ---------------------------------------------------------------

#ifndef SOUND_MEM_C
	//-- Sound Memory Allocation

	extern UBYTE	ASM LIB_EXPORT(AllocSampleMem(REG(a0,SInfo *si),REG(d0,ULONG slen),REG(d1,UBYTE chan)));
	extern void		ASM LIB_EXPORT(FreeSampleMem(REG(a0,SInfo *si)));

	//-- helper

	extern void		SInfo_SetVBuf(SInfo *si,UBYTE chan);

#endif

#endif /* SOUND_MEM_H */

//-- eof ----------------------------------------------------------------------
