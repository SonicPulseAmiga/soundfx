/****h* debugtools/sos_math_decl.h [1.20] *
*
*  NAME
*    sos_math_decl.h
*  COPYRIGHT
*    $VER: sos_math_decl.h 1.20 (02.08.98) © by Stefan Kost 1998-1998
*  FUNCTION
*    math-function wrapper-decls
*  AUTHOR
*    Stefan Kost
*  CREATION DATE
*    15.Feb.1998
*  MODIFICATION HISTORY
*    02.Aug.1998	V 1.20	first release
*    15.Feb.1998	V 0.00	initial version
*  NOTES
*
*******
*/

#ifdef DEBUG
	#ifndef DEBUG_DEFS
		#ifdef _M68881
			#undef acos
			#undef asin
			#undef atan
			#undef cos
			#undef cosh
			#undef exp
			#undef fabs
			#undef log
			#undef log10
			#undef sin
			#undef sinh
			#undef sqrt
			#undef tan
			#undef tanh

			#undef fintrz
			#undef flognp1
			#undef fetoxm1
			#undef fatanh
			#undef fetox
			#undef ftwotox
			#undef ftentox
			#undef flogn
			#undef flog2
			#undef fneg
			#undef fgetman
			#undef fgetexp
		#endif
		#ifdef _FFP
			#undef acos
			#undef asin
			#undef atan
			#undef cos
			#undef cosh
			#undef exp
			#undef fabs
			#undef floor
			#undef log
			#undef log10
			#undef sin
			#undef sinh
			#undef sqrt
			#undef tan
			#undef tanh

			#undef ceil
			#undef sincos
			#undef pow
		#endif
		#ifdef _IEEE
			#undef acos
			#undef asin
			#undef atan
			#undef cos
			#undef cosh
			#undef exp
			#undef fabs
			#undef floor
			#undef log
			#undef log10
			#undef sin
			#undef sinh
			#undef sqrt
			#undef tan
			#undef tanh

			#undef ceil
			#undef sincos
			#undef pow
		#endif

		extern double SOS_acos(double x,char *file,unsigned long line);
		#define acos(x) SOS_acos(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __acos(x) SOS_acos(x,NULL,0)

		extern double SOS_asin(double x,char *file,unsigned long line);
		#define asin(x) SOS_asin(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __asin(x) SOS_asin(x,NULL,0)

		extern double SOS_atan(double x,char *file,unsigned long line);
		#define atan(x) SOS_atan(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __atan(x) SOS_atan(x,NULL,0)

		extern double SOS_atan2(double x,double y,char *file,unsigned long line);
		#define atan2(x,y) SOS_atan2(x,y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __atan2(x,y) SOS_atan2(x,y,NULL,0)

		extern double SOS_ceil(double x,char *file,unsigned long line);
		#define ceil(x) SOS_ceil(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ceil(x) SOS_ceil(x,NULL,0)

		extern double SOS_cos(double x,char *file,unsigned long line);
		#define cos(x) SOS_cos(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __cos(x) SOS_cos(x,NULL,0)

		extern double SOS_cosh(double x,char *file,unsigned long line);
		#define cosh(x) SOS_cosh(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __cosh(x) SOS_cosh(x,NULL,0)

		extern double SOS_cot(double x,char *file,unsigned long line);
		#define cot(x) SOS_cot(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __cot(x) SOS_cot(x,NULL,0)

		extern double SOS_coth(double x,char *file,unsigned long line);
		#define coth(x) SOS_coth(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __coth(x) SOS_coth(x,NULL,0)

		extern double SOS_exp(double x,char *file,unsigned long line);
		#define exp(x) SOS_exp(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __exp(x) SOS_exp(x,NULL,0)

		extern double SOS_fabs(double x,char *file,unsigned long line);
		#define fabs(x) SOS_fabs(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __fabs(x) SOS_fabs(x,NULL,0)

		extern double SOS_floor(double x,char *file,unsigned long line);
		#define floor(x) SOS_floor(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __floor(x) SOS_floor(x,NULL,0)

		extern double SOS_fmod(double y,double z,char *file,unsigned long line);
		#define fmod(y,z) SOS_fmod(y,z,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __fmod(y,z) SOS_fmod(y,z,NULL,0)

		extern double SOS_frexp(double v,int *xp,char *file,unsigned long line);
		#define frexp(v,xp) SOS_frexp(v,xp,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __frexp(v,xp) SOS_frexp(v,xp,NULL,0)

		extern double SOS_ldexp(double d,int x,char *file,unsigned long line);
		#define ldexp(d,x) SOS_ldexp(d,x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __ldexp(d,x) SOS_ldexp(d,x,NULL,0)

		extern double SOS_log(double x,char *file,unsigned long line);
		#define log(x) SOS_log(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __log(x) SOS_log(x,NULL,0)

		extern double SOS_log10(double x,char *file,unsigned long line);
		#define log10(x) SOS_log10(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __log10(x) SOS_log10(x,NULL,0)

		extern double SOS_pow(double x,double y,char *file,unsigned long line);
		#define pow(x,y) SOS_pow(x,y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __pow(x,y) SOS_pow(x,y,NULL,0)

		extern double SOS_pow2(double x,char *file,unsigned long line);
		#define pow2(x) SOS_pow2(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __pow2(x) SOS_pow2(x,NULL,0)

		extern double SOS_sin(double x,char *file,unsigned long line);
		#define sin(x) SOS_sin(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __sin(x) SOS_sin(x,NULL,0)

		extern double SOS_sinh(double x,char *file,unsigned long line);
		#define sinh(x) SOS_sinh(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __sinh(x) SOS_sinh(x,NULL,0)

		extern double SOS_sqrt(double x,char *file,unsigned long line);
		#define sqrt(x) SOS_sqrt(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __sqrt(x) SOS_sqrt(x,NULL,0)

		extern double SOS_tan(double x,char *file,unsigned long line);
		#define tan(x) SOS_tan(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __tan(x) SOS_tan(x,NULL,0)

		extern double SOS_tanh(double x,char *file,unsigned long line);
		#define tanh(x) SOS_tanh(x,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __tanh(x) SOS_tanh(x,NULL,0)

		// additional routines

		extern double SOS_add_dbl(double x,double y,char *file,unsigned long line);
		#define add_dbl(x,y) SOS_add_dbl((double)x,(double)y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __add_dbl(x,y) SOS_add_dbl((double)x,(double)y,NULL,0)

		extern double SOS_sub_dbl(double x,double y,char *file,unsigned long line);
		#define sub_dbl(x,y) SOS_sub_dbl((double)x,(double)y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __sub_dbl(x,y) SOS_sub_dbl((double)x,(double)y,NULL,0)

		extern double SOS_mul_dbl(double x,double y,char *file,unsigned long line);
		#define mul_dbl(x,y) SOS_mul_dbl((double)x,(double)y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __mul_dbl(x,y) SOS_mul_dbl((double)x,(double)y,NULL,0)

		extern double SOS_div_dbl(double x,double y,char *file,unsigned long line);
		#define div_dbl(x,y) SOS_div_dbl((double)x,(double)y,DEBUG_SRCPATH __FILE__,__LINE__)
		#define __div_dbl(x,y) SOS_div_dbl((double)x,(double)y,NULL,0)
	#endif
