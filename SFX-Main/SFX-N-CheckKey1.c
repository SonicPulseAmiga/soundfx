/****h*SoundFX/SFX-CheckKey1.c_ [4.2] *
*
*  NAME
*    SFX-CheckKey1.c_
*  COPYRIGHT
*    $VER: SFX-CheckKey1.c_ 4.2 (24.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    keyfile checking - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    24.Apr.2002	V 4.2	most recent version
*    23.Feb.2001    V 4.1   most recent version
*    14.Aug.1998    V 3.7   most recent version
*    14.Aug.1998    V 3.7   initial version
*  NOTES
*
*******
*/

#define SFX_CheckKey1_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckKeyFile1(void);

//-- definitions --------------------------------------------------------------

UBYTE CheckKeyFile1(void)
{
	UBYTE ret=STAT_DEMO;
#ifdef CHECK_KEY
	struct Process *me;
	struct Window *msgwin;

	INTRO;

	me=(struct Process *)FindTask(NULL);
	msgwin=me->pr_WindowPtr;me->pr_WindowPtr=(APTR)-1;
	SFXKeyBase1=NULL;                                           // init copy of KeyBase
																// Check Key itself
	if(!SFXKeyBase) SFXKeyBase=OpenLibrary(DecryptString("A@SV?n`|cli`v*vc}+n`|"),0);		// "DEVS:keyfiles/sfx.key"
	if(!SFXKeyBase) SFXKeyBase=OpenLibrary(DecryptString("uwjbalw?n`|cli`v*vc}+n`|"),0);	// "progdir:keyfiles/sfx.key"
	if(!SFXKeyBase) SFXKeyBase=OpenLibrary(DecryptString("uwjbalw?vc}+n`|"),0);				// "progdir:sfx.key"
	if(!SFXKeyBase) {
		struct DosList *dl;
		dl=LockDosList(LDF_ALL|LDF_READ);
		if(FindDosEntry(dl,DecryptString("N@\\V"),LDF_ALL)) {								// "KEYS:"
			SFXKeyBase=OpenLibrary(DecryptString("N@\\V?vc}+n`|"),0);						// "KEYS:sfx.key"
		}
		UnLockDosList(LDF_ALL|LDF_READ);
	}
	if(!SFXKeyBase) {
		char path[FILENAME_MAX+10];
		if(GetVar(DecryptString("N@\\UDQM"),path,FILENAME_MAX,GVF_GLOBAL_ONLY)!=-1) {		// "KEYPATH"
			AddPart(path,DecryptString("vc}+n`|"),FILENAME_MAX+10);							// "sfx.key"
			SFXKeyBase=OpenLibrary(path,0l);
		}
	}
	if(!SFXKeyBase) ret=CheckOldKeyFile1();
	else {
		SFXKeyBase1=SFXKeyBase;         // copy content of KeyBase
		RunTime.keypd=Key_GetPlainData();
		RunTime.keycd=Key_GetCodedData();
		Key_GetDiffs(&RunTime.keyrowd,&RunTime.keycold);
		if(Key_GetKeyType(&ret)) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyType failed"); }
		if(Key_GetKeyNr  (&RunTime.keynr)) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyNr failed"); }
		if(Key_GetMinMax (&RunTime.keymi,&RunTime.keyma,&RunTime.keymabits)) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyMinMax failed"); }
		if(Key_GetRv     (&RunTime.keyrv)) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("GetKeyRv failed"); }

//      MSG1("KeyNr : %ld",RunTime.keynr);
//      MSG3("KeyMi : %d  KeyMa : %d  KeyMaBits : %d",RunTime.keymi,RunTime.keyma,RunTime.keymabits);
//      MSG1("KeyRv : %d",RunTime.keyrv);
//      MSG1("KeyPd : %08lx",RunTime.keypd);
//      MSG1("KeyRowd : %ld",RunTime.keyrowd);MSG3("%d %d %d",RunTime.keyrowd[0],RunTime.keyrowd[10],RunTime.keyrowd[20]);
//      MSG1("KeyCold : %ld",RunTime.keycold);

		strncpy(Key.line0,&RunTime.keypd[  0],39);Key.line0[39]='\0';TrimString(Key.line0);
		strncpy(Key.line1,&RunTime.keypd[ 40],39);Key.line1[39]='\0';TrimString(Key.line1);
		strncpy(Key.line2,&RunTime.keypd[ 80],39);Key.line2[39]='\0';TrimString(Key.line2);
		strncpy(Key.line3,&RunTime.keypd[120],39);Key.line3[39]='\0';TrimString(Key.line3);
		strncpy(Key.line4,&RunTime.keypd[160],39);Key.line4[39]='\0';TrimString(Key.line4);
		strncpy(Key.line5,&RunTime.keypd[200],39);Key.line5[39]='\0';TrimString(Key.line5);
		strncpy(Key.line6,&RunTime.keypd[240],39);Key.line6[39]='\0';TrimString(Key.line6);
		strncpy(Key.line7,&RunTime.keypd[280],39);Key.line7[39]='\0';TrimString(Key.line7);
		strncpy(Key.line8,&RunTime.keypd[320],39);Key.line8[39]='\0';TrimString(Key.line8);
		strncpy(Key.line9,&RunTime.keypd[360],39);Key.line9[39]='\0';TrimString(Key.line9);
		sprintf(RunTime.username,"%s %s",Key.line2,Key.line1);

//      MSG1("Line0 : [%s]",Key.line0);
//      MSG1("Line1 : [%s]",Key.line1);
//      MSG1("Line2 : [%s]",Key.line2);
	}
	me->pr_WindowPtr=msgwin;

	ERRC((ret==STAT_ILLEGAL),"CheckKeyFile1() => Illegal");
	ERRC((ret==STAT_DEMO),"CheckKeyFile1() => Demo");
											// Compare with Database
	ret|=CheckKeyList();
	ret|=CheckKeyContents();
