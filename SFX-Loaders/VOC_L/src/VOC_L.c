/****h* VOC/VOC_L.c [4.2] *
*
*  NAME
*    VOC_L.c
*  COPYRIGHT
*    $VER: VOC_L.c 4.2 (12.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    VOC loader for SoundFX
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    12.Sep.2002	V 4.2	most recent version
*    07.Feb.2001	V 4.1	most recent version
*    18.Jun.2000	V 1.2	most recent version
*    13.Sep.1998	V 1.1	most recent version
*    02.Jul.1997	V 1.0	initial version
*  NOTES
*
*******
*/

/****u* _loaders/VOC_L *
*  Function.eng
*    Loads SoundBlaster-VOC samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,ADPCM-8:4,ADPCM-8:3,ADPCM-8:2,A-LAW,µ-LAW)</td></tr>
*    </table>
*    The VOC format was introduced by "Creative Labs", creators of the
*    Soundblaster-cards on the PC. It was created for easy playback from disks
*    and hard disks or CDs giving it a host of advantages. However due to
*    inconsequent planning it became nessecary to 'add' features which slow down
*    handling of this format. Most programs aren't able to read but one
*    (the 1.1 version) of the VOC format. __SFX can read and write all known
*    versions of this format.
*  Funktion.ger
*    Läd SoundBlaster-VOC Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,ADPCM-8:4,ADPCM-8:3,ADPCM-8:2,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dieses Format wurde von der Firma "Creative Labs", dem Hersteller der
*    SoundBlaster-Karten für PCs eingefürt. Das Format ist für das direkte
*    Abspielen der Samples von dem Datenträger ausgelegt und hat in dieser
*    Richtung mehrere Vorteile. Allerdings ist dieses Format etwas inkonsequent
*    geplant worden, so das einige Erweiterungen notwendig wurden, die die
*    Handhabung des Formates sehr erschweren. Die meisten Programme können
*    lediglich die Formatversion 1.1 lesen. __SFX kann alle bekannten Versionen
*    des Formates laden und Speichern.
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

#define VOC_L_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "VOC.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-loaders/"PRJ_NAME"/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

char *LoadVOCDataBlk(SInfo *si,LoadData *ldata,ULONG pack,ULONG len);

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

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
	struct VocHdr vochdr;
	FILE *infile;
	ULONG size,nslen;
	UBYTE quit=0;
	UWORD ver,chkver;
	UBYTE blkhdr,blkx=0;
	struct VocBlk1 vocblk1;
	struct VocBlk2 vocblk2;
	struct VocBlk3 vocblk3;
	struct VocBlk4 vocblk4;
	struct VocBlk5 vocblk5;
	struct VocBlk6 vocblk6;
	struct VocBlk7 vocblk7;
	struct VocBlk8 vocblk8;
	struct VocBlk9 vocblk9;
	LoadData ldata;
	SInfo *si=pdata->dst[0];
	char *sc_fmt;

	if(!(infile=fopen(pdata->fn,"rb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(!fread(&vochdr,sizeof(struct VocHdr),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	vochdr.ID[19]=0;
	if(strcmp(vochdr.ID,"Creative Voice File")) { sprintf(errmsg,GetString(errWrongFiletype),"VOC");Message(errWrongFiletype,errmsg,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	vochdr.BlockLength=LitEnd2BigEnd_uword(vochdr.BlockLength);
	chkver=(((UWORD)vochdr.ChkVersion_MSB<<8)|(UWORD)vochdr.ChkVersion_LSB);
	ver=(((UWORD)vochdr.Version_MSB<<8)|(UWORD)vochdr.Version_LSB);
	if(chkver!=0x1233-ver) { Message(errUnsupportedSubtype,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
	si->slen=0;
	strncpy(ldata.fn,pdata->fn,FILENAME_MAX);ldata.fn[FILENAME_MAX-1]=0;
	while(!quit) {
		if(!fread(&blkhdr,1,1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
		if(blkhdr!=VOC_DATA) blkx=0;
		else {
			if(blkx) {		/* letzter Block war Extendetblock */
				si->srat=256000000/(65536-LitEnd2BigEnd_uword(vocblk8.tc));
				SET_CHANNELS(si,(vocblk8.mode-1));
			}
		}
		// DEBUG
		//MSG1("blk %d",blkhdr);
		// DEBUG
		switch(blkhdr) {
			case VOC_TERMINATOR:
				MSG("BLK 0 : Terminator");
				quit=1;
				break;
			case VOC_DATA:
				MSG("BLK 1 : Data");
				if(!fread(&vocblk1,sizeof(vocblk1),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				size=vocblk1.size[0]|(vocblk1.size[1]<<8)|(vocblk1.size[2]<<16);
				if(!blkx) {
					si->srat=1000000/(256-vocblk1.tc);
					SET_CHANNELS(si,CH_MONO);
				}
				else vocblk1.pack=vocblk8.pack;
				// DEBUG
				//MSG1("\tpack : %d",vocblk1.pack);
				// DEBUG
				ldata.offs=ftell(infile);
				ldata.chmask=CH_ALL;
				ldata.flags=LDF_NONE;
				sc_fmt=LoadVOCDataBlk(si,&ldata,vocblk1.pack,size-VocBlk1Size);
				fseek(infile,size-VocBlk1Size,SEEK_CUR);
				break;
			case VOC_CONTINUATION:
				MSG("BLK 2 : Continuation");
				if(!fread(&vocblk2,sizeof(vocblk2),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				size=vocblk2.size[0]|(vocblk2.size[1]<<8)|(vocblk2.size[2]<<16);
				ldata.offs=ftell(infile);
				ldata.chmask=CH_ALL;
				ldata.flags=LDF_NONE;
				sc_fmt=LoadVOCDataBlk(si,&ldata,vocblk1.pack,size-VocBlk2Size);
				fseek(infile,size-VocBlk2Size,SEEK_CUR);
				break;
			case VOC_SILENCE:
				MSG("BLK 3 : Silence");
				if(!fread(&vocblk3,sizeof(vocblk3),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				nslen=si->slen+LitEnd2BigEnd_uword(vocblk3.period);
				if(AllocSampleMem(si,nslen,CH_COUNT(si->channels))) { Message(errAllocateMemory,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				break;
			case VOC_MARKER:
				MSG("BLK 4 : Marker");
				if(!fread(&vocblk4,sizeof(vocblk4),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				break;
			case VOC_ASCII:
				MSG("BLK 5 : Ascii");
				if(!fread(&vocblk5,sizeof(vocblk5),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				size=vocblk5.size[0]|(vocblk5.size[1]<<8)|(vocblk5.size[2]<<16);
				fseek(infile,(size-VocBlk5Size),SEEK_CUR);
				break;
			case VOC_LOOPSTART:
				MSG("BLK 6 : Loop Start");
				if(!fread(&vocblk6,sizeof(vocblk6),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				si->loops=si->slen;
				si->loop=SFXLOOP_FORWARD;
				break;
			case VOC_LOOPEND:
				MSG("BLK 7 : Loop End");
				if(!fread(&vocblk7,sizeof(vocblk7),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				si->loopl=si->slen-si->loops;
				break;
			case VOC_EXTENDED:
				MSG("BLK 8 : Extended");
				if(!fread(&vocblk8,sizeof(vocblk8),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				blkx=1;
				break;
			case VOC_NEWDATA:
				MSG("BLK 9 : New Data");
				if(!fread(&vocblk9,sizeof(vocblk9),1,infile)) { Message(errReadData,NULL,__FILE__,__LINE__);fclose(infile);return(FALSE); }
				size=vocblk9.size[0]|(vocblk9.size[1]<<8)|(vocblk9.size[2]<<16);
				si->srat=(ULONG)LitEnd2BigEnd_uword(vocblk9.frequency);
				if(!si->srat) { Message(errNoSampleRate,NULL,__FILE__,__LINE__);si->srat=16780; }
				//if(vocblk9.width==8) x_bits=8;
				//if(vocblk9.width==0 || vocblk9.width==16) x_bits=16;
				SET_CHANNELS(si,(vocblk9.channel-1));
				// DEBUG
				//printf("\tpack : %d\n",vocblk9.pack);
				// DEBUG
				ldata.offs=ftell(infile);
				ldata.chmask=CH_ALL;
				ldata.flags=LDF_NONE;
				SHOW_LONG(ldata.offs);
				SHOW_LONG(size-VocBlk9Size);
				sc_fmt=LoadVOCDataBlk(si,&ldata,vocblk9.pack,size-VocBlk9Size);
				fseek(infile,(size-VocBlk9Size),SEEK_CUR);
				break;
		}
	}
	fclose(infile);
	si->zoomxl=si->slen;
	if(!si->slen) return(FALSE);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,sc_fmt);

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

char *LoadVOCDataBlk(SInfo *si,LoadData *ldata,ULONG pack,ULONG len) {
	ULONG slen=si->slen;
	char *fmt=NULL;

	switch(pack) {
		case COMPVOC_PCM8:
			fmt=STR_PCM8;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM8,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_ADPCM8_4:
			fmt=STR_VOC_ADPCM8_4;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_ADPCM8_3:
			fmt=STR_VOC_ADPCM8_3;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_ADPCM8_2:
			fmt=STR_VOC_ADPCM8_2;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_PCM16:
			fmt=STR_PCM16;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_PCM16,CONV(ENDIAN_L_B,0),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_ULAW:
			fmt=STR_ULAW;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_ULAW,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_ULAW,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_ULAW,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
		case COMPVOC_ALAW:
			fmt=STR_ALAW;
			switch(si->channels) {
				case CH_MONO:	slen+=ReadData(si,ldata,len,CRUNCH_ALAW,CONV(0,SIGNUM_U_S),CHANNEL_MONO);break;
				case CH_STEREO:	slen+=ReadData(si,ldata,len,CRUNCH_ALAW,CONV(0,SIGNUM_U_S),CHANNEL_STEREOI);break;
				case CH_QUADRO:	slen+=ReadData(si,ldata,len,CRUNCH_ALAW,CONV(0,SIGNUM_U_S),CHANNEL_QUADROI);break;
			}
			break;
	}
	si->slen=slen;
	return(fmt);
}

//-- eof ----------------------------------------------------------------------
