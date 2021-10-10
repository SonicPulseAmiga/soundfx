#ifndef  LIBRARIES_UNITCONV_H
#define  LIBRARIES_UNITCONV_H

/*
**	$VER: unitconv.h 1.0 (05.05.98)
**
**	(C) Copyright 1993-1998 Stefan Kost
**	    All Rights Reserved
*/

//-- include --------------------------------------------------------------------------------------

//-- Ansi C/C++

#include <ctype.h>
#include <math.h>
#include <m68881.h>			// include math-extensions depending on type of math beeing used
#include <mffp.h>
#include <mieeedoub.h>
#define PIM2	6.28318530717958647692
#define MAXBYTE 255
#define MAXBYTEP 127
#define MAXBYTEN -128
#define MAXWORD 65535
#define MAXWORDP 32767
#define MAXWORDN -32768
#define MAXLONG 4294967295
#define MAXLONGP 2147483647
#define MAXLONGN -2147483648
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Amiga OS

#define __USE_SYSBASE
#include <exec/types.h>
#include <cmacros.h>

#include <clib/alib_protos.h>			/* Prototypes für Supportfunctions */

#include <proto/exec.h>					/* Prototypes für Libraryfunctions */

/*-- units --------------------------------------------------------------------------------------*/

#define INVALID_UNIT		255
#define DONT_ADD_UNIT_NAME	128

#define UGRP_AMPLITUDE	0
	#define AMP_FC			 0			//
	#define AMP_ABSPROZ		 1			// %
	#define AMP_ABSPROM		 2			// %%
	#define AMP_RELPROZ		 3			// +/- %
	#define AMP_RELPROM		 4			// +/- %%
	#define AMP_DEZIBEL	 	 5			// +/- db
	#define AMP_LEVEL		 6			// lv

#define UGRP_RELFREQUENCY	1
	#define RFRQ_FC			 0			//
	#define RFRQ_ABSPROZ	 1			// %
	#define RFRQ_ABSPROM	 2			// %%
	#define RFRQ_RELPROZ	 3			// +/- %
	#define RFRQ_RELPROM	 4			// +/- %%
	#define RFRQ_SEMITONES	 5			// +/- st
	#define RFRQ_CENTS		 6			// +/- ct
	#define RFRQ_ST_CT		 7			// +/- st:ct

#define UGRP_ABSFREQUENCY	2
	#define AFRQ_HZ			 0			// hz
	#define AFRQ_KEY		 1			// key

#define UGRP_RELTIME     	3
	#define RTME_FC			 0			//
	#define RTME_PROZ		 1			// %
	#define RTME_PROM		 2			// %%
	#define RTME_REPEATS	 3			// rpts
	
#define UGRP_ABSTIME     	4
	#define ATME_H			 0			// h
	#define ATME_M			 1			// m
	#define ATME_S			 2			// s
	#define ATME_MS			 3			// ms
	#define ATME_S_MS		 4			// s:ms
	#define ATME_M_S		 5			// m:s
	#define ATME_H_M		 6			// h:m
	#define ATME_M_S_MS		 7			// m:s:ms
	#define ATME_H_M_S		 8			// h:m:s
	#define ATME_H_M_S_MS	 9			// h:m:s:ms
	#define ATME_SAMPLES	10			// sv
	#define ATME_PALFRAMES	11			// pf
	#define ATME_NTSCFRAMES	12			// nf
	#define ATME_MOVFRAMES	13			// mf
	#define ATME_S_PF		14			// s:pf
	#define ATME_M_S_PF		15			// m:s:pf
	#define ATME_H_M_S_PF	16			// h:m:s:pf
	#define ATME_S_NF		17			// s:nf
	#define ATME_M_S_NF		18			// m:s:nf
	#define ATME_H_M_S_NF	19			// h:m:s:nf
	#define ATME_S_MF		20			// s:mf
	#define ATME_M_S_MF		21			// m:s:mf
	#define ATME_H_M_S_MF	22			// h:m:s:mf

#define UGRP_PROPOTION		5
	#define PROP_FC			 0			//
	#define PROP_PROZ		 1			// %
	#define PROP_PROM		 2			// %%

#define UGRP_COUNT			6
	#define COUN_ABS		 0			//
	#define COUN_REL		 1			// +/-

#define UGRP_PHASE			7
	#define PHA_FC			 0			//
	#define PHA_PROZ		 1			// %
	#define PHA_PROM		 2			// %%
	#define PHA_D			 3			// °
	#define PHA_M			 4			// '
	#define PHA_S			 5			// "
	#define PHA_M_S			 6			// ':"
	#define PHA_D_M			 7			// °:'
	#define PHA_D_M_S		 8			// °:':"
	#define PHA_RAD			 9			// rad
	#define PHA_GRD			 10			// grd

/*-- eof --------------------------------------------------------------------------------------------------------*/

#endif	 /* LIBRARIES_UNITCONV_H */
