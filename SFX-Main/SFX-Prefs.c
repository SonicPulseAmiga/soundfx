/****h* SoundFX/SFX-Prefs.c [4.3] *
*
*  NAME
*    SFX-Prefs.c
*  COPYRIGHT
*    $VER: SFX-Prefs.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    preferences io - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    26.Apr.2002    V 4.2   most recent version
*    23.Feb.2001    V 4.1   most recent version
*                           * new options (copystr,authstr)
*    16.Jul.2000    V 4.00  most recent version
*                           * new options (nolines,thrsize,contextbutton)
*    26.Jan.2000    V 3.82  most recent version
*    27.Apr.1999    V 3.81  most recent version
*    17.Jan.1999    V 3.80  most recent version
*    18.Sep.1998    V 3.71  most recent version
*                           * new prefs format, using configio.library now
*    11.Aug.1998    V 3.70  release
*    18.Jul.1998    V 3.70  initial version
*  NOTES
*    * Keychecks
*      114+(key^279)  1367,1388,1630
*      114+(key^291)  1540
*******
*/

#define SFX_Prefs_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- io routines

void ReadPrefs(char *fn);
void WritePrefs(char *fn);

//-- defaults

void SetDefPOptions(int nr);

//-- defines ------------------------------------------------------------------

//-- for Prefsmenu

#define PMEN_LOAD        0
#define PMEN_USE         1
#define PMEN_SAVE        2
#define PMEN_HELP        3
#define PMEN_QUIT        4

#define PMEN_DEFAULTS   10
#define PMEN_LASTSAVED  11
#define PMEN_LASTUSED   12

//-- globals ------------------------------------------------------------------

//-- Preferences-Strukturen

SFXPrefs_Gui                sfxprefs_gui;
SFXPrefs_Misc               sfxprefs_misc;
SFXPrefs_SBuf               sfxprefs_sbuf;
SFXPrefs_VMem               sfxprefs_vmem;
SFXPrefs_Win                sfxprefs_win;

//-- definitions --------------------------------------------------------------

//-- io routines

