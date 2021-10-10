/****h* sfxsupport.library/sfxloaddata.h [4.2] *
*
*  NAME
*    sfxloaddata.h
*  COPYRIGHT
*    $VER: sfxloaddata.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    loaddata related parts of sfxsupport.h
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

#ifndef  LIBRARIES_SFXLOADDATA_H
#define  LIBRARIES_SFXLOADDATA_H

//-- LoadData -------------------------------------------------------------------------------------

/* I can't supply all params to the library [lib-savt.c/WriteData()] directly, because we are
 * running out of registers.
 */

typedef struct {
	char    fn[FILENAME_MAX];
	ULONG   offs;
	APTR    tab;                    // for de-packing
	UBYTE   chmask;                 // for just saving selected channels
	UBYTE   flags;					// see below
	APTR    codecdata;              // additional data for the codec
} LoadData;

#define LDF_NONE	0
// no channel offset
#define LDF_NOCHOFF	1

#endif   /* LIBRARIES_SFXLOADDATA_H */

//-- eof ----------------------------------------------------------------------
