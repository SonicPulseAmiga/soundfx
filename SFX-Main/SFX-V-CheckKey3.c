/****h* SoundFX/SFX-CheckKey3.c_ [4.1] *
*
*  NAME
*    SFX-CheckKey3.c_
*  COPYRIGHT
*    $VER: SFX-CheckKey3.c_ 4.1 (23.02.01) © by Stefan Kost 1998-2001
*  FUNCTION
*    Keyfilechecking - definitions
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    13.Aug.1998
*  MODIFICATION HISTORY
*    23.Feb.2001    V 4.1   most recent version
*    13.Aug.1998    V 3.70  most recent version
*    13.Aug.1998    V 3.70  initial version
*  NOTES
*
*******
*/

#define SFX_CheckKey3_C

//-- includes -----------------------------------------------------------------

#include "SFX-Includes.h"

//-- prototypes ---------------------------------------------------------------

UBYTE CheckKeyFile3(void);

//-- definitions --------------------------------------------------------------

// returns 0 on success and 0xFF on failure

UBYTE CheckKeyFile3(void)
{
    UBYTE ret=0;
#ifdef CHECK_KEY
    register UBYTE i,j;
    UBYTE mi,ma;
    ULONG avg,ix;

	INTRO;

    if(SFXKeyBase) {    // only does the check for new keys
//      MSG1("rv=%3d",RunTime.keyrv);
        for(i=0;i<10;i++) {                                 // calculate rowdifferences
            mi=0xFF;ma=0;avg=0;
            for(j=0;j<39;j++) {
                ix=i*40+j;
                avg=avg+RunTime.keypd[ix];
                if(RunTime.keypd[ix]<mi) mi=RunTime.keypd[ix];
                if(RunTime.keypd[ix]>ma) ma=RunTime.keypd[ix];
            }

//          MSG4("%2d  %3d %3d %3d",i,RunTime.keyrowd[   i],RunTime.keyrowd[10+i],RunTime.keyrowd[20+i]);
//          MSG1("[%s]",&RunTime.keypd[i*40]);
//          MSG2("ma=%d mi=%d",ma,mi);
//          MSG1("avg=%d",avg);

            if((RunTime.keyrowd[   i]^RunTime.keyrv)!=(ma-mi)) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : rowd1 failed"); }
            if((RunTime.keyrowd[10+i])!=(avg&0xFF)) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : rowd2 failed"); }
            if((RunTime.keyrowd[20+i])!=(avg/40  )) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : rowd3 failed"); }
        }
        for(i=0;i<39;i++) {                                 // calculate coldifferences
            mi=0xFF;ma=0;avg=0;
            for(j=0;j<10;j++) {
                ix=j*40+i;
                avg=avg+RunTime.keypd[ix];
                if(RunTime.keypd[ix]<mi) mi=RunTime.keypd[ix];
                if(RunTime.keypd[ix]>ma) ma=RunTime.keypd[ix];
            }
            if((RunTime.keycold[   i]^RunTime.keyrv)!=(ma-mi)) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : cold1 failed"); }
            if((RunTime.keycold[40+i])!=(avg&0xFF)) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : cold2 failed"); }
            if((RunTime.keycold[80+i])!=(avg/10  )) { ret=0xFF;RunTime.keyerr|=KEYERR_ILLEGAL_KEY;ERR("CheckKey3 : cold3 failed"); }
        }
    }
#endif

	OUTRO;

    return(ret);
}

//-- eof ----------------------------------------------------------------------
