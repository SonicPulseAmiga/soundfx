/****h* Equalize-FFT-3D/Equalize-FFT-3D.c [4.2] *
*
*  NAME
*    Equalize-FFT-3D.c
*  COPYRIGHT
*    $VER: Equalize-FFT-3D.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Equalize-FFT-3D core file
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
/****u* _operators/Equalize-FFT-3D *
*  Function.eng
*    Morphs between 8 equalizer curves in a cube into a result-curve,
*    which then modifies the amplitude of the frequency components of a sample.
*  Funktion.ger
*    Morpht zwischen 8 Equalizerkurven in einem Würfel zu einer Ergebniskurve,,
*    welche die Lautstärke der Frequenzbestandteile des Samples ändert.
*  Parameter.eng
*    Frequency-Curves (Eqf1..8)
*      Your source equalizer curves. When you click on the PopUp-Symbol a file
*      requester appears to let you choose an equalizer preset. These can be
*      made with the <a href="Equalize-FFT.html">Equalize-FFT</a> operator.
*      You can even select multiple presets at once. This will load multiple curves.
*    X-Axis (<a href="../node03.02.01.html">P1</a>)
*      location of the point on the X-axis
*    Y-Axis (<a href="../node03.02.01.html">P2</a>)
*      location of the point on the Y-axis
*    Z-Axis (<a href="../node03.02.01.html">P3</a>)
*      location of the point on the Z-axis
*    Path
*      This area shows the path of the curve inside the cube. During the
*      calculation a point will wander along the curve from one end to the other.
*      The distance of the point to the corners defines how much of the equalizer
*      shape assigned to that corner effects the result equalizer shape.<br>
*      With the "View" gadget, you can choose from where to look on the cube
*      and with "Prec." you choose how detailed the curve will be drawn.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      what windowfunction to use
*    Bands
*      Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
*    Steps
*      SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
*      longer calculation. Steps can no be bigger than the half of the number of bands.
*  Parameter.ger
*    Frequency-Curves (Eqf1..8)
*      Wenn Sie auf das PopUp-Symbol klicken, erscheint ein Dateirequester, aus
*      dem Sie ein Equalizerpreset auswählen können. Diese können mit dem
*      <a href="Equalize-FFT.html">Equalize-FFT</a> Operator erstellen.
*      Sie könenn sogar mehrere Pesets auswählen. Dann werden mehrere Kurven geladen.
*    X-Axis (<a href="../node03.02.01.html">P1</a>)
*      Position des Punktes auf der X-Achse
*    Y-Axis (<a href="../node03.02.01.html">P2</a>)
*      Position des Punktes auf der Y-Achse
*    Z-Axis (<a href="../node03.02.01.html">P3</a>)
*      Position des Punktes auf der Z-Achse
*    Path
*      In diesem Feld wird der Pfad, als Kurve im Würfel, dargestellt. Während
*      der Berechnung wird ein Punkt entlang der Kurve vom Begin bis zum Ende
*      wandern. Die Entfernung dies Punktes zu den Ecken bestimmt wie stark die
*      Equalizerkurven die den Ecken zugeordnet wurden in die Ergebniskurve
*      einfließen.<br>
*      Mit "View" läßt sich der Ansichtspunkt festlegen und mit "Prec." die
*      Genauigkeit mit der die Kurve gezeichnet wird.
*    Window (<a href="../node03.02.03.html">W1</a>)
*      welche Fensterfunktion verwendet wird.
*    Bands
*      Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
*      die Frequenzauflösung ist dann aber auch nicht so fein.
*    Steps
*      Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
*      werden, desto genauer das Ergebnis und desto höher auch die Rechenzeit. Steps darf max. halb
*      so groß wie die Nummer der Bänder sein.
*  Notes.eng
*    The results of the operator are very unpredictable. This means you are invited
*    to experiment (e.g. try a large noise sample and one of the included presets).
*    The FX is generally well suited to produce e.g. Sci-Fi sounds.<br>
*    This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
*    for its calculations.
*  Hinweise.ger
*    Die Resultate dieses Operators sind sehr unvorhersehbar. Das bedeutet das sie
*    ruhig etwas herumexperimentieren sollten (z.B. nehmen sie doch mal ein langes
*    Rausch-Sample und eines der mitgelieferten Presets).
*    The Effekt ist recht gut für z.B. Sci-Fi Sounds geeignet.<br>
*    Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
*******
*/

#define EQUALIZE_FFT_3D_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Equalize-FFT-3D.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);	
}

