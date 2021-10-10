/****h* sfxsupport.library/sfxprowin.h [4.2] *
*
*  NAME
*    sfxprowin.h
*  COPYRIGHT
*    $VER: sfxprowin.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    progressbar window related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    15.Apr.2003
*  MODIFICATION HISTORY
*    15.Apr.2003	V 4.2	most recent version
*    15.Apr.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXPROWIN_H
#define  LIBRARIES_SFXPROWIN_H

//-- ProgressWindow -------------------------------------------------------------------------------

typedef char ProWinTitle[80];

typedef struct {
	/*--------------------------------- for the GUI */
	struct Window   *pw;                    /* window */
	struct Gadget   *proGList;              /* gadget list */
	STRPTR          title;                  /* window title */
	/*--------------------------------- translation factors */
	double          ptrs;                   /* factor to get bar-pos    from curlen */
	double          prozh;                  /* factor to get percentage from curlen */
	/*--------------------------------- positions of bar */
	double          ptr;                    /* starts at the left side and tells how far we are */
	UWORD           optr;                   /* last pos of ptr, so we can rectfill from optr to ptr */
	UWORD           gy1,gy2;                /* y-pos for bar */
	UWORD           gx1;                    /* x-pos for bar */
	/*--------------------------------- positions of percentage text */
	UWORD           tx,ty;                  /* text x,y-pos */
	char            pstr[6];                /* textpuffer for percentage */
	/*--------------------------------- how far are we */
	ULONG           maxlen;                 /* equals to 100 % */
	ULONG 			*curlen;                /* how far we are */
	/*--------------------------------- for asynchronous communication */
	ULONG           a4,a6;                  /* for data access */
	ULONG           mainsig,worksig;        /* syncronisation signals */
	struct Process  *maintask,*worktask;
	volatile BYTE  status;
	/*--------------------------------- for quiet,emedded */
	UBYTE           type;
} PWData;

#define PWT_STD 0
#define PWT_EMB 1
#define PWT_HID 2

//-- progressbar status

#define ProWinEnd               0xFFFFFFD0
#define ProWinAborted(pos)      (pos>=ProWinEnd)
#define ProWinRunning(pos)      (pos< ProWinEnd)

#endif   /* LIBRARIES_SFXPROWIN_H */

//-- eof ----------------------------------------------------------------------
