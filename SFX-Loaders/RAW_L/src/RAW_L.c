/****h* RAW/RAW_L.c [0.0] *
*
*  NAME
*    RAW_L.c
*  COPYRIGHT
*    $VER: RAW_L.c 0.0 (14.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    RAW loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    07.Sep.1998
*  MODIFICATION HISTORY
*    14.Apr.2004    V 0.0   most recent version
*    01.Mar.2004	V 4.3	most recent version
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*                           C : neue CheckRAWType Methode (uses average difs)
*    02.Aug.2000	V 4.0	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    02.Jun.1997	V 1.0	initial version
*  NOTES
*    * problematisch : schlecht ausgesteuerte Samples
*  IDEAS
*    * wenn die avgdifs ziemlich ähnlich sind
*      * problably clipped samples zählen,
*        wenn man 8bit samples als 16bit interpretiert
*        sind clipped samples recht unwarscheinlich
*    * histograph shapes
*      RAW        : 1001		-> 9
*      SRAW       : 0110		-> 6
*      ALAW/ULAW  : 0101/1010	-> 5/10
*    * ev auch ALAW/ULAW load simulieren
*
*******
*/

/****u* _loaders/RAW_L *
*  Function.eng
*    Loads RAW Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    A RAW sample realy isn't a format. It's 'raw' sound data. This is an advantage
*    in one way as it's easy to handle. The downside is that no other information
*    but the sample itself is saved (no loop points,bit resolution...).
*    __SFX at least trys to scans the sample for sign-type, bit-resolution and
*    endian-type (16 bit).<br>
*    As a new feature since version 3.70, you can program the RAW-loader by yourself.
*    If you work often with e.g. data from audio-cd's, then name those files "<name>.cdda".
*    To program the loader, you set all parameters in the left half of the RAW-loader :
*    <table border="0">
*    <tr><td>Type</td><td>=PCM16</td></tr>
*    <tr><td>Endian</td><td>=no</td></tr>
*    <tr><td>Sign</td><td>=signed</td></tr>
*    <tr><td>Channel</td><td>=mono/stereo interleaved</td></tr>
*    <tr><td>SRate</td><td>=44100</td></tr>
*    <tr><td>Offs</td><td>=0</td></tr>
*    <tr><td>Check File Type</td><td>Off</td></tr>
*    </table>
*    Save this as "cdda.cfg". No click on Add (on the right half) to create a new
*    type (thefore CheckFileType has to be enabled). Enter ".cdda" into
*    the field which contains "extension/header". Now click on that popup-symbol
*    and select the "cdda.cfg". Everytime a files end on ".cdda" the settings from
*    "cdda.cfg" are used now.
*    If you want to check the file contents and not the ending, use a "#" instead
*    of a "." as the first char (e.g. "#ALAW").
*  Funktion.ger
*    Läd RAW Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    RAW-Sample haben eigentlich gar kein Format. Hier werden nur die "rohen"
*    Sounddaten abgespeichert. Das hat den Vorteil, daß dieses Format sehr einfach
*    zu handhaben ist, aber auch den Nachteil, daß keinerlei zusätzliche Daten wie
*    Samplingrate, Loops, Bitauflösung usw. gespeichert werden können. __SFX
*    versucht daher zumindestens die Bitauflösung, den Vorzeichentyp und das
*    Endianformat (16 bit) zu erkennen.<br>
*    Als neues Feature ab V 3.70 können, sie den RAW-Loader nun selbst programmieren.
*    Wenn Sie also z.B. oft mit Daten von Audio-CD's arbeiten, nennen Sie diese Dateien
*    beispielsweise "<name>.cdda". Um den Loader zu programmieren, stellen Sie im
*    RAW-Loader auf der linken Seite alle Parameter wie folgt ein ein :
*    <table border="0">
*    <tr><td>Type</td><td>=PCM16</td></tr>
*    <tr><td>Endian</td><td>=no</td></tr>
*    <tr><td>Sign</td><td>=signed</td></tr>
*    <tr><td>Channel</td><td>=mono/stereo interleaved</td></tr>
*    <tr><td>SRate</td><td>=44100</td></tr>
*    <tr><td>Offs</td><td>=0</td></tr>
*    <tr><td>Check File Type</td><td>Aus</td></tr>
*    </table>
*    und speichern dies unter "cdda.cfg". Jetzt erstellen Sie mit Add einen
*    neuen Typ auf der rechten Seite (dazu muß CheckFileType an sein) und
*    tragen bei "extension/header" ".cdda" ein. Dann clicken Sie auf das
*    PopUp-Symbol und wählen die "cdda.cfg" aus. Jedes mal wenn eine Datei
*    auf ".cdda" endet, wird nun die "cdda.cfg" verwendet.<br>
*    Wenn Sie nicht die Dateiendung, sondern den Inhalt testen möchten, verwenden
*    Sie statt einem "." als erstes Zeichen ein "#" (Bsp. "#ALAW").
*  Parameter.eng
*    Type
*      type of compression
*        PCM8 : not compressed 8bit
*        PCM16 : not compressed 16bit
*        PCM24 : not compressed 24bit
*        PCM32 : not compressed 32bit
*        µ-Law : µ-Law (14:8) compressed 14bit
*        µ-Law Inv : µ-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)
*        A-Law : A-Law (14:8) compressed 14bit
*        A-Law Inv : A-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)
*    Endian
*      should SFX convert endians. Intel-proecessor based systems store 16 bit data inverted, this oprions fixes that.
*    Sign
*      load the Sample as a signed or unsigned sample.
*        signed : Amiga, Sgi
*        unsigned : Mac, Atari, PC
*    Channel
*      with how many channels is the sample stored and in which way.
*    SRate
*      which samplingrate should be used
*    Offs
*      how many bytes should be skiped in the begin (to skip a header of known length).
*    Check File Type
*      should SFX check the file extension and investigate the data statistically
*      to find out the format and adjust the loading parameter accordingly.
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        PCM24 : ungepackt 24bit
*        PCM32 : ungepackt 32bit
*        µ-Law : µ-Law (14:8) gepackt 14bit
*        µ-Law Inv : µ-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)
*        A-Law : A-Law (14:8) gepackt 14bit
*        A-Law Inv : A-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)
*    Endian
*      ob eine Endiankonvertierung durchgeführt werden soll. Intel-Prozessor basierte
*      Systeme speichern 16bit Wörter umgekehrt und diese Option  korregiert das.
*    Sign
*      ob das Sample als vorzeichenbehaftetes oder nicht-vorzeichenbehaftetes geladen werden soll
*        signed : Amiga, Sgi
*        unsigned : Mac, Atari, PC
*    Channel
*      wieviele Kanäle geladen werden sollen und wie sie aufgebaut sind.
*    SRate
*      welche Samplingrate soll eingetragen werden.
*    Offs
*      Wieviele Bytes am Anfang Übersrungen werden sollen (um einen Kopfblock bekannter Länge zu überspringen).
*    Check File Type
*      Ob der Loader die Fileextension überprüfen bzw. die Daten statistisch auf
*      ihr Format untersuchen und die Ladeparameter dementspechend anpassen soll.
*  Notes.eng
*    The offs parameter is <b>not</b> for seeking into the sample, although it can be used that way.
*    For 16bit sample you need to take care, that you only skip an even number of bytes then.
*  Hinweise.ger
*    Der off-Parameter dient <b>nicht</b> zum vorspringen im Sample, auch wenn das funktioniert.
*    Bei 16bit Samples muß man dann aber sicherstellen, das man nur eine gerade Anzahl an Bytes vorspringt.
*******
*/

