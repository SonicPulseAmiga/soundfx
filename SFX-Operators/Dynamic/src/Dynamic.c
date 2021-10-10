/****h* Dynamic/Dynamic.c [4.3] *
*
*  NAME
*    Dynamic.c
*  COPYRIGHT
*    $VER: Dynamic.c 4.3 (01.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Dynamic core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    01.Apr.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*  IDEAS
*    graphs for min-mod and max-mod
*
*******
*/
/****u* _operators/Dynamic *
*  Function.eng
*    Amplifies or deamplifies the volume of a sample depending on its amplitude.
*    Provides complex changes of the <a href="../node06.03.00.Dynamic.html">dynamics</a> of the sample.
*  Funktion.ger
*    Verstärkt bzw. verringert die Lautstärke des Samples in Abhängigkeit von seiner Amplitude.
*    Ermöglicht komplexe Eingriffe in die <a href="../node06.03.00.Dynamic.html">Dynamik</a> des Samples.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Ratio loud (<a href="../node03.02.01.html">P2</a>)
*      amplitude-change of loud signals
*    Ratio quiet (<a href="../node03.02.01.html">P3</a>)
*      amplitude-change of quiet signals
*    Threshold (<a href="../node03.02.01.html">P4</a>)
*      determines the break point between the quiet and loud ratio - whenever the signals
*      amplitude exceeds the threshold the loud ratio will be applied otherwise the quiet
*      ratio will be used.
*    Knee
*      the are two variant, one is edgy and the other fades smoothly
*    Characteristics
*      These graphs show the effect of the settings. Read it a a translation table - the volume
*      of the source sample denotes the x postion, then the curve can be used to find the
*      respective y postion which denotes the output volume. 
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Ratio loud (<a href="../node03.02.01.html">P2</a>)
*      Lautstärkeänderung für die lauten Signale
*    Ratio quiet (<a href="../node03.02.01.html">P3</a>)
*      Lautstärkeänderung für die leisen Signale
*    Threshold (<a href="../node03.02.01.html">P4</a>)
*      bestimmt den Übergangspunkt zwischen leiser und lauter Ratio - immer wenn die Amplitude
*      des Signals den Schwellwert überschreitet wird die 'loud Ratio' angewendet, sonst wird
*      die 'quiet Ratio' gewählt
*    Knee
*      es gibt zwei Varianten, eine eckige und eine geglättete
*    Characteristics
*      Diese Kurven zeigen die Auswirkungen der Einstellungen. Lesen sie es wie eine Übersetzungstabelle - die
*      Lautstärke des Eingangssignals bestimmt die x Position, dann kann über die Kurve die zugehörige y Postion
*      bestimmt werden, welche die die Ausgangslautstärke darstellt.
*  Notes.eng
*    Here are a few examples:<ul>
*    <li>Compressor: squeezes the sample together:)<br>Ratio loud &lt;100 %, Ratio quiet &gt;100 %</li>
*    <li>Expander: expands the sample<br>Ratio loud &gt;100, Ratio quiet &lt;100 %</li>
*    <li>Limiter: amplifies the quiet parts of the sample<br>Ratio loud =100, Ratio quiet &gt;100 %</li>
*    <li>Delimiter: amplifies the loud parts of the sample<br>Ratio loud &gt;100, Ratio quiet =100 %</li>
*    </ul>
*  Hinweise.ger
*    Folgend noch ein paar Beispiele:<ul>
*    <li>Compresor: staucht das Sample<br>Ratio loud &lt;100 %, Ratio quiet &gt;100 %</li>
*    <li>Expander: zerrt das Sample auseinander<br>Ratio loud &gt;100, Ratio quiet &lt;100 %</li>
*    <li>Limiter: verstärkt die leisen Teile des Samples<br>Ratio loud =100, Ratio quiet &gt;100 %</li>
*    <li>Delimiter: verstärkt die lauten Teile des Samples<br>Ratio loud &gt;100, Ratio quiet =100 %</li>
*    </ul>
*******
*/

#define DYNAMIC_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Dynamic.h"

//-- Debugtools

#define DEBUG_SRCPATH "project:sfx-operators/"PRJ_NAME"/"
#include <debug/dt_decl.h>

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

