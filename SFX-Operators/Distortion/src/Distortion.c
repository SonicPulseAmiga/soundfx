/****h* Distortion/Distortion.c [4.2] *
*
*  NAME
*    Distortion.c
*  COPYRIGHT
*    $VER: Distortion.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Distortion core file
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
/****u* _operators/Distortion *
*  Function.eng
*    Creates distortion and fuzz effects.
*  Funktion.ger
*    Erzeugt Distortion und Fuzz Effekte.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Distortion Shape (<a href="../node03.02.01.html">P2</a>)
*      this shape determines the kind and the amount of distortion
*    Map
*      the shape can be mapped in various ways :
*        full range : as it is [-max to max]
*        mirrored : copied and rotated around the origin [-max to 0]=-[0 to max], which yields same shapes for positive and negative sample-values
*    Wrap
*      Choose how to handle clipping. The modi below are available :
*        NoClip: don't care
*        Clip : overdriven values are clipped
*        Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.
*        Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Distortion Shape (<a href="../node03.02.01.html">P2</a>)
*      diese Form bestimmt die Art und die Stärke der Verzerrungen.
*    Map
*      die Kurve kann auf verschiedene Arten übertragen werden :
*        full range : so wie sie ist [-max to max]
*        mirrored : oder auch kopiert und um ihren Ursprung gedreht werden [-max to 0]=-[0 to max], was die gleichen Kurven für positive wie auch negative Samplewerte ergibt
*    Wrap
*      Gibt an, wie eine mögliche Übersteuerung des Signals behandelt werden soll.
*      Hierbei gibt es 4 Modi :
*        NoClip: es wird nicht auf Übersteuerte Werte gestestet.
*        Clip : die übersteuerten Werte werden auf Maximum bzw. Minimum gesetzt
*        Wrap1 : der übersteuerte Anteil wird an der anderen Seite wieder hereingeschoben, und zwar solange, bis er komplett im Normalbereich ist.
*        Wrap2 : der übersteuerte Anteil wird solange an der Ober- und Unterkante umgeklappt, bis er komplett im Normalbereich ist.
*  Notes.eng
*    The shape acts as a kind of lookup table. If the shape would be a straight
*    line (from bottom left to top right), nothing would change in the sound.
*    But the more different the shape looks like, the more distorted the sound will be.
*  Hinweise.ger
*    Die Kurve dient als eine Art Übersetzungstabelle. Wenn die Kurve eine gerade
*    Linie (von links unten nach rechts oben) ist, würde sich nichts am Klang ändern.
*    Je mehr die Kurve jedoch davon abweicht, desto verzerrter wird der Sound klingen.
*
*******
*/

#define DISTORTION_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h>
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Distortion.h"

//-- globals ------------------------------------------------------------------

//-- definitions --------------------------------------------------------------

int init(void) {
	return(TRUE);
}

int instantiate(SFXCoreInstance *instance) {
	return(TRUE);
}

int activate(SFXCoreInstance *instance) {
	unsigned long i;
	BShapeFuncPtr modfunc2;
	void *moddata2;
	SAMPLE *trans;
	LONG diss,disd,curval;

	// Modulator 2
	AssignBShape(&instance->mod2);
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	diss=instance->diss;
	disd=instance->disd;

	if(!(instance->trans=(SAMPLE *)AllocVec((SMP_MAX+1)*sizeof(SAMPLE),MEMF_PUBLIC))) return(FALSE);
	trans=instance->trans;
	switch(instance->map) {
		case MAP_FULLRANGE:
			instance->mod2.slen=SMP_MAX;
			for(i=0;i<SMP_MAX;i++) {
				curval=(SAMPLE)(diss+(double)disd*modfunc2(i,SMP_MAX,moddata2));
				if		(curval>SMP_MAXP) curval=SMP_MAXP;
				else if	(curval<SMP_MAXN) curval=SMP_MAXN;
				trans[i]=(SAMPLE)curval;
			}
			break;
		case MAP_MIRRORED:
			instance->mod2.slen=(SMP_MAXP+1);
			for(i=0;i<=SMP_MAXP;i++) {
				curval=(SAMPLE)(diss+(double)disd*modfunc2(i,(SMP_MAXP+1),moddata2));
				if		(curval>SMP_MAXP) curval=SMP_MAXP;
				else if	(curval<SMP_MAXN) curval=SMP_MAXN;
				trans[(SMP_MAXP+1)+i]=(SAMPLE)curval;
				curval=-curval;
				if		(curval>SMP_MAXP) curval=SMP_MAXP;
				else if	(curval<SMP_MAXN) curval=SMP_MAXN;
				trans[(SMP_MAXP+1)-i]=(SAMPLE)curval;
			}
			break;
		default:
			//DEBUG
			printf(__FILE__"::activate(0x%08lx) !!! undefined mapping !!!\n",(unsigned long)instance);
			//DEBUG
			break;
	}
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
}