void ReadPrefs(char *fn)
{
	register UWORD i;
	char strItemName[20],strItemData1[200],*strItemData2;
	char sample_path[SFXPrefs_SBuf_SamplePathLen];

	INTRO;

	//-- set default colors here, as we cycle through them in a loop later
	sfxprefs_gui.ScreenColors[ 0][0]=176;sfxprefs_gui.ScreenColors[ 0][1]=160;sfxprefs_gui.ScreenColors[ 0][2]=160;
	sfxprefs_gui.ScreenColors[ 1][0]= 31;sfxprefs_gui.ScreenColors[ 1][1]= 15;sfxprefs_gui.ScreenColors[ 1][2]= 15;
	sfxprefs_gui.ScreenColors[ 2][0]=255;sfxprefs_gui.ScreenColors[ 2][1]=239;sfxprefs_gui.ScreenColors[ 2][2]=239;
	sfxprefs_gui.ScreenColors[ 3][0]=175;sfxprefs_gui.ScreenColors[ 3][1]=143;sfxprefs_gui.ScreenColors[ 3][2]=207;
	sfxprefs_gui.ScreenColors[ 4][0]= 63;sfxprefs_gui.ScreenColors[ 4][1]=255;sfxprefs_gui.ScreenColors[ 4][2]= 63;
	sfxprefs_gui.ScreenColors[ 5][0]=207;sfxprefs_gui.ScreenColors[ 5][1]=191;sfxprefs_gui.ScreenColors[ 5][2]=191;
	sfxprefs_gui.ScreenColors[ 6][0]=115;sfxprefs_gui.ScreenColors[ 6][1]= 99;sfxprefs_gui.ScreenColors[ 6][2]= 99;
	sfxprefs_gui.ScreenColors[ 7][0]=255;sfxprefs_gui.ScreenColors[ 7][1]=255;sfxprefs_gui.ScreenColors[ 7][2]=127;
	sfxprefs_gui.ScreenColors[ 8][0]=100;sfxprefs_gui.ScreenColors[ 8][1]=  0;sfxprefs_gui.ScreenColors[ 8][2]= 31;
	sfxprefs_gui.ScreenColors[ 9][0]=100;sfxprefs_gui.ScreenColors[ 9][1]=  0;sfxprefs_gui.ScreenColors[ 9][2]= 47;
	sfxprefs_gui.ScreenColors[10][0]=255;sfxprefs_gui.ScreenColors[10][1]=  0;sfxprefs_gui.ScreenColors[10][2]= 63;
	sfxprefs_gui.ScreenColors[11][0]= 16;sfxprefs_gui.ScreenColors[11][1]= 64;sfxprefs_gui.ScreenColors[11][2]=255;
	sfxprefs_gui.ScreenColors[12][0]=255;sfxprefs_gui.ScreenColors[12][1]=255;sfxprefs_gui.ScreenColors[12][2]=  0;
	sfxprefs_gui.ScreenColors[13][0]=211;sfxprefs_gui.ScreenColors[13][1]=160;sfxprefs_gui.ScreenColors[13][2]=  0;
	sfxprefs_gui.ScreenColors[14][0]=  0;sfxprefs_gui.ScreenColors[14][1]=168;sfxprefs_gui.ScreenColors[14][2]=  0;
	sfxprefs_gui.ScreenColors[15][0]=  0;sfxprefs_gui.ScreenColors[15][1]= 97;sfxprefs_gui.ScreenColors[15][2]=  0;
	for(i=16;i<SFXPrefs_Gui_MaxColors;i++) sfxprefs_gui.ScreenColors[i][0]=sfxprefs_gui.ScreenColors[i][1]=sfxprefs_gui.ScreenColors[i][2]=0x00;

	//-- now read in config
	CfgIO_ReadConfig(cfg,fn);
	MSG1("cfg [%s] has been read",(fn?fn:""));

#ifdef CHECK_REG
	SetKeyStatus_3;
	SetKeyStatus2_0;
	ERRC4(!IsKeyStatusOkay,"!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
	if(IsKeyStatusIllegal_2) {
		char fn[256];
		
		MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
		OpenLibrary("configio.library ",3);
		sprintf(fn,"configio.library >= V 3.1 (errcode : %08lx)",RunTime.keyerr);
		StdCError(fn,errAccessFile,ENOENT,__FILE__,114+(RunTime.keynr^279));      // to make it not too obvious
#ifndef CHECK_NOCRASH
		KillAllTasks();
#endif
//      RunTime.status=STAT_DEMO;
		return;
	}
#endif

	strncpy(sfxprefs_vmem.swap_path,    CfgIO_ReadString         (cfg,"VirtualMemory"   ,"SwapPath"     ,"progdir:"),SFXPrefs_VMem_SwapPathLen-1);sfxprefs_vmem.swap_path[SFXPrefs_VMem_SwapPathLen-1]='\0';
	sfxprefs_vmem.blksize=              CfgIO_ReadUnsignedInteger(cfg,"VirtualMemory"   ,"BlockSize"    ,VM_BUFSIZE);
	sfxprefs_vmem.enabled=              CfgIO_ReadBool           (cfg,"VirtualMemory"   ,"Enabled"      ,FALSE);
//  MSG("\tvmem section has been read");

	sfxprefs_misc.sig_a=                CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"SignalAudio"  ,FALSE);
	sfxprefs_misc.sig_s=                CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"SignalScreen" ,TRUE);
	sfxprefs_misc.backlogo=             CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"BackLogo"     ,TRUE);
	sfxprefs_misc.showtips=             CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"ShowTips"		,TRUE);
	sfxprefs_misc.realmem=              CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"ShowRealMem"  ,TRUE);
	sfxprefs_misc.virtmem=              CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"ShowVirtMem"  ,TRUE);
//  sfxprefs_misc.keynr=                CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"KeyNumber"    ,TRUE);
	sfxprefs_misc.srcselect=            CfgIO_MatchString(
										CfgIO_ReadString         (cfg,"Miscellaneous"   ,"SrcSelect"    ,SrcSelLabels[0]),SrcSelLabels);
	sfxprefs_misc.contextbutton=        CfgIO_MatchString(
										CfgIO_ReadString         (cfg,"Miscellaneous"   ,"ContextButton",ContextButtonLabels[0]),ContextButtonLabels);
	sfxprefs_misc.askexit=              CfgIO_MatchString(
										CfgIO_ReadString         (cfg,"Miscellaneous"   ,"AskExit"      ,AskExitLabels[ASKEXIT_ALWAYS]),AskExitLabels);
	sfxprefs_misc.onewindow=            CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"OneWindow"    ,FALSE);
	sfxprefs_misc.logmode=              CfgIO_MatchString(
										CfgIO_ReadString         (cfg,"Miscellaneous"   ,"LogMode"      ,LogModeLabels[LOGMODE_NEVER]),LogModeLabels);
	strncpy(sfxprefs_misc.log_path,     CfgIO_ReadString         (cfg,"Miscellaneous"   ,"LogPath"      ,"T:sfx.log"),SFXPrefs_Misc_LogPathLen-1);sfxprefs_misc.log_path[SFXPrefs_Misc_LogPathLen-1]='\0';
	RunTime.onewindow=sfxprefs_misc.onewindow;
	sfxprefs_misc.saveicons=            CfgIO_ReadBool           (cfg,"Miscellaneous"   ,"SaveIcons"    ,TRUE);
	sfxprefs_misc.deftool=              CfgIO_MatchString(
										CfgIO_ReadString         (cfg,"Miscellaneous"   ,"DefTool"      ,DefToolLabels[1]),DefToolLabels);
