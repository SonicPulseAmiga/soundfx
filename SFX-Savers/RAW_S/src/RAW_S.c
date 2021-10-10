/****h* RAW/RAW_S.c [4.3] *
*
*  NAME
*    RAW_S.c
*  COPYRIGHT
*    $VER: RAW_S.c 4.3 (01.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    RAW saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    01.Mar.2004	V 4.3	most recent version
*    29.May.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _savers/RAW_S *
*  Function.eng
*    Saves only the "raw" sample-data.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    A RAW sample realy isn't a format. It's 'raw' sound data. This is an advantage
*    in one way as it's easy to handle. The downside is that no other information
*    but the sample itself is saved (no loop points,bit resolution...).
*  Funktion.ger
*    Speichert nur die "rohen" Sampledaten.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    RAW-Sample haben eigentlich gar kein Format. Hier werden nur die "rohen"
*    Sounddaten abgespeichert. Das hat den Vorteil, daß dieses Format sehr einfach
*    zu handhaben ist, aber auch den Nachteil, daß keinerlei zusätzliche Daten wie
*    Samplingrate, Loops, Bitauflösung usw. gespeichert werden können.
*  Parameter.eng
*    Type
*      type of compression
*        PCM8 : not compressed 8bit
*        PCM16 : not compressed 16bit
*        PCM24 : not compressed 24bit
*        PCM32 : not compressed 32bit
*        PCM16c : not compressed 16bit combined
*        µ-Law : µ-Law (14:8) compressed 14bit
*        µ-Law Inv : µ-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)
*        A-Law : A-Law (14:8) compressed 14bit
*        A-Law Inv : A-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)
*    Endian
*      should SFX convert endians. Intel-proecessor based systems store 16 bit data inverted, this oprions fixes that.
*    Sign
*      store the Sample as a signed or unsigned sample.
*        signed : Amiga, Sgi
*        unsigned : Mac, Atari, PC
*    Channel
*      with how many channels should the sample get stored and in which way.
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        PCM24 : ungepackt 24bit
*        PCM32 : ungepackt 32bit
*        PCM16c : ungepackt 16bit kombiniert
*        µ-Law : µ-Law (14:8) gepackt 14bit
*        µ-Law Inv : µ-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)
*        A-Law : A-Law (14:8) gepackt 14bit
*        A-Law Inv : A-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)
*    Endian
*      ob eine Endiankonvertierung durchgeführt werden soll. Intel-Prozessor basierte
*      Systeme speichern 16bit Wörter umgekehrt und diese Option  korregiert das.
*    Sign
*      ob das Sample als vorzeichenbehaftetes oder nicht-vorzeichenbehaftetes gespeichert werden soll
*        signed : Amiga, Sgi
*        unsigned : Mac, Atari, PC
*    Channel
*      wieviele Kanäle gespeichert werden sollen und wie sie aufgebaut sind.
*  Notes.eng
*    none
*  Hinweise.ger
*   keine
*******
*/

#define RAW_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "RAW.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_TYPE		(GADID_BASE+1)
#define GADIX_TYPE		(GADIX_BASE+1)
#define GADID_ENDIAN	(GADID_TYPE+1)
#define GADIX_ENDIAN	(GADIX_TYPE+1)
#define GADID_SIGN		(GADID_ENDIAN+1)
#define GADIX_SIGN		(GADIX_ENDIAN+1)
#define GADID_CHANNEL	(GADID_SIGN+1)
#define GADIX_CHANNEL	(GADIX_SIGN+1)
#define GADID_SAVEHDR	(GADID_CHANNEL+1)
#define GADIX_SAVEHDR	(GADIX_CHANNEL+1)
#define GADID_HDR		(GADID_SAVEHDR+1)
#define GADIX_HDR		(GADIX_SAVEHDR+1)

#define GADID_SRC       (GADID_HDR+1)
#define GADIX_SRC       (GADIX_HDR+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source		src;        /* Quelle */
	UBYTE format;			/* bits && compression */
	UBYTE sign;				/* 0=unsigned,1=signed */
	UBYTE endian;			/* 0=no,1=yes(convert) */
	UBYTE channel;			/* 0 mono,1 stereo,2 quadro */
	UBYTE savehdr;			/* 0=no,1=yes */
	char  hdr[20];

	PrefsData moddesc[2];
};


char *FormLabels[]={STR_PCM8,STR_PCM16,STR_PCM24,STR_PCM32,STR_ULAW,STR_ULAW_INV,STR_ALAW,STR_ALAW_INV,NULL};
char *SignLabels[]={"signed","unsigned",NULL};
char *EndiLabels[]={"Motorola","Intel",NULL};
char *ChanLabels[]={"mono","stereo","st. interleav.","quadro","qu. interleav.",NULL};

