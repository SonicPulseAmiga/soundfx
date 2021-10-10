/*
	Data for "imgLoad2" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>

UWORD chip imgLoad2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8001,0xF440,0x8002,0x0A40,0x83E4,0xE140,0x8019,0x1540,
	0x8BE8,0xCD40,0x96B0,0x1D40,0x97FF,0xE240,0x96AA,0xAC40,
	0x9555,0x6240,0x967F,0xFD40,0x9540,0x0540,0x969F,0xE540,
	0x9480,0x0A40,0x953F,0xCA40,0x9500,0x1440,0x967F,0x9440,
	0x9400,0x2840,0x97FF,0xE840,0x8800,0x1040,0x87FF,0xE040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4001,0xF420,
	0x4003,0xFE20,0x43E7,0xFF20,0x47FF,0x1FA0,0x4BFE,0xEBA0,
	0x5FF7,0x33A0,0x7FFF,0xE3A0,0x7FFF,0xFF20,0x7D55,0x7620,
	0x7EAA,0xBF20,0x7DFF,0xFFA0,0x7EFF,0xFFA0,0x7DFF,0xFFA0,
	0x7FFF,0xFF20,0x7FFF,0xFF20,0x7FFF,0xFE20,0x7FFF,0xFE20,
	0x7FFF,0xFC20,0x7FFF,0xFC20,0x5FFF,0xF820,0x4FFF,0xF020,
	0x47FF,0xE020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4000,0x0040,
	0x4001,0xF440,0x4002,0x0A40,0x43E5,0x1540,0x441A,0x2940,
	0x480A,0xD140,0x5140,0x2140,0x5000,0x0240,0x52AA,0x8C40,
	0x5155,0x4240,0x5200,0x0140,0x513F,0xF940,0x5260,0x1940,
	0x507F,0xF240,0x50C0,0x3240,0x50FF,0xE440,0x5180,0x6440,
	0x53FF,0xC840,0x5000,0x0840,0x4800,0x1040,0x47FF,0xE040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0xF400,
	0x0002,0x0A00,0x03E4,0x0100,0x0418,0x0080,0x0C00,0x0080,
	0x1000,0x0080,0x2000,0x0080,0x2000,0x0100,0x2000,0x0200,
	0x2000,0x0100,0x2000,0x0080,0x2000,0x0080,0x2000,0x0080,
	0x2000,0x0100,0x2000,0x0100,0x2000,0x0200,0x2000,0x0200,
	0x2000,0x0400,0x2000,0x0400,0x1000,0x0800,0x0800,0x1000,
	0x07FF,0xE000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgLoad2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgLoad2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
