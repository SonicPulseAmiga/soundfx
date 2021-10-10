/****h* VOC/VOC_S.c [4.3] *
*
*  NAME
*    VOC_S.c
*  COPYRIGHT
*    $VER: VOC_S.c 4.3 (17.10.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    VOC saver for SoundFX
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

/****u* _savers/VOC_S *
*  Function.eng
*    Saves SoundBlaster-VOC samples.
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
*    Speichert SoundBlaster-VOC Samples.
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
*    Type
*      type of compression
*        PCM8 : not compressed 8bit
*        PCM16 : not compressed 16bit
*        ADPCM8_4 : AdaptiveDelta (2:1) compressed 8bit
*        ADPCM8_3 : AdaptiveDelta (3:1) compressed 8bit
*        ADPCM8_2 : AdaptiveDelta (4:1) compressed 8bit
*        µ-Law : µ-Law (14:8) compressed 14bit
*    Header
*      file format version :
*        1.20 : Use blocktype 9 for soundheader.
*        1.15 : Use blocktype 8 and 1 for soundheader.
*        1.10	: Use blocktype 1 only for soundheader
*      In my experience most programs can't read the new VOC formats. I therefore
*      added the option to save the old format. Using version 1.10 might be the
*      safest. Please take these limitations into account:
*        1.15 : only 8-bit samples
*        1.10 : only mono and 8-bit samples
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        ADPCM8_4 : AdaptiveDelta (2:1) gepackt 8bit
*        ADPCM8_3 : AdaptiveDelta (3:1) gepackt 8bit
*        ADPCM8_2 : AdaptiveDelta (4:1) gepackt 8bit
*        µ-Law : µ-Law (14:8) gepackt 14bit
*    Header
*      Dateiformatversion :
*        1.20 : Es wird der Blocktyp 9 für den Soundheader verwendet.
*        1.15 : Es werden die Blocktypen 8 und 1 für den Soundheader verwendet.
*        1.10 : Es wird nur der Blocktyp 1 verwendet.
*      Ich habe die Erfahrung gemacht, daß die meisten Programme die  neueren
*      VOC-Files nicht lesen. Deshalb habe ich die Möglichkeit offengelassen,
*      auch die älteren Formate zu speichern. Am sichersten ist die Version 1.10.
*      Allerdings sollte mann folgende Einschränkungen bedenken:
*        1.15 : nur 8-bit Samples
*        1.10 : nur Mono und 8-bit Samples
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define VOC_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "VOC.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- private Stuff

void SaveVOCDataBlk(struct Instance *instance,SInfo *si,SaveData *sdata,UBYTE channels);

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
	UBYTE format;				/* bits && compression */
	UBYTE header;				/* 1.10 , 1.15 , 1.20 */

	PrefsData moddesc[2];
};

char *FormLabels[]={STR_PCM8,STR_PCM16,STR_VOC_ADPCM8_4,STR_VOC_ADPCM8_3,STR_VOC_ADPCM8_2,STR_ULAW,STR_ALAW,0l};
char *VersLabels[]={"1.10","1.15","1.20",0l};

SFXMod_Globals

//-- definitions --------------------------------------------------------------

//-- Initialisation and Destruction