//  MSG("\tmisc section has been read");

	sfxprefs_gui.scrmode=               CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"ScreenMode"     ,INVALID_ID);
	sfxprefs_gui.xres=                  CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"XRes"           ,0);
	sfxprefs_gui.yres=                  CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"YRes"           ,0);
	sfxprefs_gui.depth=                 CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"Depth"          ,0);
	sfxprefs_gui.oscan=                 CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"OverScan"       ,0);
	sfxprefs_gui.autoscroll=            CfgIO_ReadBool           (cfg,"GUI"         ,"AutoScroll"     ,0);
	strncpy(sfxprefs_gui.scrfont_name,  CfgIO_ReadString         (cfg,"GUI"         ,"FontName"       ,"Trinomic.font"),SFXPrefs_Gui_FontNameLen-1);sfxprefs_gui.scrfont_name[SFXPrefs_Gui_FontNameLen-1]='\0';
	sfxprefs_gui.scrfont_size=          CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"FontSize"       ,6);
	strncpy(sfxprefs_gui.statfont_name, CfgIO_ReadString         (cfg,"GUI"         ,"StatusFontName" ,"Trinomic.font"),SFXPrefs_Gui_FontNameLen-1);sfxprefs_gui.statfont_name[SFXPrefs_Gui_FontNameLen-1]='\0';
	sfxprefs_gui.statfont_size=         CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"StatusFontSize" ,6);
	for(i=0;i<SFXPrefs_Gui_MaxColors;i++) {
		sprintf(strItemName,"Color%02d",i);
		sprintf(strItemData1,"%3d,%3d,%3d",sfxprefs_gui.ScreenColors[i][0],sfxprefs_gui.ScreenColors[i][1],sfxprefs_gui.ScreenColors[i][2]);
		strItemData2=                       CfgIO_ReadString          (cfg,"GUI"            ,strItemName    ,strItemData1);
		sfxprefs_gui.ScreenColors[i][0]=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
		sfxprefs_gui.ScreenColors[i][1]=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
		sfxprefs_gui.ScreenColors[i][2]=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	}
	sfxprefs_gui.Pens[PEN_GUI_BACK]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_Back"          ,5);
	sfxprefs_gui.Pens[PEN_GUI_SHINE]=           CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_Shine"         ,2);
	sfxprefs_gui.Pens[PEN_GUI_SHADOW]=          CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_Shadow"        ,6);
	sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT]=       CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_Highlight"     ,3);
	sfxprefs_gui.Pens[PEN_GUI_COLORTEXT]=       CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_ColorText"     ,7);
	sfxprefs_gui.Pens[PEN_GUI_COLORTEXTGLOW]=   CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_ColorTextGlow" ,0);
	sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]=   CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_GradientStart" ,6);
	sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND]=     CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenGUI_GradientEnd"   ,0);

	sfxprefs_gui.Pens[PEN_SMP_BACK]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Back"          ,1);
	sfxprefs_gui.Pens[PEN_SMP_TEXT]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Text"          ,2);
	sfxprefs_gui.Pens[PEN_SMP_LINE]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Line"          ,4);
	sfxprefs_gui.Pens[PEN_SMP_GRID]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Grid"          ,6);
	sfxprefs_gui.Pens[PEN_SMP_MAXL]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_MaxL"          ,7);
	sfxprefs_gui.Pens[PEN_SMP_RMSL]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_RmsL"          ,7);
	sfxprefs_gui.Pens[PEN_SMP_AVGL]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_AvgL"          ,7);
	sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]=       CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Highlight"     ,3);
	sfxprefs_gui.Pens[PEN_SMP_LOOP]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Loop"          ,3);
	sfxprefs_gui.Pens[PEN_SMP_MARK]=            CfgIO_ReadUnsignedInteger(cfg,"GUI"         ,"PenSMP_Mark"          ,3);