double INLINE gettrans(SAMPLE *trans,UWORD index) {
	trans=(SAMPLE *)(((ULONG)trans)+Samples2Bytes(index));
	return((double)*trans);
}

void process_noclip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf,*trans;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	//
	trans=instance->trans;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		// this causes faulty code, when using optimize-global and math=standard
		//*(dbuf++)=(SAMPLE)(((double)*sbuf)*dry+((double)trans[(ULONG)(*(sbuf++)-SMP_MAXN)])*eff);

		// this still causes faulty code when using optimize-global, it is the array-access
		// fxval=(double)trans[0]; works, but fxval=(double)trans[index]; works not ??
		//*(dbuf++)=(SAMPLE)(((double)*sbuf)*dry+((double)trans[(ULONG)(((LONG)*(sbuf++))-SMP_MAXN)])*eff);

		*(dbuf++)=dry*((double)*sbuf)+eff*gettrans(trans,(UWORD)(((LONG)*(sbuf++))-SMP_MAXN));

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_clip(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf,*trans;
	LONG curval;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	trans=instance->trans;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		//curval=(LONG)((double)*sbuf*dry+((double)trans[(UWORD)(((LONG)*(sbuf++))-SMP_MAXN)])*eff);
		curval=(LONG)(dry*((double)*sbuf)+eff*gettrans(trans,(UWORD)(((LONG)*(sbuf++))-SMP_MAXN)));
		if		(curval>SMP_MAXP) curval=SMP_MAXP;
		else if	(curval<SMP_MAXN) curval=SMP_MAXN;
		*(dbuf++)=(SAMPLE)curval;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap1(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf,*trans;
	LONG curval;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	trans=instance->trans;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		//curval=(LONG)((double)*sbuf*dry+((double)trans[(ULONG)(((LONG)*(sbuf++))-SMP_MAXN)])*eff);
		curval=(LONG)(dry*((double)*sbuf)+eff*gettrans(trans,(UWORD)(((LONG)*(sbuf++))-SMP_MAXN)));
		if		(curval>SMP_MAXP) curval=((curval-SMP_MAXN)&SMP_MAXMASK)+SMP_MAXN;
		else if	(curval<SMP_MAXN) curval=((curval-SMP_MAXP)&SMP_MAXMASK)+SMP_MAXP;
		*(dbuf++)=(SAMPLE)curval;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void process_wrap2(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	BShapeFuncPtr modfunc1;
	void *moddata1;
	SAMPLE *dbuf,*sbuf,*trans;
	LONG curval;

	curlen=instance->curlen;

	// Source, Dest.
	sbuf=instance->sbuf;
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;

	trans=instance->trans;

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

		//curval=(LONG)((double)*sbuf*dry+((double)trans[(ULONG)(((LONG)*(sbuf++))-SMP_MAXN)])*eff);
		curval=(LONG)(dry*((double)*sbuf)+eff*gettrans(trans,(UWORD)(((LONG)*(sbuf++))-SMP_MAXN)));
		while(curval>SMP_MAXP || curval<SMP_MAXN) {
			if		(curval>SMP_MAXP) curval=SMP_MAXP-(curval-SMP_MAXP);
			else if	(curval<SMP_MAXN) curval=SMP_MAXN-(curval-SMP_MAXN);
		}
		*(dbuf++)=(SAMPLE)curval;

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->trans)	FreeVec(instance->trans);
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
