/****h* sfxsupport.library/fxtools-cfg.h [4.2] *
*
*  NAME
*    fxtools-cfg.h
*  COPYRIGHT
*    $VER: fxtools-cfg.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    some fx cfg helpers
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    24.Mar.2003
*  MODIFICATION HISTORY
*    24.Mar.2003    V 4.2   most recent version
*    24.Mar.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#ifndef FXTOOLS_CFG_H
#define FXTOOLS_CFG_H

//-- prototypes ---------------------------------------------------------------

#ifndef FXTOOLS_CFG_C
	//-- Config Tools

	extern void			ASM LIB_EXPORT(InitFXParams(REG(a0,PrefsData *fxdesc)));
	extern void			ASM LIB_EXPORT(DoneFXParams(REG(a0,PrefsData *fxdesc)));
	extern void			ASM LIB_EXPORT(FreeFXParams(REG(a0,PrefsData *fxdesc)));

	extern void			ASM LIB_EXPORT(SaveModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc)));
	extern void			ASM LIB_EXPORT(LoadModSettings(REG(a0,APTR cfg),REG(a1,PrefsData *fxdesc)));

#endif

#endif /* FXTOOLS_CFG_H */

//-- eof ----------------------------------------------------------------------
