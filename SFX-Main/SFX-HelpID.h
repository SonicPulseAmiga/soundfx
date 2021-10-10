/****h* SoundFX/SFX-HelpID.h [4.2] *
*
*  NAME
*    SFX-HelpID.h
*  COPYRIGHT
*    $VER: SFX-HelpID.h 4.2 (15.04.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    help id (built in english)
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    15.Apr.2003
*  MODIFICATION HISTORY
*    15.Apr.2003	V 4.2	most recent version
*    15.Apr.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef SFX_HELPID_H
#define SFX_HELPID_H

//-- Main Help ID's -------------------------------------------------------------------------------

#define HELPID_MAIN         0
#define HELPID_INDEX        (HELPID_MAIN+1)
#define HELPID_OPWIN        (HELPID_INDEX+1)
#define HELPID_LOWIN        (HELPID_OPWIN+1)
#define HELPID_SAWIN        (HELPID_LOWIN+1)
#define HELPID_PLWIN        (HELPID_SAWIN+1)
#define HELPID_RXOPWIN      (HELPID_PLWIN+1)
#define HELPID_BUFWIN       (HELPID_RXOPWIN+1)
#define HELPID_EDWIN        (HELPID_BUFWIN+1)
#define HELPID_ZOOMWIN      (HELPID_EDWIN+1)
#define HELPID_RNGWIN       (HELPID_ZOOMWIN+1)
#define HELPID_SMPWIN       (HELPID_RNGWIN+1)
#define HELPID_OPTWIN       (HELPID_SMPWIN+1)
#define HELPID_PERIWIN      (HELPID_OPTWIN+1)
#define HELPID_SRCSELWIN    (HELPID_PERIWIN+1)
#define HELPID_PREFSGUIWIN  (HELPID_SRCSELWIN+1)
#define HELPID_PREFSSMPWIN  (HELPID_PREFSGUIWIN+1)
#define HELPID_PREFSVMEMWIN (HELPID_PREFSSMPWIN+1)
#define HELPID_PREFSMISCWIN (HELPID_PREFSVMEMWIN+1)
#define HELPID_INFOWIN      (HELPID_PREFSMISCWIN+1)
#define HELPID_STATUSWIN    (HELPID_INFOWIN+1)
#define HELPID_WORKSHOPMAIN (HELPID_STATUSWIN+1)
#define HELPID_WFKTSELWIN   (HELPID_WORKSHOPMAIN+1)
#define HELPID_MODWINCURVE  (HELPID_WFKTSELWIN+1)
#define HELPID_MODWINCYCLE  (HELPID_MODWINCURVE+1)
#define HELPID_MODWINVECTOR (HELPID_MODWINCYCLE+1)
#define HELPID_MODWINUSER   (HELPID_MODWINVECTOR+1)

#define HELPID_MSG1         (HELPID_MODWINUSER+1)
#define HELPID_MSG2_1       (HELPID_MSG1+1)
#define HELPID_MSG2_2       (HELPID_MSG2_1+1)
#define HELPID_MSG2_3       (HELPID_MSG2_2+1)
#define HELPID_MSG2_4       (HELPID_MSG2_3+1)
#define HELPID_MSG3         (HELPID_MSG2_4+1)
#define HELPID_MSG3_1       (HELPID_MSG3+1)
#define HELPID_MSG3_2       (HELPID_MSG3_1+1)
#define HELPID_MSG3_3       (HELPID_MSG3_2+1)
#define HELPID_MSG3_4       (HELPID_MSG3_3+1)
#define HELPID_MSG3_5       (HELPID_MSG3_4+1)
#define HELPID_MSG3_6       (HELPID_MSG3_5+1)
#define HELPID_MSG4         (HELPID_MSG3_6+1)
#define HELPID_MSG4_1       (HELPID_MSG4+1)
#define HELPID_MSG4_2       (HELPID_MSG4_1+1)
#define HELPID_MSG4_3       (HELPID_MSG4_2+1)
#define HELPID_MSG5         (HELPID_MSG4_3+1)
#define HELPID_MSG5_1       (HELPID_MSG5+1)
#define HELPID_MSG5_2       (HELPID_MSG5_1+1)
#define HELPID_QUE          (HELPID_MSG5_2+1)
#define HELPID_QUE1         (HELPID_QUE+1)
#define HELPID_QUE1_1       (HELPID_QUE1+1)
#define HELPID_QUE1_2       (HELPID_QUE1_1+1)
#define HELPID_QUE2         (HELPID_QUE1_2+1)
#define HELPID_QUE2_1       (HELPID_QUE2+1)
#define HELPID_QUE2_2       (HELPID_QUE2_1+1)
#define HELPID_QUE2_3       (HELPID_QUE2_2+1)

#define HELPID_ANZ          (HELPID_QUE2_3+1)

#endif /* SFX_HELPID_H */

//-- eof ----------------------------------------------------------------------