//  MSG("\tgui  section readed");

	sfxprefs_sbuf.drawmode=                 CfgIO_MatchString(
											CfgIO_ReadString         (cfg,"SampleBuffer"    ,"DrawMode"         ,DrawModeLabels[DM_LINES]),DrawModeLabels);
	sfxprefs_sbuf.maxlines=                 CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"MaxLines"         ,TRUE);
	sfxprefs_sbuf.rmslines=                 CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"RmsLines"         ,TRUE);
	sfxprefs_sbuf.avglines=                 CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"AvgLines"         ,TRUE);
	sfxprefs_sbuf.unitx=                    CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"UnitX"            ,ATME_SAMPLES);
	sfxprefs_sbuf.unity=                    CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"UnitY"            ,AMP_LEVEL);
	sfxprefs_sbuf.rasterx=                  CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"RasterX"          ,TRUE);
	sfxprefs_sbuf.rastery=                  CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"RasterY"          ,TRUE);
	sfxprefs_sbuf.scalax=                   CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"ScalaX"           ,TRUE);
	sfxprefs_sbuf.scalay=                   CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"ScalaY"           ,TRUE);
	strncpy(sfxprefs_sbuf.scafont_name,     CfgIO_ReadString         (cfg,"SampleBuffer"    ,"ScalaFontName"    ,"Trinomic.font"),SFXPrefs_SBuf_FontNameLen-1);sfxprefs_sbuf.scafont_name[SFXPrefs_SBuf_FontNameLen-1]='\0';
	sfxprefs_sbuf.scafont_size=             CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"ScalaFontSize"    ,6);
	sfxprefs_sbuf.safemode=                 CfgIO_MatchString(
											CfgIO_ReadString         (cfg,"SampleBuffer"    ,"SafeMode"         ,SafeLabels[0]),SafeLabels);
	sfxprefs_sbuf.quickdraw=                CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"QuickDraw"        ,TRUE);

	strncpy(sample_path,                    CfgIO_ReadString         (cfg,"SampleBuffer"    ,"SamplePath"       ,"progdir:"),SFXPrefs_SBuf_SamplePathLen-1);sample_path[SFXPrefs_SBuf_SamplePathLen-1]='\0';
	strncpy(sfxprefs_sbuf.sample_lo_path,   CfgIO_ReadString         (cfg,"SampleBuffer"    ,"SampleLoaderPath" ,sample_path),SFXPrefs_SBuf_SamplePathLen-1);sfxprefs_sbuf.sample_lo_path[SFXPrefs_SBuf_SamplePathLen-1]='\0';
	strncpy(sfxprefs_sbuf.sample_sa_path,   CfgIO_ReadString         (cfg,"SampleBuffer"    ,"SampleSaverPath"  ,sample_path),SFXPrefs_SBuf_SamplePathLen-1);sfxprefs_sbuf.sample_sa_path[SFXPrefs_SBuf_SamplePathLen-1]='\0';
	strcpy(lopath,sfxprefs_sbuf.sample_lo_path);strcpy(sapath,sfxprefs_sbuf.sample_sa_path);

	sfxprefs_sbuf.storage=                  CfgIO_MatchString(
											CfgIO_ReadString         (cfg,"SampleBuffer"    ,"Storage"          ,StorageLabels[VM_MEMORY]),StorageLabels);
	sfxprefs_sbuf.xs1=                      CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"NormalXRes"       ,280);
	sfxprefs_sbuf.ys1=                      CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"NormalYRes"       ,64);
	sfxprefs_sbuf.xs2=                      CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"ZoomedXRes"       ,80);
	sfxprefs_sbuf.ys2=                      CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"ZoomedYRes"       ,16);
	sfxprefs_sbuf.mode=                     CfgIO_MatchString(
											CfgIO_ReadString         (cfg,"SampleBuffer"    ,"SizeMode"         ,SizeModeLabels[0]),SizeModeLabels);

	sfxprefs_sbuf.nolines=                  CfgIO_ReadBool           (cfg,"SampleBuffer"    ,"NoLines"          ,TRUE);
	sfxprefs_sbuf.thrsize=                  CfgIO_ReadUnsignedInteger(cfg,"SampleBuffer"    ,"ThreshSize"       ,5000000);

	sprintf(strItemData1,"(C) by %s",RunTime.username);
	strcpy(sfxprefs_sbuf.copystr,           CfgIO_ReadString         (cfg,"SampleBuffer"    ,"CopyrightString"  ,strItemData1));
	strcpy(sfxprefs_sbuf.authstr,           CfgIO_ReadString         (cfg,"SampleBuffer"    ,"AuthorString"     ,RunTime.username));
