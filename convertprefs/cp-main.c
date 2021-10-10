/****h*convertprefs/cp-main.c [1.00] *
*
*  NAME
*    cp-main.c
*  COPYRIGHT
*    $VER: cp-main.c 1.00 (15.09.98) © by Stefan Kost 1998-1998
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Aug.1998
*  MODIFICATION HISTORY
*    15.Sep.1998	V 1.00	most recent version
*    21.Aug.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#define CP_Main_C

//-- includes -----------------------------------------------------------------

#include "cp-includes.h"

//-- prototypes ---------------------------------------------------------------

void Init(void);
void Done(LONG exitcode);

void ReadOldPrefs(STRPTR fn);
void WriteNewPrefs(STRPTR fn);

//-- defines ------------------------------------------------------------------

//-- globals ------------------------------------------------------------------

//-- system libraries

struct Library 		*ConfigIOBase=NULL;

//-- Prefs ----------------------------------------------------------------------------------------

#define PREF_FORM		MakeID('F','O','R','M')
#define PREF_PREF		MakeID('P','R','E','F')
#define PREF_VERS		MakeID('V','E','R','S')
#define PREF_PATH		MakeID('P','A','T','H')			// obsolete ->sbuf,vmem
#define PREF_GUI_		MakeID('G','U','I','_')
#define PREF_WIN_		MakeID('W','I','N','_')
#define PREF_MISC		MakeID('M','I','S','C')
#define PREF_SBUF		MakeID('S','B','U','F')
#define PREF_VMEM		MakeID('V','M','E','M')
#define PREF_AHI_		MakeID('A','H','I','_')			// just temporarilly

#define PREF_PARA		MakeID('P','A','R','A')			// for Modules
#define PREF_NAME		MakeID('N','A','M','E')
#define PREF_DATA		MakeID('D','A','T','A')

typedef struct							/* Prefs */
{
	ULONG	id;								/* FORM,VERS,PATH,GUI_,MISC,SBUF, */
	ULONG	len;							/* 4+(8+2)+(8+768)+(8+166)+(8+2)+(8+14)=1002 */
} SFXPrefs_Hdr;

typedef struct								/* VERS : 2 Bytes */
{
	UBYTE	ver;							/* Version   */
	UBYTE	rev;							/* Reversion */
} SFXPrefs_Ver;

typedef struct								/* PATH : 768 Bytes */						/* OBSOLETE */
{
	char sample_path[256];					/* Pfade für Filerequester */
	char swap_path[256];
	char unused_path[256];					/* unused */
} SFXPrefs_Path;

typedef struct								/* GUI_ : >166 Bytes */
{
	ULONG	scrmode;						/* ScreenMode */
	UWORD	xres,yres;						/* Auflösung */
	UBYTE	oscan;							/* Overscan */
	UBYTE	autoscroll;						/* Autoscroll */
	UBYTE	ScreenColors[16][3];			/* Bildschirmfarben (48 Bytes) */
	UBYTE	Pens[20];						/* Pennumbers */
	char	fontname[80];					/* Fontname */
	UBYTE	fontsize;						/* Fontgröße */
	UBYTE	depth;							/* Screendepth */
} SFXPrefs_Gui;

#define PEN_GUI_BACK		 0			//-- Pennumbers
#define PEN_GUI_SHINE		 1
#define PEN_GUI_SHADOW		 2
#define PEN_GUI_HIGHLIGHT	 3
#define PEN_GUI_COLORTEXT	 4

#define PEN_SMP_BACK		 5
#define PEN_SMP_TEXT		 6
#define PEN_SMP_LINE		 7
#define PEN_SMP_GRID		 8
#define PEN_SMP_MAXL		 9
#define PEN_SMP_RMSL		10
#define PEN_SMP_AVGL		11
#define PEN_SMP_HIGHLIGHT	12
#define PEN_SMP_LOOP		13
#define PEN_SMP_MARK		14

