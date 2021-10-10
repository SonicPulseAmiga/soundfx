/****h* sfxsupport.library/sfxsource.h [4.2] *
*
*  NAME
*    sfxsource.h
*  COPYRIGHT
*    $VER: sfxsource.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    source related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Aug.2002
*  MODIFICATION HISTORY
*    19.Aug.2002	V 4.2	most recent version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXSOURCE_H
#define  LIBRARIES_SFXSOURCE_H

//-- Source ---------------------------------------------------------------------------------------

typedef struct {
	SInfo           *srcbuf;
	UBYTE           rmode;
} Source;

#define SRC_IDCT    3
#define SRC_IXCT    3

#define RNGMD_ALL       0
#define RNGMD_WINDOW    1
#define RNGMD_RANGE     2

// for PrefsData
#define PDS_SOURCE          sizeof(Source)

#endif   /* LIBRARIES_SFXSOURCE_H */

//-- eof ----------------------------------------------------------------------
