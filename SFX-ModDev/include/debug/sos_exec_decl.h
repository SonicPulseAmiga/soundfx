/****h* debugtools/sos_exec_decl.h [1.20] *
*
*  NAME
*    sos_exec_decl.h
*  COPYRIGHT
*    $VER: sos_exec_decl.h 1.20 (02.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    exec-function wrapper-decls
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Feb.1998
*  MODIFICATION HISTORY
*    02.Aug.1998	V 1.20	first release
*    12.Feb.1998	V 0.00	initial version
*  NOTES
*
*******
*/

#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern void SOS_AbortIO(struct IORequest *ioreq,char *file,unsigned long line);
		#define AbortIO(ioreq) SOS_AbortIO(ioreq,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AbortIO(ioreq) SOS_AbortIO(ioreq,NULL,0)

		extern void SOS_AddDevice(struct Device *device,char *file,unsigned long line);
		#define AddDevice(device) SOS_AddDevice(device,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddDevice(device) SOS_AddDevice(device,NULL,0)

		extern void SOS_AddHead(struct List *list,struct Node *node,char *file,unsigned long line);
		#define AddHead(list,node) SOS_AddHead(list,node,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddHead(list,node) SOS_AddHead(list,node,NULL,0)

		extern void SOS_AddIntServer(long intNumber,struct Interrupt *interrupt,char *file,unsigned long line);
		#define AddIntServer(intNumber,interrupt) SOS_AddIntServer(intNumber,interrupt,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddIntServer(intNumber,interrupt) SOS_AddIntServer(intNumber,interrupt,NULL,0)

		extern void SOS_AddLibrary(struct Library *library,char *file,unsigned long line);
		#define AddLibrary(library) SOS_AddLibrary(library,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddLibrary(library) SOS_AddLibrary(library,NULL,0)

		extern void SOS_AddMemHandler(struct Interrupt *memhand,char *file,unsigned long line);
		#define AddMemHandler(memhand) SOS_AddMemHandler(memhand,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddMemHandler(memhand) SOS_AddMemHandler(memhand,NULL,0)

		extern void SOS_AddMemList(ULONG size,ULONG attributes,LONG pri,APTR base,STRPTR name,char *file,unsigned long line);
		#define AddMemList(size,attributes,pri,base,name) SOS_AddMemList(size,attributes,pri,base,name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddMemList(size,attributes,pri,base,name) SOS_AddMemList(size,attributes,pri,base,name,NULL,0)

		extern void SOS_AddPort(struct MsgPort *port,char *file,unsigned long line);
		#define AddPort(port) SOS_AddPort(port,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddPort(port) SOS_AddPort(port,NULL,0)

		extern void SOS_AddResource(APTR resource,char *file,unsigned long line);
		#define AddResource(resource) SOS_AddResource(resource,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddResource(resource) SOS_AddResource(resource,NULL,0)

		extern void SOS_AddSemaphore(struct SignalSemaphore *sigSem,char *file,unsigned long line);
		#define AddSemaphore(sigSem) SOS_AddSemaphore(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddSemaphore(sigSem) SOS_AddSemaphore(sigSem,NULL,0)

		extern void SOS_AddTail(struct List *list,struct Node *node,char *file,unsigned long line);
		#define AddTail(list,node) SOS_AddTail(list,node,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddTail(list,node) SOS_AddTail(list,node,NULL,0)

		extern void SOS_AddTask(struct Task *task,APTR initialPC,APTR finalPC,char *file,unsigned long line);
		#define AddTask(task,initialPC,finalPC) SOS_AddTask(task,initialPC,finalPC,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AddTask(task,initialPC,finalPC) SOS_AddTask(task,initialPC,finalPC,NULL,0)

		extern void SOS_Alert(ULONG alertNum,char *file,unsigned long line);
		#define Alert(alertNum) SOS_Alert(alertNum,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Alert(alertNum) SOS_Alert(alertNum,NULL,0)

		extern APTR SOS_AllocAbs(unsigned long byteSize,APTR location,char *file,unsigned long line);
		#define AllocAbs(byteSize,location) SOS_AllocAbs(byteSize,location,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocAbs(byteSize,location) SOS_AllocAbs(byteSize,location,NULL,0)

		extern APTR SOS_Allocate(struct MemHeader *freeList,unsigned long byteSize,char *file,unsigned long line);
		#define Allocate(freeList,byteSize) SOS_Allocate(freeList,byteSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Allocate(freeList,byteSize) SOS_Allocate(freeList,byteSize,NULL,0)

		extern struct MemList *SOS_AllocEntry(struct MemList *entry,char *file,unsigned long line);
		#define AllocEntry(entry) SOS_AllocEntry(entry,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocEntry(entry) SOS_AllocEntry(entry,NULL,0)

		extern APTR SOS_AllocMem(unsigned long byteSize,unsigned long requirements,char *file,unsigned long line);
		#define AllocMem(byteSize,requirements) SOS_AllocMem(byteSize,requirements,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocMem(byteSize,requirements) SOS_AllocMem(byteSize,requirements,NULL,0)

		extern APTR SOS_AllocPooled(APTR poolHeader,unsigned long memSize,char *file,unsigned long line);
		#define AllocPooled(poolHeader,memSize) SOS_AllocPooled(poolHeader,memSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocPooled(poolHeader,memSize) SOS_AllocPooled(poolHeader,memSize,NULL,0)

		extern BYTE SOS_AllocSignal(long signalNum,char *file,unsigned long line);
		#define AllocSignal(signalNum) SOS_AllocSignal(signalNum,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocSignal(signalNum) SOS_AllocSignal(signalNum,NULL,0)

		extern LONG SOS_AllocTrap(long trapNum,char *file,unsigned long line);
		#define AllocTrap(trapNum) SOS_AllocTrap(trapNum,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocTrap(trapNum) SOS_AllocTrap(trapNum,NULL,0)

		extern APTR SOS_AllocVec(unsigned long byteSize,unsigned long requirements,char *file,unsigned long line);
		#define AllocVec(byteSize,requirements) SOS_AllocVec(byteSize,requirements,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AllocVec(byteSize,requirements) SOS_AllocVec(byteSize,requirements,NULL,0)

		extern LONG SOS_AttemptSemaphore(struct SignalSemaphore *signalSemaphore,char *file,unsigned long line);
		#define AttemptSemaphore(signalSemaphore) SOS_AttemptSemaphore(signalSemaphore,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AttemptSemaphore(signalSemaphore) SOS_AttemptSemaphore(signalSemaphore,NULL,0)

		extern LONG SOS_AttemptSemaphoreShared(struct SignalSemaphore *signalSemaphore,char *file,unsigned long line);
		#define AttemptSemaphoreShared(signalSemaphore) SOS_AttemptSemaphoreShared(signalSemaphore,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AttemptSemaphoreShared(signalSemaphore) SOS_AttemptSemaphoreShared(signalSemaphore,NULL,0)

		extern ULONG SOS_AvailMem(ULONG attributes,char *file,unsigned long line);
		#define AvailMem(attributes) SOS_AvailMem(attributes,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __AvailMem(attributes) SOS_AvailMem(attributes,NULL,0)

		extern void SOS_CacheClearE(APTR address,ULONG length,ULONG caches,char *file,unsigned long line);
		#define CacheClearE(address,length,caches) SOS_CacheClearE(address,length,caches,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CacheClearE(address,length,caches) SOS_CacheClearE(address,length,caches,NULL,0)

		extern void SOS_CacheClearU(char *file,unsigned long line);
		#define CacheClearU() SOS_CacheClearU(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CacheClearU() SOS_CacheClearU(NULL,0)

		extern void SOS_CacheControl(ULONG cacheBits,ULONG cacheMask,char *file,unsigned long line);
		#define CacheControl(cacheBits,cacheMask) SOS_CacheControl(cacheBits,cacheMask,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CacheControl(cacheBits,cacheMask) SOS_CacheControl(cacheBits,cacheMask,NULL,0)

		extern void SOS_CachePostDMA(APTR vaddress,ULONG *length,ULONG flags,char *file,unsigned long line);
		#define CachePostDMA(vaddress,length,flags) SOS_CachePostDMA(vaddress,length,flags,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CachePostDMA(vaddress,length,flags) SOS_CachePostDMA(vaddress,length,flags,NULL,0)

		extern APTR SOS_CachePreDMA(APTR vaddress,ULONG *length,ULONG flags,char *file,unsigned long line);
		#define CachePreDMA(vaddress,length,flags) SOS_CachePreDMA(vaddress,length,flags,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CachePreDMA(vaddress,length,flags) SOS_CachePreDMA(vaddress,length,flags,NULL,0)

		extern void SOS_Cause(struct Interrupt *interrupt,char *file,unsigned long line);
		#define Cause(interrupt) SOS_Cause(interrupt,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Cause(interrupt) SOS_Cause(interrupt,NULL,0)

		extern struct IORequest *SOS_CheckIO(struct IORequest *ioreq,char *file,unsigned long line);
		#define CheckIO(ioreq) SOS_CheckIO(ioreq,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CheckIO(ioreq) SOS_CheckIO(ioreq,NULL,0)

		extern void SOS_CloseDevice(struct IORequest *ioRequest,char *file,unsigned long line);
