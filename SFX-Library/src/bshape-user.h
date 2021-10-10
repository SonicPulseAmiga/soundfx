/****h* sfxsupport.library/bshape-user.h [4.1] *
*
*  NAME
*    bshape-user.h
*  COPYRIGHT
*    $VER: bshape-user.h 4.1 (25.04.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Modulator BlendShapes : User - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    25.Apr.2001	V 4.1	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef BSHAPE_USER_H
#define BSHAPE_USER_H

//-- prototypes ---------------------------------------------------------------

#ifndef BSHAPE_USER_C
	//-- Modulator User

	extern void Describe_ModUser  (Modulator *mod);

	extern double		ASM BShape_UserNormal_Single			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserNormal_Repeat			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserNormal_Stretch			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAbsolut_Single			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAbsolut_Repeat			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAbsolut_Stretch			(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAmpEnv_MaxDecay_Single	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAmpEnv_MaxDecay_Repeat	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserAmpEnv_MaxDecay_Stretch	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_PeakSearch_Single	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_PeakSearch_Repeat	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_PeakSearch_Stretch(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_Energy_Single		(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_Energy_Repeat		(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
	extern double		ASM BShape_UserFrqEnv_Energy_Stretch	(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,void *md));
#endif

//-- globals ------------------------------------------------------------------

#ifndef BSHAPE_C
	extern UBYTE *TypeLabels[];
	extern UBYTE *ModeLabels[];
	extern UBYTE *EnvLabels[2][2];
#endif

#endif /* BSHAPE_USER_H */

//-- eof ----------------------------------------------------------------------