#define RAW_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "RAW.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- Logging

//#define LOG_CRT	1

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

UBYTE CheckRAWType(char *fn,int len);
#ifdef LOG_CRT
void CheckRAWTypeLog(STRPTR filename,STRPTR fmt,...);
#endif

void LoadRDL(void);
void SaveRDL(void);
void FreeRDL(void);

struct rawdetect *AddRDN(void);
void RemRDN(struct rawdetect *rd);

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
#define GADID_RATE		(GADID_CHANNEL+1)
#define GADIX_RATE		(GADIX_CHANNEL+1)
#define GADID_OFFS		(GADID_RATE+1)
#define GADIX_OFFS		(GADIX_RATE+1)
#define GADID_CHKTYPE	(GADID_OFFS+1)
#define GADIX_CHKTYPE	(GADIX_OFFS+1)

#define GADID_RDLIST	(GADID_CHKTYPE+1)
#define GADIX_RDLIST	(GADIX_CHKTYPE+1)
#define GADID_DETECT	(GADID_RDLIST+1)
#define GADIX_DETECT	(GADIX_RDLIST+1)
#define GADID_CFGPOPUP	(GADID_DETECT+1)
#define GADIX_CFGPOPUP	(GADIX_DETECT+1)
#define GADID_CFG		(GADID_CFGPOPUP+1)
#define GADIX_CFG		(GADIX_CFGPOPUP+1)
#define GADID_RDADD		(GADID_CFG+1)
#define GADIX_RDADD		(GADIX_CFG+1)
#define GADID_RDDEL		(GADID_RDADD+1)
#define GADIX_RDDEL		(GADIX_RDADD+1)

#define LAST_GADIX		(GADIX_RDDEL+1)


//-- Autodetecting RAW-Types

#define UNKNOWN	0
#define RAW8	1
#define SRAW8	2
#define RAW16	3
#define SRAW16	4
#define RAW16E	5
#define SRAW16E	6
#define ULAW	7
#define ALAW	8

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	UBYTE format;			/* bits && compression */
	UBYTE sign;				/* 0=unsigned,1=signed */
	UBYTE endian;			/* 0=no,1=yes(covert) */
	UBYTE channel;			/* 0 mono,1 stereo,2 quadro */
	ULONG rate;				/* sampling rate */
	ULONG offs;				/* header skip */
	UBYTE chktype;			/* check type */
};

struct rawdetect {
	struct Node node;
	char detect[256];		// detect string
//	char *prefs;			// its node.ln_Name;
};

struct List	rawdetectlist;

char *FormLabels[]={STR_PCM8,STR_PCM16,STR_PCM24,STR_PCM32,STR_ULAW,STR_ULAW_INV,STR_ALAW,STR_ALAW_INV,NULL};
char *SignLabels[]={"signed","unsigned",NULL};
char *EndiLabels[]={"Motorola","Intel",NULL};
char *ChanLabels[]={"mono","stereo","st. interleav.","quadro","qu. interleav.",NULL};