//		#define CloseDevice(ioRequest) do { SOS_CloseDevice(ioRequest,DEBUG_SRCPATH __FILE__,__LINE__);ioRequest=DT_INVALID_POINTER; } while(0)
		#define CloseDevice(ioRequest) SOS_CloseDevice(ioRequest,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CloseDevice(ioRequest) SOS_CloseDevice(ioRequest,NULL,0)

		extern void SOS_CloseLibrary(struct Library *library,char *file,unsigned long line);
//		#define CloseLibrary(library) do { SOS_CloseLibrary(library,DEBUG_SRCPATH __FILE__,__LINE__);library=DT_INVALID_POINTER; } while(0)
		#define CloseLibrary(library) SOS_CloseLibrary(library,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CloseLibrary(library) SOS_CloseLibrary(library,NULL,0)

		extern void SOS_ColdReboot(char *file,unsigned long line);
		#define ColdReboot() SOS_ColdReboot(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ColdReboot() SOS_ColdReboot(NULL,0)

		extern void SOS_CopyMem(APTR source,APTR dest,ULONG size,char *file,unsigned long line);
		#define CopyMem(source,dest,size) SOS_CopyMem(source,dest,size,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CopyMem(source,dest,size) SOS_CopyMem(source,dest,size,NULL,0)

		extern void SOS_CopyMemQuick(APTR source,APTR dest,ULONG size,char *file,unsigned long line);
		#define CopyMemQuick(source,dest,size) SOS_CopyMemQuick(source,dest,size,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CopyMemQuick(source,dest,size) SOS_CopyMemQuick(source,dest,size,NULL,0)

		extern struct IORequest *SOS_CreateIORequest(struct MsgPort *port,unsigned long size,char *file,unsigned long line);
		#define CreateIORequest(port,size) SOS_CreateIORequest(port,size,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CreateIORequest(port,size) SOS_CreateIORequest(port,size,NULL,0)

		extern struct MsgPort *SOS_CreateMsgPort(char *file,unsigned long line);
		#define CreateMsgPort() SOS_CreateMsgPort(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CreateMsgPort() SOS_CreateMsgPort(NULL,0)

		extern APTR SOS_CreatePool(unsigned long requirements,unsigned long puddleSize,unsigned long threshSize,char *file,unsigned long line);
		#define CreatePool(requirements,puddleSize,threshSize) SOS_CreatePool(requirements,puddleSize,threshSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __CreatePool(requirements,puddleSize,threshSize) SOS_CreatePool(requirements,puddleSize,threshSize,NULL,0)

		extern void SOS_Deallocate(struct MemHeader *freeList,APTR memoryBlock,unsigned long byteSize,char *file,unsigned long line);
