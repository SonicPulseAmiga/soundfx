/****h* IFF-MAUD/IFF-MAUD_S.c [4.3] *
*
*  NAME
*    IFF-MAUD_S.c
*  COPYRIGHT
*    $VER: IFF-MAUD_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    IFF-MAUD saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1995
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Apr.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    30.Oct.1998	V 1.1	most recent version
*    02.Jul.1995	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _savers/IFF-MAUD_S *
*  Function.eng
*    Saves IFF-MAUD samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,A-LAW,µ-LAW)</td></tr>
*    </table>
*    This is an IFF-type format, which was introduced by MacroSystems (the producer
*    of the Toccata and Maestro-boards). Is supports multichannel samples, several
*    bit resolutions and compression.
*  Funktion.ger
*    Speichert IFF-MAUD Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dies ist ein dem IFF-Standart entsprechendes Dateiformat,welches von der Firma
*    MacroSystems (die Hersteller der Toccata und Maestro-Karten) eingeführt wurde.
*    Diese Format unterstützt mehrkanalige Samples, unterschiedliche Bitauflösungen
*    und Kompression der Audiodaten.
*  Parameter.eng
*    Type
*      type of compression
*        PCM8 : not compressed 8bit
*        PCM16 : not compressed 16bit
*        PCM24 : not compressed 24bit
*        PCM32 : not compressed 32bit
*        FDPCM8_4 : FibonacciDelta (2:1) compressed 8bit
*        µ-Law : µ-Law (14:8) compressed 14bit
*        A-Law : A-Law (14:8) compressed 14bit
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        PCM24 : ungepackt 24bit
*        PCM32 : ungepackt 32bit
*        FDPCM8_4 : FibonacciDelta (2:1) gepackt 8bit
*        µ-Law : µ-Law (14:8) gepackt 14bit
*        A-Law : A-Law (14:8) gepackt 14bit
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define IFF_MAUD_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "IFF-MAUD.h"


//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_FORMAT	(GADID_BASE+1)
#define GADIX_FORMAT	(GADIX_BASE+1)

#define GADID_SRC       (GADID_FORMAT+1)
#define GADIX_SRC       (GADIX_FORMAT+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source			src;        /* Quelle */
	UBYTE format;			/* bits && compression */

	PrefsData moddesc[2];
};

char *FormLabels[]={STR_PCM8,STR_PCM16,STR_PCM24,STR_PCM32,STR_FDPCM8_4,STR_ULAW,STR_ALAW,0l};

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
	if(cfg)				CfgIO_RemConfig(cfg);
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
		ng.ng_LeftEdge		=xs1+RunTime->scx[(308>>1)-110];
		ng.ng_TopEdge		=ys3+RunTime->scy[2];
		ng.ng_Width			=RunTime->scx[100];
		ng.ng_Height		=RunTime->scy[11];
		ng.ng_GadgetText	=GetString(btnSave);
		ng.ng_TextAttr		=RunTime->ScrFontB;
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=RunTime->VisualInfo;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[(308>>1)+10];
		ng.ng_GadgetText	=GetString(menPrefsUse);
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetID		=GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		g=AddSource(&instance->src,modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[132];
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_FORMAT;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_FORMAT]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous" ,xs1,ys2,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;
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
										case GADID_FORMAT:
											instance->format=icode;
											break;
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
	struct MaudHeader mhdr;
	FILE *outfile;
	LONG subtype;
	ULONG bufl,size;
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
	if(si->channels>CH_STEREO) { Message(errWriteData,NULL,__FILE__,__LINE__);return(FALSE); }
	//-- !!! LOCALIZE !!!

	if(!(outfile=fopen(pdata->fn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	size=4L;
	size+=(long)(sizeof(mhdr)+8L);					/* MHDR */
	bufl=Samples2Bytes(sdata.savelength);
	switch(instance->format) {
		case FORMAT_MAUD_PCM8:		bufl=GetCrunchedLength(CRUNCH_PCM8,	   bufl);mhdr.SampleSizeC= 8;mhdr.SampleSizeU= 8;mhdr.Compression=COMPMAUD_PCM;break;
		case FORMAT_MAUD_PCM16:		bufl=GetCrunchedLength(CRUNCH_PCM16,   bufl);mhdr.SampleSizeC=16;mhdr.SampleSizeU=16;mhdr.Compression=COMPMAUD_PCM;break;
		case FORMAT_MAUD_PCM24:		bufl=GetCrunchedLength(CRUNCH_PCM24,   bufl);mhdr.SampleSizeC=24;mhdr.SampleSizeU=24;mhdr.Compression=COMPMAUD_PCM;break;
		case FORMAT_MAUD_PCM32:		bufl=GetCrunchedLength(CRUNCH_PCM32,   bufl);mhdr.SampleSizeC=32;mhdr.SampleSizeU=32;mhdr.Compression=COMPMAUD_PCM;break;
		case FORMAT_MAUD_FDPCM8_4:	bufl=GetCrunchedLength(CRUNCH_FDPCM8_4,bufl);mhdr.SampleSizeC= 4;mhdr.SampleSizeU= 8;mhdr.Compression=COMPMAUD_FDPCM;break;
		case FORMAT_MAUD_ULAW:		bufl=GetCrunchedLength(CRUNCH_ULAW,    bufl);mhdr.SampleSizeC= 8;mhdr.SampleSizeU=16;mhdr.Compression=COMPMAUD_ULAW;break;
		case FORMAT_MAUD_ALAW:		bufl=GetCrunchedLength(CRUNCH_ALAW,    bufl);mhdr.SampleSizeC= 8;mhdr.SampleSizeU=16;mhdr.Compression=COMPMAUD_ALAW;break;
	}
	bufl*=(si->channels+1);
	size+=(bufl+8L);								/* MDAT */

	WriteChunkHeader(IFF_FORM,size,outfile);		/* Grundstruktur schreiben */
	subtype=IFF_MAUD;
	if(!fwrite(&subtype,sizeof(subtype),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

	WriteChunkHeader(IFF_MHDR,sizeof(mhdr),outfile);
	mhdr.Samples		=sdata.savelength*(si->channels+1);
	mhdr.RateSource		=si->srat;
	mhdr.RateDevide		=1;
	if(si->channels==0) mhdr.ChannelInfo=MCI_MONO;
	if(si->channels==1) mhdr.ChannelInfo=MCI_STEREO;
	mhdr.Channels		=si->channels+1;
	mhdr.Reserved1=mhdr.Reserved1=mhdr.Reserved1=0l;
	if(!fwrite(&mhdr,sizeof(mhdr),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

	WriteChunkHeader(IFF_MDAT,bufl,outfile);
	sdata.offs=ftell(outfile);
	fclose(outfile);
	switch(instance->format) {
		case FORMAT_MAUD_PCM8:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_PCM16:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_PCM24:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_PCM32:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_FDPCM8_4:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_FDPCM8_4,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_ULAW:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
		case FORMAT_MAUD_ALAW:
			switch(si->channels) {
				case CH_MONO:	WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO:	WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
			}
			break;
	}

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,FormLabels[instance->format]);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ instance->format=CfgIO_MatchString(RexxPar3,FormLabels);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ sprintf(ret,"%s",FormLabels[instance->format]);/*pf=1;*/ }
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
	instance->format	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_MAUD_PCM16]),FormLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->format=FORMAT_MAUD_PCM16;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
