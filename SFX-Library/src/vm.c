/****h* sfxsupport.library/vm.c [4.2] *
*
*  NAME
*    vm.c
*  COPYRIGHT
*    $VER: vm.c 4.2 (06.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    virtual memory routines -- definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    20.Jul.1998
*  MODIFICATION HISTORY
*    06.Mar.2003	V 4.2	most recent version
*    12.Nov.2000	V 4.1	most recent version
*    23.Feb.2000	V 1.3	most recent version
*    31.Oct.1998	V 1.2	most recent version
*    20.Jul.1998	V 1.1	initial version
*  NOTES
*
*******
*/

#define VM_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- VM support

UBYTE ASM LIB_EXPORT(ConvMem2Drv(REG(a0,SInfo *si)));
UBYTE ASM LIB_EXPORT(ConvDrv2Mem(REG(a0,SInfo *si)));

SmpAccess ASM *LIB_EXPORT(SAInit(REG(a0,SInfo *si),REG(d0,UBYTE chan),REG(d1,ULONG start), REG(d2,ULONG len),REG(d3,UBYTE vm_mode),REG(d4,ULONG blklen)));
void ASM LIB_EXPORT(SADone(REG(a0,SmpAccess *sa),REG(a1,SInfo *si)));

UBYTE ASM LIB_EXPORT(CopyData(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,ULONG srcpos),REG(d1,ULONG dstpos),REG(d2,ULONG len)));
UBYTE ASM LIB_EXPORT(CopyDataCh(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,UBYTE ch),REG(d1,ULONG srcpos),REG(d2,ULONG dstpos),REG(d3,ULONG len)));

void ASM LIB_EXPORT(ExchangeData(REG(a0,SInfo *src),REG(a1,SInfo *dst)));

void ASM LIB_EXPORT(RecalcLoopPlayData(void));

//-- vmem-sample recovery

void SaveSInfo(SInfo *si);
void RemoveSInfo(SInfo *si);
void RenameSInfo(SInfo *src,SInfo *dst);

//-- helper

UBYTE CopyDataSub(void *src,void *dst,UWORD mode,ULONG len);
STRPTR MakeVMemName(SInfo *si,UBYTE channel);
STRPTR MakeSInfoName(SInfo *si);
void SafeRemove(STRPTR fn);
void SafeRename(STRPTR sfn,STRPTR dfn);
void SafeClose(int fh);

//-- IO functions

UBYTE ASM SA_MemRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length));
UBYTE ASM SA_MemWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length));

UBYTE ASM SA_StreamRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length));
UBYTE ASM SA_StreamWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length));

// drive io is defined in asyncio.c|h

//-- definitions --------------------------------------------------------------

//-- VM support