//		#define Deallocate(freeList,memoryBlock,byteSize) do { SOS_Deallocate(freeList,memoryBlock,byteSize,DEBUG_SRCPATH __FILE__,__LINE__);memoryBlock=DT_INVALID_POINTER; } while(0)
		#define Deallocate(freeList,memoryBlock,byteSize) SOS_Deallocate(freeList,memoryBlock,byteSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Deallocate(freeList,memoryBlock,byteSize) SOS_Deallocate(freeList,memoryBlock,byteSize,NULL,0)

		extern void SOS_Debug(ULONG flags,char *file,unsigned long line);
		#define Debug(flags) SOS_Debug(flags,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Debug(flags) SOS_Debug(flags,NULL,0)

		extern void SOS_DeleteIORequest(APTR ioRequest,char *file,unsigned long line);
//		#define DeleteIORequest(ioRequest) do { SOS_DeleteIORequest(ioRequest,DEBUG_SRCPATH __FILE__,__LINE__);ioRequest=DT_INVALID_POINTER; } while(0)
		#define DeleteIORequest(ioRequest) SOS_DeleteIORequest(ioRequest,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __DeleteIORequest(ioRequest) SOS_DeleteIORequest(ioRequest,NULL,0)

		extern void SOS_DeleteMsgPort(struct MsgPort *port,char *file,unsigned long line);
//		#define DeleteMsgPort(port) do { SOS_DeleteMsgPort(port,DEBUG_SRCPATH __FILE__,__LINE__);port=DT_INVALID_POINTER; } while(0)
		#define DeleteMsgPort(port) SOS_DeleteMsgPort(port,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __DeleteMsgPort(port) SOS_DeleteMsgPort(port,NULL,0)

		extern void SOS_DeletePool(APTR poolHeader,char *file,unsigned long line);
//		#define DeletePool(poolHeader) do { SOS_DeletePool(poolHeader,DEBUG_SRCPATH __FILE__,__LINE__);poolHeader=DT_INVALID_POINTER; } while(0)
		#define DeletePool(poolHeader) SOS_DeletePool(poolHeader,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __DeletePool(poolHeader) SOS_DeletePool(poolHeader,NULL,0)

		extern void SOS_Disable(char *file,unsigned long line);
		#define Disable() SOS_Disable(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Disable() SOS_Disable(NULL,0)

		extern BYTE SOS_DoIO(struct IORequest *ioreq,char *file,unsigned long line);
		#define DoIO(ioreq) SOS_DoIO(ioreq,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __DoIO(ioreq) SOS_DoIO(ioreq,NULL,0)

		extern void SOS_Enable(char *file,unsigned long line);
		#define Enable() SOS_Enable(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Enable() SOS_Enable(NULL,0)

		extern void SOS_Enqueue(struct List *list, struct Node *node,char *file,unsigned long line);
		#define Enqueue(list,node) SOS_Enqueue(list,node,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Enqueue(list,node) SOS_Enqueue(list,node,NULL,0)

		extern struct Node *SOS_FindName(struct List *start,STRPTR name,char *file,unsigned long line);
		#define FindName(start,name) SOS_FindName(start,name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FindName(start,name) SOS_FindName(start,name,NULL,0)

		extern struct MsgPort *SOS_FindPort(STRPTR name,char *file,unsigned long line);
		#define FindPort(name) SOS_FindPort(name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FindPort(name) SOS_FindPort(name,NULL,0)

		extern struct Resident *SOS_FindResident(STRPTR name,char *file,unsigned long line);
		#define FindResident(name) SOS_FindResident(name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FindResident(name) SOS_FindResident(name,NULL,0)

		extern struct SignalSemaphore *SOS_FindSemaphore(STRPTR name,char *file,unsigned long line);
		#define FindSemaphore(name) SOS_FindSemaphore(name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FindSemaphore(name) SOS_FindSemaphore(name,NULL,0)

		extern void SOS_Forbid(char *file,unsigned long line);
		#define Forbid() SOS_Forbid(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Forbid() SOS_Forbid(NULL,0)

		extern void SOS_FreeEntry(struct MemList *entry,char *file,unsigned long line);
