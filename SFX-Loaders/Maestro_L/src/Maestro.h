/******************************************************************************
** SFX-FileDefs.h                                                            **
**---------------------------------------------------------------------------**
** SFX FileFormatDefinitions                                                 **
**---------------------------------------------------------------------------**
** Version : V 3.0                                                           **
** Date    : 06.01.1995                                                      **
** Author  : Stefan Kost                                                     **
******************************************************************************/

struct MaestroHeader
{
	UBYTE unknown1[4];		/* 01 00 02 00 , 00 00 02 00 */
	UWORD format;			/* bit 0 Stereo/Mono , bit 1 16bit/8bit */
	UWORD unknown2;			/* 00 , 01 */
	ULONG length;
	ULONG rate;
};

#define CHANNEL_MASK	1
#define BITRES_MASK		2

/* format tags */

#define FORMAT_MAESTRO_PCM8		0
#define FORMAT_MAESTRO_PCM16	1
