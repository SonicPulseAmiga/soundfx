/****h*SoundFX/SFX-ModLoader.h [4.3] *
*
*  NAME
*    SFX-ModLoader.h
*  COPYRIGHT
*    $VER: SFX-ModLoader.h 4.3 (01.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    sample loader interface - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    01.Mar.2004	V 4.3	most recent version
*    03.Jun.2000    V 4.00  most recent version
*    18.Aug.1998    V 3.70  most recent version
*    14.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#ifndef SFX_ModLoader_H
#define SFX_ModLoader_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ModLoader_C
	extern BOOL LoadSample(void);
	extern BOOL LoadSampleRexx(char *fn);
	extern void LoadSampleInterface(void);

	//-- helper

   extern UBYTE FindExt(ModInfo *mi,char *ext);

	//-- helper for arexx cmds
	extern void HandleLoaderCmd(STRPTR params,STRPTR ret);
#endif

#endif /* SFX_ModLoader_H */

//-- eof ----------------------------------------------------------------------
