/****h* sfxsupport.library/image.c [4.3] *
*
*  NAME
*    images.c
*  COPYRIGHT
*    $VER: image.c 4.3 (08.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    image handling routines - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    16.Okt.2003
*  MODIFICATION HISTORY
*    08.Apr.2004	V 4.3	most recent version
*    16.Okt.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#define IMAGE_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- image saving routines

void		ASM LIB_EXPORT(SaveImage(REG(a0,STRPTR fn)));

//-- definitions --------------------------------------------------------------

//-- image saving routines

void SAVEDS ASM LIB_EXPORT(SaveImage(REG(a0,STRPTR fn))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(fn,__FUNC__,"1.par",4));

/** @todo see XPKTXT:SOURCE/RKM_Lib/Icon/GlowIconImage.c, XPKTXT:SOURCE/RKM_Lib/DataTypes/Src/ClipView/save.c */
/*
	
	Object *out=NULL;
	struct BitMap * bm2=NULL;
	BPTR file;

	bm2 = AllocBitMap(dstBitMapWidth,dstBitMapHeight,bmh->bmh_Depth,BMF_CLEAR,NULL);
	if(bm2 == NULL)
	{
		error = ERROR_NO_FREE_STORE;
		goto out;
	}

	if(out=NewDTObject(FilePart(fn),
		DTA_SourceType,		DTST_RAM,
		DTA_GroupID,		GID_PICTURE,
		PDTA_NumColors,		numColours,
		PDTA_BitMap,		bm2,
		PDTA_ModeID,		modeID,
		TAG_DONE)) {
		if(file=Open(fn,MODE_NEWFILE)) {
			DoMethod(out,DTM_WRITE,NULL,file,DTWM_IFF,NULL);
		}
	}
*/
}

//-- eof ----------------------------------------------------------------------
