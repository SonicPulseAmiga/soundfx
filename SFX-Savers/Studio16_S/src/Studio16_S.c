/****h* Studio16/Studio16_S.c [4.3] *
*
*  NAME
*    Studio16_S.c
*  COPYRIGHT
*    $VER: Studio16_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Studio16 saver for SoundFX
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

/****u* _savers/Studio16_S *
*  Function.eng
*    Saves Studio16 samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-16)</td></tr>
*    </table>
*    Those samples are used with the Studio16 Software, which is bundled with
*    soundcards of the company Surrize.<br>
*    Many thanks to Kenneth "Kenny" Nilsen for his work and help.
*  Funktion.ger
*    Speichert Studio16 Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>nein (PCM-16)</td></tr>
*    </table>
*    Solche Samples werden von der Studio16 Software benutzt, welche den Soundkarten
*    der Firma Sunrize beiliegt.<br>
*    Vielen Dank an Kenneth "Kenny" Nilsen für seine Arbeit und Hilfe.
*  Parameter.eng
*    none
*  Parameter.ger
*    keine
*  Notes.eng
*    This format does not support multi-channel-samples (stereo or quadro).
*    __SFX offers a workaround for it. Stereo-samples will be saved as name_l.ext
*    and name_r.ext (where name is the filename and ext is the extension) and
*    quadro-samples as name_l.ext, name_r.ext, name_f.ext and name_b.ext.
*  Hinweise.ger
*    Dieses Format unterstützt keine Mehrkanal-Samples  (Stereo oder Quadro).
*    __SFX bietet dafür eine Lösung. Stereo-Samples werden als name_l.ext
*    und name_r.ext gespeichert (wobei name der Dateiname und ext die Dateierweiterung sind) and
*    Quadro-Samples als name_l.ext, name_r.ext, name_f.ext und name_b.ext.
*******
*/

#define Studio16_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Studio16.h"

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
	struct SampleFileTag shdr;
	FILE *outfile;
	ULONG id;
	char fn2[FILENAME_MAX],drive[FNSIZE],path[FNSIZE],node[FNSIZE],node2[FNSIZE],ext[FESIZE];
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

	strsfn(pdata->fn,drive,path,node,ext);
	if(si->extdata) memcpy(&shdr,si->extdata,sizeof(struct SampleFileTag));			/* are there extra data originating from a studio16 file */
	else {
		shdr.params.Rate				=si->srat;
		shdr.params.Filter3db			=S16_FILTER_INIT;
		shdr.params.Volume				=S16_VOL_NORMAL;
		shdr.params.StartTC.hours		=0;
		shdr.params.StartTC.min			=0;
		shdr.params.StartTC.sec			=0;
		shdr.params.StartTC.frames		=0;
		shdr.params.SmpteSamplingRate	=0.0;
		shdr.params.Pan					=S16_PAN_MID;
		shdr.params.Flags				=S16_FLAG_INIT;
		shdr.params.Reserved[0]			=0;

		shdr.params.RealSize			=si->slen;	/* number of samples (bytesize/2). 		length of samp with no data clips */
		shdr.params.EditSize			=si->slen;	/* number of samples after editlist.	length of samp with using data clips */

		//-- can't we use this for loops ?
		shdr.dclips[0].Start=0;
		shdr.dclips[0].End	=si->slen-1;
		for(i=1;i<DS_NUM_CLIPS;i++) shdr.dclips[i].Start=shdr.dclips[i].End=0;
		memset(&shdr.dregions,0,2624);
	}

	switch(si->channels) {
		case CH_MONO:
			if((outfile=fopen(pdata->fn,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,pdata->fn,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=1;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);
			}
			else { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			break;
		case CH_STEREO:
			sprintf(node2,"%s_L",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=1;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			sprintf(node2,"%s_R",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=2;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			break;
		case CH_QUADRO:
			sprintf(node2,"%s_L",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=1;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			sprintf(node2,"%s_R",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=2;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			sprintf(node2,"%s_F",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=4;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			sprintf(node2,"%s_B",node);
			strmfn(fn2,drive,path,node2,ext);
			if((outfile=fopen(fn2,"wb"))) {
				id=STUDIO16_KWIK;
				if(!fwrite(&id,4,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				if(!fwrite(&shdr,sizeof(struct SampleFileTag),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				strncpy(sdata.fn,fn2,FILENAME_MAX);sdata.fn[FILENAME_MAX-1]=0;
				sdata.offs=ftell(outfile);
				sdata.chmask=8;
				fclose(outfile);
				WriteData(si,&sdata,CRUNCH_PCM16,CONV(0,0),CHANNEL_QUADRO);
			}
			else { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			break;
	}

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
	//no params
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
