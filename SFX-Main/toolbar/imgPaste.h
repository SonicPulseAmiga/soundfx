/*
	Data for "imgPaste2" Image
*/

#include <exec/types.h>

UWORD chip imgPaste2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x801F,0x0040,0x8020,0x8040,
	0x8040,0x4040,0x818E,0x3040,0x8211,0x0840,0x841F,0x0440,
	0x8931,0x9240,0x8820,0x8240,0x881F,0x1240,0x8800,0x1240,
	0x887F,0xD240,0x8818,0xD240,0x885D,0xD240,0x881D,0xD240,
	0x887F,0xD240,0x880F,0x9240,0x8877,0x1240,0x882A,0x1240,
	0x8800,0x1240,0x897F,0xE240,0x8400,0x0440,0x8200,0x0840,
	0x81FF,0xF040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x401F,0x0020,0x4031,0x8020,0x402E,0x8020,0x41E0,0xF020,
	0x42CE,0x6820,0x43DF,0x7820,0x43E0,0xE820,0x43FF,0xE820,
	0x4380,0x2820,0x43FF,0xE820,0x43BF,0xE820,0x43FF,0xE820,
	0x438F,0xA820,0x43FF,0xE820,0x438F,0xE820,0x43D7,0xE820,
	0x43FF,0xE820,0x4280,0x1820,0x41FF,0xF020,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x401F,0x0040,0x403F,0x8040,
	0x4060,0xC040,0x41D1,0x7040,0x43CE,0x7840,0x4600,0x0C40,
	0x4DC0,0x7640,0x4D1F,0x1640,0x4D20,0x8640,0x4D00,0x0640,
	0x4D7F,0xC640,0x4D00,0x0640,0x4D40,0x0640,0x4D00,0x0640,
	0x4D70,0x4640,0x4D00,0x0640,0x4D70,0x0640,0x4D28,0x0640,
	0x4D00,0x0640,0x4D80,0x1640,0x4600,0x0C40,0x43FF,0xF840,
	0x41FF,0xF040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x001F,0x0000,0x003F,0x8000,
	0x0060,0xC000,0x01C0,0x7000,0x03C0,0x7800,0x0600,0x0C00,
	0x0C00,0x0600,0x0C00,0x0600,0x0C00,0x0600,0x0C00,0x0600,
	0x0C00,0x0600,0x0C00,0x0600,0x0C08,0x8600,0x0C08,0x8600,
	0x0C05,0x0600,0x0C05,0x0600,0x0C02,0x0600,0x0C00,0x0600,
	0x0C00,0x0600,0x0C00,0x0600,0x0600,0x0C00,0x03FF,0xF800,
	0x01FF,0xF000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPaste2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPaste2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgPaste1" Image
*/

#include <exec/types.h>

UWORD chip imgPaste1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x401F,0x0020,0x4020,0x8020,
	0x4040,0x4020,0x418E,0x3020,0x4211,0x0820,0x441F,0x0420,
	0x4931,0x9220,0x4820,0x8220,0x481F,0x1220,0x4800,0x1220,
	0x487F,0xD220,0x4818,0xD220,0x485D,0xD220,0x481D,0xD220,
	0x487F,0xD220,0x480F,0x9220,0x4877,0x1220,0x482A,0x1220,
	0x4800,0x1220,0x497F,0xE220,0x4400,0x0420,0x4200,0x0820,
	0x41FF,0xF020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x801F,0x0040,0x8031,0x8040,0x802E,0x8040,0x81E0,0xF040,
	0x82CE,0x6840,0x83DF,0x7840,0x83E0,0xE840,0x83FF,0xE840,
	0x8380,0x2840,0x83FF,0xE840,0x83BF,0xE840,0x83FF,0xE840,
	0x838F,0xA840,0x83FF,0xE840,0x838F,0xE840,0x83D7,0xE840,
	0x83FF,0xE840,0x8280,0x1840,0x81FF,0xF040,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x401F,0x0040,0x403F,0x8040,
	0x4060,0xC040,0x41D1,0x7040,0x43CE,0x7840,0x4600,0x0C40,
	0x4DC0,0x7640,0x4D1F,0x1640,0x4D20,0x8640,0x4D00,0x0640,
	0x4D7F,0xC640,0x4D00,0x0640,0x4D40,0x0640,0x4D00,0x0640,
	0x4D70,0x4640,0x4D00,0x0640,0x4D70,0x0640,0x4D28,0x0640,
	0x4D00,0x0640,0x4D80,0x1640,0x4600,0x0C40,0x43FF,0xF840,
	0x41FF,0xF040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x001F,0x0000,0x003F,0x8000,
	0x0060,0xC000,0x01C0,0x7000,0x03C0,0x7800,0x0600,0x0C00,
	0x0C00,0x0600,0x0C00,0x0600,0x0C00,0x0600,0x0C00,0x0600,
	0x0C00,0x0600,0x0C00,0x0600,0x0C08,0x8600,0x0C08,0x8600,
	0x0C05,0x0600,0x0C05,0x0600,0x0C02,0x0600,0x0C00,0x0600,
	0x0C00,0x0600,0x0C00,0x0600,0x0600,0x0C00,0x03FF,0xF800,
	0x01FF,0xF000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPaste1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPaste1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgPaste0" Image
*/

#include <exec/types.h>

UWORD chip imgPaste0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,0x0000,
	0x0020,0x8000,0x004E,0x4000,0x01D1,0x7000,0x021F,0x0800,
	0x0531,0x9400,0x0420,0x8400,0x041F,0x1400,0x0400,0x1400,
	0x047F,0xD400,0x0418,0xD400,0x045D,0xD400,0x041D,0xD400,
	0x047F,0xD400,0x040F,0x9400,0x0477,0x1400,0x042A,0x1400,
	0x0400,0x1400,0x057F,0xE400,0x0200,0x0800,0x01FF,0xF000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x001F,0x0000,0x0031,0x8000,0x002E,0x8000,0x01E0,0xF000,
	0x02CE,0x6800,0x03DF,0x7800,0x03E0,0xE800,0x03FF,0xE800,
	0x0380,0x2800,0x03FF,0xE800,0x03BF,0xE800,0x03FF,0xE800,
	0x038F,0xA800,0x03FF,0xE800,0x038F,0xE800,0x03D7,0xE800,
	0x03FF,0xE800,0x0280,0x1800,0x01FF,0xF000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x001F,0x0000,
	0x0020,0x8000,0x0051,0x4000,0x01CE,0x7000,0x0200,0x0800,
	0x05C0,0x7400,0x051F,0x1400,0x0520,0x8400,0x0500,0x0400,
	0x057F,0xC400,0x0500,0x0400,0x0540,0x0400,0x0500,0x0400,
	0x0570,0x4400,0x0500,0x0400,0x0570,0x0400,0x0528,0x0400,
	0x0500,0x0400,0x0580,0x1400,0x0200,0x0800,0x01FF,0xF000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0008,0x8000,0x0008,0x8000,
	0x0005,0x0000,0x0005,0x0000,0x0002,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPaste0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPaste0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};