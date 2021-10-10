/****h* cmacros/cmacros.h [0.00] *
*
*  NAME
*    cmacros.h
*  COPYRIGHT
*    $VER: cmacros.h 0.00 (01.08.00) © by Stefan Kost 1998-2000
*  FUNCTION
*    Collection of useful C-macros
*  AUTHOR
*    Stefan Kost			sk
*      * main author
*    Harald Pehl			hp
*      * more ESC-Codes
*      * tip to enclose all macro-parameters in brackets
*  CREATION DATE
*    07.Aug.1998
*  MODIFICATION HISTORY
*    01.Aug.2000	V 2.20	most recent version
*                           * more keycodes
*    06.Mar.1999	V 2.10	most recent version
*							* additional ansi sequences (hp)
*							* bugfixes (hp)
*                           * size-macros (sk)
*                           * more raw-keycodes
*    07.Aug.1998	V 2.00	extended version
*    03.Jul.1995	V 1.00	initial version
*  NOTES
*   Installation :                                                          
*     just copy it to include:                                              
*   Future :                                                                
*     if you know some more macros, you are using often, please mail them to
*     me (kost@imn.htwk-leipzig.de)                                         
*   Comment :                                                               
*     read with tabs=4                                                      
*******
*/

/*-- Bitmanipulation Macros -------------------------------------------------*/
/* n ist the variable to work with                                           */
/* m ist the bitposition to change                                           */
/* BitClr(11,0) => 10 ; this clears the lowest bit in the number 11          */

#define BitSet(n,m)			( (n)|(1L<<(m)) )
#define BitClr(n,m)			( (n)&~(1L<<(m)) )
#define BitTest(n,m)		( (n)&(1L<<(m))>>(m) )
#define BitToggle(n,m)		( (n)^(1L<<(m)) )

/*-- Size Macros ------------------------------------------------------------*/
/* calculates how many bytes a certain number of bits would require          */
/* n ist the number of bits                                                  */
/* BytesSize(5) => 1 , you need one Byte to store 5 Bits                     */

#define ByteSize(n)			(((n)+ 7)>>3)
#define WordSize(n)			(((n)+15)>>4)
#define LongSize(n)			(((n)+31)>>5)

/*-- Variable Swapping ------------------------------------------------------*/
/* swaps the contents of variable a with b and uses c as buffer              */
/* all variables should have the same typ and should be lvalues              */

/*#define Swap(a,b,c)			c=a;a=b;b=c;*/

/*-- Variable Swapping ------------------------------------------------------*/
/* swaps the contents of variable a with b without a buffer - cool hehe ;-)  */
/* both variables should have the same typ (interger only)                   */
/* and should be lvalues                                                     */

#define Swap(a,b)			do{ a^=b;b^=a;a^=b; } while(0)

/*-- Rangechecking ----------------------------------------------------------*/
/* checks if a is larger than lo and smaller than hi, returns value which    */
/* is within bounds                                                          */
/* RangeX : bounds are excluded, RangeI : bounds are included                */
/* Parameters should be variables, expressions are calculated multiple times */
/* and may cause sideeffects (do NOT use parameters like a++ etc.)           */

#define RangeX(a,lo,hi)		( ((a)>=(hi) ? ((hi)-1) : (a))<=(lo) ? ((lo)-1) : ((a)>=(hi) ? ((hi)-1) : (a)) )
#define RangeI(a,lo,hi)		( ((a)>=(hi) ? (hi) : (a))<=(lo) ? (lo) : ((a)>=(hi) ? (hi) : (a)) )

/*-- Odd/Even-Check ---------------------------------------------------------*/
/* simply check if a given number is odd or even                             */

#define Odd(a)				( (a)&1L )
#define Even(a)				( !((a)&1L) )

/*-- Endianconversion -------------------------------------------------------*/
/* converts between motorola`s and intel`s (buuuh) number format             */

#define LitEnd2BigEnd_16(w)	( ((w)&0xFF)<< 8 | ((w)&0xFF00)>>8 )
#define LitEnd2BigEnd_32(l)	( ((l)&0xFF)<<24 | ((l)&0xFF00)<<8 | ((l)&0xFF0000)>>8 | ((l)&0xFF000000)>>24 )

