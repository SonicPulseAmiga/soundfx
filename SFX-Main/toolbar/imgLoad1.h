/*
	Data for "imgLoad1" Image
*/

#include <exec/types.h>
#include <intuition/intuition.h>


UWORD imgLoad1PaletteRGB4[16] =
{
	0x0AAA,0x0212,0x0FEF,0x0A8C,0x04F4,0x0CBB,0x0766,0x0FF7,
	0x0602,0x0903,0x0F04,0x014F,0x0FF0,0x0EA0,0x00B0,0x0070
};

ULONG imgLoad1PaletteRGB32[50] =
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

UWORD chip imgLoad1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x43FF,0xF820,0x47FF,0xFC20,
	0x4C00,0x0620,0x5800,0x0320,0x72F5,0xEDA0,0x720A,0x1DA0,
	0x72FF,0xEDA0,0x7204,0x0DA0,0x72FB,0xEDA0,0x7200,0x0DA0,
	0x727F,0xEDA0,0x72FF,0xEDA0,0x7200,0x0DA0,0x73FF,0xFDA0,
	0x7308,0x3DA0,0x7318,0x3DA0,0x7318,0xBDA0,0x7359,0xBDA0,
	0x731B,0x3DA0,0x5BFF,0xFB20,0x4C00,0x0620,0x47FF,0xFC20,
	0x43FF,0xF820,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x83FF,0xF840,
	0x87FF,0xFC40,0x8C04,0x0640,0x9A0A,0x1B40,0x9A15,0x0B40,
	0x9A00,0x1B40,0x9A00,0x1B40,0x9A00,0x1B40,0x9A00,0x1B40,
	0x9A00,0x1B40,0x9A00,0x1B40,0x9BFF,0xFB40,0x9BFF,0xFB40,
	0x9B86,0x7B40,0x9B94,0x7B40,0x9B90,0x7B40,0x9BD0,0x7B40,
	0x9BD0,0xFB40,0x8C00,0x0640,0x87FF,0xFC40,0x83FF,0xF840,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x43FF,0xF840,0x47FF,0xFC40,
	0x4C00,0x0640,0x5800,0x0340,0x70F1,0xF1C0,0x7000,0x01C0,
	0x70C0,0x71C0,0x7000,0x11C0,0x70F1,0xF1C0,0x7000,0x11C0,
	0x707F,0xF1C0,0x70FF,0xF1C0,0x71FF,0xF1C0,0x7000,0x01C0,
	0x708E,0x01C0,0x7084,0x01C0,0x7080,0x81C0,0x7081,0x81C0,
	0x70C3,0x01C0,0x5800,0x0340,0x4C00,0x0640,0x47FF,0xFC40,
	0x43FF,0xF840,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x03FF,0xF800,0x0400,0x0400,
	0x0800,0x0200,0x1004,0x0100,0x220E,0x0880,0x221F,0x0880,
	0x223F,0x8880,0x220E,0x0880,0x220E,0x0880,0x220E,0x0880,
	0x2200,0x0880,0x2200,0x0880,0x2200,0x0880,0x23FF,0xF880,
	0x2300,0x0880,0x2300,0x2880,0x2300,0x2880,0x2300,0x2880,
	0x2300,0x2880,0x1000,0x0100,0x0800,0x0200,0x0400,0x0400,
	0x03FF,0xF800,0x0000,0x0000,0x0000,0x0000
};

struct Image imgLoad1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgLoad1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
