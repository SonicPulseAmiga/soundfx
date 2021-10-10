/****h* debugtools/sos_stdc_decl.h [1.20] *
*
*  NAME
*    sos_stdc_decl.h
*  COPYRIGHT
*    $VER: sos_stdc_decl.h 1.20 (02.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    stdc-function wrapper-decls
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    04.Mai.1998
*  MODIFICATION HISTORY
*    02.Aug.1998	V 1.20	first release
*    04.Mai.1998	V 1.00	initial version
*  NOTES
*
*******
*/

#ifdef DEBUG
	#ifndef DEBUG_DEFS
		extern int SOS_remove(const char *name,char *file,unsigned long line);
		#define remove(name) SOS_remove(name,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __remove(name) SOS_remove(name,NULL,0)

		extern int SOS_rename(const char *old, const char *new,char *file,unsigned long line);
		#define rename(old,new) SOS_rename(old,new,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __rename(old,new) SOS_rename(old,new,NULL,0)

		extern FILE *SOS_tmpfile(char *file,unsigned long line);
		#define tmpfile() SOS_tmpfile(DEBUG_SRCPATH __FILE__,__LINE__)
		#define __tmpfile() SOS_tmpfile(,NULL,0)

		extern int SOS_fclose(FILE *fp,char *file,unsigned long line);
		#define fclose(fp) SOS_fclose(fp,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __fclose(fp) SOS_fclose(fp,NULL,0)

		extern int SOS_fflush(FILE *,char *file,unsigned long line);
		#define fflush(fp) SOS_fflush(fp,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __fflush(fp) SOS_fflush(fp,NULL,0)

		extern FILE *SOS_fopen(const char *name,const char *mode,char *file,unsigned long line);
		#define fopen(name,mode) SOS_fopen(name,mode,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __fopen(name,mode) SOS_fopen(name,mode,NULL,0)

		extern FILE *SOS_freopen(const char *name, const char *mode, FILE *fp,char *file,unsigned long line);
		#define freopen(name,mode,fp) SOS_freopen(name,mode,fp,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __freopen(name,mode,fp) SOS_freopen(name,mode,fp,NULL,0)

	#endif
#else
	#define __remove(name)			remove(name)
	#define __rename(old,new)		rename(old,new)
	#define __fclose(fp)			fclose(fp)
	#define __fflush(fp)			fflush(fp)
	#define __fopen(name,mode)		fopen(name,mode)
#endif

//-- eof ----------------------------------------------------------------------
