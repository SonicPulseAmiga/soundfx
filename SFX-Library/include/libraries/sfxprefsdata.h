/****h* sfxsupport.library/sfxprefsdata.h [4.2] *
*
*  NAME
*    sfxprefsdata.h
*  COPYRIGHT
*    $VER: sfxprefsdata.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    prefsdata related parts of sfxsupport.h
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

#ifndef  LIBRARIES_SFXPREFSDATA_H
#define  LIBRARIES_SFXPREFSDATA_H

//-- PrefsData ------------------------------------------------------------------------------------

typedef struct {
	UWORD   type;
	STRPTR  prefix;
	APTR    data;
} PrefsData;

#define PD_END           0
#define PD_SOURCE        1
#define PD_MODULATOR     2
#define PD_INTERPOLATOR  3
#define PD_WINFUNCTION   4
#define PD_COREINSTANCE  5
#define PD_OTHER        99

#endif   /* LIBRARIES_SFXPREFSDATA_H */

//-- eof ----------------------------------------------------------------------
