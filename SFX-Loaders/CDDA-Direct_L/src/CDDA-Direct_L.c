/****h* CDDA-Direct/CDDA-Direct_L.c [4.2] *
*
*  NAME
*    CDDA-Direct_L.c
*  COPYRIGHT
*    $VER: CDDA-Direct_L.c 4.2 (05.01.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Clipboard loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    20.Jun.1999
*  MODIFICATION HISTORY
*    05.Jan.2003	V 4.2	most recent version
*    17.Nov.2000	V 4.1	most recent version
*    03.Oct.1999	V 1.2	most recent version
*    20.Jun.1997	V 1.0	initial version
*  NOTES
*
*  IDEAS
*    * Add speed control at tracklist-req
*    * create new list from exec->devices in prefs-win, where know-not-scsi devices are excluded
*      (ariadne,audio,clipbord,console,diskspare,gameport,input,keyboard,mfm,narrator,netinfo,ramdrive,timer,trackdisk)
*******
*/

/****u* _loaders/CDDA-Direct_L *
*  Function.eng
*    Copies files digitaly (1:1) from CDs. This has the advantage of very high
*    quality, because it avoids converting the data (digital-&gt;analog and again
*    analog-&gt;digital).<br>
*    Instead of a file requester, a track listing will appear, where you can
*    choose the desired track and set start/end/length.
*    Read the chapter about <a href="../node01.05.09.html">recording/sampling</a> as well.
*  Funktion.ger
*    Kopiert Daten digital (1:1) von CDs. Dies hat den Vorteil einer sehr hohen
*    Qualität, da die Daten nicht gewandelt werden müßen (digital-&gt;analog und
*    wieder analog-&gt;digital).<br>
*    Anstatt eines Filerequesters erscheint ein Tracklisting der CD, in dem Sie
*    den gewünschten Titel auswählen und Start/Ende/Länge einstelllen können.
*    Lesen sie auch den Abschnitt über <a href="../node01.05.09.html">Aufnahme/Sampling</a>
*  Parameter.eng
*    Device
*      Name of the device-driver which controls the cd-drive.
*    Unit
*      Numer of device
*    Method
*      The method which should be used to read from the drive.
*    Memory
*      Which memory should be used for internal read buffer.
*        Any : doesn't matter
*        Fast : choose only if you have some
*        24bit : go for this if you experience crashes
*  Parameter.ger
*    Device
*      Name des Gerätetreibers an dem das CD-Laufwerk angeschlossen ist.
*    Unit
*      Nummer des Gerätes
*    Method
*      Methode mit der versucht wird vom Laufwerk zu lesen.
*    Memory
*      In welchem Speichertyp der interne Lesepuffer angelegt weren soll.
*        Any : egal
*        Fast :	nur anwählen, wenn sie auch welches haben
*        24bit : falls es zu Abstürzen kommt
*  Notes.eng
*    This won't work with all drives. At first not every cd-rom or cd-writer
*    is capable of DAE (Digital Audio Extraction) and what is worse, there is
*    no standart way of doing it. To check, if your drive can do it and if yes
*    how, see the list below.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_HEAD><th>Drive   </th><th>Type          </th><th>Interface</th><th>DAE</th><th>Modes</th></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Plextor </td><td>CD-ROM PX-32TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Plextor </td><td>CD-ROM PX-40TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Ricoh   </td><td>              </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM, Toshiba</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-523S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Teac    </td><td>CD-R55S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-R58S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Toshiba </td><td>              </td><td>SCSI     </td><td>yes</td><td>Toshiba</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Traxdata</td><td>CDR4120       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    </table>
*    Please send me reports to complete this list.
*  Hinweise.ger
*    Dies funktioniert nicht mit allen Laufwerken. Zum Einen sind nicht alle
*    CD-Roms und CD-Writer in der Lage DAE (Digital Audio Extraction) anzubieten
*    und was noch viel schlimmer ist, es gibt kein Standardverfahren dafür.
*    Um herauszufinden ob Ihr Laufwerk so etwas kann und wenn ja wie, schauen
*    sie bitte in der folgenden Liste nach.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_HEAD><th>Laufwerk</th><th>Typ           </th><th>Anschluß </th><th>DAE</th><th>Modi</th></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Plextor </td><td>CD-ROM PX-32TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Plextor </td><td>CD-ROM PX-40TS</td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Ricoh   </td><td>              </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM, Toshiba</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-523S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Teac    </td><td>CD-R55S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Teac    </td><td>CD-R58S       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Toshiba </td><td>              </td><td>SCSI     </td><td>yes</td><td>Toshiba</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Traxdata</td><td>CDR4120       </td><td>SCSI     </td><td>yes</td><td>Plextor/Sony/IBM</td></tr>
*    </table>
*    Bitte senden Sie mir ihre Erfahrungen zu um die Liste zu kompletieren.
*******
*/

#define CDDA_Direct_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>
#include <devices/scsidisk.h>
#include <devices/scsidefs.h>

//-- Local

#include "project.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- datatypes ----------------------------------------------------------------

typedef char TrackName[100];

#define MAX_SCSI_UNITS	14

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private stuff - scsi helper

ULONG SCSIStatus(struct SCSICmd *scsi_cmd);
void  SCSICmd_6 (UBYTE *cmd,UBYTE commandCode,UBYTE lun,UWORD pageCode,UBYTE length,UBYTE control);
void  SCSICmd_10(UBYTE *cmd,UBYTE commandCode,UBYTE lun,ULONG pageCode,UWORD length,UBYTE control);
void  SCSICmd_12(UBYTE *cmd,UBYTE commandCode,UBYTE lun,ULONG pageCode,ULONG length,UBYTE control);

BOOL  SCSIDeviceInit(struct Instance *instance);
void  SCSIDeviceDone(struct Instance *instance);

BOOL  SCSICmdSubmit(struct Instance *instance,UBYTE cmdnr);

//-- private stuff - info gathering

LONG  SCSIGetUnits(struct Instance *instance,STRPTR *names);
LONG  SCSIGetTracks(struct Instance *instance,struct List *TrackList,struct Node *TrackNodes,TrackName *TrackNames,ULONG *TrackSE);

//-- private stuff - gui helper (local 'file'requester)

UBYTE GetCDDAUnit(struct Instance *instance,struct TrackData *unit);
void UnitReq_UpdateSEL(struct Gadget *modGadgets[],struct Window *modWin,ULONG rs,ULONG re,ULONG rl,UBYTE sh);

//-- private stuff - various drives support

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_DEVICE	(GADID_BASE+1)
#define GADIX_DEVICE	(GADIX_BASE+1)
#define GADID_UNIT		(GADID_DEVICE+1)
#define GADIX_UNIT		(GADIX_DEVICE+1)
#define GADID_METHOD	(GADID_UNIT+1)
#define GADIX_METHOD	(GADIX_UNIT+1)
#define GADID_MEMORY	(GADID_METHOD+1)
#define GADIX_MEMORY	(GADIX_METHOD+1)

#define GADID_TRACK		(GADID_MEMORY+1)
#define GADIX_TRACK		(GADIX_MEMORY+1)
#define GADID_START_ST	(GADID_TRACK+1)
#define GADIX_START_ST	(GADIX_TRACK+1)
#define GADID_START_SL	(GADID_START_ST+1)
#define GADIX_START_SL	(GADIX_START_ST+1)
#define GADID_END_ST	(GADID_START_SL+1)
#define GADIX_END_ST	(GADIX_START_SL+1)
#define GADID_END_SL	(GADID_END_ST+1)
#define GADIX_END_SL	(GADIX_END_ST+1)
#define GADID_LENGTH_ST	(GADID_END_SL+1)
#define GADIX_LENGTH_ST	(GADIX_END_SL+1)
#define GADID_LENGTH_SL	(GADID_LENGTH_ST+1)
#define GADIX_LENGTH_SL	(GADIX_LENGTH_ST+1)

#define LAST_GADIX		(GADIX_LENGTH_SL+1)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

#define CMDS	4

struct Instance {
	char	device[FILENAME_MAX];		// device to use
	LONG	unit;						// unit to use
	ULONG	method;						// rip-method
	ULONG	memory;						// memory flags

	BOOL			SCSIDevice;
	struct MsgPort	*MyMsgPort		[CMDS];				/* For receiving replies to device requests. */
	struct IOStdReq	*MyIORequest	[CMDS];				/* Commands to scsi.device are put into this IO request record. */
	struct SCSICmd	*MySCSICommand	[CMDS];				/* The IO request points to this record which points to the various buffers
														   needed for running the command.  If the buffer pointer fields in this record
														   are NULL then the buffers haven't been allocated yet. */
	UBYTE *Cmd[CMDS],*CmdData[CMDS],*SenseData[CMDS];	/* IO-Buffers */
	UBYTE *CddaData[CMDS];
	BOOL  memok;
	BOOL DeviceOpen[CMDS];				// TRUE if the scsi.device was OpenDevice'd and needs to be closed when the program exits.
};

