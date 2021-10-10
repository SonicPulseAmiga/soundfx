/****h* SoundFX/SFX-GUIMenu.c [4.3] *
*
*  NAME
*    SFX-GUIMenu.c
*  COPYRIGHT
*    $VER: SFX-GUIMenu.c 4.3 (13.10.03) by Stefan Kost 1998-2003
*  FUNCTION
*    gui menu routines - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Jan.2002
*  MODIFICATION HISTORY
*    13.Oct.2003	V 4.3	most recent version
*    10.Sep.2002    V 4.2   most recent version
*    21.Jan.2002    V 4.2	initial version
*  NOTES
*
*******
*/

#define SFX_GUIMenu_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- global Menu handling

void Test_Menu(ULONG mitem,UWORD iqual);
void Refr_GlobalMenu(void);
void Refr_GlobalMRUMenu(void);
void Refr_MRUMenu(void);

//-- helper

void Lock_GlobalMenu(void);
void Unlock_GlobalMenu(void);

void Uncheck_MenuItem(ULONG itemnr);
void Check_MenuItem(ULONG itemnr);

void Disable_MenuItem(ULONG itemnr);
void Enable_MenuItem(ULONG itemnr);

//-- Help Menu handling

void ShowHelpMail(void);
void ShowHelpAbout(void);

//-- Quit handling

void TryQuit(void);

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

//-- global Menu handling