//		#define FreeEntry(entry) do { SOS_FreeEntry(entry,DEBUG_SRCPATH __FILE__,__LINE__);entry=DT_INVALID_POINTER; } while(0)
		#define FreeEntry(entry) SOS_FreeEntry(entry,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreeEntry(entry) SOS_FreeEntry(entry,NULL,0)

		extern void SOS_FreeMem(APTR memoryBlock,unsigned long byteSize,char *file,unsigned long line);
//		#define FreeMem(memoryBlock,byteSize) do { SOS_FreeMem(memoryBlock,byteSize,DEBUG_SRCPATH __FILE__,__LINE__);memoryBlock==DT_INVALID_POINTER; } while(0)
		#define FreeMem(memoryBlock,byteSize) SOS_FreeMem(memoryBlock,byteSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreeMem(memoryBlock,byteSize) SOS_FreeMem(memoryBlock,byteSize,NULL,0)

		extern void SOS_FreePooled(APTR poolHeader,APTR memory,unsigned long memSize,char *file,unsigned long line);
//		#define FreePooled(poolHeader,memory,memSize) do { SOS_FreePooled(poolHeader,memory,memSize,DEBUG_SRCPATH __FILE__,__LINE__);memory=DT_INVALID_POINTER; } while(0)
		#define FreePooled(poolHeader,memory,memSize) SOS_FreePooled(poolHeader,memory,memSize,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreePooled(poolHeader,memory,memSize) SOS_FreePooled(poolHeader,memory,memSize,NULL,0)

		extern void SOS_FreeSignal(long signalNum,char *file,unsigned long line);
//		#define FreeSignal(signalNum) do { SOS_FreeSignal(signalNum,DEBUG_SRCPATH __FILE__,__LINE__);signalNum=-1; } while(0)
		#define FreeSignal(signalNum) SOS_FreeSignal(signalNum,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreeSignal(signalNum) SOS_FreeSignal(signalNum,NULL,0)

		extern void SOS_FreeTrap(long trapNum,char *file,unsigned long line);
//		#define FreeTrap(trapNum) do { SOS_FreeTrap(trapNum,DEBUG_SRCPATH __FILE__,__LINE__);trapNum=-1; } while(0)
		#define FreeTrap(trapNum) SOS_FreeTrap(trapNum,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreeTrap(trapNum) SOS_FreeTrap(trapNum,NULL,0)

		extern void SOS_FreeVec(APTR memoryBlock,char *file,unsigned long line);
