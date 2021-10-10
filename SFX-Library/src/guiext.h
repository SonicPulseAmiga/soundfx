/****h* sfxsupport.library/guiext.h [1.30] *
*
*  NAME
*    guiext.hc
*  COPYRIGHT
*    $VER: guiext.h 1.30 (25.01.00) © by Stefan Kost 1998-12000
*  FUNCTION
*    gui extensions - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    25.Jan.2000
*  MODIFICATION HISTORY
*    25.Jan.2000	V 1.30	most recent version
*    25.Jan.2000	V 1.30	initial version
*  NOTES
*
*******
*/

#ifndef GUIEXT_H
#define GUIEXT_H

//-- prototypes ---------------------------------------------------------------

#ifndef GUIEXT_C
	//-- extended event handling

	void		ASM LIB_EXPORT(NG_HandleEvents(REG(d0,ULONG iclass),REG(d1,UWORD icode),REG(d2,WORD imx),REG(d3,WORD imy),REG(a0,APTR iadr),REG(a1,struct Window *iwin),REG(a2,BOOL *closedd)));
#endif

#endif /* GUIEXT_H */

//-- eof ----------------------------------------------------------------------
