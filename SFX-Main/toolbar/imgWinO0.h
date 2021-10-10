/*
	Data for "imgWinO0" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>


UWORD imgWinO0PaletteRGB4[16] =
{
	0x0AAA,0x0212,0x0FEF,0x0A8C,0x04F4,0x0CBB,0x0766,0x0FF7,
	0x0602,0x0903,0x0F04,0x014F,0x0FF0,0x0EA0,0x00B0,0x0070
};

ULONG imgWinO0PaletteRGB32[50] =
{
	0x00100000,	/* Record Header */
	0xAEEEEEEE,0xA2222222,0xA2222222,
	0x20000000,0x10000000,0x20000000,
	0xFFFFFFFF,0xEFFFFFFF,0xFFFFFFFF,
	0xAEEEEEEE,0x8EEEEEEE,0xCEEEEEEE,
	0x3DDDDDDD,0xFFFFFFFF,0x3DDDDDDD,
	0xCEEEEEEE,0xBEEEEEEE,0xBEEEEEEE,
	0x71111111,0x61111111,0x61111111,
	0xFFFFFFFF,0xFFFFFFFF,0x7DDDDDDD,
	0x65555555,0x00000000,0x20000000,
	0x96666666,0x00000000,0x31111111,
	0xFFFFFFFF,0x00000000,0x41111111,
	0x0CCCCCCC,0x3DDDDDDD,0xFFFFFFFF,
	0xFFFFFFFF,0xFFFFFFFF,0x00000000,
	0xF3333333,0xAEEEEEEE,0x00000000,
	0x00000000,0xBEEEEEEE,0x00000000,
	0x00000000,0x7DDDDDDD,0x00000000,
	0x00000000	/* Terminator */
};

UWORD chip imgWinO0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0FFF,0xFE00,
	0x1000,0x0300,0x17FF,0xF900,0x1000,0x0100,0x13FF,0xF900,
	0x13FF,0xF900,0x137F,0xF900,0x137F,0xF900,0x132F,0x6900,
	0x1204,0x0900,0x1000,0x0100,0x121E,0xC900,0x137F,0xD900,
	0x137F,0xF900,0x13FF,0xF900,0x13FF,0xF900,0x13FF,0xF900,
	0x13FF,0xF900,0x1000,0x0100,0x1800,0x0100,0x0FFF,0xFE00,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0FFF,0xFC00,0x0DFF,0xAE00,0x0FFF,0xFE00,0x0C00,0x0600,
	0x0C80,0x0600,0x0C80,0x0600,0x0C10,0x1600,0x0C48,0x0600,
	0x0C02,0x4600,0x0C00,0x0E00,0x0C00,0x4600,0x0C41,0x1600,
	0x0C00,0x2600,0x0C80,0x0600,0x0C00,0x0600,0x0C00,0x0600,
	0x0C00,0x0600,0x0FFF,0xFE00,0x07FF,0xFE00,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0FFF,0xFE00,
	0x1000,0x0300,0x1200,0x5300,0x17FF,0xFB00,0x1400,0x0300,
	0x1480,0x0300,0x1480,0x0300,0x1490,0x1300,0x14D8,0x9300,
	0x15FB,0xF300,0x17FF,0xFB00,0x15E1,0x7300,0x14C1,0x3300,
	0x1480,0x2300,0x1480,0x0300,0x1400,0x0300,0x1400,0x0300,
	0x1400,0x0300,0x1000,0x0300,0x1FFF,0xFF00,0x0FFF,0xFE00,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x05FF,0xA800,0x0000,0x0000,0x0000,0x0000,
	0x0080,0x0000,0x0080,0x0000,0x0010,0x1000,0x0048,0x0000,
	0x0002,0x4000,0x0000,0x0800,0x0000,0x4000,0x0041,0x1000,
	0x0000,0x2000,0x0080,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgWinO0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgWinO0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};