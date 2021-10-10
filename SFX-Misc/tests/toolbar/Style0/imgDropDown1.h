/*
	Data for "imgDropDown1" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>

UWORD chip imgDropDown1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x4000,0x0020,0x4000,0x0020,0x4000,0x0020,
	0x42AA,0xA820,0x4400,0x0C20,0x41FF,0xF020,0x4080,0x2020,
	0x4040,0x4020,0x4020,0x8020,0x4011,0x0020,0x400A,0x0020,
	0x4004,0x0020,0x4000,0x0020,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x4000,0x0020,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0x8000,0x0040,0x87FF,0xFC40,0x8800,0x0240,
	0x9555,0x5540,0xABFF,0xF2C0,0x9600,0x0D40,0x8B7F,0xDA40,
	0x85BF,0xB440,0x82DF,0x6840,0x816E,0xD040,0x80B5,0xA040,
	0x805B,0x4040,0x802E,0x8040,0x8015,0x0040,0x800A,0x0040,
	0x8004,0x0040,0x8000,0x0040,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4000,0x0040,
	0x4000,0x0040,0x4000,0x0040,0x4000,0x0040,0x47FF,0xFC40,
	0x4AAA,0xAA40,0x5400,0x0D40,0x4BFF,0xEA40,0x457F,0xD440,
	0x42BF,0xA840,0x415F,0x5040,0x40AE,0xA040,0x4055,0x4040,
	0x402A,0x8040,0x4015,0x0040,0x400A,0x0040,0x4004,0x0040,
	0x4000,0x0040,0x4000,0x0040,0x4000,0x0040,0x4000,0x0040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x07FF,0xFC00,0x0FFF,0xFE00,
	0x1800,0x0300,0x3000,0x0180,0x1800,0x0300,0x0C00,0x0600,
	0x0600,0x0C00,0x0300,0x1800,0x0180,0x3000,0x00C0,0x6000,
	0x0060,0xC000,0x0031,0x8000,0x001B,0x0000,0x000E,0x0000,
	0x0004,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgDropDown1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgDropDown1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};