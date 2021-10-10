/****h* SDS-File/SDS-File_L.c [4.2] *
*
*  NAME
*    SDS-File_L.c
*  COPYRIGHT
*    $VER: SDS-File_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    SDS-File loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    29.Jun.1999	V 1.2	most recent version
*    22.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*  IDEAS
*    support loading separate channels to separate files (a la Studio16)
*******
*/

/****u* _loaders/SDS-File_L *
*  Function.eng
*    Loads Sample Dump Standard files.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>no (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    This format allows you to exchange samples with you sampler (profi-sampler,
*    not those parallel port ones). Additionally you need a SysEx dumper. Send
*    the sample from the sampler via MIDI/SCSI and save the received data to a
*    file (prefered ending .SDS). These files can be loaded into __SFX then.
*  Funktion.ger
*    Läd Sample Dump Standard Dateien.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>nein (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    Dieses Format ermöglicht es Ihnen Samples mit Ihrem Sampler (Profisampler,
*    keine Parallelportsampler) auszutauschen. Dazu benötigen sie weiterhin einen
*    SysEx Dumper. Senden Sie das Sample vom Sampler aus per MIDI/SCSI und speichern
*    Sie die empfangene SysEx Datei ab (bevorzugte Endung .SDS). Diese kann nun
*    von __SFX geladen werden.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SDS_File_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>
#include <libraries/asyncio.h>
#include <proto/asyncio.h>

//-- Local

#include "project.h"
#include "SDS-File.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	UBYTE dummy;
};

//-- Library-Basisaddrs.

struct Library				*AsyncIOBase=NULL;

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(AsyncIOBase	=OpenLibrary("asyncio.library",39L)))							{ ADosError("asynchio.library >=V39.0",errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; }
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(AsyncIOBase)		CloseLibrary(AsyncIOBase);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

SFXMod_DoClientDataInit
SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	return(TRUE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	struct SampleHeader shdr;
	struct SampleBlock sblk;
	register UBYTE i;
	FILE *hdrfile;
	struct AsyncFile *infile;
	ULONG rate,slen,loops,loope;
	ULONG blen=120,curlen;
	SInfo *si=pdata->dst[0];

	if(!(hdrfile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	//-- sizeof returns 22, but structure is 21 bytes only
	if(!fread(&shdr,21,1,hdrfile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(hdrfile);return(FALSE); }
	if(shdr.sox!=SYSEX_START || shdr.eox!=SYSEX_END || shdr.type!=SDS_HEADER) { sprintf(errmsg,GetString(errWrongFiletype),"SDS");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(hdrfile);return(FALSE); }
	fclose(hdrfile);

	rate=(shdr.rate2  <<14)|(shdr.rate1  <<7)|(shdr.rate0  );
	slen=(shdr.length2<<14)|(shdr.length1<<7)|(shdr.length0);
	loops=(shdr.loopstart2<<14)|(shdr.loopstart1<<7)|(shdr.loopstart0);
	loope=(shdr.loopend2  <<14)|(shdr.loopend1  <<7)|(shdr.loopend0  );

	// DEBUG
	MSG1("SDS-File : <%s>",pdata->fn);
	MSG1("\tchannelnr : %3d",shdr.channelnr);		// samples >mono seem to dump several headers+data
	//MSG1("\tsamplenr  : %3d",shdr.samplenr);		// is number in sampler memory
	MSG1("\tbits      : %3d",shdr.bits);
	MSG1("\trate      : %5d",rate);
	MSG1("\tslen      : %5d",slen);
	MSG1("\tloopstart : %5d",loops);
	MSG1("\tloopend   : %5d",loope);
	MSG1("\tloop      : %5d",shdr.loopmode);
	// DEBUG
	if(rate) si->srat=(ULONG)(1000000000.0/(double)rate);
	else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
	si->slen=slen;

	switch(shdr.loopmode) {
		case SDS_LOOP_FORWARD:
			si->loop=SFXLOOP_FORWARD;
			si->loops=loops;
			si->loopl=loope-loops;
			break;
	}
	     if(shdr.bits==8) blen=120;
	else if(shdr.bits<17) blen= 80;
	else if(shdr.bits<25) blen= 60;
	else if(shdr.bits<29) blen= 40;

	MSG1("using blen = %d",blen);

	if(!(AllocSampleMem(si,slen,1))) { si->slen=0;Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }

	if((infile=OpenAsync(pdata->fn,MODE_READ,4064))) {
		SmpAccess * const dstsa=SAInit(si,0,0,slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,blen);	// sequentiell Blöcke konstanter Größe schreiben
		ProWinTitle title;
		
		SeekAsync(infile,21,MODE_START);
		sprintf(title,GetString(msgProgressLoad),1,1);
		if(dstsa) {
			SAFuncPtr const SAWrite=dstsa->safunc;
			PWData * const pwd=NewProWin();
			if(pwd) {
				register SAMPLE *dbuf;
				UBYTE okay=TRUE;
				StartProWin(pwd,&curlen,title,slen);		// sets curlen=0;
				if(shdr.bits<15) {
					MSG("\tread with two 7-bit bytes");
					for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && okay && (ReadAsync(infile,&sblk,127)!=0) && (curlen<slen));dstsa->seg++) {
						dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
						if(sblk.sox!=SYSEX_START || sblk.eox!=SYSEX_END || sblk.type!=SDS_BLOCK) { sprintf(errmsg,GetString(errWrongFiletype),"SDS");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);okay=FALSE; }
						else {
							for(i=0;((i<120) && (curlen<slen));i+=2,curlen++) {
								*(dbuf++)=((WORD)(((sblk.data[i+1])|(sblk.data[i]<<7))>>(14-shdr.bits)))+SMP_MAXN;
							}
							SAWrite(dstsa,dstsa->blen);							// akt Block schreiben
						}
					}
				}
				else if(shdr.bits<22) {
					MSG("\tread with three 7-bit bytes");
					for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && okay && (ReadAsync(infile,&sblk,127)!=0) && (curlen<slen));dstsa->seg++) {
						dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
						if(sblk.sox!=SYSEX_START || sblk.eox!=SYSEX_END || sblk.type!=SDS_BLOCK) { sprintf(errmsg,GetString(errWrongFiletype),"SDS");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);okay=FALSE; }
						else {
							for(i=0;((i<120) && (curlen<slen));i+=3,curlen++) {
								*(dbuf++)=((WORD)(((sblk.data[i+2])|(sblk.data[i+1]<<7)|(sblk.data[i]<<14))>>(21-shdr.bits)))+SMP_MAXN;
							}
							SAWrite(dstsa,dstsa->blen);							// akt Block schreiben
						}
					}
				}
				else if(shdr.bits<29) {
					MSG("\tread with four 7-bit bytes");
					for(dstsa->seg=0;((dstsa->seg<dstsa->seganz) && okay && (ReadAsync(infile,&sblk,127)!=0) && (curlen<slen));dstsa->seg++) {
						dbuf=(SAMPLE *)dstsa->buffer1;						// Pointer auf dst-Block
						if(sblk.sox!=SYSEX_START || sblk.eox!=SYSEX_END || sblk.type!=SDS_BLOCK) { sprintf(errmsg,GetString(errWrongFiletype),"SDS");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);okay=FALSE; }
						else {
							for(i=0;((i<120) && (curlen<slen));i+=4,curlen++) {
								*(dbuf++)=((WORD)(((sblk.data[i+3])|(sblk.data[i+2]<<7)|(sblk.data[i+1]<<14)|(sblk.data[i]<<21))>>(28-shdr.bits)))+SMP_MAXN;
							}
							SAWrite(dstsa,dstsa->blen);							// akt Block schreiben
						}
					}
				}
				DelProWin(pwd);
			}
			SADone(dstsa,si);					// Samplezugiffshandles freigeben
		}
		CloseAsync(infile);
	}
	else { ADosError(pdata->fn,errOpenFile,IoErr(),__FILE__,__LINE__);return(FALSE); }
	si->zoomxl=si->slen;

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM16);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
}

void SaveSettings(struct Instance *instance,char *fname) {
}

void SetDefaultSettings(struct Instance *instance) {
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