#endif
#ifdef CHECK_DELFINA
	struct Library *DelfinaBase;
	DELFPTR mem;

	if((DelfinaBase=OpenLibrary("delfina.library",0))) {
		if((mem=Delf_AllocMem(2,DMEMF_XDATA))) {
			Delf_Poke(mem,DMEMF_XDATA,0xAFFE);
			ret=(Delf_Peek(mem,DMEMF_XDATA)==0xAFFE);
			Delf_FreeMem(mem,DMEMF_XDATA);
			if(ret) {
				ret=STAT_USER;
				sprintf(RunTime.username,"DelfinaUser");
			}
		}
		CloseLibrary(DelfinaBase);
	}
#endif
#ifdef CHECK_PRELUDE
	struct Library *ExpansionBase;
	struct Library *PreludeBase;
	
	if((PreludeBase=OpenLibrary("prelude.library",0))) {
		if(ExpansionBase=OpenLibrary("expansion.library",0)) {
			if(FindConfigDev(NULL,0x4231,0x1)) {
				ret=STAT_USER;
				sprintf(RunTime.username,"PreludeUser");
			}
			CloseLibrary(ExpansionBase);
		}
		CloseLibrary(PreludeBase);
	}
#endif
#ifdef CHECK_REPULSE
	struct Library *ExpansionBase;
	struct Library *RepulseBase;
	
	if((RepulseBase=OpenLibrary("repulse.library",0))) {
		if(ExpansionBase=OpenLibrary("expansion.library",0)) {
			if(FindConfigDev(NULL,0x4144,0x0)) {
				ret=STAT_USER;
				sprintf(RunTime.username,"RepulseUser");
			}
			CloseLibrary(ExpansionBase);
		}
		CloseLibrary(RepulseBase);
	}
#endif

	ERRC((ret==STAT_ILLEGAL),"CheckKeyFile1() => Illegal");
	ERRC((ret==STAT_DEMO),"CheckKeyFile1() => Demo");

	if(ret!=STAT_ILLEGAL) ret&=CheckKeyFile2();

	OUTRO;

	return(ret);
}

//-- eof ----------------------------------------------------------------------