int __saveds __asm __UserLibInit(REG(a6,struct Library *SFXModBase)) {
	INITDEBUG;
	SFXMod_OpenCommonLibs;
	if(!(cfg=CfgIO_NewConfig(PRJ_NAME,"$VER: %s "NUM2STR(PRJ_HEADERION)"."NUM2STR(PRJ_REVISION)" (%s) © by Stefan Kost 1993-%s"))) goto Error;
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
		ng.ng_Width			=RunTime->scx[115];
		ng.ng_GadgetText	="Type";
		ng.ng_GadgetID		=GADID_TYPE;
		ng.ng_Flags			=PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_TYPE]=g;

		ng.ng_LeftEdge		=xs1+RunTime->scx[215];
		ng.ng_Width			=RunTime->scx[85];
		ng.ng_GadgetText	="Header";
		ng.ng_GadgetID		=GADID_HEADER;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,VersLabels,GTCY_Active,instance->header,TAG_DONE);
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
	struct VocHdr vochdr;
	FILE *outfile;
	ULONG size;
	UBYTE blkhdr;
	ULONG bufl1,bufl2,bufl3;
	ULONG blk1,blk2,blk3;
	struct VocBlk1 vocblk1;
	struct VocBlk6 vocblk6;
	struct VocBlk7 vocblk7;
	struct VocBlk8 vocblk8;
	struct VocBlk9 vocblk9;
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

	vocblk6.repeat=0xFFFF;		/* infinite loops */

	if(!(outfile=fopen(pdata->fn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }

	strcpy(vochdr.ID,"Creative Voice File");vochdr.ID[19]=10;
	vochdr.BlockLength=LitEnd2BigEnd_uword(26);

	switch(instance->header) {
		case VERSION_1_10:
			vochdr.Version_MSB   =0x01;vochdr.Version_LSB   =0x0A;
			vochdr.ChkVersion_MSB=0x11;vochdr.ChkVersion_LSB=0x29;
			//MSG("VOC-Version 1.10");
			break;
		case VERSION_1_15:
			vochdr.Version_MSB   =0x01;vochdr.Version_LSB   =0x0F;
			vochdr.ChkVersion_MSB=0x11;vochdr.ChkVersion_LSB=0x24;
			//MSG("VOC-Version 1.15");
			break;
		case VERSION_1_20:
			vochdr.Version_MSB   =0x01;vochdr.Version_LSB   =0x14;
			vochdr.ChkVersion_MSB=0x11;vochdr.ChkVersion_LSB=0x1F;
			//MSG("VOC-Version 1.20");
			break;
	}

	if(!fwrite(&vochdr,sizeof(struct VocHdr),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
	if((instance->src.rmode==RNGMD_ALL) && si->loop) {
		//MSG("  with loop");
		blk1=si->loops;						/* 0 bis loop */
		blk2=si->loopl;						/* loop */
		blk3=si->slen-(blk1+blk2);			/* loop bis ende */

		switch(instance->header) {
			case VERSION_1_10:
				bufl1=Samples2Bytes(blk1);
				bufl2=Samples2Bytes(blk2);
				bufl3=Samples2Bytes(blk3);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:
						bufl1=GetCrunchedLength(CRUNCH_PCM8,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_PCM8,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_PCM8,bufl3);
						vocblk1.pack=COMPVOC_PCM8;
						break;
					case FORMAT_VOC_ADPCM8_4:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl3);
						vocblk1.pack=COMPVOC_ADPCM8_4;
						break;
					case FORMAT_VOC_ADPCM8_3:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl3);
						vocblk1.pack=COMPVOC_ADPCM8_3;
						break;
					case FORMAT_VOC_ADPCM8_2:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl3);
						vocblk1.pack=COMPVOC_ADPCM8_2;
						break;
				}
				vocblk1.tc=(UBYTE)(256-(1000000/si->srat));

				if(blk1) {
					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl1+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk1;sdata.savestart=0;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,0);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPSTART;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk6Size;
				vocblk6.size[0]=(UBYTE)(size&0xFF);vocblk6.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk6.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk6,sizeof(vocblk6),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk2) {
					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl2+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk2;sdata.savestart=si->loops;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,0);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPEND;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk7Size;
				vocblk7.size[0]=(UBYTE)(size&0xFF);vocblk7.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk7.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk7,sizeof(vocblk7),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk3) {
					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl3+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk3;sdata.savestart=si->loops+si->loopl;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,0);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}
				break;
			case VERSION_1_15:
				bufl1=Samples2Bytes(blk1);
				bufl2=Samples2Bytes(blk2);
				bufl3=Samples2Bytes(blk3);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:
						bufl1=GetCrunchedLength(CRUNCH_PCM8,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_PCM8,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_PCM8,bufl3);
						vocblk1.pack=vocblk8.pack=COMPVOC_PCM8;
						break;
					case FORMAT_VOC_ADPCM8_4:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl3);
						vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_4;
						break;
					case FORMAT_VOC_ADPCM8_3:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl3);
						vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_3;
						break;
					case FORMAT_VOC_ADPCM8_2:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl3);
						vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_2;
						break;
				}
				bufl1*=(si->channels+1);
				bufl2*=(si->channels+1);
				bufl3*=(si->channels+1);
				vocblk8.tc=LitEnd2BigEnd_uword((UWORD)(65535-(256000000/si->srat)));
				vocblk8.mode=si->channels+1;
				vocblk1.tc=(UBYTE)(256-(1000000/si->srat));

				if(blk1) {
					blkhdr=VOC_EXTENDED;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=VocBlk8Size;
					vocblk8.size[0]=(UBYTE)(size&0xFF);vocblk8.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk8.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk8,sizeof(vocblk8),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl1+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk1;sdata.savestart=0;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPSTART;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk6Size;
				vocblk6.size[0]=(UBYTE)(size&0xFF);vocblk6.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk6.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk6,sizeof(vocblk6),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk2) {
					blkhdr=VOC_EXTENDED;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=VocBlk8Size;
					vocblk8.size[0]=(UBYTE)(size&0xFF);vocblk8.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk8.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk8,sizeof(vocblk8),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl2+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk2;sdata.savestart=si->loops;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPEND;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk7Size;
				vocblk7.size[0]=(UBYTE)(size&0xFF);vocblk7.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk7.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk7,sizeof(vocblk7),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk3) {
					blkhdr=VOC_EXTENDED;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=VocBlk8Size;
					vocblk8.size[0]=(UBYTE)(size&0xFF);vocblk8.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk8.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk8,sizeof(vocblk8),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

					blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl3+VocBlk1Size;
					vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk3;sdata.savestart=si->loops+si->loopl;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}
				break;
			case VERSION_1_20:
				bufl1=Samples2Bytes(blk1);
				bufl2=Samples2Bytes(blk2);
				bufl3=Samples2Bytes(blk3);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:
						bufl1=GetCrunchedLength(CRUNCH_PCM8,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_PCM8,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_PCM8,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_PCM8;
						break;
					case FORMAT_VOC_PCM16:
						bufl1=GetCrunchedLength(CRUNCH_PCM16,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_PCM16,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_PCM16,bufl3);
						vocblk9.width=16;
						vocblk9.pack=COMPVOC_PCM16;
						break;
					case FORMAT_VOC_ADPCM8_4:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_ADPCM8_4;
						break;
					case FORMAT_VOC_ADPCM8_3:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_ADPCM8_3;
						break;
					case FORMAT_VOC_ADPCM8_2:
						bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_ADPCM8_2;
						break;
					case FORMAT_VOC_ULAW:
						bufl1=GetCrunchedLength(CRUNCH_ULAW,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_ULAW,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_ULAW,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_ULAW;
						break;
					case FORMAT_VOC_ALAW:
						bufl1=GetCrunchedLength(CRUNCH_ALAW,bufl1);
						bufl2=GetCrunchedLength(CRUNCH_ALAW,bufl2);
						bufl3=GetCrunchedLength(CRUNCH_ALAW,bufl3);
						vocblk9.width=8;
						vocblk9.pack=COMPVOC_ALAW;
						break;
				}
				bufl1*=(si->channels+1);
				bufl2*=(si->channels+1);
				bufl3*=(si->channels+1);
				vocblk9.frequency=LitEnd2BigEnd_uword((UWORD)si->srat);
				vocblk9.channel=si->channels+1;
				vocblk9.reserved[0]=vocblk9.reserved[1]=vocblk9.reserved[2]=vocblk9.reserved[3]=0;

				if(blk1) {
					blkhdr=VOC_NEWDATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl1+VocBlk9Size;
					vocblk9.size[0]=(UBYTE)(size&0xFF);vocblk9.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk9.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk9,sizeof(vocblk9),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk1;sdata.savestart=0;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPSTART;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk6Size;
				vocblk6.size[0]=(UBYTE)(size&0xFF);vocblk6.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk6.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk6,sizeof(vocblk6),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk2) {
					blkhdr=VOC_NEWDATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl2+VocBlk9Size;
					vocblk9.size[0]=(UBYTE)(size&0xFF);vocblk9.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk9.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk9,sizeof(vocblk9),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk2;sdata.savestart=si->loops;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}

				blkhdr=VOC_LOOPEND;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk7Size;
				vocblk7.size[0]=(UBYTE)(size&0xFF);vocblk7.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk7.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk7,sizeof(vocblk7),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				if(blk3) {
					blkhdr=VOC_NEWDATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					size=bufl3+VocBlk9Size;
					vocblk9.size[0]=(UBYTE)(size&0xFF);vocblk9.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk9.size[2]=(UBYTE)((size&0xFF0000)>>16);
					if(!fwrite(&vocblk9,sizeof(vocblk9),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
					sdata.savelength=blk3;sdata.savestart=si->loops+si->loopl;sdata.offs=ftell(outfile);fclose(outfile);
					SaveVOCDataBlk(instance,si,&sdata,si->channels);
					if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				}
				break;
		}
	}
	else {
		//MSG("  without loop");
		blk1=sdata.savelength;				/* 0 bis ende */

		switch(instance->header) {
			case VERSION_1_10:
				bufl1=Samples2Bytes(blk1);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:		bufl1=GetCrunchedLength(CRUNCH_PCM8,		bufl1);vocblk1.pack=COMPVOC_PCM8;break;
					case FORMAT_VOC_ADPCM8_4:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);vocblk1.pack=COMPVOC_ADPCM8_4;break;
					case FORMAT_VOC_ADPCM8_3:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);vocblk1.pack=COMPVOC_ADPCM8_3;break;
					case FORMAT_VOC_ADPCM8_2:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);vocblk1.pack=COMPVOC_ADPCM8_2;break;
				}
				vocblk1.tc=(UBYTE)(256-(1000000/si->srat));

				blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=bufl1+VocBlk1Size;
				vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				sdata.offs=ftell(outfile);fclose(outfile);
				SaveVOCDataBlk(instance,si,&sdata,0);
				if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				break;
			case VERSION_1_15:
				bufl1=Samples2Bytes(blk1);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:		bufl1=GetCrunchedLength(CRUNCH_PCM8,		bufl1);vocblk1.pack=vocblk8.pack=COMPVOC_PCM8;break;
					case FORMAT_VOC_ADPCM8_4:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_4;break;
					case FORMAT_VOC_ADPCM8_3:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_3;break;
					case FORMAT_VOC_ADPCM8_2:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);vocblk1.pack=vocblk8.pack=COMPVOC_ADPCM8_2;break;
				}
				bufl1*=(si->channels+1);
				vocblk8.tc=LitEnd2BigEnd_uword((UWORD)(65535-(256000000/si->srat)));
				vocblk8.mode=si->channels+1;
				vocblk1.tc=(UBYTE)(256-(1000000/si->srat));

				blkhdr=VOC_EXTENDED;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=VocBlk8Size;
				vocblk8.size[0]=(UBYTE)(size&0xFF);vocblk8.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk8.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk8,sizeof(vocblk8),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

				blkhdr=VOC_DATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=bufl1+VocBlk1Size;
				vocblk1.size[0]=(UBYTE)(size&0xFF);vocblk1.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk1.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk1,sizeof(vocblk1),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				sdata.offs=ftell(outfile);fclose(outfile);
				SaveVOCDataBlk(instance,si,&sdata,si->channels);
				if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				break;
			case VERSION_1_20:
				bufl1=Samples2Bytes(blk1);
				switch(instance->format) {
					case FORMAT_VOC_PCM8:		bufl1=GetCrunchedLength(CRUNCH_PCM8,		bufl1);vocblk9.width= 8;vocblk9.pack=COMPVOC_PCM8;break;
					case FORMAT_VOC_PCM16:		bufl1=GetCrunchedLength(CRUNCH_PCM16,		bufl1);vocblk9.width=16;vocblk9.pack=COMPVOC_PCM16;break;
					case FORMAT_VOC_ADPCM8_4:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_4,bufl1);vocblk9.width= 4;vocblk9.pack=COMPVOC_ADPCM8_4;break;
					case FORMAT_VOC_ADPCM8_3:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_3,bufl1);vocblk9.width= 3;vocblk9.pack=COMPVOC_ADPCM8_3;break;
					case FORMAT_VOC_ADPCM8_2:	bufl1=GetCrunchedLength(CRUNCH_VOC_ADPCM8_2,bufl1);vocblk9.width= 2;vocblk9.pack=COMPVOC_ADPCM8_2;break;
					case FORMAT_VOC_ULAW:		bufl1=GetCrunchedLength(CRUNCH_ULAW,		bufl1);vocblk9.width= 8;vocblk9.pack=COMPVOC_ULAW;break;
					case FORMAT_VOC_ALAW:		bufl1=GetCrunchedLength(CRUNCH_ALAW,		bufl1);vocblk9.width= 8;vocblk9.pack=COMPVOC_ALAW;break;
				}
				bufl1*=(si->channels+1);
				vocblk9.frequency=LitEnd2BigEnd_uword((UWORD)si->srat);
				vocblk9.channel=si->channels+1;
				vocblk9.reserved[0]=vocblk9.reserved[1]=vocblk9.reserved[2]=vocblk9.reserved[3]=0;

				blkhdr=VOC_NEWDATA;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				size=bufl1+VocBlk9Size;
				vocblk9.size[0]=(UBYTE)(size&0xFF);vocblk9.size[1]=(UBYTE)((size&0xFF00)>>8);vocblk9.size[2]=(UBYTE)((size&0xFF0000)>>16);
				if(!fwrite(&vocblk9,sizeof(vocblk9),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
				sdata.offs=ftell(outfile);fclose(outfile);
				SaveVOCDataBlk(instance,si,&sdata,si->channels);
				if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
				break;
		}
	}
	blkhdr=VOC_TERMINATOR;if(!fwrite(&blkhdr,1,1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
	fclose(outfile);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,FormLabels[instance->format]);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ instance->format=CfgIO_MatchString(RexxPar3,FormLabels);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Header"))	{ instance->header=CfgIO_MatchString(RexxPar3,VersLabels);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))	{ sprintf(ret,"%s",FormLabels[instance->format]);pf=1; }
		if(!pf && !stricmp(RexxPar2,"Header"))	{ sprintf(ret,"%s",VersLabels[instance->header]);/*pf=1;*/ }
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
					 CfgIO_ReadString			(cfg,"General"	,"Header"		,VersLabels[VERSION_1_20]),VersLabels);
	instance->format	=CfgIO_MatchString(
					 CfgIO_ReadString			(cfg,"General"	,"Format"		,FormLabels[FORMAT_VOC_PCM8]),FormLabels);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString		  (cfg,"General"	,"Header"		,VersLabels[instance->header]);
	CfgIO_WriteString		  (cfg,"General"	,"Format"		,FormLabels[instance->format]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->format=FORMAT_VOC_PCM8;
	instance->header=VERSION_1_20;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_TYPE   ],modWin,0l,GTCY_Active,instance->format,TAG_DONE);
	GT_SetGadgetAttrs(modGadgets[GADIX_HEADER ],modWin,0l,GTCY_Active,instance->header,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

void SaveVOCDataBlk(struct Instance *instance,SInfo *si,SaveData *sdata,UBYTE channels) {
	switch(instance->format) {
		case FORMAT_VOC_PCM8:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_PCM16:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_ADPCM8_4:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_4,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_ADPCM8_3:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_3,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_ADPCM8_2:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_VOC_ADPCM8_2,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_ULAW:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_ULAW,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_ULAW,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_ULAW,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_VOC_ALAW:
			switch(channels) {
				case CH_MONO:	WriteData(si,sdata,CRUNCH_ALAW,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,sdata,CRUNCH_ALAW,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,sdata,CRUNCH_ALAW,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
	}
}

//-- eof ----------------------------------------------------------------------