//		#define FreeVec(memoryBlock) do { SOS_FreeVec(memoryBlock,DEBUG_SRCPATH __FILE__,__LINE__);memoryBlock=DT_INVALID_POINTER; } while(0)
		#define FreeVec(memoryBlock) SOS_FreeVec(memoryBlock,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __FreeVec(memoryBlock) SOS_FreeVec(memoryBlock,NULL,0)

		extern UWORD SOS_GetCC(char *file,unsigned long line);
		#define GetCC() SOS_GetCC(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __GetCC() SOS_GetCC(NULL,0)

		extern struct Message *SOS_GetMsg(struct MsgPort *port,char *file,unsigned long line);
		#define GetMsg(port) SOS_GetMsg(port,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __GetMsg(port) SOS_GetMsg(port,NULL,0)

		extern void SOS_InitCode(ULONG startClass,ULONG version,char *file,unsigned long line);
		#define InitCode(startClass,version) SOS_InitCode(startClass,version,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __InitCode(startClass,version) SOS_InitCode(startClass,version,NULL,0)

		extern APTR SOS_InitResident(struct Resident *resident,ULONG segList,char *file,unsigned long line);
		#define InitResident(resident,segList) SOS_InitResident(resident,segList,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __InitResident(resident,segList) SOS_InitResident(resident,segList,NULL,0)

		extern void SOS_InitSemaphore(struct SignalSemaphore *sigsem,char *file,unsigned long line);
		#define InitSeaphore(sigsem) SOS_InitSemaphore(sigsem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __InitSeaphore(sigsem) SOS_InitSemaphore(sigsem,NULL,0)

		extern void SOS_InitStruct(struct InitStruct *initTable,APTR memory,ULONG size,char *file,unsigned long line);
		#define InitStruct(initTable,memory,size) SOS_InitStruct(initTable,memory,size,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __InitStruct(initTable,memory,size) SOS_InitStruct(initTable,memory,size,NULL,0)

		extern void SOS_Insert(struct List *list, struct Node *node, struct Node *listNode,char *file,unsigned long line);
		#define Insert(list,node,listNode) SOS_Insert(list,node,listNode,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Insert(list,node,listNode) SOS_Insert(list,node,listNode,NULL,0)

		extern void SOS_MakeFunctions(APTR target,APTR functionArray,unsigned long funcDispBase,char *file,unsigned long line);
		#define MakeFunctions(target,functionArray,funcDispBase) SOS_MakeFunctions(target,functionArray,funcDispBase,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __MakeFunctions(target,functionArray,funcDispBase) SOS_MakeFunctions(target,functionArray,funcDispBase,NULL,0)

		extern struct Library *SOS_MakeLibrary(APTR funcInit,APTR structInit,unsigned long (*libInit)(),unsigned long dataSize,unsigned long segList,char *file,unsigned long line);
		#define MakeLibrary(funcInit,structInit,libInit,dataSize,segList) SOS_MakeLibrary(funcInit,structInit,libInit,dataSize,segList,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __MakeLibrary(funcInit,structInit,libInit,dataSize,segList) SOS_MakeLibrary(funcInit,structInit,libInit,dataSize,segList,NULL,0)

		extern ULONG SOS_ObtainQuickVector(APTR interruptCode,char *file,unsigned long line);
		#define ObtainQuickVector(interruptCode) SOS_ObtainQuickVector(interruptCode,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ObtainQuickVector(interruptCode) SOS_ObtainQuickVector(interruptCode,NULL,0)

		extern void SOS_ObtainSemaphore(struct SignalSemaphore *sigSem,char *file,unsigned long line);
		#define ObtainSemaphore(sigSem) SOS_ObtainSemaphore(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ObtainSemaphore(sigSem) SOS_ObtainSemaphore(sigSem,NULL,0)

		extern void SOS_ObtainSemaphoreList(struct List *sigSem,char *file,unsigned long line);
		#define ObtainSemaphoreList(sigSem) SOS_ObtainSemaphoreList(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ObtainSemaphoreList(sigSem) SOS_ObtainSemaphoreList(sigSem,NULL,0)

		extern void SOS_ObtainSemaphoreShared(struct SignalSemaphore *sigSem,char *file,unsigned long line);
		#define ObtainSemaphoreShared(sigSem) SOS_ObtainSemaphoreShared(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ObtainSemaphoreShared(sigSem) SOS_ObtainSemaphoreShared(sigSem,NULL,0)

		extern struct Library *SOS_OldOpenLibrary(UBYTE *libName,char *file,unsigned long line);
		#define OldOpenLibrary(libName) SOS_OldOpenLibrary(libName,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __OldOpenLibrary(libName) SOS_OldOpenLibrary(libName,NULL,0)

		extern BYTE SOS_OpenDevice(UBYTE *devName,unsigned long unit,struct IORequest *ioRequest,unsigned long flags,char *file,unsigned long line);
		#define OpenDevice(devName,unit,ioRequest,flags) SOS_OpenDevice(devName,unit,ioRequest,flags,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __OpenDevice(devName,unit,ioRequest,flags) SOS_OpenDevice(devName,unit,ioRequest,flags,NULL,0)

		extern struct Library *SOS_OpenLibrary(UBYTE *libName,unsigned long version,char *file,unsigned long line);
		#define OpenLibrary(libName,version) SOS_OpenLibrary(libName,version,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __OpenLibrary(libName,version) SOS_OpenLibrary(libName,version,NULL,0)

		extern APTR SOS_OpenResource(STRPTR resName,char *file,unsigned long line);
		#define OpenResource(resName) SOS_OpenResource(resName,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __OpenResource(resName) SOS_OpenResource(resName,NULL,0)

		extern void SOS_Permit(char *file,unsigned long line);
		#define Permit() SOS_Permit(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Permit() SOS_Permit(NULL,0)

		extern void SOS_Procure(struct SignalSemaphore *sigsem,struct SemaphoreMessage *semmsg,char *file,unsigned long line);
		#define Procure(sigsem,semmsg) SOS_Procure(sigsem,semmsg,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Procure(sigsem,semmsg) SOS_Procure(sigsem,semmsg,NULL,0)

		extern void SOS_PutMsg(struct MsgPort *port,struct Message *msg,char *file,unsigned long line);
		#define PutMsg(port,msg) SOS_PutMsg(port,msg,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __PutMsg(port,msg) SOS_PutMsg(port,msg,NULL,0)

		extern APTR SOS_RawDoFmt(UBYTE *formatString,APTR dataStream,void (*putChProc)(),APTR putChData,char *file,unsigned long line);
		#define RawDoFmt(formatString,dataStream,putChProc,putChData) SOS_RawDoFmt(formatString,dataStream,putChProc,putChData,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RawDoFmt(formatString,dataStream,putChProc,putChData) SOS_RawDoFmt(formatString,dataStream,putChProc,putChData,NULL,0)

		extern void SOS_ReleaseSemaphore(struct SignalSemaphore *sigSem,char *file,unsigned long line);
		#define ReleaseSemaphore(sigSem) SOS_ReleaseSemaphore(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ReleaseSemaphore(sigSem) SOS_ReleaseSemaphore(sigSem,NULL,0)

		extern void SOS_ReleaseSemaphoreList(struct List *sigSem,char *file,unsigned long line);
		#define ReleaseSemaphoreList(sigSem) SOS_ReleaseSemaphoreList(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ReleaseSemaphoreList(sigSem) SOS_ReleaseSemaphoreList(sigSem,NULL,0)

		extern void SOS_RemDevice(struct Device *device,char *file,unsigned long line);
		#define RemDevice(device) SOS_RemDevice(device,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemDevice(device) SOS_RemDevice(device,NULL,0)

		extern struct Node *SOS_RemHead(struct List *list,char *file,unsigned long line);
		#define RemHead(list) SOS_RemHead(list,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemHead(list) SOS_RemHead(list,NULL,0)

		extern void SOS_RemIntServer(long intNumber,struct Interrupt *interrupt,char *file,unsigned long line);
		#define RemIntServer(intNumber,interrupt) SOS_RemIntServer(intNumber,interrupt,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemIntServer(intNumber,interrupt) SOS_RemIntServer(intNumber,interrupt,NULL,0)

		extern void SOS_RemLibrary(struct Library *library,char *file,unsigned long line);
		#define RemLibrary(library) SOS_RemLibrary(library,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemLibrary(library) SOS_RemLibrary(library,NULL,0)

		extern void SOS_RemMemHandler(struct Interrupt *memhand,char *file,unsigned long line);
		#define RemMemHandler(memhand) SOS_RemMemHandler(memhand,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemMemHandler(memhand) SOS_RemMemHandler(memhand,NULL,0)

		extern void SOS_Remove(struct Node *node,char *file,unsigned long line);
		#define Remove(node) SOS_Remove(node,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Remove(node) SOS_Remove(node,NULL,0)

		extern void SOS_RemPort(struct MsgPort *port,char *file,unsigned long line);
		#define RemPort(port) SOS_RemPort(port,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemPort(port) SOS_RemPort(port,NULL,0)

		extern void SOS_RemResource(APTR resource,char *file,unsigned long line);
		#define RemResource(resource) SOS_RemResource(resource,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemResource(resource) SOS_RemResource(resource,NULL,0)

		extern void SOS_RemSemaphore(struct SignalSemaphore *sigSem,char *file,unsigned long line);
		#define RemSemaphore(sigSem) SOS_RemSemaphore(sigSem,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemSemaphore(sigSem) SOS_RemSemaphore(sigSem,NULL,0)

		extern struct Node *SOS_RemTail(struct List *list,char *file,unsigned long line);
		#define RemTail(list) SOS_RemTail(list,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemTail(list) SOS_RemTail(list,NULL,0)

		extern void SOS_RemTask(struct Task *task,char *file,unsigned long line);
		#define RemTask(task) SOS_RemTask(task,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __RemTask(task) SOS_RemTask(task,NULL,0)

		extern void SOS_ReplyMsg(struct Message *msg,char *file,unsigned long line);
		#define ReplyMsg(msg) SOS_ReplyMsg(msg,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ReplyMsg(msg) SOS_ReplyMsg(msg,NULL,0)

		extern void SOS_SendIO(struct IORequest *ioreq,char *file,unsigned long line);
		#define SendIO(ioreq) SOS_SendIO(ioreq,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SendIO(ioreq) SOS_SendIO(ioreq,NULL,0)

		extern ULONG SOS_SetExcept(ULONG newSignals,ULONG signalMask,char *file,unsigned long line);
		#define SetExcept(newSignals,signalMask) SOS_SetExecpt(newSignals,signalMask,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetExcept(newSignals,signalMask) SOS_SetExecpt(newSignals,signalMask,NULL,0)

		extern APTR SOS_SetFunction(struct Library *library,long funcOffset,unsigned long (*newFunction)(),char *file,unsigned long line);
		#define SetFunction(library,funcOffset,newFunction) SOS_SetFunction(library,funcOffset,newFunction,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetFunction(library,funcOffset,newFunction) SOS_SetFunction(library,funcOffset,newFunction,NULL,0)

		extern struct Interrupt *SOS_SetIntVector(long intNumber,struct Interrupt *interrupt,char *file,unsigned long line);
		#define SetIntVector(intNumber,interrupt) SOS_SetIntVector(intNumber,interrupt,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetIntVector(intNumber,interrupt) SOS_SetIntVector(intNumber,interrupt,NULL,0)

		extern ULONG SOS_SetSignal(unsigned long newSignals,unsigned long signalSet,char *file,unsigned long line);
		#define SetSignal(newSignals,signalSet) SOS_SetSignal(newSignals,signalSet,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetSignal(newSignals,signalSet) SOS_SetSignal(newSignals,signalSet,NULL,0)

		extern ULONG SOS_SetSR(unsigned long newSR,unsigned long mask,char *file,unsigned long line);
		#define SetSR(newSR,mask) SOS_SetSR(newSR,mask,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetSR(newSR,mask) SOS_SetSR(newSR,mask,NULL,0)

		extern BYTE SOS_SetTaskPri(struct Task *task,long priority,char *file,unsigned long line);
		#define SetTaskPri(task,priority) SOS_SetTaskPri(task,priority,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SetTaskPri(task,priority) SOS_SetTaskPri(task,priority,NULL,0)

		extern void SOS_Signal(struct Task *task,unsigned long signalSet,char *file,unsigned long line);
		#define Signal(task,signalSet) SOS_Signal(task,signalSet,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Signal(task,signalSet) SOS_Signal(task,signalSet,NULL,0)

		extern void SOS_StackSwap(struct StackSwapStruct *newStack,char *file,unsigned long line);
		#define StackSwap(newStack) SOS_StackSwap(newStack,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __StackSwap(newStack) SOS_StackSwap(newStack,NULL,0)

		extern ULONG SOS_SumKickData(char *file,unsigned long line);
		#define SumKickData() SOS_SumKickData(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SumKickData() SOS_SumKickData(NULL,0)

		extern void SOS_SumLibrary(struct Library *library,char *file,unsigned long line);
		#define SumLibrary(library) SOS_SumLibrary(library,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SumLibrary(library) SOS_SumLibrary(library,NULL,0)

		extern APTR SOS_SuperState(char *file,unsigned long line);
		#define SuperState() SOS_SuperState(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __SuperState() SOS_SuperState(NULL,0)

		extern ULONG SOS_Supervisor(unsigned long (*userFunction)(),char *file,unsigned long line);
		#define Supervisor(userFunction) SOS_Supervisor(userFunction,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Supervisor(userFunction) SOS_Supervisor(userFunction,NULL,0)

		extern void SOS_UserState(APTR sysStack,char *file,unsigned long line);
		#define UserState(sysStack) SOS_UserState(sysStack,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __UserState(sysStack) SOS_UserState(sysStack,NULL,0)

		extern void SOS_Vacate(struct SignalSemaphore *sigSem,struct SemaphoreMessage *bidMsg,char *file,unsigned long line);
		#define Vacate(sigSem,bidMsg) SOS_Vacate(sigSem,bidMsg,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Vacate(sigSem,bidMsg) SOS_Vacate(sigSem,bidMsg,NULL,0)

		extern ULONG SOS_Wait(unsigned long signalSet,char *file,unsigned long line);
		#define Wait(signalSet) SOS_Wait(signalSet,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __Wait(signalSet) SOS_Wait(signalSet,NULL,0)

		extern BYTE SOS_WaitIO(struct IORequest *ioRequest,char *file,unsigned long line);
		#define WaitIO(ioRequest) SOS_WaitIO(ioRequest,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __WaitIO(ioRequest) SOS_WaitIO(ioRequest,NULL,0)

		extern struct Message *SOS_WaitPort(struct MsgPort *port,char *file,unsigned long line);
		#define WaitPort(port) SOS_WaitPort(port,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __WaitPort(port) SOS_WaitPort(port,NULL,0)
	#endif
