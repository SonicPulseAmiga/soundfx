/****h* Studio16/Studio16_L.c [4.2] *
*
*  NAME
*    Studio16_L.c
*  COPYRIGHT
*    $VER: Studio16_L.c 4.2 (29.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Studio16 loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    29.May.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    02.Aug.2000	V 1.2	most recent version
*    13.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/Studio16_L *
*  Function.eng
*    Loads Studio16 samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/[quadro])</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-16)</td></tr>
*    </table>
*    Those samples are used with the Studio16 Software, which is bundled with
*    soundcards of the company Surrize.<br>
*    Many thanks to Kenneth "Kenny" Nilsen for his work and help.
*  Funktion.ger
*    Läd Studio16 Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/[quadro])</td></tr>
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
*    __SFX offers a workaround for it. Just save the separate channels in studio16 as
*    name_l.ext and name_r.ext for stereo (where name is the filename and ext is the extension)
*    and name_l.ext, name_r.ext, name_f.ext and name_b.ext for quadro.
*    Then load one of them into __SFX. This loader will then look for the other channels
*    and load them as well.
*  Hinweise.ger
*    Dieses Format unterstützt keine Mehrkanal-Samples  (Stereo oder Quadro).
*    __SFX bietet dafür eine Lösung. Speichern sie die Einzelkanäle in studio16 als
*    name_l.ext und name_r.ext bei Stereo (wobei name der Dateiname und ext die Dateierweiterung sind)
*    und name_l.ext, name_r.ext, name_f.ext und name_b.ext bei Quadro.
*    Laden sie dann eines der Dateien in __SFX. Dieser Loader sucht dann nach den anderen Kanälen
*    und läd diese mit.
*******
*/

#define Studio16_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "Studio16.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

//-- Menu ID's

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	UBYTE dummy;
};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	return(FALSE);
Error:
	__UserLibCleanup(SFXModBase);
	return(TRUE);
}

void __saveds __asm __UserLibCleanup(REG(a6,struct Library *SFXModBase)) {
	SFXMod_CloseCommonAndGUILibs;
	DONEDEBUG;
}

//-- DataInitialisation and Destruction

SFXMod_DoClientDataInit
SFXMod_DoClientDataDone

//-- Modulroutinen

UBYTE __saveds __asm LIBSFXMod_Interface(REG(a0,struct Instance *instance)) {
	return(TRUE);
}

