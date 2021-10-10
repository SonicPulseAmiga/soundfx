/****h*SoundFX/SFX-CheckOldKey1.c_ [4.2] *
*
*  NAME
*    SFX-CheckOldKey1.c_
*  COPYRIGHT
*    $VER: SFX-CheckOldKey1.c_ 4.2 (24.04.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    keyfile checking (old) - decfinitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    24.Apr.2002	V 4.2	most recent version
*    20.Feb.2001	V 4.1	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_CheckOldKey1_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckOldKeyFile1(void);

//-- definitions --------------------------------------------------------------

UBYTE CheckOldKeyFile1(void)
{
	UBYTE ret=STAT_DEMO;
	register UBYTE i;
	FILE *file;
	UWORD chkh0,chkh1,chkh2,chkh3,chkh4,chkh5,chkh6,chkh7,chkh8,chkh9;
	UWORD chkv[42];

	INTRO;

	chkh0=chkh1=chkh2=chkh3=chkh4=chkh5=chkh6=chkh7=chkh8=chkh9=0;

	file=fopen(DecryptString("A@SV?n`|cli`v*vc}+n`|"),"rb");							// "DEVS:keyfiles/sfx.key"
	if(!file) file=fopen(DecryptString("uwjbalw?n`|cli`v*vc}+n`|"),"rb");				// "progdir:keyfiles/sfx.key"
	if(!file) file=fopen(DecryptString("uwjbalw?vc}+n`|"),"rb");						// "progdir:sfx.key"
	if(!file) {
		struct DosList *dl;
		dl=LockDosList(LDF_ALL|LDF_READ);
		if(FindDosEntry(dl,DecryptString("N@\\V"),LDF_ALL)) {							// "KEYS:"
			file=fopen(DecryptString("N@\\V?vc}+n`|"),"rb");							// "KEYS:sfx.key"
		}
		UnLockDosList(LDF_ALL|LDF_READ);
	}
	if(!file) {
		char path[FILENAME_MAX+10];
		if(GetVar(DecryptString("N@\\UDQM"),path,FILENAME_MAX,GVF_GLOBAL_ONLY)!=-1) {	// "KEYPATH"
			AddPart(path,DecryptString("vc}+n`|"),FILENAME_MAX+10);						// "sfx.key"
			file=fopen(path,"rb");
		}
	}
	if(file) {
		if(fread(&Key,sizeof(Key),1,file)) {
			fclose(file);
			for(i=0;i<40;i++) {
				chkh0+=Key.line0[i];
				chkh1+=Key.line1[i];
				chkh2+=Key.line2[i];
				chkh3+=Key.line3[i];
				chkh4+=Key.line4[i];
				chkh5+=Key.line5[i];
				chkh6+=Key.line6[i];
				chkh7+=Key.line7[i];
				chkh8+=Key.line8[i];
				chkh9+=Key.line9[i];
				chkv[i]=Key.line0[i]+Key.line1[i]+Key.line2[i]+Key.line3[i]+Key.line4[i]+Key.line5[i]+Key.line6[i]+Key.line7[i]+Key.line8[i]+Key.line9[i];
				chkv[i]=(chkv[i]&0xFFFF+chkv[i]&0xFFF+chkv[i]&0xFF+chkv[i]&0xF)&0xFFFF;
			}
			chkh0=CheckSum1(chkh0);
			chkh1=CheckSum1(chkh1);
			chkh2=CheckSum1(chkh2);
			chkh3=CheckSum1(chkh3);
			chkh4=CheckSum1(chkh4);
			chkh5=CheckSum1(chkh5);
			chkh6=CheckSum1(chkh6);
			chkh7=CheckSum1(chkh7);
			chkh8=CheckSum1(chkh8);
			chkh9=CheckSum1(chkh9);

			chkv[40]=Key.line0[0]+Key.line1[1]+Key.line2[2]+Key.line3[3]+Key.line4[4]+Key.line5[5]+Key.line6[6]+Key.line7[7]+Key.line8[8]+Key.line9[9];
			chkv[40]=CheckSum1(chkv[40]);
			chkv[41]=Key.line0[9]+Key.line1[8]+Key.line2[7]+Key.line3[6]+Key.line4[5]+Key.line5[4]+Key.line6[3]+Key.line7[2]+Key.line8[1]+Key.line9[0];
			chkv[41]=CheckSum1(chkv[41]);

			ret=STAT_ILLEGAL;
			if(!strnicmp(Key.line8,"AUTHOR",6)) ret=STAT_AUTHOR;
			if(!strnicmp(Key.line8,"BETATESTER",10)) ret=STAT_TESTER;
			if(!strnicmp(Key.line8,"USER",4)) ret=STAT_USER;
			ERRC((ret==STAT_ILLEGAL),"Illegal Keytype");

			RunTime.keynr=atoi(Key.line0);
			switch(ret) {
				case STAT_AUTHOR:	// should be only 0
					if( RunTime.keynr!=  0)							 { ret=STAT_ILLEGAL;ERR1("Illegal KeyNr (%d)",RunTime.keynr); }
					break;
				case STAT_TESTER:	// should be between 1...15
					if((RunTime.keynr<   1) || (RunTime.keynr>  15)) { ret=STAT_ILLEGAL;ERR1("Illegal KeyNr (%d)",RunTime.keynr); }
					break;
				case STAT_USER:		// should be between 1000...1250
					if((RunTime.keynr<1000) || (RunTime.keynr>1250)) { ret=STAT_ILLEGAL;ERR1("Illegal KeyNr (%d)",RunTime.keynr); }
					break;
			}
			
			if(chkh0!=Key.chkh0) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh0 failed"); }
			if(chkh1!=Key.chkh1) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh1 failed"); }
			if(chkh2!=Key.chkh2) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh2 failed"); }
			if(chkh3!=Key.chkh3) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh3 failed"); }
			if(chkh4!=Key.chkh4) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh4 failed"); }
			if(chkh5!=Key.chkh5) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh5 failed"); }
			if(chkh6!=Key.chkh6) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh6 failed"); }
			if(chkh7!=Key.chkh7) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh7 failed"); }
			if(chkh8!=Key.chkh8) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh8 failed"); }
			if(chkh9!=Key.chkh9) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkh9 failed"); }
			for(i=0;i<42;i++) {
				if(chkv[i]!=Key.chkv[i]) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("Chkv failed");i=42; }
			}

			if(strncmp(Key.line9,"SoundFX",7)) { ret=STAT_ILLEGAL;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("wrong app"); }

			Key.chkh0=Key.chkh1=Key.chkh2=Key.chkh3=Key.chkh4=Key.chkh5=Key.chkh6=Key.chkh7=Key.chkh8=Key.chkh9=0;	/* als Stringterminator */

			TrimString(Key.line0);
			TrimString(Key.line1);
			TrimString(Key.line2);
			sprintf(RunTime.username,"%s %s",Key.line2,Key.line1);
		}
		else Key.chkh0=Key.chkh1=Key.chkh2=Key.chkh3=Key.chkh4=Key.chkh5=Key.chkh6=Key.chkh7=Key.chkh8=Key.chkh9=0;	/* als Stringterminator */
	}

	ERRC((ret==STAT_ILLEGAL),"CheckOldKeyFile1() => Illegal");
	ERRC((ret==STAT_DEMO),"CheckOldKeyFile1() => Demo");

	OUTRO;

	if(ret!=STAT_ILLEGAL) ret&=CheckOldKeyFile2();

	return(ret);
}

//-- eof ----------------------------------------------------------------------