typedef struct								/* GUI_ : 166 Bytes */						/* OBSOLETE */
{
	ULONG	scrmode;						/* ScreenMode */
	UWORD	xres,yres;						/* Auflösung */
	UBYTE	oscan;							/* Overscan */
	UBYTE	autoscroll;						/* Autoscroll */
	UBYTE	xdef,ydef;						/* XDefault,YDefault !!! NOW OBSOLETE !!! */
	struct	ColorSpec ScreenColors[9];		/* Bildschirmfarben (72 Bytes) */
	char	fontname[80];					/* Fontname */
	UBYTE	fontsize;						/* Fontgröße */
	UBYTE	depth;							/* Screendepth */
} SFXPrefs_Gui_033D;

typedef struct								/* neue Struktur */
{
	UWORD x1,y1,x2,y2;						/* Pos and Size */
	UBYTE state;							/* Zustand : 0->closed/1->opened */
	UBYTE pad;
	UBYTE entryname[50];					/* angewählter Eintrag */
} SFXPrefs_LWin;

typedef struct
{
	UWORD x1,y1;							/* Pos */
	UBYTE state;							/* Zustand : 0->closed/1->opened */
	UBYTE pad;
} SFXPrefs_NWin;

typedef struct
{
	SFXPrefs_LWin	opwin;
	SFXPrefs_LWin	rxopwin;
	SFXPrefs_LWin	lowin;
	SFXPrefs_LWin	sawin;
	SFXPrefs_LWin	plwin;
	SFXPrefs_LWin	bufwin;
	SFXPrefs_NWin	zowin;
	SFXPrefs_NWin	edwin;
	SFXPrefs_NWin	rngwin;
} SFXPrefs_Win;

typedef struct								/* MISC : 8 Bytes */
{
	UBYTE sig_a;							/* Wenn mit Berechnung fertig DisplayBeep() */
	UBYTE sig_s;							/* Wenn mit Berechnung fertig ScreenToFont() */
	UBYTE backlogo;							/* Backlogo on/off */
	UBYTE pad;
	ULONG keynr;							/* Keynr, wenn die sich ändert ist etwas suspect */
	UBYTE freemem,maxmem;					/* soll freier Speicher, sowie maximaler Speicher angezeigt werden */
} SFXPrefs_Misc;

typedef struct								/* SBUF : >270 Bytes */
{
	UBYTE drawmode;							/* 0 - 2 (Lines,Dots,Filled) */
	UBYTE maxlines;							/* Maxlines zeichnen ? */
	UBYTE rasterx,rastery;					/* X/Y-Raster zeichnen ? */
	UBYTE safemode;							/* Sicherheitsabfrage */
	UBYTE quickdraw;						/* schnelles Zeichnen beim Scrollen ? */
	UWORD xs1,ys1,xs2,ys2;					/* Normal and Zoomed Size */
	UBYTE mode;								/* Sizemode */
	UBYTE storage;							/* Speicherungsform */
	UBYTE rmslines,avglines;				/* RMS-,AVG Linien zeichnen */
	UWORD scaxs,scays;						/* Scalengröße */							/* ?? */
	UBYTE scalax,scalay;					/* Scala auf der X bzw. Y-Achse */
	char  sample_path[256];					/* Defaultpfad für Filerequester */
	char  scafont_name[256];				/* Scalenfont */
	UBYTE scafont_size;
	UBYTE pad2[1];
	UBYTE unitx;							/* Samples,Time,SMPTE-MF,SMPTE-PF,SMPTE-NF */
	UBYTE unity;							/* Factor,Prozent,Promille,Level,Dezibel */
} SFXPrefs_SBuf;

typedef struct								/* VMEM : ... Bytes */
{
	UBYTE swap_path[256];					/* wohin sollen die Samples ausgelagert werden */
	ULONG blksize;
	UBYTE enabled;
} SFXPrefs_VMem;

typedef struct
{
	ULONG playmode;
	UWORD playrate;
} OPTP_Ahi;

SFXPrefs_Hdr		sfxprefs_hdr;
SFXPrefs_Ver		sfxprefs_ver;
SFXPrefs_Path		sfxprefs_path;						/* OBSOLETE */
SFXPrefs_Gui		sfxprefs_gui;						//
SFXPrefs_Win		sfxprefs_win;
SFXPrefs_Misc		sfxprefs_misc;						//
SFXPrefs_SBuf		sfxprefs_sbuf;						//
SFXPrefs_VMem		sfxprefs_vmem;						//
SFXPrefs_Gui_033D 	sfxprefs_gui_033d;					/* OBSOLETE */
OPTP_Ahi			optp_Ahi;							//