UBYTE __saveds __asm LIBSFXMod_Process(REG(a0,struct Instance *instance),REG(a1,ProcessData *pdata)) {
	register UBYTE i;
	FILE *infile;
	LoadData ldata1,ldata2;
	ULONG id,slen1,slen2,sl;
	struct SampleFileTag shdr;
	char fn1[FILENAME_MAX],fn2[FILENAME_MAX],fn3[FILENAME_MAX],fn4[FILENAME_MAX],drive[FNSIZE],path[FNSIZE],node[FNSIZE],ext[FESIZE];
	UBYTE ch=CH_MONO;
	SInfo *si=pdata->dst[0];

	strsfn(pdata->fn,drive,path,node,ext);
	sl=strlen(node);
	if(sl>2) {			/* there might be an channel extension */
		node[sl-1]='L';strmfn(fn1,drive,path,node,ext);
		node[sl-1]='R';strmfn(fn2,drive,path,node,ext);
		node[sl-1]='F';strmfn(fn3,drive,path,node,ext);
		node[sl-1]='B';strmfn(fn4,drive,path,node,ext);
		if(!stricmp(&node[sl-2],"_L") || !stricmp(&node[sl-2],"_R") || !stricmp(&node[sl-2],"_F") || !stricmp(&node[sl-2],"_B")) {
			if(!access(fn1,R_OK) &&	!access(fn2,R_OK)) {
				ch=CH_STEREO;														// at leat stereo
				if(!access(fn3,R_OK) && !access(fn4,R_OK)) ch=CH_QUADRO;			// even quadro
			}
		}
	}

	switch(ch) {
		case CH_MONO:
			if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			if(!fread(&id,4,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(id!=STUDIO16_KWIK) { sprintf(errmsg,GetString(errWrongFiletype),"Studio16");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(!fread(&shdr, sizeof(shdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			strncpy(ldata1.fn,pdata->fn,FILENAME_MAX);ldata1.fn[FILENAME_MAX-1]=0;
			// DEBUG
			MSG2("ReadSize=%6ld EditSize=%6ld",shdr.params.RealSize,shdr.params.EditSize);
			// DEBUG

			if(shdr.params.Rate) si->srat=shdr.params.Rate;
			else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=44100; }
			slen1=shdr.params.RealSize;
			//slen1=(shdr.dclips[0].End-shdr.dclips[0].Start)+1;
			// DEBUG
			for(i=0;i<5;i++) MSG4("%d : [%6ld...%6ld]=%6ld",i,shdr.dclips[i].Start,shdr.dclips[i].End,(shdr.dclips[i].End-shdr.dclips[i].Start)+1);
			// DEBUG
			if((si->extdata=AllocVec(sizeof(struct SampleFileTag),MEMF_ANY))) {		// remember values like timecode, filter and pan settings
				memcpy(si->extdata,&shdr,sizeof(struct SampleFileTag));si->extlength=sizeof(struct SampleFileTag);
				strcpy(si->extid,"Studio16");
			}

			ldata1.offs=ftell(infile);
			ldata1.chmask=CH_ALL;
			ldata1.flags=LDF_NONE;
			fclose(infile);
			si->zoomxl=si->slen=ReadData(si,&ldata1,(slen1<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_MONO);
			break;
		case CH_STEREO:
			if(!(infile=fopen(fn1,"rb"))) { StdCError(fn1,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			if(!fread(&id,4,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(id!=STUDIO16_KWIK) { sprintf(errmsg,GetString(errWrongFiletype),"Studio16");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(!fread(&shdr, sizeof(shdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			strncpy(ldata1.fn,fn1,FILENAME_MAX);ldata1.fn[FILENAME_MAX-1]=0;
			if(shdr.params.Rate) si->srat=shdr.params.Rate;
			else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=44100; }
			slen1=shdr.params.RealSize;
			//slen1=(shdr.dclips[0].End-shdr.dclips[0].Start)+1;
			if((si->extdata=AllocVec(sizeof(struct SampleFileTag),MEMF_ANY))) {		// remember values like timecode, filter and pan settings
				memcpy(si->extdata,&shdr,sizeof(struct SampleFileTag));si->extlength=sizeof(struct SampleFileTag);
				strcpy(si->extid,"Studio16");
			}
			ldata1.offs=ftell(infile);
			ldata1.chmask=1;
			ldata1.flags=LDF_NOCHOFF;
			fclose(infile);
			MSG1("slen1 : %5d",slen1);

			if(!(infile=fopen(fn2,"rb"))) { StdCError(fn2,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
			if(!fread(&id,4,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(id!=STUDIO16_KWIK) { sprintf(errmsg,GetString(errWrongFiletype),"Studio16");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			if(!fread(&shdr, sizeof(shdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
			strncpy(ldata2.fn,fn2,FILENAME_MAX);ldata2.fn[FILENAME_MAX-1]=0;
			if(shdr.params.Rate) si->srat=shdr.params.Rate;
			else { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=44100; }
			slen2=shdr.params.RealSize;
			//slen2=(shdr.dclips[0].End-shdr.dclips[0].Start)+1;
			//if((si->extdata=AllocVec(sizeof(struct SampleFileTag),MEMF_ANY))) {		// remember values like timecode, filter and pan settings
				//memcpy(si->extdata,&shdr,sizeof(struct SampleFileTag));si->extlength=sizeof(struct SampleFileTag);
				//strcpy(si->extid,"Studio16");
			//}
			ldata2.offs=ftell(infile);
			ldata2.chmask=2;
			ldata2.flags=LDF_NOCHOFF;
			fclose(infile);
			MSG1("slen2 : %5d",slen2);

			SET_CHANNELS(si,CH_STEREO);

			if(slen1>=slen2) {
				si->zoomxl=si->slen=ReadData(si,&ldata1,(slen1<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
				                    ReadData(si,&ldata2,(slen2<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
			}
			else {
				si->zoomxl=si->slen=ReadData(si,&ldata2,(slen2<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
				                    ReadData(si,&ldata1,(slen1<<1),CRUNCH_PCM16,CONV(0,0),CHANNEL_STEREO);
			}
			break;
	}
	if(!si->slen) return(FALSE);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,STR_PCM16);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
}

//-- local Stuff

void LoadSettings(struct Instance *instance,char *fname) {
}

void SaveSettings(struct Instance *instance,char *fname) {
}

void SetDefaultSettings(struct Instance *instance) {
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

//-- private Stuff

//-- eof ----------------------------------------------------------------------
