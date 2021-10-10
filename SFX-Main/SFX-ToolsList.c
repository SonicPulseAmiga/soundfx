/****h*SoundFX/SFX-ToolsList.c [4.2] *
*
*  NAME
*    SFX-ToolsList.c
*  COPYRIGHT
*    $VER: SFX-ToolsList.c 4.2 (30.10.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    node list processing tools - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    30.Oct.2002	V 4.2	most recent version
*    12.Sep.2000    V 4.1   most recent version
*    14.Aug.1998    V 3.70  most recent version
*    14.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_ToolsList_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void FreeList(struct List *list,ULONG size);
void FreeModList(struct List *list);

//-- SampleLists

void FlushAllSamples(void);
void FlushHiddenSamples(void);
void FlushShownSamples(void);

//-- ArrangeLists

void SortAList(UBYTE anz,ArrangeList *alist);
UBYTE match_nr2ix(UBYTE anz,UBYTE nr,ArrangeList *alist);
UBYTE match_name2nr(UBYTE anz,UBYTE *name,ArrangeList *alist);

//-- definitions --------------------------------------------------------------

void FreeList(struct List *list,ULONG size)
{
	struct Node *anode;

	while(anode=RemTail(list)) {
		if(anode->ln_Name) FreeVec((anode->ln_Name));
		FreeVec(anode);
	}
	NewList(list);
}

void FreeModList(struct List *list)
{
	struct Node *anode;

	while(anode=RemTail(list)) {
		// don't free anode->ln_Name
		FreeVec(anode);
	}
	NewList(list);
}

//-- SampleLists

void FlushAllSamples(void)
{
	SInfo *si,*nsi;
	//-- alle noch geladenen Samples freigeben
	si=(SInfo *)GetFirstNode(&BufferList);
	while(!EndOfList(&BufferList,(struct Node *)si)) {
		if(si->lockct) { si=(SInfo *)GetNextNode(((struct Node *)si)); }
		else {
			if(RunTime.play && si==RunTime.plsi) StopSample();
			nsi=(SInfo *)GetNextNode(((struct Node *)si));
			RemSample(si);
			si=nsi;
		}
	}
}

void FlushHiddenSamples(void)
{
	SInfo *si,*nsi;
	//-- alle noch geladenen Samples freigeben
	si=(SInfo *)GetFirstNode(&BufferList);
	while(!EndOfList(&BufferList,(struct Node *)si)) {
		if(si->lockct || !SFX_SOUND_IS_HIDDEN(si)) { si=(SInfo *)GetNextNode(((struct Node *)si)); }
		else {
			if(RunTime.play && si==RunTime.plsi) StopSample();
			nsi=(SInfo *)GetNextNode(((struct Node *)si));
			RemSample(si);
			si=nsi;
		}
	}
}

void FlushShownSamples(void)
{
	SInfo *si,*nsi;
	//-- alle noch geladenen Samples freigeben
	si=(SInfo *)GetFirstNode(&BufferList);
	while(!EndOfList(&BufferList,(struct Node *)si)) {
		if(si->lockct || !SFX_SOUND_IS_SHOWN(si)) { si=(SInfo *)GetNextNode(((struct Node *)si)); }
		else {
			if(RunTime.play && si==RunTime.plsi) StopSample();
			nsi=(SInfo *)GetNextNode(((struct Node *)si));
			RemSample(si);
			si=nsi;
		}
	}
}

//-- ArrangeLists

void SortAList(UBYTE anz,ArrangeList *alist)
{
	register UWORD j,k;
	char tempstr[50];
	WORD tmpi;

	for(j=0;j<anz-1;j++) {
		for(k=0;k<anz-1;k++) {
			if(stricmp(alist[k].name,alist[k+1].name)>0) {
				strcpy(tempstr,alist[k].name);strcpy(alist[k].name,alist[k+1].name);strcpy(alist[k+1].name,tempstr);
				tmpi=alist[k].nr;alist[k].nr=alist[k+1].nr;alist[k+1].nr=tmpi;
			}
		}
	}
}

UBYTE match_nr2ix(UBYTE anz,UBYTE nr,ArrangeList *alist)
{
	register UBYTE i,ix=0;

	for(i=0;i<anz;i++)
		if(alist[i].nr==nr) { ix=i;break; }
	return(ix);
}

UBYTE match_name2nr(UBYTE anz,UBYTE *name,ArrangeList *alist)
{
	register UBYTE i,ix=0;

	for(i=0;i<anz;i++)
		if(!strcmp(alist[i].name,name)) { ix=alist[i].nr;break; }
	return(ix);
}

//-- eof ----------------------------------------------------------------------