//-- definitions --------------------------------------------------------------

void Init(void)
{
	LONG ret=TRUE;

			if(!(ConfigIOBase	=OpenLibrary(ConfigIOName,ConfigIOVersion))) ret=FALSE;
//	if(ret) if(!(DOSBase		=(struct DosLibrary *)OpenLibrary("dos.library",34L))) ret=FALSE;
	if(!ret) Done(RETURN_FAIL);
}

void Done(LONG exitcode)
{
	if(ConfigIOBase)	CloseLibrary(ConfigIOBase);
//	if(DOSBase)			CloseLibrary(DOSBase);
	Exit(exitcode);
}

void ReadOldPrefs(STRPTR fn)
{
	register UBYTE i;
	FILE *prefs;
	ULONG subtyp,len;
	UWORD ver=0;

	MSG("ReadOldPrefs beg");

	if(prefs=fopen(fn,"rb"))
	{
		if(!fread(&sfxprefs_hdr,8,1,prefs)) { fclose(prefs);return; }
		if(sfxprefs_hdr.id!=PREF_FORM) { fclose(prefs);return; }
		len=sfxprefs_hdr.len-4;
		if(!fread(&subtyp,4,1,prefs)) { fclose(prefs);return; }
		if(subtyp!=PREF_PREF) { fclose(prefs);return; }

		while(len>0)
		{
			if(!fread(&sfxprefs_hdr,8,1,prefs)) { fclose(prefs);return; }
			len-=8;
			switch(sfxprefs_hdr.id)
			{
				case PREF_VERS:
					if(!fread(&sfxprefs_ver,sizeof(sfxprefs_ver),1,prefs)) { fclose(prefs);return; }
					ver=(sfxprefs_ver.ver<<8)|sfxprefs_ver.rev;
					break;
				case PREF_PATH:
					if(!fread(&sfxprefs_path,sizeof(sfxprefs_path),1,prefs)) { fclose(prefs);return; }
					strcpy(sfxprefs_sbuf.sample_path,sfxprefs_path.sample_path);
					strcpy(sfxprefs_vmem.swap_path,sfxprefs_path.swap_path);
					break;
				case PREF_GUI_:
					switch(ver)
					{
						case 0x0322:		/* 3.34 */
						case 0x0323:		/* 3.35 */
						case 0x0324:		/* 3.36 */
						case 0x0328:		/* 3.40 */
							if(!fread(&sfxprefs_gui_033d,sizeof(sfxprefs_gui_033d),1,prefs)) { fclose(prefs);return; }
							sfxprefs_gui.scrmode	=sfxprefs_gui_033d.scrmode;
							sfxprefs_gui.xres		=sfxprefs_gui_033d.xres;
							sfxprefs_gui.yres		=sfxprefs_gui_033d.yres;
							sfxprefs_gui.oscan		=sfxprefs_gui_033d.oscan;
							sfxprefs_gui.autoscroll	=sfxprefs_gui_033d.autoscroll;
							sfxprefs_gui.fontsize	=sfxprefs_gui_033d.fontsize;
							sfxprefs_gui.depth		=sfxprefs_gui_033d.depth;
							sfxprefs_gui.ScreenColors[0][0]=0xAF;sfxprefs_gui.ScreenColors[0][1]=0x9F;sfxprefs_gui.ScreenColors[0][2]=0xAF;
							sfxprefs_gui.ScreenColors[1][0]=0x1F;sfxprefs_gui.ScreenColors[1][1]=0x0F;sfxprefs_gui.ScreenColors[1][2]=0x1F;
							sfxprefs_gui.ScreenColors[2][0]=0xFF;sfxprefs_gui.ScreenColors[2][1]=0xEF;sfxprefs_gui.ScreenColors[2][2]=0xFF;
							sfxprefs_gui.ScreenColors[3][0]=0xAF;sfxprefs_gui.ScreenColors[3][1]=0x8F;sfxprefs_gui.ScreenColors[3][2]=0xCF;
							sfxprefs_gui.ScreenColors[4][0]=0x3F;sfxprefs_gui.ScreenColors[4][1]=0xFF;sfxprefs_gui.ScreenColors[4][2]=0x3F;
							sfxprefs_gui.ScreenColors[5][0]=0xCF;sfxprefs_gui.ScreenColors[5][1]=0xBF;sfxprefs_gui.ScreenColors[5][2]=0xCF;
							sfxprefs_gui.ScreenColors[6][0]=0x6F;sfxprefs_gui.ScreenColors[6][1]=0x5F;sfxprefs_gui.ScreenColors[6][2]=0x6F;
							sfxprefs_gui.ScreenColors[7][0]=0xFF;sfxprefs_gui.ScreenColors[7][1]=0xFF;sfxprefs_gui.ScreenColors[7][2]=0x7F;
							break;
						case 0x032D:		/* 3.45 */
						case 0x0332:		/* 3.50 */
							if(!fread(&sfxprefs_gui_033d,sizeof(sfxprefs_gui_033d),1,prefs)) { fclose(prefs);return; }
							sfxprefs_gui.scrmode	=sfxprefs_gui_033d.scrmode;
							sfxprefs_gui.xres		=sfxprefs_gui_033d.xres;
							sfxprefs_gui.yres		=sfxprefs_gui_033d.yres;
							sfxprefs_gui.oscan		=sfxprefs_gui_033d.oscan;
							sfxprefs_gui.autoscroll	=sfxprefs_gui_033d.autoscroll;
							sfxprefs_gui.fontsize	=sfxprefs_gui_033d.fontsize;
							sfxprefs_gui.depth		=sfxprefs_gui_033d.depth;
							for(i=0;i<8;i++)
							{
								sfxprefs_gui.ScreenColors[i][0]=(sfxprefs_gui_033d.ScreenColors[i].Red  <<4)|0x0F;
								sfxprefs_gui.ScreenColors[i][1]=(sfxprefs_gui_033d.ScreenColors[i].Green<<4)|0x0F;
								sfxprefs_gui.ScreenColors[i][2]=(sfxprefs_gui_033d.ScreenColors[i].Blue <<4)|0x0F;
							}
							break;
						case 0x033C:		/* 3.60 */
						case 0x033D:		/* 3.61 */
							if(!fread(&sfxprefs_gui_033d,sizeof(sfxprefs_gui_033d),1,prefs)) { fclose(prefs);return; }
							sfxprefs_gui.scrmode	=sfxprefs_gui_033d.scrmode;
							sfxprefs_gui.xres		=sfxprefs_gui_033d.xres;
							sfxprefs_gui.yres		=sfxprefs_gui_033d.yres;
							sfxprefs_gui.oscan		=sfxprefs_gui_033d.oscan;
							sfxprefs_gui.autoscroll	=sfxprefs_gui_033d.autoscroll;
							sfxprefs_gui.fontsize	=sfxprefs_gui_033d.fontsize;
							sfxprefs_gui.depth		=sfxprefs_gui_033d.depth;
							for(i=0;i<8;i++)
							{
								sfxprefs_gui.ScreenColors[i][0]=sfxprefs_gui_033d.ScreenColors[i].Red;
								sfxprefs_gui.ScreenColors[i][1]=sfxprefs_gui_033d.ScreenColors[i].Green;
								sfxprefs_gui.ScreenColors[i][2]=sfxprefs_gui_033d.ScreenColors[i].Blue;
							}
							strcpy(sfxprefs_gui.fontname,sfxprefs_gui_033d.fontname);
							break;
						default:
							if(!fread(&sfxprefs_gui,sizeof(sfxprefs_gui),1,prefs)) { fclose(prefs);return; }
							break;
					}
					break;
				case PREF_WIN_:
					if(!fread(&sfxprefs_win,sizeof(sfxprefs_win),1,prefs)) { fclose(prefs);return; }
					break;
				case PREF_MISC:
					switch(ver)
					{
						case 0x0300:		/* 3.00 */
						case 0x0322:		/* 3.34 */
						case 0x0323:		/* 3.35 */
							if(!fread(&sfxprefs_misc,sfxprefs_hdr.len,1,prefs)) { fclose(prefs);return; }
							sfxprefs_misc.keynr=0xABADFACE;
							break;
						case 0x032D:		/* 3.45 */
						case 0x0332:		/* 3.50 */
						case 0x033C:		/* 3.60 */
						case 0x033D:		/* 3.61 */
							if(!fread(&sfxprefs_misc,sfxprefs_hdr.len,1,prefs)) { fclose(prefs);return; }
							sfxprefs_misc.keynr=0xABADFACE;
							sfxprefs_misc.freemem=1;
							sfxprefs_misc.maxmem=1;
							break;
						default:
							if(!fread(&sfxprefs_misc,sizeof(sfxprefs_misc),1,prefs)) { fclose(prefs);return; }
							break;
					}
					break;
				case PREF_SBUF:
					switch(ver)
					{
						case 0x0300:		/* 3.00 */
						case 0x0322:		/* 3.34 */
						case 0x0323:		/* 3.35 */
						case 0x032D:		/* 3.45 */
						case 0x0332:		/* 3.50 */
						case 0x033C:		/* 3.60 */
							if(!fread(&sfxprefs_sbuf,sfxprefs_hdr.len,1,prefs)) { fclose(prefs);return; }
							strcpy(sfxprefs_sbuf.sample_path,sfxprefs_path.sample_path);
							strcpy(sfxprefs_sbuf.scafont_name,"Trinomic.font");
							sfxprefs_sbuf.scafont_size=6;
							sfxprefs_sbuf.rmslines=1;
							sfxprefs_sbuf.unitx=ATME_SAMPLES;
							sfxprefs_sbuf.unity=AMP_LEVEL;
							break;
						case 0x033D:		/* 3.61 */
							if(!fread(&sfxprefs_sbuf,sfxprefs_hdr.len,1,prefs)) { fclose(prefs);return; }
							strcpy(sfxprefs_sbuf.scafont_name,"Trinomic.font");
							sfxprefs_sbuf.scafont_size=6;
							sfxprefs_sbuf.rmslines=1;
							sfxprefs_sbuf.unitx=ATME_SAMPLES;
							sfxprefs_sbuf.unity=AMP_LEVEL;
							break;
						default:
							if(!fread(&sfxprefs_sbuf,sizeof(sfxprefs_sbuf),1,prefs)) { fclose(prefs);return; }
							break;
					}
					break;
				case PREF_VMEM:
					if(!fread(&sfxprefs_vmem,sizeof(sfxprefs_vmem),1,prefs)) { fclose(prefs);return; }
					break;
				case PREF_AHI_:
					if(!fread(&optp_Ahi,sizeof(optp_Ahi),1,prefs)) { fclose(prefs);return; }
				default:
					fseek(prefs,sfxprefs_hdr.len,1);
					break;
			}
			len-=sfxprefs_hdr.len;
			if(sfxprefs_hdr.len&1) { len--;fseek(prefs,1L,1); }
		}
		fclose(prefs);
	}
	MSG("ReadOldPrefs end");
}