struct TrackData {
	LONG	track;						// track to read
	ULONG	startLBA,endLBA;			// Logical Block Address to start and end reading
	UBYTE	channels;					// how many channels has this track (stereo,quadro)
};

STRPTR MethLabels[]={
	"Plextor, Sony",
	"Toshiba",
	0l
};

#define METH_PlexSony	0
#define	METH_Tosh		1

STRPTR MemoLabels[]={
	"Any","Fast","24bit",0l
};

#define BUFMEM_Any		0
#define BUFMEM_Fast		1
#define BUFMEM_24bit	2

#define SCSI_BUFFER_SIZES 1024						/* All the buffers (data, command, sense) in MySCSICommand are this size and
													   are allocated from DMA-able memory.  Though scsi.device is reported to use
													   programmed IO rather than DMA for transfers <256 bytes. */
#define SCSI_MAX_SIZE_6		254
#define SCSI_MAX_SIZE_10	1022
#define SCSI_MAX_SIZE_12	1022
#define SCSI_MAX_SIZE		1022

#define SCSI_CDDA_LEN		2352					/* subcode 0 -> no sub code*/
//#define SCSI_CDDA_LEN		2368					/* subcode 1 -> sub q code*/
//#define SCSI_CDDA_LEN		2448					/* subcode 2 -> all sub codes*/
//#define SCSI_CDDA_LEN		  96					/* subcode 3 -> only sub codes*/
#define SCSI_CDDA_SIZES		(SCSI_CDDA_LEN*SCSIUC_FPS)		/* read one second at once */
//#define SCSI_CDDA_SIZES		SCSI_CDDA_LEN		/* read 1/75 second at once */		

//-- Library-Basisaddrs.

struct Library 				*ListToolsBase=NULL;

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	SFXMod_OpenGUILibs;
	if(!(ListToolsBase	=OpenLibrary(ListToolsName,ListToolsVersion)))					{ ADosError(ListToolsName" >=V"NUM2STR(ListToolsVersion),errOpenLibrary,IoErr(),__FILE__,__LINE__);goto Error; }
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	if(cfg)				CfgIO_RemConfig(cfg);
	if(ListToolsBase)	CloseLibrary(ListToolsBase);
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

void __saveds __asm *LIBSFXMod_ClientDataInit(REG(a0,RTime *RunTime_)) {
	register UBYTE i;
	ULONG memm;
	struct Instance *instance;

	INTRO;

	if(instance=(struct Instance *)AllocVec(sizeof(struct Instance),MEMF_ANY|MEMF_CLEAR)) {
		SFXMod_ClientDataInitCommon;
		instance->SCSIDevice=FALSE;
	
		for(i=0;i<CMDS;i++) {
			if(!(instance->MyMsgPort	[i]=CreateMsgPort())) goto Error;
			if(!(instance->MyIORequest	[i]=(struct IOStdReq *)CreateIORequest(instance->MyMsgPort[i],sizeof(struct IOStdReq)))) goto Error;
			if(!(instance->MySCSICommand[i]=AllocMem(sizeof(struct SCSICmd),MEMF_PUBLIC|MEMF_CLEAR))) goto Error;
	
			/* Set up for a direct SCSI command through the scsi.device. */
			instance->MyIORequest[i]->io_Command		=HD_SCSICMD;
			instance->MyIORequest[i]->io_Data			=instance->MySCSICommand[i];
			instance->MyIORequest[i]->io_Length			=sizeof(struct SCSICmd);
			switch(instance->memory) {
				case BUFMEM_Any:	memm=MEMF_PUBLIC;break;
				case BUFMEM_Fast:	memm=MEMF_FAST|MEMF_PUBLIC;break;
				case BUFMEM_24bit:	memm=MEMF_24BITDMA|MEMF_PUBLIC;break;
				default:			memm=MEMF_PUBLIC;break;
			}
			if(!(instance->Cmd[i]		=AllocMem(SCSI_BUFFER_SIZES		 ,memm))) goto Error;
			if(!(instance->CmdData[i]	=AllocMem(SCSI_BUFFER_SIZES		 ,memm))) goto Error;
			if(!(instance->SenseData[i]	=AllocMem(SCSI_BUFFER_SIZES		 ,memm))) goto Error;
			if(!(instance->CddaData[i]	=AllocMem((SCSI_CDDA_SIZES+16384),memm))) goto Error;
	
			instance->MySCSICommand[i]->scsi_Data		=(UWORD *)instance->CmdData[i];
			instance->MySCSICommand[i]->scsi_Command	=instance->Cmd[i];
			instance->MySCSICommand[i]->scsi_SenseData	=instance->SenseData[i];
			instance->MySCSICommand[i]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
			//instance->MySCSICommand[i]->scsi_CmdLength=SCSI_MAX_SIZE;			/* command buffer size. */
			instance->MySCSICommand[i]->scsi_SenseLength=SCSI_MAX_SIZE;			/* error message buffer size. */
		}
		MSG("  cmds okay");
	
		instance->memok=TRUE;
	}
	OUTRO;
	return((void *)instance);
Error:
	instance->memok=FALSE;
	LIBSFXMod_ClientDataDone(instance);
	
	OUTRO;
	return(NULL);
}

