/****h* SoundFX/SFX.h [4.2] *
*
*  NAME
*    SFX.h
*  COPYRIGHT
*    $VER: SFX.h 4.2 (15.05.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    main source code for sfx - declarations
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    06.Aug.1998
*  MODIFICATION HISTORY
*    15.May.2002	V 4.2	most recent version
*    28.Nov.2000	V 4.1	most recent version
*    20.Feb.2000	V 4.0	most recent version
*    29.Jan.2000	V 3.82	most recent version
*    24.Aug.1998	V 3.71	most recent version
*    06.Aug.1998	V 3.7	initial version
*  NOTES
*
*******
*/

#ifndef SFX_H
#define SFX_H

//-- defines ------------------------------------------------------------------

#define playeranz	 4

#define MAX_ARGS 32

//-- prototypes ---------------------------------------------------------------

#ifndef SFX_C
	extern void CloseAll(UBYTE level,UBYTE return_code);
#endif

//-- globals ------------------------------------------------------------------

extern ULONG os_version;

//-- Libarybases

extern struct ExecBase 		*SysBase;
extern struct DosLibrary	*DOSBase;

#ifndef SFX_C

	//-- Libarybases

	extern struct IntuitionBase	*IntuitionBase;
	extern struct GfxBase		*GfxBase;
	extern struct RxsLib		*RexxSysBase;
	extern struct Library		*AslBase,*ConfigIOBase,*DiskfontBase,*GadToolsBase;
	extern struct Library		*IconBase,*IdentifyBase,*SFXSupportBase;
	extern struct Library		*UnitConvBase,*UtilityBase,*WorkbenchBase;
	#ifdef CHECK_DELFINA
		extern struct Library *DelfinaBase;
	#endif

	//-- OPT-Structuren

	extern OPTP_Amiga_8bit		optp_Amiga_8bit;
	extern OPTP_Amiga_14bit		optp_Amiga_14bit;
	extern OPTP_Amiga_14cbit	optp_Amiga_14cbit;
	extern OPTP_Ahi				optp_Ahi;

	//-- the shared memory structure

	extern RTime 				RunTime;

	//-- Modullisten

	extern struct List			EffektList;
	extern struct List			LoaderList;
	extern struct List			SaverList;
	extern struct List			PlayerList;
	extern struct List			RexxList;
	extern struct List			BufferList;

	//-- Multiwindow-Messageports

	extern struct MsgPort		*swin_up;
	extern struct MsgPort		*gwin_up;

	//-- Arexx-Port

	extern struct MsgPort		*arexxport;
	extern char 				*arexxname;

	//-- AppIcon-Port

	extern struct MsgPort		*appiconport;

	//-- shifted MsgPort Signals

	extern ULONG				apsig,bwsig,gwsig,swsig,rxsig;

	//-- sfxscreentitle (containing memory status)

	extern char					sfxtitle[256];

	//-- BuiltIn Modules

	extern ArrangeList pllist[playeranz];

	//-- keyfile related

	extern KeyFile Key;
	extern struct Library *SFXKeyBase,*SFXKeyBase1;
	extern UWORD keycounter;

	//-- used for error messages

	extern char sfxmsg[256];

	//-- the configuration

	extern APTR cfg;

	//-- path for loader/saver ASL requests

	extern char lopath[FILENAME_MAX],sapath[FILENAME_MAX];

	//-- for Edit/Utillities

	extern SInfo *clip,temp;

	//-- startup args (initial filesnames)

	extern ULONG num_args;
	extern STRPTR file_args[MAX_ARGS];

#endif

#endif /* SFX_H */

//-- eof ----------------------------------------------------------------------
