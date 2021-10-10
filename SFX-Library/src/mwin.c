/****h* sfxsupport.library/mwin.c [1.20] *
*
*  NAME
*    mwin.c
*  COPYRIGHT
*    $VER: mwin.c 1.20 (02.03.99) © by Stefan Kost 1998-1999
*  FUNCTION
*    multiport-windows - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Jul.1998
*  MODIFICATION HISTORY
*    02.Mar.1999	V 1.20	most recent version
*    18.Jul.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#define LOAT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"

//-- prototypes ---------------------------------------------------------------

//-- MultiPortWindows

struct Window *	ASM LIB_EXPORT(OpenMWindow(REG(a0,struct TagItem *wintags),REG(a1,struct MsgPort *userport),REG(a2,UBYTE *ct),REG(d0,ULONG idcmps)));
void			ASM LIB_EXPORT(CloseMWindow(REG(a0,struct Window *win),REG(a1,UBYTE *ct)));

//-- definitions --------------------------------------------------------------

//-- MultiPortWindows

struct Window * SAVEDS ASM LIB_EXPORT(OpenMWindow(REG(a0,struct TagItem *wintags),REG(a1,struct MsgPort *userport),REG(a2,UBYTE *ct),REG(d0,ULONG idcmps))) {
	struct Window *win;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(wintags,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(userport,__FUNC__,"2.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ct,__FUNC__,"3.par",4));

	Forbid();
	if(win=OpenWindowTagList(0l,wintags)) {
		win->UserPort=userport;
		ModifyIDCMP(win,idcmps);(*ct)++;
		GetRunTime(sig_in)|=(1L<<userport->mp_SigBit);				// it doesn't matter if we set is twice (or even 1000 times)
		Permit();
		SetAPen(win->RPort,1);SetBPen(win->RPort,5);
		SetMenuStrip(win,GetRunTime(Menu));
	}
	else { Permit();MSG("OpenWindowTagList failed"); }
	return(win);
}

void SAVEDS ASM LIB_EXPORT(CloseMWindow(REG(a0,struct Window *win),REG(a1,UBYTE *ct))) {
	struct IntuiMessage *imsg,*imsg2;
	struct MsgPort *up;

	ASSERT_RESET;
	ASSERTQ(IS_VALID_POINTER_N0(win,__FUNC__,"1.par",4));
	ASSERTQ(IS_VALID_POINTER_N0(ct,__FUNC__,"2.par",4));

	/** @ todo: is this saveguard neccesary */
	if(win) {
		Forbid();
		up=win->UserPort;
		//ModifyIDCMP(win,0l);			/* vielleicht besser */
		win->IDCMPFlags=0l;
		win->UserPort=0l;
		imsg=(struct IntuiMessage *)(up->mp_MsgList.lh_Head);
		while(imsg && !EndOfList(&(up->mp_MsgList),(struct Node *)imsg)) {		// alle Messages aus dem Port entfernen
			imsg2=(struct IntuiMessage *)(((struct Node *)imsg)->ln_Succ);			// get next message in list, BEFORE current entry is invalid
			if(imsg->IDCMPWindow==win) { Remove(imsg);ReplyMsg(imsg); }
			imsg=imsg2;
		}
		(*ct)--;
		if(!(*ct)) { GetRunTime(sig_in)&=~(1L<<up->mp_SigBit); }		// if it was the last window using this port
		Permit();
		ClearMenuStrip(win);
	 	CloseWindow(win);
	}
	else MSG("Window doesn't exists !");
}

//-- eof ----------------------------------------------------------------------
