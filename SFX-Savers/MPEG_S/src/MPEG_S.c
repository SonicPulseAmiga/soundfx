/****h* MPEG/MPEG_S.c [4.3] *
*
*  NAME
*    MPEG_S.c
*  COPYRIGHT
*    $VER: MPEG_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    MPEG saver for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    17.Oct.2003	V 4.3	most recent version
*    19.Apr.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*							Add : support for several encoder engines
*    10.Feb.1999	V 1.1	most recent version
*    23.Jan.1999	V 1.0	initial version
*  NOTES
*    add parameter 'encoderoutput' which toogles the kind of handle we open
*    add params for pipes
*		pipe:		/250000
*		apipe:
*		awnpipe:
*
*******
*/

/****u* _savers/MPEG_S *
*  Function.eng
*    Saves highly compressed MPEG samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes</td></tr>
*    </table>
*    Due to the high compression ratio, this will take a while. In fact it is
*    highly recomended to have an 68060 for this.<br>
*    This module uses external encoders (separate programs).
*    Therfore I've tried to make it highly configurable.
*  Funktion.ger
*    Speichert hochkomprimierte MPEG Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja</td></tr>
*    </table>
*    Wegen der hohen Kompression, kann das Abspeichern eine Weile dauern. Es
*    ist empfehlenswert mindestens einen 68060'er für soetwas zu besitzen.<br>
*    Dieses Modul benutzt externe Encoder (separate Programme).
*    Deshalb habe ich versucht es möglichst konfigurierbar zu halten.
*  Parameter.eng
*    Encoder
*      Choose the executable of the encoder you want to use. It has been testet with the
*      supplied 8Hz, as well as with  Pegase, Lame and Ncoder.
*    Parameter
*      This is the parameter template which is passed on the command line to the encoder
*      (the encoder will be run as a background process and be feed with data from __SFX).
*      These placeholders are currently suported :
*        %b : the bitrate
*        %c : the parameter string for mono/stereo files (see below)
*        %i : the input filename
*        %o : the output filename
*        %r0 : the samplingrate in Hz
*        %r1 : the samplingrate in kHz (at the moment just 32, 44.1, 48)
*    MonoStr
*      The parameter string for mono-files which is used above with "%c".
*    StereoStr
*      The parameter string for stereo-files which is used above with "%c".
*    Wave
*      This determines in which format the sample data is passed to the encoder.
*        CDDA
*        RIFF-WAV
*    BitRate
*      Strength of compression. Says how many bits per second are allowed. The
*      lower the bitrate is, the lower is the quality.
*    Pipe
*      If you have problems with the pipe: device, you may try an alternative one
*      like apipe: or awnpipe:.
*  Parameter.ger
*    Encoder
*      Wählen sie die Programmdatei des zu verwendenten Encoders. Es wurden bisher
*      der mitgeliferte 8Hz, sowie Pegase, Lame und Ncoder getestet.
*    Parameter
*      Dies ist die Parameter Vorlage, die dem Encoder auf der Kommandozeile übergeben wird
*      (der Encoder wird als Hintergrundprozess gestartet und vom __SFX mit Daten versorgt).
*      Folgende Platzhalter sind hier derzeit erlaubt :
*        %b : die Bitrate
*        %c : der Parameterstring für Mono/Stereodateien (siehe unten)
*        %i : der Name der Eingangsdatei
*        %o : der Name der Ergebnisdatei
*        %r0 : die Samplingrate in Hz
*        %r1 : die Samplingrate in kHz (zur Zeit nur 32, 44.1, 48)
*    MonoStr
*      Der Parameter für Monodateien welcher oben in Verbindung mit "%c" genutzt wird.
*    StereoStr
*      Der Parameter für Stereodateien welcher oben in Verbindung mit "%c" genutzt wird.
*    Wave
*      Hiermit geben sie an, in welchem Format die Sampledaten dem Encoder übergeben
*      werden sollen.
*        CDDA
*        RIFF-WAV
*    BitRate
*      Stärke der Kompression. Gibt an, wieviele Bits pro Sekunde erlaubt sind.
*      Je niedriger der Wert ist, desto niedriger ist die Qualität.
*    Pipe
*      Wenn sie Probleme mit dem pipe: Gerät haben, konnen sie auch eine Pipe
*      wie apipe: oder awnpipe: einsetzen.
*  Notes.eng
*    There are presets for most common encoders supplied. Still it is neccesary
*    that you adapt the path of the respective encoder executable or copy
*    the binaries to the sfx/_saver folder under the respective name.<br>
*    If you want to adapt it for further encoders, I recomend studing the
*    supplied ones.<br>
*    The saver uses a pipe device to send data to the encoder engine.
*    Please make sure the selected pipe-device is mounted.
*  Hinweise.ger
*    Es werden presets für die gängigen Encoder mitgeliefert. Es ist jedoch noch
*    notwendig, das sie die Pfade der entsprechenden Encoder anpassen oder
*    die Programme in das sfx/_savers Verzeichnis unter dem entspechenden namen kopieren.<br>
*    Wenn sie Anpassungen für weitere Encoder erstellen möchten, empehle ich sich
*    die beliegenden Prests anzusehen.<br>
*    Der Saver benutzt ein Pipe Gerät um die Daten zu dem Encoder zu senden.
*    Bitte stellen sie sicher das die gewählte Pipe gemountet ist.
*******
*/

