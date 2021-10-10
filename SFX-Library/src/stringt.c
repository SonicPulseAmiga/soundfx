/****h* sfxsupport.library/stringt.c [4.1] *
*
*  NAME
*    stringt.c
*  COPYRIGHT
*    $VER: stringt.c 4.1 (03.01.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    string related helpers - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    12.Jul.1998
*  MODIFICATION HISTORY
*    03.Jan.2001	V 4.1	most recent version
*    19.Sep.1998	V 1.2	most recent version
*    12.Jul.1998	V 1.1	initial version
*  NOTES
*
*******
*/

#define STRINGT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

void		ASM LIB_EXPORT(FilterString(REG(a0,STRPTR strbuf)));
void 		ASM LIB_EXPORT(StripString(REG(a0,STRPTR strbuf)));
void 		ASM LIB_EXPORT(TrimString(REG(a0,STRPTR strbuf)));
STRPTR 		ASM LIB_EXPORT(StringCopy(REG(a0,STRPTR src)));

//-- definitions --------------------------------------------------------------

void SAVEDS ASM LIB_EXPORT(FilterString(REG(a0,STRPTR strbuf))) {
	Param buf;
	STRPTR allowed="0123456789+-.";
	register UBYTE i,j;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(strbuf,__FUNC__,"1.par",4));

	i=j=0;
	while(strbuf[i]!=0 && i!=PARBUFLEN) {
		if(strchr(allowed,strbuf[i])) {
			buf[j]=strbuf[i];j++;
		}
		i++;
	}
	buf[j]=0;j++;
	for(i=0;i!=j;i++) strbuf[i]=buf[i];
}

void SAVEDS ASM LIB_EXPORT(StripString(REG(a0,STRPTR strbuf))) {
	register UBYTE i,j;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(strbuf,__FUNC__,"1.par",4));

	i=j=0;
	while(strbuf[i]) {
		if(strbuf[i]>32) strbuf[j++]=strbuf[i];		// overread all chars <33
		i++;
	}
	strbuf[j]='\0';
}

void SAVEDS ASM LIB_EXPORT(TrimString(REG(a0,STRPTR strbuf))) {
	register UBYTE i,j;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(strbuf,__FUNC__,"1.par",4));

	i=0;while(strbuf[i]==' ') i++;
	j=0;while(strbuf[i]) strbuf[j++]=strbuf[i++];
	j--;while(strbuf[j]==' ') strbuf[j--]='\0';
}

/** @todo use a memory pool for this (UserLibInit/Done)
 * memory pools have the disadvantag, that they do not
 * track the allocation size, like AllocVec.
 * We could do this on our own, by allocating 4 more
 * bytes storing the size there and returning &mem[1]
 * This would require AllocPooledVec and FreePooledVec
 * (in mem.c)
 */

STRPTR  SAVEDS ASM LIB_EXPORT(StringCopy(REG(a0,STRPTR src))) {
	STRPTR dst,tmp1,tmp2;
	UBYTE sl;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));

	//-- get length incl '\0'
	tmp1=src;sl=1;while(*(tmp1++)) sl++;
	if(sl>1) {
		if((tmp2=dst=(STRPTR)AllocVec(sl,MEMF_ANY))) {
			tmp1=src;
			while(*tmp1) *(tmp2++)=*(tmp1++);
			*tmp2='\0';
		}
		return(dst);
	}
	else return(NULL);
}

//-- eof ----------------------------------------------------------------------