void Test_Menu(ULONG mitem,UWORD iqual) {
	struct MenuItem     *item;
	UBYTE esc=0;
	char fname[FILENAME_MAX];

	while(mitem!=MENUNULL && !esc) {
		if(item=ItemAddress(RunTime.Menu,mitem)) {
			switch((ULONG)MENU_USERDATA(item)) {
				case MAIN_MENID_PRJ_NEW:			SwitchIDCMPOFF();NewSampleWin();SwitchIDCMPON();break;
				case MAIN_MENID_PRJ_LOAD:			SwitchIDCMPOFF();LoadSample();SwitchIDCMPON();break;	/** @TODO: add LoadSampleInterface() */
				case MAIN_MENID_PRJ_SAVE:			if(RunTime.aktsamples) { SwitchIDCMPOFF();SaveSample();SwitchIDCMPON(); }break;	/** @TODO: add SaveSampleInterface() */
				case MAIN_MENID_PRJ_FLUSHALL:		if(RunTime.aktsamples && Question(reqRemoveAll,GetString(strYes),GetString(strNo))) { FlushAllSamples();RunTime.samplestat=SMPSTAT_LASTOLD; }break;
				case MAIN_MENID_PRJ_FLUSHHIDDEN:    if(RunTime.aktsamples && Question(reqRemoveHidden,GetString(strYes),GetString(strNo))) { FlushHiddenSamples();RunTime.samplestat=SMPSTAT_LASTOLD; }break;
				case MAIN_MENID_PRJ_FLUSHSHOWN:		if(RunTime.aktsamples && Question(reqRemoveShown,GetString(strYes),GetString(strNo))) { FlushShownSamples();RunTime.samplestat=SMPSTAT_LASTOLD; }break;
				case MAIN_MENID_PRJ_RUNOP:
					if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) { SwitchIDCMPOFF();StartRexx();SwitchIDCMPON(); }
					else { SwitchIDCMPOFF();ProcessSample();SwitchIDCMPON(); }
					break;
				case MAIN_MENID_PRJ_RUNRX:			SwitchIDCMPOFF();StartRexx();SwitchIDCMPON();break;
				case MAIN_MENID_PRJ_PLAYALL:
					if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) PlaySample_Rng();
					else PlaySample_All();
					break;
				case MAIN_MENID_PRJ_PLAYRNG:		PlaySample_Rng();break;
				case MAIN_MENID_PRJ_PLAYSTOP:		if(RunTime.play) StopSample();break;
				case MAIN_MENID_PRJ_RECORD:			SwitchIDCMPOFF();RecordWin();SwitchIDCMPON();break;
				case MAIN_MENID_PRJ_BATCHPROC:		SwitchIDCMPOFF();BatchProcWin();SwitchIDCMPON();if(!RunTime.aktsamples) RunTime.samplestat=SMPSTAT_LASTOLD;break;
				case MAIN_MENID_PRJ_INFO:			SwitchIDCMPOFF();InfoWin();SwitchIDCMPON();break;
				case MAIN_MENID_PRJ_MRU1:			strcpy(fname,MRU_GetFileName(1));LoadSampleRexx(fname);break;
				case MAIN_MENID_PRJ_MRU2:			strcpy(fname,MRU_GetFileName(2));LoadSampleRexx(fname);break;
				case MAIN_MENID_PRJ_MRU3:			strcpy(fname,MRU_GetFileName(3));LoadSampleRexx(fname);break;
				case MAIN_MENID_PRJ_MRU4:			strcpy(fname,MRU_GetFileName(4));LoadSampleRexx(fname);break;
				case MAIN_MENID_PRJ_MRU5:			strcpy(fname,MRU_GetFileName(5));LoadSampleRexx(fname);break;
				case MAIN_MENID_PRJ_QUIT:			TryQuit();break;

				case MAIN_MENID_EDIT_CUT:			if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_CUTB:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_CUTE:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Cut(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_COPY:			if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_COPYB:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_COPYE:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Copy(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_GRAB:			if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_GRABB:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_GRABE:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Grab(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_PASTE:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_PASTEB:		SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON();break;
				case MAIN_MENID_EDIT_PASTEE:		SwitchIDCMPOFF();Edit_Paste(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON();break;
				case MAIN_MENID_EDIT_ERASE:			if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_ERASEB:		if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_ERASEE:		if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Erase(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_ZERO:			if(RunTime.aktbuf->mark && RunTime.aktbuf->markxl>0) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_ZEROB:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_ZEROE:			if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Zero(RunTime.aktbuf,EDIT_2END);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_OVERWRITE:		if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Overwrite(RunTime.aktbuf,EDIT_RANGE);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_OVERWRITEB:	Edit_Overwrite(RunTime.aktbuf,EDIT_2BEGIN);SwitchIDCMPON();break;
				case MAIN_MENID_EDIT_EXCHANGE:		if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Exchange(RunTime.aktbuf);SwitchIDCMPON(); } break;
				case MAIN_MENID_EDIT_REPLACE:		if(RunTime.aktbuf->mark) { SwitchIDCMPOFF();Edit_Replace(RunTime.aktbuf);SwitchIDCMPON(); } break;

				case MAIN_MENID_RNG_LOOP2MARK:		if( RunTime.aktbuf->loop) Range_Loop2Mark(RunTime.aktbuf);break;
				case MAIN_MENID_RNG_MARK2LOOP:		if((RunTime.aktbuf->mark) && (RunTime.aktbuf->markxl>1)) Range_Mark2Loop(RunTime.aktbuf);break;
				case MAIN_MENID_RNG_ZOOM2LOOP:		Range_Zoom2Loop(RunTime.aktbuf);break;
				case MAIN_MENID_RNG_ZOOM2MARK:		Range_Zoom2Mark(RunTime.aktbuf);break;
				case MAIN_MENID_RNG_MARKOFF:		if(RunTime.aktbuf->mark) Range_MarkOff(RunTime.aktbuf);break;
				case MAIN_MENID_RNG_MARKALL:		Range_MarkAll(RunTime.aktbuf);break;

				case MAIN_MENID_ZOOM_IN:			ZoomIn(RunTime.aktbuf,RunTime.zoommode+1);break;
				case MAIN_MENID_ZOOM_PIXEL:			ZoomPixel(RunTime.aktbuf,RunTime.zoommode+1);break;
				case MAIN_MENID_ZOOM_OUT:			ZoomOut(RunTime.aktbuf,RunTime.zoommode+1);break;
				case MAIN_MENID_ZOOM_FULL:			ZoomFull(RunTime.aktbuf,RunTime.zoommode+1);break;

				case MAIN_MENID_CL_CURR:			CleanUpCurr();break;
				case MAIN_MENID_CL_ALL:				CleanUpAll();break;
				case MAIN_MENID_CL_ALLNORM:			CleanUpAllNorm();break;
				case MAIN_MENID_CL_ALLZOOM:			CleanUpAllZoom();break;

				case MAIN_MENID_UT_SWAPEND:			SwitchIDCMPOFF();UtilitiesSwapEndian(RunTime.aktbuf);	SwitchIDCMPON();break;
				case MAIN_MENID_UT_SWAPSIGN:		SwitchIDCMPOFF();UtilitiesSwapSign(RunTime.aktbuf);		SwitchIDCMPON();break;
				case MAIN_MENID_UT_INTERLEAVE:		SwitchIDCMPOFF();UtilitiesInterleave(RunTime.aktbuf);	SwitchIDCMPON();break;
				case MAIN_MENID_UT_DEINTERLEAVE:	SwitchIDCMPOFF();UtilitiesDeInterleave(RunTime.aktbuf);	SwitchIDCMPON();break;

				case MAIN_MENID_PREFS_GUI:			SwitchIDCMPOFF();esc=PrefsGUI(); SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_SAMP:			SwitchIDCMPOFF();esc=PrefsSamp();SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_VMEM:			SwitchIDCMPOFF();esc=PrefsVMem();SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_MISC:			SwitchIDCMPOFF();esc=PrefsMisc();SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_USE:		    SwitchIDCMPOFF();WritePrefs("ENV:sfx.cfg");SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_SAVE:			SwitchIDCMPOFF();WritePrefs("ENVARC:sfx.cfg");WritePrefs("ENV:sfx.cfg");SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_RESTOREUSED:	SwitchIDCMPOFF();RunTime.tryreopen=esc=TRUE;ReadPrefs("ENVARC:sfx.cfg");ReadPrefs("ENV:sfx.cfg");SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_RESTORESAVED:	SwitchIDCMPOFF();RunTime.tryreopen=esc=TRUE;ReadPrefs("ENVARC:sfx.cfg");SwitchIDCMPON();break;
				case MAIN_MENID_PREFS_DEFAULT:	    SwitchIDCMPOFF();RunTime.tryreopen=esc=TRUE;ReadPrefs(NULL);SwitchIDCMPON();break;

				case MAIN_MENID_HELP_CT:			ShowHelp("html/nodes/node__.__.__.html");break;
				case MAIN_MENID_HELP_IX:			ShowHelp("html/nodes/node__.__.__.html");break;
				case MAIN_MENID_HELP_WS:			ShowHelp("html/nodes/node05.__.__.html");break;
				case MAIN_MENID_HELP_ONLINE:		ShowHelp(GetString(urlHome));break;
				case MAIN_MENID_HELP_MAIL:			ShowHelpMail();break;
				case MAIN_MENID_HELP_SHOWTIPS:		sfxprefs_misc.showtips=(item->Flags&CHECKED?1:0);break;
				case MAIN_MENID_HELP_ABOUT:			ShowHelpAbout();break;
			}
			mitem=item->NextSelect;
		}
	}
}

void Refr_GlobalMenu(void) {
	struct MenuItem *item;

	INTRO;
	Lock_GlobalMenu();
	if(RunTime.aktbuf) {
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 2,0))) item->Flags|=ITEMENABLED;        // Save
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,0))) item->Flags|=ITEMENABLED;        // Flush
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,0))) item->Flags|=ITEMENABLED;        // FlushAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,1))) item->Flags|=ITEMENABLED;        // FlushHidden
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,2))) item->Flags|=ITEMENABLED;        // FlushShown
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 9,0))) item->Flags|=ITEMENABLED;        // PlayAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0,10,0))) item->Flags|=ITEMENABLED;        // PlayRng
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0,11,0))) item->Flags|=ITEMENABLED;        // Stop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 0,0))) item->Flags|=ITEMENABLED;        // Cut
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 1,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 2,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 3,0))) item->Flags|=ITEMENABLED;        // Copy
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 4,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 5,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 6,0))) item->Flags|=ITEMENABLED;        // Grab
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 7,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 8,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 9,0))) item->Flags|=ITEMENABLED;        // Paste
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,10,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,11,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,12,0))) item->Flags|=ITEMENABLED;        // Erase
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,13,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,14,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,15,0))) item->Flags|=ITEMENABLED;        // Zero
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,16,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,17,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,18,0))) item->Flags|=ITEMENABLED;        // Overwrite
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,19,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,21,0))) item->Flags|=ITEMENABLED;        // Exchange
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,22,0))) item->Flags|=ITEMENABLED;        // Replace
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 0,0))) item->Flags|=ITEMENABLED;        // Mark2Loop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 1,0))) item->Flags|=ITEMENABLED;        // Loop2Mark
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 2,0))) item->Flags|=ITEMENABLED;        // Zoom2Loop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 3,0))) item->Flags|=ITEMENABLED;        // Zoom2Mark
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 5,0))) item->Flags|=ITEMENABLED;        // MarkOff
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 6,0))) item->Flags|=ITEMENABLED;        // MarkAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 0,0))) item->Flags|=ITEMENABLED;        // Zoom
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 1,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 2,0))) item->Flags|=ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 3,0))) item->Flags|=ITEMENABLED;
		if(RunTime.onewindow) {																	// CleanUp
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 0,0))) item->Flags&=~ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 1,0))) item->Flags&=~ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 2,0))) item->Flags&=~ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 3,0))) item->Flags&=~ITEMENABLED;
		}
		else {
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 0,0))) item->Flags|=ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 1,0))) item->Flags|=ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 2,0))) item->Flags|=ITEMENABLED;
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 3,0))) item->Flags|=ITEMENABLED;
		}
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 0,0))) item->Flags|=ITEMENABLED;        // Utilities
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 1,0))) item->Flags|=ITEMENABLED;
		if(RunTime.aktbuf->channels>CH_MONO) {													// interleave chnnels
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 3,0))) item->Flags|=ITEMENABLED;
		}
		else {
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 3,0))) item->Flags&=~ITEMENABLED;
		}
		if(RunTime.aktbuf->channels<CH_QUADRO) {												// de-interleave chnnels
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 4,0))) item->Flags|=ITEMENABLED;
		}
		else {
			if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 4,0))) item->Flags&=~ITEMENABLED;
		}
	}
	else {
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 2,0))) item->Flags&=~ITEMENABLED;       // Save
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,0))) item->Flags&=~ITEMENABLED;       // Flush
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,0))) item->Flags&=~ITEMENABLED;       // FlushAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,1))) item->Flags&=~ITEMENABLED;       // FlushHidden
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 4,2))) item->Flags&=~ITEMENABLED;       // FlushShown
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0, 9,0))) item->Flags&=~ITEMENABLED;       // PlayAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0,10,0))) item->Flags&=~ITEMENABLED;       // PlayRng
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0,11,0))) item->Flags&=~ITEMENABLED;       // Stop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 0,0))) item->Flags&=~ITEMENABLED;       // Cut
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 1,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 2,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 3,0))) item->Flags&=~ITEMENABLED;       // Copy
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 4,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 5,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 6,0))) item->Flags&=~ITEMENABLED;       // Grab
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 7,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 8,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1, 9,0))) item->Flags&=~ITEMENABLED;       // Paste
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,10,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,11,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,12,0))) item->Flags&=~ITEMENABLED;       // Erase
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,13,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,14,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,15,0))) item->Flags&=~ITEMENABLED;       // Zero
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,16,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,17,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,18,0))) item->Flags&=~ITEMENABLED;       // Overwrite
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,19,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,21,0))) item->Flags&=~ITEMENABLED;       // Exchange
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(1,22,0))) item->Flags&=~ITEMENABLED;       // Replace
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 0,0))) item->Flags&=~ITEMENABLED;       // Mark2Loop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 1,0))) item->Flags&=~ITEMENABLED;       // Loop2Mark
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 2,0))) item->Flags&=~ITEMENABLED;       // Zoom2Loop
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 3,0))) item->Flags&=~ITEMENABLED;       // Zoom2Mark
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 5,0))) item->Flags&=~ITEMENABLED;       // MarkOff
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(2, 6,0))) item->Flags&=~ITEMENABLED;       // MarkAll
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 0,0))) item->Flags&=~ITEMENABLED;       // Zoom
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 1,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 2,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(3, 3,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 0,0))) item->Flags&=~ITEMENABLED;       // CleanUp
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 1,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 2,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(4, 3,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 0,0))) item->Flags&=~ITEMENABLED;       // Utilities
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 1,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 3,0))) item->Flags&=~ITEMENABLED;
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(5, 4,0))) item->Flags&=~ITEMENABLED;
	}
	Refr_MRUMenu();
	Unlock_GlobalMenu();
	OUTRO;
}