//  MSG("\tsbuf section has been read");

	strItemData2=                           CfgIO_ReadString         (cfg,"Windows"     ,"Operators"        ,"510,  9,124,233,1,Amplify");
	sfxprefs_win.opwin.x1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.opwin.y1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.opwin.x2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.opwin.y2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.opwin.state=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	strncpy(sfxprefs_win.opwin.entryname,CfgIO_GetSubItem(strItemData2,&strItemData2,','),49);sfxprefs_win.opwin.entryname[49]='\0';
	strItemData2=                           CfgIO_ReadString         (cfg,"Windows"     ,"RexxOperators"    ,"391,187,113, 57,1,Info");
	sfxprefs_win.rxopwin.x1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.rxopwin.y1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.rxopwin.x2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.rxopwin.y2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.rxopwin.state=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	strncpy(sfxprefs_win.rxopwin.entryname,CfgIO_GetSubItem(strItemData2,&strItemData2,','),49);sfxprefs_win.rxopwin.entryname[49]='\0';
	strItemData2=                           CfgIO_ReadString         (cfg,"Windows"     ,"Loaders"          ,"391,  9,113, 57,1,Universal");
	sfxprefs_win.lowin.x1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.lowin.y1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.lowin.x2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.lowin.y2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.lowin.state=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	strncpy(sfxprefs_win.lowin.entryname,CfgIO_GetSubItem(strItemData2,&strItemData2,','),49);sfxprefs_win.lowin.entryname[49]='\0';
	strItemData2=                           CfgIO_ReadString         (cfg,"Windows"     ,"Savers"           ,"391, 74,113, 57,1,RAW");
	sfxprefs_win.sawin.x1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.sawin.y1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.sawin.x2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.sawin.y2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.sawin.state=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	strncpy(sfxprefs_win.sawin.entryname,CfgIO_GetSubItem(strItemData2,&strItemData2,','),49);sfxprefs_win.sawin.entryname[49]='\0';
	strItemData2=                           CfgIO_ReadString         (cfg,"Windows"     ,"Players"          ,"391,139,113, 39,1,Amiga-14bit");
	sfxprefs_win.plwin.x1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.plwin.y1=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.plwin.x2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));sfxprefs_win.plwin.y2=(UWORD)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	sfxprefs_win.plwin.state=(UBYTE)atoi(CfgIO_GetSubItem(strItemData2,&strItemData2,','));
	strncpy(sfxprefs_win.plwin.entryname,CfgIO_GetSubItem(strItemData2,&strItemData2,','),49);sfxprefs_win.plwin.entryname[49]='\0';
//  MSG("\twin section  has been read");

	optp_Amiga_8bit.hfilter=                CfgIO_ReadBool           (cfg,"Player_Amiga8"   ,"HardwareFilter"   ,TRUE);
	optp_Amiga_8bit.rateclip=               CfgIO_ReadUnsignedInteger(cfg,"Player_Amiga8"   ,"RateClip"         ,0);
	optp_Amiga_14bit.hfilter=               CfgIO_ReadBool           (cfg,"Player_Amiga14"  ,"HardwareFilter"   ,TRUE);
	optp_Amiga_14bit.rateclip=              CfgIO_ReadUnsignedInteger(cfg,"Player_Amiga14"  ,"RateClip"         ,0);
	optp_Amiga_14cbit.hfilter=              CfgIO_ReadBool           (cfg,"Player_Amiga14c" ,"HardwareFilter"   ,TRUE);
	optp_Amiga_14cbit.rateclip=             CfgIO_ReadUnsignedInteger(cfg,"Player_Amiga14c" ,"RateClip"         ,0);
	optp_Ahi.playmode=                      CfgIO_ReadUnsignedInteger(cfg,"Player_Ahi"      ,"Mode"             ,AHI_DEFAULT_ID);
	optp_Ahi.playrate=                      CfgIO_ReadUnsignedInteger(cfg,"Player_Ahi"      ,"Rate"             ,22050);
	optp_Ahi.recomode=                      CfgIO_ReadUnsignedInteger(cfg,"Sampler_Ahi"     ,"Mode"             ,AHI_DEFAULT_ID);
	optp_Ahi.recorate=                      CfgIO_ReadUnsignedInteger(cfg,"Sampler_Ahi"     ,"Rate"             ,22050);
