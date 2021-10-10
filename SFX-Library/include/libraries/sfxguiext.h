/****h* sfxsupport.library/sfxguiext.h [4.2] *
*
*  NAME
*    sfxguiext.h
*  COPYRIGHT
*    $VER: sfxguiext.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    guiext related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    19.Aug.2002
*  MODIFICATION HISTORY
*    19.Aug.2002	V 4.2	most recent version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXGUIEXT_H
#define  LIBRARIES_SFXGUIEXT_H

//-- ToolbarExtension ----------------------------------------------------------

#define nt_Pack(w,h) ((ULONG)(w<<16)|h)
#define nt_UnpackX(p) ((p&0xFFFF0000)>>16)
#define nt_UnpackY(p) (p&0x0000FFFF)

/* @todo what about event hooks ? Like e.g. :
 *   On_Mouse_Over
 *   On_Click
 */
struct NewToolbar {
	ULONG nt_Type;                  /* see below */
	struct Image *nt_Image;         /* image which is shown normally, when NULL data is interpreted different */
	union {
		struct Image *nt_HighImage;     /* image which is shown when mouse pointer is over it */
		ULONG nt_Width;                 /* width of space */
		ULONG nt_Pos;                   /* pos of toolbar */
	};
	union {
		struct Image *nt_ActiveImage;   /* image which is shown when mouse button is down */
		ULONG nt_Height;                /* height of space */
		ULONG nt_Size;                  /* size of toolbar */
	};
	STRPTR nt_Label;                /* toolbar's label */
	ULONG nt_Border;                /* see below */
	ULONG nt_GadgetID;              /* the id you'll get via intuimessage*/
	struct Gadget *nt_Gadget;       /* Gadget-pointers are entered here */
};

/* nt_Type determines what each NewToolbar structure corresponds to.
 */
#define NT_TOOLBAR      1       /* Toolbar header */
#define NT_ITEM         2       /* Toolbar item */
#define NT_SUB          3       /* Toolbar sub-item */

/* The NewToolbar array should be terminated with a NewToolbar whose
 * nt_Type equals NT_END.
 */
#define NT_END          0       /* End of NewToolbar array */

#define NTB_NONE        0
#define NTB_RAISED      1
#define NTB_SUNKEN      2
#define NTB_RIDGE       3
#define NTB_GROOVE      4

//-- MousePointerExtension -----------------------------------------------------

typedef struct {
	APTR            Pointer;
	struct BitMap   ptrbm;
	UWORD           ptrdata[256];
} MPointer;

#define MPTR_STANDARD		0
#define MPTR_CROSSHAIR		1
#define MPTR_MOVE			2
#define MPTR_RANGE			3
#define MPTR_SIZE64			4
#define MPTR_SIZE73			5
#define MPTR_SIZE82			6
#define MPTR_SIZE91			7

#endif   /* LIBRARIES_SFXGUIEXT_H */

//-- eof -----------------------------------------------------------------------
