/****h* sfxmod/sfxoperator.h [0.0] *
*
*  NAME
*    sfxoperator.h
*  COPYRIGHT
*    $VER: sfxoperator.h 0.0 (14.04.03) © by Stefan Kost 1998-2003
*  FUNCTION
*    operator plugins for SoundFX
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    14.Apr.2003
*  MODIFICATION HISTORY
*    14.Apr.2003    V 4.2   most recent version
*    14.Apr.2003    V 4.2   initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXOPERATOR_H
#define  LIBRARIES_SFXOPERATOR_H

#define SFXCommonCoreInstanceFields \
	unsigned long			srat;\
	unsigned long			slen;\
	volatile unsigned long	curlen;


#endif	 /* LIBRARIES_SFXOPERATOR_H */
