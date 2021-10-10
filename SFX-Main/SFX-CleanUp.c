/****h* SoundFX/SFX-CleanUp.c [4.2] *
*
*  NAME
*    SFX-CleanUp.c
*  COPYRIGHT
*    $VER: SFX-CleanUp.c 4.2 (03.06.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    CleanUp functions - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    22.Feb.1997
*  MODIFICATION HISTORY
*    03.Jun.2002	V 4.2	most recent version
*    27.Aug.2000	V 4.1	most recent version
*    30.Jan.2000	V 3.82	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    22.Feb.1997	V 3.35	initial version
*  NOTES
*
*******
*/

#define SFX_CleanUp_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

void CleanUpCurr(void);
void CleanUpAll(void);
void CleanUpAllNorm(void);
void CleanUpAllZoom(void);

//-- definitions --------------------------------------------------------------

void CleanUpCurr(void)
{
	if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) {
		HideSample();
		SetOptWinBounds(&RunTime.aktbuf->xpos,&RunTime.aktbuf->ypos,RunTime.aktbuf->xres,RunTime.aktbuf->yres);
		ShowSample();
	}
}

void CleanUpAll(void)
{
	SInfo *oldaktbuf=RunTime.aktbuf;
	struct Node *si;

	TemporaryHideAllSounds();
	foreach(si,&BufferList) {								/* reshow all temporary hidden samples */
		if(SFX_SOUND_IS_TEMPHIDDEN((SInfo *)si)) {
			RunTime.aktbuf=(SInfo *)si;
			SetOptWinBounds(&RunTime.aktbuf->xpos,&RunTime.aktbuf->ypos,RunTime.aktbuf->xres,RunTime.aktbuf->yres);
			ShowSample();
		}
	}
	RunTime.aktbuf=oldaktbuf;
	Refr_ToolbarBuffer();
	if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
}

void CleanUpAllNorm(void)
{
	SInfo *oldaktbuf=RunTime.aktbuf;
	struct Node *si;

	TemporaryHideAllSounds();
	foreach(si,&BufferList) {								 /* reshow all temporary hidden samples */
		if(SFX_SOUND_IS_TEMPHIDDEN((SInfo *)si)) {
			RunTime.aktbuf=(SInfo *)si;
			RunTime.aktbuf->xres=sfxprefs_sbuf.xs1;
			RunTime.aktbuf->yres=sfxprefs_sbuf.ys1;
			SetOptWinBounds(&RunTime.aktbuf->xpos,&RunTime.aktbuf->ypos,RunTime.aktbuf->xres,RunTime.aktbuf->yres);
			ShowSample();
		}
	}
	RunTime.aktbuf=oldaktbuf;
	Refr_ToolbarBuffer();
	if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
}

void CleanUpAllZoom(void)
{
	SInfo *oldaktbuf=RunTime.aktbuf;
	struct Node *si;

	TemporaryHideAllSounds();
	foreach(si,&BufferList) {								 /* reshow all temporary hidden samples */
		if(SFX_SOUND_IS_TEMPHIDDEN((SInfo *)si)) {
			RunTime.aktbuf=(SInfo *)si;
			RunTime.aktbuf->xres=sfxprefs_sbuf.xs2;
			RunTime.aktbuf->yres=sfxprefs_sbuf.ys2;
			SetOptWinBounds(&RunTime.aktbuf->xpos,&RunTime.aktbuf->ypos,RunTime.aktbuf->xres,RunTime.aktbuf->yres);
			ShowSample();
		}
	}
	RunTime.aktbuf=oldaktbuf;
	Refr_ToolbarBuffer();
	if(SFX_SOUND_IS_SHOWN(RunTime.aktbuf)) ActivateWindow(RunTime.aktbuf->win);
}

//-- eof ----------------------------------------------------------------------

