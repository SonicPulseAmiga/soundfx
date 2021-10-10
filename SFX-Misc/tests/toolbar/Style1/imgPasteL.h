/*
	Data for "imgPasteL2" Image
*/

#include <exec/types.h>

UWORD chip imgPasteL2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8007,0xC040,0x8008,0x2040,
	0x8010,0x1040,0x8063,0x8C40,0x8084,0x4240,0x8107,0xC140,
	0x824C,0x64C0,0x8208,0x20C0,0x8407,0xC4C0,0x8900,0x04C0,
	0x931F,0xF4C0,0xA706,0x34C0,0x9317,0x74C0,0x8907,0x74C0,
	0x841F,0xF4C0,0x8203,0xE4C0,0x821D,0xC4C0,0x820A,0x84C0,
	0x8200,0x04C0,0x825F,0xF8C0,0x8100,0x0140,0x8080,0x0240,
	0x807F,0xFC40,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4007,0xC020,0x400C,0x6020,0x400B,0xA020,0x4078,0x3C20,
	0x40B3,0x9A20,0x40F7,0xDE20,0x41F8,0x3A20,0x42FF,0xFA20,
	0x44E0,0x0A20,0x48FF,0xFA20,0x44EF,0xFA20,0x42FF,0xFA20,
	0x41E3,0xEA20,0x40FF,0xFA20,0x40E3,0xFA20,0x40F5,0xFA20,
	0x40FF,0xFA20,0x40A0,0x0620,0x407F,0xFC20,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4007,0xC040,0x400F,0xE040,
	0x4018,0x3040,0x4074,0x5C40,0x40F3,0x9E40,0x4180,0x0340,
	0x4370,0x1DC0,0x4347,0xC5C0,0x4648,0x21C0,0x4C40,0x01C0,
	0x585F,0xF1C0,0x7040,0x01C0,0x5850,0x01C0,0x4C40,0x01C0,
	0x465C,0x11C0,0x4340,0x01C0,0x435C,0x01C0,0x434A,0x01C0,
	0x4340,0x01C0,0x4360,0x05C0,0x4180,0x0340,0x40FF,0xFE40,
	0x407F,0xFC40,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0007,0xC000,0x000F,0xE000,
	0x0018,0x3000,0x0070,0x1C00,0x00F0,0x1E00,0x0180,0x0300,
	0x0300,0x0180,0x0300,0x0180,0x0600,0x0180,0x0C00,0x0180,
	0x1800,0x0180,0x3000,0x0180,0x1802,0x2180,0x0C02,0x2180,
	0x0601,0x4180,0x0301,0x4180,0x0300,0x8180,0x0300,0x0180,
	0x0300,0x0180,0x0300,0x0180,0x0180,0x0300,0x00FF,0xFE00,
	0x007F,0xFC00,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPasteL2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPasteL2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgPasteL1" Image
*/

#include <exec/types.h>

UWORD chip imgPasteL1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4007,0xC020,0x4008,0x2020,
	0x4010,0x1020,0x4063,0x8C20,0x4084,0x4220,0x4107,0xC120,
	0x424C,0x64A0,0x4208,0x20A0,0x4407,0xC4A0,0x4900,0x04A0,
	0x531F,0xF4A0,0x6706,0x34A0,0x5317,0x74A0,0x4907,0x74A0,
	0x441F,0xF4A0,0x4203,0xE4A0,0x421D,0xC4A0,0x420A,0x84A0,
	0x4200,0x04A0,0x425F,0xF8A0,0x4100,0x0120,0x4080,0x0220,
	0x407F,0xFC20,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8007,0xC040,0x800C,0x6040,0x800B,0xA040,0x8078,0x3C40,
	0x80B3,0x9A40,0x80F7,0xDE40,0x81F8,0x3A40,0x82FF,0xFA40,
	0x84E0,0x0A40,0x88FF,0xFA40,0x84EF,0xFA40,0x82FF,0xFA40,
	0x81E3,0xEA40,0x80FF,0xFA40,0x80E3,0xFA40,0x80F5,0xFA40,
	0x80FF,0xFA40,0x80A0,0x0640,0x807F,0xFC40,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4007,0xC040,0x400F,0xE040,
	0x4018,0x3040,0x4074,0x5C40,0x40F3,0x9E40,0x4180,0x0340,
	0x4370,0x1DC0,0x4347,0xC5C0,0x4648,0x21C0,0x4C40,0x01C0,
	0x585F,0xF1C0,0x7040,0x01C0,0x5850,0x01C0,0x4C40,0x01C0,
	0x465C,0x11C0,0x4340,0x01C0,0x435C,0x01C0,0x434A,0x01C0,
	0x4340,0x01C0,0x4360,0x05C0,0x4180,0x0340,0x40FF,0xFE40,
	0x407F,0xFC40,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0007,0xC000,0x000F,0xE000,
	0x0018,0x3000,0x0070,0x1C00,0x00F0,0x1E00,0x0180,0x0300,
	0x0300,0x0180,0x0300,0x0180,0x0600,0x0180,0x0C00,0x0180,
	0x1800,0x0180,0x3000,0x0180,0x1802,0x2180,0x0C02,0x2180,
	0x0601,0x4180,0x0301,0x4180,0x0300,0x8180,0x0300,0x0180,
	0x0300,0x0180,0x0300,0x0180,0x0180,0x0300,0x00FF,0xFE00,
	0x007F,0xFC00,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPasteL1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPasteL1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgPasteL0" Image
*/

#include <exec/types.h>

UWORD chip imgPasteL0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0007,0xC000,
	0x0008,0x2000,0x0013,0x9000,0x0074,0x5C00,0x0087,0xC200,
	0x014C,0x6500,0x0108,0x2100,0x0207,0xC500,0x0500,0x0500,
	0x0B1F,0xF500,0x1706,0x3500,0x0B17,0x7500,0x0507,0x7500,
	0x021F,0xF500,0x0103,0xE500,0x011D,0xC500,0x010A,0x8500,
	0x0100,0x0500,0x015F,0xF900,0x0080,0x0200,0x007F,0xFC00,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0007,0xC000,0x000C,0x6000,0x000B,0xA000,0x0078,0x3C00,
	0x00B3,0x9A00,0x00F7,0xDE00,0x01F8,0x3A00,0x02FF,0xFA00,
	0x04E0,0x0A00,0x08FF,0xFA00,0x04EF,0xFA00,0x02FF,0xFA00,
	0x01E3,0xEA00,0x00FF,0xFA00,0x00E3,0xFA00,0x00F5,0xFA00,
	0x00FF,0xFA00,0x00A0,0x0600,0x007F,0xFC00,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0007,0xC000,
	0x0008,0x2000,0x0014,0x5000,0x0073,0x9C00,0x0080,0x0200,
	0x0170,0x1D00,0x0147,0xC500,0x0248,0x2100,0x0440,0x0100,
	0x085F,0xF100,0x1040,0x0100,0x0850,0x0100,0x0440,0x0100,
	0x025C,0x1100,0x0140,0x0100,0x015C,0x0100,0x014A,0x0100,
	0x0140,0x0100,0x0160,0x0500,0x0080,0x0200,0x007F,0xFC00,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0002,0x2000,0x0002,0x2000,
	0x0001,0x4000,0x0001,0x4000,0x0000,0x8000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgPasteL0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgPasteL0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