//  MSG("\tplayer  section has been read");

	if(!IsKeyStatusOkay) {     // just in case someone copied someone elses config-file or played around with the keyfile
		// disable vmem
		sfxprefs_vmem.enabled=FALSE;
		sfxprefs_sbuf.storage=VM_MEMORY;

		// reset infostrings
		sprintf(RunTime.username,"SoundFX");
		sprintf(sfxprefs_sbuf.copystr,"(C) by %s",RunTime.username);
		strcpy(sfxprefs_sbuf.authstr,RunTime.username);
	}

	OUTRO;
}

void WritePrefs(STRPTR fn)
{
	register UWORD i;
	char strItemName[20],strItemData[200];

	if(RunTime.winBackDrop) {           /// avoid all this below, when we're just resaving the prefs
#ifdef CHECK_REG
		ERRC4(!IsKeyStatusOkay,"!!! DEMO !!! : %d %d %d %d",RunTime.status,RunTime.status2,RunTime.status3,RunTime.keyerr);
		if(IsKeyStatusIllegal_1 || IsKeyStatusIllegal_2) {
			char fn[256];
			MSG3("KeyTrap : %ld,%ld,%ld",RunTime.status,RunTime.status2,RunTime.status3);
			OpenLibrary("conFigio.library ",1);
			sprintf(fn,"configio.library >= V 1.1 (errcode : %08lx)",RunTime.keyerr);
			StdCError(fn,errAccessFile,ENOENT,__FILE__,114+(RunTime.keynr^279));      // to make it not too obvious
#ifndef CHECK_NOCRASH
			KillAllTasks();
#endif
//          RunTime.status=STAT_DEMO;
			return;
		}
#endif
	}

	CfgIO_WriteString         (cfg,"VirtualMemory"  ,"SwapPath"     ,sfxprefs_vmem.swap_path);
	CfgIO_WriteUnsignedInteger(cfg,"VirtualMemory"  ,"BlockSize"    ,sfxprefs_vmem.blksize);
	CfgIO_WriteBool           (cfg,"VirtualMemory"  ,"Enabled"      ,sfxprefs_vmem.enabled);
//  MSG("\tvmem section written");

	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"SignalAudio"  ,sfxprefs_misc.sig_a);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"SignalScreen" ,sfxprefs_misc.sig_s);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"BackLogo"     ,sfxprefs_misc.backlogo);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"ShowTips" 	,sfxprefs_misc.showtips);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"ShowRealMem"  ,sfxprefs_misc.realmem);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"ShowVirtMem"  ,sfxprefs_misc.virtmem);