void __saveds __asm LIBSFXMod_ClientDataDone(REG(a0,struct Instance *instance)) {
	if(instance) {
		register UBYTE i;

		for(i=0;i<CMDS;i++) {
			if(instance->MySCSICommand[i]) {
				if(instance->Cmd[i])		FreeMem(instance->Cmd[i]		,SCSI_BUFFER_SIZES);
				if(instance->CmdData[i])	FreeMem(instance->CmdData[i]	,SCSI_BUFFER_SIZES);
				if(instance->SenseData[i])	FreeMem(instance->SenseData[i]	,SCSI_BUFFER_SIZES);
				if(instance->CddaData[i])	FreeMem(instance->CddaData[i]	,(SCSI_CDDA_SIZES+16384));
				FreeMem(instance->MySCSICommand[i],sizeof(struct SCSICmd));
			}
			if(instance->MyIORequest[i])	DeleteIORequest(instance->MyIORequest[i]);
			if(instance->MyMsgPort  [i])	DeleteMsgPort  (instance->MyMsgPort  [i]);
		}
		SFXMod_ClientDataDoneCommon;
		FreeVec((void *)instance);
	}
}

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	SFXMod_Interface_GUIVariables

	register UBYTE j;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[98+13])+6;
	UWORD winh=ys2+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[500]);
	UWORD xh;
	LONG aktdev;

	char *UnitNames[MAX_SCSI_UNITS]={
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                ",
		"                                                                                "
	};
	struct List UnitList,DeviList;
	struct Node UnitNodes[MAX_SCSI_UNITS]={0},DeviNodes[128]={0};
	struct Node *node;
	STRPTR DeviFilter[]= {
		"8n1.device",
		"absram.device",
		"ahi.device",
		"amia.device",
		"ariadne.device",
		"ariadne_ii.device",
		"artser.device",
		"asimcdfs.device",
		"audio.device",
		"bookmark.device",
		"cardmark.device",
		"cdtv.device",
		"clipboard.device",
		"console.device",
		"Delfpar.device",
		"delfser.device",
		"diskspare.device",
		"empcd.device",
		"fakesr.device",
		"fd.device",
		"file.device",
		"filedisk.device",
		"gameport.device",
		"hackdisk.device",
		"hardfile.device",
		"hyperCOM30Z.device",
		"hyperPAR.device",
		"input.device",
		"keyboard.device",
		"mfm.device",
		"mscd.device",
		"mshf.device",
		"multi-os.device",
		"narrator.device",
		"netinfo.device",
		"parallel.device",
		"parnet.device",
		"PC2AmParallel.device",
		"printer.device",
		"ramdrive.device",
		"serial.device",
		"spool.device",
		"timer.device",
		"trackdisk.device",
		NULL
	};
	STRPTR *filt;
	BOOL found;

	INTRO;

	// currently already already done on init
	//SFXMod_OpenGUILibs;

	
	NewList(&DeviList);			/* Device Liste bauen */
	j=0;
	foreach(node,&SysBase->DeviceList) {									// seek for first name containing 'SCSI'
		filt=DeviFilter;found=FALSE;
		while(*filt && !found) {
			if(!stricmp(*filt,node->ln_Name)) found=TRUE;
			filt++;
		}
		if(!found && j<128) {
			DeviNodes[j].ln_Name=node->ln_Name;
			AddTail(&DeviList,&DeviNodes[j]);
			j++;
		}
	}

	NewList(&UnitList);			/* Unit Liste bauen */
	for(j=0;j<MAX_SCSI_UNITS;j++) {
		sprintf(UnitNames[j],"%02d \t05   \t09 -        \t20 -                \t39 -    \t46 -",j);
		UnitNodes[j].ln_Name=UnitNames[j];
		AddTail(&UnitList,&UnitNodes[j]);
	}

	if(instance->device[0]) {		// there is already a device name supplied
		aktdev=GetIndexByName(&DeviList,instance->device);	// is it still available
		if(aktdev==-1) instance->device[0]='\0';				// not it is not, use auto detect
	}
	if(!instance->device[0]) {		// there is no device name supplied (or device name is not available)
		char *src,*dst;

		aktdev=0;
		foreach(node,&DeviList) {									// seek for first name containing 'SCSI'
			for(src=node->ln_Name,dst=instance->device;*src;++src, ++dst) *dst = toupper(*src);
			*dst='\0';
			if(strstr(instance->device,"SCSI")) break;
			else aktdev++;
		}
		if(EndOfList(&DeviList,node)) aktdev=0;
		else {
			strcpy(instance->device,node->ln_Name);
			instance->unit=SCSIGetUnits(instance,(STRPTR *)UnitNames);	// read in units & autodetect cd-roms
		}
	}
	else {
		LONG unit;
		unit=SCSIGetUnits(instance,(STRPTR *)UnitNames);		// read in units & autodetect cd-roms
		if(!instance->unit) instance->unit=unit;
	}

	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[130]);
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	=GetString(btnLoad);
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[275]);
		ng.ng_GadgetText	=GetString(menPrefsUse);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[5]);
		ng.ng_TopEdge		=ys1+GetRunTime(scy[2+6])+6;
		ng.ng_Width			=GetRunTime(scx[120]);
		ng.ng_Height		=GetRunTime(scy[88]);
		ng.ng_GadgetText	="Devices";
		ng.ng_Flags			=PLACETEXT_ABOVE;
		ng.ng_GadgetID		=GADID_DEVICE;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,GTLV_Labels,&DeviList,GTLV_Selected,aktdev,GTLV_Top,aktdev,GTLV_ShowSelected,0l,TAG_DONE);
		modGadgets[GADIX_DEVICE]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[130]);
		ng.ng_Width			=GetRunTime(scx[365]);
		ng.ng_GadgetText	="Unit";
		ng.ng_GadgetID		=GADID_UNIT;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
			GTLV_Labels,&UnitList,
			GTLV_Selected,instance->unit,
			GTLV_Top,0,
			GTLV_ShowSelected,0l,
			GTLV_CallBack,GetRunTime(tablelvhook),
			GTLV_MaxPen,7,
		TAG_DONE);
		modGadgets[GADIX_UNIT]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[75]);
		ng.ng_TopEdge		=ys1+GetRunTime(scy[4+6+88])+6;
		ng.ng_Width			=GetRunTime(scx[220]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="Method";
		ng.ng_Flags			=PLACETEXT_LEFT;
		ng.ng_GadgetID		=GADID_METHOD;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,MethLabels,GTCY_Active,instance->method,TAG_DONE);
		modGadgets[GADIX_METHOD]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[395]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_GadgetText	="BufferMemory";
		ng.ng_GadgetID		=GADID_MEMORY;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,MemoLabels,GTCY_Active,instance->memory,TAG_DONE);
		modGadgets[GADIX_MEMORY]=g;

		if(g) {
			SFXMod_Interface_FReqInit
			SFXMod_Interface_WinInit

			//SetAPen(modrp,7);
			//Move(modrp,xs1+GetRunTime(scx[5]),ys1+GetRunTime(scy[6]));Text(modrp,"Layer I & II",12);
			//Move(modrp,xs1+GetRunTime(scx[5]),ys2+GetRunTime(scy[6]));Text(modrp,"Layer III",9);

			xh=GetRunTime(scx[500]);
			DrawGadWB(modrp,xs1,ys1,xh,GetRunTime(scy[98+13])+6);						/* Params */
			DrawGadWB(modrp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(1)]));			/* Go */

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
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		 /* Store actual settings */
									ret=TRUE;runing=FALSE;
									break;
								case GADID_OKAY:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		 /* Store actual settings */
									runing=FALSE;
									break;
								case GADID_DEVICE:
									MSG1("akt device %d",icode);
									strcpy(instance->device,GetNameByIndex(&DeviList,icode));
									instance->unit=SCSIGetUnits(instance,UnitNames);
									GT_SetGadgetAttrs(modGadgets[GADIX_UNIT],modWin,0l,GTLV_Labels,&UnitList,GTLV_Selected,instance->unit,TAG_DONE);
									break;
								case GADID_UNIT:	instance->unit=icode;break;
								case GADID_METHOD:	instance->method=icode;break;											
								case GADID_MEMORY:	instance->memory=icode;break;											
							}
							break;
						case IDCMP_VANILLAKEY:
							switch(icode) {
								case KEY_ENTER:
								case KEY_RETURN:
									SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		 /* Store actual settings */
									ret=TRUE;runing=FALSE;
									break;
								case KEY_ESC:	runing=FALSE;break;
								case KEY_TAB:	/*ActivateGadget(modGadgets[GADIX_L12_M_Q],modWin,0l);*/ break;
								case '?':		Message(strNoID,PRJ_VERS,NULL,0l);break;
							}
							break;
						case IDCMP_CLOSEWINDOW:
							runing=FALSE;
							break;
						case IDCMP_RAWKEY:
							if(icode==RKEY_HELP) ShowHelp("html/nodes/"SFXMod_HelpBase"/"PRJ_NAME".html");
							break;
						case IDCMP_MENUPICK:
							while(icode!=MENUNULL) {
								if(item=ItemAddress(GetRunTime(FXMenu),icode)) {
									switch((ULONG)MENU_USERDATA(item)) {
										case MENID_LOAD:
											MODFReqtags[9].ti_Data=FALSE;
											if(AslRequest(modFRequest,MODFReqtags)) {
												strmfp(fn,modFRequest->fr_Drawer,modFRequest->fr_File);
												LoadSettings(instance,fn);
												RefreshGUI(instance,modGadgets,modWin);
											}
											break;
										case MENID_SAVE:
											MODFReqtags[9].ti_Data=TRUE;
											if(AslRequest(modFRequest,MODFReqtags)) {
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

	OUTRO;
	return(ret);
Error:
	return(FALSE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	register UBYTE i;
	struct TrackData unit;
	ULONG lba,lbn,*badr[CMDS],*blen[CMDS];
	LONG blocks;
	ULONG curlen;
	UBYTE resfail=0;
	UBYTE cnr1=0,cnr2=0,cont=FALSE;
	SInfo *si=pdata->dst[0];

	//-- Toshiba specific
	static struct Tosh_cddamode {
		UBYTE sense_data_length,medium_type;
		UBYTE reserved1;
		UBYTE bdlength;
		ULONG density;   /* = densitycode << 24 */
		ULONG blocklen;
	} tosh_cddamode;
	UBYTE old_bdlength;
	ULONG old_density;

	INTRO;

	if(pdata->fn && *pdata->fn) {
		ULONG TrackSEL[404],*tptr;
		UBYTE track;
		char *fnptr;

		MSG1("filename : [%s]",pdata->fn);
		if(!strcmp(pdata->fn,"?")) {
			MSG("  get tracks #1");
			track=SCSIGetTracks(instance,NULL,NULL,NULL,TrackSEL);
			tptr=&TrackSEL[(track<<2)+0];
			fnptr=pdata->fn;
			// build list of number starting from track until end
			MSG("  build list");
			MSG4("    %2d : %6ld %6ld %6ld",track,tptr[0],tptr[1],tptr[3]);
			while(tptr[0] || tptr[1] || tptr[3]) {
				sprintf(fnptr,"%3d,",track);
				fnptr+=4;track++;
				tptr=&tptr[4];
				//MSG4("    %2d : %6ld %6ld %6ld",track,tptr[0],tptr[1],tptr[3]);
			}
			MSG1("  =[%s]",pdata->fn);
		}
		else {
			unit.track=atoi(pdata->fn);
			MSG("  get tracks #2");
			if(unit.track<100) {
				track=SCSIGetTracks(instance,NULL,NULL,NULL,TrackSEL);
				if(unit.track>=track) {
					unit.startLBA	=TrackSEL[(unit.track<<2)+0];
					unit.endLBA		=TrackSEL[(unit.track<<2)+1];
					unit.channels	=TrackSEL[(unit.track<<2)+3];
					if(unit.startLBA|unit.endLBA|unit.channels) cont=TRUE;
				}
			}
		}
	}
	else cont=GetCDDAUnit(instance,&unit);

	if(cont) {
		MSG1("reading track #%ld",unit.track);
		lba=unit.startLBA;
		blocks=(unit.endLBA-unit.startLBA);
		lbn=(ULONG)(SCSI_CDDA_SIZES/SCSI_CDDA_LEN);
		si->srat=44100;

		MSG1("  blocks : %ld",blocks);
		MSG1("  lbn    : %ld",lbn);

		if(SCSIDeviceInit(instance)) {
			MSG("  ---> InitDrive begin");
			switch(instance->method) {			// drive specific initialisation
				case METH_PlexSony:
					break;
				case METH_Tosh:
					instance->MySCSICommand[0]->scsi_Flags		=SCSIF_READ|SCSIF_AUTOSENSE;
					instance->MySCSICommand[0]->scsi_CmdLength	=6;
					instance->MySCSICommand[0]->scsi_Data		=(UWORD *)instance->CmdData[0];
					instance->MySCSICommand[0]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
					SCSICmd_6(instance->Cmd[0],SCSI_CD_MODE_SENSE_6,0,1,SCSI_MAX_SIZE_6,0);
					if(SCSICmdSubmit(instance,0)) {
						if(instance->MySCSICommand[0]->scsi_Actual) {
							old_bdlength=((struct Tosh_cddamode*)instance->CmdData[0])->bdlength;
							old_density =((struct Tosh_cddamode*)instance->CmdData[0])->density;
						}
						else { Message(strNoID,"ERROR : No data received at all from ToshibaInit#1!",__FILE__,__LINE__); }
					}
					else { Message(strNoID,"ToshibaInit#1 failed",__FILE__,__LINE__); }
					tosh_cddamode.sense_data_length=tosh_cddamode.medium_type=tosh_cddamode.reserved1=0;
					tosh_cddamode.bdlength=8;
					tosh_cddamode.density =0x82L << 24;
					tosh_cddamode.blocklen=SCSI_CDDA_LEN;
					instance->MySCSICommand[0]->scsi_Flags		=SCSIF_WRITE|SCSIF_AUTOSENSE;
					instance->MySCSICommand[0]->scsi_Data		=(UWORD *)&tosh_cddamode;
					instance->MySCSICommand[0]->scsi_Length		=sizeof(struct Tosh_cddamode);
					SCSICmd_6(instance->Cmd[0],SCSI_CD_MODE_SELECT_6,0,0,sizeof(struct Tosh_cddamode),0);
					instance->Cmd[0][1]=0x10;
					if(SCSICmdSubmit(instance,0)) {
						if(!instance->MySCSICommand[0]->scsi_Actual) {
							Message(strNoID,"ERROR : No data received at all from ToshibaInit#2!",__FILE__,__LINE__);
						}
					}
					else { Message(strNoID,"ToshibaInit#2 failed",__FILE__,__LINE__); }
					break;
			}
			MSG("  ---> InitDrive end");

			for(i=0;i<CMDS;i++) {
				SCSICmd_12(&instance->Cmd[i][2],SCSI_CD_V1_READ_CDDA,0,lba,lbn,0);	// cmd 0
				instance->Cmd[i][10+2]=0;													// subcode;
				instance->MySCSICommand[i]->scsi_Flags		=SCSIF_READ|SCSIF_AUTOSENSE;
				instance->MySCSICommand[i]->scsi_Command	=&instance->Cmd[i][2];
				instance->MySCSICommand[i]->scsi_CmdLength	=12;
				instance->MySCSICommand[i]->scsi_Data		=(UWORD *)instance->CddaData[i];
				instance->MySCSICommand[i]->scsi_Length		=SCSI_CDDA_SIZES;				/* can take this much answer data. */
				badr[i]=(ULONG *)&instance->Cmd[i][2+2];
				blen[i]=(ULONG *)&instance->Cmd[i][6+2];
			}
			switch(unit.channels) {
				case 0:		// stereo
					MSG("stereo");
					si->slen=SCSIBC_SPB2*blocks;
					SET_CHANNELS(si,CH_STEREO);							         // there might be also quadro samples
					if(!(AllocSampleMem(si,si->slen,CH_COUNT(si->channels)))) resfail=1;
					if(!resfail) {
						PWData * const pwd=NewProWin();
						if(pwd)	{
							SmpAccess * const dstsa1=SAInit(si,0,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,(SCSI_CDDA_SIZES>>1));	// sequentiell Blöcke konstanter Größe schreiben
							SmpAccess * const dstsa2=SAInit(si,1,0,si->slen  ,VM_MODE_WRITE|VM_MODE_SEQ|VM_MODE_BLK|VM_MODE_CBLK,(SCSI_CDDA_SIZES>>1));	// sequentiell Blöcke konstanter Größe schreiben
							if(dstsa1 && dstsa2) {
								SAFuncPtr const SAWrite=dstsa1->safunc;
								register SAMPLE *dbuf1,*dbuf2;
								ProWinTitle title;
								ULONG recvbytes;
								UBYTE queue=0;
								register UBYTE *datas,*datae;

								MSG1("si->slen               : %ld",si->slen);
								MSG1("reading blocks of size : %ld",SCSI_CDDA_SIZES);
								MSG2("dstsa1->blen,seganz    : %ld %ld",dstsa1->blen,dstsa1->seganz);
								MSG2("dstsa2->blen,seganz    : %ld %ld",dstsa2->blen,dstsa2->seganz);
								sprintf(title,GetString(msgProgressLoad),1,1);
								StartProWin(pwd,&curlen,title,si->slen);				// sets curlen=0;

								*badr[cnr1]=lba;											// start first read
								if(blocks<lbn) { *blen[cnr1]=blocks;blocks=0; }
								else { blocks-=lbn;lba+=lbn; }
								SendIO((struct IORequest *)instance->MyIORequest[cnr1]);queue++;
								cnr1=(cnr1+1)&0x11;

								MSG("after first read");

								if(blocks) {
									*badr[cnr1]=lba;										// start second read
									if(blocks<lbn) { *blen[cnr1]=blocks;blocks=0; }
									else { blocks-=lbn;lba+=lbn; }
									SendIO((struct IORequest *)instance->MyIORequest[cnr1]);queue++;
									cnr1=(cnr1+1)&0x11;
								}

								MSG("after second read");

								dstsa1->seg=dstsa2->seg=0;
								while(queue) {
									dbuf1=(SAMPLE *)dstsa1->buffer1;						// Pointer auf dst-Block
									dbuf2=(SAMPLE *)dstsa2->buffer1;						// Pointer auf dst-Block
									WaitIO((struct IORequest *)instance->MyIORequest[cnr2]);queue--;
									recvbytes=instance->MySCSICommand[cnr2]->scsi_Actual;
									datas=instance->CddaData[cnr2];									// akt Buffer to read
									datae=&datas[recvbytes];
									cnr2=(cnr2+1)&0x11;										// und einer weiter

									if(blocks) {
										*badr[cnr1]=lba;										// start next read
										if(blocks<lbn) { *blen[cnr1]=blocks;blocks=0; }
										else { blocks-=lbn;lba+=lbn; }
										SendIO((struct IORequest *)instance->MyIORequest[cnr1]);queue++;
										cnr1=(cnr1+1)&0x11;
									}
									while(datas<datae) {
										*(dbuf1++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf2++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf1++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf2++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf1++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf2++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf1++)=(*(datas++))|((WORD)(*(datas++))<<8);
										*(dbuf2++)=(*(datas++))|((WORD)(*(datas++))<<8);
									}
									SAWrite(dstsa1,dstsa1->blen);dstsa1->seg++;						// akt Block schreiben
									SAWrite(dstsa2,dstsa2->blen);dstsa2->seg++;						// akt Block schreiben
									if(ProWinAborted(curlen)) {
										while(queue) {
											WaitIO((struct IORequest *)instance->MyIORequest[cnr2]);queue--;
											cnr2=(cnr2+1)&0x11;
										}
										break;					// es wurde abgebrochen
									}
									else curlen+=(recvbytes>>2);
								}
							}
							if(dstsa1) SADone(dstsa1,si);					// Samplezugiffshandles freigeben
							if(dstsa2) SADone(dstsa2,si);					// Samplezugiffshandles freigeben
							DelProWin(pwd);
						}
					}
					break;
				case 1:		// quadro
					MSG("quadro");
					si->slen=SCSIBC_SPB4*blocks;
					si->channels=2;si->aktch=4;si->channelmask=15;			// there might be also quadro samples
					break;
			}
			si->zoomxl=si->slen;
			for(i=0;i<CMDS;i++) {
				instance->MySCSICommand[i]->scsi_Command		=instance->Cmd[i];
			}

			switch(instance->method) {			// drive specific deinitialisation
				case METH_PlexSony:
					break;
				case METH_Tosh:
					tosh_cddamode.bdlength=old_bdlength;
					tosh_cddamode.density =old_density;
					instance->MySCSICommand[0]->scsi_Flags		=SCSIF_WRITE|SCSIF_AUTOSENSE;
					instance->MySCSICommand[0]->scsi_Data		=(UWORD *)&tosh_cddamode;
					instance->MySCSICommand[0]->scsi_Length		=sizeof(struct Tosh_cddamode);
					SCSICmd_6(instance->Cmd[0],SCSI_CD_MODE_SELECT_6,0,0,sizeof(struct Tosh_cddamode),0);
					instance->Cmd[0][1]=0x10;
					SCSICmdSubmit(instance,0);
					break;
			}

			SCSIDeviceDone(instance);
		}
	}
	else return(FALSE);
	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
	CfgIO_ReadConfig(cfg,fname);

	strcpy(instance->device,	CfgIO_ReadString			(cfg,"General"	,"Device"		,"scsi.device"));
	instance->unit=			CfgIO_ReadUnsignedInteger	(cfg,"General"	,"Unit"			,0);
	instance->method=		CfgIO_ReadUnsignedInteger	(cfg,"General"	,"Method"		,0);
	instance->memory=		CfgIO_MatchString(
							CfgIO_ReadString			(cfg,"General"	,"Memory"		,MemoLabels[instance->memory]),MemoLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	CfgIO_WriteString			(cfg,"General"	,"Device"		,instance->device);
	CfgIO_WriteUnsignedInteger	(cfg,"General"	,"Unit"			,instance->unit);
	CfgIO_WriteUnsignedInteger	(cfg,"General"	,"Method"		,instance->method);
	CfgIO_WriteString			(cfg,"General"	,"Memory"		,MemoLabels[instance->memory]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	strcpy(instance->device,"scsi.device");
	instance->unit=0;
	instance->method=0;
	instance->memory=1;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
}

//-- private Stuff

ULONG SCSIStatus(struct SCSICmd *scsi_cmd) {
	return((ULONG)(((scsi_cmd->scsi_Status)&0x3E)>>1));
}

void SCSICmd_6(UBYTE *cmd,UBYTE commandCode,UBYTE lun,UWORD pageCode,UBYTE length,UBYTE control) {
	cmd[0]=commandCode;
	cmd[1]=lun<<5;
	cmd[2]=(UBYTE)((pageCode&0xFF00)>>8);
	cmd[3]=(UBYTE)( pageCode&0x00FF    );
	cmd[4]=length;
	cmd[5]=control;
}

void SCSICmd_10(UBYTE *cmd,UBYTE commandCode,UBYTE lun,ULONG pageCode,UWORD length,UBYTE control) {
	cmd[0]=commandCode;
	cmd[1]=lun<<5;
	cmd[2]=(UBYTE)((pageCode&0xFF000000)>>24);
	cmd[3]=(UBYTE)((pageCode&0x00FF0000)>>16);
	cmd[4]=(UBYTE)((pageCode&0x0000FF00)>> 8);
	cmd[5]=(UBYTE)( pageCode&0x000000FF     );
	cmd[6]=0;
	cmd[7]=(UBYTE)((length&0xFF00)>>8);
	cmd[8]=(UBYTE)( length&0x00FF    );
	cmd[9]=control;
}

void SCSICmd_12(UBYTE *cmd,UBYTE commandCode,UBYTE lun,ULONG pageCode,ULONG length,UBYTE control) {
	cmd[ 0]=commandCode;
	cmd[ 1]=lun<<5;
	cmd[ 2]=(UBYTE)((pageCode&0xFF000000)>>24);
	cmd[ 3]=(UBYTE)((pageCode&0x00FF0000)>>16);
	cmd[ 4]=(UBYTE)((pageCode&0x0000FF00)>> 8);
	cmd[ 5]=(UBYTE)( pageCode&0x000000FF     );
	cmd[ 6]=(UBYTE)((length&0xFF000000)>>24);
	cmd[ 7]=(UBYTE)((length&0x00FF0000)>>16);
	cmd[ 8]=(UBYTE)((length&0x0000FF00)>> 8);
	cmd[ 9]=(UBYTE)( length&0x000000FF     );
	cmd[10]=0;
	cmd[11]=control;
}

/******************************************************************************
 * Opens the scsi.device driver and sets up the related data structures.
 * Returns TRUE if successful.
 */

BOOL SCSIDeviceInit(struct Instance *instance) {
	register UBYTE i;
	int ErrorCode;

	INTRO;

	if(instance->memok) {
		for(i=0;i<CMDS;i++) {
			if(  ErrorCode		=OpenDevice(instance->device,instance->unit,(struct IORequest *)instance->MyIORequest[i],0L)) {
			    switch(ErrorCode) {
					case HFERR_NoBoard:
						MSG2("OpenDevice for %s unit %d failed : the is no board to serve unit",instance->device,instance->unit);
						break;
					case HFERR_SelfUnit:
						MSG2("OpenDevice for %s unit %d failed : unit is host-adapter",instance->device,instance->unit);
						break;
					//case GVP_NOUNIT:
						//MSG2("is unused.");
						//break;
					default:
						MSG3("OpenDevice for %s unit %d failed, code %d.",instance->device,instance->unit,ErrorCode);
						break;
				}
				instance->DeviceOpen[i]=FALSE;
				goto Error;
			}
			else instance->DeviceOpen[i]=TRUE;
		}

		OUTRO;

		return(TRUE);
	}
Error:
	SCSIDeviceDone(instance);
	return(FALSE);
}

void SCSIDeviceDone(struct Instance *instance) {
	register UBYTE i;

	INTRO;

	for(i=0;i<CMDS;i++) {
		if(instance->DeviceOpen[i])	{ CloseDevice((struct IORequest *)instance->MyIORequest[i]);instance->DeviceOpen[i]=FALSE; }
	}
	OUTRO;
}

/******************************************************************************
 * Submits the SCSI command in MyIORequest/MySCSICommand to the scsi.device
 * and prints out any resulting errors.  Returns TRUE if no errors.
 */

BOOL SCSICmdSubmit(struct Instance *instance,UBYTE cnr) {
	int ErrorCode;
	//register UWORD i;
	UBYTE stat;
#ifdef DEBUG
	static UBYTE *strStatus  []={ "Good","Check Condition","Condition Met","Busy","Intermediate","Intermediate Condition Met","Reservation Conflict","Command Terminated","Queue Full" };
	static UBYTE *strSenseKey[]={ "No Sense","Recovered Error","Not Ready","Medium Error","Hardware Error","Illegal Request","Unit Attention","-","-","-","-","Aborted Command","-","-","-","-" };
#endif

	instance->MySCSICommand[cnr]->scsi_Status		=0;	/* In case a *.device IO error happens. */
	instance->MySCSICommand[cnr]->scsi_SenseActual	=0;	/* Some scsi drivers don't set this. */

	//MSG1("before DoIO : %ld",MyIORequest[cnr]->io_Message.mn_Node.ln_Type);
	instance->MyIORequest[cnr]->io_Message.mn_Node.ln_Type=0;
	if(!(ErrorCode=DoIO(((struct IORequest *)instance->MyIORequest[cnr])))) {
		//MSG1("after  DoIO : %ld",MyIORequest[cnr]->io_Message.mn_Node.ln_Type);
		if((stat=SCSIStatus(instance->MySCSICommand[cnr]))!=SCSIStat_Good) {
#ifdef DEBUG
			ERR2("ERROR : SCSI command failed, SCSI error code 0x%08lx : %s",(ULONG)stat,strStatus[stat]);
			if(instance->MySCSICommand[cnr]->scsi_SenseActual>0) {
				MSG1("  error code        : 0x%08lx",(int)(instance->MySCSICommand[cnr]->scsi_SenseData[0]&0x7F));
				MSG2("  sense key         : 0x%04lx : %s",(int)(instance->MySCSICommand[cnr]->scsi_SenseData[2]&0x0F),strSenseKey[(instance->MySCSICommand[cnr]->scsi_SenseData[2]&0x0F)]);
				//printf("  information bytes : ");for(i= 3;i< 7;i++) printf("0x%02x ",(int)MySCSICommand[cnr]->scsi_SenseData[i]);putchar('\n');
				MSG1("  add. sense length : %ld",(int)instance->MySCSICommand[cnr]->scsi_SenseData[7]);
				//printf("  cmd. specific     : ");for(i= 8;i<11;i++) printf("0x%02x ",(int)MySCSICommand[cnr]->scsi_SenseData[i]);putchar('\n');
				MSG2("  ASC,ASCQ          : 0x%08lx, 0x%08lx",(int)instance->MySCSICommand[cnr]->scsi_SenseData[12],(int)instance->MySCSICommand[cnr]->scsi_SenseData[13]);
			}
#endif
			return(FALSE);
		}
	}
	else {
		switch(instance->MyIORequest[cnr]->io_Error) {
			case HFERR_SelTimeout:
				ERR("ERROR : DoIO returned HFERR_SelTimeout.\n");
				break;
			case HFERR_BadStatus:
				ERR("ERROR : DoIO returned HFERR_BadStatus.\n");
				break;
			default:
				ERR1("ERROR : DoIO returned code %d.\n",ErrorCode);
				break;
		}
		return(FALSE);	
	}
	return(TRUE);
}

LONG SCSIGetUnits(struct Instance *instance,STRPTR *names) {
	UBYTE pdt,UnitNumber,oldUnit;
	LONG ret=-1;

	INTRO;

	if(instance->device && instance->device[0]) {
		oldUnit=instance->unit;
		MSG1("  evaluating units for %s",instance->device);
		for(UnitNumber=0;UnitNumber<MAX_SCSI_UNITS;UnitNumber++) {
			MSG1("  evaluating unit %1ld",UnitNumber);
			instance->unit=UnitNumber;
			if(SCSIDeviceInit(instance)) {
				instance->MySCSICommand[0]->scsi_Flags		=SCSIF_READ|SCSIF_AUTOSENSE;
				SCSICmd_6(instance->Cmd[0],SCSI_INQUIRY,0,0,SCSI_MAX_SIZE_6,0);
				instance->MySCSICommand[0]->scsi_CmdLength	=6;
				instance->MySCSICommand[0]->scsi_Data		=(UWORD *)instance->CmdData[0];
				instance->MySCSICommand[0]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
				if(SCSICmdSubmit(instance,0)) {
					if(instance->MySCSICommand[0]->scsi_Actual) {
						pdt=instance->CmdData[0][ 0]&0x1F;
						// DEBUG
						//printf("  Vendor Identification  : ");for(i= 8;i<16;i++) putchar(CmdData[0][i]);putchar('\n');
						//printf("  Product Identification : ");for(i=16;i<32;i++) putchar(CmdData[0][i]);putchar('\n');
						//printf("  Product Revision Level : ");for(i=32;i<36;i++) putchar(CmdData[0][i]);putchar('\n');
						//printf("  Firmware Timestamp     : ");for(i=36;i<56;i++) putchar(CmdData[0][i]);putchar('\n');
						// DEBUG
						strncpy(&names[UnitNumber][13],&instance->CmdData[0][ 8], 8);
						strncpy(&names[UnitNumber][26],&instance->CmdData[0][16],16);
						strncpy(&names[UnitNumber][47],&instance->CmdData[0][32], 4);
						strncpy(&names[UnitNumber][56],&instance->CmdData[0][36],20);
						names[UnitNumber][65]='\0';
						if(pdt==SCSIDT_CD_ROM) {
							names[UnitNumber][8]='*';
							if(ret==-1) ret=UnitNumber;
						}
						else names[UnitNumber][8]=' ';
						//SCSICmd_6(Cmd[0],SCSI_TEST_UNIT_READY,0,0,0,0);
						//MySCSICommand[0]->scsi_CmdLength	=6;
						//MySCSICommand[0]->scsi_Data			=(UWORD *)CmdData[0];
						//MySCSICommand[0]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
						//if(SCSICmdSubmit(instance,0)) {
							//printf("  Medium Available\n");
						//}
					}
					else {
						sprintf(names[UnitNumber],"%02d \t05   \t09 -        \t20 -                \t39 -    \t46 -",UnitNumber);
						Message(strNoID,"ERROR : No data received at all from the 'inquiry' command!",__FILE__,__LINE__);
					}
				}
				else {
					sprintf(names[UnitNumber],"%02d \t05   \t09 -        \t20 -                \t39 -    \t46 -",UnitNumber);
					Message(strNoID,"ERROR : 'inquiry' command failed",__FILE__,__LINE__);
				}
				SCSIDeviceDone(instance);
			}
			//-- not an error (we're in a loop)
			else MSG("  scsi-device init failed");
		}
		instance->unit=oldUnit;
	}
	else { Message(strNoID,"no device selected",__FILE__,__LINE__); }

	OUTRO;
	
	if(ret==-1) ret=0;
	return(ret);
}

LONG SCSIGetTracks(struct Instance *instance,struct List *TrackList,struct Node *TrackNodes,TrackName *TrackNames,ULONG *TrackSEL) {
	UBYTE *CmdData1,CmdData2[SCSI_MAX_SIZE];
	UBYTE pdt;
	ULONG trackix;
	LONG Track=-1;
	BOOL tf=FALSE;

	if(TrackList) NewList(TrackList);		// we are allowed to do this, because no pointer is lost 

	if(SCSIDeviceInit(instance))	{
		MSG("SCSIDevice initialized");
		instance->MySCSICommand[0]->scsi_Flags		  =SCSIF_READ|SCSIF_AUTOSENSE;

		SCSICmd_6(instance->Cmd[0],SCSI_INQUIRY,0,0,SCSI_MAX_SIZE_6,0);
		instance->MySCSICommand[0]->scsi_CmdLength	=6;
		instance->MySCSICommand[0]->scsi_Data		=(UWORD *)instance->CmdData[0];
		instance->MySCSICommand[0]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
		if(SCSICmdSubmit(instance,0)) {
			if(instance->MySCSICommand[0]->scsi_Actual) {
				MSG1("  Received %ld inquiry bytes.\n",(ULONG)instance->MySCSICommand[0]->scsi_Actual);
				pdt=instance->CmdData[0][ 0]&0x1F;
				if(pdt==SCSIDT_CD_ROM) {
					SCSICmd_6(instance->Cmd[0],SCSI_TEST_UNIT_READY,0,0,0,0);
					instance->MySCSICommand[0]->scsi_CmdLength	=6;
					instance->MySCSICommand[0]->scsi_Data		=(UWORD *)instance->CmdData[0];
					instance->MySCSICommand[0]->scsi_Length		=SCSI_MAX_SIZE;			/* can take this much answer data. */
					if(SCSICmdSubmit(instance,0)) {
						MSG("  Medium Available\n");
						SCSICmd_10(instance->Cmd[0],SCSI_CD_READ_TOC,0,0,SCSI_MAX_SIZE_10,0);
						instance->MySCSICommand[0]->scsi_CmdLength	=10;
						if(SCSICmdSubmit(instance,0)) {
							if(instance->MySCSICommand[0]->scsi_Actual) {
								memcpy(CmdData2,instance->CmdData[0],instance->MySCSICommand[0]->scsi_Actual);
								instance->Cmd[0][1]|=2;
								if(SCSICmdSubmit(instance,0)) {
									if(instance->MySCSICommand[0]->scsi_Actual) {
										UBYTE t,*td1,*td2;
										ULONG nfrm,ofrm=0,min,sec,frm;
										char *strCtrl0[]={ "without preemphasis","with preemphasis" };
										char *strCtrl1[]={ "digital copy prohibited","digital copy permited" };
										char *strCtrl3[]={ "stereo","quadro" };

										CmdData1=instance->CmdData[0];

										MSG("  Table Of Contents");
										MSG1("    TOC data length : %ld",(((UWORD)CmdData1[0])<<8|CmdData1[1]));
										MSG1("    first track nr  : %ld",CmdData1[2]);
										MSG1("    last track nr   : %ld",CmdData1[3]);
										//MSG1("    track  adr  abs_addr  min:sec:frm\n");
										td1=&CmdData1[4];td2=&CmdData2[4];
										nfrm=td1[7]+(td1[6]*SCSIUC_FPS)+(td1[5]*SCSIUC_FPM);
										//frm=nfrm-ofrm;
										//min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);
										//sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
										//printf("                          %3d:%3d:%3d\n",min,sec,frm);
										td1=&td1[8];ofrm=nfrm;
										for(t=CmdData1[2],trackix=0;t<=CmdData1[3];t++,trackix++) {
											if(!tf) {				// do we still searching for first audio track ?
												if(!(td2[1]&0x04)) {	// is this an audio track ?
													Track=t-1;tf=TRUE;		// yes, it is
												}
											}
											nfrm=td1[7]+(td1[6]*SCSIUC_FPS)+(td1[5]*SCSIUC_FPM);
											frm=nfrm-ofrm;
											*(TrackSEL++)=(((ULONG)td2[  4])<<24|((ULONG)td2[  5])<<16|((ULONG)td2[  6])<<8|td2[  7]);
											*(TrackSEL++)=(((ULONG)td2[8+4])<<24|((ULONG)td2[8+5])<<16|((ULONG)td2[8+6])<<8|td2[8+7]);
											*(TrackSEL++)=frm;					// frames
											*(TrackSEL++)=(td2[1]&0x08)>>3;		// channels
											min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);
											sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
											if(TrackNames && TrackNodes) {
												if(td2[1]&0x04) sprintf(TrackNames[trackix],"%2d \t05 %02d:%02d:%02d \t16 data",td2[2],min,sec,frm);
												else			sprintf(TrackNames[trackix],"%2d \t05 %02d:%02d:%02d \t16 audio %s,%s,%s",td2[2],min,sec,frm,strCtrl0[(td2[1]&0x01)],strCtrl1[(td2[1]&0x02)>>1],strCtrl3[(td2[1]&0x08)>>3]);
												AddTail(TrackList,&TrackNodes[trackix]);
											}
											td1=&td1[8];td2=&td2[8];ofrm=nfrm;
										}
									}			
									else { Message(strNoID,"ERROR : No data received at all from the 'read toc' command!",__FILE__,__LINE__); }
								}
							}			
							else { Message(strNoID,"ERROR : No data received at all from the 'read toc' command!",__FILE__,__LINE__); }
						}
						else { Message(strNoID,"ERROR : 'read toc' command failed!",__FILE__,__LINE__); }
					}
					//-- no error
					else MSG("  No Medium Available");
				}
				else {
					char msg[500];
					sprintf(msg,"ERROR : device has to be a cd-rom (type: %d!=%d (SCSIDT_CD_ROM))!",pdt,SCSIDT_CD_ROM);
					Message(strNoID,msg,__FILE__,__LINE__); 
				}
			}
			else { Message(strNoID,"ERROR : No data received at all from the 'inquiry' command!",__FILE__,__LINE__); }
		}
		SCSIDeviceDone(instance);
	}
	*(TrackSEL++)=0;*(TrackSEL++)=0;*(TrackSEL++)=0;*(TrackSEL++)=0;
	return(Track);
}

/* GetCDDAUnit requester
*/

UBYTE GetCDDAUnit(struct Instance *instance,struct TrackData *unit) {
	register UBYTE i;
	struct NewGadget ng;
	struct Window *modWin;
	struct Gadget *g;
	struct Gadget *modGList=NULL;
	struct Gadget *modGadgets[LAST_GADIX];
	struct RastPort *modrp;
	struct IntuiMessage *imsg;
	ULONG iclass;
	APTR iadr;
	USHORT icode;
	UBYTE ret=FALSE,runing=TRUE,scroll=FALSE;
	UWORD ys1=2;
	UWORD ys2=ys1+1+GetRunTime(scy[98])+6;
	UWORD ys3=ys2+1+GetRunTime(scy[FRAME_HEIGHT(3)]);
	UWORD winh=ys3+2+GetRunTime(scy[FRAME_HEIGHT(1)]);
	UWORD xs1=2;
	UWORD winw=xs1+3+GetRunTime(scx[500]);
	UWORD xh;
	struct TagItem wintags[]= {
		WA_Left,			0,
		WA_Top,				0,
		WA_InnerWidth,		0,
		WA_InnerHeight,		0,
		WA_IDCMP,			BUTTONIDCMP|LISTVIEWIDCMP|SLIDERIDCMP|IDCMP_VANILLAKEY|IDCMP_RAWKEY|IDCMP_CLOSEWINDOW|IDCMP_MENUPICK|IDCMP_ACTIVEWINDOW|IDCMP_DISKINSERTED|IDCMP_DISKREMOVED,
		WA_Flags,			WFLG_SMART_REFRESH|WFLG_ACTIVATE|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_GIMMEZEROZERO|WFLG_NEWLOOKMENUS,
		WA_Gadgets,			0l,
		WA_Title,			(ULONG)PRJ_NAME,
		WA_ScreenTitle,		(ULONG)PRJ_VERS,
		WA_PubScreenName,	(ULONG)SFX_PUBSCRNAME,
		WA_BlockPen,		1,
		TAG_DONE
	};
	
	struct List TrackList;
	struct Node TrackNodes[100]={0};
	TrackName TrackNames[100]={0};
	ULONG TrackSEL[404],TrackL;		// track start, end and length
	ULONG rs,re,rl;					// range
	ULONG min,sec,frm;				// conversion
	char trackstr[10];
	UBYTE sh;

	for(i=0;i<100;i++) TrackNodes[i].ln_Name=TrackNames[i];
	unit->track=SCSIGetTracks(instance,&TrackList,TrackNodes,TrackNames,TrackSEL);

	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge		=xs1+GetRunTime(scx[130]);
		ng.ng_TopEdge		=ys3+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[100]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	=GetString(btnLoad);
		ng.ng_TextAttr		=GetRunTime(ScrFontB);
		ng.ng_GadgetID		=GADID_GO;
		ng.ng_Flags			=PLACETEXT_IN;
		ng.ng_VisualInfo	=GetRunTime(VisualInfo);
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);
		modGadgets[GADIX_GO]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[275]);
		ng.ng_GadgetText	=GetString(menPrefsUse);
		ng.ng_TextAttr		=GetRunTime(ScrFont);
		ng.ng_GadgetID		=GADID_OKAY;
		g=CreateGadget(BUTTON_KIND,g,&ng,TAG_DONE);

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[5]);					// Trackliste
		ng.ng_TopEdge		=ys1+GetRunTime(scy[2+6])+6;
		ng.ng_Width			=GetRunTime(scx[490]);
		ng.ng_Height		=GetRunTime(scy[88]);
		ng.ng_GadgetText	="Tracks";
		ng.ng_Flags			=PLACETEXT_ABOVE;
		ng.ng_GadgetID		=GADID_TRACK;
		g=CreateGadget(LISTVIEW_KIND,g,&ng,
			GTLV_Labels,0l,
			GTLV_Selected,0,
			GTLV_Top,0,
			GTLV_ShowSelected,0l,
			GTLV_CallBack,GetRunTime(tablelvhook),
			GTLV_MaxPen,7,
		TAG_DONE);
		modGadgets[GADIX_TRACK]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[50]);					// Rangestart
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2]);
		ng.ng_Width			=GetRunTime(scx[94]);
		ng.ng_Height		=GetRunTime(scy[11]);
		ng.ng_GadgetText	="Start";
		ng.ng_GadgetID		=GADID_START_ST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,0l,GTST_MaxChars,8,TAG_DONE);
		modGadgets[GADIX_START_ST]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[145]);
		ng.ng_Width			=GetRunTime(scx[350]);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_START_SL;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,0,GTSL_Level,0,GTSL_MaxPixelLen,0,PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		modGadgets[GADIX_START_SL]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[50]);					// Rangeend
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2+12]);
		ng.ng_Width			=GetRunTime(scx[94]);
		ng.ng_GadgetText	="End";
		ng.ng_GadgetID		=GADID_END_ST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,0l,GTST_MaxChars,8,TAG_DONE);
		modGadgets[GADIX_END_ST]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[145]);
		ng.ng_Width			=GetRunTime(scx[350]);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_END_SL;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,0,GTSL_Level,0,GTSL_MaxPixelLen,0,PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		modGadgets[GADIX_END_SL]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[50]);					// Rangelength
		ng.ng_TopEdge		=ys2+GetRunTime(scy[2+24]);
		ng.ng_Width			=GetRunTime(scx[94]);
		ng.ng_GadgetText	="Length";
		ng.ng_GadgetID		=GADID_LENGTH_ST;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(STRING_KIND,g,&ng,GTST_String,0l,GTST_MaxChars,8,TAG_DONE);
		modGadgets[GADIX_LENGTH_ST]=g;

		ng.ng_LeftEdge		=xs1+GetRunTime(scx[145]);
		ng.ng_Width			=GetRunTime(scx[350]);
		ng.ng_GadgetText	="";
		ng.ng_GadgetID		=GADID_LENGTH_SL;
		g=CreateGadget(SLIDER_KIND,g,&ng,GTSL_Min,0,GTSL_Max,0,GTSL_Level,0,GTSL_MaxPixelLen,0,PGA_Freedom,LORIENT_HORIZ,GA_RelVerify,TRUE,GA_Immediate,TRUE,TAG_DONE);
		modGadgets[GADIX_LENGTH_SL]=g;

		if(g) {
			wintags[0].ti_Data=(GetRunTime(xres)-winw)>>1;
			wintags[1].ti_Data=(GetRunTime(yres)-winh)>>1;
			wintags[2].ti_Data=winw;
			wintags[3].ti_Data=winh;
			wintags[6].ti_Data=(ULONG)modGList;
			if(modWin=OpenWindowTagList(0l,wintags)) {
				GT_RefreshWindow(modWin,0l);
				modrp=modWin->RPort;SetFont(modrp,GetRunTime(scrfont));

				//SetAPen(modrp,7);
				//Move(modrp,xs1+GetRunTime(scx[5]),ys1+GetRunTime(scy[6]));Text(modrp,"Layer I & II",12);
				//Move(modrp,xs1+GetRunTime(scx[5]),ys2+GetRunTime(scy[6]));Text(modrp,"Layer III",9);

				xh=GetRunTime(scx[500]);
				DrawGadWB(modrp,xs1,ys1,xh,GetRunTime(scy[98])+6);						/* Params */
				DrawGadWB(modrp,xs1,ys2,xh,GetRunTime(scy[FRAME_HEIGHT(3)]));			/* Params */
				DrawGadWB(modrp,xs1,ys3,xh,GetRunTime(scy[FRAME_HEIGHT(1)]));			/* Go */

				if(unit->track>-1) {
					GT_SetGadgetAttrs(modGadgets[GADIX_GO       ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
					TrackL=TrackSEL[(unit->track<<2)+2];
					GT_SetGadgetAttrs(modGadgets[GADIX_TRACK    ],modWin,0l,GTLV_Labels,&TrackList,GTLV_Selected,unit->track,GA_Disabled,FALSE,TAG_DONE);
					rs=0;
					//frm=0;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
					sprintf(trackstr,"00:00:00");
					GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
					re=frm=(TrackL-1);min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
					sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
					GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
					rl=frm=TrackL;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
					sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
					GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
					sh=0;while((TrackL>>sh)>32767) sh++;
					GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(rs>>sh),GA_Disabled,FALSE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(re>>sh),GA_Disabled,FALSE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,( TrackL   >>sh),GTSL_Level,(rl>>sh),GA_Disabled,FALSE,TAG_DONE);
				}
				else {
					GT_SetGadgetAttrs(modGadgets[GADIX_GO       ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_TRACK    ],modWin,0l,GTLV_Labels,0L,GTLV_Selected,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
					GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
				}

				while(runing) {
					WaitPort(modWin->UserPort);
					while(imsg=GT_GetIMsg(modWin->UserPort)) {
						iclass	=imsg->Class;
						iadr	=imsg->IAddress;
						icode	=imsg->Code;
						GT_ReplyIMsg(imsg);
						switch(iclass) {
							case IDCMP_ACTIVEWINDOW:
								WindowToFront(modWin);
								break;
							case IDCMP_DISKINSERTED:
								MSG2("ins disk : 0x%08lx 0x%08lx",iadr,icode);
								// first just check for medium-present
								unit->track=SCSIGetTracks(instance,&TrackList,TrackNodes,TrackNames,TrackSEL);
								if(unit->track>-1) {
									GT_SetGadgetAttrs(modGadgets[GADIX_GO       ],modWin,0l,GA_Disabled,FALSE,TAG_DONE);
									TrackL=TrackSEL[(unit->track<<2)+2];
									GT_SetGadgetAttrs(modGadgets[GADIX_TRACK    ],modWin,0l,GTLV_Labels,&TrackList,GTLV_Selected,unit->track,GA_Disabled,FALSE,TAG_DONE);
									rs=0;
									//frm=0;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
									sprintf(trackstr,"00:00:00");
									GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
									re=frm=(TrackL-1);min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
									sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
									GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
									rl=frm=TrackL;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
									sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
									GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,trackstr,GA_Disabled,FALSE,TAG_DONE);
									sh=0;while((TrackL>>sh)>32767) sh++;
									GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(rs>>sh),GA_Disabled,FALSE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(re>>sh),GA_Disabled,FALSE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,( TrackL   >>sh),GTSL_Level,(rl>>sh),GA_Disabled,FALSE,TAG_DONE);
								}
								else {
									GT_SetGadgetAttrs(modGadgets[GADIX_GO       ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_TRACK    ],modWin,0l,GTLV_Labels,0L,GTLV_Selected,0,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
									GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
								}
								break;
							case IDCMP_DISKREMOVED:		// check if it is our current cd before
								MSG2("rem disk : 0x%08lx 0x%08lx",iadr,icode);
								// first just check for not medium-present
								GT_SetGadgetAttrs(modGadgets[GADIX_GO       ],modWin,0l,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_TRACK    ],modWin,0l,GTLV_Labels,0L,GTLV_Selected,0,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,0,GTSL_Level,0,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
								GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,0L,GA_Disabled,TRUE,TAG_DONE);
								break;
							case IDCMP_GADGETUP:
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_GO:
										//SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store current settings */
										unit->startLBA	=TrackSEL[(unit->track<<2)]+rs;
										unit->endLBA	=TrackSEL[(unit->track<<2)]+re;
										unit->channels	=TrackSEL[(unit->track<<2)+3];
										ret=TRUE;runing=FALSE;
										break;
									case GADID_OKAY:
										//SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
										runing=FALSE;
										break;
									case GADID_TRACK:
										unit->track=icode;
										TrackL=TrackSEL[(unit->track<<2)+2];
										rs=0;re=TrackL-1;rl=TrackL;
										sh=0;while((TrackL>>sh)>32767) sh++;
										GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(rs>>sh),GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Max,((TrackL-1)>>sh),GTSL_Level,(re>>sh),GA_Disabled,FALSE,TAG_DONE);
										GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Max,( TrackL   >>sh),GTSL_Level,(rl>>sh),GA_Disabled,FALSE,TAG_DONE);
										frm=rs;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
										sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
										GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,trackstr,TAG_DONE);
										frm=re;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
										sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
										GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,trackstr,TAG_DONE);
										frm=rl;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
										sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
										GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,trackstr,TAG_DONE);
										break;
									case GADID_START_ST:
										sscanf(((struct StringInfo *)(modGadgets[GADIX_START_ST]->SpecialInfo))->Buffer,"%02ld:%02ld:%02ld",&min,&sec,&frm);
										rs=frm+(sec*SCSIUC_FPS)+(min*SCSIUC_FPM);
										if(rs>(TrackL-1)) { rs=re=(TrackL-1);rl=0; }
										else {
											if(rs+rl>(TrackL-1)) { re=(TrackL-1);rl=re-rs; }
											else re=rs+rl;
										}
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										break;
									case GADID_START_SL:
										rs=(icode<<sh);
										if(rs+rl>(TrackL-1)) { re=(TrackL-1);rl=re-rs; }
										else re=rs+rl;
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										scroll=FALSE;
										break;
									case GADID_END_ST:
										sscanf(((struct StringInfo *)(modGadgets[GADIX_END_ST]->SpecialInfo))->Buffer,"%02ld:%02ld:%02ld",&min,&sec,&frm);
										re=frm+(sec*SCSIUC_FPS)+(min*SCSIUC_FPM);
										if(re>(TrackL-1)) { re=(TrackL-1);rl=re-rs; }
										else {
											if(re<rl) rl=re-rs;
											else rs=re-rl;
										}
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										break;
									case GADID_END_SL:
										re=(icode<<sh);
										if(re<rl) rl=re-rs;
										else rs=re-rl;
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										scroll=FALSE;
										break;
									case GADID_LENGTH_ST:
										sscanf(((struct StringInfo *)(modGadgets[GADIX_LENGTH_ST]->SpecialInfo))->Buffer,"%02ld:%02ld:%02ld",&min,&sec,&frm);
										rl=frm+(sec*SCSIUC_FPS)+(min*SCSIUC_FPM);
										if(rs+rl>(TrackL-1)) {
											re=TrackL-1;
											if(re<rl) { rs=0;rl=re-rs; }
											else rs=re-rl;
										}
										else re=rs+rl;
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										break;
									case GADID_LENGTH_SL:
										rl=(icode<<sh);
										if(rs+rl>(TrackL-1)) { re=TrackL-1;rs=re-rl; }
										else re=rs+rl;
										UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
										scroll=FALSE;
										break;
								}
								break;
							case IDCMP_GADGETDOWN:
								switch((int)(((struct Gadget *)iadr)->GadgetID)) {
									case GADID_START_SL:
									case GADID_END_SL:
									case GADID_LENGTH_SL:
										scroll=TRUE;break;
								}
								break;
							case IDCMP_MOUSEMOVE:
								if(scroll) {
									switch((int)(((struct Gadget *)iadr)->GadgetID)) {
										case GADID_START_SL:
											rs=(icode<<sh);
											if(rs+rl>(TrackL-1)) { re=(TrackL-1);rl=re-rs; }
											else re=rs+rl;
											UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
											break;
										case GADID_END_SL:
											re=(icode<<sh);
											if(re<rl) rl=re-rs;
											else rs=re-rl;
											UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
											break;
										case GADID_LENGTH_SL:
											rl=(icode<<sh);
											if(rs+rl>(TrackL-1)) { re=TrackL-1;rs=re-rl; }
											else re=rs+rl;
											UnitReq_UpdateSEL(modGadgets,modWin,rs,re,rl,sh);
											break;
									}
								}
								break;
							case IDCMP_VANILLAKEY:
								switch(icode) {
									case KEY_ENTER:
									case KEY_RETURN:
										SaveSettings(instance,ENVDIR"loaders/"PRJ_NAME".cfg");		/* Store actual settings */
										ret=TRUE;runing=FALSE;
										break;
									case KEY_ESC:
										runing=FALSE;
										break;
									case KEY_TAB:
										//ActivateGadget(modGadgets[GADIX_L12_M_Q],modWin,0l);
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
						}
					}
				}
				CloseWindow(modWin);
			}
			else Message(errOpenWindow,NULL,__FILE__,__LINE__);
		}
		else Message(errCreateGadgets,NULL,__FILE__,__LINE__);
		FreeGadgets(modGList);
	}
	else Message(errCreateContext,NULL,__FILE__,__LINE__);
	return(ret);
}