/*-- IFF-Handling -----------------------------------------------------------*/
/* generates chunk-ID`s for iff-files                                        */

#define MakeID(a,b,c,d)		( (LONG)(a)<<24L | (LONG)(b)<<16L | (c)<<8 | (d) )

/*-- Linear Bleanding -------------------------------------------------------*/
/* Blend(0.25,5.0,50.0) returns a value which equates 25 % between 5 and 50  */

#define Blend(s,lo,hi)		( (lo)+(s)*((hi)-(lo)) )

/*-- Converts a number which is a #define to a string -----------------------*/
/* Example                                                                   */
/*  #define VERSION 3                                                        */
/*  #define VSTRING "MyProg V "NUM2STR(VERSION)" (C) by myself"              */

#define NUM2STR(a) NUM2STR_SUB(a)
#define NUM2STR_SUB(a) #a

/*-- Generates strings for cpu/fpu settings ---------------------------------*/
/* you can then include the CPU/FPU name in the version-string of your prog. */
/* e.g. #define VSTRING "<...> compiled for "CPUNAME"/"FPUNAME               */

#ifdef _CPUNAME
	#undef _CPUNAME
#endif
#ifdef _M68060
	#define _CPUNAME "68060"
#else
	#ifdef _M68040
		#define _CPUNAME "68040"
	#else
		#ifdef _M68030
			#define _CPUNAME "68030"
		#else
			#ifdef _M68020
				#define _CPUNAME "68020"
			#else
				#ifdef _M68010
					#define _CPUNAME "68010"
				#else
					#ifdef _M68000
						#define _CPUNAME "68000"
					#else
						#define _CPUNAME "-----"
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef _FPUNAME
	#undef _FPUNAME
#endif
#ifdef _M68881
	/* FPU */
	#define _FPUNAME "FPU"
#else
	#ifdef _FFP
		/* FFP */
		#define _FPUNAME "FFP"
	#else
		#ifdef _IEEE
			/* IEEE */
			#define _FPUNAME "IEEE"
		#else
			/* Std */
			#define _FPUNAME "Std."
		#endif
	#endif
#endif

/*-- Generates a Color32 Tag ------------------------------------------------*/

#define MakeCol32(a) ((a)<<24L | (a)<<16L | (a)<<8L | (a))

/*-- Makes a number to be a multiple of something ---------------------------*/
/* e.g. MultipleOf(val,3) makes val to be a multiple of 3                    */

#define MultipleOf(a,b) (((ULONG)((b)/(a)))*(a))

/*-- Ansi-X3.64 ESC-Commandsequences ----------------------------------------*/
/* use these for nicer console-output, e.g.                                  */
/* printf(BOLD_ON"SuperCopy"BOLD_OFF" (C) by "FGCOL(3)"myself"FGCOL(1));     */
/* the compiler will (should) cat the strings together automatically         */

#define ANSI_RESET		"\033c"
#define ANSI_GFXRESET	"\033[0m"

#define BOLD_ON			"\033[1m"
#define BOLD_OFF		"\033[22m"

#define ITALIC_ON		"\033[3m"
#define ITALIC_OFF		"\033[23m"

#define UNDELINE_ON		"\033[4m"
#define UNDERLINE_OFF	"\033[24m"

#define INVERSE_ON		"\033[7m"
#define INVERSE_OFF		"\033[27m"

#define TEXT_BGCOL		"\033[8m"
#define TEXT_FGCOL		"\033[28m"

#define FGCOL(n)		"\033[3"NUM2STR(n)"m"				// n=0...7  (n=9 -> default color=1)
#define BGCOL(n)		"\033[4"NUM2STR(n)"m"				// n=0...7  (n=9 -> default color=1)

#define CURSOR_TO_BOL	"\001"
#define CURSOR_TO_EOL	"\032"

#define CLR_SCR			"\014"
#define CLR_LINE		"\002"
#define CLR_TO_END		"\013"
#define BACKSPACE		"\010"

