/****h* SoundFX/SFX-ModTools.h [4.2] *
*
*  NAME
*    SFX-ModTools.h
*  COPYRIGHT
*    $VER: SFX-ModTools.h 4.2 (23.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Modulsupportfunktionen - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    23.May.2002	V 4.2	most recent version
*    10.Sep.2000	V 4.1	most recent version
*    18.Aug.1998	V 3.70	most recent version
*    13.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_ModTools_H
#define SFX_ModTools_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ModTools_C

	//-- get installed modules

	extern UWORD GetModuleInfos(char *ln,char *dn,struct List *modlist,UBYTE namecorr);
	extern UWORD ScanDirectory(char *ln,char *dn,struct List *modlist,UBYTE namecorr);
	extern UBYTE CheckValidModule(char *dn,char *fn);
	extern void ParseModInfo(char *dn,char *fn,ModNode *node);

	//-- module index guide

	extern void BuildIndexGuide(struct List *list,char *typ);

	//-- amigaguide contents array

	extern void BuildAGContents(void);
	extern void FreeAGContents(void);

	//-- preset-handling

	extern void SetPreset(STRPTR group,STRPTR mod, STRPTR cfg);

	//-- user notification

	extern void NotifyUser(void);

#endif

#endif /* SFX_ModTools_H */

//-- includes -----------------------------------------------------------------

//-- prototypes ---------------------------------------------------------------

//-- defines ------------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
