/****h* SoundFX/SFX-SmpUnits.c [4.3] *
*
*  NAME
*    SFX-SmpUnits.c
*  COPYRIGHT
*    $VER: SFX-SmpUnits.c 4.3 (03.03.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    unit conversion for sampleunits - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    12.Aug.1998
*  MODIFICATION HISTORY
*    03.Mar.2004	V 4.3	most recent version
*    12.Nov.1998	V 3.71	most recent version
*    12.Aug.1998	V 3.70	most recent version
*    12.Aug.1998	V 3.70	initial version
*  NOTES
*
*******
*/

#define SFX_SmpUnits_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

char  *ToXUnit (UBYTE unitx,ULONG val,ULONG srat);
ULONG FromXUnit(UBYTE unitx,char *text,ULONG srat);
char  *ToYUnit (UBYTE unity,LONG  val);
LONG  FromYUnit(UBYTE unity,char *text);

//-- definitions --------------------------------------------------------------

char *ToXUnit(UBYTE unitx,ULONG val,ULONG srat)
{
	static Param text;
	Param text2;

	switch(unitx) {
		case ATME_SAMPLES:	sprintf(text,"%ld",val);break;
		case ATME_H_M_S_MS:
		case ATME_H_M_S_MF:
		case ATME_H_M_S_PF:
		case ATME_H_M_S_NF:
			sprintf(text2,"%lf ms",(double)val/((double)srat/1000.0));
			UC_ConvertParameter(text,text2,UGRP_ABSTIME,unitx|DONT_ADD_UNIT_NAME);
			break;
		UNEXPECTED_DEFAULT
	}
	return(text);
}

ULONG FromXUnit(UBYTE unitx,char *text,ULONG srat)
{
	ULONG val;
	double dval;
	register UBYTE i,j[4],jnr;

	//-- skip leading spaces
	while(*text==' ') text++;
	//-- ignore negative values
	if(*text=='-') text++;

	//-- count parts in compond values
	if(unitx!=ATME_SAMPLES)	{
		i=j[0]=0;jnr=1;
		while(text[i]) {			// search delimiter
			if(text[i]==':') {
				j[jnr++]=i+1;text[i]='\0';
				MSG2("%d : [%s]",jnr-2,&text[j[jnr-2]]);
			}	// Delimiter gefunden
			i++;
		}
		MSG2("%d : [%s]",jnr-1,&text[j[jnr-1]]);
		MSG1("AnzValues=%d",jnr);
	}
	switch(unitx) {
		case ATME_SAMPLES:	val=atoi(text);break;
		case ATME_H_M_S_MS:
			switch(jnr) {
				case 1: dval=                                                                               atof(&text[j[0]]);break;
				case 2: dval=                                                      atof(&text[j[0]])*1000.0+atof(&text[j[1]]);break;
				case 3: dval=                            atof(&text[j[0]])*60000.0+atof(&text[j[1]])*1000.0+atof(&text[j[2]]);break;
				case 4: dval=atof(&text[j[0]])*3600000.0+atof(&text[j[1]])*60000.0+atof(&text[j[2]])*1000.0+atof(&text[j[3]]);break;
			}
			val=(ULONG)(dval*((double)srat/1000.0));
			break;
		case ATME_H_M_S_MF:
			switch(jnr) {
				case 1: dval=                                                                               atof(&text[j[0]])*0.024;break;
				case 2: dval=                                                      atof(&text[j[0]])*1000.0+atof(&text[j[1]])*0.024;break;
				case 3: dval=                            atof(&text[j[0]])*60000.0+atof(&text[j[1]])*1000.0+atof(&text[j[2]])*0.024;break;
				case 4: dval=atof(&text[j[0]])*3600000.0+atof(&text[j[1]])*60000.0+atof(&text[j[2]])*1000.0+atof(&text[j[3]])*0.024;break;
			}
			val=(ULONG)(dval*((double)srat/1000.0));
			break;
		case ATME_H_M_S_PF:
			switch(jnr) {
				case 1: dval=                                                                               atof(&text[j[0]])*0.025;break;
				case 2: dval=                                                      atof(&text[j[0]])*1000.0+atof(&text[j[1]])*0.025;break;
				case 3: dval=                            atof(&text[j[0]])*60000.0+atof(&text[j[1]])*1000.0+atof(&text[j[2]])*0.025;break;
				case 4: dval=atof(&text[j[0]])*3600000.0+atof(&text[j[1]])*60000.0+atof(&text[j[2]])*1000.0+atof(&text[j[3]])*0.025;break;
			}
			val=(ULONG)(dval*((double)srat/1000.0));
			break;
		case ATME_H_M_S_NF:
			switch(jnr) {
				case 1: dval=                                                                               atof(&text[j[0]])*0.030;break;
				case 2: dval=                                                      atof(&text[j[0]])*1000.0+atof(&text[j[1]])*0.030;break;
				case 3: dval=                            atof(&text[j[0]])*60000.0+atof(&text[j[1]])*1000.0+atof(&text[j[2]])*0.030;break;
				case 4: dval=atof(&text[j[0]])*3600000.0+atof(&text[j[1]])*60000.0+atof(&text[j[2]])*1000.0+atof(&text[j[3]])*0.030;break;
			}
			val=(ULONG)(dval*((double)srat/1000.0));
			break;
		UNEXPECTED_DEFAULT
	}
	return(val);
}

char *ToYUnit(UBYTE unity,LONG val)
{
	static char text[PARBUFLEN+1];

	switch(unity) {
		case AMP_FC:		sprintf(text,"%+6.4lf",(double)val/32768.0);break;
		case AMP_ABSPROZ:	sprintf(text,"%+6ld",(LONG)((double)val/327.68));break;
		case AMP_ABSPROM:	sprintf(text,"%+6ld",(LONG)((double)val/32.768));break;
		case AMP_LEVEL:		sprintf(text,"%+6ld",val);break;
		case AMP_DEZIBEL:
			if(val!=0.0) sprintf(text,"%+6.4lf",(3.0*log(fabs((double)val/32768.0)))/log(1.41421356237));
			else sprintf(text,"   inf");
			break;
		UNEXPECTED_DEFAULT
	}
	return(text);
}

LONG FromYUnit(UBYTE unity,char *text)
{
	LONG val;

	switch(unity) {
		case AMP_FC:		val=(ULONG)(32768.000*atof(text));break;
		case AMP_ABSPROZ:	val=(ULONG)(  327.680*atof(text));break;
		case AMP_ABSPROM:	val=(ULONG)(   32.768*atof(text));break;
		case AMP_LEVEL:		val=atoi(text);break;
		case AMP_DEZIBEL:
			if(strcmp(text,"   inf")) val=(ULONG)(32768.0*mypow(1.41421356237,(atof(text)/3.0)));
			else val=0.0;
			break;
		UNEXPECTED_DEFAULT
	}
	return(val);
}

//-- eof ----------------------------------------------------------------------
