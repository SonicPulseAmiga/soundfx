/******************************************************************************
** SFX-Chipregs.h                                                            **
**---------------------------------------------------------------------------**
** SFX Chipregisterdefinitionen für Amiga-Player                             **
**---------------------------------------------------------------------------**
** Version : V 3.70                                                          **
** Date    : Mittwoch 24-Jun-98                                              **
** Author  : Stefan Kost                                                     **
******************************************************************************/

#ifndef SFX_CHIPREGDEFS_H
#define SFX_CHIPREGDEFS_H

/* Chipregisterdefinitionen */

#ifdef CHIPREG_DEFS

ULONG *aud0lc =(ULONG *)0xdff0A0;	/* Zeiger auf Audiodaten */
ULONG *aud1lc =(ULONG *)0xdff0B0;
ULONG *aud2lc =(ULONG *)0xdff0C0;
ULONG *aud3lc =(ULONG *)0xdff0D0;
UWORD *aud0len=(UWORD *)0xdff0A4;	/* Länge der Audiodaten */
UWORD *aud1len=(UWORD *)0xdff0B4;
UWORD *aud2len=(UWORD *)0xdff0C4;
UWORD *aud3len=(UWORD *)0xdff0D4;
UWORD *aud0per=(UWORD *)0xdff0A6;	/* Periode der Audiodaten */
UWORD *aud1per=(UWORD *)0xdff0B6;
UWORD *aud2per=(UWORD *)0xdff0C6;
UWORD *aud3per=(UWORD *)0xdff0D6;
UWORD *aud0vol=(UWORD *)0xdff0A8;	/* Lautstärke der Audiodaten */
UWORD *aud1vol=(UWORD *)0xdff0B8;
UWORD *aud2vol=(UWORD *)0xdff0C8;
UWORD *aud3vol=(UWORD *)0xdff0D8;
UWORD *dmacon =(UWORD *)0xdff096;	/* DMAControllregister (write) */
UWORD *dmaconr=(UWORD *)0xdff002;	/* DMAControllregister (read) */
UWORD *adkcon =(UWORD *)0xdff09E;	/* AudioControlRegister für VolumeModulation */
UWORD *intreq =(UWORD *)0xdff09C;	/* IRQ-Regs */
UWORD *intena =(UWORD *)0xdff09A;
UBYTE *PRA	  =(UBYTE *)0xbfe001;	/* u.a. Filter */

#else

extern ULONG *aud0lc ;
extern ULONG *aud1lc ;
extern ULONG *aud2lc ;
extern ULONG *aud3lc ;
extern UWORD *aud0len;
extern UWORD *aud1len;
extern UWORD *aud2len;
extern UWORD *aud3len;
extern UWORD *aud0per;
extern UWORD *aud1per;
extern UWORD *aud2per;
extern UWORD *aud3per;
extern UWORD *aud0vol;
extern UWORD *aud1vol;
extern UWORD *aud2vol;
extern UWORD *aud3vol;
extern UWORD *dmacon ;
extern UWORD *dmaconr;
extern UWORD *adkcon ;
extern UWORD *intreq ;
extern UWORD *intena ;
extern UBYTE *PRA	 ;

#endif

#endif /* SFX_CHIPREGDEFS_H */


















