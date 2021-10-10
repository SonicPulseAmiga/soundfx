/****h*SoundFX/SFX-ToolsList.h [4.2] *
*
*  NAME
*    SFX-ToolsList.h
*  COPYRIGHT
*    $VER: SFX-ToolsList.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    node list processing tools - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*    27.Aug.2000    V 4.1   most recent version
*    14.Aug.1998    V 3.70  most recent version
*    14.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#ifndef SFX_ToolsList_H
#define SFX_ToolsList_H

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_ToolsList_C
	extern void FreeList(struct List *list,ULONG size);
	extern void FreeModList(struct List *list);

	//-- SampleLists

	extern void FlushAllSamples(void);
	extern void FlushHiddenSamples(void);
	extern void FlushShownSamples(void);

	//-- ArrangeLists

	extern void	SortAList(UBYTE anz,ArrangeList *alist);
	extern UBYTE match_nr2ix(UBYTE anz,UBYTE nr,ArrangeList *alist);
	extern UBYTE match_name2nr(UBYTE anz,UBYTE *name,ArrangeList *alist);
#endif

#endif /* SFX_ToolsList_H */

//-- eof ----------------------------------------------------------------------