UBYTE ConvFlags[]={
	CONV_SIGNUM_F,					// PCM8
	CONV_SIGNUM_F|CONV_ENDIAN_F,	// PCM16
	CONV_SIGNUM_F|CONV_ENDIAN_F,	// PCM24
	CONV_SIGNUM_F|CONV_ENDIAN_F,	// PCM32
	0,								// ULAW
	0,								// ULAW_INV
	0,								// ALAW
	0								// ALAW_INV
};

UBYTE ChanFlags[]={
	CHANNEL_MONO,
	CHANNEL_STEREO,
	CHANNEL_STEREOI,
	CHANNEL_QUADRO,
	CHANNEL_QUADROI
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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(3);
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
		ng.ng_GadgetText	="Endian";
		ng.ng_GadgetID		=GADID_ENDIAN;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EndiLabels,GTCY_Active,instance->endian,GA_Disabled,((ConvFlags[instance->format]&CONV_ENDIAN_F)?FALSE:TRUE),TAG_DONE);
		modGadgets[GADIX_ENDIAN]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_GadgetText	="Sign";
		ng.ng_GadgetID		=GADID_SIGN;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,SignLabels,GTCY_Active,instance->sign,GA_Disabled,((ConvFlags[instance->format]&CONV_SIGNUM_F)?FALSE:TRUE),TAG_DONE);
		modGadgets[GADIX_SIGN]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[205];
		ng.ng_GadgetText	="Channel";
		ng.ng_GadgetID		=GADID_CHANNEL;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ChanLabels,GTCY_Active,instance->channel,TAG_DONE);
		modGadgets[GADIX_CHANNEL]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[37];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime->scx[26];
		ng.ng_GadgetText	="Hdr";
		ng.ng_GadgetID		=GADID_SAVEHDR;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->savehdr,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_SAVEHDR]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[64];
		ng.ng_Width			=RunTime->scx[237];
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_HDR;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,19,GTST_String,instance->hdr,TAG_DONE);
		modGadgets[GADIX_HDR]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Miscellaneous" ,xs1,ys2,STDFRM_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(3));               /* Miscellaneous */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);                  /* Go */

			SetSource   (&instance->src,modWin,modGadgets,GADIX_SRC);
			if(!instance->savehdr)  GT_SetGadgetAttrs(modGadgets[GADIX_HDR   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);

			if(!enabled) GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);

			sigin=(1L<<modWin->UserPort->mp_SigBit)|(1L<<nt.sig);

			while(runing) {
				sigout=Wait(sigin);
				if(sigout&(1L<<nt.sig)) {
					if(!enabled && RunTime->aktsamples) {
						enabled=TRUE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
						instance->src.srcbuf=LockBuffer(RunTime->aktbuf);
						instance->src.rmode=SetRngMode(instance->src.srcbuf);
					}
					else if(enabled && !RunTime->aktsamples) {
						enabled=FALSE;
						GT_SetGadgetAttrs(modGadgets[GADIX_GO    ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
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
											RefreshGUI(instance,modGadgets,modWin);
											break;
										case GADID_ENDIAN:
											instance->endian=icode;
											break;
										case GADID_SIGN:
											instance->sign=icode;
											break;
										case GADID_CHANNEL:
											instance->channel=icode;
											break;
										case GADID_SAVEHDR:
											instance->savehdr=modGadgets[GADIX_SAVEHDR]->Flags&GFLG_SELECTED;
											if(instance->savehdr) GT_SetGadgetAttrs(modGadgets[GADIX_HDR   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
											else 				 GT_SetGadgetAttrs(modGadgets[GADIX_HDR   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
											break;
										case GADID_HDR:
											strncpy(instance->hdr,((struct StringInfo *)(modGadgets[GADIX_HDR]->SpecialInfo))->Buffer,19);
											instance->hdr[19]='\0';
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB: if(instance->savehdr) ActivateGadget(modGadgets[GADIX_HDR],modWin,NULL);break;
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
	FILE *outfile;
	SaveData sdata;
	SInfo *si;
	UBYTE ret=TRUE;

	switch(pdata->callmd) {
		case CM_GUI:
			break;
		case CM_DIRECT:             // wir wurden e.g. per ARexx gestartet
			UnlockBuffer(instance->src.srcbuf);
			ret&=((instance->src.srcbuf=LockBuffer(pdata->src[0]))!=0);
			if(!ret) return(FALSE);
			instance->src.rmode=RNGMD_ALL;
			break;
	}
	si=instance->src.srcbuf;

	SetRngs(si,&sdata.savestart,&sdata.savelength,instance->src.rmode);
	strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]='\0';
	sdata.chmask=CH_ALL;

	MSG1("callmd = %ld",pdata->callmd);
	MSG1("rngmd = %ld",instance->src.rmode);
	MSG2("saving range from %ld of %ld length",sdata.savestart,sdata.savelength);

	if((outfile=fopen(pdata->fn,"wb"))) {
		if(instance->savehdr) fwrite(instance->hdr,strlen(instance->hdr),1,outfile);	// save header
		sdata.offs=ftell(outfile);
		fclose(outfile);
		switch(instance->format) {
			case FORMAT_RAW_PCM8:	  WriteData(si,&sdata,CRUNCH_PCM8    ,CONV(0,instance->sign),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_PCM16:	  WriteData(si,&sdata,CRUNCH_PCM16   ,CONV(instance->endian,instance->sign),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_PCM24:	  WriteData(si,&sdata,CRUNCH_PCM24   ,CONV(instance->endian,instance->sign),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_PCM32:	  WriteData(si,&sdata,CRUNCH_PCM32   ,CONV(instance->endian,instance->sign),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_ULAW:	  WriteData(si,&sdata,CRUNCH_ULAW    ,CONV(0,0),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_ULAW_INV: WriteData(si,&sdata,CRUNCH_ULAW_INV,CONV(0,0),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_ALAW:	  WriteData(si,&sdata,CRUNCH_ALAW    ,CONV(0,0),ChanFlags[instance->channel]);break;
			case FORMAT_RAW_ALAW_INV: WriteData(si,&sdata,CRUNCH_ALAW_INV,CONV(0,0),ChanFlags[instance->channel]);break;
		}
	}
	else { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	//-- set file comment
	{
		UBYTE ch=si->channels;
		switch(ChanFlags[instance->channel]) {
			case CHANNEL_MONO:
				ch=CH_MONO;break;
			case CHANNEL_STEREO:
			case CHANNEL_STEREOI:
				ch=CH_STEREO;break;
			case CHANNEL_QUADRO:
			case CHANNEL_QUADROI:
				ch=CH_QUADRO;break;
		}
		SetSampleComment(si,pdata->fn,PRJ_NAME,FormLabels[instance->format]);
		si->channels=ch;
	}
	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ instance->format =CfgIO_MatchString(RexxPar3,FormLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Sign"))	{ instance->sign   =CfgIO_MatchString(RexxPar3,SignLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Endian"))	{ instance->endian =CfgIO_MatchString(RexxPar3,EndiLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Channel"))	{ instance->channel=CfgIO_MatchString(RexxPar3,ChanLabels);pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ sprintf(ret,"%s",FormLabels[instance->format ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Sign"))	{ sprintf(ret,"%s",SignLabels[instance->sign   ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Endian"))	{ sprintf(ret,"%s",EndiLabels[instance->endian ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Channel"))	{ sprintf(ret,"%s",ChanLabels[instance->channel]);/*pf=TRUE;*/ }
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
					 CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_RAW_PCM8]),FormLabels);
	instance->sign	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Sign"			,SignLabels[0]),SignLabels);
	instance->endian	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Endian"		,EndiLabels[0]),EndiLabels);
	instance->channel=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Channel"		,ChanLabels[CHANNEL_MONO]),ChanLabels);
	instance->savehdr=CfgIO_ReadBool			(cfg,"General"	,"SaveHdr"		,FALSE);
	strncpy(instance->hdr,CfgIO_ReadString		(cfg,"General"	,"Hdr"			,"\0"),19);instance->hdr[19]='\0';
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);
	CfgIO_WriteString		  (cfg,"General"	,"Sign"			,SignLabels[instance->sign]);
	CfgIO_WriteString		  (cfg,"General"	,"Endian"		,EndiLabels[instance->endian]);
	CfgIO_WriteString		  (cfg,"General"	,"Channel"		,ChanLabels[instance->channel]);
	CfgIO_WriteBool			  (cfg,"General"	,"SaveHdr"		,instance->savehdr);
	CfgIO_WriteString		  (cfg,"General"	,"Hdr"			,instance->hdr);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=NULL;
	instance->src.rmode=RNGMD_ALL;

	instance->format=FORMAT_RAW_PCM8;
	instance->sign=0;			/* signed */
	instance->endian=0;			/* no */
	instance->channel=CHANNEL_MONO;
	instance->savehdr=FALSE;			/* no header */
	instance->hdr[0]='\0';
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_TYPE   ],modWin,NULL,GTCY_Active,instance->format,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_CHANNEL],modWin,NULL,GTCY_Active,instance->channel,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SIGN   ],modWin,NULL,GTCY_Active,instance->sign  ,GA_Disabled,((ConvFlags[instance->format]&CONV_SIGNUM_F)?FALSE:TRUE),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ENDIAN ],modWin,NULL,GTCY_Active,instance->endian,GA_Disabled,((ConvFlags[instance->format]&CONV_ENDIAN_F)?FALSE:TRUE),TAG_DONE);
	if(!instance->savehdr)  GT_SetGadgetAttrs(modGadgets[GADIX_HDR   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);

}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
