/****h* SoundFX/SFX-GUIKeys.c [4.2] *
*
*  NAME
*    SFX-GUIKeys.c
*  COPYRIGHT
*    $VER: SFX-GUIKeys.c 4.2 (03.06.02) by Stefan Kost 1998-2002
*  FUNCTION
*    gui keyboard routines - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    21.Jan.2002
*  MODIFICATION HISTORY
*    03.Jun.2002    V 4.2   most recent version
*    21.Jan.2002    V 4.2	initial version
*  NOTES
*
*******
*/

#define SFX_GUIKeys_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- global shortcut tests

void Test_VKeys(UWORD icode,SInfo *entry);
void Test_RKeys(UWORD icode,UWORD iqual,SInfo *entry);

//-- globals ------------------------------------------------------------------

void Test_VKeys(UWORD icode,SInfo *entry) {
	//INTRO;

	switch(icode) {
		case 'r':           /* Run Operator */
			SwitchIDCMPOFF();
			ProcessSample();
			SwitchIDCMPON();
			break;
		case 'R':			/* Run Rexx */
			SwitchIDCMPOFF();
			StartRexx();
			SwitchIDCMPON();
			break;
		case 'L':
		case 'l':           /* Load */
			SwitchIDCMPOFF();
			RunTime.swallowactivate=TRUE;
			LoadSample();
			SwitchIDCMPON();
			break;
		case 'S':
		case 's':           /* Save */
			if(RunTime.aktsamples) {
				SwitchIDCMPOFF();
				SaveSampleInterface();
				SwitchIDCMPON();
			}
			break;
		case ' ':           /* Stop */
			if(RunTime.play) StopSample();
			else if(RunTime.aktsamples) PlaySample_All();
			break;
		case 'P':           /* Play Range/Window */
			if(RunTime.aktsamples) PlaySample_Rng();
			break;
		case 'p':           /* Play All*/
			if(RunTime.aktsamples) PlaySample_All();
			break;
		case '+':       /* next */
			if(RunTime.aktsamples) {
				if(!IsLastOfList(&BufferList,((struct Node *)RunTime.aktbuf))) {
					RunTime.aktbuf=(SInfo *)GetNextNode((struct Node *)RunTime.aktbuf);
					if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
						WindowToFront(RunTime.aktbuf->win);
						ActivateWindow(RunTime.aktbuf->win);
					}
				}
			}
			break;
		case '-':       /* prev */
			if(RunTime.aktsamples) {
				if(!IsFirstOfList(&BufferList,((struct Node *)RunTime.aktbuf))) {
					RunTime.aktbuf=(SInfo *)GetPrevNode((struct Node *)RunTime.aktbuf);
					if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
						WindowToFront(RunTime.aktbuf->win);
						ActivateWindow(RunTime.aktbuf->win);
					}
				}
			}
			break;
		case '<':       /* ZoomIn */
			if(RunTime.aktsamples && entry) ZoomIn(entry,1);
			break;
		case '>':       /* ZoomOut */
			if(RunTime.aktsamples && entry) ZoomOut(entry,1);
			break;
		case 0x09:      /* Tab */
			RunTime.rngmode=(RunTime.rngmode+1)&3;
			Refr_ToolbarRange();
			break;
		case 0x7F:          /* Flush All Samples */
			if(RunTime.aktsamples) {
				if(Question(reqRemoveAll,GetString(strYes),GetString(strNo))) FlushAllSamples();
			}
			RunTime.samplestat=SMPSTAT_LASTOLD;
			break;
	}
	//OUTRO;
}

void Test_RKeys(UWORD icode,UWORD iqual,SInfo *entry) {
	//INTRO;
	if(icode<128) {          // just keydown
		switch(icode) {
			case RKEY_DEL:		/* Flush All Samples */
				if(RunTime.aktsamples) {
					if(Question(reqRemoveAll,GetString(strYes),GetString(strNo))) FlushAllSamples();
				}
				RunTime.samplestat=SMPSTAT_LASTOLD;
				break;
			case 0x13:          /* 'R' Run Operator/Rexx */
				if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) {
					SwitchIDCMPOFF();
					StartRexx();
					SwitchIDCMPON();
				}
				else {
					SwitchIDCMPOFF();
					ProcessSample();
					SwitchIDCMPON();
				}
				break;
			case 0x28:          /* 'L' Load */
				SwitchIDCMPOFF();
				RunTime.swallowactivate=TRUE;
				LoadSample();
				SwitchIDCMPON();
				break;
			case 0x21:          /* 'S' Save */
				if(RunTime.aktsamples) {
					SwitchIDCMPOFF();
					SaveSampleInterface();
					SwitchIDCMPON();
				}
				break;
			case 0x19:          /* 'P' Play */
				if(RunTime.aktsamples) {
					if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) PlaySample_Rng();
					else PlaySample_All();
				}
				break;
			case RKEY_SPACE:          /* Play/Stop */
				if(RunTime.play) StopSample();
				else if(RunTime.aktsamples) {
					if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) PlaySample_Rng();
					else PlaySample_All();
				}
				break;
			case 0x1B:          /* next */
			case 0x5E:
				if(RunTime.aktsamples) {
					if(!IsLastOfList(&BufferList,((struct Node *)RunTime.aktbuf))) {
						RunTime.aktbuf=(SInfo *)GetNextNode((struct Node *)RunTime.aktbuf);
						if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
							WindowToFront(RunTime.aktbuf->win);
							ActivateWindow(RunTime.aktbuf->win);
						}
					}
				}
				break;
			case 0x3A:          /* prev */
			case 0x4A:
				if(RunTime.aktsamples) {
					if(!IsFirstOfList(&BufferList,((struct Node *)RunTime.aktbuf))) {
						RunTime.aktbuf=(SInfo *)GetPrevNode((struct Node *)RunTime.aktbuf);
						if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
							WindowToFront(RunTime.aktbuf->win);
							ActivateWindow(RunTime.aktbuf->win);
						}
					}
				}
				break;
			case 0x38:              /* ZoomIn */
				if(RunTime.aktsamples && entry) {
					if(iqual&IEQUALIFIER_CONTROL) ZoomIn(entry,2);              // zoom vertically
					else ZoomIn(entry,1);                                       // zoom horizontally
				}
				break;
			case 0x39:              /* ZoomOut */
				if(RunTime.aktsamples && entry) {
					if(iqual&IEQUALIFIER_CONTROL) ZoomOut(entry,2);             // zoom vertically
					else ZoomOut(entry,1);                                      // zoom horizontally
				}
				break;
			case 0x30:              /* ZoomIn & Out */
				if(RunTime.aktsamples && entry) {
					if(iqual&IEQUALIFIER_CONTROL) {                 // zoom vertically
						if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) ZoomOut(entry,2);
						else ZoomIn(entry,2);
					}
					else {                                          // zoom horizontally
						if(iqual&(IEQUALIFIER_LSHIFT|IEQUALIFIER_RSHIFT)) ZoomOut(entry,1);
						else ZoomIn(entry,1);
					}
				}
				break;
			case RKEY_TAB:  /* Tab */
				RunTime.rngmode=(RunTime.rngmode+1)&3;
				Refr_ToolbarRange();
				break;
		}
	}
	//OUTRO;
}

//-- eof ----------------------------------------------------------------------