void Refr_GlobalMRUMenu(void) {
	INTRO;
	Lock_GlobalMenu();
	Refr_MRUMenu();
	Unlock_GlobalMenu();
	OUTRO;
}

void Refr_MRUMenu(void) {
	register UBYTE i;
	struct MenuItem *item;

	for(i=1;i<=5;i++) {
		if(item=ItemAddress(RunTime.Menu,FULLMENUNUM(0,17+i,0))) {	// MRU[i]
			if(strlen(MRU_GetFileName(i))) { 				// test, if filename is not empty
				item->Flags|=ITEMENABLED;
			}
			else {
				item->Flags&=~ITEMENABLED;
			}
		}
	}
}

//-- helper

void Lock_GlobalMenu(void) {
	ClearMenuStrip(RunTime.winBackDrop);
	ClearMenuStrip(RunTime.winStatus);
	ClearMenuStrip(RunTime.winToolbar);
	if(RunTime.aktbuf) {
		struct Node *si;
		foreach(si,&BufferList) {                               /* hide the menu in all visible sample-windows */
			if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ClearMenuStrip(((SInfo *)si)->win); }
		}
	}
}

void Unlock_GlobalMenu(void) {
	ResetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
	ResetMenuStrip(RunTime.winStatus,RunTime.Menu);
	ResetMenuStrip(RunTime.winToolbar,RunTime.Menu);
	if(RunTime.aktbuf) {
		struct Node *si;
		foreach(si,&BufferList) {                               /* reactivate the menu in all visible sample-windows */
			if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ResetMenuStrip(((SInfo *)si)->win,RunTime.Menu); }
		}
	}
}