UBYTE SAVEDS ASM LIB_EXPORT(ConvMem2Drv(REG(a0,SInfo *si))) {
	register ULONG i;
	register UBYTE l;
	char fn[FILENAME_MAX];
	int swf;
	UBYTE fail=FALSE;
	LONG w;
	BYTE sample[256]={0};

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(si->storage==VM_MEMORY);

	if(si->storage==VM_MEMORY) {
		for(l=0;l<CH_COUNT(si->channels);l++) {				// swap all data to disk
			strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
			w=0;
			if((swf=open(fn,O_RDWR|O_CREAT,0))!=-1) {
				if((write(swf,si->buf[l],Samples2Bytes(si->slen)))==-1) { fail=TRUE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
				for(i=0;(i<(GetRunTime(sfxprefs_vmem)->blksize>>8) && w!=-1);i++) w=write(swf,&sample,256);		// make some space for read's over the end
				if(w==-1) { fail=TRUE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
				SafeClose(swf);
			}
			else { fail=TRUE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
		}
		if(!fail) {									// hat geklappt
			for(l=0;l<CH_COUNT(si->channels);l++) { FreeVec(si->memory[l]);si->memory[l]=si->buf[l]=NULL; }
			si->memlen=0;
			si->storage=VM_DRIVE;
			si->vbuf[0]=si->vbuf[1]=si->vbuf[2]=si->vbuf[3]=NULL;
			SaveSInfo(si);
		}
		else {
			for(l=0;l<CH_COUNT(si->channels);l++) {			// erase all files produced until now
				strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
				SafeRemove(fn);
			}
		}
	}
	else {
		ERR1("Called ConvMem2Drv() for sample with storagetype=%ld",si->storage);
	}
	OUTRO;
	return((UBYTE)(!fail));
}

UBYTE SAVEDS ASM LIB_EXPORT(ConvDrv2Mem(REG(a0,SInfo *si))) {
	register UBYTE l;
	char fn[FILENAME_MAX];
	int swf;
	UBYTE fail=FALSE;
	ULONG vlen;
	
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERTQ(si->storage==VM_DRIVE);

	if(si->storage==VM_DRIVE) {
		vlen=(((si->slen>>14)+1)<<14);
		for(l=0;l<CH_COUNT(si->channels);l++) {				// read all data from disk
			if((si->memory[l]=AllocVec(Samples2Bytes(vlen),MEMF_ANY|MEMF_CLEAR))) {
				si->buf[l]=si->memory[l];
				strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
				if((swf=open(fn,O_RDONLY,0))!=-1) {
					if((read(swf,si->buf[l],Samples2Bytes(si->slen)))==-1) { fail=TRUE;l=CH_MAX;LIB_INTCALL(Message(errReadData,NULL,__FILE__,__LINE__)); }
					SafeClose(swf);
				}
				else { fail=TRUE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
			}
			else { fail=TRUE;l=CH_MAX;LIB_INTCALL(Message(errAllocateMemory,NULL,__FILE__,__LINE__)); }
		}
		if(!fail) {									// hat geklappt
			si->memlen=vlen;
			for(l=0;l<CH_COUNT(si->channels);l++)	{		// erase all files
				strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
				SafeRemove(fn);
			}
			RemoveSInfo(si);
			si->storage=VM_MEMORY;
			SInfo_SetVBuf(si,CH_COUNT(si->channels));
		}
		else {
			for(l=0;l<CH_COUNT(si->channels);l++) {
				if(si->memory[l]) { FreeVec(si->memory[l]);si->memory[l]=NULL; }
			}
		}
	}
	else {
		ERR1("Called ConvDrv2Mem() for sample with storagetype=%ld",si->storage);
	}
	OUTRO;
	return((UBYTE)(!fail));
}

//
// REG(a0) SInfo si			Sample to initiate access for
// REG(d0) UBYTE chan		Channel to work with
// REG(d1) ULONG start		Offset to process
// REG(d2) ULONG len		Length to process
// REG(d3) UBYTE vm_mode	R/W dir/blk

SmpAccess SAVEDS ASM *LIB_EXPORT(SAInit(REG(a0,SInfo *si),REG(d0,UBYTE chan),REG(d1,ULONG start), REG(d2,ULONG len),REG(d3,UBYTE vm_mode),REG(d4,ULONG blklen))) {
	SmpAccess *sa;
	UBYTE fail=FALSE;
	//ULONG st2=start+len;
	char fn[FILENAME_MAX];

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERT(si->storage!=VM_NODATA);
	
	if((sa=AllocVec(sizeof(SmpAccess),MEMF_ANY|MEMF_CLEAR))) {			// allocate command structure
		sa->a4=__builtin_getreg(REG_A4);
		sa->a6=__builtin_getreg(REG_A6);
		sa->mainsig=sa->worksig=-1;
		sa->start=Samples2Bytes(start);
		sa->vm_mode=vm_mode;
		sa->state=STATE_READY;
		sa->seek=-1;
		switch(si->storage) {
			case VM_MEMORY:
				sa->buffer4=(BYTE *)si->buf[chan];	// für seeks
				switch(vm_mode&VM_MODE_DIR_BLK) {				   // dir/blk
					case VM_MODE_DIR:
						sa->cblen=0;
						sa->blen=Samples2Bytes(len);								// zu bearbeitender Bereich
						if((vm_mode&VM_MODE_CBLK_VBLK)==VM_MODE_CBLK) {
							MSG(" Memory direct cblk");
							sa->seganz=1;								// nur ein Block, PreRead
							switch(vm_mode&VM_MODE_READ_WRITE) {			   // read / write
								case VM_MODE_READ:
									sa->safunc=SA_MemRead;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=&sa->buffer1[sa->blen];
									break;
								case VM_MODE_WRITE:
									sa->safunc=SA_MemWrite;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=NULL;									// unused
									break;
								UNEXPECTED_DEFAULT
							}
						}
						else {
							MSG(" Memory direct vblk");
							sa->seganz=0;								// -> kein PreRead
							switch(vm_mode&VM_MODE_READ_WRITE) {			   // read / write
								case VM_MODE_READ:
									sa->safunc=SA_MemRead;
									sa->buffer1=NULL;									// direkt Daten zurückgeben
									sa->buffer2=(BYTE *)(&si->vbuf[chan][start]);
									break;
								case VM_MODE_WRITE:
									sa->safunc=SA_MemWrite;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=NULL;									// unused
									break;
								UNEXPECTED_DEFAULT
							}
						}
						break;
					case VM_MODE_BLK:
						sa->cblen=0;
						if(blklen) sa->blen=blklen;								// zu bearbeitender Bereich
						else sa->blen=GetRunTime(sfxprefs_vmem)->blksize;				// default-value
						if((vm_mode&VM_MODE_CBLK_VBLK)==VM_MODE_CBLK) {
							MSG("  Memory blocked cblk");
							sa->seganz=Samples2Bytes(len)/sa->blen;				// Anzahl der Segmente, PreRead
							if((sa->seganz*sa->blen)<Samples2Bytes(len)) sa->seganz++;
							switch(vm_mode&VM_MODE_READ_WRITE) {			   // read / write
								case VM_MODE_READ:
									sa->safunc=SA_MemRead;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=&sa->buffer1[sa->blen];
									break;
								case VM_MODE_WRITE:
									sa->safunc=SA_MemWrite;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=NULL;									// unused
									break;
								UNEXPECTED_DEFAULT
							}
						}
						else {
							MSG("  Memory blocked vblk");
							sa->seganz=0;								// -> kein PreRead
							switch(vm_mode&VM_MODE_READ_WRITE) {			   // read / write
								case VM_MODE_READ:
									sa->safunc=SA_MemRead;
									sa->buffer1=NULL;									// direkt Daten zurückgeben
									sa->buffer2=(BYTE *)(&si->vbuf[chan][start]);
									break;
								case VM_MODE_WRITE:
									sa->safunc=SA_MemWrite;
									sa->buffer1=(BYTE *)(&si->vbuf[chan][start]);	// direkt Daten zurückgeben
									sa->buffer2=NULL;									// unused
									break;
								UNEXPECTED_DEFAULT
							}
						}
						// already done for cblk and should be 0 for vblk
						//if((sa->seganz*sa->blen)<Samples2Bytes(len)) sa->seganz++;
						break;
					UNEXPECTED_DEFAULT
				}
				break;
			case VM_DRIVE:
				switch(vm_mode&VM_MODE_DIR_BLK) {				   // dir/blk
					case VM_MODE_DIR:
						MSG("  Drive direct");
						sa->cblen=0;
						sa->blen=Samples2Bytes(len);								// zu bearbeitender Bereich
						sa->seganz=1;									// nur ein Block
						strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,chan));
						switch(vm_mode&VM_MODE_READ_WRITE) {		   // read / write
							case VM_MODE_READ:
								if((sa->fh=open(fn,O_RDONLY,0))==-1) fail=1;
								else lseek(sa->fh,sa->start,SEEK_SET);
								break;
							case VM_MODE_WRITE:
								if((sa->fh=open(fn,O_RDWR|O_CREAT,0))==-1) fail=1;
								else lseek(sa->fh,sa->start,SEEK_SET);
								break;
							UNEXPECTED_DEFAULT
						}
						break;
					case VM_MODE_BLK:
						MSG("  Drive blocked");
						sa->cblen=0;
						if(blklen) sa->blen=blklen;			 					// zu bearbeitender Bereich
						else sa->blen=GetRunTime(sfxprefs_vmem)->blksize;				// default-value
						if((vm_mode&VM_MODE_CBLK_VBLK)==VM_MODE_CBLK) {
							sa->seganz=Samples2Bytes(len)/sa->blen;	// Anzahl der Segmente
							if((sa->seganz*sa->blen)<Samples2Bytes(len)) sa->seganz++;
						}
						else sa->seganz=0;											// -> kein PreRead
						if(!(sa->buffer1=AllocVec(sa->blen,MEMF_ANY|MEMF_CLEAR))) fail=1;
						if(!(sa->buffer2=AllocVec(sa->blen,MEMF_ANY|MEMF_CLEAR))) fail=1;
						if(!fail) {
							strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,chan));
							switch(vm_mode&VM_MODE_READ_WRITE) {			   // read / write
								case VM_MODE_READ:
									if(!LIB_INTCALL(ASOpen(sa,fn,O_RDONLY))) {
										//-- for playing Loops (ideally we only need this when playing, not when drawing)
										if(si->loop) if(!(sa->buffer3=AllocVec(sa->blen,MEMF_ANY|MEMF_CLEAR))) fail=TRUE;
										if(vm_mode&VM_MODE_IRQ) sa->safunc=LIB_INTCALL(ASReadIRQ);
										else sa->safunc=LIB_INTCALL(ASRead);
									}
									else fail=TRUE;
									break;
								case VM_MODE_WRITE:
									if(!LIB_INTCALL(ASOpen(sa,fn,O_RDWR|O_CREAT|O_APPEND))) {
										//if(vm_mode&VM_MODE_IRQ) sa->safunc=LIB_INTCALL(ASWriteIRQ);
										//else sa->safunc=LIB_INTCALL(ASWrite);
										sa->safunc=LIB_INTCALL(ASWrite);
									}
									else fail=TRUE;
									break;
								UNEXPECTED_DEFAULT
							}
							//if(!fail) lseek(sa->fh,sa->start,MODE_START);	// wird schon beim Öffnen gemacht
						}
						break;
					UNEXPECTED_DEFAULT
				}
				break;
			case VM_STREAM:
				MSG("  Stream");
				sa->blen=Samples2Bytes(((StorageStream *)(si->storageinfo))->blen);
				sa->seganz=Samples2Bytes(len)/sa->blen;				// Anzahl der Segmente, PreRead
				if((sa->seganz*sa->blen)<Samples2Bytes(len)) sa->seganz++;
				((StorageStream *)(si->storageinfo))->state=&sa->state;
				// fx-task
				sa->worktask=((StorageStream *)(si->storageinfo))->worktask=(struct Process *)FindTask(NULL);
				sa->worksig=((StorageStream *)(si->storageinfo))->worksig=AllocSignal(-1);
				// adapter-task
				sa->maintask=((StorageStream *)(si->storageinfo))->maintask;
				sa->mainsig=((StorageStream *)(si->storageinfo))->mainsig;
				//MSG1("    info : 0x%08lx",((StorageStream *)(si->storageinfo)));
				//MSG2("    main : 0x%08lx %ld",sa->maintask,sa->mainsig);
				//MSG2("    work : 0x%08lx %ld",sa->worktask,sa->worksig);
				switch(vm_mode&VM_MODE_READ_WRITE) {		   // read / write
					case VM_MODE_READ:
						sa->safunc=SA_StreamRead;
						sa->buffer1=(BYTE *)(si->vbuf[chan]);	// direkt Daten zurückgeben
						sa->buffer2=(BYTE *)(si->vbuf[chan]);
						break;
					case VM_MODE_WRITE:
						sa->safunc=SA_StreamWrite;
						sa->buffer1=(BYTE *)(si->vbuf[chan]);	// direkt Daten zurückgeben
						sa->buffer2=(BYTE *)(si->vbuf[chan]);
						break;
					UNEXPECTED_DEFAULT
					}
				break;
			UNEXPECTED_DEFAULT
		}
		sa->blensh=0;
		while((1L<<sa->blensh)<sa->blen) sa->blensh++;	// log2(sa->blen)
		if(fail) { LIB_INTCALL(SADone(sa,si));sa=NULL; }
		else {
			// when using VM_DRIVE and VM_MODE_DIR thn safunc is null
			//ASSERT_RESET;
			//ASSERTQ(IS_VALID_POINTER_N0(sa->safunc,__FUNC__,"sa->safunc",4));
			//MSG4("  sa-buffer1,2 0x%08lx 0x%08lx  seganz=%ld blen=%ld",sa->buffer1,sa->buffer2,sa->seganz,sa->blen);
		}
	}
	OUTRO_PTR(sa);
	return(sa);
}

