/****h* SDS-File/SDS-File_S.c [4.3] *
*
*  NAME
*    SDS-File_S.c
*  COPYRIGHT
*    $VER: SDS-File_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SDS-File saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    03.Jun.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    21.Feb.1999	V 1.2	most recent version
*    22.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*  IDEAS
*    support saving separate channels to separate files (a la Studio16)
*******
*/

/****u* _savers/SDS-File_S *
*  Function.eng
*    Saves Sample Dump Standard files.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>no (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    This format allows you to exchange samples with you sampler (profi-sampler,
*    not those parallel port ones). Additionally you need a SysEx dumper. Save
*    the sample to a file and send this to the sampler via MIDI/SCSI.
*  Funktion.ger
*    Speichert Sample Dump Standard Dateien.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>nein (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
*    </table>
*    Dieses Format ermöglicht es Ihnen Samples mit Ihrem Sampler (Profisampler,
*    keine Parallelportsampler) auszutauschen. Dazu benötigen sie weiterhin einen
*    SysEx Dumper. Speichern sie die Datei und senden sie diese an den Sampler
*    per MIDI/SCSI.
*  Parameter.eng
*    Channel
*      Midi channel over which the sample should be transfered.
*    Sample
*      Sample bank number into which the sample should store the data.
*  Parameter.ger
*    Channel
*      Midikanalnummer auf der das Sample gesendet werden soll (0-16).
*    Sample
*      Speicherplatznummer auf der der Sampler die Daten ablegen soll.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SDS_File_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>
#include <libraries/asyncio.h>
#include <proto/asyncio.h>

//-- Local

#include "project.h"
#include "SDS-File.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_CHNR		(GADID_BASE+1)
#define GADIX_CHNR		(GADIX_BASE+1)
#define GADID_SMPNR		(GADID_CHNR+1)
#define GADIX_SMPNR		(GADIX_CHNR+1)

#define GADID_SRC       (GADID_SMPNR+1)
#define GADIX_SRC       (GADIX_SMPNR+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source          src;       	/* Quelle */
	UBYTE			channelnr;	/* send it to which midi-channel */
	UWORD			samplenr;	/* store it to which sample-number */

	PrefsData moddesc[2];
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
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(cfg)             CfgIO_RemConfig(cfg);
	if(AsyncIOBase)		CloseLibrary(AsyncIOBase);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void __saveds __asm *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {
	struct Instance *instance;

	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ModDescInit(0,PD_SOURCE      ,"S1",&(instance->src));
		SFXMod_ModDescInit(1,PD_END         ,0l  ,0l);	
		SFXMod_ClientDataInitCommon;
	}
	return((void *)instance);
}

