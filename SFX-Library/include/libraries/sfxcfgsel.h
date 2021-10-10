/****h* sfxsupport.library/sfxcfgsel.h [4.2] *
*
*  NAME
*    sfxcfgsel.h
*  COPYRIGHT
*    $VER: sfxcfgsel.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    cfgsel related parts of sfxsupport.h
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

#ifndef  LIBRARIES_SFXCFGSEL_H
#define  LIBRARIES_SFXCFGSEL_H

//-- CfgSel ---------------------------------------------------------------------------------------

typedef struct {
	struct List cfgs;
	struct Node *aktcfg;
	ULONG       action;
	char        path[FILENAME_MAX];
	ULONG       ss,sm;          // for GUI interaction
	ULONG       aktcfgnr;
	UBYTE       akttest;
} CfgSel;

#define CFGSEL_IDCT 4
#define CFGSEL_IXCT 4

#define CFGSEL_ACTION_NONE  0
#define CFGSEL_ACTION_GO    1
#define CFGSEL_ACTION_LOAD  2

#endif   /* LIBRARIES_SFXCFGSEL_H */

//-- eof ----------------------------------------------------------------------