void Uncheck_MenuItem(ULONG itemnr) {
	struct MenuItem     *item;
	struct Node *si;

	if(RunTime.winBackDrop) {
		ClearMenuStrip(RunTime.winBackDrop);
		ClearMenuStrip(RunTime.winStatus);
		ClearMenuStrip(RunTime.winToolbar);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* hide the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ClearMenuStrip(((SInfo *)si)->win); }
			}
		}
	}
	if(item=ItemAddress(RunTime.Menu,itemnr)) item->Flags&=~CHECKED;
	if(RunTime.winBackDrop) {
		ResetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
		ResetMenuStrip(RunTime.winStatus,RunTime.Menu);
		ResetMenuStrip(RunTime.winToolbar,RunTime.Menu);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* reactivate the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ResetMenuStrip(((SInfo *)si)->win,RunTime.Menu); }
			}
		}
	}
}

void Check_MenuItem(ULONG itemnr) {
	struct MenuItem     *item;
	struct Node *si;

	if(RunTime.winBackDrop) {
		ClearMenuStrip(RunTime.winBackDrop);
		ClearMenuStrip(RunTime.winStatus);
		ClearMenuStrip(RunTime.winToolbar);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* hide the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ClearMenuStrip(((SInfo *)si)->win); }
			}
		}
	}
	if(item=ItemAddress(RunTime.Menu,itemnr)) item->Flags|=CHECKED;
	if(RunTime.winBackDrop) {
		ResetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
		ResetMenuStrip(RunTime.winStatus,RunTime.Menu);
		ResetMenuStrip(RunTime.winToolbar,RunTime.Menu);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* reactivate the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ResetMenuStrip(((SInfo *)si)->win,RunTime.Menu); }
			}
		}
	}
}

