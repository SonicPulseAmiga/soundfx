/*
	Data for "imgEraseR2" Image
*/

#include <exec/types.h>

UWORD chip imgEraseR2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8003,0xC040,
	0x8004,0x2040,0x8008,0x1040,0x8011,0x8840,0x8023,0xC840,
	0x8046,0x4840,0x808C,0x4840,0x8118,0xC440,0x823F,0x9240,
	0x8463,0x1940,0x88FE,0x1CC0,0x918C,0x1940,0xA318,0x9240,
	0xA631,0x4440,0xA462,0x2840,0xA4C4,0x1040,0xA788,0x0040,
	0xA310,0x0040,0x9020,0x0040,0x8840,0x0040,0x8780,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x4003,0xC020,0x4006,0x6020,0x400C,0x2020,
	0x4019,0xA020,0x4033,0xA020,0x4067,0x3020,0x40C0,0x6820,
	0x419C,0xE420,0x4301,0xA220,0x4673,0x2420,0x4CE6,0x2820,
	0x49CC,0x1020,0x4B98,0x0020,0x4B30,0x0020,0x4860,0x0020,
	0x4CC0,0x0020,0x4780,0x0020,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4003,0xC040,
	0x4007,0xE040,0x400C,0x3040,0x401A,0x5840,0x4034,0x1840,
	0x4068,0x1840,0x40D0,0x1840,0x41A0,0x0C40,0x4340,0x4640,
	0x46A2,0x8340,0x4D01,0x41C0,0x5A8A,0xC340,0x7515,0xC640,
	0x722B,0x6C40,0x7056,0x3840,0x70AC,0x1040,0x7358,0x0040,
	0x74B0,0x0040,0x5860,0x0040,0x4FC0,0x0040,0x4780,0x0040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0xC000,
	0x0007,0xE000,0x000C,0x3000,0x0018,0x1800,0x0031,0x9800,
	0x0063,0x9800,0x00C7,0x9800,0x018F,0x8C00,0x0300,0x0600,
	0x0600,0x0300,0x0C00,0x4180,0x1800,0xC300,0x3001,0xC600,
	0x3003,0x6C00,0x3006,0x3800,0x300C,0x1000,0x3018,0x0000,
	0x3030,0x0000,0x1860,0x0000,0x0FC0,0x0000,0x0780,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseR2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseR2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgEraseR1" Image
*/

#include <exec/types.h>

UWORD chip imgEraseR1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4003,0xC020,
	0x4004,0x2020,0x4008,0x1020,0x4011,0x8820,0x4023,0xC820,
	0x4046,0x4820,0x408C,0x4820,0x4118,0xC420,0x423F,0x9220,
	0x4463,0x1920,0x48FE,0x1CA0,0x518C,0x1920,0x6318,0x9220,
	0x6631,0x4420,0x6462,0x2820,0x64C4,0x1020,0x6788,0x0020,
	0x6310,0x0020,0x5020,0x0020,0x4840,0x0020,0x4780,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0x8003,0xC040,0x8006,0x6040,0x800C,0x2040,
	0x8019,0xA040,0x8033,0xA040,0x8067,0x3040,0x80C0,0x6840,
	0x819C,0xE440,0x8301,0xA240,0x8673,0x2440,0x8CE6,0x2840,
	0x89CC,0x1040,0x8B98,0x0040,0x8B30,0x0040,0x8860,0x0040,
	0x8CC0,0x0040,0x8780,0x0040,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4003,0xC040,
	0x4007,0xE040,0x400C,0x3040,0x401A,0x5840,0x4034,0x1840,
	0x4068,0x1840,0x40D0,0x1840,0x41A0,0x0C40,0x4340,0x4640,
	0x46A2,0x8340,0x4D01,0x41C0,0x5A8A,0xC340,0x7515,0xC640,
	0x722B,0x6C40,0x7056,0x3840,0x70AC,0x1040,0x7358,0x0040,
	0x74B0,0x0040,0x5860,0x0040,0x4FC0,0x0040,0x4780,0x0040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0003,0xC000,
	0x0007,0xE000,0x000C,0x3000,0x0018,0x1800,0x0031,0x9800,
	0x0063,0x9800,0x00C7,0x9800,0x018F,0x8C00,0x0300,0x0600,
	0x0600,0x0300,0x0C00,0x4180,0x1800,0xC300,0x3001,0xC600,
	0x3003,0x6C00,0x3006,0x3800,0x300C,0x1000,0x3018,0x0000,
	0x3030,0x0000,0x1860,0x0000,0x0FC0,0x0000,0x0780,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseR1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseR1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgEraseR0" Image
*/

#include <exec/types.h>

UWORD chip imgEraseR0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0003,0xC000,0x0004,0x2000,0x0009,0x9000,0x0013,0xD000,
	0x0026,0x5000,0x004C,0x5000,0x0098,0xC800,0x013F,0x9400,
	0x0263,0x1A00,0x04FE,0x5D00,0x098C,0xDA00,0x1319,0x5400,
	0x1632,0x2800,0x1464,0x1000,0x14C8,0x0000,0x1790,0x0000,
	0x1320,0x0000,0x0840,0x0000,0x0780,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0003,0xC000,0x0006,0x6000,0x000C,0x2000,
	0x0019,0xA000,0x0033,0xA000,0x0067,0x3000,0x00C0,0x6800,
	0x019C,0xE400,0x0301,0xA200,0x0673,0x2400,0x0CE6,0x2800,
	0x09CC,0x1000,0x0B98,0x0000,0x0B30,0x0000,0x0860,0x0000,
	0x0CC0,0x0000,0x0780,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0003,0xC000,0x0004,0x2000,0x000A,0x5000,0x0014,0x1000,
	0x0028,0x1000,0x0050,0x1000,0x00A0,0x0800,0x0140,0x4400,
	0x02A2,0x8200,0x0501,0x4100,0x0A8A,0xC200,0x1515,0x4400,
	0x122A,0x2800,0x1054,0x1000,0x10A8,0x0000,0x1350,0x0000,
	0x14A0,0x0000,0x0840,0x0000,0x0780,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x8000,
	0x0003,0x8000,0x0007,0x8000,0x000F,0x8000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseR0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseR0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};