#define MPEG_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "MPEG.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_CFGSEL		(GADID_BASE+1)
#define GADIX_CFGSEL		(GADIX_BASE+1)
#define GADID_CFGLIST		(GADID_CFGSEL)
#define GADIX_CFGLIST		(GADIX_CFGSEL)
#define GADID_CFGNAME		(GADID_CFGLIST+1)
#define GADIX_CFGNAME		(GADIX_CFGLIST+1)
#define GADID_CFGADD		(GADID_CFGNAME+1)
#define GADIX_CFGADD		(GADIX_CFGNAME+1)
#define GADID_CFGDEL		(GADID_CFGADD+1)
#define GADIX_CFGDEL		(GADIX_CFGADD+1)

#define GADID_ENGINEPOPUP	(GADID_CFGSEL+CFGSEL_IDCT)
#define GADIX_ENGINEPOPUP	(GADIX_CFGSEL+CFGSEL_IXCT)
#define GADID_ENGINE		(GADID_ENGINEPOPUP+1)
#define GADIX_ENGINE		(GADIX_ENGINEPOPUP+1)
#define GADID_ENGINEPAR		(GADID_ENGINE+1)
#define GADIX_ENGINEPAR		(GADIX_ENGINE+1)
#define GADID_MONOSTR		(GADID_ENGINEPAR+1)
#define GADIX_MONOSTR		(GADIX_ENGINEPAR+1)
#define GADID_STEREOSTR		(GADID_MONOSTR+1)
#define GADIX_STEREOSTR		(GADIX_MONOSTR+1)
#define GADID_WAVETYPE		(GADID_STEREOSTR+1)
#define GADIX_WAVETYPE		(GADIX_STEREOSTR+1)

#define GADID_BITRATE		(GADID_WAVETYPE+1)
#define GADIX_BITRATE		(GADIX_WAVETYPE+1)
#define GADID_PIPEDEV		(GADID_BITRATE+1)
#define GADIX_PIPEDEV		(GADIX_BITRATE+1)

#define GADID_SRC			(GADID_PIPEDEV+1)
#define GADIX_SRC			(GADIX_PIPEDEV+1)

#define LAST_GADIX      	(GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source			src;        /* Quelle */
	UBYTE	engine[FILENAME_MAX];			// which encoder to use
	UBYTE	enginepar[FILENAME_MAX];		// what parameter to supply
	UBYTE	monostr[40];			// how does the parameter for a mono-encoding looks like
	UBYTE	stereostr[40];			// how does the parameter for a stereo-encoding looks like
	UBYTE	wavetype;				// what does the encoder wants cdda, wav
	UWORD	bitrate;				// 32,40,48,56,64,80,96,112,128,160,192,224,256 and 320
	Param	pipedev;				// which pipe to use

	PrefsData moddesc[2];
};

char *WaveLabels[]={"CDDA","RIFF-WAV",0l};

#define WT_CDDA		0
#define WT_RIFF_WAV	1

char *BitrLabels[]={"32","40","48","56","64","80","96","112","128","160","192","224","256","320",0l};