void Disable_MenuItem(ULONG itemnr) {
	struct MenuItem     *item;
	struct Node *si;

	if(RunTime.winBackDrop) {
		ClearMenuStrip(RunTime.winBackDrop);
		ClearMenuStrip(RunTime.winStatus);
		ClearMenuStrip(RunTime.winToolbar);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* hide the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ClearMenuStrip(((SInfo *)si)->win); }
			}
		}
	}
	if(item=ItemAddress(RunTime.Menu,itemnr)) item->Flags&=~ITEMENABLED;
	if(RunTime.winBackDrop) {
		ResetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
		ResetMenuStrip(RunTime.winStatus,RunTime.Menu);
		ResetMenuStrip(RunTime.winToolbar,RunTime.Menu);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* reactivate the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ResetMenuStrip(((SInfo *)si)->win,RunTime.Menu); }
			}
		}
	}
}

void Enable_MenuItem(ULONG itemnr) {
	struct MenuItem     *item;
	struct Node *si;

	if(RunTime.winBackDrop) {
		ClearMenuStrip(RunTime.winBackDrop);
		ClearMenuStrip(RunTime.winStatus);
		ClearMenuStrip(RunTime.winToolbar);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* hide the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ClearMenuStrip(((SInfo *)si)->win); }
			}
		}
	}
	if(item=ItemAddress(RunTime.Menu,itemnr)) item->Flags|=ITEMENABLED;
	if(RunTime.winBackDrop) {
		ResetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
		ResetMenuStrip(RunTime.winStatus,RunTime.Menu);
		ResetMenuStrip(RunTime.winToolbar,RunTime.Menu);
		if(RunTime.aktbuf) {
			foreach(si,&BufferList) {                               /* reactivate the menu in all visible sample-windows */
				if(SFX_SOUND_IS_SHOWN((SInfo *)si)) { ResetMenuStrip(((SInfo *)si)->win,RunTime.Menu); }
			}
		}
	}
}

