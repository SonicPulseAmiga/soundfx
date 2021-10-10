/****h* SoundFX/SFX-IDCMPCtrl.c [4.2] *
*
*  NAME
*    SFX-IDCMPCtrl.c
*  COPYRIGHT
*    $VER: SFX-IDCMPCtrl.c 4.2 (03.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    window enabling and disabling for SFX - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    10.Aug.1998
*  MODIFICATION HISTORY
*    03.Jun.2002	V 4.2	most recent version
*    20.Feb.2000	V 4.00	most recent version
*    25.Jan.2000	V 3.82	most recent version
*    24.Aug.1998	V 3.70	most recent version
*    10.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_IDCMPCtrl_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

//-- IDCMPCtrl

void SwitchIDCMPOFF(void);
void SwitchIDCMPON(void);

//-- definitions --------------------------------------------------------------

void SwitchIDCMPOFF(void)
{
	struct Node *si;

	if(RunTime.winBackDrop) {
		Request(&NullReq,RunTime.winBackDrop);
		SetWindowPointer(RunTime.winBackDrop,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
	}
	if(RunTime.winStatus) {
		Request(&NullReq,RunTime.winStatus);
		SetWindowPointer(RunTime.winStatus,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
	}
	if(RunTime.winToolbar) {
		Request(&NullReq,RunTime.winToolbar);
		SetWindowPointer(RunTime.winToolbar,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
	}
	if(RunTime.winDropDown) {
		Request(&NullReq,RunTime.winDropDown);
		SetWindowPointer(RunTime.winDropDown,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
	}
	foreach(si,&BufferList) {                               /* block all shown sample-windows */
		if(SFX_SOUND_IS_SHOWN((SInfo *)si)) {
			Request(&NullReq,((SInfo *)si)->win);
			SetWindowPointer(((SInfo *)si)->win,WA_BusyPointer,TRUE,WA_PointerDelay,TRUE,TAG_DONE);
			//((SInfo *)si)->win->Flags&=~WFLG_SIZEGADGET;
		}
	}
}

void SwitchIDCMPON(void)
{
	struct Node *si;

	if(RunTime.winBackDrop) {
		EndRequest(&NullReq,RunTime.winBackDrop);
		SetWindowPointer(RunTime.winBackDrop,TAG_DONE);
	}
	if(RunTime.winStatus) {
		EndRequest(&NullReq,RunTime.winStatus);
		SetWindowPointer(RunTime.winStatus,TAG_DONE);
	}
	if(RunTime.winToolbar) {
		EndRequest(&NullReq,RunTime.winToolbar);
		SetWindowPointer(RunTime.winToolbar,TAG_DONE);
	}
	if(RunTime.winDropDown) {
		EndRequest(&NullReq,RunTime.winDropDown);
		SetWindowPointer(RunTime.winDropDown,TAG_DONE);
	}
	foreach(si,&BufferList) {                               /* unblock all shown sample-windows */
		if(SFX_SOUND_IS_SHOWN((SInfo *)si)) {
			EndRequest(&NullReq,((SInfo *)si)->win);
			SetWindowPointer(((SInfo *)si)->win,TAG_DONE);
			//((SInfo *)si)->win->Flags|=WFLG_SIZEGADGET;
		
		}
	}
}

//-- eof ----------------------------------------------------------------------