void WriteNewPrefs(STRPTR fn)
{
	APTR cfg;
	register ULONG i;
	char strItemName[20],strItemData[200];

	UBYTE *DrawModeLabels[]={ "Lines","Dots","DotsAbs","Filled","FilledAbs","FilledHQ",0l };
	UBYTE *SafeLabels[]=	{ "never","if unsaved","ever",0l };
	UBYTE *StorageLabels[]=	{ "Memory","Drive",0l };
	UBYTE *XUnitLabels[]=	{ "Samples","Time","SMPTE 24 fps","SMPTE 25 fps","SMPTE 30 fps",0l };
	UBYTE *YUnitLabels[]=	{ "Factor","Per cent","Per thousand","Level","Dezibel",0l };
	UBYTE *SizeModeLabels[]={ "Absolut","Relative (%%)",0l };

	MSG("WriteNewPrefs beg");
	if((cfg=CfgIO_NewConfig("SoundFX","$VER: %s 3.70 (%s) © by Stefan Kost 1993-%s")))
	{
		MSG("cfg created");
		CfgIO_WriteString		  (cfg,"VirtualMemory"	,"SwapPath"		,sfxprefs_vmem.swap_path);
		CfgIO_WriteUnsignedInteger(cfg,"VirtualMemory"	,"BlockSize"	,sfxprefs_vmem.blksize);
		CfgIO_WriteBool			  (cfg,"VirtualMemory"	,"Enabled"		,sfxprefs_vmem.enabled);
		MSG("\tvmem section written");

		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"SignalAudio"	,sfxprefs_misc.sig_a);
		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"SignalScreen"	,sfxprefs_misc.sig_s);
		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"BackLogo"		,sfxprefs_misc.backlogo);
		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"ShowFreeMem"	,sfxprefs_misc.freemem);
		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"ShowMaxMem"	,sfxprefs_misc.maxmem);