//-- Help Menu handling

void ShowHelpMail(void) {
	//ShowHelp("mailto:soundfx@sonicpulse.de?subject="PRJ_NAME"-"NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)"."NUM2STR(PRJ_BUILD)"-"__AMIGADATE__"-"VERS_BITS"-"_CPUNAME"/"_FPUNAME);

	if(IdentifyBase) {
		ULONG id_len;
		STRPTR id_string;
		STRPTR fmt_string = "mailto:soundfx@sonicpulse.de"
							"?subject="PRJ_VERS
							"&body=%0a%0a%0a"
							"[ConfigIO]%0a"
							"%09appname=SoundFX-SysInfo%0a"
							"%09version=$$VER: sysinfo.cfg "NUM2STR(PRJ_VERSION)"."NUM2STR(PRJ_REVISION)" © by Stefan Kost 1993-2002%0a"
							"%0a"
							"[Hardware]%0a"
							"%09ComputerSystem=$SYSTEM$%0a"
							"%09CPU=$CPU$/$CPUCLOCK$%0a"
							"%09FPU=$FPU$/$FPUCLOCK$%0a"
							"%09MMU=$MMU$%0a"
							"%09ChipSet=$CHIPSET$%0a"
							"%09Memory.Chip=$CHIPRAM$%0a"
							"%09Memory.Fast=$FASTRAM$%0a"
							"%0a"
							"[Software]%0a"
							"%09AmigaOS=$OSNR$ ($OSVER$)%0a"
							"%09Workbench=$WBVER$%0a"
							"%09Exec=$EXECVER$%0a"
							"%09SetPatch=$SETPATCHVER$%0a"
							"%09GraphicSystem=$GFXSYS$%0a"
							"%09AudioSystem=$AUDIOSYS$%0a";
		//-- Get the size of the buffer
		id_len = IdEstimateFormatSize(fmt_string, NULL);
		MSG1("max.strlen = %ld",id_len);
		//--  Allocate the buffer
		if(id_string = AllocVec(id_len,MEMF_ANY)) {
			//--  Get the formatted string with system information
			IdFormatString(fmt_string, id_string, id_len, NULL);

			ShowHelp(id_string);
			FreeVec(id_string);
		}
		else ShowHelp("mailto:soundfx@sonicpulse.de?subject="PRJ_VERS);
	}
	else ShowHelp("mailto:soundfx@sonicpulse.de?subject="PRJ_VERS);
}

