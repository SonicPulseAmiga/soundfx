/*
	Data for "imgWinO2" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>


UWORD imgWinO2PaletteRGB4[16] =
{
	0x0AAA,0x0212,0x0FEF,0x0A8C,0x04F4,0x0CBB,0x0766,0x0FF7,
	0x0602,0x0903,0x0F04,0x014F,0x0FF0,0x0EA0,0x00B0,0x0070
};

ULONG imgWinO2PaletteRGB32[50] =
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

UWORD chip imgWinO2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8FFF,0xFE40,0x9000,0x0140,
	0xA000,0x02C0,0xA7FF,0xF8C0,0xA000,0x00C0,0xA3FF,0xF8C0,
	0xA3FF,0xF8C0,0xA37F,0xF8C0,0xA37F,0xF8C0,0xA32F,0x68C0,
	0xA204,0x08C0,0xA000,0x00C0,0xA21E,0xC8C0,0xA37F,0xD8C0,
	0xA37F,0xF8C0,0xA3FF,0xF8C0,0xA3FF,0xF8C0,0xA3FF,0xF8C0,
	0xA3FF,0xF8C0,0xA000,0x00C0,0xA800,0x00C0,0x9000,0x0140,
	0x8FFF,0xFE40,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4FFF,0xFC20,0x4DFF,0xAE20,0x4FFF,0xFE20,0x4C00,0x0620,
	0x4C80,0x0620,0x4C80,0x0620,0x4C10,0x1620,0x4C48,0x0620,
	0x4C02,0x4620,0x4C00,0x0E20,0x4C00,0x4620,0x4C41,0x1620,
	0x4C00,0x2620,0x4C80,0x0620,0x4C00,0x0620,0x4C00,0x0620,
	0x4C00,0x0620,0x4FFF,0xFE20,0x47FF,0xFE20,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4FFF,0xFE40,0x5FFF,0xFF40,
	0x7000,0x03C0,0x7200,0x53C0,0x77FF,0xFBC0,0x7400,0x03C0,
	0x7480,0x03C0,0x7480,0x03C0,0x7490,0x13C0,0x74D8,0x93C0,
	0x75FB,0xF3C0,0x77FF,0xFBC0,0x75E1,0x73C0,0x74C1,0x33C0,
	0x7480,0x23C0,0x7480,0x03C0,0x7400,0x03C0,0x7400,0x03C0,
	0x7400,0x03C0,0x7000,0x03C0,0x7FFF,0xFFC0,0x5FFF,0xFF40,
	0x4FFF,0xFE40,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0FFF,0xFE00,0x1FFF,0xFF00,
	0x3000,0x0180,0x35FF,0xA980,0x3000,0x0180,0x3000,0x0180,
	0x3080,0x0180,0x3080,0x0180,0x3010,0x1180,0x3048,0x0180,
	0x3002,0x4180,0x3000,0x0980,0x3000,0x4180,0x3041,0x1180,
	0x3000,0x2180,0x3080,0x0180,0x3000,0x0180,0x3000,0x0180,
	0x3000,0x0180,0x3000,0x0180,0x3000,0x0180,0x1FFF,0xFF00,
	0x0FFF,0xFE00,0x0000,0x0000,0x0000,0x0000
};

struct Image imgWinO2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgWinO2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
