/****h* RIFF-WAV/RIFF-WAV_S.c [4.3] *
*
*  NAME
*    RIFF-WAV_S.c
*  COPYRIGHT
*    $VER: RIFF-WAV_S.c 4.3 (16.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    RIFF-WAV saver for SoundFX
*  AUTHOR
*    Stefan Kost            sk      kost@imn.htwk-leipzig.de
*  CREATION DATE
*    02.Jul.1997
*  MODIFICATION HISTORY
*    16.Apr.2004    V 4.3   most recent version
*							A: support for IEEE formats
*    29.May.2002    V 4.2   most recent version
*    07.Feb.2001    V 4.1   most recent version
*                           C: save unimportant chunks *after* important ones
*    18.Jun.2000    V 1.2   most recent version
*    30.Oct.1998    V 1.1   most recent version
*    02.Jul.1997    V 1.0   initial version
*  NOTES
*
*******
*/

/****u* _savers/RIFF-WAV_S *
*  Function.eng
*    Saves RIFF-WAV samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    This format was introduced by Windows on the PC and borrows heavily from
*    the IFF standard. The WAV format represents one of the most used formats on
*    the PC.
*  Funktion.ger
*    Speichert RIFF-WAV Samples.
*    <table border="0" cellspacing="1" cellpadding="2" width="100%>
*    <tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorwärts)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
*    <tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
*    </table>
*    Dieses Format wurde mit Window auf dem PC eingeführt und ist stark an den
*    IFF-Standart angelehnt. Das WAV-Format ist das wichtigste Sampleformat auf
*    dem PC.
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
*  Parameter.ger
*    Type
*      Art der Kompression
*        PCM8 : ungepackt 8bit
*        PCM16 : ungepackt 16bit
*        PCM24 : ungepackt 24bit
*        PCM32 : ungepackt 32bit
*        µ-Law : µ-Law (14:8) gepackt 14bit
*        A-Law : A-Law (14:8) gepackt 14bit
*        IEEE-32 : Fließkomma 32bit
*        IEEE-64 : Fließkomma 64bit
*  Notes.eng
*    none
*  Hinweise.ger
*    keine
*******
*/

#define RIFF_WAV_S_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"
#undef SFXMod_HasCfgSel

//-- Global

#include <libraries/sfxmod.h>

//-- Local

#include "project.h"
#include "RIFF-WAV.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-savers/"PRJ_NAME"_S/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

SFXMod_Prototypes

//-- defines ------------------------------------------------------------------

//-- Gadget ID's

#define GADID_FORMAT    (GADID_BASE+1)
#define GADIX_FORMAT    (GADIX_BASE+1)

#define GADID_SRC       (GADID_FORMAT+1)
#define GADIX_SRC       (GADIX_FORMAT+1)

#define LAST_GADIX      (GADIX_SRC+SRC_IXCT)

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER: "PRJ_VERS;

struct Instance {
	Source			src;        /* Quelle */
	UBYTE			format;     /* bits && compression */

	PrefsData moddesc[2];
};