UBYTE ConvFlags []={
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

#ifdef LOG_CRT
	UBYTE *crt_names[]= {"UNKNOWN","RAW8","SRAW8","RAW16","SRAW16","RAW16E","SRAW16E","ULAW","ALAW" };
#endif

//-- Library-Basisaddrs.

struct Library				*ListToolsBase=NULL;

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(ListToolsBase	=OpenLibrary(ListToolsName,ListToolsVersion)))					{ ADosError(ListToolsName" >=V"NUM2STR(ListToolsVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; }
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	NewList(&rawdetectlist);
	LoadRDL();
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	FreeRDL();
	if(cfg)				CfgIO_RemConfig(cfg);
	if(ListToolsBase)	CloseLibrary(ListToolsBase);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

SFXMod_DoClientDataInit
SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables

	UBYTE rdlchanged=FALSE,rdaktentry;
	struct rawdetect *rd;
	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(7);
	UWORD winh=ys2+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD xs2=xs1+1+GetRunTime(scx[175]);
	UWORD winw=xs2+3+GetRunTime(scx[150]);
	UWORD xh;

	SFXMod_OpenGUILibs;
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[42]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	=GetString(btnLoad);
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[182]);
		ng.ng_GadgetText	=GetString(menPrefsUse);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[60]);					// Options
		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[110]);
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_TYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_TYPE]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+12]);
		ng.ng_GadgetText	="Endian";
		ng.ng_GadgetID		=GADID_ENDIAN;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,EndiLabels,GTCY_Active,instance->endian,GA_Disabled,((ConvFlags[instance->format]&CONV_ENDIAN_F)?FALSE:TRUE),TAG_DONE);
		modGadgets[GADIX_ENDIAN]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+24]);
		ng.ng_GadgetText	="Sign";
		ng.ng_GadgetID		=GADID_SIGN;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,SignLabels,GTCY_Active,instance->sign,GA_Disabled,((ConvFlags[instance->format]&CONV_SIGNUM_F)?FALSE:TRUE),TAG_DONE);
		modGadgets[GADIX_SIGN]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+36]);
		ng.ng_GadgetText	="Channel";
		ng.ng_GadgetID		=GADID_CHANNEL;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,ChanLabels,GTCY_Active,instance->channel,TAG_DONE);
		modGadgets[GADIX_CHANNEL]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+48]);
		ng.ng_GadgetText	="SRate";
		ng.ng_GadgetID		=GADID_RATE;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,10,GTIN_Number,instance->rate,TAG_DONE);
		modGadgets[GADIX_RATE]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+60]);
		ng.ng_GadgetText	="Offs";
		ng.ng_GadgetID		=GADID_OFFS;
		g=CreateGadget(INTEGER_KIND,g,&ng,GTIN_MaxChars,10,GTIN_Number,instance->offs,TAG_DONE);
		modGadgets[GADIX_OFFS]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[144]);
		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+72]);
		ng.ng_Width			=GetRunTime(scx[26]);
		ng.ng_GadgetText	="Check File Type";
		ng.ng_GadgetID		=GADID_CHKTYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->chktype,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_CHKTYPE]=g;

		ng.ng_LeftEdge		=xs2+GetRunTime(scx[5]);					// Detect List
		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[140]);
		ng.ng_Height		=GetRunTime(scy[48]);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_RDLIST;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,&rawdetectlist,GTLV_Selected,0,GTLV_Top,0,GTLV_ShowSelected,0,TAG_DONE);
		modGadgets[GADIX_RDLIST]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+48]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_DETECT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,64,GTST_String,NULL,TAG_DONE);
		modGadgets[GADIX_DETECT]=g;

		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+60]);
		ng.ng_Width			=GetRunTime(scx[15]);
		ng.ng_TextAttr		=GetRunTime(SigFont);
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_CFGPOPUP;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_CFGPOPUP]=g;

		ng.ng_LeftEdge		=xs2+GetRunTime(scx[21]);
		ng.ng_Width			=GetRunTime(scx[124]);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_CFG;
		g=CreateGadget(TEXT_KIND,g,&ng,GTTX_Border,TRUE,GTTX_Text,NULL,TAG_DONE);
		modGadgets[GADIX_CFG]=g;

		ng.ng_LeftEdge		=xs2+GetRunTime(scx[5]);
		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET+72]);
		ng.ng_Width			=GetRunTime(scx[70]);
		ng.ng_GadgetText	="_Add";
		ng.ng_GadgetID		=GADID_RDADD;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		modGadgets[GADIX_RDADD]=g;

		ng.ng_LeftEdge		=xs2+GetRunTime(scx[75]);
		ng.ng_GadgetText	="_Del";
		ng.ng_GadgetID		=GADID_RDDEL;
		g=CreateGadget(BUTTON_KIND,g,&ng,GT_Underscore,'_',TAG_DONE);
		modGadgets[GADIX_RDDEL]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			DrawTitle(modrp,"Options"			,xs1,ys1,GetRunTime(scx[175]));
			DrawTitle(modrp,"Autodetect"		,xs2,ys1,GetRunTime(scx[150]));

			DrawGadWB(modrp,xs1,ys1,GetRunTime(scx[175]),ANYFRM_HEIGHT(7));		/* Par */
			DrawGadWB(modrp,xs2,ys1,GetRunTime(scx[150]),ANYFRM_HEIGHT(7));		/* Detect */
			xh=1+GetRunTime(scx[175+150]);
			DrawGadWB(modrp,xs1,ys2,xh,CMDFRM_HEIGHT);							/* Go */

			RefreshGUI(instance,modGadgets,modWin);
			rdaktentry=0;

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					SFXMod_Interface_AnalyseIMsg
					switch(iclass) {
						case IDCMP_ACTIVEWINDOW:
							WindowToFront(modWin);
							break;
						case IDCMP_GADGETUP:
							switch((int)(((struct Gadget *)iadr)->GadgetID))
							{
								case GADID_GO:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
									ret=TRUE;runing=FALSE;
									break;
								case GADID_OKAY:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
									runing=FALSE;
									break;
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
								case GADID_RATE:
									CheckGad_int(modGadgets[GADIX_RATE],modWin,(LONG *)(&instance->rate),1,2,MAXLONGP);
									break;
								case GADID_OFFS:
									CheckGad_int(modGadgets[GADIX_OFFS],modWin,(LONG *)(&instance->offs),1,0,MAXLONGP);
									break;
								case GADID_CHKTYPE:
									instance->chktype=modGadgets[GADIX_CHKTYPE]->Flags&GFLG_SELECTED;
									RefreshGUI(instance,modGadgets,modWin);
									break;
								case GADID_RDLIST:
									rdaktentry=icode;
									rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,rdaktentry);
									GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,FALSE,GTLV_Selected,rdaktentry,GTLV_Top,rdaktentry,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,FALSE,GTST_String,rd->detect,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,FALSE,GTTX_Text,rd->node.ln_Name,TAG_DONE);
									break;
								case GADID_DETECT:
									rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,rdaktentry);
									strcpy(rd->detect,((struct StringInfo *)(modGadgets[GADIX_DETECT]->SpecialInfo))->Buffer);
									rdlchanged=TRUE;
									break;
								case GADID_CFGPOPUP:
									MODFReqtags[9].ti_Data=FALSE;
									if(AslRequest(modFRequest,MODFReqtags))
									{
										rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,rdaktentry);
										strcpy(rd->node.ln_Name,modFRequest->fr_File);
										GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,FALSE,GTLV_Labels,&rawdetectlist,GTLV_Selected,rdaktentry,GTLV_Top,rdaktentry,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,FALSE,GTTX_Text,rd->node.ln_Name,TAG_DONE);
									}
									break;
								case GADID_RDADD:
									if(rd=AddRDN())
									{
										rdaktentry=0;
										GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,FALSE,GTLV_Labels,&rawdetectlist,GTLV_Selected,rdaktentry,GTLV_Top,rdaktentry,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,FALSE,GTST_String,rd->detect,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFGPOPUP],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,FALSE,GTTX_Text,rd->node.ln_Name,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_RDDEL   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
										rdlchanged=TRUE;
									}
									break;
								case GADID_RDDEL:
									rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,rdaktentry);
									RemRDN(rd);
									rdaktentry=0;
									if((rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,0)))
									{
										GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,FALSE,GTLV_Labels,&rawdetectlist,GTLV_Selected,rdaktentry,GTLV_Top,rdaktentry,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,FALSE,GTST_String,rd->detect,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,FALSE,GTTX_Text,rd->node.ln_Name,TAG_DONE);
									}
									else
									{
										GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,TRUE,GTST_String,NULL,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFGPOPUP],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,TRUE,GTTX_Text,NULL,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_RDADD   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_RDDEL   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
									}
									rdlchanged=TRUE;
									break;
							}
							break;
						case IDCMP_VANILLAKEY:
							switch(icode)
							{
								case KEY_ENTER:
								case KEY_RETURN:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
									ret=TRUE;runing=FALSE;
									break;
								case KEY_ESC:
									runing=FALSE;
									break;
								case KEY_TAB:
									ActivateGadget(modGadgets[GADIX_RATE],modWin,0);
									break;
								case '?':
									Message(strNoID,PRJ_VERS,NULL,0l);
									break;
							}
							break;
						case IDCMP_CLOSEWINDOW:
							runing=FALSE;
							break;
						case IDCMP_RAWKEY:
							if(icode==RKEY_HELP) ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html");
							break;
						case IDCMP_MENUPICK:
							while(icode!=MENUNULL)
							{
								if(item=ItemAddress(GetRunTime(FXMenu),icode))
								{
									switch((ULONG)MENU_USERDATA(item))
									{
										case MENID_LOAD:
											MODFReqtags[9].ti_Data=FALSE;
											if(AslRequest(modFRequest,MODFReqtags))
											{
												strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
												LoadSettings(instance,fn);
												RefreshGUI(instance,modGadgets,modWin);
											}
											break;
										case MENID_SAVE:
											MODFReqtags[9].ti_Data=TRUE;
											if(AslRequest(modFRequest,MODFReqtags))
											{
												strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
												SaveSettings(instance,fn);
											}
											break;
										case MENID_RESET:
											RecallLastSettings(instance);
											RefreshGUI(instance,modGadgets,modWin);
											break;
										case MENID_DEFAULT:
											RecallDefaultSettings(instance);
											RefreshGUI(instance,modGadgets,modWin);
											break;
										case MENID_GO:
											SaveSettings(instance,ENVDIR"loaders/RAW.cfg");		/* Store actual settings */
											ret=TRUE;runing=FALSE;
											break;
										case MENID_HELP:
											ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html");
											break;
										case MENID_ABOUT:
											Message(strNoID,PRJ_VERS,NULL,0l);
											break;
										case MENID_QUIT:
											runing=FALSE;
											break;
									}
									icode=item->NextSelect;
								}
							}
							break;
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
	if(rdlchanged) SaveRDL();
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	ULONG bufl;
	LoadData ldata;
	char ext[FESIZE],hdr[64];		// for rawdetect
	FILE *infile;
	char prefsfn[256];
	struct rawdetect *rd=(struct rawdetect *)rawdetectlist.lh_Head;
	UBYTE done=FALSE;
	UBYTE channel;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;
	
	if((bufl=GetFileLength(pdata->fn))>0) {
		if(instance->chktype) {		// trying to detect the type
			strsfn(pdata->fn,NULL,NULL,NULL,ext);
			if((infile=fopen(pdata->fn,"rb"))) {
				fread(hdr,64,1,infile);
				fclose(infile);
			}
			while(!EndOfList(&rawdetectlist,(struct Node *)rd) && !done) {	// testing all possibillities
				switch(rd->detect[0]) {
					case '.': if(!stricmp(ext,&rd->detect[1])) done=TRUE;break;
					case '#': if(!strnicmp(hdr,&rd->detect[1],strlen(&rd->detect[1]))) done=TRUE;break;
				}
				if(done) {
					//MSG1("RAW_TypeDetect = %s",rd->node.ln_Name);
					sprintf(prefsfn,PROGDIR"_loaders/"PRJ_NAME"/%s",rd->node.ln_Name);LoadSettings(instance,prefsfn);
				}
				rd=(struct rawdetect *)rd->node.ln_Succ;
			}
			if(!done) {
				switch(CheckRAWType(pdata->fn,bufl)) {
					case RAW8:		instance->format=FORMAT_RAW_PCM8; instance->sign=1;instance->endian=0;break;
					case SRAW8: 	instance->format=FORMAT_RAW_PCM8; instance->sign=0;instance->endian=0;break;
					case RAW16:		instance->format=FORMAT_RAW_PCM16;instance->sign=1;instance->endian=0;break;
					case SRAW16:	instance->format=FORMAT_RAW_PCM16;instance->sign=0;instance->endian=0;break;
					case RAW16E:	instance->format=FORMAT_RAW_PCM16;instance->sign=1;instance->endian=1;break;
					case SRAW16E:	instance->format=FORMAT_RAW_PCM16;instance->sign=0;instance->endian=1;break;
					case ULAW:		instance->format=FORMAT_RAW_ULAW; instance->sign=0;instance->endian=0;break;
					case ALAW:		instance->format=FORMAT_RAW_ALAW; instance->sign=0;instance->endian=0;break;
				}
			}
		}
		bufl-=instance->offs;
		channel=ChanFlags[instance->channel];
		switch(channel) {
			case CHANNEL_MONO:
				SET_CHANNELS(si,CH_MONO);break;
			case CHANNEL_STEREO:
			case CHANNEL_STEREOI:
				SET_CHANNELS(si,CH_STEREO);break;
			case CHANNEL_QUADRO:
			case CHANNEL_QUADROI:
				SET_CHANNELS(si,CH_QUADRO);break;
		}
		si->srat=instance->rate;
		strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]='\0';
		ldata.offs=instance->offs;
		ldata.chmask=CH_ALL;
		ldata.flags=LDF_NONE;
		switch(instance->format) {
			case FORMAT_RAW_PCM8:	  sc_fmt=STR_PCM8    ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_PCM8    ,CONV(0,instance->sign),channel);break;
			case FORMAT_RAW_PCM16:	  sc_fmt=STR_PCM16   ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_PCM16   ,CONV(instance->endian,instance->sign),channel);break;
			case FORMAT_RAW_PCM24:	  sc_fmt=STR_PCM24   ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_PCM24   ,CONV(instance->endian,instance->sign),channel);break;
			case FORMAT_RAW_PCM32:	  sc_fmt=STR_PCM32   ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_PCM32   ,CONV(instance->endian,instance->sign),channel);break;
			case FORMAT_RAW_ULAW:	  sc_fmt=STR_ULAW    ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_ULAW    ,CONV(0,0),channel);break;
			case FORMAT_RAW_ULAW_INV: sc_fmt=STR_ULAW_INV;si->slen=ReadData(si,&ldata,bufl,CRUNCH_ULAW_INV,CONV(0,0),channel);break;
			case FORMAT_RAW_ALAW:	  sc_fmt=STR_ALAW    ;si->slen=ReadData(si,&ldata,bufl,CRUNCH_ALAW    ,CONV(0,0),channel);break;
			case FORMAT_RAW_ALAW_INV: sc_fmt=STR_ALAW_INV;si->slen=ReadData(si,&ldata,bufl,CRUNCH_ALAW_INV,CONV(0,0),channel);break;
		}
		if(!si->slen) return(FALSE);
		si->zoomxl=si->slen;
	}
	else { StdCError(pdata->fn,errAccessFile,errno,__FILE__,__LINE__);return(FALSE); }

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	MSG2(__FUNC__" : func=%s param=%s",RexxPar0,RexxPar2);

	if(!stricmp(RexxPar0,"SFX_SetLoaderParam")) {
		if(       !stricmp(RexxPar2,"Format"))		{ instance->format =CfgIO_MatchString(RexxPar3,FormLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Sign"))		{ instance->sign   =CfgIO_MatchString(RexxPar3,SignLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Endian"))		{ instance->endian =CfgIO_MatchString(RexxPar3,EndiLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Channel"))		{ instance->channel=CfgIO_MatchString(RexxPar3,ChanLabels);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ instance->rate   =atoi(RexxPar3);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Offset"))		{ instance->offs   =atoi(RexxPar3);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"CheckType"))	{ instance->chktype=atoi(RexxPar3)&0x1;pf=TRUE; }
		if(pf) SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetLoaderParam")) {
		if(       !stricmp(RexxPar2,"Format"))		{ sprintf(ret,"%s",FormLabels[instance->format ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Sign"))		{ sprintf(ret,"%s",SignLabels[instance->sign   ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Endian"))		{ sprintf(ret,"%s",EndiLabels[instance->endian ]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Channel"))		{ sprintf(ret,"%s",ChanLabels[instance->channel]);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Rate"))		{ sprintf(ret,"%d",instance->rate);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"Offset"))		{ sprintf(ret,"%d",instance->offs);pf=TRUE; }
		if(!pf && !stricmp(RexxPar2,"CheckType"))	{ sprintf(ret,"%d",instance->chktype);/*pf=TRUE;*/ }
	}
	if(!cf && !stricmp(RexxPar0,"SFX_SetLoaderPreset")) {
		if(!stricmp(RexxPar2,"default"))  { RecallDefaultSettings(instance); }
		else { sprintf(fn,PROGDIR"_loaders/"PRJ_NAME"/%s.cfg",RexxPar2);LoadSettings(instance,fn); }
		SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");
	}
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	instance->format =CfgIO_MatchString(
					  CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_RAW_PCM8]),FormLabels);
	instance->sign	 =CfgIO_MatchString(
					  CfgIO_ReadString			(cfg,"General"	,"Sign"			,SignLabels[0]),SignLabels);
	instance->endian =CfgIO_MatchString(
					  CfgIO_ReadString			(cfg,"General"	,"Endian"		,EndiLabels[0]),EndiLabels);
	instance->channel=CfgIO_MatchString(
					  CfgIO_ReadString			(cfg,"General"	,"Channel"		,ChanLabels[CHANNEL_MONO]),ChanLabels);
	instance->rate	 =CfgIO_ReadUnsignedInteger	(cfg,"General"	,"Rate"			,16780);
	instance->offs	 =CfgIO_ReadUnsignedInteger	(cfg,"General"	,"Offset"		,0);
	instance->chktype=CfgIO_ReadBool			(cfg,"General"	,"CheckType"	,TRUE);
}

