/*
	Data for "imgEraseL2" Image
*/

#include <exec/types.h>

UWORD chip imgEraseL2Data[216] =
{
	/* Plane 0 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x3C40,
	0x8000,0x4240,0x8000,0x8140,0x8001,0x18C0,0x8002,0x3CC0,
	0x8104,0x64C0,0x8288,0xC4C0,0x8451,0x8CC0,0x8923,0xF8C0,
	0x9306,0x3140,0xA70F,0xE240,0x9318,0xC440,0x8931,0x8840,
	0x8463,0x1040,0x8246,0x2040,0x824C,0x4040,0x8278,0x8040,
	0x8231,0x0040,0x8102,0x0040,0x8084,0x0040,0x8078,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x4000,0x3C20,0x4000,0x6620,0x4000,0xC220,
	0x4001,0x9A20,0x4003,0x3A20,0x4106,0x7220,0x428C,0x0620,
	0x4499,0xCC20,0x48B0,0x1820,0x44E7,0x3020,0x42CE,0x6020,
	0x419C,0xC020,0x40B9,0x8020,0x40B3,0x0020,0x4086,0x0020,
	0x40CC,0x0020,0x4078,0x0020,0x4000,0x0020,0x4000,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4000,0x3C40,
	0x4000,0x7E40,0x4000,0xC340,0x4001,0xA5C0,0x4003,0x41C0,
	0x4106,0x81C0,0x438D,0x01C0,0x46DA,0x01C0,0x4C74,0x05C0,
	0x586A,0x2B40,0x7050,0x1640,0x5828,0xAC40,0x4C51,0x5840,
	0x4622,0xB040,0x4305,0x6040,0x430A,0xC040,0x4335,0x8040,
	0x434B,0x0040,0x4186,0x0040,0x40FC,0x0040,0x4078,0x0040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x3C00,
	0x0000,0x7E00,0x0000,0xC300,0x0001,0x8180,0x0003,0x1980,
	0x0106,0x3980,0x038C,0x7980,0x06D8,0xF980,0x0C70,0x0180,
	0x1860,0x0300,0x3040,0x0600,0x1800,0x0C00,0x0C00,0x1800,
	0x0600,0x3000,0x0300,0x6000,0x0300,0xC000,0x0301,0x8000,
	0x0303,0x0000,0x0186,0x0000,0x00FC,0x0000,0x0078,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseL2 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseL2Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgEraseL1" Image
*/

#include <exec/types.h>

UWORD chip imgEraseL1Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x7FFF,0xFFE0,0x4000,0x0020,0x4000,0x3C20,
	0x4000,0x4220,0x4000,0x8120,0x4001,0x18A0,0x4002,0x3CA0,
	0x4104,0x64A0,0x4288,0xC4A0,0x4451,0x8CA0,0x4923,0xF8A0,
	0x5306,0x3120,0x670F,0xE220,0x5318,0xC420,0x4931,0x8820,
	0x4463,0x1020,0x4246,0x2020,0x424C,0x4020,0x4278,0x8020,
	0x4231,0x0020,0x4102,0x0020,0x4084,0x0020,0x4078,0x0020,
	0x4000,0x0020,0x0000,0x0020,0xFFFF,0xFFE0,
	/* Plane 1 */
	0xFFFF,0xFFE0,0x8000,0x0000,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0x8000,0x3C40,0x8000,0x6640,0x8000,0xC240,
	0x8001,0x9A40,0x8003,0x3A40,0x8106,0x7240,0x828C,0x0640,
	0x8499,0xCC40,0x88B0,0x1840,0x84E7,0x3040,0x82CE,0x6040,
	0x819C,0xC040,0x80B9,0x8040,0x80B3,0x0040,0x8086,0x0040,
	0x80CC,0x0040,0x8078,0x0040,0x8000,0x0040,0x8000,0x0040,
	0x8000,0x0040,0xFFFF,0xFFC0,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x7FFF,0xFFC0,0x4000,0x0040,0x4000,0x3C40,
	0x4000,0x7E40,0x4000,0xC340,0x4001,0xA5C0,0x4003,0x41C0,
	0x4106,0x81C0,0x438D,0x01C0,0x46DA,0x01C0,0x4C74,0x05C0,
	0x586A,0x2B40,0x7050,0x1640,0x5828,0xAC40,0x4C51,0x5840,
	0x4622,0xB040,0x4305,0x6040,0x430A,0xC040,0x4335,0x8040,
	0x434B,0x0040,0x4186,0x0040,0x40FC,0x0040,0x4078,0x0040,
	0x4000,0x0040,0x7FFF,0xFFC0,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x3C00,
	0x0000,0x7E00,0x0000,0xC300,0x0001,0x8180,0x0003,0x1980,
	0x0106,0x3980,0x038C,0x7980,0x06D8,0xF980,0x0C70,0x0180,
	0x1860,0x0300,0x3040,0x0600,0x1800,0x0C00,0x0C00,0x1800,
	0x0600,0x3000,0x0300,0x6000,0x0300,0xC000,0x0301,0x8000,
	0x0303,0x0000,0x0186,0x0000,0x00FC,0x0000,0x0078,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseL1 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseL1Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
/*
	Data for "imgEraseL0" Image
*/

#include <exec/types.h>

UWORD chip imgEraseL0Data[216] =
{
	/* Plane 0 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x3C00,0x0000,0x4200,0x0000,0x9900,0x0001,0x3D00,
	0x0002,0x6500,0x0104,0xC500,0x0289,0x8D00,0x0553,0xF900,
	0x0B66,0x3200,0x174F,0xE400,0x0B18,0xC800,0x0531,0x9000,
	0x0263,0x2000,0x0146,0x4000,0x014C,0x8000,0x0179,0x0000,
	0x0132,0x0000,0x0084,0x0000,0x0078,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 1 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x3C00,0x0000,0x6600,0x0000,0xC200,
	0x0001,0x9A00,0x0003,0x3A00,0x0106,0x7200,0x028C,0x0600,
	0x0499,0xCC00,0x08B0,0x1800,0x04E7,0x3000,0x02CE,0x6000,
	0x019C,0xC000,0x00B9,0x8000,0x00B3,0x0000,0x0086,0x0000,
	0x00CC,0x0000,0x0078,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 2 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x3C00,0x0000,0x4200,0x0000,0xA500,0x0001,0x4100,
	0x0002,0x8100,0x0105,0x0100,0x028A,0x0100,0x0454,0x0500,
	0x086A,0x2A00,0x1050,0x1400,0x0828,0xA800,0x0451,0x5000,
	0x0222,0xA000,0x0105,0x4000,0x010A,0x8000,0x0135,0x0000,
	0x014A,0x0000,0x0084,0x0000,0x0078,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	/* Plane 3 */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1800,
	0x0000,0x3800,0x0000,0x7800,0x0000,0xF800,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

struct Image imgEraseL0 =
{
	0, 0,		/* LeftEdge, TopEdge */
	27, 27, 4,	/* Width, Height, Depth */
	imgEraseL0Data,	/* ImageData */
	0x000F, 0x0000,	/* PlanePick, PlaneOnOff */
	NULL		/* NextImage */
};
