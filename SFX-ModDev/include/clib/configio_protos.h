/****h* configio/configio_protos.h [1.10] *
*
*  NAME
*    configio_protos.h
*  COPYRIGHT
*    $VER: configio_protos.h 1.10 (19.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    prototypes
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    21.Aug.1998
*  MODIFICATION HISTORY
*    19.Sep.1998	V 1.10	most recent version
*    21.Aug.1998	V 1.00	most recent version
*    21.Aug.1998	V 0.00	initial version
*  NOTES
*
*******
*/

#ifndef  CLIB_CONFIGIO_PROTOS_H
#define  CLIB_CONFIGIO_PROTOS_H

#include <exec/types.h>

/*****************************************************************************/

APTR	CfgIO_NewConfig				(STRPTR appname,STRPTR appver);
LONG	CfgIO_RemConfig				(APTR cfg);
LONG	CfgIO_WriteConfig			(APTR cfg,STRPTR filename);
LONG	CfgIO_ReadConfig 			(APTR cfg,STRPTR filename);

LONG	CfgIO_DeleteItem			(APTR cfg,STRPTR section,STRPTR itemname);
LONG	CfgIO_DeleteSection 		(APTR cfg,STRPTR section);
LONG	CfgIO_DeleteConfig			(APTR cfg);
STRPTR	CfgIO_GetSubItem			(STRPTR data,STRPTR *cont,TEXT delimiter);
ULONG	CfgIO_MatchString			(STRPTR str,STRPTR *strlist);

LONG	CfgIO_WriteString			(APTR cfg,STRPTR section,STRPTR itemname,STRPTR data);
STRPTR	CfgIO_ReadString			(APTR cfg,STRPTR section,STRPTR itemname,STRPTR def);

LONG	CfgIO_WriteBool				(APTR cfg,STRPTR section,STRPTR itemname,BOOL data);
BOOL	CfgIO_ReadBool				(APTR cfg,STRPTR section,STRPTR itemname,BOOL def);

LONG	CfgIO_WriteSignedInteger	(APTR cfg,STRPTR section,STRPTR itemname,LONG data);
LONG	CfgIO_ReadSignedInteger		(APTR cfg,STRPTR section,STRPTR itemname,LONG def);

LONG	CfgIO_WriteUnsignedInteger	(APTR cfg,STRPTR section,STRPTR itemname,ULONG data);
ULONG	CfgIO_ReadUnsignedInteger	(APTR cfg,STRPTR section,STRPTR itemname,ULONG def);

LONG	CfgIO_WriteFloat			(APTR cfg,STRPTR section,STRPTR itemname,double *data);
double	CfgIO_ReadFloat				(APTR cfg,STRPTR section,STRPTR itemname,double *def);

#endif	 /* CLIB_CONFIGIO_PROTOS_H */