//  CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"KeyNumber"    ,sfxprefs_misc.keynr);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"SrcSelect"    ,SrcSelLabels[sfxprefs_misc.srcselect]);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"ContextButton",ContextButtonLabels[sfxprefs_misc.contextbutton]);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"AskExit"		,AskExitLabels[sfxprefs_misc.askexit]);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"OneWindow"    ,sfxprefs_misc.onewindow);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"LogMode"      ,LogModeLabels[sfxprefs_misc.logmode]);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"LogPath"      ,sfxprefs_misc.log_path);
	CfgIO_WriteBool           (cfg,"Miscellaneous"  ,"SaveIcons"    ,sfxprefs_misc.saveicons);
	CfgIO_WriteString         (cfg,"Miscellaneous"  ,"DefTool"      ,DefToolLabels[sfxprefs_misc.deftool]);
	//  MSG("\tmisc section written");

	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"ScreenMode"     ,sfxprefs_gui.scrmode);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"XRes"           ,sfxprefs_gui.xres);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"YRes"           ,sfxprefs_gui.yres);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"Depth"          ,sfxprefs_gui.depth);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"OverScan"       ,sfxprefs_gui.oscan);
	CfgIO_WriteBool           (cfg,"GUI"            ,"AutoScroll"     ,sfxprefs_gui.autoscroll);
	CfgIO_WriteString         (cfg,"GUI"            ,"FontName"       ,sfxprefs_gui.scrfont_name);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"FontSize"       ,sfxprefs_gui.scrfont_size);
	CfgIO_WriteString         (cfg,"GUI"            ,"StatusFontName" ,sfxprefs_gui.statfont_name);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"StatusFontSize" ,sfxprefs_gui.statfont_size);
	for(i=0;i<SFXPrefs_Gui_MaxColors;i++) {
		sprintf(strItemName,"Color%02d",i);
		sprintf(strItemData,"%3d,%3d,%3d",sfxprefs_gui.ScreenColors[i][0],sfxprefs_gui.ScreenColors[i][1],sfxprefs_gui.ScreenColors[i][2]);
		CfgIO_WriteString     (cfg,"GUI"            ,strItemName    ,strItemData);
	}
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_Back"          ,sfxprefs_gui.Pens[PEN_GUI_BACK]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_Shine"         ,sfxprefs_gui.Pens[PEN_GUI_SHINE]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_Shadow"        ,sfxprefs_gui.Pens[PEN_GUI_SHADOW]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_Highlight"     ,sfxprefs_gui.Pens[PEN_GUI_HIGHLIGHT]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_ColorText"     ,sfxprefs_gui.Pens[PEN_GUI_COLORTEXT]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_ColorTextGlow" ,sfxprefs_gui.Pens[PEN_GUI_COLORTEXTGLOW]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_GradientStart" ,sfxprefs_gui.Pens[PEN_GUI_GRADIENTSTART]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenGUI_GradientEnd"   ,sfxprefs_gui.Pens[PEN_GUI_GRADIENTEND]);
	
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Back"          ,sfxprefs_gui.Pens[PEN_SMP_BACK]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Text"          ,sfxprefs_gui.Pens[PEN_SMP_TEXT]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Line"          ,sfxprefs_gui.Pens[PEN_SMP_LINE]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Grid"          ,sfxprefs_gui.Pens[PEN_SMP_GRID]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_MaxL"          ,sfxprefs_gui.Pens[PEN_SMP_MAXL]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_RmsL"          ,sfxprefs_gui.Pens[PEN_SMP_RMSL]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_AvgL"          ,sfxprefs_gui.Pens[PEN_SMP_AVGL]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Highlight"     ,sfxprefs_gui.Pens[PEN_SMP_HIGHLIGHT]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Loop"          ,sfxprefs_gui.Pens[PEN_SMP_LOOP]);
	CfgIO_WriteUnsignedInteger(cfg,"GUI"            ,"PenSMP_Mark"          ,sfxprefs_gui.Pens[PEN_SMP_MARK]);
//  MSG("\tgui  section written");

	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"DrawMode"         ,DrawModeLabels[sfxprefs_sbuf.drawmode]);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"MaxLines"         ,sfxprefs_sbuf.maxlines);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"RmsLines"         ,sfxprefs_sbuf.rmslines);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"AvgLines"         ,sfxprefs_sbuf.avglines);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"UnitX"            ,sfxprefs_sbuf.unitx);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"UnitY"            ,sfxprefs_sbuf.unity);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"RasterX"          ,sfxprefs_sbuf.rasterx);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"RasterY"          ,sfxprefs_sbuf.rastery);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"ScalaX"           ,sfxprefs_sbuf.scalax);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"ScalaY"           ,sfxprefs_sbuf.scalay);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"ScalaFontName"    ,sfxprefs_sbuf.scafont_name);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"ScalaFontSize"    ,sfxprefs_sbuf.scafont_size);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"SafeMode"         ,SafeLabels[sfxprefs_sbuf.safemode]);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"QuickDraw"        ,sfxprefs_sbuf.quickdraw);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"SampleLoaderPath" ,sfxprefs_sbuf.sample_lo_path);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"SampleSaverPath"  ,sfxprefs_sbuf.sample_sa_path);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"Storage"          ,StorageLabels[sfxprefs_sbuf.storage]);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"NormalXRes"       ,sfxprefs_sbuf.xs1);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"NormalYRes"       ,sfxprefs_sbuf.ys1);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"ZoomedXRes"       ,sfxprefs_sbuf.xs2);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"ZoomedYRes"       ,sfxprefs_sbuf.ys2);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"SizeMode"         ,SizeModeLabels[sfxprefs_sbuf.mode]);
	CfgIO_WriteBool           (cfg,"SampleBuffer"   ,"NoLines"          ,sfxprefs_sbuf.nolines);
	CfgIO_WriteUnsignedInteger(cfg,"SampleBuffer"   ,"ThreshSize"       ,sfxprefs_sbuf.thrsize);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"CopyrightString"  ,sfxprefs_sbuf.copystr);
	CfgIO_WriteString         (cfg,"SampleBuffer"   ,"AuthorString"     ,sfxprefs_sbuf.authstr);
