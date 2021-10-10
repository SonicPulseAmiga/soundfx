/****h* debugtools/sos_listtools_decl.h [1.20] *
*
*  NAME
*    sos_listtools_decl.h
*  COPYRIGHT
*    $VER: sos_listtools_decl.h 1.20 (05.06.99) © by Stefan Kost 1998-1999
*  FUNCTION
*    listtools-function wrapper-decls
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    04.Mai.1998
*  MODIFICATION HISTORY
*    05.Jun.1999	V 1.20	first release
*    04.Mai.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern struct Node *GetNodeByName(struct List *list,STRPTR name);
		#define GetNodeByName(list,name) SOS_GetNodeByName(list,name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __GetNodeByName(list,name) SOS_GetNodeByName(list,name,NULL,0)
	#endif
#else
	#define __GetNodeByName(list,name)		GetNodeByName(list,name)
#endif

//-- eof ----------------------------------------------------------------------