void SAVEDS ASM LIB_EXPORT(SADone(REG(a0,SmpAccess *sa),REG(a1,SInfo *si))) {
	//INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"2.par",4));
	
	if(sa) {
		switch(si->storage) {
			case VM_MEMORY:
				break;
			case VM_DRIVE:
				switch(sa->vm_mode&VM_MODE_DIR_BLK) {
					case VM_MODE_DIR:
						if(sa->fh!=-1) SafeClose(sa->fh);
						break;
					case VM_MODE_BLK:
						LIB_INTCALL(ASClose(sa));
						FreeVec(sa->buffer1);
						FreeVec(sa->buffer2);
						FreeVec(sa->buffer3);
						break;
					UNEXPECTED_DEFAULT
				}
				break;
			case VM_STREAM:
				FreeSignal(sa->worksig);
				break;
			UNEXPECTED_DEFAULT
		}
		FreeVec(sa);
	}
	else {
		ERR("called with sa==NULL");
	}
	//OUTRO;
}

UBYTE SAVEDS ASM LIB_EXPORT(CopyData(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,ULONG srcpos),REG(d1,ULONG dstpos),REG(d2,ULONG len))) {
	register UBYTE l;
	SmpAccess *srcsa,*dstsa;
	WORD mode=0;
	UBYTE okay=TRUE;
	
	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER(dst,__FUNC__,"2.par",4));

	if(src) mode|=((src->storage)<<8);
	else mode|=(VM_NODATA<<8);
	if(dst) mode|=(dst->storage);
	else mode|=VM_NODATA;
	MSG4("  mode 0x%04lx  srcpos %5ld  dstpos %5ld  len %5ld",mode,srcpos,dstpos,len);

	switch(mode) {
		case CPD_NIL2MEM:		// clear part in memory
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				okay=CopyDataSub(NULL,(void *)(&dst->vbuf[l][dstpos]),CPD_NIL2MEM,len);
			}
			break;
		case CPD_NIL2DRV:		// clear part in file
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				if((dstsa=LIB_INTCALL(SAInit(dst,l,dstpos,len,VM_MODE_WRITE|VM_MODE_BLK,VM_DEFAULT_BLKLEN)))) {
					okay=CopyDataSub(NULL,(void *)dstsa,CPD_NIL2DRV,len);
					LIB_INTCALL(SADone(dstsa,dst));
				}
				else {
					MSG("SAInit() failed");
					okay=FALSE;
				}
			}
			break;
		case CPD_MEM2MEM:
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				okay=CopyDataSub((void *)(&src->vbuf[l][srcpos]),(void *)(&dst->vbuf[l][dstpos]),CPD_MEM2MEM,len);
			}
			break;
		case CPD_MEM2DRV:
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				if((dstsa=LIB_INTCALL(SAInit(dst,l,dstpos,len,VM_MODE_WRITE|VM_MODE_DIR,VM_DEFAULT_BLKLEN)))) {
					okay=CopyDataSub((void *)(&src->vbuf[l][srcpos]),(void *)dstsa,CPD_MEM2DRV,len);
					LIB_INTCALL(SADone(dstsa,dst));
				}
				else {
					MSG("SAInit() failed");
					okay=FALSE;
				}
			}
			break;
		case CPD_DRV2MEM:
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				if((srcsa=LIB_INTCALL(SAInit(src,l,srcpos,len,VM_MODE_READ|VM_MODE_DIR,VM_DEFAULT_BLKLEN)))) {
					okay=CopyDataSub((void *)srcsa,(void *)(&dst->vbuf[l][dstpos]),CPD_DRV2MEM,len);
					LIB_INTCALL(SADone(srcsa,src));
				}
				else {
					MSG("SAInit() failed");
					okay=FALSE;
				}
			}
			break;
		case CPD_DRV2DRV:
			for(l=0;((l<CH_COUNT(dst->channels)) && okay);l++) {
				srcsa=LIB_INTCALL(SAInit(src,l,srcpos,len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN));
				dstsa=LIB_INTCALL(SAInit(dst,l,dstpos,len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN));
				if(srcsa && dstsa) {
					okay=CopyDataSub((void *)srcsa,(void *)dstsa,CPD_DRV2DRV,len);
				}
				else {
					MSG("SAInit() failed");
					okay=FALSE;
				}
				if(srcsa) LIB_INTCALL(SADone(srcsa,src));
				if(dstsa) LIB_INTCALL(SADone(dstsa,dst));
			}
			break;
		UNEXPECTED_DEFAULT
	}
	OUTRO_BOOL(okay);
	return(okay);
}

