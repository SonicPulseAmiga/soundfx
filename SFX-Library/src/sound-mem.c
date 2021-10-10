/****h* sfxsupport.library/sound-mem.c [4.2] *
*
*  NAME
*    sound-mem.c
*  COPYRIGHT
*    $VER: sound-mem.c 4.27.07.08.02 by Stefan Kost 1998-2002
*  FUNCTION
*    sound memory allocation - definitions
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    19.Jul.1998
*  MODIFICATION HISTORY
*    07.Aug.2002    V 4.2   most recent version
*    11.Jan.2001    V 4.1   most recent version
*    16.Jul.2000    V 1.3   most recent version
*    27.Apr.1999    V 1.2   most recent version
*    19.Jul.1998    V 1.0   initial version
*  NOTES
*
*******
*/

#define SOUND_MEM_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- Sound Memory Allocation

UBYTE       ASM LIB_EXPORT(AllocSampleMem(REG(a0,SInfo *si),REG(d0,ULONG slen),REG(d1,UBYTE chan)));
void        ASM LIB_EXPORT(FreeSampleMem(REG(a0,SInfo *si)));

//-- helper

void		SInfo_SetVBuf(SInfo *si,UBYTE chan);

//-- definitions --------------------------------------------------------------

//-- Sound Memory Allocation

UBYTE SAVEDS ASM LIB_EXPORT(AllocSampleMem(REG(a0,SInfo *si),REG(d0,ULONG slen),REG(d1,UBYTE chan))) {
	register ULONG i;
	register UBYTE l;
	UBYTE ret=TRUE;
	UWORD null=0;
	char fn[FILENAME_MAX],tfn[FILENAME_MAX];
	FILE *swf,*tswf;
	ULONG new_vlen=(((slen>>14)+1)<<14),old_vlen;      // end is expanded to 8192 Samples
	SAMPLE *newmem;

	INTRO;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));
	ASSERT(slen>0);
	ASSERT(chan>0);
	if(!ASSERT_OKAY) return(FALSE);

	MSG1("si->id=%ld",si->id);
	MSG2("slen=%ld -> %ld",slen,new_vlen);
	MSG1("chan=%ld",chan);

	switch(si->storage) {
		case VM_NODATA:                     // neues Sample
			MSG("new sample");
			if(GetRunTime(sfxprefs_sbuf)->storage==VM_MEMORY) {
				for(l=0;l<chan;l++) {           // try to allocate Memory
					ASSERT(si->memory[l]==NULL);
					if(!(si->memory[l]=(WORD *)AllocVec(Samples2Bytes(new_vlen),MEMF_ANY|MEMF_CLEAR))) { ret=FALSE;break; }
					MSG2("memory[%1d]=0x%08lx",l,si->memory[l]);
				}
			}
			else ret=FALSE;
			if(ret) {
				MSG("  -> succes");
				si->memlen=new_vlen;
				si->slen=slen;si->storage=VM_MEMORY;
			}
			else {
				MSG("  -> fail or vmem requested");
				for(l=0;l<chan;l++) {
					MSG2("freeing memory[%1d]=0x%08lx",l,si->memory[l]);
					if(si->memory[l]) { FreeVec(si->memory[l]);si->memory[l]=NULL; }
				}
				if(GetRunTime(sfxprefs_vmem)->enabled) {
					ret=TRUE;si->slen=slen;si->storage=VM_DRIVE;
					MSG("  -> using vmem");
				}
			}
			break;
		case VM_MEMORY:             		// Samplespeicher soll verlängert/verkürzt werden
			MSG("change length of memory sample");
			old_vlen=(((si->slen>>14)+1)<<14);
			for(l=0;l<chan;l++) {
				if((newmem=(SAMPLE *)LIB_INTCALL(ReAllocVec(si->memory[l],Samples2Bytes(old_vlen),Samples2Bytes(new_vlen),MEMF_ANY|MEMF_CLEAR)))) {
					si->memory[l]=newmem;
				}
				else {
					ret=FALSE;break;
				}
			}
			if(!ret) {
				if(GetRunTime(sfxprefs_vmem)->enabled) {
					ret=TRUE;
					LIB_INTCALL(ConvMem2Drv(si));     // jetzt noch Länge ändern
					if(slen>si->slen) {
						for(l=0;l<chan;l++) {       // es wird länger
							strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
							if((swf=fopen(fn,"ab"))) {
								if(!(fwrite(&null,2,(slen-si->slen),swf))) { ret=FALSE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
								fclose(swf);
							}
							else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
						}
					}
					else if(slen<si->slen) {
						for(l=0;l<chan;l++) {       // es wird kürzer
							strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
							strmfp(tfn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,CH_MAX+l));
							rename(fn,tfn);     // altes file -> temp file
							if((swf=fopen(fn,"wb"))) {
								if((tswf=fopen(tfn,"rb"))) {
									/** @todo this is extremly inefficient ! */
									for(i=0;i<slen;i++) {
										if(fread(&null,2,1,tswf)) if(!(fwrite(&null,2,1,swf))) { ret=FALSE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
									}
									fclose(tswf);
								}
								else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(tfn,errOpenFile,errno,__FILE__,__LINE__)); }
								fclose(swf);
							}
							else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
						}
					}
					if(!ret) {
						for(l=0;l<chan;l++) {
							strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
							SafeRemove(fn);
						}
						si->storage=VM_NODATA;
					}
					else { si->slen=slen;si->storage=VM_DRIVE; }
				}
			}
			else {
				si->memlen=new_vlen;
				si->slen=slen;si->storage=VM_MEMORY;
			}
			break;
		case VM_DRIVE:              // Samplespeicher soll verlängert werden
			MSG("change length of drive sample");
			if(slen>si->slen) {
				for(l=0;l<chan;l++) {       // es wird länger
					strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
					if((swf=fopen(fn,"ab"))) {
						if(!(fwrite(&null,2,(slen-si->slen),swf))) { ret=FALSE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
						fclose(swf);
					}
					else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
				}
			}
			if(slen<si->slen) {
				for(l=0;l<chan;l++) {       // es wird kürzer
					strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
					strmfp(tfn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,CH_MAX+l));
					rename(fn,tfn);     // altes file -> tmp file
					if((swf=fopen(fn,"wb"))) {
						if((tswf=fopen(tfn,"rb"))) {
							/** @todo this is extremly inefficient ! */
							for(i=0;i<slen;i++) {
								if(fread(&null,2,1,tswf)) if(!(fwrite(&null,2,1,swf))) { ret=FALSE;l=CH_MAX;LIB_INTCALL(Message(errWriteData,NULL,__FILE__,__LINE__)); }
							}
							fclose(tswf);
						}
						else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(tfn,errOpenFile,errno,__FILE__,__LINE__)); }
						fclose(swf);
					}
					else { ret=FALSE;l=CH_MAX;LIB_INTCALL(StdCError(fn,errOpenFile,errno,__FILE__,__LINE__)); }
				}
			}
			if(!ret) {
				for(l=0;l<chan;l++) {
					strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
					SafeRemove(fn);
				}
				si->storage=VM_NODATA;
			}
			else { si->slen=slen;si->storage=VM_DRIVE; }
			break;
		case VM_STREAM:
			break;
		UNEXPECTED_DEFAULT
	}
	//-- let buffers point to memory
	si->buf[0]=si->memory[0];
	si->buf[1]=si->memory[1];
	si->buf[2]=si->memory[2];
	si->buf[3]=si->memory[3];
	//-- do channel mapping
	SInfo_SetVBuf(si,chan);

	MSG3("memory                  buf        vbuf       slen=%ld chan=%1d storage=%1d",si->slen,si->channels,si->storage);
	MSG4("0x%08lx...0x%08lx 0x%08lx 0x%08lx",si->memory[0],(si->memory[0]?(&si->memory[0][new_vlen]):0),si->buf[0],si->vbuf[0]);
	MSG4("0x%08lx...0x%08lx 0x%08lx 0x%08lx",si->memory[1],(si->memory[1]?(&si->memory[1][new_vlen]):0),si->buf[1],si->vbuf[1]);
	MSG4("0x%08lx...0x%08lx 0x%08lx 0x%08lx",si->memory[2],(si->memory[2]?(&si->memory[2][new_vlen]):0),si->buf[2],si->vbuf[2]);
	MSG4("0x%08lx...0x%08lx 0x%08lx 0x%08lx",si->memory[3],(si->memory[3]?(&si->memory[3][new_vlen]):0),si->buf[3],si->vbuf[3]);

	MSG1("ret=%1d",ret);
	OUTRO;
	return(ret);
}