void UnitReq_UpdateSEL(struct Gadget *modGadgets[],struct Window *modWin,ULONG rs,ULONG re,ULONG rl,UBYTE sh) {
	ULONG min,sec,frm;
	char trackstr[10];
	
	GT_SetGadgetAttrs(modGadgets[GADIX_START_SL ],modWin,0l,GTSL_Level,(rs>>sh),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_END_SL   ],modWin,0l,GTSL_Level,(re>>sh),TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_SL],modWin,0l,GTSL_Level,(rl>>sh),TAG_DONE);
	frm=rs;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
	sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
	GT_SetGadgetAttrs(modGadgets[GADIX_START_ST ],modWin,0l,GTST_String,trackstr,TAG_DONE);
	frm=re;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
	sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
	GT_SetGadgetAttrs(modGadgets[GADIX_END_ST   ],modWin,0l,GTST_String,trackstr,TAG_DONE);
	frm=rl;min=(ULONG)(frm/SCSIUC_FPM);frm-=(min*SCSIUC_FPM);sec=(ULONG)(frm/SCSIUC_FPS);frm-=(sec*SCSIUC_FPS);
	sprintf(trackstr,"%02d:%02d:%02d",min,sec,frm);
	GT_SetGadgetAttrs(modGadgets[GADIX_LENGTH_ST],modWin,0l,GTST_String,trackstr,TAG_DONE);
}

//-- eof ----------------------------------------------------------------------