UBYTE SAVEDS ASM LIB_EXPORT(CopyDataCh(REG(a0,SInfo *src),REG(a1,SInfo *dst),REG(d0,UBYTE ch),REG(d1,ULONG srcpos),REG(d2,ULONG dstpos),REG(d3,ULONG len))) {
	SmpAccess *srcsa,*dstsa;
	WORD mode;
	UBYTE okay=TRUE;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER(dst,__FUNC__,"2.par",4));

	mode=((src->storage)<<8)|(dst->storage);
	//MSG1("mode 0x%04lx",mode);

	switch(mode) {
		case CPD_MEM2MEM:
			ERRC1((src->vbuf[ch]==NULL),"NULL src->vbuf[%1d]",ch);
			ERRC1((dst->vbuf[ch]==NULL),"NULL dst->vbuf[%1d]",ch);
			okay=CopyDataSub((void *)(&src->vbuf[ch][srcpos]),(void *)(&dst->vbuf[ch][dstpos]),CPD_MEM2MEM,len);
			break;
		case CPD_MEM2DRV:
			if((dstsa=LIB_INTCALL(SAInit(dst,ch,dstpos,len,VM_MODE_WRITE|VM_MODE_DIR,VM_DEFAULT_BLKLEN)))) {
				okay=CopyDataSub((void *)(&src->vbuf[ch][srcpos]),(void *)dstsa,CPD_MEM2DRV,len);
				LIB_INTCALL(SADone(dstsa,dst));
			}
			else {
				MSG("SAInit() failed");
				okay=FALSE;
			}
			break;
		case CPD_DRV2MEM:
			if((srcsa=LIB_INTCALL(SAInit(src,ch,srcpos,len,VM_MODE_READ|VM_MODE_DIR,VM_DEFAULT_BLKLEN)))) {
				okay=CopyDataSub((void *)srcsa,(void *)(&dst->vbuf[ch][dstpos]),CPD_DRV2MEM,len);
				LIB_INTCALL(SADone(srcsa,src));
			}
			else {
				MSG("SAInit() failed");
				okay=FALSE;
			}
			break;
		case CPD_DRV2DRV:
			srcsa=LIB_INTCALL(SAInit(src,ch,srcpos,len,VM_MODE_READ |VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN));
			dstsa=LIB_INTCALL(SAInit(dst,ch,dstpos,len,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK,VM_DEFAULT_BLKLEN));
			if(srcsa && dstsa) {
				okay=CopyDataSub((void *)srcsa,(void *)dstsa,CPD_DRV2DRV,len);
			}
			else {
				MSG("SAInit() failed");
				okay=FALSE;
			}
			if(srcsa) LIB_INTCALL(SADone(srcsa,src));
			if(dstsa) LIB_INTCALL(SADone(dstsa,dst));
			break;
		UNEXPECTED_DEFAULT
	}
	OUTRO_BOOL(okay);
	return(okay);
}