void SAVEDS ASM LIB_EXPORT(FreeSampleMem(REG(a0,SInfo *si))) {
	register UBYTE l;
	char fn[FILENAME_MAX];

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(si,__FUNC__,"1.par",4));

	if(si->slen>0) {
		switch(si->storage) {
			case VM_NODATA:
				break;
			case VM_MEMORY:                             // free memory
				if(si->memory[0]) { FreeVec(si->memory[0]);si->memory[0]=NULL; }
				if(si->memory[1]) { FreeVec(si->memory[1]);si->memory[1]=NULL; }
				if(si->memory[2]) { FreeVec(si->memory[2]);si->memory[2]=NULL; }
				if(si->memory[3]) { FreeVec(si->memory[3]);si->memory[3]=NULL; }
				if(!si->memory[0] && si->memory[1] && si->memory[2] && si->memory[3]) si->memlen=0;
				break;
			case VM_DRIVE:
				for(l=0;l<(si->channels+1);l++) {       // erase all files
					strmfp(fn,GetRunTime(sfxprefs_vmem)->swap_path,MakeVMemName(si,l));
					SafeRemove(fn);
				}
				break;
			case VM_STREAM:
				break;
			UNEXPECTED_DEFAULT
		}
		si->buf[0]=si->buf[1]=si->buf[2]=si->buf[3]=si->vbuf[0]=si->vbuf[1]=si->vbuf[2]=si->vbuf[3]=NULL;
		si->storage=VM_NODATA;
	}
	else {
		ERR("Called FreeSampleMem for slen==0");
	}
	//MSG("Samplememory freed");
}

//-- helper

void SInfo_SetVBuf(SInfo *si,UBYTE chan) {
	switch(chan-1) {
		case CH_MONO:
			si->vbuf[0]=si->vbuf[1]=si->vbuf[2]=si->vbuf[3]=si->buf[0];
			break;
		case CH_STEREO:
			si->vbuf[0]=si->vbuf[2]=si->buf[0];
			si->vbuf[1]=si->vbuf[3]=si->buf[1];
			break;
		case CH_QUADRO:
			si->vbuf[0]=si->buf[0];
			si->vbuf[1]=si->buf[1];
			si->vbuf[2]=si->buf[2];
			si->vbuf[3]=si->buf[3];
			break;
		UNEXPECTED_DEFAULT
	}
}

//-- eof ----------------------------------------------------------------------
