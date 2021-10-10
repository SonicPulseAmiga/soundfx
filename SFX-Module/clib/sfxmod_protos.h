#ifndef  CLIB_SFXMOD_PROTOS_H
#define  CLIB_SFXMOD_PROTOS_H

/*
**	$VER: sfxmod_protos.h 3.0 (17.01.97)
**
**	(C) Copyright 1993-1996 Stefan Kost
**	    All Rights Reserved
*/

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef  LIBRARIES_SFXSUPPORT_H
#include <libraries/sfxsupport.h>
#endif

#ifndef  LIBRARIES_SFXMOD_H
#include <libraries/sfxmod.h>
#endif

/*****************************************************************************/

void	*SFXMod_ClientDataInit(RTime *RunTime_);
void	SFXMod_ClientDataDone(void *instance);

UBYTE	SFXMod_Interface(void *instance);
UBYTE	SFXMod_Process(void *instance,ProcessData *pdata);
void	SFXMod_HandleARexxCmd(void *instance,char *params,char *ret);

#endif	 /* CLIB_SFXMOD_PROTOS_H */
