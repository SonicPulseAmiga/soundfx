/****h* sfxsupport.library/req-interpol.h [4.3] *
*
*  NAME
*    req-interpol.h
*  COPYRIGHT
*    $VER: req-interpol.h 4.3 (05.08.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    get interpolator requester - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    05.Aug.2003
*  MODIFICATION HISTORY
*    05.Aug.2003	V 4.3	most recent version
*    05.Aug.2003	V 4.3	initial version
*  NOTES
*
*******
*/

#ifndef REQ_INTERPOL_H
#define REQ_INTERPOL_H

//-- prototypes ---------------------------------------------------------------

#ifndef REQ_INTERPOL_C
	extern UBYTE GetInterpol(Interpolator *interpol);
#endif

#endif /* REQ_INTERPOL_H */

//-- eof ----------------------------------------------------------------------
