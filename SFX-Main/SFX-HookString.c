/****h* SoundFX/SFX-HookString.c [4.2] *
*
*  NAME
*    SFX-HookString.c
*  COPYRIGHT
*    $VER: SFX-HookString.c 4.2 (10.09.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    string-gadget edithook replacements - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    10.Sep.2002	V 4.2	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_HookString_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

ULONG __asm __saveds STFilterHook(register __a1 unsigned long *msg,register __a2 struct SGWork *sgw);

//-- globals ------------------------------------------------------------------

char				strbuffer[PARBUFLEN+1];
struct Hook			strhook={	{0,0},STFilterHook,0,0 };
struct StringExtend	stringextend={	NULL,1,5,1,5,0L,&strhook,strbuffer,{0L,0L,0L,0L} };
struct StringExtend	stringextend2={	NULL,1,5,1,5,0L,&strhook,strbuffer,{0L,0L,0L,0L} };

//-- definitions --------------------------------------------------------------

ULONG __asm __saveds STFilterHook(register __a1 unsigned long *msg,register __a2 struct SGWork *sgw) {
	if(*msg==SGH_KEY) {
		//MSG3("EditOp=0x%lx  Code=%ld / %ld",sgw->EditOp,sgw->Code,sgw->IEvent->ie_Code);
		//MSG4("<%s> <%s> <%s> <%s>",sgw->WorkBuffer,sgw->PrevBuffer,sgw->StringInfo->Buffer,sgw->StringInfo->UndoBuffer);
		sgw->Actions|=SGA_USE;
		switch(sgw->EditOp) {
			case EO_NOOP:
				//MSG("  EO_NOOP");
				switch(sgw->IEvent->ie_Code) {
					case RKEY_ESC:
						//MSG("    ESC");
						strcpy(sgw->WorkBuffer,sgw->StringInfo->UndoBuffer);
		        	    sgw->Actions|=SGA_END;
						sgw->Actions|=SGA_REDISPLAY;
						break;
					case RKEY_HELP:
						//MSG("    HELP");
   	    		    	sgw->Actions&=~SGA_USE;
   		        		sgw->Actions|=SGA_END;
		   	        	sgw->Actions|=SGA_REUSE;
						break;
				}
				break;
			case EO_REPLACECHAR:
			case EO_INSERTCHAR:
				//MSG("  EO_INSERT/REPLACE");
				if(!(sgw->IEvent->ie_Qualifier&(IEQUALIFIER_LCOMMAND|IEQUALIFIER_RCOMMAND))) {
			        sgw->WorkBuffer[sgw->BufferPos-1]=sgw->Code;
				}
				else {
   	    	    	sgw->Actions&=~SGA_USE;
   	        		sgw->Actions|=SGA_END;
	   	        	sgw->Actions|=SGA_REUSE;
				}
				break;
			case EO_ENTER:		// get triggerd by TAB,SHIFT+TAB,RETURN,ENTER
				//MSG("  EO_ENTER");
				sgw->Actions|=SGA_NEXTACTIVE;
	            sgw->Actions&=~SGA_USE;
				sgw->Actions|=SGA_END;
				break;
		}
		return(TRUE);
	}
	return(FALSE);
}

//-- eof ----------------------------------------------------------------------