/****** sfxsupport.library/ExchangeData [4.2] *
*
*******
*/
void SAVEDS ASM LIB_EXPORT(ExchangeData(REG(a0,SInfo *src),REG(a1,SInfo *dst))) {
	// always access dst->... , because src->... is not initialized fully (our temp buffer)
	register UBYTE l;
	char fn1[FILENAME_MAX],fn2[FILENAME_MAX];

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(dst,__FUNC__,"2.par",4));

	LIB_INTCALL(FreeSampleMem(dst));
	//MSG("  old resoures freed");
	dst->storage	=src->storage;
	dst->memlen	    =src->memlen;
	dst->memoffset	=src->memoffset;
	//-- we have to do that by ourselves
	//dst->channels	=src->channels;
	//dst->channelmask=src->channelmask;
	//MSG1("  src->storage : %ld",src->storage);
	switch(src->storage) {
		case VM_MEMORY:								// copy memory
			MSG1("MEM : chan %ld",CH_COUNT(dst->channels));
			for(l=0;l<CH_COUNT(src->channels);l++) {
				dst->memory[l]=src->memory[l];
				src->memory[l]=NULL;
			}
			break;
		case VM_DRIVE:
			/** @TODO: check without the Delay() <------------------------------ !!! */
			Delay(10);
			MSG1("DRV : chan %ld",CH_COUNT(dst->channels));
			for(l=0;l<CH_COUNT(src->channels);l++) {		  // erase all files
				strmfp(fn1,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(src,l));
				strmfp(fn2,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(dst,l));
				SafeRename(fn1,fn2);
			}
			SaveSInfo(dst);
			break;
		UNEXPECTED_DEFAULT
	}
	//-- let buffers point to memory
	dst->buf[0]=dst->memory[0];
	dst->buf[1]=dst->memory[1];
	dst->buf[2]=dst->memory[2];
	dst->buf[3]=dst->memory[3];
	//-- do channel mapping
	SInfo_SetVBuf(dst,CH_COUNT(dst->channels));

	src->storage=VM_NODATA;
	OUTRO;
}