#else

	#define __AbortIO(ioreq)												AbortIO(ioreq)
	#define __AddDevice(device)												AddDevice(device)
	#define __AddHead(list,node)											AddHead(list,node)
	#define __AddIntServer(intNumber,interrupt)								AddIntServer(intNumber,interrupt)
	#define __AddLibrary(library)											AddLibrary(library)
	#define __AddMemHandler(memhand)										AddMemHandler(memhand)
	#define __AddMemList(size,attributes,pri,base,name)						AddMemList(size,attributes,pri,base,name)
	#define __AddPort(port)													AddPort(port)
	#define __AddResource(resource)											AddResource(resource)
	#define __AddSemaphore(sigSem)											AddSemaphore(sigSem)
	#define __AddTail(list,node)											AddTail(list,node)
	#define __AddTask(task,initialPC,finalPC)								AddTask(task,initialPC,finalPC)
	#define __Alert(alertNum)												Alert(alertNum)
	#define __AllocAbs(byteSize,location)									AllocAbs(byteSize,location)
	#define __Allocate(freeList,byteSize)									Allocate(freeList,byteSize)
	#define __AllocEntry(entry)												AllocEntry(entry)
	#define __AllocMem(byteSize,requirements)								AllocMem(byteSize,requirements)
	#define __AllocPooled(poolHeader,memSize)								AllocPooled(poolHeader,memSize)
	#define __AllocSignal(signalNum)										AllocSignal(signalNum)
	#define __AllocTrap(trapNum)											AllocTrap(trapNum)
	#define __AllocVec(byteSize,requirements)								AllocVec(byteSize,requirements)
	#define __AttemptSemaphore(signalSemaphore)								AttemptSemaphore(signalSemaphore)
	#define __AttemptSemaphoreShared(signalSemaphore)						AttemptSemaphoreShared(signalSemaphore)
	#define __AvailMem(attributes)											AvailMem(attributes)
	#define __CacheClearE(address,length,caches)							CacheClearE(address,length,caches)
	#define __CacheClearU()													CacheClearU()
	#define __CacheControl(cacheBits,cacheMask)								CacheControl(cacheBits,cacheMask)
	#define __CachePostDMA(vaddress,length,flags)							CachePostDMA(vaddress,length,flags)
	#define __CachePreDMA(vaddress,length,flags)							CachePreDMA(vaddress,length,flags)
	#define __Cause(interrupt)												Cause(interrupt)
	#define __CheckIO(ioreq)												CheckIO(ioreq)
	#define __CloseDevice(ioRequest)										CloseDevice(ioRequest)
	#define __CloseLibrary(library)											CloseLibrary(library)
	#define __ColdReboot()													ColdReboot()
	#define __CopyMem(source,dest,size)										CopyMem(source,dest,size)
	#define __CopyMemQuick(source,dest,size)								CopyMemQuick(source,dest,size)
	#define __CreateIORequest(port,size)									CreateIORequest(port,size)
	#define __CreateMsgPort()												CreateMsgPort()
	#define __CreatePool(requirements,puddleSize,threshSize)				CreatePool(requirements,puddleSize,threshSize)
	#define __Deallocate(freeList,memoryBlock,byteSize)						Deallocate(freeList,memoryBlock,byteSize)
	#define __Debug(flags)													Debug(flags)
	#define __DeleteIORequest(ioRequest)									DeleteIORequest(ioRequest)
	#define __DeleteMsgPort(port)											DeleteMsgPort(port)
	#define __DeletePool(poolHeader)										DeletePool(poolHeader)
	#define __Disable()														Disable()
	#define __DoIO(ioreq)													DoIO(ioreq)
	#define __Enable()														Enable()
	#define __Enqueue(list,node)											Enqueue(list,node)
	#define __FindName(start,name)											FindName(start,name)
	#define __FindPort(name)												FindPort(name)
	#define __FindResident(name)											FindResident(name)
	#define __FindSemaphore(name)											FindSemaphore(name)
	#define __Forbid()														Forbid()
	#define __FreeEntry(entry)												FreeEntry(entry)
	#define __FreeMem(memoryBlock,byteSize)									FreeMem(memoryBlock,byteSize)
	#define __FreePooled(poolHeader,memory,memSize)							FreePooled(poolHeader,memory,memSize)
	#define __FreeSignal(signalNum)											FreeSignal(signalNum)
	#define __FreeTrap(trapNum)												FreeTrap(trapNum)
	#define __FreeVec(memoryBlock)											FreeVec(memoryBlock)
	#define __GetCC()														GetCC()
	#define __GetMsg(port)													GetMsg(port)
	#define __InitCode(startClass,version)									InitCode(startClass,version)
	#define __InitResident(resident,segList)								InitResident(resident,segList)
	#define __InitSeaphore(sigsem)											InitSemaphore(sigsem)
	#define __InitStruct(initTable,memory,size)								InitStruct(initTable,memory,size)
	#define __Insert(list,node,listNode)									Insert(list,node,listNode)
	#define __MakeFunctions(target,functionArray,funcDispBase)				MakeFunctions(target,functionArray,funcDispBase)
	#define __MakeLibrary(funcInit,structInit,libInit,dataSize,segList)		MakeLibrary(funcInit,structInit,libInit,dataSize,segList)
	#define __ObtainQuickVector(interruptCode)								ObtainQuickVector(interruptCode)
	#define __ObtainSemaphore(sigSem)										ObtainSemaphore(sigSem)
	#define __ObtainSemaphoreList(sigSem)									ObtainSemaphoreList(sigSem)
	#define __ObtainSemaphoreShared(sigSem)									ObtainSemaphoreShared(sigSem)
	#define __OldOpenLibrary(libName)										OldOpenLibrary(libName)
	#define __OpenDevice(devName,unit,ioRequest,flags)						OpenDevice(devName,unit,ioRequest,flags)
	#define __OpenLibrary(libName,version)									OpenLibrary(libName,version)
	#define __OpenResource(resName)											OpenResource(resName)
	#define __Permit()														Permit()
	#define __Procure(sigsem,semmsg)										Procure(sigsem,semmsg)
	#define __PutMsg(port,msg)												PutMsg(port,msg)
	#define __RawDoFmt(formatString,dataStream,putChProc,putChData)			RawDoFmt(formatString,dataStream,putChProc,putChData)
	#define __ReleaseSemaphore(sigSem)										ReleaseSemaphore(sigSem)
	#define __ReleaseSemaphoreList(sigSem)									ReleaseSemaphoreList(sigSem)
	#define __RemDevice(device)												RemDevice(device)
	#define __RemHead(list)													RemHead(list)
	#define __RemIntServer(intNumber,interrupt)								RemIntServer(intNumber,interrupt)
	#define __RemLibrary(library)											RemLibrary(library)
	#define __RemMemHandler(memhand)										RemMemHandler(memhand)
	#define __Remove(node)													Remove(node)
	#define __RemPort(port)													RemPort(port)
	#define __RemResource(resource)											RemResource(resource)
	#define __RemSemaphore(sigSem)											RemSemaphore(sigSem)
	#define __RemTail(list)													RemTail(list)
	#define __RemTask(task)													RemTask(task)
	#define __ReplyMsg(msg)													ReplyMsg(msg)
	#define __SendIO(ioreq)													SendIO(ioreq)
	#define __SetExcept(newSignals,signalMask)								SetExecpt(newSignals,signalMask)
	#define __SetFunction(library,funcOffset,newFunction)					SetFunction(library,funcOffset,newFunction)
	#define __SetIntVector(intNumber,interrupt)								SetIntVector(intNumber,interrupt)
	#define __SetSignal(newSignals,signalSet)								SetSignal(newSignals,signalSet)
	#define __SetSR(newSR,mask)												SetSR(newSR,mask)
	#define __SetTaskPri(task,priority)										SetTaskPri(task,priority)
	#define __Signal(task,signalSet)										Signal(task,signalSet)
	#define __StackSwap(newStack)											StackSwap(newStack)
	#define __SumKickData()													SumKickData()
	#define __SumLibrary(library)											SumLibrary(library)
	#define __SuperState()													SuperState()
	#define __Supervisor(userFunction)										Supervisor(userFunction)
	#define __UserState(sysStack)											UserState(sysStack)
	#define __Vacate(sigSem,bidMsg)											Vacate(sigSem,bidMsg)
	#define __Wait(signalSet)												Wait(signalSet)
	#define __WaitIO(ioRequest)												WaitIO(ioRequest)
	#define __WaitPort(port)												WaitPort(port)

#endif

//-- eof ----------------------------------------------------------------------
