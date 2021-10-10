/****h* Swap/Swap.c [4.2] *
*
*  NAME
*    Swap.c
*  COPYRIGHT
*    $VER: Swap.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Swap core file
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
/****u* _operators/Swap *
*  Function.eng
*    Swaps sample data repeatedly within a certain range
*  Funktion.ger
*    Vertauscht wiederholt Sampledaten innerhalb eines Bereiches
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Swap Range (<a href="../node03.02.01.html">P2</a>)
*      Range, inside of which samplevalues are to be swapped
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Swap Range (<a href="../node03.02.01.html">P2</a>)
*      Bereich, in dem vertauscht werden soll
*  Notes.eng
*    The range shouldn't be too big as the sample might sound "retarded" or like
*    severely stupid mamals coughed into the sample.<br>
*    The sample will sound sharper, with a saw characteristic, because many
*    trebbles have been added.
*  Hinweise.ger
*    Der Bereich sollte nicht zu groß gewählt werden, da das Sample sonst einfach
*    verstümmelt klingt.<br>
*    Das Sample klingt nach der Bearbeitung durch diesen Operator schärfer,
*    sägender, da viele Obertöne hinzugefügt wurden.
*******
*/

#define SWAP_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Swap.h"

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
	if(!(instance->rbuf=NewRingBuf(instance->mrng,&instance->rbufbase,&instance->rbufmask,&instance->rbufsize))) return(FALSE);
	return(TRUE);
}

void activate_ch(SFXCoreInstance *instance,unsigned char ch) {
	AssignBShape(&instance->mod1);
	AssignBShape(&instance->mod2);
	memset(instance->rbuf,0,(instance->rbufsize<<1));
	instance->rbufw=instance->rbufbase;
	instance->rpos=0;
	instance->spos=instance->dpos=0;
	instance->off=0;
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	ULONG  rngs,rngd,rng;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	SAMPLE *rbufw,*rbufr1,*rbufr2;
	SAMPLE *rbufbase;
	ULONG rbufmask;
	ULONG rpos,spos,dpos;

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
	rngs=instance->rngs;
	rngd=instance->rngd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	spos=instance->spos;
	dpos=instance->dpos;
	rpos=instance->rpos;

	while((spos>0) && (dpos<samples) && (instance->curlen<instance->slen)) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);

		eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		rng=rngs+rngd*modfunc2(curlen,instance->slen,moddata2);

		if(!instance->off) {
			*rbufw=*(sbuf++);rpos++;
			spos--;curlen++;instance->curlen++;
			rbufw=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw+1)&rbufmask));
			i=0;
		}
		else {			// continue ?
			i=instance->off;
			rng=instance->rng;
			instance->off=0;
		}
		if(rpos>=rng) {						/* we've got enough to reverse */
			rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-(rpos-rng)-i)&rbufmask));
			rbufr2=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-rpos+i)&rbufmask));
			for(;((i<(ULONG)rng) && (dpos<samples));i++) {	/* take care that we don't overrun buffer boundaries */
				*(dbuf++)=(WORD)((double)*rbufr2*dry+(double)*rbufr1*eff);dpos++;
				rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr1-1)&rbufmask));
				rbufr2=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr2+1)&rbufmask));
			}
			if(dpos==samples) {
				instance->off=i;
				instance->rng=rng;
			}
			else {
				rpos-=rng;
			}
		}

		//if(fabs(res)>instance->maxo) instance->maxo=fabs(res);
	}
	instance->rbufw=rbufw;
	instance->sbuf=sbuf;
	instance->dbuf=dbuf;
	instance->spos=spos;
	instance->dpos=dpos;
	instance->rpos=rpos;
}

void process_rest(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	ULONG  rngs,rngd,rng;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf;
	SAMPLE *rbufw,*rbufr1,*rbufr2;
	SAMPLE *rbufbase;
	ULONG rbufmask;
	ULONG rpos,dpos;

	curlen=instance->curlen;

	// Source, Dest.
	dbuf=instance->dbuf;
	// Modulator 1
	moddata1=(void *)(instance->mod1.bshpdata);
	modfunc1=instance->mod1.modfunc;
	effs=instance->effs;
	effd=instance->effd;
	// Modulator 2
	moddata2=(void *)(instance->mod2.bshpdata);
	modfunc2=instance->mod2.modfunc;
	rngs=instance->rngs;
	rngd=instance->rngd;
	//
	rbufbase=instance->rbufbase;
	rbufmask=instance->rbufmask;
	rbufw=instance->rbufw;
	dpos=instance->dpos;
	rpos=instance->rpos;

	eff=effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;

	i=instance->off;
	rng=instance->rng;
	instance->off=0;

	rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-(rpos-rng)-i)&rbufmask));
	rbufr2=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufw-rpos+i)&rbufmask));
	for(;((i<(ULONG)rng) && (dpos<samples));i++) {	/* take care that we don't overrun buffer boundaries */
		*(dbuf++)=(WORD)((double)*rbufr2*dry+(double)*rbufr1*eff);dpos++;
		rbufr1=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr1-1)&rbufmask));
		rbufr2=(SAMPLE *)((ULONG)rbufbase|((ULONG)(rbufr2+1)&rbufmask));
	}
	if(dpos==samples) {
		instance->off=i;
	}
	else {
		instance->off=rng;
	}

	instance->rbufw=rbufw;
	instance->dbuf=dbuf;
	instance->dpos=dpos;
	instance->rpos=rpos;
}

void deactivate_ch(SFXCoreInstance *instance,unsigned char ch) {
}

void deactivate(SFXCoreInstance *instance) {
	if(instance->rbuf)	{ DelRingBuf(instance->rbuf);instance->rbuf=NULL; }
}

void destroy(SFXCoreInstance *instance) {
}

void done(void) {
}
//-- eof ----------------------------------------------------------------------
