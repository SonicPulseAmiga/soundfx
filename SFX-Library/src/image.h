/****h* sfxsupport.library/image.h [4.3] *
*
*  NAME
*    image.h
*  COPYRIGHT
*    $VER: image.h 4.3 (08.04.04) © by Stefan Kost 1998-12004
*  FUNCTION
*    image handling routines - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    16.Okt.2003
*  MODIFICATION HISTORY
*    08.Apr.2004    V 4.3   most recent version
*    16.Oct.2003	V 4.3	most recent version
*    16.Okt.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#ifndef IMAGE_H
#define IMAGE_H

//-- prototypes ---------------------------------------------------------------

#ifndef IMAGE_C
	//-- image saving routines

	extern void		ASM LIB_EXPORT(SaveImage(REG(a0,STRPTR nodename)));
#endif

#endif /* HELP_H */

//-- eof ----------------------------------------------------------------------