#define BEEP			"\007"

#define SET_XRES(n)		"\033["NUM2STR(n)"u"
#define SET_YRES(n)		"\033["NUM2STR(n)"t"

#define SET_XPOS(n)		"\033["NUM2STR(n)"x"
#define SET_YPOS(n)		"\033["NUM2STR(n)"y"

#define INSERT_SPACE    "\033[@"

#define CURSOR_UP       "\033[#A"
#define CURSOR_DOWN     "\033[#B"
#define CURSOR_RIGHT    "\033[#C"
#define CURSOR_LEFT     "\033[#D"

#define LINE_UP         "\033[#F"
#define LINE_DOWN       "\033[#E"
#define DEL_EOL         "\033[#K"
#define INSERT_LINE     "\033[#L"
#define DEL_LINE        "\033[#M"

#define SCROLL_UP       "\033[#S"
#define SCROLL_DOWN     "\033[#T"

#ifdef __cplusplus
#include <stream.h>
#define MOVE(x,y)       cout << "\033[" << x << "x" << "\033[" << y << "y"
#define LOCATE(s,z)     cout << "\033[" << s << ";" << z << "H"
#define SET_A_PEN(a)    cout << "\033[3" << a+4 << "m"
#define SET_B_PEN(b)    cout << "\033[4" << b+4 << "m"
#else
#include <stdio.h>
#define MOVE(x,y)       printf("\033[%dx\033[%dy", x, y)
#define LOCATE(s,z)     printf("\033[%d;%dH", s, z)
#define SET_A_PEN(a)    printf("\033[3%dm", a+4)
#define SET_B_PEN(b)    printf("\033[4%dm", b+4)
#endif


/*-- Listsupport ------------------------------------------------------------*/
/* checks if a given node is the end of a list, by checking list->lh_Tail,   */
/* which always points to 0l                                                 */

#define EndOfList(list,node) ((node)==(struct Node *)(&((list)->lh_Tail)))
//#define EndOfList(list,node) (!(ULONG)(*(node)))

/*-- Keyconstants -----------------------------------------------------------*/
/* make your code better readable by using these constants                   */
/* KEY_#?  are constants which would match VanillaKey results                */
/* RKEY_#? are constants which would match RawKey results                    */

#define KEY_BACKSPACE		0x08
#define KEY_TAB				0x09
#define KEY_ENTER			0x0A
#define KEY_RETURN			0x0D
#define KEY_ESC				0x1B
#define KEY_SPACE			0x20
#define KEY_0				0x30
#define KEY_1				0x31
#define KEY_2				0x32
#define KEY_3				0x33
#define KEY_4				0x34
#define KEY_5				0x35
#define KEY_6				0x36
#define KEY_7				0x37
#define KEY_8				0x38
#define KEY_9				0x39
#define KEY_DEL				0x7F

#define RKEY_SPACE			0x40
#define RKEY_BACKSPACE		0x41
#define RKEY_TAB			0x42
#define RKEY_ENTER			0x43
#define RKEY_RETURN			0x44
#define RKEY_ESC			0x45
#define RKEY_DEL			0x46
#define RKEY_CURSORUP		0x4C
#define RKEY_CURSORDOWN		0x4D
#define RKEY_CURSORLEFT		0x4E
#define RKEY_CURSORRIGHT	0x4F
#define RKEY_F1				0x50
#define RKEY_F2				0x51
#define RKEY_F3				0x52
#define RKEY_F4				0x53
#define RKEY_F5				0x54
#define RKEY_F6				0x55
#define RKEY_F7				0x56
#define RKEY_F8				0x57
#define RKEY_F9				0x58
#define RKEY_F10			0x59
#define RKEY_HELP			0x5F
#define RKEY_LSHIFT			0x60
#define RKEY_RSHIFT			0x61
#define RKEY_CTRL			0x62
#define RKEY_CAPSLOCK		0x63
#define RKEY_LALT			0x64
#define RKEY_RALT			0x65
#define RKEY_LAMIGA			0x66
#define RKEY_RAMIGA			0x67
#define RKEY_NUM0			0x0F
#define RKEY_NUM1			0x1D
#define RKEY_NUM2			0x1E
#define RKEY_NUM3			0x1F
#define RKEY_NUM4			0x2D
#define RKEY_NUM5			0x2E
#define RKEY_NUM6			0x2F
#define RKEY_NUM7			0x3D
#define RKEY_NUM8			0x3E
#define RKEY_NUM9			0x3F

