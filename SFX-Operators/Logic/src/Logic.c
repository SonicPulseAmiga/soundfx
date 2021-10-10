/****h* Logic/Logic.c [4.2] *
*
*  NAME
*    Logic.c
*  COPYRIGHT
*    $VER: Logic.c 4.2 (11.03.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    Logic core file
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
/****u* _operators/Logic *
*  Function.eng
*    Does a logical operation to the sampledata whith the choosen function.
*  Funktion.ger
*    Unterzieht die Sampledaten einer logischen Verknüpfung mit der ausgewählten Funktion.
*  Parameter.eng
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      how much the operator effects the outcome
*    Logic Operant (<a href="../node03.02.01.html">P2</a>)
*      value what is to be used for the operation
*    Type
*      What function should be used
*  Parameter.ger
*    Effect (<a href="../node03.02.01.html">P1</a>)
*      wie stark der Effekt in das Ergebnis einfließt
*    Logic Operant (<a href="../node03.02.01.html">P2</a>)
*      Wert mit dem operiert werden soll
*    Type
*      Funktion die verwendet werden soll
*  Notes.eng
*    For people who like to experiment. Effect parameter should be kept low.<br>
*    One can also use this operator to "encode" or encrypt samples. For this
*    you need a sample TO encrypt and a sample THAT encrypts. The later is the
*    key. Set the LogicOperand parameter to 32767 lv <-> -32768 lv and activate
*    the blendshape "User/Normal". Choose your key sample for the modulator.
*    The Effect parameter is set to 100 %. Choose the function "Xor". After
*    the operation, if you'll listen to the new data, there won't be much in
*    terms of listenable sound. Repeat the same operation and you'll get your
*    sample back the way it was.
*  Hinweise.ger
*    Etwas für Leute die gerne mal experimentieren. Der Effektanteil sollte niedrig
*    gehalten werden.<br>
*    Man kann diesen Operator auch zur Verschlüsselung von Samples nutzen. Dazu
*    benötigen Sie ein Sample, welches Sie verschlüsseln wollen und ein Sample,
*    welches als Schlüssel dient. Stellen Sie für den Parameter LogicOperant die
*    Werte 32767 lv <-> -32768 lv ein und aktivieren das BlendShape "User/Normal".
*    Als Modulator wählen Sie Ihr Schlüssel-Sample. Den Effekt-Anteil sellen Sie
*    auf 100 %. Als Funktion stellen Sie "Xor" ein. Nach der Operation ist von dem
*    Ausgangssample nicht viel übrig, nur noch Störgeräusche. Wenn Sie die Operation
*    jedoch mit gleichen Parametern wiederholen, ist das Sample wieder im Originalzustand.
*******
*/

#define LOGIC_C

//-- includes -----------------------------------------------------------------

#include "../../cfg.h"

#include <libraries/sfxsupport.h> 
#include <proto/sfxsupport.h>

//-- Local
#include "project.h"
#include "Logic.h"
#include <logic2state.h>

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
}

void process(SFXCoreInstance *instance,unsigned long samples) {
	unsigned long i,curlen;
	double effs,effd,eff,dry;
	double logs,logd;
	BShapeFuncPtr modfunc1,modfunc2;
	void *moddata1,*moddata2;
	SAMPLE *dbuf,*sbuf;
	WORD logv;
	UBYTE *ltab;

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
	logs=instance->logs;
	logd=instance->logd;
	
	ltab=logic2_tab[instance->typ];

	for(i=0;((i<samples) && (instance->curlen<instance->slen));i++,curlen++,instance->curlen++) {
		//if(fabs(*sbuf)>instance->maxi) instance->maxi=fabs(*sbuf);
		
		eff =effs+effd*modfunc1(curlen,instance->slen,moddata1);dry=1.0-eff;
		logv=logs+logd*modfunc2(curlen,instance->slen,moddata2);

		*(dbuf++)=(SAMPLE)((double)*sbuf*dry+(double)(logic2_w(*(sbuf++),logv,ltab))*eff);

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
