/****h* listtools/listtools_protos.h [0.0] *
*
*  NAME
*    listtools_protos.h
*  COPYRIGHT
*    $VER: listtools_protos.h 0.0 (27.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    prototypes
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    25.Mai.1999
*  MODIFICATION HISTORY
*    27.Aug.2000	V 0.0	most recent version
*    06.Jun.1999	V 1.00	most recent version
*    25.Mai.1999	V 0.00	initial version
*  NOTES
*
*******
*/

#ifndef  CLIB_LISTTOOLS_PROTOS_H
#define  CLIB_LISTTOOLS_PROTOS_H

#include <exec/exec.h>
#include <libraries/listtools.h>

/*****************************************************************************/

struct Node *	GetNodeByName 		(struct List *list,STRPTR name);
struct Node *	GetNodeByIndex		(struct List *list,ULONG index);
LONG			GetIndexByName		(struct List *list,STRPTR name);
LONG			GetIndexByNode		(struct List *list,struct Node *node);
STRPTR			GetNameByIndex		(struct List *list,ULONG index);

STRPTR *		CreateNameArray		(struct List *list);
void			DeleteNameArray		(STRPTR *namearray);

void			InsertBefore		(struct Node *newnode,struct Node *curnode);
void			InsertAfter			(struct Node *newnode,struct Node *curnode);
void			InsertAt			(struct List *list,struct Node *newnode,ULONG index);
void			InsertByNameAsc		(struct List *list,struct Node *node);
void			InsertByNameDesc	(struct List *list,struct Node *node);
void			InsertByPriAsc		(struct List *list,struct Node *node);
void			InsertByPriDesc		(struct List *list,struct Node *node);
void			InsertUserAsc		(struct List *list,struct Node *node,CompareHook cmpfunc);
void			InsertUserDesc		(struct List *list,struct Node *node,CompareHook cmpfunc);

ULONG			GetNumberOfNodes	(struct List *list);
void			ReverseList			(struct List *list);

struct Node *	RemoveAt 			(struct List *list,ULONG index);

void			SortByNameAsc		(struct List *list);
void			SortByNameDesc		(struct List *list);
void			SortByPriAsc		(struct List *list);
void			SortByPriDesc		(struct List *list);
void			SortUserAsc			(struct List *list,CompareHook cmpfunc);
void			SortUserDesc		(struct List *list,CompareHook cmpfunc);

#endif	 /* CLIB_LISTTOOLS_PROTOS_H */
