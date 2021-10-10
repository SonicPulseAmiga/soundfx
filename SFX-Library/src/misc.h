/****h* sfxsupport.library/misc.h [4.2] *
*
*  NAME
*    misc.h
*  COPYRIGHT
*    $VER: misc.h 4.2 (01.08.02) © by Stefan Kost 1998-2002
*  FUNCTION
*    misc routines - declarations
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    18.Sep.1998
*  MODIFICATION HISTORY
*    01.Aug.2002	V 4.2	most recent version
*    27.Aug.2000	V 4.1	most recent version
*    02.Aug.2000	V 4.00	most recent version
*    20.Sep.1998	V 1.20	most recent version
*    18.Sep.1998	V 1.20	initial version
*  NOTES
*
*******
*/

#ifndef MISC_H
#define MISC_H

//-- prototypes ---------------------------------------------------------------

#ifndef MISC_C
	//-- SampleListVerwaltung

	extern SInfo *			ASM LIB_EXPORT(GetBufferPtr(REG(d0,ULONG id),REG(d1,WORD nr)));
	extern UBYTE  			ASM LIB_EXPORT(GetBufferNr(REG(d0,ULONG id)));

	//-- Version testing

	extern ULONG			ASM LIB_EXPORT(GetSFXVersion(void));

	//-- Miscellaneous

	extern double			bound(double val,double lower,double upper);

#endif

//-- globals ------------------------------------------------------------------

#ifndef MISC_C
	extern RTime 	*RunTime;
	extern char sfxmsg[256];
	extern struct Library *AHIBase;
	extern UBYTE os_version;

	//-- Library-Basisaddrs.

	extern struct ExecBase		*SysBase;
	extern struct Library	 	*AmigaGuideBase;
	extern struct Library	 	*AslBase;
	extern struct Library	 	*ConfigIOBase;
	extern struct Library	 	*DiskfontBase;
	extern struct DosLibrary	*DOSBase;
	extern struct Library	 	*GadToolsBase;
	extern struct GfxBase	 	*GfxBase;
	extern struct IntuitionBase	*IntuitionBase;
	extern struct LocaleBase	*LocaleBase;
	extern struct Library		*UnitConvBase;

	//-- Locale

	extern struct Catalog *sfxcat;
	extern ULONG msgid2hlpid[strSoundFXMessage];

#endif

#endif /* MISC_H */

//-- eof ----------------------------------------------------------------------
