/****h* Mix-3D/Mix-3D.c [4.2] *
*
*  NAME
*    Mix-3D.c
*  COPYRIGHT
*    $VER: Mix-3D.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Mix-3D core file
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
/****u* _operators/Mix-3D *
*  Function.eng
*    Mixes 8 samples via a path in a cube
*  Funktion.ger
*    Mischt 8 Samples über Pfad in einen Würfel
*  Parameter.eng
*    Sources
*      The source-samples which then go into the mix.
*    X-Axis (<a href="../node03.02.01.html">P1</a>)
*      location of the point on the X-axis
*    Y-Axis (<a href="../node03.02.01.html">P2</a>)
*      location of the point on the Y-axis
*    Z-Axis (<a href="../node03.02.01.html">P3</a>)
*      location of the point on the Z-axis
*    Path
*      This area shows the path of the curve inside the cube. During the
*      calculation a point will wander along the curve from one end to the other.
*      The distance of the point to the corners defines how much of the source
*      assigned to that corner will gets mixed into the result.<br>
*      With the "View" gadget, you can choose from where to look on the cube
*      and with "Prec." you choose how detailed the curve will be drawn.
*  Parameter.ger
*    Sources
*      Die Ausgangssamples die gemischt werden.
*    X-Axis (<a href="../node03.02.01.html">P1</a>)
*      Position des Punktes auf der X-Achse
*    Y-Axis (<a href="../node03.02.01.html">P2</a>)
*      Position des Punktes auf der Y-Achse
*    Z-Axis (<a href="../node03.02.01.html">P3</a>)
*      Position des Punktes auf der Z-Achse
*    Path
*      In diesem Feld wird der Pfad, als Kurve im Würfel, dargestellt. Während
*      der Berechnung wird ein Punkt entlang der Kurve vom Begin bis zum Ende
*      wandern. Die Entfernung dies Punktes zu den Ecken bestimmt wie laut die
*      zu diesen Ecken zugeordneten Samples in das Ergebnis gemischt werden.<br>
*      Mit "View" läßt sich der Ansichtspunkt festlegen und mit "Prec." die
*      Genauigkeit mit der die Kurve gezeichnet wird.
*  Notes.eng
*    Just mix various versions of one sample together and do this twice with different
*    curves. Then join the results to one stereo sample.
*  Hinweise.ger
*    Mixen sie doch mal verschiedene Variationen eines Samples zusammen. Wiederholen
*    sie dies mit einer anderen Kurve und verbinden sie die Resultate zu einem
*    Stereosample.
*******
*/

#define MIX_3D_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Mix-3D.h"

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
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double axis_xs,axis_x1,axis_x2,axis_xd;
	double axis_ys,axis_y1,axis_y2,axis_yd;
	double axis_zs,axis_z1,axis_z2,axis_zd;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf1,*sbuf2,*sbuf3,*sbuf4,*sbuf5,*sbuf6,*sbuf7,*sbuf8;
	ULONG en1,en2,en3,en4,en5,en6,en7,en8;\
	double val1;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf1=instance->sbuf1;en1=instance->en1;
	sbuf2=instance->sbuf2;en2=instance->en2;
	sbuf3=instance->sbuf3;en3=instance->en3;
	sbuf4=instance->sbuf4;en4=instance->en4;
	sbuf5=instance->sbuf5;en5=instance->en5;
	sbuf6=instance->sbuf6;en6=instance->en6;
	sbuf7=instance->sbuf7;en7=instance->en7;
	sbuf8=instance->sbuf8;en8=instance->en8;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	axis_xs=instance->axis_xs;
	axis_xd=instance->axis_xd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	axis_ys=instance->axis_ys;
	axis_yd=instance->axis_yd;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	axis_zs=instance->axis_zs;
	axis_zd=instance->axis_zd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		axis_x1=1.0-(axis_x2=(axis_xs+axis_xd*modfunc1(curlen,instance->slen,moddata1)));
		axis_y1=1.0-(axis_y2=(axis_ys+axis_yd*modfunc2(curlen,instance->slen,moddata2)));
		axis_z1=1.0-(axis_z2=(axis_zs+axis_zd*modfunc3(curlen,instance->slen,moddata3)));

		val1=0.0;
		if(curlen<en1) val1+=((double)*(sbuf1++)*(axis_x1*axis_y1*axis_z1));
		if(curlen<en2) val1+=((double)*(sbuf2++)*(axis_x2*axis_y1*axis_z1));
		if(curlen<en3) val1+=((double)*(sbuf3++)*(axis_x2*axis_y1*axis_z2));
		if(curlen<en4) val1+=((double)*(sbuf4++)*(axis_x1*axis_y1*axis_z2));
		if(curlen<en5) val1+=((double)*(sbuf5++)*(axis_x1*axis_y2*axis_z1));
		if(curlen<en6) val1+=((double)*(sbuf6++)*(axis_x2*axis_y2*axis_z1));
		if(curlen<en7) val1+=((double)*(sbuf7++)*(axis_x2*axis_y2*axis_z2));
		if(curlen<en8) val1+=((double)*(sbuf8++)*(axis_x1*axis_y2*axis_z2));
		*(dbuf++)=(SAMPLE)val1;

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
