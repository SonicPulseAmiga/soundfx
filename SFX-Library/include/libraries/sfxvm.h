/****h* sfxsupport.library/sfxvm.h [4.3] *
*
*  NAME
*    sfxvm.h
*  COPYRIGHT
*    $VER: sfxvm.h 4.3 (08.04.04) © by Stefan Kost 1998-2004
*  FUNCTION
*    virtual memory related parts of sfxsupport.h
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    15.Apr.2003
*  MODIFICATION HISTORY
*    08.Apr.2004	V 4.3	most recent version
*    15.Apr.2003	V 4.2	most recent version
*    15.Apr.2003	V 4.2	initial version
*  NOTES
*
*******
*/

#ifndef  LIBRARIES_SFXVM_H
#define  LIBRARIES_SFXVM_H

//-- Virtual Memory Stuff -------------------------------------------------------------------------

#define VM_NODATA   0
#define VM_MEMORY   1
#define VM_DRIVE    2
#define VM_STREAM   3

#define VM_MODE_READ_WRITE   1  // bit 0
#define VM_MODE_READ         0
#define VM_MODE_WRITE        1
#define VM_MODE_SEQ_RND      2  // bit 1
#define VM_MODE_SEQ          0
#define VM_MODE_RND          2
#define VM_MODE_DIR_BLK      4  // bit 2
#define VM_MODE_DIR          0                  // work directly with the file
#define VM_MODE_BLK          4                  // use async blkIO
#define VM_MODE_CBLK_VBLK    8  // bit 3
#define VM_MODE_CBLK         0                  // constant blocs
#define VM_MODE_VBLK         8                  // variable blocs
#define VM_MODE_IRQ         16  // bit 4        // for device samples, use read & writes without waits

#define VM_DEFAULT_BLKLEN   0               // use default blklen from prefs

typedef struct smpaccess {
	BYTE            *buffer1,*buffer2;          // für double buffering
	BYTE            *buffer3,*buffer4;          // für loopstartblock und für seeks in mem
	UBYTE ASM       (*safunc)(REG(a0,struct smpaccess *sa),REG(d0,ULONG length));   // sample data access function
	int             fh;
	ULONG           blen,blensh;                // segmentlength (in bytes), nr of shifts (to do the multiplication)
	ULONG           cblen;                      // current blocklength to read
	ULONG           start;                      // seekoffset (in bytes)
	ULONG           seganz;                     // how many segments
	ULONG           seg,aseg;                   // segment nr to read/write, segment nr of fileptr
	ULONG           a4,a6;
	volatile UBYTE  state;						// state of I/O task
	UBYTE			vm_mode;					// sample access mode
	BYTE            mainsig,worksig;            // for communication
	struct Process  *maintask,*worktask;
	STRPTR          fn;                         // filename
	int             file_mode;					// file access mode
	LONG            seek;                       // -1 means no seek required before next read
	int				last_error_no;
} SmpAccess;

typedef UBYTE (* ASM SAFuncPtr)(REG(a0,SmpAccess *sa),REG(d0,ULONG length));

#define STATE_READY     0
#define STATE_NOTIFY    1
#define STATE_WORKING   2
#define STATE_FINISH    3

#define VM_BUFSIZE 65536

#define CPD_NIL2MEM ((VM_NODATA<<8)|VM_MEMORY)
#define CPD_NIL2DRV ((VM_NODATA<<8)|VM_DRIVE )
#define CPD_MEM2MEM ((VM_MEMORY<<8)|VM_MEMORY)
#define CPD_MEM2DRV ((VM_MEMORY<<8)|VM_DRIVE )
#define CPD_DRV2MEM ((VM_DRIVE <<8)|VM_MEMORY)
#define CPD_DRV2DRV ((VM_DRIVE <<8)|VM_DRIVE )

#endif   /* LIBRARIES_SFXVM_H */

//-- eof ----------------------------------------------------------------------
