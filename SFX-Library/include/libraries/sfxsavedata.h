/****h* sfxsupport.library/sfxsavedata.h [4.2] *
*
*  NAME
*    sfxsavedata.h
*  COPYRIGHT
*    $VER: sfxsavedata.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    savedata related parts of sfxsupport.h
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

#ifndef  LIBRARIES_SFXSAVEDATA_H
#define  LIBRARIES_SFXSAVEDATA_H

//-- SaveData -------------------------------------------------------------------------------------

/* I can't supply all params to the library [lib-savt.c/WriteData()] directly, because we are
 * running out of registers.
 */

typedef struct {
	char    fn[FILENAME_MAX];
	ULONG   offs;
	APTR    tab;                    // for packing
	ULONG   savestart,savelength;
	UBYTE   chmask;                 // for just saving selected channels
} SaveData;

#endif   /* LIBRARIES_SFXSAVEDATA_H */

//-- eof ----------------------------------------------------------------------
