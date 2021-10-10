/****h* sfxsupport.library/vm.h [4.2] *
*
*  NAME
*    vm.h
*  COPYRIGHT
*    $VER: vm.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    virtual memory routines -- declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Sep.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*    19.Sep.1998	V 1.20	most recent version
*    19.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef VM_H
#define VM_H

//-- prototypes ---------------------------------------------------------------

#ifndef VM_C
	//-- VM support

	extern UBYTE 		ASM LIB_EXPORT(ConvMem2Drv(REG(a0,SInfo *si)));
	extern UBYTE		ASM LIB_EXPORT(ConvDrv2Mem(REG(a0,SInfo *si)));

	extern SmpAccess	ASM *LIB_EXPORT(SAInit(REG(a0,SInfo *si),REG(d0,UBYTE chan),REG(d1,ULONG start), REG(d2,ULONG len),REG(d3,UBYTE mode),REG(d4,ULONG blklen)));
	extern void			ASM LIB_EXPORT(SADone(REG(a0,SmpAccess *sa),REG(a1,SInfo *si)));

	extern UBYTE		ASM LIB_EXPORT(CopyData(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,ULONG srcpos),REG(d1,ULONG dstpos),REG(d2,ULONG len)));
	extern UBYTE		ASM LIB_EXPORT(CopyDataCh(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,UBYTE ch),REG(d1,ULONG srcpos),REG(d2,ULONG dstpos),REG(d3,ULONG len)));

	extern void			ASM LIB_EXPORT(ExchangeData(REG(a0,SInfo *src),REG(a1,SInfo *dst)));

	extern void			ASM LIB_EXPORT(RecalcLoopPlayData(void));

	//-- vmem-sample recovery
	extern void			SaveSInfo(SInfo *si);
	extern void			RemoveSInfo(SInfo *si);
	extern void			RenameSInfo(SInfo *src,SInfo *dst);

	//-- helper
	extern UBYTE		CopyDataSub(void *src,void *dst,UWORD mode,ULONG len);
	extern STRPTR		MakeVMemName(SInfo *si,UBYTE channel);
	extern void			SafeRemove(STRPTR fn);

	//-- IO functions

	extern UBYTE		ASM SA_MemRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length));
	extern UBYTE		ASM SA_MemWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length));

	extern UBYTE		ASM SA_StreamRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length));
	extern UBYTE		ASM SA_StreamWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length));
#endif

#endif /* VM_H */

//-- eof ----------------------------------------------------------------------