void __saveds __asm LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) {
	if(instance) {
		SFXMod_ClientDataDoneCommon;
		FreeVec((void *)instance);
	}
}

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables
	SFXMod_Interface_CfgSelVariables
	SFXMod_Interface_NotifyVariables

	UWORD ys1=2;
	UWORD ys2=ys1+1+SRCSEL_HEIGHT;
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge      =xs1+RunTime->scx[(308>>1)-110];
		ng.ng_TopEdge       =ys3+RunTime->scy[2];
		ng.ng_Width         =RunTime->scx[100];
		ng.ng_Height        =RunTime->scy[11];
		ng.ng_GadgetText    =GetString(btnSave);
		ng.ng_TextAttr      =RunTime->ScrFontB;
		ng.ng_GadgetID      =GADID_GO;
		ng.ng_Flags         =PLACETEXT_IN;
		ng.ng_VisualInfo    =RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		ng.ng_LeftEdge      =xs1+RunTime->scx[(308>>1)+10];
		ng.ng_GadgetText    =GetString(menPrefsUse);
		ng.ng_TextAttr      =RunTime->ScrFont;
		ng.ng_GadgetID      =GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		g=AddSource(&instance->src,modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[57];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[75];
		ng.ng_GadgetText	="Channel";
		ng.ng_GadgetID		=GADID_CHNR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_Number,instance->channelnr,GTIN_MaxChars,2,TAG_DONE);
		modGadgets[GADIX_CHNR]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[205];
		ng.ng_Width			=RunTime->scx[95];
		ng.ng_GadgetText	="Sample";
		ng.ng_GadgetID		=GADID_SMPNR;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_Number,instance->samplenr,GTIN_MaxChars,5,TAG_DONE);
		modGadgets[GADIX_SMPNR]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous" ,xs1,ys2,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(1));               /* Miscellaneous */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);                  /* Go */

			SetSource   (&instance->src,modWin,modGadgets,GADIX_SRC);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=1;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=0;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					}
					SetSource   (&instance->src,modWin,modGadgets,GADIX_SRC);
				}
				if(sigout&(1L<<modWin->UserPort->mp_SigBit)) {
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						SFXMod_Interface_AnalyseIMsg
						switch(iclass) {
							case IDCMP_GADGETUP:
								gnr=(ULONG)(((struct Gadget *)iadr)->GadgetID);
								if(!HandleSource(&instance->src,modWin,modGadgets,GADID_SRC,GADIX_SRC,gnr,icode)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_CHNR:
											instance->channelnr=atoi(((struct StringInfo *)(modGadgets[GADIX_CHNR]->SpecialInfo))->Buffer);
											break;
										case GADID_SMPNR:
											instance->samplenr=atoi(((struct StringInfo *)(modGadgets[GADIX_SMPNR]->SpecialInfo))->Buffer);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB: ActivateGadget(modGadgets[GADIX_CHNR],modWin,0l);break;
								}
								break;
							SFXMod_Interface_Events
						}
					}
				}
			}
			SFXMod_Interface_WinDone
			SFXMod_Interface_FReqDone
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	SFXMod_Interface_NotifyDone
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	struct SampleHeader shdr;
	struct SampleBlock sblk;
	register UBYTE j;
	FILE *hdrfile;
	struct AsyncFile *outfile;
	ULONG loope,rate,curlen;
	UBYTE chk;
	WORD sval;
	SaveData sdata;
	SInfo *si;
	UBYTE ret=1;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:             // wir wurden e.g. per ARexx gestartet
			UnlockBuffer(instance->src.srcbuf);
			ret&=((instance->src.srcbuf=LockBuffer(pdata->src[0]))!=0);
			if(!ret) return(ret);
			instance->src.rmode=RNGMD_ALL;
			break;
	}
	si=instance->src.srcbuf;

	SetRngs(si,&sdata.savestart,&sdata.savelength,instance->src.rmode);
	strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
	sdata.chmask=CH_ALL;

	//-- !!! LOCALIZE !!!
	if(si->channels>CH_MONO) { Message(errWriteData,NULL,__FILE__,__LINE__);return(FALSE); }
	//-- !!! LOCALIZE !!!

	if(!(hdrfile=fopen(pdata->fn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	rate=(ULONG)(1.0/((double)si->srat/1000000000));

	shdr.sox=sblk.sox=SYSEX_START;
	shdr.channelnr=sblk.channelnr=instance->channelnr;
	shdr.type=SDS_HEADER;
	shdr.samplenr0=instance->samplenr&0x7F;shdr.samplenr1=(instance->samplenr>>7)&0x7F;
	shdr.bits=16;
	shdr.rate0  =    rate&0x7F;shdr.rate1  =(    rate>>7)&0x7F;shdr.rate2  =(    rate>>14)&0x7F;
	shdr.length0=si->slen&0x7F;shdr.length1=(si->slen>>7)&0x7F;shdr.length2=(si->slen>>14)&0x7F;
	if(instance->src.rmode==RNGMD_ALL) {
		switch(si->loop) {
			case SFXLOOP_NONE:
				shdr.loopmode=SDS_LOOP_NONE;
				shdr.loopstart0=shdr.loopstart1=shdr.loopstart2=0;
				shdr.loopend0=sdata.savelength&0x7F;shdr.loopend1=(sdata.savelength>>7)&0x7F;shdr.loopend2=(sdata.savelength>>14)&0x7F;
				break;
			case SFXLOOP_FORWARD:
				loope=si->loops+si->loopl;
				shdr.loopmode=SDS_LOOP_FORWARD;
				shdr.loopstart0=si->loops&0x7F;shdr.loopstart1=(si->loops>>7)&0x7F;shdr.loopstart2=(si->loops>>14)&0x7F;
				shdr.loopend0=      loope&0x7F;shdr.loopend1=  (    loope>>7)&0x7F;shdr.loopend2  =(    loope>>14)&0x7F;
				break;
		}
	}
	else {
		shdr.loopmode=SDS_LOOP_NONE;
		shdr.loopstart0=shdr.loopstart1=shdr.loopstart2=0;
		shdr.loopend0=sdata.savelength&0x7F;shdr.loopend1=(sdata.savelength>>7)&0x7F;shdr.loopend2=(sdata.savelength>>14)&0x7F;
	}
	shdr.eox=sblk.eox=SYSEX_END;		
	//-- sizeof returns 22, but structure is 21 bytes only
	if(!fwrite(&shdr,21,1,hdrfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(hdrfile);return(FALSE); }
	fclose(hdrfile);

	MSG2("sizeof(shdr)=%ld , sizeof(struct SampleHeader)=%ld",sizeof(shdr),sizeof(struct SampleHeader));

	if((outfile=OpenAsync(pdata->fn,MODE_APPEND,4064)))	{
		SmpAccess * const srcsa=SAInit(si,0,sdata.savestart,sdata.savelength,VM_MODE_READ|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,Samples2Bytes(40));	// sequentiell Blöcke konstanter Größe lesen
		ProWinTitle title;
		sprintf(title,GetString(msgProgressSave),1,1);
		if(srcsa) {
			SAFuncPtr const SARead=srcsa->safunc;
			PWData * const pwd=NewProWin();
			if(pwd) {
				register SAMPLE *sbuf;
				sblk.type=SDS_BLOCK;
				StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;
				MSG("before loop ...");
				for(srcsa->seg=0;srcsa->seg<srcsa->seganz;srcsa->seg++) {
					sblk.blockct=srcsa->seg;
					chk=2^srcsa->seg;			// XOR of data after SYSEX_START till chk
					sbuf=(SAMPLE *)srcsa->buffer1;SARead(srcsa,srcsa->blen);						// Pointer auf dst-Block
					for(j=0;j<(3*Bytes2Samples(srcsa->blen));j+=3) {
						if(curlen<si->slen) { sval=*(sbuf++)+SMP_MAXN;curlen++; }
						else sval=SMP_MAXN;
						sblk.data[j+2]=(UBYTE)(sval&0x3)<<5;
						sblk.data[j+1]=(UBYTE)((UWORD)(sval>>2)&0x7F);
						sblk.data[j]=(UBYTE)((UWORD)(sval>>9)&0x7F);		
						chk^=(sblk.data[j]^sblk.data[j+1]^sblk.data[j+2]);
					}
					sblk.chksum=chk;
					WriteAsync(outfile,&sblk,127);
				}
				DelProWin(pwd);
			}
			SADone(srcsa,si);					// Samplezugiffshandles freigeben
		}
		CloseAsync(outfile);
	}
	else { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	//-- set file comment
	{
		UBYTE ch=si->channels;
		si->channels=CH_MONO;
		SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM16);
		si->channels=ch;
	}
	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"ChannelNr"))	{ instance->channelnr=atoi(RexxPar3);pf=1; }
		if(!pf && !stricmp(RexxPar2,"SampleNr"))	{ instance->samplenr=atoi(RexxPar3);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"ChannelNr"))	{ sprintf(ret,"%d",instance->channelnr);pf=1; }
		if(!pf && !stricmp(RexxPar2,"SampleNr"))	{ sprintf(ret,"%d",instance->samplenr);/*pf=1;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetSaverPreset")) {
		if(!stricmp(RexxPar2,"default"))  { RecallDefaultSettings(instance); }
		else { sprintf(fn,PROGDIR"_savers/"PRJ_NAME"/%s.cfg",RexxPar2);LoadSettings(instance,fn); }
		SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
}

/*-- local Stuff --------------------------------------------------------------------------------*/

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	LoadModSettings(cfg,instance->moddesc);
	instance->channelnr	=CfgIO_ReadUnsignedInteger(cfg,"General"	,"ChannelNr",	0);
	instance->samplenr	=CfgIO_ReadUnsignedInteger(cfg,"General"	,"SampleNr",	0);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);

	CfgIO_WriteUnsignedInteger(cfg,"General"	,"ChannelNr"	,instance->channelnr);
	CfgIO_WriteUnsignedInteger(cfg,"General"	,"SampleNr"		,instance->samplenr);
	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->channelnr=0;
	instance->samplenr=0;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
