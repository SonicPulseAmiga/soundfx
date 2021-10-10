/****h* SND-AU/SND-AU_S.c [4.3] *
*
*  NAME
*    SND-AU_S.c
*  COPYRIGHT
*    $VER: SND-AU_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    SND-AU saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Apr.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    30.Oct.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _savers/SND-AU_S *
*  Function.eng
*    Saves SND-AU samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
*    </table>
*    These samples come mainly from the SUN, NEXT or DEC computers or in common :
*    most UNIX-based machines are using this format. The format is pretty simple -
*    a small header followed by the sound data. In most cases these are µ-Law packed.
*  Funktion.ger
*    Speichert SND-AU Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kan‰le     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
*    </table>
*    Diese Samples stammen von SUN-, NEXT- oder DEC-Rechnern bzw. auf Rechnern
*    die unter UNIX arbeiten. Das Formate ist recht einfach aufgebaut - ein
*    einfacher Header und dann die Sounddaten. Diese sind meistens µ-Law gepackt.
*  Parameter.eng
*    Type
*      type of compression
*        PCM8 : not compressed 8bit
*        PCM16 : not compressed 16bit
*        PCM24 : not compressed 24bit
*        PCM32 : not compressed 32bit
*        µ-Law : µ-Law (14:8) compressed 14bit
*        A-Law : A-Law (14:8) compressed 14bit
*        IEEE-32 : floating point 32bit
*        IEEE-64 : floating point 64bit
*    Hdr
*      Fileheader
*        SND : SUN's
*        DEC : DEC-workstation's
*        I_SND,I_DEC : PC with UNIX (LINUX)
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        PCM24 : ungepackt 24bit
*        PCM32 : ungepackt 32bit
*        PCM16c : ungepackt 16bit kombiniert
*        µ-Law : µ-Law (14:8) gepackt 14bit
*        A-Law : A-Law (14:8) gepackt 14bit
*        IEEE-32 : Flieﬂkomma 32bit
*        IEEE-64 : Flieﬂkomma 64bit
*    Hdr
*      Dateikopf
*        SND : SUN's
*        DEC : DEC-workstation's
*        I_SND,I_DEC : PC mit UNIX (LINUX)
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define SND_AU_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "SND-AU.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_TYPE		(GADID_BASE+1)
#define GADIX_TYPE		(GADIX_BASE+1)
#define GADID_HEADER	(GADID_TYPE+1)
#define GADIX_HEADER	(GADIX_TYPE+1)

#define GADID_SRC       (GADID_HEADER+1)
#define GADIX_SRC       (GADIX_HEADER+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source			src;        /* Quelle */
	UBYTE format;			/* bits && compression */
	UBYTE header;			/* SND,DEC,ISND,IDEC */

	PrefsData moddesc[2];
};

char *FormLabels[]={STR_PCM8,STR_PCM16,STR_PCM24,STR_PCM32,STR_ULAW,STR_ALAW,STR_IEEE32,STR_IEEE64,0l};
char *HdrLabels[]={"SND","DEC","I_SND","I_DEC",0l};

