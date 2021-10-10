/****h* TX16W/TX16W_S.c [4.3] *
*
*  NAME
*    TX16W_S.c
*  COPYRIGHT
*    $VER: TX16W_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    TX16W saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Apr.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.20	most recent version
*    13.Sep.1998	V 1.10	most recent version
*    02.Jul.1997	V 1.00	initial version
*  NOTES
*
*******
*/

/****u* _savers/TX16W_S *
*  Function.eng
*    Saves samples for the Yamaha TX16W.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>no (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-12)</td></tr>
*    </table>
*    These samples are always 12-bit, are limited in length to 262144 samples
*    (attack- and sustainpart) and supporting only three different rates
*    (16 kHz, 33 kHz, 50 kHz).
*  Funktion.ger
*    Speichert Samples für den Yamaha TX16W.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>nein (mono)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>nein (PCM-12)</td></tr>
*    </table>
*    Diese Samples sind immer 12-bit, in Ihrer Länge begrenzt auf 262144 Samples
*    (Attack- und Sustainpart) and unterstützen nur drei verschiedene Raten
*    (16 kHz, 33 kHz, 50 kHz).
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

#define TX16W_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "TX16W.h"


//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_SRC       (GADID_BASE+1)
#define GADIX_SRC       (GADIX_BASE+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source          src;        /* Quelle */

	PrefsData moddesc[2];	
};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(cfg)             CfgIO_RemConfig(cfg);
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
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge      =xs1+RunTime->scx[(308>>1)-110];
		ng.ng_TopEdge       =ys2+RunTime->scy[2];
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

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,CMDFRM_HEIGHT);                  /* Go */

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
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									//case KEY_TAB: ActivateGadget(modGadgets[GADIX_],modWin,0l);break;
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
	register UBYTE i;
	struct SampleHeader shdr;
	FILE *outfile;
	ULONG slen1,slen2;
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

	//-- !!! LOCALIZE !!!
	if(si->channels>CH_MONO) { Message(errWriteData,NULL,__FILE__,__LINE__);return(FALSE); }
	//-- !!! LOCALIZE !!!

	SetRngs(si,&sdata.savestart,&sdata.savelength,instance->src.rmode);
	strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
	sdata.chmask=CH_ALL;

	if((outfile=fopen(pdata->fn,"wb"))) {
		if(!fwrite(&"LM8953",6,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
		for(i=0;i<10;i++) shdr.nulls[i]=0;
		shdr.dummy_aeg[0]=shdr.dummy_aeg[1]=0;
		shdr.dummy_aeg[2]=shdr.dummy_aeg[3]=shdr.dummy_aeg[4]=0x7F;
		shdr.dummy_aeg[5]=0x3F;
		if(instance->src.rmode==RNGMD_ALL) {
			switch(si->loop) {
				case SFXLOOP_NONE:
				case SFXLOOP_PINGPONG:
					shdr.format=FORMAT_NOTLOOPED;
					if(sdata.savelength>131072) {
						Message(errSampleToLong,NULL,__FILE__,__LINE__);
						sdata.savelength=131072;
					}
					shdr.atc_length[0]= sdata.savelength     &0xFF;shdr.rpt_length[0]=0;
					shdr.atc_length[1]=(sdata.savelength>> 8)&0xFF;shdr.rpt_length[1]=0;
					shdr.atc_length[2]=(sdata.savelength>>16)&0x01;shdr.rpt_length[2]=0;
					break;
				case SFXLOOP_FORWARD:
					shdr.format=FORMAT_LOOPED;
					slen1=si->loops;
					if(slen1>131072) {
						Message(errSampleToLong,NULL,__FILE__,__LINE__);
						slen1=131072;
					}
					slen2=si->loopl;
					if(slen2>131072) {
						Message(errSampleToLong,NULL,__FILE__,__LINE__);
						slen2=131072;
					}
					sdata.savelength=slen1+slen2;
					shdr.atc_length[0]= slen1     &0xFF;shdr.rpt_length[0]= slen2     &0xFF;
					shdr.atc_length[1]=(slen1>> 8)&0xFF;shdr.rpt_length[1]=(slen2>> 8)&0xFF;
					shdr.atc_length[2]=(slen1>>16)&0x01;shdr.rpt_length[2]=(slen2>>16)&0x01;
					break;
			}
		}
		else {
			shdr.format=FORMAT_NOTLOOPED;
			if(sdata.savelength>131072) {
				Message(errSampleToLong,NULL,__FILE__,__LINE__);
				sdata.savelength=131072;
			}
			shdr.atc_length[0]= sdata.savelength     &0xFF;shdr.rpt_length[0]=0;
			shdr.atc_length[1]=(sdata.savelength>> 8)&0xFF;shdr.rpt_length[1]=0;
			shdr.atc_length[2]=(sdata.savelength>>16)&0x01;shdr.rpt_length[2]=0;
		}
		if(si->srat<24500) {
			shdr.sample_rate=RATE_16000;
			shdr.atc_length[2]|=RATE_16000_0;
			shdr.rpt_length[2]|=RATE_16000_1;
		}
		else if(si->srat<41500) {
			shdr.sample_rate=RATE_33000;
			shdr.atc_length[2]|=RATE_33000_0;
			shdr.rpt_length[2]|=RATE_33000_1;
		}
		else {
			shdr.sample_rate=RATE_50000;
			shdr.atc_length[2]|=RATE_50000_0;
			shdr.rpt_length[2]|=RATE_50000_1;
		}
		shdr.unused[0]=0xFF;shdr.unused[1]=0x00;
		if(!fwrite(&shdr,sizeof(struct SampleHeader),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

		strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
		sdata.offs=ftell(outfile);
		fclose(outfile);
		WriteData(si,&sdata,CRUNCH_PCM12,CONV(0,0),CHANNEL_MONO);
	}
	else { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	//-- set file comment
	{
		UBYTE ch=si->channels;
		si->channels=CH_MONO;
		SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM12);
		si->channels=ch;
	}

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff --------------------------------------------------------------

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	LoadModSettings(cfg,instance->moddesc);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