void process_hard(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double rlos,rlod,rlo;
	double rqus,rqud,rqu;
	double exts,extd,ext;
	//double ratio;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	LONG curval;
	double ival,oval;
	double thrx,thry;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	rlos=instance->rlos;
	rlod=instance->rlod;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	rqus=instance->rqus;
	rqud=instance->rqud;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	exts=instance->exts;
	extd=instance->extd;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		rlo=rlos+rlod*modfunc1(curlen,instance->slen,moddata1);
		rqu=rqus+rqud*modfunc2(curlen,instance->slen,moddata2);
		ext=exts+extd*modfunc3(curlen,instance->slen,moddata3);

		thrx=ext;thry=thrx*rqu;
		if(thry>1.0) {
			thry=1.0;
			thrx=thry/rqu;
		}

		//-- old smoothed version
		//ratio=((double)labs(*sbuf))/(double)(SMP_MAXP+1);
		//if(ext!=1.0) ratio=mypow(ratio,ext);
		//ratio=rqu+(ratio*(rlo-rqu));

		curval=*sbuf++;
		ival=((double)labs(curval))/(double)(SMP_MAXP+1);
		if(ival<thrx) {	// apply rqu
			oval=ival*rqu;
		}
		else {			// apply rlo
			oval=thry+((ival-thrx)*rlo);
		}

		if(curval<0) curval=(LONG)(oval*SMP_MAXN);
		else curval=(LONG)(oval*SMP_MAXP);
		CLIPINT(curval);
		*(dbuf++)=(WORD)curval;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_soft(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double rlos,rlod,rlo;
	double rqus,rqud,rqu;
	double exts,extd,ext;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	LONG curval;
	double t0,t1,ival,oval,val;
	double thrx,thry,midx;
	double datax[3],datay[3];
	double bdl,bdu/*,dif*/;
	UBYTE j;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	rlos=instance->rlos;
	rlod=instance->rlod;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	rqus=instance->rqus;
	rqud=instance->rqud;
	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	exts=instance->exts;
	extd=instance->extd;

	// (constant) interpolation data
	datax[0]= 0.0;          datay[0]=0.0;
	datax[2]= 1.0;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		rlo=rlos+rlod*modfunc1(curlen,instance->slen,moddata1);
		rqu=rqus+rqud*modfunc2(curlen,instance->slen,moddata2);
		ext=exts+extd*modfunc3(curlen,instance->slen,moddata3);

		thrx=ext;thry=thrx*rqu;
		if(thry>1.0) {
			thry=1.0;
			thrx=thry/rqu;
		}
		// interpolation data
		val=thry+(1.0-thrx)*rlo;	// end gain
		/*datax[0]= 0.0;            datay[0]=0.0;*/
		  datax[1]= thrx;           datay[1]=thry;
		/*datax[2]= 1.0;*/          datay[2]=val;

		curval=*sbuf++;
		ival=((double)labs(curval))/(double)(SMP_MAXP+1);

		// search midx to be approx ival, then oval=midy
		/*
		// compleete search
		dif=1.0;
		for(t0=0.0;t0<(1.0+(1/500.0));t0+=(1/500.0)) {
			// bezier with 3 control points
			t1=1.0-t0;
			midx=datax[0]*t1*t1 + datax[1]*2.0*t0*t1 + datax[2]*t0*t0;
			//midy=datay[0]*t1*t1 + datay[1]*2.0*t0*t1 + datay[2]*t0*t0;
			if(fabs(ival-midx)<dif) {
				dif=fabs(ival-midx);
				oval=datay[0]*t1*t1 + datay[1]*2.0*t0*t1 + datay[2]*t0*t0;
			}
		}
		*/
		// binary search
		//MSGC1((curlen<40),"ival=%6.4f",ival);
		bdl=0.0;bdu=1.0;
		for(j=0;j<10;j++) {
			t0=bdl+((bdu-bdl)*0.5);
			t1=1.0-t0;
			midx=datax[0]*t1*t1 + datax[1]*2.0*t0*t1 + datax[2]*t0*t0;
			//midy=datay[0]*t1*t1 + datay[1]*2.0*t0*t1 + datay[2]*t0*t0;
			//MSGC6(((curlen>25) && (curlen<40)),"  j=%2d bdl=%6.4f t0=%6.4f bdu=%6.4f  midx=%6.4f  dif=%6.4f",j,bdl,t0,bdu,midx,fabs(ival-midx));
			if(fabs(ival-midx)<0.001) break;
			else if(ival<midx) bdu=t0;
			else bdl=t0;
		}
		oval=datay[0]*t1*t1 + datay[1]*2.0*t0*t1 + datay[2]*t0*t0;


		if(curval<0) curval=(LONG)(oval*SMP_MAXN);
		else curval=(LONG)(oval*SMP_MAXP);
		CLIPINT(curval);
		*(dbuf++)=(WORD)curval;

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