//		CfgIO_WriteBool			  (cfg,"Miscellaneous"	,"KeyNumber"	,sfxprefs_misc.keynr);
		MSG("\tmisc section written");

		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"ScreenMode"	,sfxprefs_gui.scrmode);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"XRes"			,sfxprefs_gui.xres);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"YRes"			,sfxprefs_gui.yres);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"Depth"		,sfxprefs_gui.depth);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"OverScan"		,sfxprefs_gui.oscan);
		CfgIO_WriteBool			  (cfg,"GUI"			,"AutoScroll"	,sfxprefs_gui.autoscroll);
		CfgIO_WriteString		  (cfg,"GUI"			,"FontName"		,sfxprefs_gui.fontname);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"FontSize"		,sfxprefs_gui.fontsize);
		for(i=0;i<16;i++)
		{
			sprintf(strItemName,"Color%02d",i);
			sprintf(strItemData,"%3d,%3d,%3d",sfxprefs_gui.ScreenColors[i][0],sfxprefs_gui.ScreenColors[i][1],sfxprefs_gui.ScreenColors[i][2]);
			CfgIO_WriteString	  (cfg,"GUI"			,strItemName	,strItemData);
		}
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenGUI_Back"		,sfxprefs_gui.Pens[PEN_GUI_BACK]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenGUI_Shine"		,sfxprefs_gui.Pens[PEN_GUI_SHINE]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenGUI_Shadow"	,sfxprefs_gui.Pens[PEN_GUI_SHADOW]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenGUI_Highlight"	,sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenGUI_ColorText"	,sfxprefs_gui.Pens[PEN_GUI_COLORTEXT]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Back"		,sfxprefs_gui.Pens[PEN_SMP_BACK]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Text"		,sfxprefs_gui.Pens[PEN_SMP_TEXT]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Line"		,sfxprefs_gui.Pens[PEN_SMP_LINE]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Grid"		,sfxprefs_gui.Pens[PEN_SMP_GRID]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_MaxL"		,sfxprefs_gui.Pens[PEN_SMP_MAXL]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_RmsL"		,sfxprefs_gui.Pens[PEN_SMP_RMSL]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_AvgL"		,sfxprefs_gui.Pens[PEN_SMP_AVGL]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Highlight"	,sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Loop"		,sfxprefs_gui.Pens[PEN_SMP_LOOP]);
		CfgIO_WriteUnsignedInteger(cfg,"GUI"			,"PenSMP_Mark"		,sfxprefs_gui.Pens[PEN_SMP_MARK]);
		MSG("\tgui  section written");

		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"DrawMode"			,DrawModeLabels[sfxprefs_sbuf.drawmode]);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"MaxLines"			,sfxprefs_sbuf.maxlines);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"RmsLines"			,sfxprefs_sbuf.rmslines);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"AvgLines"			,sfxprefs_sbuf.avglines);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"UnitX"			,sfxprefs_sbuf.unitx);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"UnitY"			,sfxprefs_sbuf.unity);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"RasterX"			,sfxprefs_sbuf.rasterx);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"RasterY"			,sfxprefs_sbuf.rastery);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"ScalaX"			,sfxprefs_sbuf.scalax);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"ScalaY"			,sfxprefs_sbuf.scalay);
		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"ScalaFontName"	,sfxprefs_sbuf.scafont_name);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"ScalaFontSize"	,sfxprefs_sbuf.scafont_size);
		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"SafeMode"			,SafeLabels[sfxprefs_sbuf.safemode]);
		CfgIO_WriteBool			  (cfg,"SampleBuffer"	,"QuickDraw"		,sfxprefs_sbuf.quickdraw);
		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"SamplePath"		,sfxprefs_sbuf.sample_path);
		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"Storage"			,StorageLabels[sfxprefs_sbuf.storage]);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"NormalXRes"		,sfxprefs_sbuf.xs1);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"NormalYRes"		,sfxprefs_sbuf.ys1);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"ZoomedXRes"		,sfxprefs_sbuf.xs2);
		CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"	,"ZoomedYRes"		,sfxprefs_sbuf.ys2);
		CfgIO_WriteString		  (cfg,"SampleBuffer"	,"SizeMode"			,SizeModeLabels[sfxprefs_sbuf.mode]);
		MSG("\tsbuf section written");

		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.opwin.x1,sfxprefs_win.opwin.y1,sfxprefs_win.opwin.x2,sfxprefs_win.opwin.y2,sfxprefs_win.opwin.state,sfxprefs_win.opwin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"Operators"		,strItemData);
		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.rxopwin.x1,sfxprefs_win.rxopwin.y1,sfxprefs_win.rxopwin.x2,sfxprefs_win.rxopwin.y2,sfxprefs_win.rxopwin.state,sfxprefs_win.rxopwin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"RexxOperators"	,strItemData);
		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.lowin.x1,sfxprefs_win.lowin.y1,sfxprefs_win.lowin.x2,sfxprefs_win.lowin.y2,sfxprefs_win.lowin.state,sfxprefs_win.lowin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"Loaders"			,strItemData);
		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.sawin.x1,sfxprefs_win.sawin.y1,sfxprefs_win.sawin.x2,sfxprefs_win.sawin.y2,sfxprefs_win.sawin.state,sfxprefs_win.sawin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"Savers"			,strItemData);
		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.plwin.x1,sfxprefs_win.plwin.y1,sfxprefs_win.plwin.x2,sfxprefs_win.plwin.y2,sfxprefs_win.plwin.state,sfxprefs_win.plwin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"Players"			,strItemData);
		sprintf(strItemData,"%d,%d,%d,%d,%d,%s",sfxprefs_win.bufwin.x1,sfxprefs_win.bufwin.y1,sfxprefs_win.bufwin.x2,sfxprefs_win.bufwin.y2,sfxprefs_win.bufwin.state,sfxprefs_win.bufwin.entryname);
		CfgIO_WriteString		  (cfg,"Windows"		,"Buffers"			,strItemData);
		sprintf(strItemData,"%d,%d,%d",sfxprefs_win.zowin.x1,sfxprefs_win.zowin.y1,sfxprefs_win.zowin.state);
		CfgIO_WriteString		  (cfg,"Windows"		,"Zoom"				,strItemData);
		sprintf(strItemData,"%d,%d,%d",sfxprefs_win.edwin.x1,sfxprefs_win.edwin.y1,sfxprefs_win.edwin.state);
		CfgIO_WriteString		  (cfg,"Windows"		,"Edit"				,strItemData);
		sprintf(strItemData,"%d,%d,%d",sfxprefs_win.rngwin.x1,sfxprefs_win.rngwin.y1,sfxprefs_win.rngwin.state);
		CfgIO_WriteString		  (cfg,"Windows"		,"Range"			,strItemData);
		MSG("\twin  section written");

		CfgIO_WriteUnsignedInteger(cfg,"Player_Ahi"		,"Mode"				,optp_Ahi.playmode);
		CfgIO_WriteUnsignedInteger(cfg,"Player_Ahi"		,"Rate"				,optp_Ahi.playrate);
		MSG("\tahi  section written");

		CfgIO_WriteConfig(cfg,fn);
		MSG("cfg written");
		CfgIO_RemConfig(cfg);
		MSG("cfg removed");
	}
	MSG("WriteNewPrefs end");
}

void main(void)
{
	Init();

	ReadOldPrefs("envarc:sfx.prefs");
	WriteNewPrefs("envarc:sfx.cfg");

	Done(RETURN_OK);
}

//-- eof ----------------------------------------------------------------------
