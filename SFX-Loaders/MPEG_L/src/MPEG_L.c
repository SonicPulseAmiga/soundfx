/****h* MPEG/MPEG_L.c [0.0] *
*
*  NAME
*    MPEG_L.c
*  COPYRIGHT
*    $VER: MPEG_L.c 0.0 (15.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    MPEG Loader module for SoundFX (uses MPEGA.library) - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    05.Sep.1998
*  MODIFICATION HISTORY
*    15.Apr.2004    V 0.0   most recent version
*    29.May.2002	V 4.2	most recent version
*    03.Jan.2001	V 4.1	most recent version
*    25.Oct.1999	V 1.2	most recent version
*    05.Sep.1998	V 1.0	initial version
*  NOTES
*
*  IDEAS
*    allow to specify which mpega compatible library to use
*******
*/

/****u* _loaders/MPEG_L *
*  Function.eng
*    Loads MPEG Samples with a decoder compatible to the mpega.library
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes</td></tr>
*    </table>
*    You will find lots of sample in this format on the internet. Due to its high
*    compression ratio, it's of excellent use for compressing whole songs.
*  Funktion.ger
*    Läd MPEG Samples mit einem Decoder der zur mpega.library kompatibel ist.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja</td></tr>
*    </table>
*    Sie werden viele dieser Samples im Internet finden. Wegen der sehr guten
*    Packrate, ist dieses Format hervorragend geeignet um komplette Songs zu packen.
*  Parameter.eng
*    Engine
*      Allows to choose an mpega.library compatible decoder library. There are
*      versions of the mpega.library available, which offer better quality
*      (FPU and MAD) but run slower. You can use a PPC based decoder as well,
*      if you have such a CPU.
*    Layer I&II
*      These settings affect Layer I and Layer II files.
*      You can choose the quallity for decoding for mono and stereo files separately.
*      The lower it is, the faster it loads.
*      If you want to save some memory you may force the loader
*      to convert stereo files to mono
*    Layer III
*      Same as above, but for Layer III files.
*  Parameter.ger
*    Engine
*      Erlaubt die Auswahl einer mpega.library kompatiblen Decoder-Bibliothek. Von der
*      mpega.library existieren Versionen (FPU und MAD) die eine höhere Qualität bieten,
*      aber langsamer sind. Sie können auch einen PPC basierenden Decoder verwenden
*      wenn sie eine solche CPU haben.
*    Layer I&II
*      Diese Einstellungen beziehen sich auf die Layer I und Layer II Dateien.
*      Sie können Qualität des Entpackens für Mono und Stereo Dateien getrennt einstellen.
*      Wenn sie etwas Speicher sparen möchten, können sie weiterhin erzwingen, daß
*      Stereodateien nach Mono konvertieret werden sollen.
*    Layer III
*      Genau wie oben, nur jedoch für Layer III Dateien.
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define MPEG_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>
#include <libraries/mpega.h>
#include <proto/mpega.h>

//-- Local

#include "MPEG.h"
#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_ENGINEPOPUP	(GADID_BASE+1)
#define GADIX_ENGINEPOPUP	(GADIX_BASE+1)
#define GADID_ENGINE		(GADID_ENGINEPOPUP+1)
#define GADIX_ENGINE		(GADIX_ENGINEPOPUP+1)

#define GADID_L12_M_Q	(GADID_ENGINE+1)
#define GADIX_L12_M_Q	(GADIX_ENGINE+1)
#define GADID_L12_M_MF	(GADID_L12_M_Q+1)
#define GADIX_L12_M_MF	(GADIX_L12_M_Q+1)
#define GADID_L12_S_Q	(GADID_L12_M_MF+1)
#define GADIX_L12_S_Q	(GADIX_L12_M_MF+1)
#define GADID_L12_S_MF	(GADID_L12_S_Q+1)
#define GADIX_L12_S_MF	(GADIX_L12_S_Q+1)
#define GADID_L12_FM	(GADID_L12_S_MF+1)
#define GADIX_L12_FM	(GADIX_L12_S_MF+1)
#define GADID_L3_M_Q	(GADID_L12_FM+1)
#define GADIX_L3_M_Q	(GADIX_L12_FM+1)
#define GADID_L3_M_MF	(GADID_L3_M_Q+1)
#define GADIX_L3_M_MF	(GADIX_L3_M_Q+1)
#define GADID_L3_S_Q	(GADID_L3_M_MF+1)
#define GADIX_L3_S_Q	(GADIX_L3_M_MF+1)
#define GADID_L3_S_MF	(GADID_L3_S_Q+1)
#define GADIX_L3_S_MF	(GADIX_L3_S_Q+1)
#define GADID_L3_FM		(GADID_L3_S_MF+1)
#define GADIX_L3_FM		(GADIX_L3_S_MF+1)

#define LAST_GADIX		(GADIX_L3_FM+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	MPEGA_CTRL	mpg_ctrl;
	UBYTE	engine[FILENAME_MAX];			// which encoder to use
};

char *QualLabels[]={ "Low","Medium","High",0l };

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

SFXMod_DoClientDataInit
SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables

	UWORD ys1=2;
	UWORD ys2=ys1+1+ANYFRM_HEIGHT(1);
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(3);
	UWORD ys4=ys3+1+ANYFRM_HEIGHT(3);
	UWORD winh=ys4+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[328]);
	UWORD xh;

	SFXMod_OpenGUILibs;
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[42]);
		ng.ng_TopEdge		=ys4+GetRunTime(scy[2]);
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

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[79]);					// Encoder Engine
		ng.ng_TopEdge		=ys1+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[15]);
		ng.ng_TextAttr		=GetRunTime(SigFont);
		ng.ng_GadgetText	=IMAGE_PopUp;
		ng.ng_GadgetID		=GADID_ENGINEPOPUP;
		ng.ng_Flags			=PLACETEXT_IN;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_ENGINEPOPUP]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[95]);
		ng.ng_Width			=GetRunTime(scx[203]);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_ENGINE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_MaxChars,FILENAME_MAX,GTST_String,instance->engine,TAG_DONE);
		modGadgets[GADIX_ENGINE]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);					// Layer I & II
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_GadgetText	="Mono Quality";
		ng.ng_GadgetID		=GADID_L12_M_Q;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,QualLabels,GTCY_Active,instance->mpg_ctrl.layer_1_2.mono.quality,TAG_DONE);
		modGadgets[GADIX_L12_M_Q]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET)+12];
		ng.ng_GadgetText	="Stereo Quality";
		ng.ng_GadgetID		=GADID_L12_S_Q;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,QualLabels,GTCY_Active,instance->mpg_ctrl.layer_1_2.stereo.quality,TAG_DONE);
		modGadgets[GADIX_L12_S_Q]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[FRMCONTENT_YOFFSET)+24];
		ng.ng_Width			=GetRunTime(scx[26]);
		ng.ng_GadgetText	="Force Mono";
		ng.ng_GadgetID		=GADID_L12_FM;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->mpg_ctrl.layer_1_2.force_mono,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_L12_FM]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);					// Layer III
		ng.ng_TopEdge		=ys3+GetRunTime(scy[FRMCONTENT_YOFFSET]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_GadgetText	="Mono Quality";
		ng.ng_GadgetID		=GADID_L3_M_Q;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,QualLabels,GTCY_Active,instance->mpg_ctrl.layer_3.mono.quality,TAG_DONE);
		modGadgets[GADIX_L3_M_Q]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[FRMCONTENT_YOFFSET)+12];
		ng.ng_GadgetText	="Stereo Quality";
		ng.ng_GadgetID		=GADID_L3_S_Q;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,QualLabels,GTCY_Active,instance->mpg_ctrl.layer_3.stereo.quality,TAG_DONE);
		modGadgets[GADIX_L3_S_Q]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[100]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[FRMCONTENT_YOFFSET)+24];
		ng.ng_Width			=GetRunTime(scx[26]);
		ng.ng_GadgetText	="Force Mono";
		ng.ng_GadgetID		=GADID_L3_FM;
		g=CreateGadget(CHECKBOX_KIND,g,&ng,GTCB_Checked,instance->mpg_ctrl.layer_3.force_mono,GTCB_Scaled,TRUE,TAG_DONE);
		modGadgets[GADIX_L3_FM]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			xh=GetRunTime(scx[328]);
			DrawTitle(modrp,"Decoder Engine"	,xs1,ys1,xh);
			DrawTitle(modrp,"Layer I & II"		,xs1,ys2,xh);
			DrawTitle(modrp,"Layer III"			,xs1,ys3,xh);

			DrawGadWB(modrp,xs1,ys1,xh,ANYFRM_HEIGHT(1));		/* Engine */
			DrawGadWB(modrp,xs1,ys2,xh,ANYFRM_HEIGHT(3));		/* Layer I & II */
			DrawGadWB(modrp,xs1,ys3,xh,ANYFRM_HEIGHT(3));		/* Layer III */
			DrawGadWB(modrp,xs1,ys4,xh,CMDFRM_HEIGHT);			/* Go */

			while(runing) {
				WaitPort(modWin->UserPort);
				while(imsg=GT_GetIMsg(modWin->UserPort)) {
					SFXMod_Interface_AnalyseIMsg
					switch(iclass) {
						case IDCMP_ACTIVEWINDOW:
							WindowToFront(modWin);
							break;
						case IDCMP_GADGETUP:
							switch((int)(((struct Gadget *)iadr)->GadgetID)) {
								case GADID_GO:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
									ret=TRUE;runing=FALSE;
									break;
								case GADID_OKAY:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
									runing=FALSE;
									break;
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
								case GADID_L12_M_Q:	instance->mpg_ctrl.layer_1_2.mono.	quality=icode;break;
								case GADID_L12_S_Q:	instance->mpg_ctrl.layer_1_2.stereo.	quality=icode;break;
								case GADID_L12_FM:	instance->mpg_ctrl.layer_1_2.force_mono=modGadgets[GADIX_L12_FM]->Flags&GFLG_SELECTED;break;
								case GADID_L3_M_Q:	instance->mpg_ctrl.layer_3.	mono.	quality=icode;break;
								case GADID_L3_S_Q:	instance->mpg_ctrl.layer_3.	stereo.	quality=icode;break;
								case GADID_L3_FM:	instance->mpg_ctrl.layer_3.	force_mono=modGadgets[GADIX_L3_FM ]->Flags&GFLG_SELECTED;break;
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
									ActivateGadget(modGadgets[GADIX_L12_M_Q],modWin,0l);
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
											SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
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
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	SInfo *si=pdata->dst[0];
	struct Library *MPEGABase;
	MPEGA_STREAM *mpg;
	UBYTE ret=TRUE;

	// allow other mpeg interface compatible libraries to be loaded
	if(!(MPEGABase=OpenLibrary(instance->engine,2L))) { sprintf(errmsg,"%s >=V2.0",instance->engine);ADosError(errmsg,errOpenLibrary,IoErr(),__FILE__,__LINE__);return(FALSE); }
	if(mpg=MPEGA_open(pdata->fn,&instance->mpg_ctrl)) {
		MSG1("mpeg-stream %s opened succesfully",pdata->fn);
		MSG1("\tnorm        : %d",mpg->norm);
		MSG1("\tlayer       : %d",mpg->layer);
		MSG1("\tbitrate     : %d kps",mpg->bitrate);
		MSG1("\tfrequency   : %d Hz",mpg->frequency);
		MSG1("\tchannels    : %d",mpg->channels);
		MSG1("\tduration    : %d ms",mpg->ms_duration);
		MSG1("\tprivate     : %d",mpg->private_bit);
		MSG1("\tcopyright   : %d",mpg->copyright);
		MSG1("\toriginal    : %d",mpg->original);

		si->srat=mpg->frequency;
		si->slen=(ULONG)(0.5+mpg->frequency*((double)mpg->ms_duration/1000.0));
		SET_CHANNELS(si,(mpg->channels-1));
	
		MSG1("\tduration    : %d samples",si->slen);

		if(AllocSampleMem(si,si->slen,mpg->channels)) {
			WORD pcmbuf0[MPEGA_PCM_SIZE],pcmbuf1[MPEGA_PCM_SIZE];
			WORD *pcmbuf[MPEGA_MAX_CHANNELS];
			WORD *pcmptr0,*pcmptr1;
			ULONG curlen=0;
			register ULONG i,dpos=0;
			LONG declen=0;
			ProWinTitle title;

			MSG("AllocSampleMem okay");

			pcmbuf[0]=pcmbuf0;pcmbuf[1]=pcmbuf1;
			switch(mpg->channels) {
				case 1:		// mono
				{
					SmpAccess * const dstsa0=SAInit(si,0,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					MSG("--> mono");
					sprintf(title,GetString(msgProgressLoad),1,1);
					if(dstsa0) {
						SAFuncPtr const SAWrite=dstsa0->safunc;
						PWData * const pwd=NewProWin();
						if(pwd) {
							SAMPLE *dbuf0=(SAMPLE *)dstsa0->buffer1;			// Pointer auf dst-Block
							StartProWin(pwd,&curlen,title,si->slen);					// sets curlen=0;
							while((declen!=MPEGA_ERR_EOF) && (curlen<si->slen)) {
								if((declen=MPEGA_decode_frame(mpg,pcmbuf))>0) {
									pcmptr0=pcmbuf[0];
									for(i=0;((i<declen) && (curlen<si->slen));i++,curlen++) {
										*(dbuf0++)=*(pcmptr0++);dpos++;
										if(dpos==Bytes2Samples(dstsa0->blen)) {
											SAWrite(dstsa0,dstsa0->blen);dstsa0->seg++;					// akt Block schreiben
											dbuf0=(SAMPLE *)dstsa0->buffer1;dpos=0;						// Pointer auf dst-Block
										}
									}
								}
								else ERR2("decoding error #%ld at length=%ld",declen,curlen);
							}
							if((declen<0) && (declen!=MPEGA_ERR_EOF)) ret=FALSE;
							curlen=si->slen;
							if(dpos) { SAWrite(dstsa0,dstsa0->blen);dstsa0->seg++; }				// akt Block schreiben
							DelProWin(pwd);
						}
					}
					if(dstsa0) SADone(dstsa0,si);
				}
				break;
				case 2:		// stereo
				{
					SmpAccess * const dstsa0=SAInit(si,0,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					SmpAccess * const dstsa1=SAInit(si,1,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,VM_DEFAULT_BLKLEN);	// sequentiell Blöcke konstanter Größe schreiben
					MSG("--> stereo");
					sprintf(title,GetString(msgProgressLoad),1,1);
					if(dstsa0 && dstsa1) {
						SAFuncPtr const SAWrite=dstsa0->safunc;
						PWData * const pwd=NewProWin();
						if(pwd) {
							SAMPLE *dbuf0=(SAMPLE *)dstsa0->buffer1;			// Pointer auf dst-Block
							SAMPLE *dbuf1=(SAMPLE *)dstsa1->buffer1;			// Pointer auf dst-Block
							StartProWin(pwd,&curlen,title,si->slen);					// sets curlen=0;
							while((declen!=MPEGA_ERR_EOF) && (curlen<si->slen)) {
								if((declen=MPEGA_decode_frame(mpg,pcmbuf))>0) {
									// DEBUG
									//MSG1("curlen = %ld",curlen);
									// DEBUG
									pcmptr0=pcmbuf[0];pcmptr1=pcmbuf[1];
									for(i=0;((i<declen) && (curlen<si->slen));i++,curlen++) {
										*(dbuf0++)=*(pcmptr0++);*(dbuf1++)=*(pcmptr1++);dpos++;
										if(dpos==Bytes2Samples(dstsa0->blen)) {
											SAWrite(dstsa0,dstsa0->blen);dstsa0->seg++;					// akt Block schreiben
											dbuf0=(SAMPLE *)dstsa0->buffer1;							// Pointer auf dst-Block
											SAWrite(dstsa1,dstsa1->blen);dstsa1->seg++;					// akt Block schreiben
											dbuf1=(SAMPLE *)dstsa1->buffer1;dpos=0;						// Pointer auf dst-Block
										}
									}
								}
								else ERR2("decoding error #%ld at length=%ld",declen,curlen);
							}
							if((declen<0) && (declen!=MPEGA_ERR_EOF)) ret=FALSE;
							// DEBUG
							//MSGC1((declen==MPEGA_ERR_EOF),"declen==MPEGA_ERR_EOF (curlen=%ld)",curlen);
							//MSGC((curlen>=si->slen),"curlen>=si->slen");
							// DEBUG
							curlen=si->slen;
							if(dpos) {
								SAWrite(dstsa0,dstsa0->blen);dstsa0->seg++;								// akt Block schreiben
								SAWrite(dstsa1,dstsa1->blen);dstsa1->seg++;								// akt Block schreiben
							}
							DelProWin(pwd);
						}
					}
					if(dstsa0) SADone(dstsa0,si);
					if(dstsa1) SADone(dstsa1,si);
				}
				break;
			}
		}
		else { si->slen=0;Message(errAllocateMemory,NULL,__FILE__,__LINE__);return(FALSE); }
		//-- set file comment
		{	
			char sc_fmt[50];
		
			sprintf(sc_fmt,"%d kbit",mpg->bitrate);
			SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);
		}
		MPEGA_close(mpg);
		si->zoomxl=si->slen;
	}
	else { MSG("Can't open mpeg-stream");return(FALSE); }
	CloseLibrary(MPEGABase);
	return(ret);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	instance->mpg_ctrl.layer_1_2.mono.  quality=CfgIO_MatchString(
											   CfgIO_ReadString			(cfg,"Layer_1_2"	,"Mono_Quality"		,QualLabels[MPEGA_QUALITY_HIGH]),QualLabels);
	instance->mpg_ctrl.layer_1_2.stereo.quality=CfgIO_MatchString(
											   CfgIO_ReadString			(cfg,"Layer_1_2"	,"Stereo_Quality"	,QualLabels[MPEGA_QUALITY_HIGH]),QualLabels);
	instance->mpg_ctrl.layer_1_2.force_mono=	   CfgIO_ReadBool			(cfg,"Layer_1_2"	,"Force_Mono"		,FALSE);

	instance->mpg_ctrl.layer_3.	mono.  quality=CfgIO_MatchString(
											   CfgIO_ReadString			(cfg,"Layer_3"		,"Mono_Quality"		,QualLabels[MPEGA_QUALITY_HIGH]),QualLabels);
	instance->mpg_ctrl.layer_3.	stereo.quality=CfgIO_MatchString(
											   CfgIO_ReadString			(cfg,"Layer_3"		,"Stereo_Quality"	,QualLabels[MPEGA_QUALITY_HIGH]),QualLabels);
	instance->mpg_ctrl.layer_3.	force_mono=	   CfgIO_ReadBool			(cfg,"Layer_3"		,"Force_Mono"		,FALSE);

	strcpy(instance->engine,		CfgIO_ReadString		(cfg,"General"	,"Engine"		,"mpega.library"));
}

void SaveSettings(struct Instance *instance,char *fname) {
	CfgIO_WriteString		  (cfg,"Layer_1_2"	,"Mono_Quality"		,QualLabels[instance->mpg_ctrl.layer_1_2.mono.  quality]);
	CfgIO_WriteString		  (cfg,"Layer_1_2"	,"Stereo_Quality"	,QualLabels[instance->mpg_ctrl.layer_1_2.stereo.quality]);
	CfgIO_WriteBool			  (cfg,"Layer_1_2"	,"Force_Mono"		,instance->mpg_ctrl.layer_1_2.force_mono);

	CfgIO_WriteString		  (cfg,"Layer_3"	,"Mono_Quality"		,QualLabels[instance->mpg_ctrl.layer_3.	mono.  quality]);
	CfgIO_WriteString		  (cfg,"Layer_3"	,"Stereo_Quality"	,QualLabels[instance->mpg_ctrl.layer_3.	stereo.quality]);
	CfgIO_WriteBool			  (cfg,"Layer_3"	,"Force_Mono"		,instance->mpg_ctrl.layer_3.	 force_mono);

	CfgIO_WriteString		  (cfg,"General"	,"Engine"		,instance->engine);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->mpg_ctrl.bs_access=NULL;
	instance->mpg_ctrl.layer_1_2.force_mono=FALSE;
	instance->mpg_ctrl.layer_1_2.mono.	freq_div=1;
	instance->mpg_ctrl.layer_1_2.mono.	quality=MPEGA_QUALITY_HIGH;
	instance->mpg_ctrl.layer_1_2.mono.	freq_max=MAXLONGP;
	instance->mpg_ctrl.layer_1_2.stereo.	freq_div=1;
	instance->mpg_ctrl.layer_1_2.stereo.	quality=MPEGA_QUALITY_HIGH;
	instance->mpg_ctrl.layer_1_2.stereo.	freq_max=MAXLONGP;
	instance->mpg_ctrl.layer_3.	force_mono=FALSE;
	instance->mpg_ctrl.layer_3.	mono.	freq_div=1;
	instance->mpg_ctrl.layer_3.	mono.	quality=MPEGA_QUALITY_HIGH;
	instance->mpg_ctrl.layer_3.	mono.	freq_max=MAXLONGP;
	instance->mpg_ctrl.layer_3.	stereo.	freq_div=1;
	instance->mpg_ctrl.layer_3.	stereo.	quality=MPEGA_QUALITY_HIGH;
	instance->mpg_ctrl.layer_3.	stereo.	freq_max=MAXLONGP;
	instance->mpg_ctrl.check_mpeg=0;
	instance->mpg_ctrl.stream_buffer_size=0l;

	strcpy(instance->engine,"mpega.library");
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	GT_SetGadgetAttrs(modGadgets[GADIX_L12_M_Q],modWin,0l,GTCY_Active ,instance->mpg_ctrl.layer_1_2.	mono.	quality,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_L12_S_Q],modWin,0l,GTCY_Active ,instance->mpg_ctrl.layer_1_2.	stereo.	quality,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_L12_FM ],modWin,0l,GTCB_Checked,instance->mpg_ctrl.layer_1_2.	force_mono,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_L3_M_Q ],modWin,0l,GTCY_Active ,instance->mpg_ctrl.layer_3.	mono.	quality,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_L3_S_Q ],modWin,0l,GTCY_Active ,instance->mpg_ctrl.layer_3.	stereo.	quality,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_L3_FM  ],modWin,0l,GTCB_Checked,instance->mpg_ctrl.layer_3.	force_mono,TAG_DONE);
}

//-- private Stuff

//-- eof ----------------------------------------------------------------------
