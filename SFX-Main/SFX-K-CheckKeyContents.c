/****h*SoundFX/SFX-CheckKeyContents.c_ [4.1] *
*
*  NAME
*    SFX-CheckKeyContents.c_
*  COPYRIGHT
*    $VER: SFX-CheckKeyContents.c_ 4.1 (23.02.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    key contents checking - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Aug.1998
*  MODIFICATION HISTORY
*    23.Feb.2001	V 4.1	most recent version
*    14.Aug.1998	V 3.70	most recent version
*    14.Aug.1998	V 3.70	initial version
*  NOTES
*    Zeile	Länge	Type			Desc
*    0		5		            num	Keynr.
*    1		#?		alpha punct		Nachname
*    2		#?		alpha punct		Vorname
*    3		#?		alpha punct	num Addresse
*    4		#?		alpha punct		Stadt
*    5		#?		alpha num		PLZ
*    6		#? 		alpha			Land
*    7		#?		date			Datum
*    8		<10		alpha			Type {Author/Betatester/User}
*    9		<7		alpha			Progname {SoundFX}
*
*******
*/

#define SFX_CheckKeyContents_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckKeyContents(void);

//--helper

UBYTE KeyLineLen(char *str);
UBYTE KeyLineType(char *str,UBYTE l);

//-- defines ------------------------------------------------------------------

#define KPD_NUM		 1
#define KPD_ALPHA	 2
#define KPD_ALPHANUM 3
#define KPD_PUNCT	 4
#define KPD_NUMPUNCT 5
#define KPD_SOND	 8

//-- definitions --------------------------------------------------------------

UBYTE CheckKeyContents(void)
{
	UBYTE ret=0;

#ifdef CHECK_KEY
	UBYTE l,t;

	INTRO;

	if(RunTime.status) {
		MSG("checking key contents");
		l=KeyLineLen(Key.line0);t=KeyLineType(Key.line0,l);if(  t!=KPD_NUM							|| l!=5) { ret=255;ERR2("Line 0 failed, %2d %04x",l,t); }	// Keynr
		l=KeyLineLen(Key.line1);t=KeyLineType(Key.line1,l);if(!(t& KPD_ALPHA)	|| (t& KPD_NUM)		|| l==0) { ret=255;ERR2("Line 1 failed, %2d %04x",l,t); }	// Name (Vorname)
		l=KeyLineLen(Key.line2);t=KeyLineType(Key.line2,l);if(!(t& KPD_ALPHA)	|| (t& KPD_NUM)		|| l==0) { ret=255;ERR2("Line 2 failed, %2d %04x",l,t); }	// Vorname / Firma
		l=KeyLineLen(Key.line3);t=KeyLineType(Key.line3,l);if(!(t& KPD_ALPHA)						|| l==0) { ret=255;ERR2("Line 3 failed, %2d %04x",l,t); }	// Adresse
		l=KeyLineLen(Key.line4);t=KeyLineType(Key.line4,l);if(!(t& KPD_ALPHA)	|| (t& KPD_NUM)		|| l==0) { ret=255;ERR2("Line 4 failed, %2d %04x",l,t); }	// Stadt
		l=KeyLineLen(Key.line5);t=KeyLineType(Key.line5,l);if(!(t& KPD_ALPHANUM)					|| l==0) { ret=255;ERR2("Line 5 failed, %2d %04x",l,t); }	// PLZ
		l=KeyLineLen(Key.line6);t=KeyLineType(Key.line6,l);if(  t!=KPD_ALPHA						|| l==0) { ret=255;ERR2("Line 6 failed, %2d %04x",l,t); }	// Land
		l=KeyLineLen(Key.line7);t=KeyLineType(Key.line7,l);if(!(t& KPD_NUMPUNCT) || (t& KPD_ALPHA)	|| l==0) { ret=255;ERR2("Line 7 failed, %2d %04x",l,t); }	// Datum
		l=KeyLineLen(Key.line8);t=KeyLineType(Key.line8,l);if(  t!=KPD_ALPHA						|| l>10) { ret=255;ERR2("Line 8 failed, %2d %04x",l,t); }	// Status
		l=KeyLineLen(Key.line9);t=KeyLineType(Key.line9,l);if(  t!=KPD_ALPHA						|| l> 7) { ret=255;ERR2("Line 9 failed, %2d %04x",l,t); }	// Programm
		if(ret==255) RunTime.keyerr|=KEYERR_INVALID_KEY_CONTENTS;
		MSG("ready");
	}
#endif

	OUTRO;

	return(ret);
}

//-- helper

UBYTE KeyLineLen(char *str)
{
	register WORD i=strlen(str);

	if(i>0) {
		i--;
		while(i>-1 && str[i]==' ') i--;
		i++;
	}
	return((UBYTE)i);
}

UBYTE KeyLineType(char *str,UBYTE l)
{
	UBYTE typ=0;
	register UBYTE i;
	
	for(i=0;i<l;i++) {
		if(isdigit(str[i]))	typ|=KPD_NUM;
		if(isalpha(str[i]))	typ|=KPD_ALPHA;
		if(ispunct(str[i]))	typ|=KPD_PUNCT;
		if(str[i]< 32)		typ|=KPD_SOND;
		if(str[i]>160)		typ|=KPD_ALPHA;
	}
	return(typ);
}

//-- eof ----------------------------------------------------------------------
