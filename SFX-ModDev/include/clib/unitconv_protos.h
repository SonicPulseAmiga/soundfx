#ifndef  CLIB_UNITCONV_PROTOS_H
#define  CLIB_UNITCONV_PROTOS_H

/*
**	$VER: unitconv_protos.h 1.0 (05.05.98)
**
**	(C) Copyright 1993-1998 Stefan Kost
**	    All Rights Reserved
*/

#include <exec/types.h>

/*****************************************************************************/

void			ConvertParameter(char *dstval,char *srcval,UBYTE group,UBYTE dsttyp);
UBYTE			GetParameterUnit(char *srcval,UBYTE group);

#endif	 /* CLIB_UNITCONV_PROTOS_H */
