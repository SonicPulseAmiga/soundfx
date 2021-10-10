/****h* sfxsupport.library/mem.c [4.2] *
*
*  NAME
*    mem.c
*  COPYRIGHT
*    $VER: mem.c 4.2 (04.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    better memory allocation - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    04.Jun.2002	V 4.2	most recent version
*    29.May.2001	V 4.2	most recent version
*							A: monitor mem usage
*    31.May.2000	V 1.30	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define MEM_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- additional memory allocation
APTR		ASM LIB_EXPORT(ReAllocVec(REG(a0,APTR addro),REG(d0,ULONG sizeo),REG(d1,ULONG sizen),REG(d2,ULONG typen)));

//-- monitor memory usage
void		ASM LIB_EXPORT(MemUsage(REG(a0,STRPTR str)));
ULONG		AvailVMem(void);

//-- definitions --------------------------------------------------------------

//-- additional memory allocation

/* only called by sound-mem.c */

APTR SAVEDS ASM LIB_EXPORT(ReAllocVec(REG(a0,APTR addro),REG(d0,ULONG sizeo),REG(d1,ULONG sizen),REG(d2,ULONG typen))) {
	APTR addrn=addro;
	ULONG size=min(sizen,sizeo);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(addro,__FUNC__,"1.par",4));

	if(addro && size) {
		if((addrn=AllocVec(sizen,typen))) {
			memcpy(addrn,addro,size);
			FreeVec(addro);
		}
	}
	return(addrn);
}

//-- monitor memory usage

void SAVEDS ASM LIB_EXPORT(MemUsage(REG(a0,STRPTR str))) {

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(str,__FUNC__,"1.par",4));

	switch((GetRunTime(sfxprefs_misc)->virtmem<<1)|(GetRunTime(sfxprefs_misc)->realmem)) {
		case 0: sprintf(str,VERS);break;
		case 1: sprintf(str,VERS"   RealMem=%ld/%ld Bytes",AvailMem(MEMF_ANY|MEMF_LARGEST),AvailMem(MEMF_ANY));break;
		case 2: sprintf(str,VERS"   VirtMem=%ld Bytes",AvailVMem());break;
		case 3:	sprintf(str,VERS"   RealMem=%ld/%ld Bytes  VirtMem=%ld Bytes",AvailMem(MEMF_ANY|MEMF_LARGEST),AvailMem(MEMF_ANY),AvailVMem());break;
	}
}

ULONG AvailVMem(void)
{
	struct InfoData InfoData;
	BPTR FileLock;
	ULONG free=0;
	char drive[FILENAME_MAX];

	if(GetRunTime(sfxprefs_vmem)->enabled) {
		strsfn(GetRunTime(sfxprefs_vmem)->swap_path,drive,NULL,NULL,NULL);
		if((FileLock=Lock(drive,ACCESS_READ))) {
			if(Info(FileLock,&InfoData)) {
				free=(InfoData.id_NumBlocks-InfoData.id_NumBlocksUsed)*InfoData.id_BytesPerBlock;
			}
			UnLock(FileLock);
		}
	}
	return(free);
}

//-- eof ----------------------------------------------------------------------
