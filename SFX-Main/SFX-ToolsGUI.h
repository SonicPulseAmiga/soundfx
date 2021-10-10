/****h*SoundFX/SFX-Tools.h [4.1] *
*
*  NAME
*    SFX-Tools.h
*  COPYRIGHT
*    $VER: SFX-Tools.h 4.1 (30.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    gui releated helper - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    30.Aug.2000	V 4.1	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#ifndef SFX_ToolsGUI_H
#define SFX_ToolsGUI_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ToolsGUI_C
	extern void CText(struct RastPort *rp,UWORD x1,UWORD x2,UWORD y,char *text);

	extern void FlushMsgPort(struct MsgPort *mp);

	extern void RefreshGads(ULONG mode);
	extern void ChangeSamples(void);

	extern void ObtainGradientColors(void);
	extern void FreeGradientColors(void);
#endif

#endif /* SFX_ToolsGUI_H */

//-- eof ----------------------------------------------------------------------