char *FormLabels[]={STR_PCM8,STR_PCM16,STR_PCM24,STR_PCM32,STR_ULAW,STR_ALAW,STR_IEEE32,STR_IEEE64,0l};

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
	UWORD ys3=ys2+1+ANYFRM_HEIGHT(1);
	UWORD winh=ys3+2+CMDFRM_HEIGHT;
	UWORD xs1=2;
	UWORD winw=xs1+3+STDFRM_WIDTH;
	UWORD xh;

	SFXMod_OpenGUILibs;
	SFXMod_Interface_NotifyInit
	if(g=CreateContext(&modGList)) {
		ng.ng_LeftEdge      =xs1+RunTime->scx[(308>>1)-110];
		ng.ng_TopEdge       =ys3+RunTime->scy[2];
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

		ng.ng_LeftEdge      =xs1+RunTime->scx[37];
		ng.ng_TopEdge       =ys2+RunTime->scy[FRMCONTENT_YOFFSET];
		ng.ng_Width         =RunTime->scx[132];
		ng.ng_GadgetText    ="Type";
		ng.ng_GadgetID      =GADID_FORMAT;
		ng.ng_Flags         =PLACETEXT_LEFT;
		g=CreateGadget(CYCLE_KIND,g,&ng,GTCY_Labels,FormLabels,GTCY_Active,instance->format,TAG_DONE);
		modGadgets[GADIX_FORMAT]=g;

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
										case GADID_FORMAT:
											instance->format=icode;
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
	struct WaveFmt              wavefmt;
	//struct WaveFmt_ADPCM        wavefmtadpcm;
	//struct WaveFmt_ADPCM_Coeff  wavefmtadpcmcoeff;
	//struct WaveFmt_DVIIMA_ADPCM wavefmtdviimaadpcm;
	struct WaveFact             wavefact;
	struct WaveSmpl             wavesmpl;
	struct WaveSmpl_Loop        wavesmplloop;
	FILE *outfile;
	LONG subtype;
	ULONG bufl,fmtl,loopl=0,size;
	WORD wSize;
	SaveData sdata;
	SInfo *si;
	UBYTE ret=TRUE;

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

	MSG2("saving range from %ld of %ld length",sdata.savestart,sdata.savelength);

	if(!(outfile=fopen(pdata->fn,"wb"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	size=4L;                                                /* subtype */

	fmtl=sizeof(wavefmt);
	bufl=Samples2Bytes(sdata.savelength);
	switch(instance->format) {
		case FORMAT_WAV_PCM8:
			bufl=GetCrunchedLength(CRUNCH_PCM8,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_PCM);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_PCM8,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1));
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(8);
			break;
		case FORMAT_WAV_PCM16:
			bufl=GetCrunchedLength(CRUNCH_PCM16,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_PCM);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_PCM16,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1)*2);
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(16);
			break;
		case FORMAT_WAV_PCM24:
			bufl=GetCrunchedLength(CRUNCH_PCM24,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_PCM);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_PCM24,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1)*3);
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(24);
			break;
		case FORMAT_WAV_PCM32:
			bufl=GetCrunchedLength(CRUNCH_PCM32,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_PCM);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_PCM32,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1)*4);
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(32);
			break;
		case FORMAT_WAV_IEEE32:
			bufl=GetCrunchedLength(CRUNCH_IEEE32,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_IEEE_FLOAT);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_IEEE32,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1));
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(32);
			fmtl+=sizeof(wSize);
			size+=(12L);                                    /* fact */
			break;
		case FORMAT_WAV_IEEE64:
			bufl=GetCrunchedLength(CRUNCH_IEEE64,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_IEEE_FLOAT);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_IEEE64,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1));
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(64);
			fmtl+=sizeof(wSize);
			size+=(12L);                                    /* fact */
			break;
		case FORMAT_WAV_ULAW:
			bufl=GetCrunchedLength(CRUNCH_ULAW,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_MULAW);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_ULAW,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1));
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(8);
			fmtl+=sizeof(wSize);
			size+=(12L);                                    /* fact */
			break;
		case FORMAT_WAV_ALAW:
			bufl=GetCrunchedLength(CRUNCH_ALAW,bufl);
			wavefmt.wFormatTag          =LitEnd2BigEnd_uword(WAVE_FORMAT_ALAW);
			wavefmt.dwAvgBytesPerSec    =GetCrunchedLength(CRUNCH_ALAW,(si->srat<<1)*(si->channels+1));
			wavefmt.dwAvgBytesPerSec    =LitEnd2BigEnd_ulong(wavefmt.dwAvgBytesPerSec);     /* because it's a macro */
			wavefmt.wBlockAlign         =LitEnd2BigEnd_uword((si->channels+1));
			wavefmt.wBitsPerSample      =LitEnd2BigEnd_uword(8);
			fmtl+=sizeof(wSize);
			size+=(12L);                    	             /* fact */
			break;
		//case FORMAT_WAV_MS_ADPCM_SP:
			//break;
		//case FORMAT_WAV_MS_ADPCM_MP:
			//break;
		//case FORMAT_WAV_IMA_ADPCM_2:
			//break;
	}
	size+=(long)(fmtl+8L);     				                 /* fmt_ */

	// ################ no loop, if saving range ###########################

	if(instance->src.rmode==RNGMD_ALL && si->loop==SFXLOOP_FORWARD) {
		loopl=sizeof(wavesmpl)+sizeof(wavesmplloop);
		size+=(loopl+8L);                                   /* smpl */
	}

	// ################ no loop, if saving range ###########################

	bufl*=(si->channels+1);
	size+=(bufl+8L);                                        /* data */
	// DEBUG
	//MSG1("fmtl : %6ld",fmtl);
	//MSG1("bufl : %6ld",bufl);
	//MSG1("size : %6ld",size);
	// DEBUG

	WriteChunkHeader(WAV_RIFF,LitEnd2BigEnd_ulong(size),outfile);   /* Grundstruktur schreiben */
	subtype=WAV_WAVE;
	if(!fwrite(&subtype,sizeof(subtype),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

	WriteChunkHeader(WAV_fmt_,LitEnd2BigEnd_ulong(fmtl),outfile);
	wavefmt.wChannels       =LitEnd2BigEnd_uword(si->channels+1);
	wavefmt.dwSamplesPerSec =LitEnd2BigEnd_ulong(si->srat);
	if(!fwrite(&wavefmt,sizeof(wavefmt),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
	switch(instance->format) {
		case FORMAT_WAV_PCM8:
		case FORMAT_WAV_PCM16:
		case FORMAT_WAV_PCM24:
		case FORMAT_WAV_PCM32:
			break;
		case FORMAT_WAV_IEEE32:
		case FORMAT_WAV_IEEE64:
		case FORMAT_WAV_ULAW:
		case FORMAT_WAV_ALAW:
			wSize=LitEnd2BigEnd_uword(0);	// 0 bytes of extra data
			if(!fwrite(&wSize,sizeof(wSize),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
			WriteChunkHeader(WAV_fact,LitEnd2BigEnd_ulong(sizeof(wavefact)),outfile);
			wavefact.dwFileSize=LitEnd2BigEnd_ulong(si->slen);
			if(!fwrite(&wavefact,sizeof(wavefact),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
			break;
	}

	WriteChunkHeader(WAV_data,LitEnd2BigEnd_ulong(bufl),outfile);
	sdata.offs=ftell(outfile);
	fclose(outfile);
	switch(instance->format) {
		case FORMAT_WAV_PCM8:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_PCM8,CONV(0,SIGNUM_S_U),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_PCM16:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_PCM16,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_PCM24:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_PCM24,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_PCM32:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_PCM32,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_IEEE32:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_IEEE32,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_IEEE32,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_IEEE32,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_IEEE64:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_IEEE64,CONV(ENDIAN_B_L,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_IEEE64,CONV(ENDIAN_B_L,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_IEEE64,CONV(ENDIAN_B_L,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_ULAW:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_ULAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
		case FORMAT_WAV_ALAW:
			switch(si->channels) {
				case CH_MONO:   WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_MONO);break;
				case CH_STEREO: WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_STEREOI);break;
				case CH_QUADRO: WriteData(si,&sdata,CRUNCH_ALAW,CONV(0,0),CHANNEL_QUADROI);break;
			}
			break;
	}

	if(!(outfile=fopen(pdata->fn,"ab"))) { StdCError(pdata->fn,errOpenFile,errno,__FILE__,__LINE__);return(FALSE); }
	if(instance->src.rmode==RNGMD_ALL && si->loop==SFXLOOP_FORWARD) {
		WriteChunkHeader(WAV_smpl,LitEnd2BigEnd_ulong(loopl),outfile);
		wavesmpl.dwIMAManufacturer  =LitEnd2BigEnd_ulong(0);
		wavesmpl.dwIMAProduct       =LitEnd2BigEnd_ulong(0);
		wavesmpl.dwSamplePeriod     =(ULONG)(0.5+(1000000000.0/(double)si->srat));
		wavesmpl.dwSamplePeriod     =LitEnd2BigEnd_ulong(wavesmpl.dwSamplePeriod);
		wavesmpl.dwUnityMIDINote    =LitEnd2BigEnd_ulong(0);
		wavesmpl.dwPitchFraction    =LitEnd2BigEnd_ulong(0);
		wavesmpl.dwSMPTEType        =LitEnd2BigEnd_ulong(30);
		wavesmpl.dwSMPTEOffset      =LitEnd2BigEnd_ulong(0);
		wavesmpl.dwNumLoops         =LitEnd2BigEnd_ulong(1);
		wavesmpl.dwSamplerSpecificSize=LitEnd2BigEnd_ulong(0);
		if(!fwrite(&wavesmpl,sizeof(wavesmpl),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }

		wavesmplloop.dwLoopID       =LitEnd2BigEnd_ulong(0);
		wavesmplloop.dwType         =LitEnd2BigEnd_ulong(WAV_SMPL_LOOPFWD);
		wavesmplloop.dwStart        =LitEnd2BigEnd_ulong(si->loops);
		wavesmplloop.dwEnd          =si->loops+si->loopl;
		wavesmplloop.dwEnd          =LitEnd2BigEnd_ulong(wavesmplloop.dwEnd);
		wavesmplloop.dwFraction     =LitEnd2BigEnd_ulong(0);
		wavesmplloop.lPlayCount     =LitEnd2BigEnd_ulong(WAV_SMPL_LOOPINF);
		if(!fwrite(&wavesmplloop,sizeof(wavesmplloop),1,outfile)) { Message(errWriteData,NULL,__FILE__,__LINE__);fclose(outfile);return(FALSE); }
	}
	// LIST INAM 
	fclose(outfile);

	//-- set file comment
	SetSampleComment(si,pdata->fn,PRJ_NAME,FormLabels[instance->format]);

	return(TRUE);
}

void __saveds __asm LIBSFXMod_HandleARexxCmd(REG(a0,struct Instance *instance),REG(a1,char *params),REG(a2,char *ret)) {
	UBYTE cf=FALSE,pf=FALSE;
	char fn[256];

	if(!stricmp(RexxPar0,"SFX_SetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))  { instance->format=CfgIO_MatchString(RexxPar3,FormLabels);pf=1; }
		if(pf) SaveSettings(instance,ENVDIR"savers/"PRJ_NAME".cfg");
	}
	if(!cf && !stricmp(RexxPar0,"SFX_GetSaverParam")) {
		if(       !stricmp(RexxPar2,"Format"))  { sprintf(ret,"%s",FormLabels[instance->format]);/*pf=1;*/ }
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
	instance->format =CfgIO_MatchString(
					 CfgIO_ReadString           (cfg,"General"  ,"Format"       ,FormLabels[FORMAT_WAV_PCM16]),FormLabels);

	MSG1("rmode=%ld",instance->src.rmode);
}

void SaveSettings(struct Instance *instance,char *fname) {
	SaveModSettings(cfg,instance->moddesc);
	CfgIO_WriteString         (cfg,"General"    ,"Format"       ,FormLabels[instance->format]);

	CfgIO_WriteConfig(cfg,fname);
}

void SetDefaultSettings(struct Instance *instance) {
	instance->src.srcbuf=0L;
	instance->src.rmode=RNGMD_ALL;

	instance->format=FORMAT_WAV_PCM16;
}

SFXMod_RecallLastSettings
SFXMod_RecallDefaultSettings

void RefreshGUI(struct Instance *instance,struct Gadget *modGadgets[],struct Window *modWin) {
	SetSource(&instance->src,modWin,modGadgets,GADIX_SRC);
	GT_SetGadgetAttrs(modGadgets[GADIX_FORMAT],modWin,0l,GTCY_Active,instance->format,TAG_DONE);
}

//-- private Stuff ------------------------------------------------------------

//-- eof ----------------------------------------------------------------------
