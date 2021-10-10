/****h* Hall/Hall.c [4.3] *
*
*  NAME
*    Hall.c
*  COPYRIGHT
*    $VER: Hall.c 4.3 (28.01.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    Hall core file
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    11.Mar.2003
*  MODIFICATION HISTORY
*    28.Jan.2004	V 4.3	most recent version
*    11.Mar.2003	V 4.2	most recent version
*    11.Mar.2003	V 4.2	initial version
*  NOTES
*
*******
*/
/****u* _operators/Hall *
*  Function.eng
*    Reverberates the signal. Simulates three reflectionphases - early reflections, mainhall, diffuse hall.
*  Funktion.ger
*    Verhallt das Signal. Es werden drei Reflektionsphasen nachgebildet - Frühreflektionen, Haupthall, diffuser Hall (Nachhall).
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Feedback, Early Reflections (<a href="../node03.02.01.html">P2</a>)
*      how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
*    Volume, Early Reflections (<a href="../node03.02.01.html">P3</a>)
*      how loud the early reflections appear in the result
*    Delay, Early Reflections (ErDelS,ErDelE,ErNr)
*      nr of delays and the time-range they cover
*    Feedback, Main Reflections (<a href="../node03.02.01.html">P4</a>)
*      how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
*    Delay, Main Reflections (MrDelS,MrDelE,MrNr)
*      nr of delays and the time-range they cover
*    Diff
*      length of diffuse hall.
*    Ampf
*      final amplification
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Feedback, Early Reflections (<a href="../node03.02.01.html">P2</a>)
*      wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
*      negativ sein um ein inverses Feedback zu erzeugen.
*    Volume, Early Reflections (<a href="../node03.02.01.html">P3</a>)
*      wie laut die Frühreflektionen im Ergebnis zu hören sein sollen
*    Delay, Early Reflections (ErDelS,ErDelE,ErNr)
*      Anzahl der Echos und der abgedeckte Zeitbereich
*    Feedback, Main Reflections (<a href="../node03.02.01.html">P4</a>)
*      wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
*      negativ sein um ein inverses Feedback zu erzeugen.
*    Delay, Main Reflections (MrDelS,MrDelE,MrNr)
*      Anzahl der Echos und der abgedeckte Zeitbereich
*    Diff
*      Länge des diffusen Halls.
*    Ampf
*      abschließende Lautstärkeanpassung
*  Notes.eng
*    I know that this is far from perfect yet. It's basically the same algorithm
*    as before, but with lots of parameters exposed.
*  Hinweise.ger
*    Ich weiß das das immer noch nicht perfekt ist. Der Algorithmus ist im Prinzip
*    der gleiche wie zuvor, nur das nun vile Parameter offengelegt worden sind.
*******
*/

