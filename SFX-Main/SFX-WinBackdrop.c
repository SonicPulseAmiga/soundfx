/****h* SoundFX/SFX-WinBackdrop.c [4.2] *
*
*  NAME
*    SFX-WinBackdrop.c
*  COPYRIGHT
*    $VER: SFX-WinBackdrop.c 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    Backdropwindow for SFX Pubscreen - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    11.Aug.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    26.Jan.2000	V 3.82	most recent version
*    11.Aug.1998	V 3.70	most recent version
*    11.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_WinBackdrop_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"
#include "SFX-ImgBacklogo.h"

//-- prototypes ---------------------------------------------------------------

UBYTE Open_BDWin(void);
void Test_BDWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr);

//-- definitions --------------------------------------------------------------

UBYTE Open_BDWin(void)
{
	UBYTE ret=1;

	BackDrop_Wintags_Left		=RunTime.clientrect.LeftEdge;
	BackDrop_Wintags_Top		=RunTime.clientrect.TopEdge;
	BackDrop_Wintags_Width		=RunTime.clientrect.Width;
	BackDrop_Wintags_Height		=RunTime.clientrect.Height;
	BackDrop_Wintags_ScreenTitle=(ULONG)sfxtitle;
	if(RunTime.winBackDrop=OpenWindowTagList(NULL,BackDrop_Wintags)) {
		struct RastPort *rp=RunTime.winBackDrop->RPort;

		SetRast(rp,6);
		DrawGadWB(rp,0,0,RunTime.clientrect.Width-1,RunTime.clientrect.Height-1);
		RunTime.winBackDrop->UserData=WT_BackDrop;
		if(sfxprefs_misc.backlogo) {
			SetABPenDrMd(rp,1,5,JAM1);
			DrawImage(rp,&backlogo,(RunTime.xres-277)>>1,(RunTime.yres-103)>>1);
		}
		SetMenuStrip(RunTime.winBackDrop,RunTime.Menu);
		ret=0;
	}
	return(ret);
}

void Test_BDWin(ULONG iclass,UWORD icode,WORD imx,WORD imy,UWORD iqual,APTR iadr)
{
	switch(iclass) {
		case IDCMP_RAWKEY:
			switch(icode) {
				case RKEY_HELP: ShowHelp("html/nodes/node__.__.__.html");break;
				default:        Test_RKeys(icode,iqual,RunTime.aktbuf); break;
			}
			break;
		case IDCMP_VANILLAKEY:  Test_VKeys(icode,RunTime.aktbuf);break;
		case IDCMP_MENUPICK:    Test_Menu(icode,iqual);break;
	}
}

//-- eof ----------------------------------------------------------------------
