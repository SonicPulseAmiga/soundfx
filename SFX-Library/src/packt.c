/****h* sfxsupport.library/packt.c [1.30] *
*
*  NAME
*    packt.c
*  COPYRIGHT
*    $VER: packt.c 1.30 (18.06.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    packertools for loader & savers - definitions
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    09.Feb.1996
*  MODIFICATION HISTORY
*    18.Jun.2000	V 1.30	most recent version
*    18.Sep.1998	V 1.20	most recent version
*    09.Feb.1996	V 1.20	initial version
*  NOTES
*
*******
*/

#define PACKT_C

//-- includes -----------------------------------------------------------------

#include "sfxlib.h"
#include "project:sfx-main/sfx-filedefs.h"

//-- prototypes ---------------------------------------------------------------

void ASM LIB_EXPORT(GenIOTables(void));

void GenFibDltTab_4(BYTE *fibtab);
void GenFibDltTab_6(LONG *fibtab);
void GenExpDltTab_4(BYTE *exptab);
void GenExpDltTab_5(LONG *exptab);
void GenULawTab_8(WORD *exptab);

void CalculateBlockSizes(UBYTE crunch,ULONG *hdrlen,ULONG *datalen);

//-- globals ------------------------------------------------------------------

BYTE fibtab4[16];
LONG fibtab6[64];
BYTE exptab4[16];
LONG exptab5[32];
WORD ulawtab[8];

//  Fixed point Delta adaption table:
//  Next Delta = Delta * gaiP4tab[ this output ] / MSADPCM_PSCALE
WORD gaiP4tab[] = { 230, 230, 230, 230, 307, 409, 512, 614, 768, 614, 512, 409, 307, 230, 230, 230 };

//-- definitions --------------------------------------------------------------

void SAVEDS ASM LIB_EXPORT(GenIOTables(void)) {
	GenFibDltTab_4(fibtab4);
	GenFibDltTab_6(fibtab6);
	GenExpDltTab_4(exptab4);
	GenExpDltTab_5(exptab5);
	GenULawTab_8(ulawtab);
}

void GenFibDltTab_4(BYTE *fibtab) {
	register BYTE i;

	fibtab[6]=-2;fibtab[7]=-1;fibtab[8]=0;fibtab[9]=1;fibtab[10]=2;

	for(i=11;i<16;i++) fibtab[i]=fibtab[i-1]+fibtab[i-2];
	for(i= 5;i>=0;i--) fibtab[i]=fibtab[i+1]+fibtab[i+2];
}

void GenFibDltTab_6(LONG *fibtab) {
	register BYTE i;

	fibtab[30]=-2;fibtab[31]=-1;fibtab[32]=0;fibtab[33]=1;fibtab[34]=2;

	for(i=35;i< 56;i++) fibtab[i]=fibtab[i-1]+fibtab[i-2];
	for(i=56;i< 64;i++) fibtab[i]=fibtab[55];
	for(i=29;i>= 9;i--) fibtab[i]=fibtab[i+1]+fibtab[i+2];
	for(i= 8;i>= 0;i--) fibtab[i]=fibtab[ 9];

}

void GenExpDltTab_4(BYTE *exptab) {
	register BYTE i;

	for(i=0;i<8;i++) exptab[i]=-(1<<(7-i));
	exptab[8]=0;
	for(i=9;i<16;i++) exptab[i]=1<<(i-9);
}

void GenExpDltTab_5(LONG *exptab) {
	register BYTE i;

	for(i=0;i<16;i++) exptab[i]=-(1<<(15-i));
	exptab[16]=0;
	for(i=17;i<32;i++) exptab[i]=1<<(i-17);
}

// 1  2     -1...0
// 2  4     -2...1
// 3  8     -8...4
// 4 16   -128...64
// 5 32 -32768...16384

void GenULawTab_8(WORD *exptab) {
	register UBYTE i;

	for(i=0;i<8;i++) {
		exptab[i]=((1<<i)-1)*132;
	}
}

void CalculateBlockSizes(UBYTE crunch,ULONG *hdrlen,ULONG *datalen) {
	switch(crunch) {			//  Kopflänge (Bytes)	Datenblocklänge
		case CRUNCH_PCM32:			*hdrlen=0;			*datalen=MultipleOf(16,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_PCM24:			*hdrlen=0;			*datalen=MultipleOf(12,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_PCM16:			*hdrlen=0;			*datalen=MultipleOf( 8,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_PCM12:			*hdrlen=0;			*datalen=MultipleOf( 3,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_PCM8:
		case CRUNCH_ULAW:
		case CRUNCH_ULAW_INV:
		case CRUNCH_ALAW:
		case CRUNCH_ALAW_INV:		*hdrlen=0;			*datalen=MultipleOf( 4,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_IEEE64:			*hdrlen=0;			*datalen=MultipleOf(32,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_IEEE32:			*hdrlen=0;			*datalen=MultipleOf(16,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_FDPCM8_4:		*hdrlen=2;			*datalen=MultipleOf( 4,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_FDPCM16_6:		*hdrlen=2;			*datalen=MultipleOf(24,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_EDPCM8_4:		*hdrlen=2;			*datalen=MultipleOf( 4,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_EDPCM16_5:		*hdrlen=2;			*datalen=MultipleOf(40,GetRunTime(sfxprefs_vmem)->blksize);	break;
		case CRUNCH_VOC_ADPCM8_4:
		case CRUNCH_VOC_ADPCM8_3:
		case CRUNCH_VOC_ADPCM8_2:	*hdrlen=2;			*datalen=MultipleOf( 4,GetRunTime(sfxprefs_vmem)->blksize);	break;
	}
}

//-- eof ----------------------------------------------------------------------