#define HALL_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Hall.h"

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
	double mrdel,delst;
	//unsigned long j;
	unsigned char /*prime,*/k;
	/*unsigned long del;*/
	double ffca,facbk,facbkst,bright;

	instance->mdelv=(ULONG)(1.2*instance->erdele);				// max delay for multi-tap delay
	MSG1("max early ref del : %ld",instance->mdelv);

	ffca=instance->drfc*((double)(instance->srat)/44100.0);
	instance->mdela[0]=(ULONG)(225.0*ffca);						// delays for allpass filters
	instance->mdela[1]=(ULONG)(341.0*ffca);
	instance->mdela[2]=(ULONG)(439.0*ffca);
	instance->mdela[3]=(ULONG)(556.0*ffca);
	//MSG1("mdela[0] : %ld",instance->mdela[0]);
	//MSG1("mdela[1] : %ld",instance->mdela[1]);
	//MSG1("mdela[2] : %ld",instance->mdela[2]);
	//MSG1("mdela[3] : %ld",instance->mdela[3]);
	instance->facda[0]=0.72;instance->facdaa[0]=1.0-(instance->facda[0]*instance->facda[0]);	// feedback for allpass filters
	instance->facda[1]=0.63;instance->facdaa[1]=1.0-(instance->facda[1]*instance->facda[1]);
	instance->facda[2]=0.57;instance->facdaa[2]=1.0-(instance->facda[2]*instance->facda[2]);
	instance->facda[3]=0.49;instance->facdaa[3]=1.0-(instance->facda[3]*instance->facda[3]);

	if(!(instance->bufv	=(double  *)AllocVec((instance->mdelv*sizeof(double  )),MEMF_ANY))) return(FALSE);
	if(!(instance->delv	=(ULONG   *)AllocVec((instance->vanz *sizeof(ULONG   )),MEMF_ANY))) return(FALSE);
	if(!(instance->faccv=(double  *)AllocVec((instance->vanz *sizeof(double  )),MEMF_ANY))) return(FALSE);
	if(!(instance->delk	=(ULONG   *)AllocVec((instance->kanz *sizeof(ULONG   )),MEMF_ANY))) return(FALSE);
	if(!(instance->mdelk=(ULONG   *)AllocVec((instance->kanz *sizeof(ULONG   )),(MEMF_ANY|MEMF_CLEAR)))) return(FALSE);
	if(!(instance->facbk=(double  *)AllocVec((instance->kanz *sizeof(double  )),MEMF_ANY))) return(FALSE);
	if(!(instance->fbbk	=(double  *)AllocVec((instance->kanz *sizeof(double  )),MEMF_ANY))) return(FALSE);
	if(!(instance->bufk	=(double **)AllocVec((instance->kanz *sizeof(double *)),MEMF_ANY))) return(FALSE);

	//-- exponential spread
	mrdel=delst=mypow(instance->mrdeld,1.0/(double)instance->kanz);
	MSG3("mr-delay : %lf , %d x %lf",instance->mrdels,instance->kanz,delst);
	for(k=0;k<instance->kanz;k++) {					// delay times for main reflections
		instance->mdelk[k]=instance->mrdels+mrdel;
		MSG3("mr-delay[%2d]=%ld (%lf)",k,(ULONG)instance->mdelk[k],mrdel);
		mrdel*=delst;
	}

	//-- linear spread with prime number alterations
	/*
	mrdel=instance->mrdels;
	delst=instance->mrdeld/(double)instance->kanz;
	MSG4("mr-delay : %lf + ( %d * %lf ) = %lf",mrdel,instance->kanz,delst,(instance->kanz*delst));
	for(k=0;k<instance->kanz;k++) {					// delay times for main reflections
		del=(ULONG)mrdel;									// minimum delay time
		prime=FALSE;
		while(!prime && (del>2)) {
			for(j=2;j<del;j++) {
				if((((ULONG)(del/j))*j)==del) {
					j=((ULONG)instance->mrdele)+1;del--;
				}
			}
			if(j==del) prime=TRUE;
		}
		if(prime) {
			instance->mdelk[k]=del;
			MSG3("mr-delay[%2d]=%ld -> %ld",k,(ULONG)mrdel,del);
		}
		else {
			ERR2("mr-delay[%2d]=%ld -> no prime found",k,(ULONG)mrdel);
			instance->mdelk[k]=(ULONG)mrdel;		   // was mrdel|1
		}
		mrdel+=delst;
	}
	*/

	//-- the smaller the values are, the brighter is the result
	bright=1.0-instance->bright;
	facbk=(0.7*bright)-(0.2*bright);
	facbkst=(facbk-((0.7*bright)+(0.2*bright)))/(double)instance->kanz;

	for(k=0;k<instance->kanz;k++) {					// coefficients for lowpass filters in comb-filters 
		instance->facbk[k]=facbk;
		facbk-=facbkst;
	}
	for(k=0;k<instance->kanz;k++) if(!(instance->bufk[k]=(double *)AllocVec((instance->mdelk[k]*sizeof(double)),MEMF_ANY))) return(FALSE);
	for(k=0;k<AANZ          ;k++) if(!(instance->bufa[k]=(double *)AllocVec((instance->mdela[k]*sizeof(double)),MEMF_ANY))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	double erdel,delst;
	//unsigned long j;
	unsigned char /*prime,*/k;
	//unsigned long del;
	BShapeFuncPtr modfunc3;
	void *moddata3;
	double ervols,ervold;
	double erdels,erdele,erdeld;

	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	AssignBShape(&instance->mod4);

	// Modulator 3
	moddata3=(void *)(instance->mod3.bshpdata);
	modfunc3=instance->mod3.modfunc;
	ervols=instance->ervols;
	ervold=instance->ervold;
	erdels=instance->erdels;
	erdele=instance->erdele;
	erdeld=instance->erdeld;

	memset(instance->bufv,0,instance->mdelv*sizeof(double));					/* Clear Delays */
	for(k=0;k<instance->kanz;k++) memset(instance->bufk[k],0,instance->mdelk[k]*sizeof(double));
	for(k=0;k<AANZ          ;k++) memset(instance->bufa[k],0,instance->mdela[k]*sizeof(double));

	//-- exponential spread
	erdel=delst=mypow(erdeld,1.0/(double)instance->vanz);
	MSG3("er-delay : %lf , %d x %lf",erdels,instance->vanz,delst);
	for(k=0;k<instance->vanz;k++) {					// delay times for main reflections
		instance->delv[k]=erdels+erdel;
		MSG3("er-delay[%2d]=%ld (%lf)",k,(ULONG)instance->delv[k],erdel);
		erdel*=delst;
	}

	//-- linear spread with prime number alterations
	/*
	erdel=erdels;
	delst=erdeld/(double)instance->vanz;
	MSG4("er-delay : %lf + ( %d * %lf ) = %lf",erdel,instance->vanz,delst,(instance->vanz*delst));
	for(k=0;k<instance->vanz;k++) {					// delay times for early reflections
		del=erdel;									// minimum delay time
		MSG2("er-delay[%2d]=%ld",k,del);
		prime=FALSE;
		while(!prime && (del>2)) {
			for(j=2;j<del;j++) {
				if((((ULONG)(del/j))*j)==del) {
					j=((ULONG)erdele)+1;del--;
				}
			}
			if(j==del) prime=TRUE;
		}
		if(prime) {
			instance->delv[k]=del;
			//instance->delv[k]=instance->mdelv-del;		// delays need to be behind
			MSG3("er-delay[%2d]=%ld -> %ld",k,(ULONG)erdel,del);
		}
		else {
			ERR2("er-delay[%2d]=%ld -> no prime found",k,(ULONG)erdel);
			ERR("no prime found");
			instance->delv[k]=((ULONG)erdel);			// was erdel|1
		}
		erdel+=delst;
	}
	*/
	erdels=(double)instance->delv[0];erdele=(double)instance->delv[instance->vanz-1];
	erdeld=erdele-erdels;
	if(erdeld<1.0) { erdeld=1.0; }
	MSG1("erdeld : %lf",erdeld);
	for(k=0;k<instance->vanz;k++) {					// delay volumes for early reflections
		instance->faccv[k]=ervols+ervold*modfunc3((ULONG)(instance->delv[k]-erdels),(ULONG)erdeld,moddata3);
		MSG3("er-delay[%2d] : t=%ld fc=%6.4lf",k,instance->delv[k],instance->faccv[k]);
		instance->delv[k]=instance->mdelv-instance->delv[k];					// delays need to be behind
	}
	for(k=0;k<instance->kanz;k++) { instance->delk[k]=0;instance->fbbk[k]=0.0; }
	for(k=0;k<AANZ;k++) { instance->dela[k]=0; }
	instance->delv0=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned char k;
	double effs,effd,eff,dry;
	double erfbs,erfbd,erfb,erdr;
	double mrfbs,mrfbd,mrfb,mrdr;
	BShapeFuncPtr modfunc1,modfunc2,modfunc4;
	void *moddata1,*moddata2,*moddata4;
	SAMPLE *dbuf,*sbuf;
	double out,out1,out2,out3,in;
	double ampf;

	ULONG vanz;													/* Vorverzögerung */
	ULONG *delv,delv0;
	ULONG mdelv;
	double *faccv;

	ULONG kanz;													/* Kammfilter */
	ULONG *delk,*mdelk;
	double *facbk=NULL;
	double *fbbk=NULL;
	double dout;

	ULONG *dela,*mdela;											/* Allpass */
	double *facda,*facdaa;
	double *bufv,**bufk,**bufa;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	erfbs=instance->erfbs;
	erfbd=instance->erfbd;
	// Modulator 4
	moddata4=(void *)(instance->mod4.bshpdata);
	modfunc4=instance->mod4.modfunc;
	mrfbs=instance->mrfbs;
	mrfbd=instance->mrfbd;
	//
	ampf=instance->ampf;
	//
	vanz=instance->vanz;
	delv=instance->delv;
	delv0=instance->delv0;
	faccv=instance->faccv;
	//
	kanz=instance->kanz;
	delk=instance->delk;
	mdelk=instance->mdelk;
	facbk=instance->facbk;
	fbbk=instance->fbbk;
	//
	mdelv=instance->mdelv;
	dela=instance->dela;
	mdela=instance->mdela;
	facda=instance->facda;
	facdaa=instance->facdaa;
	bufv=instance->bufv;
	bufk=instance->bufk;
	bufa=instance->bufa;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff =effs +effd *modfunc1(curlen,instance->slen,moddata1);dry =1.0-eff;
		erfb=erfbs+erfbd*modfunc2(curlen,instance->slen,moddata2);erdr=1.0-erfb;
		mrfb=mrfbs+mrfbd*modfunc4(curlen,instance->slen,moddata4);mrdr=1.0-mrfb;

		/*
												dr    fb
											 +--*->+<-*-----+
				dr    fb                     |     v        ^
		x ->+---*->+<-*----------------+     | +--------+ +---+
			|      v                   |     | |delay   | |flt|
			|  +--------------------+  |     | +--------+ +---+
			|  |delay               |  |     |        v     ^
			|  +--------------------+  |     |        +---->+----+
			|    v  v  v   v     v     |     |                   |
			|    *  *  *   *     *     |     |  dr    fb         |
			|    +--+--+---+-----+---->+---->+--*->+<-*-----+    +---->+<-
			|                                |     v        ^    |     v
			|                                | +--------+ +---+  |
			|                                | |delay   | |flt|  |
			|                                | +--------+ +---+  |
			|                                |        v     ^    |
			|                                |        +---->+----+
											 :                   :
		*/

		in=(double)*sbuf;

		out1=0.0;												/* early reflections */
		for(k=0;k<vanz;k++) {
			out1+=(faccv[k]*bufv[delv[k]]);							/* sum early reflections */
			delv[k]=(delv[k]+1)%mdelv;
		}
		out1/=(double)vanz;
		bufv[delv0]=(in*erdr+out1*erfb);							/* feedback early reflections	*/
		delv0=(delv0+1)%mdelv;

		out2=0.0;												/* main reflections */
		for(k=0;k<kanz;k++) {
			out=bufk[k][delk[k]];									/* output from comb-filter delay */
			//dout=fbbk[k]+out;										/* too simple lowpass ? */
			dout=fbbk[k]+out*(1.0-facbk[k]);						/* simple lowpass */
			bufk[k][delk[k]]=(out1*mrdr+dout*mrfb);					/* feedback main reflections */
			delk[k]=(delk[k]+1)%mdelk[k];
			fbbk[k]=facbk[k]*dout;
			out2+=/*fc* */out;										/* sum main reflections */
		}
		out2/=(double)kanz;

		/*out3=0.0;*/											/* allpass filtering */
		for(k=0;k<AANZ;k++) {
			out3=(out2*(-facda[k])+bufa[k][dela[k]]*facdaa[k]);		/* output */
			/*out3=(-out2+bufa[k][dela[k]]);*/						/* output */
			bufa[k][dela[k]]=out2+(bufa[k][dela[k]]*facda[k]);		/* input+(bufout*feedback) */
			dela[k]=(dela[k]+1)%mdela[k];
			out2=out3;
		}

		out=((out1*0.1)+(out3*5.0));							/* Early+Main,Diff */
		out=ampf*((double)*(sbuf++)*dry+out*eff);
		CLIPFLT(out);											/* check here for overflow */
		*(dbuf++)=(SAMPLE)out;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->delv0=delv0;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	unsigned char k;
	
	if(instance->bufk) {
		for(k=0;k<instance->kanz;k++) { if(instance->bufk[k]) FreeVec((ULONG *)instance->bufk[k]); }
	}
	for(k=0;k<AANZ;k++) { if(instance->bufa[k]) FreeVec((ULONG *)instance->bufa[k]); }
	if(instance->bufv)	{ FreeVec((ULONG *)instance->bufv);instance->bufv=NULL; }
	if(instance->delv)	{ FreeVec((ULONG *)instance->delv);instance->delv=NULL; }
	if(instance->faccv)	{ FreeVec((ULONG *)instance->faccv);instance->faccv=NULL; }
	if(instance->delk)	{ FreeVec((ULONG *)instance->delk);instance->delk=NULL; }
	if(instance->mdelk)	{ FreeVec((ULONG *)instance->mdelk);instance->mdelk=NULL; }
	if(instance->facbk)	{ FreeVec((ULONG *)instance->facbk);instance->facbk=NULL; }
	if(instance->fbbk)	{ FreeVec((ULONG *)instance->fbbk);instance->fbbk=NULL; }
	if(instance->bufk)	{ FreeVec((ULONG *)instance->bufk);instance->bufk=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