//  MSG("\sbuf section written");

	sprintf(strItemData,"0,0,0,0,0,%s",GetNameByIndex(&EffektList,RunTime.aktfx));
	CfgIO_WriteString         (cfg,"Windows"        ,"Operators"        ,strItemData);
	sprintf(strItemData,"0,0,0,0,0,%s",GetNameByIndex(&RexxList,RunTime.arxop));
	CfgIO_WriteString         (cfg,"Windows"        ,"RexxOperators"    ,strItemData);
	sprintf(strItemData,"0,0,0,0,0,%s",GetNameByIndex(&LoaderList,RunTime.aloader));
	CfgIO_WriteString         (cfg,"Windows"        ,"Loaders"          ,strItemData);
	sprintf(strItemData,"0,0,0,0,0,%s",GetNameByIndex(&SaverList,RunTime.asaver));
	CfgIO_WriteString         (cfg,"Windows"        ,"Savers"           ,strItemData);
	sprintf(strItemData,"0,0,0,0,0,%s",pllist[match_nr2ix(playeranz,RunTime.aplayer,pllist)].name);
	CfgIO_WriteString         (cfg,"Windows"        ,"Players"          ,strItemData);
//  MSG("\win  section written");

	CfgIO_WriteBool           (cfg,"Player_Amiga8"  ,"HardwareFilter"   ,optp_Amiga_8bit.hfilter);
	CfgIO_WriteUnsignedInteger(cfg,"Player_Amiga8"  ,"RateClip"         ,optp_Amiga_8bit.rateclip);
	CfgIO_WriteBool           (cfg,"Player_Amiga14" ,"HardwareFilter"   ,optp_Amiga_14bit.hfilter);
	CfgIO_WriteUnsignedInteger(cfg,"Player_Amiga14" ,"RateClip"         ,optp_Amiga_14bit.rateclip);
	CfgIO_WriteBool           (cfg,"Player_Amiga14c","HardwareFilter"   ,optp_Amiga_14cbit.hfilter);
	CfgIO_WriteUnsignedInteger(cfg,"Player_Amiga14c","RateClip"         ,optp_Amiga_14cbit.rateclip);
	CfgIO_WriteUnsignedInteger(cfg,"Player_Ahi"     ,"Mode"             ,optp_Ahi.playmode);
	CfgIO_WriteUnsignedInteger(cfg,"Player_Ahi"     ,"Rate"             ,optp_Ahi.playrate);
	CfgIO_WriteUnsignedInteger(cfg,"Sampler_Ahi"    ,"Mode"             ,optp_Ahi.recomode);
	CfgIO_WriteUnsignedInteger(cfg,"Sampler_Ahi"    ,"Rate"             ,optp_Ahi.recorate);
//  MSG("\ahi  section written");

	CfgIO_WriteConfig(cfg,fn);
//  MSG("cfg written");
}

//-- defaults

void SetDefPOptions(int nr)
{
	switch(nr) {
		case 0:         /* OPTP-Amiga_8bit */
			optp_Amiga_8bit.hfilter=TRUE;
			optp_Amiga_8bit.rateclip=FALSE;
			break;
		case 1:         /* OPTP-Amiga_14bit */
			optp_Amiga_14bit.hfilter=TRUE;
			optp_Amiga_14bit.rateclip=FALSE;
			break;
		case 2:         /* OPTP-Amiga_14cbit */
			optp_Amiga_14cbit.hfilter=TRUE;
			optp_Amiga_14cbit.rateclip=FALSE;
			break;
		case 3:         /* OPTP-Ahi -- let AHI decide about an initial mode = music unit */
//            optp_Ahi.playmode=AHI_INVALID_ID;
			optp_Ahi.playmode=AHI_DEFAULT_ID;
			optp_Ahi.playrate=22050;
			break;
	}
}

//-- eof ----------------------------------------------------------------------
