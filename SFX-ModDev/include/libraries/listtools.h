#ifndef  LIBRARIES_LISTTOOLS_H
#define  LIBRARIES_LISTTOOLS_H

/*
**	$VER: listtools.h 1.0 (05.05.98)
**
**	(C) Copyright 1993-1998 Stefan Kost
**	    All Rights Reserved
*/

//-- includes -----------------------------------------------------------------

//-- Ansi C/C++

//-- Amiga OS

#define __USE_SYSBASE
#include <exec/exec.h>
#include <cmacros.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */

#include <proto/exec.h>					/* Prototypes für Libraryfunctions */

//-- definitions --------------------------------------------------------------

typedef BYTE (*CompareHook)(struct Node *node1,struct Node *node2);

//-- macros -------------------------------------------------------------------

/****** listtools/IsFirstOfList [1.00] *
*
*  NAME
*    IsFirstOfList -- is element the first of this list
*  SYNOPSIS
*    BOOL IsFirstOfList(struct List *list, struct Node *node)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define IsFirstOfList(list,node) ((list)->lh_Head==(node))

/****** listtools/IsLastOfList [1.00] *
*
*  NAME
*    IsLastOfList -- is element the last of this list
*  SYNOPSIS
*    BOOL IsLastOfList(struct List *list, struct Node *node)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define IsLastOfList(list,node) ((list)->lh_TailPred==(node))

/****** listtools/IsEndOfList [1.00] *
*
*  NAME
*    IsEndOfList -- is the end of this list reached
*  SYNOPSIS
*    BOOL IsEndOfList(struct List *list, struct Node *node)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define IsEndOfList(list,node) ((node)==(struct Node *)(&((list)->lh_Tail)))

/****** listtools/GetNextNode [1.00] *
*
*  NAME
*    GetNextNode -- get the next element
*  SYNOPSIS
*    struct Node *GetNextNode(struct Node *node)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define GetNextNode(node) ((node)->ln_Succ)

/****** listtools/GetPrevNode [1.00] *
*
*  NAME
*    GetPrevNode -- get the previous element
*  SYNOPSIS
*    struct Node *GetPrevNode(struct Node *node)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define GetPrevNode(node) ((node)->ln_Pred)

/****** listtools/GetFirstNode [1.00] *
*
*  NAME
*    GetFirstNode -- get the first element
*  SYNOPSIS
*    struct Node *GetFirstNode(struct List *list)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define GetFirstNode(list) ((list)->lh_Head)

/****** listtools/GetLastNode [1.00] *
*
*  NAME
*    GetLastNode -- get the last element
*  SYNOPSIS
*    struct Node *GetLastNode(struct List *list)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define GetLastNode(list) ((list)->lh_TailPred)

/****** listtools/foreach [1.00] *
*
*  NAME
*    foreach -- iterate through the whole list
*  SYNOPSIS
*    foreach(struct Node *node,struct List *list)
*  FUNCTION
*
*  INPUTS
*
*  RESULT
*
*  EXAMPLE
*    foreach(node,list) puts(node->ln_Name);
*
*    i=0;
*    foreach(node,list)
*    {
*       printf("list[%2d]=\"%s\"\n",i++,(node->ln_Name));
*    }
*
*  NOTES
*
*  BUGS
*
*  SEE ALSO
*
**********
*/
#define foreach(node,list) for(node=GetFirstNode(list);!IsEndOfList(list,node);node=GetNextNode(node))

//-- eof ----------------------------------------------------------------------

#endif	 /* LIBRARIES_LISTTOOLS_H */