#else

	#define add_dbl(x,y) ((x)+(y))
	#define sub_dbl(x,y) ((x)-(y))
	#define mul_dbl(x,y) ((x)*(y))
	#define div_dbl(x,y) ((x)/(y))

	#define __acos(x)		acos(x)
	#define __asin(x)		asin(x)
	#define __atan(x)		atan(x)
	#define __atan2(x,y) 	atan2(x,y)
	#define __ceil(x)	 	ceil(x)
	#define __cos(x) 		cos(x)
	#define __cosh(x) 		cosh(x)
	#define __cot(x) 		cot(x)
	#define __coth(x) 		coth(x)
	#define __exp(x) 		exp(x)
	#define __fabs(x) 		fabs(x)
	#define __floor(x)	 	floor(x)
	#define __fmod(y,z) 	fmod(y,z)
	#define __frexp(v,xp) 	frexp(v,xp)
	#define __ldexp(d,x) 	ldexp(d,x)
	#define __log(x) 		log(x)
	#define __log10(x) 		log10(x)
	#define __pow(x,y) 		pow(x,y)
	#define __pow2(x)	 	pow2(x)
	#define __sin(x) 		sin(x)
	#define __sinh(x) 		sinh(x)
	#define __sqrt(x)	 	sqrt(x)
	#define __tan(x) 		tan(x)
	#define __tanh(x)	 	tanh(x)
	#define __add_dbl(x,y) 	add_dbl((double)x,(double)y)
	#define __sub_dbl(x,y) 	sub_dbl((double)x,(double)y)
	#define __mul_dbl(x,y) 	mul_dbl((double)x,(double)y)
	#define __div_dbl(x,y) 	div_dbl((double)x,(double)y)
#endif

//-- eof ----------------------------------------------------------------------