int activate(SFXCoreInstance *instance) {
	ULONG i,size;
	double fact;

	instance->mb=1<<(instance->bands+2);								// the number of frequency bands we want to get
	instance->mw=instance->mb<<1;										// the width of the databuffer is twice as large as the number of bands
	instance->mv=instance->mw-(instance->mw>>1);						// put how many values into databuffer, zero the rest
	instance->ms=1<<instance->steps;									// do a analysys every <ms> steps
	if(instance->ms>=(instance->mb>>1)) instance->ms=instance->mb>>1;	// steps must be at least mb/2
	size=(instance->mw+2)*sizeof(double);

	if(!(instance->flbuf_r	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->flbuf_i	=(double *)AllocVec(size,MEMF_ANY))) return(FALSE);
	if(!(instance->win_fkt1	=NewWindowFkt(instance->mv,instance->wfkt1.win,&instance->wfkt1.par))) return(FALSE);
	if(!(instance->win_fkt2	=NewWindowFkt(instance->mb,WIN_BARTLETT,NULL))) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->mb)) return(FALSE);
	if(!					 NewFFTTabs(&instance->bitrevtab,&instance->sintab1,&instance->sintab2,&instance->costab,instance->mb)) return(FALSE);
	if(!(instance->irbuf	=NewRingBuf(instance->mw,&instance->irbufbase,&instance->irbufmask,&instance->irbufsize))) return(FALSE);
	if(!(instance->orbuf	=NewRingBuf(instance->mw,&instance->orbufbase,&instance->orbufmask,&instance->orbufsize))) return(FALSE);

	fact=(4.0*32767.0)/((double)instance->mb/(double)instance->ms);		// make triangle quieter depending on steps
	for(i=0;i<instance->mb;i++) instance->win_fkt2[i]*=fact;
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	AssignBShape(&instance->mod3);
	memset(instance->irbuf,0,(instance->irbufsize<<1));
	instance->irbufw=instance->irbufbase;
	memset(instance->orbuf,0,(instance->orbufsize<<1));
	instance->orbufw=instance->orbufbase;
	instance->pos=(instance->ms-1);			// weil curlen erst später erhöht wird
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	unsigned short o;
	double axis_xs,axis_x1,axis_x2,axis_xd;
	double axis_ys,axis_y1,axis_y2,axis_yd;
	double axis_zs,axis_z1,axis_z2,axis_zd;
	BShapeFuncPtr modfunc1,modfunc2,modfunc3;
	void *moddata1,*moddata2,*moddata3;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *irbufbase,*orbufbase;
	ULONG irbufmask,orbufmask;
	SAMPLE *irbufr,*irbufw,*orbufr,*orbufw;
	UWORD mb,mw,ms,mv;
	double *flbuf_r,*flbuf_i,*flp1_r,*flp1_i,*flp2_r,*flp2_i;
	double *win_fkt1,*win_fkt2,*winfp;
	ULONG pos;
	UWORD ix;
	double frq,frq2,fact,ant;
	double eqf[514],*eqfp;
	double val1,f1,f2,f3,f4,f5,f6,f7,f8;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
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
	//
	irbufbase=instance->irbufbase;
	irbufmask=instance->irbufmask;
	irbufw=instance->irbufw;
	orbufbase=instance->orbufbase;
	orbufmask=instance->orbufmask;
	orbufw=instance->orbufw;
	mb=instance->mb;
	mw=instance->mw;
	ms=instance->ms;
	mv=instance->mv;
	flbuf_r=instance->flbuf_r;
	flbuf_i=instance->flbuf_i;
	win_fkt1=instance->win_fkt1;
	win_fkt2=instance->win_fkt2;
	pos=instance->pos;
	fact=64.0/(double)instance->mb;							// for interpol frq.coeffs

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		axis_x1=1.0-(axis_x2=(axis_xs+axis_xd*modfunc1(curlen,instance->slen,moddata1)));
		axis_y1=1.0-(axis_y2=(axis_ys+axis_yd*modfunc2(curlen,instance->slen,moddata2)));
		axis_z1=1.0-(axis_z2=(axis_zs+axis_zd*modfunc3(curlen,instance->slen,moddata3)));

		f1=axis_x1*axis_y1*axis_z1;	/* Stecke für Src1 */
		f2=axis_x2*axis_y1*axis_z1;	/* Stecke für Src2 */
		f3=axis_x2*axis_y1*axis_z2;	/* Stecke für Src3 */
		f4=axis_x1*axis_y1*axis_z2;	/* Stecke für Src4 */
		f5=axis_x1*axis_y2*axis_z1;	/* Stecke für Src5 */
		f6=axis_x2*axis_y2*axis_z1;	/* Stecke für Src6 */
		f7=axis_x2*axis_y2*axis_z2;	/* Stecke für Src7 */
		f8=axis_x1*axis_y2*axis_z2;	/* Stecke für Src8 */
		for(o=0;o<mb;o++) {
			ix=(UWORD)(o*fact);ant=((double)o*fact)-ix;
			frq=instance->eqf1[ix]*f1+instance->eqf2[ix]*f2+instance->eqf3[ix]*f3+instance->eqf4[ix]*f4+instance->eqf5[ix]*f5+instance->eqf6[ix]*f6+instance->eqf7[ix]*f7+instance->eqf8[ix]*f8;
			if(ant!=0.0 && ix<mb) {
				frq2=instance->eqf1[ix+1]*f1+instance->eqf2[ix+1]*f2+instance->eqf3[ix+1]*f3+instance->eqf4[ix+1]*f4+instance->eqf5[ix+1]*f5+instance->eqf6[ix+1]*f6+instance->eqf7[ix+1]*f7+instance->eqf8[ix+1]*f8;
				eqf[o]=ant*frq2+(1.0-ant)*frq;
			}
			else eqf[o]=frq;
		}

		*irbufw=*(sbuf++);
		irbufw=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw+1)&irbufmask));
		if(curlen==pos) {								/* we've read the next m2 samples */
			pos+=ms;
			flp1_r=flbuf_r;flp1_i=flbuf_i;winfp=win_fkt1;
			irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufw-mv)&irbufmask));
			for(o=0;o<mv;o++) {	/* read in values */
				*(flp1_r++)=(*(winfp++))*((double)(*irbufr)/32768.0);*(flp1_i++)=0.0;
				irbufr=(SAMPLE *)((ULONG)irbufbase|((ULONG)(irbufr+1)&irbufmask));
			}
			for(o=mv;o<mw;o++) {/* set rest to zero */
				*(flp1_r++)=*(flp1_i++)=0.0;
			}

			Transform(flbuf_r,flbuf_i,instance->bitrevtab,instance->sintab1,instance->costab,-1,mb);	/* -> frequency domain */

			flbuf_r[0]*=eqf[0];											/* 0. Band */
			flbuf_i[0]*=eqf[0];
			eqfp=&eqf[1];												/* restliche Bänder */
			flp1_r=&flbuf_r[   1];flp1_i=&flbuf_i[   1];
			flp2_r=&flbuf_r[mw-1];flp2_i=&flbuf_i[mw-1];
			for(o=0;o<mb;o++) {
				*(flp2_r--)=(*(flp1_r++)*=*eqfp);						/* mult by real fc */
				*(flp2_i--)=(*(flp1_i++)*=*(eqfp++));
			}

			Transform(flbuf_r,flbuf_i,instance->bitrevtab,instance->sintab2,instance->costab,1,mb);	/* -> time domain */

			flp1_r=flbuf_r;winfp=win_fkt2;
			orbufr=orbufw;			/* after the loop we will have m2 values ready from here */
			for(o=0;o<mb;o++) {		/* Werte zurückschreiben */
				val1=(LONG)*orbufw+(LONG)((*(winfp++))*(*(flp1_r++)));
				CLIPINT(val1);
				*orbufw=(SAMPLE)val1;
				orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufw+1)&orbufmask));
			}
			orbufw=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+ms)&orbufmask));	/* advance writebuffer from start+ms */
			for(o=0;o<ms;o++) {	/* copy to outputblock and clear rbuf */
				*(dbuf++)=*orbufr;
				*orbufr=0;
				orbufr=(SAMPLE *)((ULONG)orbufbase|((ULONG)(orbufr+1)&orbufmask));
			}
		}

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->irbufw=irbufw;
	instance->orbufw=orbufw;
	instance->pos=pos;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->flbuf_r) 	{ FreeVec((ULONG *)instance->flbuf_r);instance->flbuf_r=NULL; }
	if(instance->flbuf_i)	{ FreeVec((ULONG *)instance->flbuf_i);instance->flbuf_i=NULL; }
							  DelFFTTabs(instance->bitrevtab,instance->sintab1,instance->sintab2,instance->costab);
							  instance->bitrevtab=NULL;instance->sintab1=instance->sintab2=instance->costab=NULL;
	if(instance->win_fkt1)	{ DelWindowFkt(instance->win_fkt1);instance->win_fkt1=NULL; }
	if(instance->win_fkt2)	{ DelWindowFkt(instance->win_fkt2);instance->win_fkt2=NULL; }
	if(instance->irbuf)		{ DelRingBuf(instance->irbuf);instance->irbuf=NULL; }
	if(instance->orbuf)		{ DelRingBuf(instance->orbuf);instance->orbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