void SaveSettings(struct Instance *instance,char *fname) {
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);
	CfgIO_WriteString		  (cfg,"General"	,"Sign"			,SignLabels[instance->sign]);
	CfgIO_WriteString		  (cfg,"General"	,"Endian"		,EndiLabels[instance->endian]);
	CfgIO_WriteString		  (cfg,"General"	,"Channel"		,ChanLabels[instance->channel]);
	CfgIO_WriteUnsignedInteger(cfg,"General"	,"Rate"			,instance->rate);
	CfgIO_WriteUnsignedInteger(cfg,"General"	,"Offset"		,instance->offs);
	CfgIO_WriteBool			  (cfg,"General"	,"CheckType"	,instance->chktype);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->format=FORMAT_RAW_PCM8;
	instance->sign=0;			/* signed */
	instance->endian=0;			/* no */
	instance->channel=CHANNEL_MONO;
	instance->rate=16780;		/* default protracker rate */
	instance->offs=0;			/* no skip */
	instance->chktype=TRUE;		/* check filename extension */
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	struct rawdetect *rd;

	GT_SetGadgetAttrs(modGadgets[GADIX_TYPE   ],modWin,NULL,GTCY_Active ,instance->format,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_SIGN   ],modWin,NULL,GTCY_Active,instance->sign  ,GA_Disabled,((ConvFlags[instance->format]&CONV_SIGNUM_F)?FALSE:TRUE),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_ENDIAN ],modWin,NULL,GTCY_Active,instance->endian,GA_Disabled,((ConvFlags[instance->format]&CONV_ENDIAN_F)?FALSE:TRUE),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_CHANNEL],modWin,NULL,GTCY_Active ,instance->channel,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_RATE   ],modWin,NULL,GTIN_Number ,instance->rate,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_OFFS   ],modWin,NULL,GTIN_Number ,instance->offs,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_CHKTYPE],modWin,NULL,GTCB_Checked,instance->chktype,TAG_DONE);
	if(instance->chktype) {
		if(rd=(struct rawdetect *)GetNodeByIndex(&rawdetectlist,0)) {
			GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,FALSE,GTLV_Selected,0,GTLV_Top,0,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,FALSE,GTST_String,rd->detect,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_CFGPOPUP],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,FALSE,GTTX_Text,rd->node.ln_Name,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_RDADD   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_RDDEL   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
		}
		else {
			GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,TRUE,GTST_String,NULL,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_CFGPOPUP],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,TRUE,GTTX_Text,NULL,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_RDADD   ],modWin,NULL,GA_Disabled,FALSE,TAG_DONE);
			GT_SetGadgetAttrs(modGadgets[GADIX_RDDEL   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		}
	}
	else {
		GT_SetGadgetAttrs(modGadgets[GADIX_RDLIST  ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(modGadgets[GADIX_DETECT  ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(modGadgets[GADIX_CFGPOPUP],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(modGadgets[GADIX_CFG     ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(modGadgets[GADIX_RDADD   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
		GT_SetGadgetAttrs(modGadgets[GADIX_RDDEL   ],modWin,NULL,GA_Disabled,TRUE,TAG_DONE);
	}
}

//-- private Stuff

UBYTE CheckRAWType(char *fn,int len) {
	FILE *infile;
	register int i;
	UBYTE *buf,*ptr;
	UBYTE manr8=0,minr8=0,manr16=0,minr16=0,manr16e=0,minr16e=0;
	WORD bins8[4]={0},mas8=0,mis8=SMP_MAXP;
	WORD bino8[4]={0},mao8=0,mio8=SMP_MAXP;
	WORD bine8[4]={0},mae8=0,mie8=SMP_MAXP;
	WORD bins16[4]={0},mas16=0,mis16=SMP_MAXP;
	WORD bins16e[4]={0},mas16e=0,mis16e=SMP_MAXP;
	UBYTE bin8,bin16,bin16e;
	double dist8,dist8o,dist8e;
#ifdef LOG_CRT
	double ant8,ver8,ver8s,ant16,ver16,ver16s,ant16e,ver16e,ver16se;
	double antr16,antr16e;//	,verr16,verr16e;
#endif
	WORD fi8,fi8s,fi16,fi16s,fi16e,fi16se;
	double dif8,dif8s,dif16,dif16s,dif16e,dif16se;
	ULONG mindif;
	double div1,div2,fc;
	UWORD cv, lv, lve;
	ULONG cd;
	WORD  cvs,lvs,lvse;
	LONG  cds;
	UBYTE ret=UNKNOWN;
#ifdef LOG_CRT
	char logbuf[500],fnnode[FILENAME_MAX];
#endif

	len=min(len,5000);			/* maximal die ersten 5000 Bytes lesen */

	if(infile=fopen(fn,"rb")) {
		if(buf=(UBYTE *)AllocVec(len,MEMF_ANY)) {
			if(!(fread(buf,len,1,infile))) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(ret); }
			//-- histographs
			ptr=buf;									// histograph for 8 bit
			for(i=0;i<len;i++) bins8[*(ptr++)>>6]++;
			ptr=buf;									// histograph for 8 bit odd/even
			for(i=0;i<(len>>1);i++) {
				bino8[*(ptr++)>>6]++;
				bine8[*(ptr++)>>6]++;
			}
			ptr=buf;									// histograph for 16 bit
			for(i=0;i<(len>>1);i++) {
				bins16 [*(ptr++)>>6]++;						// motorola
				bins16e[*(ptr++)>>6]++;						// intel (endian swap)
			}
			//-- avg deltas
			dif8=dif8s=dif16=dif16s=dif16e=dif16se=0.0;
			div1=div2=1.0;								// avg delta for 8 bit
			ptr=buf;lv=(SMP_MAXP+1);lvs=0;
			for(i=0;i<len;i++) {
				fc=(div1/div2);
				cv=((UWORD)(*ptr))<<8;
				if(lv>cv) cd=lv-cv;
				else      cd=cv-lv;
				dif8=(dif8+(cd/div1))*fc;						/* avg (without overflow) */
				lv=cv;
				cvs=((WORD)*((BYTE *)ptr))<<8;
				if(lvs>cvs) cds=lvs-cvs;
				else        cds=cvs-lvs;
				dif8s=(dif8s+(cds/div1))*fc;					/* avg (without overflow) */
				lvs=cvs;
				div1=div2;div2+=1.0;ptr++;
			}
			div1=div2=1.0;								// avg delta for 16 bit
			ptr=buf;lv=(SMP_MAXP+1);lvs=0;lve=(SMP_MAXP+1);lvse=0;
			for(i=0;i<(len>>1);i++) {
				fc=(div1/div2);
				cv=(((UWORD)(*ptr))<<8)|ptr[1];
				if(lv>cv) cd=lv-cv;
				else      cd=cv-lv;
				dif16=(dif16+(cd/div1))*fc;						/* avg (without overflow) */
				lv=cv;
				cv=(((UWORD)ptr[1])<<8)|*ptr;
				if(lve>cv) cd=lve-cv;
				else       cd=cv-lve;
				dif16e=(dif16e+(cd/div1))*fc;						/* avg (without overflow) */
				lve=cv;
				cvs=(((WORD)*((BYTE *)ptr))<<8)|ptr[1];
				if(lvs>cvs) cds=lvs-cvs;
				else        cds=cvs-lvs;
				dif16s=(dif16s+(cds/div1))*fc;						/* avg (without overflow) */
				lvs=cvs;
				cvs=(((WORD)((BYTE)ptr[1]))<<8)|*ptr;
				if(lvse>cvs) cds=lvse-cvs;
				else         cds=cvs-lvse;
				dif16se=(dif16se+(cds/div1))*fc;					/* avg (without overflow) */
				lvse=cvs;
				div1=div2;div2+=1.0;ptr+=2;
			}

			//-- first values
			fi8=((UWORD)buf[0]<<8);fi8s=fi8+SMP_MAXN;
			fi16=((UWORD)buf[1]<<8)|buf[0];fi16s=fi16+SMP_MAXN;
			fi16e=((UWORD)buf[0]<<8)|buf[1];fi16se=fi16e+SMP_MAXN;
			FreeVec((LONG *)buf);
		}
		else Message(errAllocateMemory,NULL,__FILE__,__LINE__);
		fclose(infile);
		for(i=0;i<4;i++) {
			if(bins8  [i]>mas8)   { manr8  =i;mas8  =bins8[i]; }	// min,max for 8 bit
			if(bins8  [i]<mis8)   { minr8  =i;mis8  =bins8[i]; }
			if(bino8  [i]>mao8)   { mao8  =bino8[i]; }	// min,max for 8 bit odd
			if(bino8  [i]<mio8)   { mio8  =bino8[i]; }
			if(bine8  [i]>mae8)   { mae8  =bine8[i]; }	// min,max for 8 bit even
			if(bine8  [i]<mie8)   { mie8  =bine8[i]; }
			if(bins16 [i]>mas16)  { manr16 =i;mas16 =bins16[i]; }	// min,max for 16 bit
			if(bins16 [i]<mis16)  { minr16 =i;mis16 =bins16[i]; }
			if(bins16e[i]>mas16e) { manr16e=i;mas16e=bins16e[i]; }	// min,max for 16 bit endian
			if(bins16e[i]<mis16e) { minr16e=i;mis16e=bins16e[i]; }
		}
		dist8 =(double)(mas8-mis8)/ len;
		dist8o=(double)(mao8-mio8)/(len>>1);
		dist8e=(double)(mae8-mie8)/(len>>1);

#ifdef LOG_CRT
		if(mis8!=0) ant8=(double)mas8/(double)mis8;
		else ant8=999.0;
		div1=(double)(bins8[1]+bins8[2]);div2=(double)(bins8[0]+bins8[3]);
		if(div1>div2)
			if(div2!=0) ver8=div1/div2;
			else ver8=999.0;
		else
			if(div1!=0) ver8=div2/div1;
			else ver8=999.0;
		div1=(double)(bins8[0]+bins8[2]);div2=(double)(bins8[1]+bins8[3]);
		if(div1>div2)
			if(div2!=0) ver8s=div1/div2;
			else ver8s=999.0;
		else
			if(div1!=0) ver8s=div2/div1;
			else ver8s=999.0;

		if(mis16!=0) ant16=(double)mas16/(double)mis16;
		else ant16=999.0;
		div1=(double)(bins16[1]+bins16[2]);div2=(double)(bins16[0]+bins16[3]);
		if(div1>div2)
			if(div2!=0) ver16=div1/div2;
			else ver16=999.0;
		else
			if(div1!=0) ver16=div2/div1;
			else ver16=999.0;
		div1=(double)(bins16[0]+bins16[2]);div2=(double)(bins16[1]+bins16[3]);
		if(div1>div2)
			if(div2!=0) ver16s=div1/div2;
			else ver16s=999.0;
		else
			if(div1!=0) ver16s=div2/div1;
			else ver16s=999.0;
//		verr16=(ver16*100.0)/ver8;
		antr16=(ant16*100.0)/ant8;

		if(mis16e!=0) ant16e=(double)mas16e/(double)mis16e;
		else ant16e=999.0;
		div1=(double)(bins16e[1]+bins16e[2]);div2=(double)(bins16e[0]+bins16e[3]);
		if(div1>div2)
			if(div2!=0) ver16e=div1/div2;
			else ver16e=999.0;
		else
			if(div1!=0) ver16e=div2/div1;
			else ver16e=999.0;
		div1=(double)(bins16e[0]+bins16e[2]);div2=(double)(bins16e[1]+bins16e[3]);
		if(div1>div2)
			if(div2!=0) ver16se=div1/div2;
			else ver16se=999.0;
		else
			if(div1!=0) ver16se=div2/div1;
			else ver16se=999.0;
		//verr16e=(ver16e*100.0)/ver8;
		antr16e=(ant16e*100.0)/ant8;
#endif
		bin8=bin16=bin16e=0;
		if(bins8[0]>bins8[1]) bin8|=8;
		else bin8|=4;
		if(bins8[2]>bins8[3]) bin8|=2;
		else bin8|=1;
		if(bins16[0]>bins16[1]) bin16|=8;
		else bin16|=4;
		if(bins16[2]>bins16[3]) bin16|=2;
		else bin16|=1;
		if(bins16e[0]>bins16e[1]) bin16e|=8;
		else bin16e|=4;
		if(bins16e[2]>bins16e[3]) bin16e|=2;
		else bin16e|=1;

		//-- now detect
		mindif=65536;ret=SRAW8;
		if(((dist8o>0.05) && (dist8e>0.05)) || (dist8<0.1)) {
			//if((ULONG)dif8   <mindif) { mindif=(ULONG)dif8;   ret=((labs(fi8  )<labs(fi8s  ))?SRAW8:RAW8); }
			//if((ULONG)dif8s  <mindif) { mindif=(ULONG)dif8s;  ret=((labs(fi8  )<labs(fi8s  ))?SRAW8:RAW8); }
			if((fabs(dif8-dif8s)/65536.0)>0.01) {
				if((ULONG)dif8   <mindif) { mindif=(ULONG)dif8;   ret=RAW8; }
				if((ULONG)dif8s  <mindif) { mindif=(ULONG)dif8s;  ret=SRAW8; }
			}
			else ret=((labs(fi8  )<labs(fi8s  ))?SRAW8:RAW8);
		}
		else if((dist8o>0.05) && (dist8e<0.05)) {
			//if((ULONG)dif16  <mindif) { mindif=(ULONG)dif16;  ret=((labs(fi16 )<labs(fi16s ))?SRAW16:RAW16); }
			//if((ULONG)dif16s <mindif) { mindif=(ULONG)dif16s; ret=((labs(fi16 )<labs(fi16s ))?SRAW16:RAW16); }
			if((fabs(dif16-dif16s)/65536.0)>0.01) {
				if((ULONG)dif16  <mindif) { mindif=(ULONG)dif16;  ret=RAW16; }
				if((ULONG)dif16s <mindif) { mindif=(ULONG)dif16s; ret=SRAW16; }
			}
			else ret=((labs(fi16 )<labs(fi16s ))?SRAW16:RAW16);
		}
		else if((dist8o<0.05) && (dist8e>0.05)) {
			//if((ULONG)dif16e <mindif) { mindif=(ULONG)dif16e; ret=((labs(fi16e)<labs(fi16se))?SRAW16E:RAW16E); }
			//if((ULONG)dif16se<mindif) { mindif=(ULONG)dif16se;ret=((labs(fi16e)<labs(fi16se))?SRAW16E:RAW16E); }
			if((fabs(dif16e-dif16se)/65536.0)>0.01) {
				if((ULONG)dif16e <mindif) { mindif=(ULONG)dif16e; ret=RAW16E; }
				if((ULONG)dif16se<mindif) { mindif=(ULONG)dif16se;ret=SRAW16E; }
			}
			else ret=((labs(fi16e)<labs(fi16se))?SRAW16E:RAW16E);
		}

#ifdef LOG_CRT
		strsfn(fn,NULL,NULL,fnnode,NULL);
		sprintf(logbuf,"%-30s %-7s  ",fnnode,crt_names[ret]);CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"%4.2lf %4.2lf %4.2lf %4.2lf  %4.2lf -> %2d  ",(double)bins8  [0]/ len    ,(double)bins8  [1]/ len    ,(double)bins8  [2]/ len    ,(double)bins8  [3]/ len    ,dist8,bin8 );CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"%4.2lf %4.2lf %4.2lf %4.2lf  %4.2lf  ",       (double)bino8  [0]/(len>>1),(double)bino8  [1]/(len>>1),(double)bino8  [2]/(len>>1),(double)bino8  [3]/(len>>1),dist8o     );CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"%4.2lf %4.2lf %4.2lf %4.2lf  %4.2lf  ",       (double)bine8  [0]/(len>>1),(double)bine8  [1]/(len>>1),(double)bine8  [2]/(len>>1),(double)bine8  [3]/(len>>1),dist8e     );CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"-> %2d  -> %2d  ",bin16,bin16e);CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		//sprintf(logbuf,"%4.2lf %4.2lf %4.2lf %4.2lf -> %2d %4.2lf  ",(double)bins16 [0]/(len>>1),(double)bins16 [1]/(len>>1),(double)bins16 [2]/(len>>1),(double)bins16 [3]/(len>>1),bin16 ,(double)(mas16 -mis16 ])/(len>>1));CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		//sprintf(logbuf,"%4.2lf %4.2lf %4.2lf %4.2lf -> %2d %4.2lf  ",(double)bins16e[0]/(len>>1),(double)bins16e[1]/(len>>1),(double)bins16e[2]/(len>>1),(double)bins16e[3]/(len>>1),bin16e,(double)(mas16e-mis16e])/(len>>1));CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		//sprintf(logbuf,"%1d %1d %1d %1d %1d %1d  ",minr8,manr8,minr16,manr16,minr16e,manr16e);CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		//sprintf(logbuf,"%9.5lf %9.5lf %9.5lf  %9.5lf %9.5lf %9.5lf %10.5lf  %9.5lf %9.5lf %9.5lf %10.5lf  ",ant8,ver8,ver8s, ant16,ver16,ver16s,antr16, ant16e,ver16e,ver16se,antr16e);
		//CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"%6d %6d %6d %6d %6d %6d  ",fi8,fi8s,fi16,fi16s,fi16e,fi16se);
		CheckRAWTypeLog("t:checkrawtype.log",logbuf);
		sprintf(logbuf,"%6d %6d %6d %6d %6d %6d -> %6d\n",(ULONG)dif8,(LONG)dif8s,(ULONG)dif16,(LONG)dif16s,(ULONG)dif16e,(LONG)dif16se,mindif);
		CheckRAWTypeLog("t:checkrawtype.log",logbuf);
#endif
	}
	else StdCError(fn,errOpenFile,errno,__FILE__,__LINE__);
	return(ret);
}

#ifdef LOG_CRT
void CheckRAWTypeLog(STRPTR filename,STRPTR fmt,...) {
	va_list arglist;
	FILE *log;

	if(filename && (log=fopen(filename,"ab"))) {
		va_start(arglist,fmt);
		vfprintf(log,fmt,arglist);
		va_end(arglist);
		fclose(log);
	}
}
#endif

void LoadRDL(void) {
	FILE *rdl;
	struct rawdetect *rd;
	char line[256];
	register UWORD i;
	UBYTE fail=FALSE;

	if((rdl=fopen(PROGDIR"_loaders/"PRJ_NAME"/rdl.data","rb"))) {
		while(!feof(rdl) && !fail) {
			if(fgets(line,255,rdl)) {
				i=0;					// search for end of first string (it includes \0 when saving)
				while(line[i]) i++;
				i++;
				if(rd=(struct rawdetect *)AllocVec(sizeof(struct rawdetect),MEMF_ANY|MEMF_CLEAR)) {
					rd->node.ln_Name=StringCopy(line);
					strncpy(rd->detect	   ,&line[i],255);rd->detect[255]='\0';
					AddTail(&rawdetectlist,rd);
				}
				else fail=TRUE;
			}
		}
		fclose(rdl);
	}
}

void SaveRDL(void) {
	FILE *rdl;
	struct rawdetect *rd=(struct rawdetect *)rawdetectlist.lh_Head;

	if((rdl=fopen(PROGDIR"_loaders/"PRJ_NAME"/rdl.data","wb"))) {
		while(!EndOfList(&rawdetectlist,(struct Node *)rd)) {
			fwrite(rd->node.ln_Name,strlen(rd->node.ln_Name)+1,1,rdl);
			fwrite(rd->detect      ,strlen(rd->detect      )+1,1,rdl);
			fputc('\n',rdl);
			rd=(struct rawdetect *)rd->node.ln_Succ;
		}
		fclose(rdl);
	}
}

void FreeRDL(void) {
	struct rawdetect *rd;
	
	while(rd=(struct rawdetect *)RemHead(&rawdetectlist)) {
		if(rd->node.ln_Name) FreeVec(rd->node.ln_Name);
		FreeVec((APTR *)rd);
	}
}

struct rawdetect *AddRDN(void) {
	struct rawdetect *rd;

	if((rd=AllocVec(sizeof(struct rawdetect),MEMF_ANY|MEMF_CLEAR))) {
		rd->node.ln_Name=StringCopy("new type");
		strcpy(rd->detect,"extension/header");
		AddHead(&rawdetectlist,rd);
	}
	return(rd);
}

void RemRDN(struct rawdetect *rd) { 
	Remove((struct Node *)rd);
	if(rd->node.ln_Name) FreeVec(rd->node.ln_Name);
	FreeVec((APTR)rd);
}

//-- eof ----------------------------------------------------------------------