// leave here because of MakeVMemName() !
void SAVEDS ASM LIB_EXPORT(RecalcLoopPlayData(void)) {
	register UBYTE l;
	char fn[FILENAME_MAX];
	int swf;

	if(GetRunTime(play) && GetRunTime(aktbuf)==GetRunTime(plsi)) {
		GetRunTime(pllstart)=GetRunTime(plsi)->loops;
		GetRunTime(plslen)=GetRunTime(plsi)->loops+GetRunTime(plsi)->loopl;
		GetRunTime(pldiff)=GetRunTime(plslen)-GetRunTime(pllstart); // obsolete soon, [why ?]

		if(GetRunTime(plsa[0])) {						// because of the AHI-Memplayer, which has direct memoryaccess
			switch(GetRunTime(plsi)->storage) {		  // read 1.st block from loop-start
				case VM_MEMORY:
					for(l=0;l<CH_COUNT(GetRunTime(plsi)->channels);l++) GetRunTime(plsa[l])->buffer3=(BYTE *)&GetRunTime(plsi)->buf[l][GetRunTime(plsi)->loops];
					break;
				case VM_DRIVE:
					for(l=0;l<CH_COUNT(GetRunTime(plsi)->channels);l++) {
						strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(GetRunTime(plsi),l));
						if((swf=open(fn,O_RDONLY,0))!=-1) {
							lseek(swf,Samples2Bytes(GetRunTime(plsi)->loops),SEEK_SET);
							if((read(swf,GetRunTime(plsa[l])->buffer3,GetRunTime(plsa[l])->blen))==-1) LIB_INTCALL(Message(errReadData,NULL,__FILE__,__LINE__));
							SafeClose(swf);
						}
						else LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__));
					}
					break;
				UNEXPECTED_DEFAULT
			}
		}
	}
}

//-- vmem-sample recovery

void SaveSInfo(SInfo *si) {
	char fn[FILENAME_MAX];
	APTR cfg;

	// function needs to be called, when samples options have been changed (length, srat, name, ...)

	strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeSInfoName(si));

	if(cfg=CfgIO_NewConfig("SInfo"," %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s")) {
		if(si->node.ln_Name) {
			CfgIO_WriteString         (cfg,"SInfo","name"    ,si->node.ln_Name);
		}
		else {
			ERR("no name in sample info");
			CfgIO_WriteString         (cfg,"SInfo","name"    ,"unnamed");
		}
		CfgIO_WriteUnsignedInteger(cfg,"SInfo","slen"    ,si->slen);
		CfgIO_WriteUnsignedInteger(cfg,"SInfo","srat"    ,si->srat);
		CfgIO_WriteUnsignedInteger(cfg,"SInfo","channels",si->channels);
		/** @TODO: save loop info */

		CfgIO_WriteConfig(cfg,fn);
		CfgIO_RemConfig(cfg);
	}
	else ERR("Can't create SInfo config");
}