void ShowHelpAbout(void) {
	UBYTE fallback=FALSE;

	if(IdentifyBase) {
		ULONG id_len;
		STRPTR id_string;
		STRPTR fmt_string = PRJ_VERS"\n"
							"\n"
							"ARexx-Port : "SFX_AREXXNAME"\n"
							"Public-Screen : "SFX_PUBSCRNAME"\n"
							"\n"
							"Computer System: $SYSTEM$\n"
							"CPU: $CPU$/$CPUCLOCK$, FPU: $FPU$/$FPUCLOCK$\n"
							"MMU: $MMU$, $CHIPSET$ chipset\n"
							"Memory: $CHIPRAM$ Chip, $FASTRAM$ Fast RAM\n"
							"\n"
							"AmigaOS $OSNR$ ($OSVER$), Workbench $WBVER$\n"
							"Exec $EXECVER$, SetPatch $SETPATCHVER$\n"
							"Graphic System: $GFXSYS$\n"
							"Audio System: $AUDIOSYS$\n";

		//-- Get the size of the buffer
		id_len = IdEstimateFormatSize(fmt_string, NULL);
		//--  Allocate the buffer
		if(id_string = AllocVec(id_len,MEMF_ANY)) {
			//--  Get the formatted string with system information
			IdFormatString(fmt_string, id_string, id_len, NULL);
			Message(strNoID,id_string,NULL,0);
			FreeVec(id_string);
		}
		else fallback=TRUE;
	}
	else fallback=TRUE;

	//-- use default implementation, if everything else failed
	if(fallback) {
		STRPTR fmt_string = PRJ_VERS"\n"
							"\n"
							"ARexx-Port : "SFX_AREXXNAME"\n"
							"Public-Screen : "SFX_PUBSCRNAME"\n";

		Message(strNoID,fmt_string,NULL,0);
	}
}

//-- Quit handling

void TryQuit(void) {
	switch(sfxprefs_misc.askexit) {
		case ASKEXIT_NEVER: RunTime.tryquit=TRUE;break;
		case ASKEXIT_LOADED:
			if(RunTime.aktsamples) {
				if(Question(reqReallyQuit,GetString(strYes),GetString(strNo))) RunTime.tryquit=TRUE;
			}
			else RunTime.tryquit=TRUE;
			break;
		case ASKEXIT_UNSAVED:
			if(RunTime.aktsamples) {
				UBYTE saved=TRUE;
				struct Node *snd;

				foreach(snd,&BufferList) {
					saved&=((SInfo *)snd)->saved;
				}
				if(!saved) {
					if(Question(reqReallyQuit,GetString(strYes),GetString(strNo))) RunTime.tryquit=TRUE;
				}
				else RunTime.tryquit=TRUE;
			}
			else RunTime.tryquit=TRUE;
			break;
		case ASKEXIT_ALWAYS: if(Question(reqReallyQuit,GetString(strYes),GetString(strNo))) RunTime.tryquit=TRUE;break;
	}
}

//-- eof ----------------------------------------------------------------------