/*-- Portabillity -----------------------------------------------------------*/
/* using these constants helps you to write more portable code               */
/* please help me with other compilers here (I am using the SAS C/C++)       */
/* with information from                                                     */
/*   Bernardo Innocenti's CompilerSpecific.h 2.3 (26.10.97)                  */

#ifdef _DCC
	#define ALIGNED		__aligned
	#define ASM
	#define CHIP
	#define FAR
	#define	HOOK		__geta4
	#define INLINE
	#define INTERRUPT
	#define	LIBFUNC		__geta4
	#define NEAR
	#define REGARGS
	#define REG(x)		__## x
	#define SAVEDS		__geta4
	#define STACKEXT
	#define STDARGS		__stdargs
#else
#if defined(_GCC) || defined(__GNUC__)
	#define ALIGNED		__attribute__((aligned(4)))
	#define ASM
	#define CHIP
	#define FAR
	#define	HOOK		__attribute__((saveds))
	#define INLINE		inline
	#define INTERRUPT	__attribute__((interrupt))
	#define	LIBFUNC		__attribute__((saveds))
	#define NEAR
	#define REGARGS
	#define REG(reg)	__asm(#reg)
	#define SAVEDS 		__attribute__((saveds))
	#define STACKEXT
	#define STDARGS		__attribute__((stkparm))
#else
#ifdef __MAXON__
	#define ALIGNED
	#define ASM
	#define CHIP
	#define FAR
	#define HOOK
	#define INLINE		inline
	#define INTERRUPT
	#define LIBFUNC
	#define NEAR
	#define REG(x)		register __## x
	#define REGARGS
	#define SAVEDS
	#define STACKEXT
	#define STDARGS
#else
#ifdef __STORM__
	#define ALIGNED
	#define ASM
	#define CHIP
	#define FAR
	#define	HOOK		__saveds
	#define INLINE		__inline
	#define INTERRUPT	__interrupt
	#define	LIBFUNC		__saveds
	#define NEAR
	#define REGARGS		register
	#define REG(x)		register __## x
	#define SAVEDS		__saveds
	#define STACKEXT
	#define STDARGS
#else
#ifdef AZTEC_C
	#define ALIGNED		__aligned
	#define ASM
	#define CHIP
	#define FAR
	#define HOOK		__geta4
	#define INLINE
	#define INTERRUPT
	#define LIBFUNC
	#define NEAR
	#define REGARGS
	#define REG(x)		__## x
	#define SAVEDS		__geta4
	#define STACKEXT
	#define STDARGS
#else
#ifdef __SASC
	#define ALIGNED		__aligned
	#define ASM			__asm
	#define	CHIP		__chip
	#define	FAR			__far
	#define	HOOK		__asm __saveds
	#ifndef __cplusplus
		#define	INLINE	__inline
	#else
		#define	INLINE	inline
	#endif
	#define	INTERRUPT	__interrrupt
	#define	LIBFUNC		__asm __saveds
	#define	NEAR		__near
	#define	REGARGS		__regargs
	#define REG(x)		register __## x
	#define SAVEDS		__saveds
	#define	STACKEXT	__stackext
	#define	STDARGS		__stdargs
#else
	#define ALIGNED
	#define ASM
	#define CHIP
	#define FAR
	#define HOOK
	#define INLINE
	#define INTERRUPT
	#define LIBFUNC
	#define NEAR
	#define REGARGS
	#define REG(x)
	#define SAVEDS
	#define STACKEXT
	#define STDARGS
#endif
#endif
#endif
#endif
#endif
#endif

/*-- eof --------------------------------------------------------------------*/
