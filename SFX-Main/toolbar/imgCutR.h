/*
	Data for "imgCutR2" Image
*/

#include <exec/types.h>

UWORD chip imgCutR2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8082,0x0040,0x8145,0x0040,
	0x8228,0x8040,0x8492,0x4040,0x8492,0x4040,0x8244,0x8040,
	0x8244,0x9040,0x826C,0xA840,0x8139,0x4440,0x8139,0x9240,
	0x8092,0x9940,0x8111,0x9CC0,0x8228,0x9940,0x84FE,0x1240,
	0x89EF,0x0440,0x93EF,0x8840,0x926C,0x9040,0x926C,0x9040,
	0x936D,0x9040,0x89C7,0x2040,0x8410,0x4040,0x8228,0x8040,
	0x81C7,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4082,0x0020,0x4145,0x0020,0x4145,0x0020,0x40AA,0x0020,
	0x40AA,0x0020,0x4092,0x0020,0x4044,0x1020,0x4044,0x2820,
	0x4038,0x2420,0x4038,0x2220,0x40FE,0x2420,0x41FF,0x2820,
	0x43FF,0x9020,0x477D,0xC020,0x47FF,0xC020,0x47BB,0xC020,
	0x47FF,0xC020,0x43EF,0x8020,0x41C7,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4082,0x0040,0x41C7,0x0040,
	0x436D,0x8040,0x46BA,0xC040,0x46BA,0xC040,0x4355,0x8040,
	0x4355,0x9040,0x436D,0xB840,0x41BB,0x6C40,0x41BB,0xC640,
	0x40C6,0xC340,0x41C7,0xC1C0,0x4301,0xC340,0x4600,0xC640,
	0x4C00,0x6C40,0x5882,0x3840,0x5800,0x3040,0x5844,0x3040,
	0x5800,0x3040,0x4C10,0x6040,0x4638,0xC040,0x43EF,0x8040,
	0x41C7,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0082,0x0000,0x01C7,0x0000,
	0x036D,0x8000,0x0638,0xC000,0x0638,0xC000,0x0311,0x8000,
	0x0311,0x9000,0x0301,0xB800,0x0183,0x6C00,0x0183,0xC600,
	0x00D6,0xC300,0x01D7,0xC180,0x0329,0xC300,0x06FE,0xC600,
	0x0DEF,0x6C00,0x1B6D,0xB800,0x1A28,0xB000,0x1A28,0xB000,
	0x1A28,0xB000,0x0DD7,0x6000,0x0638,0xC000,0x03EF,0x8000,
	0x01C7,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgCutR2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgCutR2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgCutR1" Image
*/

#include <exec/types.h>

UWORD chip imgCutR1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4082,0x0020,0x4145,0x0020,
	0x4228,0x8020,0x4492,0x4020,0x4492,0x4020,0x4244,0x8020,
	0x4244,0x9020,0x426C,0xA820,0x4139,0x4420,0x4139,0x9220,
	0x4092,0x9920,0x4111,0x9CA0,0x4228,0x9920,0x44FE,0x1220,
	0x49EF,0x0420,0x53EF,0x8820,0x526C,0x9020,0x526C,0x9020,
	0x536D,0x9020,0x49C7,0x2020,0x4410,0x4020,0x4228,0x8020,
	0x41C7,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8082,0x0040,0x8145,0x0040,0x8145,0x0040,0x80AA,0x0040,
	0x80AA,0x0040,0x8092,0x0040,0x8044,0x1040,0x8044,0x2840,
	0x8038,0x2440,0x8038,0x2240,0x80FE,0x2440,0x81FF,0x2840,
	0x83FF,0x9040,0x877D,0xC040,0x87FF,0xC040,0x87BB,0xC040,
	0x87FF,0xC040,0x83EF,0x8040,0x81C7,0x0040,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4082,0x0040,0x41C7,0x0040,
	0x436D,0x8040,0x46BA,0xC040,0x46BA,0xC040,0x4355,0x8040,
	0x4355,0x9040,0x436D,0xB840,0x41BB,0x6C40,0x41BB,0xC640,
	0x40C6,0xC340,0x41C7,0xC1C0,0x4301,0xC340,0x4600,0xC640,
	0x4C00,0x6C40,0x5882,0x3840,0x5800,0x3040,0x5844,0x3040,
	0x5800,0x3040,0x4C10,0x6040,0x4638,0xC040,0x43EF,0x8040,
	0x41C7,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0082,0x0000,0x01C7,0x0000,
	0x036D,0x8000,0x0638,0xC000,0x0638,0xC000,0x0311,0x8000,
	0x0311,0x9000,0x0301,0xB800,0x0183,0x6C00,0x0183,0xC600,
	0x00D6,0xC300,0x01D7,0xC180,0x0329,0xC300,0x06FE,0xC600,
	0x0DEF,0x6C00,0x1B6D,0xB800,0x1A28,0xB000,0x1A28,0xB000,
	0x1A28,0xB000,0x0DD7,0x6000,0x0638,0xC000,0x03EF,0x8000,
	0x01C7,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgCutR1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgCutR1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgCutR0" Image
*/

#include <exec/types.h>

UWORD chip imgCutR0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0082,0x0000,
	0x0145,0x0000,0x02AA,0x8000,0x02AA,0x8000,0x0155,0x0000,
	0x0155,0x0000,0x016D,0x1000,0x00BA,0x2800,0x00BA,0x5400,
	0x0054,0x5A00,0x00D6,0x5D00,0x0129,0x5A00,0x02FE,0xD400,
	0x05EF,0x6800,0x0BEF,0xB000,0x0A6C,0xA000,0x0A6C,0xA000,
	0x0B6D,0xA000,0x05D7,0x4000,0x0228,0x8000,0x01C7,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0082,0x0000,0x0145,0x0000,0x0145,0x0000,0x00AA,0x0000,
	0x00AA,0x0000,0x0092,0x0000,0x0044,0x1000,0x0044,0x2800,
	0x0038,0x2400,0x0038,0x2200,0x00FE,0x2400,0x01FF,0x2800,
	0x03FF,0x9000,0x077D,0xC000,0x07FF,0xC000,0x07BB,0xC000,
	0x07FF,0xC000,0x03EF,0x8000,0x01C7,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0082,0x0000,
	0x0145,0x0000,0x02AA,0x8000,0x02AA,0x8000,0x0155,0x0000,
	0x0155,0x0000,0x016D,0x1000,0x00BA,0x2800,0x00BA,0x4400,
	0x0044,0x4200,0x00C6,0x4100,0x0101,0x4200,0x0200,0xC400,
	0x0400,0x6800,0x0882,0x3000,0x0800,0x2000,0x0844,0x2000,
	0x0800,0x2000,0x0410,0x4000,0x0228,0x8000,0x01C7,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0010,0x0000,0x0010,0x0000,0x0028,0x0000,0x00FE,0x0000,
	0x01EF,0x0000,0x036D,0x8000,0x0228,0x8000,0x0228,0x8000,
	0x0228,0x8000,0x01C7,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgCutR0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgCutR0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};