//-- Library-Basisaddrs.

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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(5);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+STDFRM_WIDTH;
	UWORD winw=xs2+3+CFGSEL_WIDTH;
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

		InitCfgSel(&cfgsel,PROGDIR"_savers/"PRJ_NAME);
		g=AddCfgSel(&cfgsel,modGadgets,g,GADID_CFGSEL,GADIX_CFGSEL,xs2,ys1,winh-2);

		g=AddSource(&instance->src,modGadgets,g,GADID_SRC,GADIX_SRC,xs1,ys1);

		ng.ng_LeftEdge		=xs1+RunTime->scx[79];					// Encoder Engine
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width			=RunTime->scx[15];
		ng.ng_TextAttr		=RunTime->SigFont;
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_ENGINEPOPUP;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_ENGINEPOPUP]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[95];
		ng.ng_Width			=RunTime->scx[203];
		ng.ng_TextAttr		=RunTime->ScrFont;
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_ENGINE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,FILENAME_MAX,GTST_String,instance->engine,TAG_DONE);
		modGadgets[GADIX_ENGINE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[79];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+12];
		ng.ng_Width			=RunTime->scx[219];
		ng.ng_GadgetText	="Parameter";
		ng.ng_GadgetID		=GADID_ENGINEPAR;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,FILENAME_MAX,GTST_String,instance->enginepar,TAG_DONE);
		modGadgets[GADIX_ENGINEPAR]=g;

		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+24];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Bitrate";
		ng.ng_GadgetID		=GADID_BITRATE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,BitrLabels,GTCY_Active,instance->bitrate,TAG_DONE);
		modGadgets[GADIX_BITRATE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[240];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="StereoStr";
		ng.ng_GadgetID		=GADID_STEREOSTR;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,40,GTST_String,instance->stereostr,TAG_DONE);
		modGadgets[GADIX_STEREOSTR]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[79];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+36];
		ng.ng_Width			=RunTime->scx[90];
		ng.ng_GadgetText	="Wave";
		ng.ng_GadgetID		=GADID_WAVETYPE;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,WaveLabels,GTCY_Active,instance->wavetype,TAG_DONE);
		modGadgets[GADIX_WAVETYPE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[240];
		ng.ng_Width			=RunTime->scx[60];
		ng.ng_GadgetText	="MonoStr";
		ng.ng_GadgetID		=GADID_MONOSTR;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,40,GTST_String,instance->monostr,TAG_DONE);
		modGadgets[GADIX_MONOSTR]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[79];
		ng.ng_TopEdge		=ys2+RunTime->scy[FRMCONTENT_YOFFSET+48];
		ng.ng_Width			=RunTime->scx[219];
		ng.ng_GadgetText	="Pipe";
		ng.ng_GadgetID		=GADID_PIPEDEV;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,PARBUFLEN,GTST_String,instance->pipedev,TAG_DONE);
		((struct StringInfo*)g->SpecialInfo)->Extension=RunTime->stringextend;
		modGadgets[GADIX_PIPEDEV]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			SetAPen(modrp,1);
			RText(modrp,"Src",xs1+RunTime->scx[29],ys1+RunTime->scy[FRMTEXT_YPOS(1)]);
			RText(modrp,"Encoder",xs1+RunTime->scx[71],ys2+RunTime->scy[FRMTEXT_YPOS(1)]);

			DrawTitle(modrp,"Source"        ,xs1,ys1,STDFRM_WIDTH);
			DrawTitle(modrp,"Settings"		,xs1,ys2,STDFRM_WIDTH);
			DrawTitle(modrp,"Presets"		,xs2,ys1,CFGSEL_WIDTH);

			xh=STDFRM_WIDTH;
			DrawGadWB(modrp,xs1,ys1,xh,SRCSEL_HEIGHT);                  /* Source */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(5));               /* Miscellaneous */
			DrawGadWB(modrp,xs1,ys3,xh,CMDFRM_HEIGHT);					/* Go */
			DrawGadWB(modrp,xs2,ys1,CFGSEL_WIDTH,CFGSEL_HEIGHT);		/* CfgSel */

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
								if(!HandleSource(&instance->src,modWin,modGadgets,GADID_SRC   ,GADIX_SRC   ,gnr,icode)
								&& !HandleCfgSel(&cfgsel      ,modWin,modGadgets,GADID_CFGSEL,GADIX_CFGSEL,gnr,icode,isec,imic)) {
									switch(gnr) {
										SFXMod_Interface_GadgetUpEvents
										case GADID_ENGINEPOPUP:
											if(instance->engine) {
												stcgfp(fn,instance->engine);MODFReqtags[6].ti_Data=(ULONG)fn;
												stcgfn(cn,instance->engine);MODFReqtags[7].ti_Data=(ULONG)cn;
											}
											else {
												MODFReqtags[6].ti_Data=(ULONG)PROGDIR"";
												MODFReqtags[7].ti_Data=(ULONG)0l;
											}
											MODFReqtags[0].ti_Data=(ULONG)"Select engine";		// Localize
											MODFReqtags[9].ti_Data=FALSE;
											if(AslRequest(modFRequest,MODFReqtags)) {
												strmfp(instance->engine,modFRequest->fr_Drawer,modFRequest->fr_File);
												GT_SetGadgetAttrs(modGadgets[GADIX_ENGINE],modWin,0l,GTST_String,instance->engine,TAG_DONE);
											}
											MODFReqtags[6].ti_Data=(ULONG)PROGDIR"_savers/"PRJ_NAME;
											MODFReqtags[7].ti_Data=(ULONG)"default.cfg";
											break;
										case GADID_ENGINE:
											strcpy(instance->engine,((struct StringInfo *)(modGadgets[GADIX_ENGINE]->SpecialInfo))->Buffer);
											break;
										case GADID_ENGINEPAR:
											strcpy(instance->enginepar,((struct StringInfo *)(modGadgets[GADIX_ENGINEPAR]->SpecialInfo))->Buffer);
											break;
										case GADID_MONOSTR:
											strcpy(instance->monostr,((struct StringInfo *)(modGadgets[GADIX_MONOSTR]->SpecialInfo))->Buffer);
											break;
										case GADID_STEREOSTR:
											strcpy(instance->stereostr,((struct StringInfo *)(modGadgets[GADIX_STEREOSTR]->SpecialInfo))->Buffer);
											break;
										case GADID_BITRATE:	instance->bitrate=icode;break;
										case GADID_WAVETYPE:instance->wavetype=icode;break;
										case GADID_PIPEDEV:
											strcpy(instance->pipedev,((struct StringInfo *)(modGadgets[GADIX_PIPEDEV]->SpecialInfo))->Buffer);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									SFXMod_Interface_VKeyEvents
									case KEY_TAB: ActivateGadget(modGadgets[GADIX_ENGINE],modWin,0l);break;
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
		DoneCfgSel(&cfgsel);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	SFXMod_Interface_NotifyDone
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	struct WaveFmt				wavefmt;
	FILE *outfile;
	LONG subtype;
	ULONG bufl,fmtl,size;
	char cmd[FILENAME_MAX<<2],ofn[FILENAME_MAX],*sptr,*dptr;
	char fullpath[256];
	BPTR lock;
	BPTR input;
	LONG srat=0 /*,lb,ub*/;
	//int qual;
	UWORD cnr;
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
	sdata.chmask=CH_ALL;

	if(si->channels>CH_STEREO) { Message(errWriteData,NULL,__FILE__,__LINE__);return(FALSE); }	// @TODO: LOCALIZE

	lock=Lock(PROGDIR"",ACCESS_READ);
	NameFromLock(lock,fullpath,255);
	UnLock(lock);

	//-- do only accept samplingrates close to the ones above
	//-- wasn't this only required for pegase ?
	if((si->srat>=30720) && (si->srat<=33280)) srat=32000;
	if((si->srat>=42336) && (si->srat<=45864)) srat=44100;
	if((si->srat>=46080) && (si->srat<=49920)) srat=48000;
	if(srat) {
		sptr=instance->enginepar;
		if(!strnicmp(instance->engine,PROGDIR"",8)) {
			cnr=sprintf(cmd,"%s/%s",fullpath,&instance->engine[8]);dptr=&cmd[cnr];
		}
		else {
			cnr=sprintf(cmd,"%s",instance->engine);dptr=&cmd[cnr];
		}
		if(lock=Lock(cmd,ACCESS_READ)) UnLock(lock);
		else {		// @TODO: LOCALIZE
			char msg[1000];
			// use ADosError(filename,messid,adoserror);
			sprintf(msg,"Failed to locate external encoder '%s'",cmd);
			Message(strNoID,msg,__FILE__,__LINE__);
			return(FALSE);
		}
		*(dptr++)=' ';
		while(*sptr) {
			//MSG1("*sptr=[%c]",*sptr);
			if(*sptr=='%') {
				sptr++;
				switch(*sptr) {
					case 'b':
						cnr=sprintf(dptr,"%s",BitrLabels[instance->bitrate]);dptr=&dptr[cnr];
						break;
					case 'c':
						switch(si->channels) {
							case CH_MONO:	cnr=sprintf(dptr,"%s",instance->monostr);dptr=&dptr[cnr];break;
							case CH_STEREO:	cnr=sprintf(dptr,"%s",instance->stereostr);dptr=&dptr[cnr];break;
						}
						break;
					case 'i':
						cnr=sprintf(dptr,"%s:sfx_mpeg",instance->pipedev);dptr=&dptr[cnr];
						break;
					case 'o':
						cnr=sprintf(dptr,"\"%s\"",pdata->fn);dptr=&dptr[cnr];
						break;
					case 'r':
						sptr++;
						switch(*sptr) {
							case '1':	/* kHz */
								if(srat==32000) { strcpy(dptr,"32");dptr=&dptr[2]; }
								if(srat==44100) { strcpy(dptr,"44.1");dptr=&dptr[4]; }
								if(srat==48000) { strcpy(dptr,"48");dptr=&dptr[2]; }
								break;
							case '0':	/* Hz */
							default:
								cnr=sprintf(dptr,"%ld",srat);dptr=&dptr[cnr];
								break;
						}
						break;
				}
				sptr++;
			}
			else *(dptr++)=*(sptr++);
		}
		*dptr='\0';
		/*
		switch(instance->engine)	{
			case ENG_8HZ:
				sprintf(cmd,"%s/_savers/mpeg_s_8Hz IN=%s:sfx_mpeg OUT=\"%s\" B=%s Q=%s %c %c",fullpath,instance->pipedev,pdata->fn,BitrLabels[instance->bitrate],instance->quality,(instance->copy?'C':' '),(instance->orig?'O':' '));
				break;
			case ENG_PEGASE:		// definitiv kein wav
				{
					char *orig[]={" ","ORIGINAL" };
					char *copy[]={" ","COPYRIGHT" };
					if(si->channels==CH_MONO) {
						sprintf(cmd,"%s/_savers/mpeg_s_Pegase FROM=%s:sfx_mpeg TO=\"%s\" MOTOROLA BR=%s FREQ=%ld MONO %s %s",fullpath,instance->pipedev,pdata->fn,BitrLabels[instance->bitrate],srat,copy[instance->copy],orig[instance->orig]);
					}
					if(si->channels==CH_STEREO) {
						sprintf(cmd,"%s/_savers/mpeg_s_Pegase FROM=%s:sfx_mpeg TO=\"%s\" MOTOROLA BR=%s FREQ=%ld JSTEREO %s %s",fullpath,instance->pipedev,pdata->fn,BitrLabels[instance->bitrate],srat,copy[instance->copy],orig[instance->orig]);
					}
				}
				break;
			case ENG_LAME:
				{
					char sratstr[5];
					qual=atoi(instance->quality);
					if(srat==32000) strcpy(sratstr,"32");
					if(srat==44100) strcpy(sratstr,"44.1");
					if(srat==48000) strcpy(sratstr,"48");
					// for wav files 
					//sprintf(cmd,"%s/_savers/mpeg_s_Lame -b %s %s %s %s %s:sfx_mpeg \"%s\"",fullpath,BitrLabels[instance->bitrate],(instance->copy?"-c":" "),(instance->orig?"-o":" "),(qual?"-h":" "),instance->pipedev,pdata->fn);
					// for cdda files
					if(si->channels==CH_MONO) {			// sampligrate !!!
						sprintf(cmd,"%s/_savers/mpeg_s_Lame -S -r -s %s -m m -b %s %s %s %s %s:sfx_mpeg \"%s\"",fullpath,sratstr,BitrLabels[instance->bitrate],(instance->copy?"-c":" "),(instance->orig?" ":"-o"),(qual?"-h":" "),instance->pipedev,pdata->fn);
					}
					else if(si->channels==CH_STEREO) {
						sprintf(cmd,"%s/_savers/mpeg_s_Lame -S -r -s %s -m j -b %s %s %s %s %s:sfx_mpeg \"%s\"",fullpath,sratstr,BitrLabels[instance->bitrate],(instance->copy?"-c":" "),(instance->orig?" ":"-o"),(qual?"-h":" "),instance->pipedev,pdata->fn);
					}
				}
				break;
		}
		*/
		MSG1("cmd=[%s]",cmd);
		input=Open("CON:0/15/800/200/SoundFX mp3/CLOSE//INACTIVE",MODE_OLDFILE);
		//input=Open("NIL:",MODE_OLDFILE);
		if(!SystemTags(cmd,SYS_Asynch,TRUE,SYS_Input,input,SYS_Output,NULL,NP_StackSize,100000,NP_Priority,-1,TAG_DONE)) {
			MSG("... started");
			sprintf(ofn,"%s:sfx_mpeg",instance->pipedev);
			switch(instance->wavetype)	{
				case WT_CDDA:
					MSG1("  writing CDDA(%s)",ofn);
					strncpy(sdata.fn,ofn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
					sdata.offs=0l;
					switch(si->channels) {
						case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM16,0,CHANNEL_MONO);break;
						case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM16,0,CHANNEL_STEREOI);break;
						case CH_QUADRO:	WriteData(si,&sdata,CRUNCH_PCM16,0,CHANNEL_STEREOI);break;
					}
					break;
				case WT_RIFF_WAV:
					MSG1("  writing RIFF_WAV(%s)",ofn);
					if(!(outfile=fopen(ofn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
					size=4L;												// subtype
					fmtl=sizeof(wavefmt);
					bufl=Samples2Bytes(sdata.savelength);
					wavefmt.wChannels		=LitEnd2BigEnd_uword(si->channels+1);
					wavefmt.dwSamplesPerSec	=LitEnd2BigEnd_ulong(si->srat);
					//switch(instance->format)	{
						//case FORMAT_WAV_PCM16:
							bufl=GetCrunchedLength(CRUNCH_PCM16,bufl);
							wavefmt.wFormatTag		=LitEnd2BigEnd_uword(WAVE_FORMAT_PCM);
							wavefmt.dwAvgBytesPerSec=GetCrunchedLength(CRUNCH_PCM16,(si->srat<<1)*(si->channels+1));
							wavefmt.dwAvgBytesPerSec=LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);		// because it's a macro
							wavefmt.wBlockAlign		=LitEnd2BigEnd_uword((si->channels+1)*2);
							wavefmt.wBitsPerSample	=LitEnd2BigEnd_uword(16);
							//break;
					//}
					size+=(long)(fmtl+8L);							// fmt_
					bufl*=(si->channels+1);
					size+=(bufl+8L);								// data

					WriteChunkHeader(WAV_RIFF,LitEnd2BigEnd_ulong(size),outfile);	// Grundstruktur schreiben
					subtype=WAV_WAVE;
					if(!fwrite(&subtype,sizeof(subtype),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

					WriteChunkHeader(WAV_fmt_,LitEnd2BigEnd_ulong(fmtl),outfile);
					wavefmt.wChannels		=LitEnd2BigEnd_uword(si->channels+1);
					wavefmt.dwSamplesPerSec	=LitEnd2BigEnd_ulong(si->srat);
					if(!fwrite(&wavefmt,sizeof(wavefmt),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					//switch(instance->format) {
						//case FORMAT_WAV_PCM16:
						//break;
					//}

					WriteChunkHeader(WAV_data,LitEnd2BigEnd_ulong(bufl),outfile);
					strncpy(sdata.fn,ofn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
					sdata.offs=ftell(outfile);
					fclose(outfile);
					//switch(instance->format)	{
						//case FORMAT_WAV_PCM16:
							switch(si->channels) {
								case CH_MONO:	WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
								case CH_STEREO:	WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
								case CH_QUADRO:	WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
							}
							//break;
					//}
					break;
			}
			MSG("... and done");

			//-- set file comment
			{
				UBYTE ch=si->channels;
				char sc_fmt[50];

				if(si->channels==CH_QUADRO) si->channels=CH_STEREO;
				sprintf(sc_fmt,"%s kbit",BitrLabels[instance->bitrate]);
				SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);
				si->channels=ch;
			}
			return(TRUE);
		}
		else { Message(strNoID,"Failed to run external encoder",NULL,0l);return(FALSE); }		// @TODO: LOCALIZE
	}
	else { Message(strNoID,"The sourcefile must have a samplingrate close to 32000, 44100 or 48000 Hz",NULL,0l);return(FALSE); }	// @TODO: LOCALIZE
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Engine"))		{ strcpy(instance->engine,RexxPar3);pf=1; }
		if(!pf && !stricmp(RexxPar2,"EnginePar"))	{ strcpy(instance->enginepar,RexxPar3);pf=1; }
		if(!pf && !stricmp(RexxPar2,"MonoStr"))		{ strcpy(instance->monostr,RexxPar3);pf=1; }
		if(!pf && !stricmp(RexxPar2,"StereoStr"))	{ strcpy(instance->stereostr,RexxPar3);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Bitrate"))		{ instance->bitrate=CfgIO_MatchString(RexxPar3,BitrLabels);pf=1; }
		if(!pf && !stricmp(RexxPar2,"PipeDev"))		{ strcpy(instance->pipedev,RexxPar3);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Engine"))		{ sprintf(ret,"%s",instance->engine);pf=1; }
		if(!pf && !stricmp(RexxPar2,"EnginePar"))	{ sprintf(ret,"%s",instance->enginepar);pf=1; }
		if(!pf && !stricmp(RexxPar2,"MonoStr"))		{ sprintf(ret,"%s",instance->monostr);pf=1; }
		if(!pf && !stricmp(RexxPar2,"StereoStr"))	{ sprintf(ret,"%s",instance->stereostr);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Bitrate"))		{ sprintf(ret,"%s",BitrLabels[instance->bitrate]);pf=1; }
		if(!pf && !stricmp(RexxPar2,"PipeDev"))		{ sprintf(ret,"%s",instance->pipedev);/*pf=1;*/ }
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
	strcpy(instance->engine,		CfgIO_ReadString		(cfg,"General"	,"Engine"		,PROGDIR"_savers/MPEG_S_8Hz"));
	strcpy(instance->enginepar,	CfgIO_ReadString		(cfg,"General"	,"EnginePar"	,"IN=%i OUT=%o B=%b %c"));
	strcpy(instance->monostr,	CfgIO_ReadString		(cfg,"General"	,"MonoStr"		,""));
	strcpy(instance->stereostr,	CfgIO_ReadString		(cfg,"General"	,"StereoStr"	,""));
	instance->bitrate=			CfgIO_MatchString(
								CfgIO_ReadString		(cfg,"General"	,"Bitrate"		,BitrLabels[BITRATE_MPEG_128]),BitrLabels);
	instance->wavetype=			CfgIO_MatchString(
								CfgIO_ReadString		(cfg,"General"	,"WaveType"		,WaveLabels[WT_RIFF_WAV]),WaveLabels);
	strcpy(instance->pipedev,	CfgIO_ReadString		(cfg,"General"	,"PipeDev"		,"pipe"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Engine"		,instance->engine);
	CfgIO_WriteString		  (cfg,"General"	,"EnginePar"	,instance->enginepar);
	CfgIO_WriteString		  (cfg,"General"	,"MonoStr"		,instance->monostr);
	CfgIO_WriteString		  (cfg,"General"	,"StereoStr"	,instance->stereostr);
	CfgIO_WriteString		  (cfg,"General"	,"WaveType"		,WaveLabels[instance->wavetype]);
	CfgIO_WriteString		  (cfg,"General"	,"Bitrate"		,BitrLabels[instance->bitrate]);
	CfgIO_WriteString		  (cfg,"General"	,"PipeDev"		,instance->pipedev);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	strcpy(instance->engine,PROGDIR"_savers/MPEG_S_8Hz");
	strcpy(instance->enginepar,"IN=%i OUT=%o B=%b %c");
	instance->monostr[0]='\0';
	instance->stereostr[0]='\0';
	instance->wavetype=WT_RIFF_WAV;
	instance->bitrate=BITRATE_MPEG_128;
	strcpy(instance->pipedev,"pipe");
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_ENGINE	],modWin,0l,GTST_String,instance->engine,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ENGINEPAR],modWin,0l,GTST_String,instance->enginepar,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_MONOSTR	],modWin,0l,GTST_String,instance->monostr,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_STEREOSTR],modWin,0l,GTST_String,instance->stereostr,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_WAVETYPE	],modWin,0l,GTST_String,instance->wavetype,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_BITRATE	],modWin,0l,GTCY_Active,instance->bitrate,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_PIPEDEV	],modWin,0l,GTST_String,instance->pipedev,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
