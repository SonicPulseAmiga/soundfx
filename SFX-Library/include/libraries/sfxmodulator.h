/****h* sfxsupport.library/sfxmodulator.h [4.3] *
*
*  NAME
*    sfxmodulator.h
*  COPYRIGHT
*    $VER: sfxmodulator.h 4.3 (08.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    modulator related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Aug.2002
*  MODIFICATION HISTORY
*    08.Apr.2004	V 4.3	most recent version
*    19.Aug.2002	V 4.2	most recent version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXMODULATOR_H
#define  LIBRARIES_SFXMODULATOR_H

//-------------------------------------------------------------------------------------------------

typedef double ASM (*BShapeFuncPtr)(REG(d0,ULONG pos),REG(d1,ULONG max),REG(a0,APTR md));

//-- Modulator ------------------------------------------------------------------------------------

typedef struct {
	Param           vals,vale;                              // Start / Endwert
	UWORD           bshape;                                 // Blendshape : None,Curve,Cycle,Vector,User,..
	BShapeFuncPtr   modfunc;                                // BShapeFunction
	char            desc[64];                               // Beschreibung
	ULONG           slen,srat;                              // für "Cycle"
	APTR            bshpdata;                               // Blenshapedata
} Modulator;

#define MOD_IDCT    7
#define MOD_IXCT    6

#define MODINIT_FIRST 1

#define BSHP_NONE       0
#define BSHP_CURVE      1
#define BSHP_CYCLE      2
//#define BSHP_VARCYCLE 3
#define BSHP_VECTOR     3
#define BSHP_USER       4

#define BSHP_ANZ        5

// for PrefsData
#define PDS_MODULATOR       sizeof(Modulator)

//-- BlendShape typen

typedef struct {
	double  exp;                    // Ausbeulung der Kurve
} ModCurve;

typedef struct {
	UWORD   oszi;                   // welcher Oszilator, siehe unten
	Param   frq;                    // bestimmt die frequenz (ev. indirekt)
	Param   pha;                    // startphase
	double  angle,anglest,hold;     // winkel,winkelschritt oder sample & hold;
	double  newrnd,diff;			// for smoothed sample & hold
} ModCycle;

#define MC_OSZI_SAW     0
#define MC_OSZI_SIN     1
#define MC_OSZI_SQR     2
#define MC_OSZI_TRI     3
#define MC_OSZI_RND     4
#define MC_OSZI_SRND    5
#define MC_OSZI_ANZ     6

#define MV_MAX_VECTORS  20

typedef struct {
	UBYTE   anz;                    // Anzahl der Vectorpunkte,max=20;
	UBYTE   ix;                     // akt. Punkt
	float   dfc;                    // Differenzenfactor im aktuellen Bereich
	float   lpos,llev;              // letzte Werte
	float   pos[MV_MAX_VECTORS];    // Positionen von 0.0...1.0
	float   lev[MV_MAX_VECTORS];    // Amplitude von 0.0...1.0
	BOOL    lockfirst;              // is first point fixed
	BOOL    locklast;               // is last point fixed
} ModVector;

typedef struct {
	UBYTE   type;                   // wie die sourcedaten interpretiert werden sollen, siehe unten
	UBYTE   bmode;                  // mode : single,repeat,stretch
	UBYTE   env;                    // Hüllkurventyp, siehe unten
	UBYTE   channel;                // Channel in modbuf
	ULONG   modid;                  // Modulationsid.
	SInfo   *modbuf;                // Modulationspuffer
	APTR    *envdata;               // für bmode=AMPENV oder FRQENV
} ModUser;

#define MU_TYPE_NORMAL  0           // User : -1.0 -> 0.0 , 1.0 -> 1.0 
#define MU_TYPE_ABSOLUT 1           // User :  0.0 -> 0.0 , 1.0 & -1.0 -> 1.0
#define MU_TYPE_AMPENV  2           // User : Volumenhüllkurve
#define MU_TYPE_FRQENV  3           // User : Frequenzhüllkurve
#define MU_TYPE_ANZ     4

#define MU_BMOD_SINGLE  0           // Sample einmal durchlaufen, dann 0.0
#define MU_BMOD_REPEAT  1           // Sample unendlich oft durchlaufen
#define MU_BMOD_STRETCH 2           // Sample auf Sourcelänge strecken
#define MU_BMOD_ANZ     3

#define MU_AENV_MAXDECAY    0

typedef struct {
	double maxval;
	double fc1,fc2;
} AmpEnvMaxDecay;

#define MU_PENV_PEAKSEARCH  0
#define MU_PENV_ENERGY      1

typedef struct {
	ULONG lastpos,curlen;
	WORD lastval;
	UBYTE maxs,mins;
} FrqEnvPeakSearch;

typedef struct {
	ULONG lastpos,curlen;
	LONG energy;
	UBYTE mins;
} FrqEnvEnergy;

#endif   /* LIBRARIES_SFXMODULATOR_H */

//-- eof ----------------------------------------------------------------------