SFXMod_Globals

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
		ng.ng_Width			=RunTime->scx[95];
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_TYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_TYPE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[205];
		ng.ng_GadgetText	="Header";
		ng.ng_GadgetID		=GADID_HEADER;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,HdrLabels,GTCY_Active,instance->header,TAG_DONE);
		modGadgets[GADIX_HEADER]=g;

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
										case GADID_TYPE:
											instance->format=icode;
											break;
										case GADID_HEADER:
											instance->header=icode;
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
	struct SunHeader shdr;
	FILE *outfile;
	LONG namelen;
	UBYTE hdrfmt=1;
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

	if(!(outfile=fopen(pdata->fn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	namelen=strlen(((struct Node *)si)->ln_Name)+1;
	switch(instance->header) {
		case HEADER__SND: shdr.id=SUN__SND;hdrfmt=1;break;
		case HEADER__DEC: shdr.id=SUN__DEC;hdrfmt=1;break;
		case HEADER_ISND: shdr.id=SUN_ISND;hdrfmt=2;break;
		case HEADER_IDEC: shdr.id=SUN_IDEC;hdrfmt=2;break;
	}
	shdr.datalen=Samples2Bytes(sdata.savelength);
	switch(instance->format) {
		case FORMAT_SND_PCM8:	shdr.datalen=GetCrunchedLength(CRUNCH_PCM8,  shdr.datalen);shdr.format=SND_FORMAT_LINEAR_8;break;
		case FORMAT_SND_PCM16:	shdr.datalen=GetCrunchedLength(CRUNCH_PCM16, shdr.datalen);shdr.format=SND_FORMAT_LINEAR_16;break;
		case FORMAT_SND_PCM24:	shdr.datalen=GetCrunchedLength(CRUNCH_PCM24, shdr.datalen);shdr.format=SND_FORMAT_LINEAR_24;break;
		case FORMAT_SND_PCM32:	shdr.datalen=GetCrunchedLength(CRUNCH_PCM32, shdr.datalen);shdr.format=SND_FORMAT_LINEAR_32;break;
		case FORMAT_SND_ULAW:	shdr.datalen=GetCrunchedLength(CRUNCH_ULAW,  shdr.datalen);shdr.format=SND_FORMAT_MULAW_8;break;
		case FORMAT_SND_ALAW:	shdr.datalen=GetCrunchedLength(CRUNCH_ALAW,	 shdr.datalen);shdr.format=SND_FORMAT_ALAW_8;break;
		case FORMAT_SND_IEEE32:	shdr.datalen=GetCrunchedLength(CRUNCH_IEEE32,shdr.datalen);shdr.format=SND_FORMAT_FLOAT;break;
		case FORMAT_SND_IEEE64:	shdr.datalen=GetCrunchedLength(CRUNCH_IEEE64,shdr.datalen);shdr.format=SND_FORMAT_DOUBLE;break;
	}
	shdr.datalen*=(si->channels+1);
	shdr.rate=si->srat;
	shdr.channels=si->channels+1;
	shdr.hdrlen=namelen+sizeof(struct SunHeader);
	if(hdrfmt==2) {
		shdr.hdrlen=	LitEnd2BigEnd_ulong(shdr.hdrlen);
		shdr.datalen=	LitEnd2BigEnd_ulong(shdr.datalen);
		shdr.format=	LitEnd2BigEnd_ulong(shdr.format);
		shdr.rate=		LitEnd2BigEnd_ulong(shdr.rate);
		shdr.channels=	LitEnd2BigEnd_ulong(shdr.channels);
	}
	if(!fwrite(&shdr,sizeof(struct SunHeader),1,outfile))	{			/* Grundstrucktur schreiben */
		Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE);
	}
	if(!fwrite(((struct Node *)si)->ln_Name,namelen,1,outfile)) {						/* Name schreiben */
		Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE);
	}

	sdata.offs=ftell(outfile);
	fclose(outfile);
	switch(instance->format) {
		case FORMAT_SND_PCM8:
			switch(si->channels) {
				case 0: WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,0),CHANNEL_MONO);break;
				case 1: WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,0),CHANNEL_STEREOI);break;
				case 3: WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_SND_PCM16:
			if(hdrfmt==1) {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case FORMAT_SND_PCM24:
			if(hdrfmt==1) {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM24,CONV(0,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM24,CONV(0,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM24,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case FORMAT_SND_PCM32:
			if(hdrfmt==1) {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM32,CONV(0,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM32,CONV(0,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM32,CONV(0,0),CHANNEL_QUADROI);break;
				}
			}
			else {
				switch(si->channels) {
					case 0: WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
					case 1: WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
					case 3: WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
				}
			}
			break;
		case FORMAT_SND_ULAW:
			switch(si->channels) {
				case 0: WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
				case 1: WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
				case 3: WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_SND_ALAW:
			switch(si->channels) {
				case 0: WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
				case 1: WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
				case 3: WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_SND_IEEE32:
			switch(si->channels) {
				case 0: WriteData(si,&sdata,CRUNCH_IEEE32,CONV(0,0),CHANNEL_MONO);break;
				case 1: WriteData(si,&sdata,CRUNCH_IEEE32,CONV(0,0),CHANNEL_STEREOI);break;
				case 3: WriteData(si,&sdata,CRUNCH_IEEE32,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_SND_IEEE64:
			switch(si->channels) {
				case 0: WriteData(si,&sdata,CRUNCH_IEEE64,CONV(0,0),CHANNEL_MONO);break;
				case 1: WriteData(si,&sdata,CRUNCH_IEEE64,CONV(0,0),CHANNEL_STEREOI);break;
				case 3: WriteData(si,&sdata,CRUNCH_IEEE64,CONV(0,0),CHANNEL_QUADROI);break;
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
		if(!pf && !stricmp(RexxPar2,"Header"))	{ instance->header=CfgIO_MatchString(RexxPar3,HdrLabels);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ sprintf(ret,"%s",FormLabels[instance->format]);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Header"))	{ sprintf(ret,"%s",HdrLabels[instance->header]);/*pf=1;*/ }
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
	instance->header	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Header"		,HdrLabels [HEADER__SND]),HdrLabels);
	instance->format	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_SND_PCM8]),FormLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Header"		,HdrLabels [instance->header]);
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->format=FORMAT_SND_PCM8;
	instance->header=HEADER__SND;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_TYPE   ],modWin,0l,GTCY_Active,instance->format,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_HEADER ],modWin,0l,GTCY_Active,instance->header,TAG_DONE);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
