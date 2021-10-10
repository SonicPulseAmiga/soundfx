/****h*SoundFX/SFX-ModOperator.h [4.1] *
*
*  NAME
*    SFX-ModOperator.h
*  COPYRIGHT
*    $VER: SFX-ModOperator.h 4.1 (02.09.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    sample operator interface - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    02.Sep.2000	V 4.1	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_ModOperator_H
#define SFX_ModOperator_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ModOperator_C
	extern void ProcessSample(void);
	extern BOOL ProcessSampleRexx(char *params);

	//-- helper for arexx cmds
	extern void HandleOperatorCmd(char *params,char *ret);
#endif

#endif /* SFX_ModOperator_H */

//-- eof ----------------------------------------------------------------------