void RemoveSInfo(SInfo *si) {
	char fn[FILENAME_MAX];

	strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeSInfoName(si));
	remove(fn);
}

void RenameSInfo(SInfo *src,SInfo *dst) {
	char fn1[FILENAME_MAX],fn2[FILENAME_MAX];

	strmfp(fn1,GetRunTime(sfxprefs_vmem)->swap_path,MakeSInfoName(src));
	strmfp(fn2,GetRunTime(sfxprefs_vmem)->swap_path,MakeSInfoName(dst));
	rename(fn1,fn2);
}


//-- helper

UBYTE CopyDataSub(void *src,void *dst,UWORD mode,ULONG len) {
	UBYTE okay=TRUE;

	INTRO;
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(src,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(dst,__FUNC__,"2.par",4));
	ASSERT(len>0);
	
	MSG4("  mode 0x%04lx  src 0x%08lx  dst 0x%08lx  len %5ld",mode,(ULONG)src,(ULONG)dst,len);
	switch(mode) {
		case CPD_NIL2MEM:
			memset((WORD *)dst,0,Samples2Bytes(len));
			break;
		case CPD_NIL2DRV:
			memset(((SmpAccess *)dst)->buffer1,0,((SmpAccess *)src)->blen);		// clear block first
			for(((SmpAccess *)dst)->seg=0;((SmpAccess *)dst)->seg<((SmpAccess *)dst)->seganz;((SmpAccess *)dst)->seg++) {
				if(!(((SmpAccess *)dst)->safunc((SmpAccess *)dst,((SmpAccess *)dst)->blen))) {
					ERR2("    error on write : %ld/%ld",((SmpAccess *)dst)->seg,((SmpAccess *)dst)->seganz);
					okay=FALSE;break;
				}
			}
			break;
		case CPD_MEM2MEM:
			memcpy((WORD *)dst,(WORD *)src,Samples2Bytes(len));
			break;
		case CPD_MEM2DRV:
			if(write(((SmpAccess *)dst)->fh,(WORD *)src,Samples2Bytes(len))==-1) okay=FALSE;
			break;
		case CPD_DRV2MEM:
			if(read (((SmpAccess *)src)->fh,(WORD *)dst,Samples2Bytes(len))==-1) okay=FALSE;
			break;
		case CPD_DRV2DRV:
			// @todo: it sometimes try to read once too often -> 5=IOERR
			for(((SmpAccess *)src)->seg=0,((SmpAccess *)dst)->seg=0;((SmpAccess *)src)->seg<(((SmpAccess *)src)->seganz-1);((SmpAccess *)src)->seg++,((SmpAccess *)dst)->seg++) {
				//MSG3("    read : %ld/%ld : filepos=%ld",((SmpAccess *)src)->seg,((SmpAccess *)src)->seganz,tell(((SmpAccess *)src)->fh));
				if(!(((SmpAccess *)src)->safunc((SmpAccess *)src,((SmpAccess *)src)->blen))) {
					ERR4("    error on read : %ld/%ld : errno=%ld : filepos=%ld",((SmpAccess *)src)->seg,((SmpAccess *)src)->seganz,((SmpAccess *)src)->last_error_no,tell(((SmpAccess *)src)->fh));
					okay=FALSE;break;
				}
				memcpy(((SmpAccess *)dst)->buffer1,((SmpAccess *)src)->buffer1,((SmpAccess *)src)->blen);
				if(!(((SmpAccess *)dst)->safunc((SmpAccess *)dst,((SmpAccess *)dst)->blen))) {
					ERR3("    error on write : %ld/%ld : %ld",((SmpAccess *)dst)->seg,((SmpAccess *)dst)->seganz,((SmpAccess *)src)->last_error_no);
					okay=FALSE;break;
				}
			}
			// read last block (read will return errno=5)
			(((SmpAccess *)src)->safunc((SmpAccess *)src,((SmpAccess *)src)->blen));
			memcpy(((SmpAccess *)dst)->buffer1,((SmpAccess *)src)->buffer1,((SmpAccess *)src)->blen);
			if(!(((SmpAccess *)dst)->safunc((SmpAccess *)dst,((SmpAccess *)dst)->blen))) {
				ERR3("    error on write : %ld/%ld : %ld",((SmpAccess *)dst)->seg,((SmpAccess *)dst)->seganz,((SmpAccess *)src)->last_error_no);
				okay=FALSE;break;
			}
			break;
		UNEXPECTED_DEFAULT
	}
	OUTRO_BOOL(okay);
	return(okay);
}

