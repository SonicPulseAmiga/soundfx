/****h* ConvertChannels/ConvertChannels.c [4.2] *
*
*  NAME
*    ConvertChannels.c
*  COPYRIGHT
*    $VER: ConvertChannels.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    ConvertChannels core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/ConvertChannels *
*  Function.eng
*    Converts between different <a href="../node06.03.00.Channel.html">channel</a> formats
*  Funktion.ger
*    Wandelt zwischen verschiedenen <a href="../node06.03.00.Channel.html">Kanal</a>formaten
*  Parameter.eng
*    Matrix (Mat x y)
*      All input values a multiplied by those factors and outputed as a sum.
*      Meaningful values for the factors are between -1.0 and 1.0.
*  Parameter.ger
*    Matrix (Mat x y)
*      Die Eingangswerte werden mit diesen Faktoren multipliziert und als Summe ausgegeben.
*      Sinnvolle Werte für die Faktoren liegen zwischen -1.0 und 1.0.
*  Notes.eng
*    This operator is capable of about every thinkable channel transformation.
*    The sample is feed into the source side and comes out of the
*    destination side. The result will have as many channels, as there are
*    filled destination rows.<br>
*    The included presets nicely demonstrating the way it works.
*  Hinweise.ger
*    Hiermit lassen sich nahezu alle denkbaren Kanalverwandlungen
*    realisieren. Das Sample liegt an der Sourceseite an und gelangt
*    an der Zielseite heraus. Das Ergebnis wird soviele Kanäle haben,
*    wie belegte Zielspalten existieren.<br>
*    Es liegen viele Presets bei, die die Arbeitsweise gut verdeutlichen.
*******
*/

#define CONVERTCHANNELS_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "ConvertChannels.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void process_m(SFXCoreInstance *instance,unsigned long samples) {
	unsigned char l;
	unsigned long i,curlen;
	SAMPLE *dbuf0,*sbuf[4];
	double mat0[4];

	curlen=instance->curlen;

	// Source, Dest., Par.
	for(l=0;l<=instance->srcch;l++) {
		sbuf[l]=instance->sbuf[l];
		mat0[l]=instance->mat0[l];
	}
	dbuf0=instance->dbuf[0];

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*dbuf0=0;
		for(l=0;l<=instance->srcch;l++) {
			*dbuf0+=mat0[l]*(*sbuf[l]);
			sbuf[l]++;
		}
		dbuf0++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_s(SFXCoreInstance *instance,unsigned long samples) {
	unsigned char l;
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*sbuf[4];
	double mat0[4],mat1[4];

	curlen=instance->curlen;

	// Source, Dest., Par.
	for(l=0;l<=instance->srcch;l++) {
		sbuf[l]=instance->sbuf[l];
		mat0[l]=instance->mat0[l];
		mat1[l]=instance->mat1[l];
	}
	dbuf0=instance->dbuf[0];
	dbuf1=instance->dbuf[1];

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*dbuf0=0;*dbuf1=0;
		for(l=0;l<=instance->srcch;l++) {
			*dbuf0+=mat0[l]*(*sbuf[l]);
			*dbuf1+=mat1[l]*(*sbuf[l]);
			sbuf[l]++;
		}
		dbuf0++;dbuf1++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_q(SFXCoreInstance *instance,unsigned long samples) {
	unsigned char l;
	unsigned long i,curlen;
	SAMPLE *dbuf0,*dbuf1,*dbuf2,*dbuf3,*sbuf[4];
	double mat0[4],mat1[4],mat2[4],mat3[4];

	curlen=instance->curlen;

	// Source, Dest., Par.
	for(l=0;l<=instance->srcch;l++) {
		sbuf[l]=instance->sbuf[l];
		mat0[l]=instance->mat0[l];
		mat1[l]=instance->mat1[l];
		mat2[l]=instance->mat2[l];
		mat3[l]=instance->mat3[l];
	}
	dbuf0=instance->dbuf[0];
	dbuf1=instance->dbuf[1];
	dbuf2=instance->dbuf[2];
	dbuf3=instance->dbuf[3];

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		*dbuf0=0;*dbuf1=0;*dbuf2=0;*dbuf3=0;
		for(l=0;l<=instance->srcch;l++) {
			*dbuf0+=mat0[l]*(*sbuf[l]);
			*dbuf1+=mat1[l]*(*sbuf[l]);
			*dbuf2+=mat2[l]*(*sbuf[l]);
			*dbuf3+=mat3[l]*(*sbuf[l]);
			sbuf[l]++;
		}
		dbuf0++;dbuf1++;dbuf2++;dbuf3++;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