STRPTR MakeVMemName(SInfo *si,UBYTE channel) {
	static char name[12];

	switch(si->channels) {
		case CH_MONO:	sprintf(name,"%08lX.0",si);break;
		case CH_STEREO:	sprintf(name,"%08lX.%1d",si,channel&1);break;
		case CH_QUADRO:	sprintf(name,"%08lX.%1d",si,channel&3);break;
		UNEXPECTED_DEFAULT
	}
	return(name);
}

STRPTR MakeSInfoName(SInfo *si) {
	static char name[14];

	sprintf(name,"%08lX.cfg",si);
	return(name);
}


#define FILEIO_DELAY 10

void SafeRemove(STRPTR fn) {
	if(remove(fn)) {
		MSG4("error removing file \"%s\" : %ld,%ld %s\n",fn,_OSERR,errno,strerror(errno));
		Delay(FILEIO_DELAY);
		if(remove(fn)) {
			ERR4("error removing file \"%s\" : %ld,%ld %s\n",fn,_OSERR,errno,strerror(errno));
		}
	}
}

void SafeRename(STRPTR sfn,STRPTR dfn) {
	if(rename(sfn,dfn)) {
		MSG5("error rename file \"%s\"->\"%s\" : %ld ,%ld %s",sfn,dfn,_OSERR,errno,strerror(errno));
		Delay(FILEIO_DELAY);
		if(rename(sfn,dfn)) {
			ERR5("error rename file \"%s\"->\"%s\" : %ld ,%ld %s",sfn,dfn,_OSERR,errno,strerror(errno));
		}
	}
}

void SafeClose(int fh) {
	//while(close(fh)==-1) Delay(10);

	if(close(fh)) {
		MSG4("error closing filehandle %d : %ld ,%ld %s",fh,_OSERR,errno,strerror(errno));
		Delay(FILEIO_DELAY);
		if(close(fh)) {
			MSG4("error closing filehandle %d : %ld ,%ld %s",fh,_OSERR,errno,strerror(errno));
		}
	}
}

//-- IO functions

UBYTE ASM SA_MemRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length)) {
#ifdef DEBUG
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);
#endif
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	// length=0 is allowed for last read
	//ASSERT(length>0);

	if(sa->seek>-1) { sa->buffer2=&sa->buffer4[sa->seek];sa->seek=-1; }
	sa->buffer1=sa->buffer2;
	sa->buffer2=&sa->buffer2[length];								// advance pointer

	ASSERTQ(IS_VALID_POINTER_N0(sa->buffer1,__FUNC__,"1.res",4));
	//-- this can be invalid (last read)
	//ASSERTQ(IS_VALID_POINTER_N0(sa->buffer2,__FUNC__,"2.res",4));
	return(TRUE);
}

UBYTE ASM SA_MemWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length)) {
#ifdef DEBUG
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);
#endif
	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));
	ASSERT(length>0);

	sa->buffer1=&sa->buffer1[length];								// advance pointer

	ASSERTQ(IS_VALID_POINTER_N0(sa->buffer1,__FUNC__,"1.res",4));
	return(TRUE);
}

//-- DEBUG
//#include <proto/casasupport.h>
//extern struct CSBase *CSBase;
//-- DEBUG

UBYTE ASM SA_StreamRead(REG(a0,SmpAccess *sa),REG(d0,ULONG length)) {
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));

	sa->state=STATE_NOTIFY;
	// wait for input data ready
	//MSG("StreamRead : wait");
	//kprintf("StreamRead : wait\n");
	__Wait(1L<<sa->worksig);
	//Delay(1);
	// send acknowledge
	//MSG("StreamRead : signal");
	//kprintf("StreamRead : signal\n");
	__Signal(sa->maintask,1L<<sa->mainsig);
	/** TODO: check without the Delay() <------------------------------ !!! */
	Delay(1);
	return(TRUE);
}

UBYTE ASM SA_StreamWrite(REG(a0,SmpAccess *sa),REG(d0,ULONG length)) {
	__builtin_putreg(REG_A4,sa->a4);
	__builtin_putreg(REG_A6,sa->a6);

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(sa,__FUNC__,"1.par",4));

	while(sa->state!=STATE_NOTIFY) Delay(1);
	// notify of output data ready
	//MSG("StreamWrite : signal");
	//kprintf("StreamWrite : signal\n");
	__Signal(sa->maintask,1L<<sa->mainsig);
	//Delay(1);
	// wait for acknowledge
	//MSG("StreamWrite : wait");
	//kprintf("StreamWrite : wait\n");
	__Wait(1L<<sa->worksig);
	sa->state=STATE_READY;
	return(TRUE);
}

//-- eof ----------------------------------------------------------------------
