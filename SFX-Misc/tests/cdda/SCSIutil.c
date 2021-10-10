/*
 *****	SCSIutil
 *
 *	A utility to do some low-level operations to a SCSI disk, e.g.
 *
 *		- start/stop motor
 *		- read sectors
 *		- read disk capacity info
 *		- get inquiry info (manufacturers name etc)
 *		- seek to a sector's cylinder (use to park heads)
 *		- play audio tracks of a CD-DA
 *		- eject/insert a medium
 *		- read digital data off audio CDs (with Sony 8003 = Apple CD300
 *		  and Toshiba 3401)
 *		- set output volume of a CD-ROM drive
 *
 *	NOTE:  this program is based on SCSI information taken from
 *	       the accompanying documentation of a NEC D3841 SCSI disk.
 *	       I don't know the extent to which SCSI standards are
 *	       supported by that disk.
 *
 *	       These commands work on the above disk. But a seek to
 *	       sector -1 (park on the NEC disk) fails on my Quantum 105.D
 *
 *	Program returns:
 *	1 - init didn't work (maybe allocmem failed, etc.)
 *	2 - wrong parameter count
 *	3 - wrong parameter
 *
 *****	Written by Gary Duncan
 *
 *	Bug reports etc via e-mail to gduncan@philips.oz.au) , or mail to
 *
 *	Gary Duncan
 *	Philips PTS
 *      23 Lakeside Dr
 *	Tally-Ho Technology Park
 *	Burwood East Vic 3151
 *	Australia
 *
 * New features and rewrites by:
 *
 *	The Software Brewery
 *	Heiko Rath
 *	Raiffeisenstr.10a
 *	D-64331 Weiterstadt
 *	Germany
 *
 *	EMail: hr@brewhr.swb.de
 *
 *****	Freely distributable for non-commercial purposes
 *
 *	Compiles under Lattice 6.50
 *	- needs AmigaDos 2.0  #includes
 *
 ***** Thanks to Markus Illenseer for some beta-testing.
 *
 *****	Function List :-
 *
 *	breakcheck()		by GD, modified by <HR>
 *	__chkabort()		by <HR>
 *	DoScsiCmd ()		by <HR>
 *	err_str()		by GD
 *	gcomp()			by GD
 *	finddrivebrand()	by <HR>
 *	GetDevName()		by GD
 *	id2string ()		by <HR>
 *	init ()			by GD, modified by <HR>
 *	inquiry()		by GD, modified by <HR>
 *	medium_removal()	by <HR>
 *	mode_sense()		by <HR>
 *	motor ()		by GD, modified by <HR>
 *	play_audio ()		by <HR>
 *	rawahexasciioutput()	by <HR>
 *	rawhexoutput()		by <HR>
 *	read_capacity()		by GD, modified by <HR>
 *	read_cdblockheader()	by <HR>
 *	read_cddaasync()	by <HR>
 *	read_sec ()		by GD, modified by <HR>
 *	read_sec_scsi ()	by GD, modified by <HR>
 *	read_subchannel()	by <HR>
 *	read_toc ()		by <HR>
 *	seek ()			by GD, modified by <HR>
 *	SendScsiCmd()		by <HR>
 *	sense_errs()		by GD
 *	set_volume()		by <HR>
 *	usage()			by GD, modified by <HR>
 *	WaitScsiCmd()		by <HR>
 *
 */

#define VERSION "2.02"

/*
 **** Includes
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <exec/types.h>
#include <exec/io.h>
#include <exec/execbase.h>
#include <exec/nodes.h>
#include <exec/memory.h>
#include <devices/trackdisk.h>
#include <devices/scsidisk.h>
#include <libraries/dos.h>
#ifdef __SASC
#include <proto/all.h>
#else
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
/* non-ANSI C macro isascii() */
#define isascii(a) (((a) & 0x80) == 0)
#endif
#include "scsi_priv.h"

/*
 * string for AmigaDOS Version Command 
 */
char A_vers[] = "\0$VER: SCSIutil " VERSION " (" __DATE__ " " __TIME__ ")";

/*
 **** Global variables
 */
UBYTE *ip_buf = NULL;
UBYTE *scsi_data = NULL;
UBYTE *toc_buf = NULL;

UBYTE *dev = "";

int scsi_id = -1;		/* ID of the SCSI device to send commands to */
UBYTE *pname;
UBYTE buffer[LINE_BUF];
int secno = -1;
DRIVETYPE whatdrive = UNKNOWN;	/* what CD-ROM brand */

UBYTE *cdda_buf[NDBLBUF];
MSGPORT *mp_ptr[NDBLBUF];
IOSTDREQ *io_ptr[NDBLBUF];
SCSICMD scsi_cmd[NDBLBUF];
UBYTE *scsi_sense[NDBLBUF];
UBYTE scsi_status[NDBLBUF];
BYTE *mono_buf[NDBLBUF];

#ifdef USE8SVX
BYTE write8svx = FALSE;		/* convert to 8SVX */
#endif	/* USE8SVX */

/*
 **** Function descr.
 */
int breakcheck (void);
int DoScsiCmd (UBYTE * data, int datasize, UBYTE * cmd, int cmdsize, UBYTE flags);
UBYTE *err_str (int err);
void exit (int status);
void finddrivebrand (void);
int gcomp (char *p1, char *p2, int len);
UBYTE *GetDevName (char *grep);
UBYTE *id2string (int id, IDTOSTRING * idtable);
BOOLEAN init (void);
void inquiry (BOOLEAN parsedoutput);
void medium_removal(BOOLEAN lock);
void mode_sense (BOOLEAN parsed, UBYTE control, UBYTE page);
void motor (int motorstatus);
void play_audio (int starttrack, int startindex, int endtrack, int endindex);
void rawhexasciioutput (UBYTE *p, UWORD numbytes, UBYTE leadspace);
void rawhexoutput (UBYTE *p, UWORD numbytes);
void read_capacity (BOOLEAN parsed);
void read_cdblockheader (BOOLEAN parsed, ULONG block);
#ifdef _DCC
__stkargs
#endif
void read_cddaasync (ULONG startblock, ULONG numblocks, BYTE
		     whichchannel, BYTE use16bit, unsigned int subcode);
void read_sec (void);
void read_sec_scsi (void);
void read_subchannel (BOOLEAN parsed, UBYTE subchannel, UBYTE subchannelformat, UBYTE track);
void read_toc (int toclong);
void seek (void);
UBYTE *sense_errs (int req, int err);
void set_volume (int vol0, int vol1, int vol2, int vol3);
void usage (void);

int WaitScsiCmd (int req);
void SendScsiCmd (int req, UBYTE * data, int datasize, UBYTE * cmd, int cmdsize, UBYTE flags);

#ifdef __SASC
void __regargs __chkabort (void);
#endif	/* __SASC */
#ifdef _DCC
#include <stdlib.h>
typedef int (*brkfuncptr)();
int brkhandler() { return(0); }
#endif



/*********************************************************************
 *
 *	main
 *
 *
 */

int
main (int argc, char **argv)
{
  UBYTE *p;
  int j = 0, i = 0;
  int returnvalue = 0;

#ifdef _DCC
  int (*oldbrkhandler)() = onbreak(brkhandler);
#endif
  if (argc == 1)
    {
      usage ();
      exit (1);
    }

  /*
   *  	see if a SCSI.device specified
   */
  if (strncmp (argv[1], "-d", 2) == 0)
    {
      j = 1;
      dev = argv[1] + 2;
    }
  else if ((dev = GetDevName (SCSI_STRING)) == NULL)
    {
      fprintf (stderr, "Error : no *scsi*.device in device list\n");
      exit (1);
    }
  pname = argv[0];
  if (argc < (j + 2))
    {
      usage ();			/* help inquiry */
      exit (1);
    }

  if (argc < (j + 3))
    {
      fprintf (stderr, "Error : Not enough params\n");
      exit (1);
    }

  /*
   *	pick up SCSI id ; do a rough check
   */
  if (sscanf (argv[j + 1], "%d", &scsi_id) != 1 || scsi_id < 0)
    {
      fprintf (stderr, "Error : Bad scsi id\n");
      exit (1);
    }

  if (*(p = argv[j + 2]) != '-')
    {
      fprintf (stderr, "Error : bad option\n");
      exit (1);
    }

  /*
   *	now set up structures etc for SCSI xfer
   */
  if (init () == FALSE)
    {
      returnvalue = 1;
      goto error;
    }


  /*
   **********************	now examine the options
   */
  if (argc == (j + 3))
    /* commands without parameter */
    {
      switch (*++p)
	{
	  /*
           ****     read capacity
           */
	case 'c':
	  if (*++p == 'r')
	    {
              read_capacity (FALSE);	/* output raw data */
            }
          else
            {
              read_capacity (TRUE);	/* output parsed data */
            }
	  break;

	  /*
           ****     inquiry (raw)
           */
	case 'i':
	  if (*++p == 'r')
	    {
              inquiry (FALSE);		/* output raw data */
            }
          else
            {
              inquiry (TRUE);		/* output parsed data */
            }
	  break;

	  /*
           ****     read TOC
           */
	case 't':
	  if (*(p+1) == 'r')
	    {
	      read_toc(0);	/* output raw data */
	    }
	  else if (*(p+1) == 'l')
	    {
	      read_toc(2);	/* output long form */
	    }
	  else
	    {
              read_toc(1);	/* output short form */
	    }
	  break;

	  /*
           ****     show volume settings
           */
	case 'v':
	  set_volume (-1, -1, -1, -1);
	  break;

	default:
	  fprintf (stderr, "Error : bad option\n");
	  returnvalue = 3;
	}
    }
  else if (argc == (j + 4))
    /* commands with one parameter */
    {
      switch (*++p)
	{
	  /*
           ****     change medium
           */
	case 'e':
	  {
	    int eject = -1;
            
            if (sscanf (argv[j + 3], "%d", &eject) != 1
                || ((eject != 0) && (eject != 1)))
              {
                fprintf (stderr, "Error : eject/load control must be 0 or 1\n");
                returnvalue = 3;
              }
            else
              {
                motor ((eject | 2));	/* eject/insert */
              }
	  }
	  break;

	  /*
           ****     read CD-ROM data block address header
           */
	case 'h':
	  {
	    ULONG block;

	    if (sscanf (argv[j + 3], "%lu", &block) != 1)
	      {
	        fprintf (stderr, "Error : Bad block no\n");
	        returnvalue = 3;
	      }
	    else
	      {
	        if (*++p == 'r')
	          {
	            read_cdblockheader (FALSE, block);
	          }
	        else
	          {
	            read_cdblockheader (TRUE, block);
	          }
	      }
	  }
	  break;

	  /*
           ****     prevent/allow medium removal
           */
	case 'l':
	  {
	    int lock = -1;

            if (sscanf (argv[j + 3], "%d", &lock) != 1
               || ((lock != 0) && (lock != 1)) )
              {
                fprintf (stderr, "Error : Medium removal control must be 0 or 1\n");
                returnvalue = 3;
              }
            else
              {
                medium_removal (lock);	/* prevent/allow medium removal */
              }
	  }
	  break;

	  /*
           ****     stop/start motor
           */
	case 'm':
	  {
	    int motoronoff = -1;

            if (sscanf (argv[j + 3], "%d", &motoronoff) != 1
               || ((motoronoff != 0) && (motoronoff != 1)) )
              {
                fprintf (stderr, "Error : motor control must be 0 or 1\n");
                returnvalue = 3;
              }
            else
              {
                motor (motoronoff);	/* turn on/off motor */
              }
	  }
	  break;

	  /*
           ****     read sectors
           */

	case 'r':
	  /*
           *        get sector #
           */
	  if (sscanf (argv[j + 3], "%d", &secno) != 1)
	    {
	      fprintf (stderr, "Error : Bad sec no\n");
	      returnvalue = 3;
	    }
	  else
	    {
	      if (*++p == 't')
	        {
                  read_sec ();	/* read sector with trackdisk.device */
	        }
	      else
	        {
                  read_sec_scsi ();	/* read sector with scsi */
	        }
	    }
	  break;

	  /*
           ****     seek to cylinder containing secno
           */
	case 's':
	  /*
           *        get sector #
           */
	  if (sscanf (argv[j + 3], "%d", &secno) != 1)
	    {
	      fprintf (stderr, "Error : Bad sec no\n");
	      returnvalue = 3;
	    }
	  else
	    {
	      seek ();
	    }
	  break;

	default:
	  fprintf (stderr, "Error : bad option\n");
	  returnvalue = 3;
	}
    }
  else if (argc == (j + 5))
    /* commands with two parameters */
    {
      ULONG startblock, numblocks;	/* used by the CDDA commands */
      BYTE whichchannel = -1;		/* read left / right channel / stereo */

      switch (*++p)
	{
	  /*
           ****     read CD-DA (8 bit left|right mono)
           */
	case 'D':
	  if ((whichchannel = toupper (*(p + 1))) != 'L'
	      && whichchannel != 'R')
	    {
	      fprintf (stderr, "Error : must be either -DL or -DR\n");
	      returnvalue = 3;
	    }
	  else
	    {
	      if (sscanf (argv[j + 3], "%lu", &startblock) != 1
	          || sscanf (argv[j + 4], "%lu", &numblocks) != 1)
	        {
	          fprintf(stderr, "Error : invalid parameter for startblock of numblocks\n");
	          returnvalue = 3;
	        }
	      else
	        {
                  read_cddaasync (startblock, numblocks, whichchannel,
				  FALSE, 0);
	        }
	    }
	  break;

#ifdef USE8SVX
	  /*
           ****     read CD-DA (output 8SVX 8 bit left|right|stereo)
           */
	case '8':
	  if ((whichchannel = toupper (*(p + 1))) != 'L' && whichchannel != 'R' && whichchannel != 'S')
	    {
	      fprintf (stderr, "Error : must be -8{L|R|S}\n");
	      returnvalue = 3;
	    }
	  else
	    {
	      if (sscanf (argv[j + 3], "%lu", &startblock) != 1
	          || sscanf (argv[j + 4], "%lu", &numblocks) != 1)
	        {
	          fprintf(stderr, "Error : invalid parameter for startblock of numblocks\n");
	        }
	      else
	        {
                  write8svx = TRUE;
                  read_cddaasync (startblock, numblocks, whichchannel,
				  FALSE, 0);
	        }
	    }
	  break;
#endif	/* USE8SVX */

	  /*
           ****     mode sense
           */
	case 'o':
	  {
	    int control, page;

            if (sscanf (argv[j + 3], "%d", &control) != 1 || control < 0 || control > 3)
              {
                fprintf(stderr, "Error : invalid parameter for control (must be 0-4)\n");
                returnvalue = 3;
              }
            else
              {
                if (sscanf (argv[j + 4], "%d", &page) != 1 || page < 0 || page > 0x3f)
                  {
                    fprintf(stderr, "Error : invalid parameter for page (must be 0-0x3f)\n");
                    returnvalue = 3;
                  }
                else
                  {
                    if (*++p == 'r')
                      /* raw */
                      {
                        mode_sense (FALSE, control, page);
                      }
                    else
                      /* parsed */
                      {
                        mode_sense (TRUE, control, page);
                      }
                  }
              }
	  }
          break;


	default:
	  fprintf (stderr, "Error : bad option\n");
	  returnvalue = 3;
	}
    }
  else if (argc == (j + 6))
    /* commands with 3 parameters */
    {
      ULONG startblock, numblocks;	/* used by the CDDA commands */
      BYTE whichchannel = -1;		/* read left / right channel / stereo */
      unsigned int subcode = 0;

      switch (*++p)
        {
	  /*
           ****     read CD-DA (16 bit raw left|right|stereo)
           */
	case 'd':
	  if ((whichchannel = toupper (*(p + 1))) != 'L'
	      && whichchannel != 'R' && whichchannel != 'S')
	    {
	      fprintf (stderr, "Error : must be -d{L|R|S}\n");
	      returnvalue = 3;
	    }
	  else
	    {
	      if (sscanf (argv[j + 3], "%lu", &startblock) != 1
	          || sscanf (argv[j + 4], "%lu", &numblocks) != 1)
	        {
	          fprintf(stderr, "Error : invalid parameter for startblock of numblocks\n");
	          returnvalue = 3;
	        }
	      else
	        {
		  if (sscanf (argv[j + 5], "%u", &subcode) != 1)
		    {
		      fprintf(stderr, "Error : invalid parameter for subcode\n");
		      returnvalue = 3;
		    }
		  else
		    read_cddaasync (startblock, numblocks, whichchannel,
				    TRUE, subcode);
	        }
	    }
	  break;

	  /*
           ****     read sub-channel information
           */
	case 'u':	/* raw */
	case 'U':	/* parsed */
	  {
	    int trackno, subchannelformat, subchannel;

            /*
             *        get sub-channel #
             */
            if (sscanf (argv[j + 3], "%d", &subchannel) != 1
                || subchannel < 0 || subchannel >255)
              {
                fprintf (stderr, "Error : Sub-channel must be 0-255\n");
                returnvalue = 3;
              }
            /*
             *        get sub-channel data format #
             */
            if (sscanf (argv[j + 4], "%d", &subchannelformat) != 1
                || subchannelformat < 0 || subchannelformat >255)
              {
                fprintf (stderr, "Error : Sub-channel data format must be 0-255\n");
                returnvalue = 3;
              }
            /*
             *        get track #
             */
            if (sscanf (argv[j + 5], "%d", &trackno) != 1 || trackno < 0
                || trackno > 99)
              {
                fprintf (stderr, "Error : Bad track no (must be 1-99)\n");
                returnvalue = 3;
              }
            else
              {
                if (*++p == 'r')
                  {
                    read_subchannel (FALSE,subchannel, subchannelformat, trackno);
                  }
                else
                  {
                    read_subchannel (TRUE,subchannel, subchannelformat, trackno);
                  }
              }
	    }
	  break;

	default:
	  fprintf (stderr, "Error : bad option\n");
	  returnvalue = 3;
	}

    }

  else if (argc == (j + 7))
    /* commands with 4 parameters */
    {
      switch (*++p)
	{
	  /*
           ****     play audio
           */
	case 'p':
	  {
	    int starttrack, startindex, endtrack, endindex;

            if (sscanf (argv[j + 3], "%d", &starttrack) != 1
                || (starttrack < 1) || (starttrack > 99))
              {
                fprintf (stderr, "Error : Starting audio track must be in the range 1-99\n");
                returnvalue = 3;
                goto error;
              }
            if (sscanf (argv[j + 4], "%d", &startindex) != 1
                || (startindex < 0) || (startindex > 99))
              {
                fprintf (stderr, "Error : Starting audio track index must be in the range 1-99\n");
                returnvalue = 3;
                goto error;
              }
            if (sscanf (argv[j + 5], "%d", &endtrack) != 1
                || (endtrack < 1) || (endtrack > 99))
              {
                fprintf (stderr, "Error : Ending audio track must be in the range 1-99\n");
                returnvalue = 3;
                goto error;
              }
            if (sscanf (argv[j + 6], "%d", &endindex) != 1
                || (endindex < 0) || (endindex > 99))
              {
                fprintf (stderr, "Error : Ending audio track index must be in the range 1-99\n");
                returnvalue = 3;
                goto error;
              }
            play_audio (starttrack, startindex, endtrack, endindex);
	  }
	  break;

	  /*
           ****     change volume settings
           */
	case 'v':
	  {
	    int i;
	    int vol[4];

	    for (i = 0; i < 4; i++)
	      {
		if (sscanf (argv[j + 3 + i], "%d", &vol[i]) != 1
		    || (vol[i] < -1) || (vol[i] > 255))
		  {
		    fprintf (stderr, "Error : Volume %d must be in the range -1 - 255\n", i);
		    returnvalue = 3;
		    goto error;
		  }
	      }
	    set_volume (vol[0], vol[1], vol[2], vol[3]);
	  }
	  break;

	default:
	  fprintf (stderr, "Error : bad option\n");
	  returnvalue = 3;
	}
    }
  else
    {
      fprintf (stderr, "Error : bad option\n");
      returnvalue = 3;
    }

error:

  for (i = 0; i < NDBLBUF; i++)
    {
      if (io_ptr[i])
	{
	  CloseDevice ((struct IORequest *) io_ptr[i]);
	  DeleteStdIO (io_ptr[i]);
	}

      if (cdda_buf[i])
	FreeMem (cdda_buf[i], MAX_CDDALEN);

      if (mp_ptr[i])
	DeletePort (mp_ptr[i]);

      if (scsi_sense[i])
	FreeMem (scsi_sense[i], SENSE_LEN);

      if (mono_buf)
	FreeMem (mono_buf[i], MAX_CDDALEN / 2);
    }

  if (toc_buf)
    FreeMem (toc_buf, MAX_TOC_LEN);

  if (ip_buf)
    FreeMem (ip_buf, TD_SECTOR);

  if (scsi_data)
    FreeMem (scsi_data, MAX_DATA_LEN);

#ifdef _DCC
  onbreak(oldbrkhandler);
#endif
  exit (returnvalue);
}

/*********************************************************************
 *
 *	Initialization function
 *
 */
BOOLEAN
init (void)
{
  int i;

  if ((scsi_data = (UBYTE *) AllocMem (MAX_DATA_LEN, MEMF_CHIP | MEMF_CLEAR)) == NULL)
    {
      fprintf (stderr, "AllocMem(0) Fail\n");
      return FALSE;
    }

  if ((ip_buf = (UBYTE *) AllocMem (TD_SECTOR, MEMF_CHIP)) == NULL)
    {
      fprintf (stderr, "AllocMem(2) Fail\n");
      return FALSE;
    }

  if ((toc_buf = (UBYTE *) AllocMem (MAX_TOC_LEN, MEMF_CHIP)) == NULL)
    {
      fprintf (stderr, "AllocMem(3) Fail\n");
      return FALSE;
    }

  for (i = 0; i < NDBLBUF; i++)
    {
      if ((scsi_sense[i] = (UBYTE *) AllocMem (SENSE_LEN, MEMF_CHIP || MEMF_CLEAR)) == NULL)
	{
	  fprintf (stderr, "AllocMem (scsi_sense[%d]) Fail\n",i);
	  return FALSE;
	}
      if ((cdda_buf[i] = (UBYTE *) AllocMem (MAX_CDDALEN, 0)) == NULL)
	{
	  fprintf (stderr, "AllocMem (cdda_buf[%d]) Fail\n",i);
	  return FALSE;
	}

      if ((mp_ptr[i] = (MSGPORT *) CreatePort (NULL, 0)) == NULL)
	{
	  fprintf (stderr, "CreatePort (mp_ptr[%d]) Fail\n",i);
	  return FALSE;
	}
      if ((io_ptr[i] = (IOSTDREQ *) CreateStdIO (mp_ptr[i])) == NULL)
	{
	  fprintf (stderr, "CreateStdIO (io_ptr[%d]) Fail\n",i);
	  return FALSE;
	}
      if (OpenDevice (dev, scsi_id, (struct IORequest *) io_ptr[i], 0) != 0)
	{
	  fprintf (stderr,
		   "Error %d while opening SCSI dev \"%s\", unit (%d)\n",
		   io_ptr[i]->io_Error, dev, scsi_id, i);
	  return FALSE;
	}
      if ((mono_buf[i] = (BYTE *) AllocMem (MAX_CDDALEN / 2, 0)) == NULL)
	{
	  fprintf (stderr, "AllocMem (mono_buf[%d]) Fail\n",i);
	  return FALSE;
	}

    }

  return TRUE;
}

/*********************************************************************
 *
 *	function to read parameter pages from a device
 */

void
mode_sense (BOOLEAN parsed, UBYTE control, UBYTE page)
{
  static SCSICMD6 command =
  {
    SCSI_CMD_MSE,	/* 0x1a MODE SENSE scsi command */
    PAD,		/* LUN | rsrvd. | DBD | rsrvd. */
    0,			/* PC | Page Code */
    PAD,		/* rsrvd. */
    0,			/* allocation length */
    PAD			/* control */
  };

  static IDTOSTRING pagecontrolfield[] =
  {
    0x00, "Current Values",
    0x01, "Changeable Values",
    0x02, "Default Values",
    0x03, "Saved Values",
    -1, "Illegal value"
  };

  UWORD i,j;
  int err;

  command.b2 = (control<<6) | page;
  command.b4 = MAX_DATA_LEN;

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
                        (UBYTE *) &command, sizeof (command),
                        (SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
  else
    {
      if (parsed == TRUE)
        /* output parsed data */
        {
          printf ("%s\n", id2string(control, pagecontrolfield));
          printf ("Mode Parameter Header:\n");
          printf (" Mode Data Length: %hu\n", scsi_data[0]);
          printf (" Medium Type: %hu\n", scsi_data[1]);
          printf (" Device-Specific Parameter: %hu\n", scsi_data[2]);
          printf (" Block Descriptor Length: %hu\n", scsi_data[3]);

	  for (i = 0; i < scsi_data[3]; i +=8)	/* print block descriptors */
	    {
	      printf("Block Descriptor Density Code: %hu\n",scsi_data[(i)+4]);
	      printf(" Number of Blocks: %lu\n", (scsi_data[(i)+5]<<16) + (scsi_data[(i)+6]<<8) + (scsi_data[(i)+7]) );
	      printf(" Byte 4 (reserved): %hu\n", scsi_data[(i)+8]);
	      printf(" Block length: %lu\n", (scsi_data[(i)+9]<<16) + (scsi_data[(i)+10]<<8) + (scsi_data[(i)+11]) );
	    }

	  for (j = (scsi_data[0]+1), i = scsi_data[3] + 4; i < j; i += scsi_data[i+1] + 2)
	    {
	      printf("Page Code: %hu\n",scsi_data[i] & 0x3f);
	      printf(" Page can%s be saved\n", (scsi_data[i] & 0x80) ? "" : " not");
	      printf(" Page Length: %hu\n", scsi_data[i+1]);
	      printf(" Mode Parameters:\n");
	      rawhexasciioutput(&scsi_data[i+2], scsi_data[i+1], 2);
	    }
        }
      else
        /* output raw data */
        {
          rawhexasciioutput(scsi_data, scsi_data[0]+1, 0);
        }
    }
}

/*********************************************************************
 *
 *	function to read CD-ROM data block address header
 *	starting block and number of blocks.
 */

void
read_cdblockheader (BOOLEAN parsed, ULONG block)
{
  static SCSICMD10 command =
  {
    SCSI_CMD_READHEADER,	/* 0x44 READ HEADER scsi command */
    0,				/* LUN | rsrvd. | MSF | rsrvd. */
    0, 0, 0, 0,			/* Logical Block Address (ULONG) */
    PAD,			/* reserved */
    0,0,			/* allocated data length */
    PAD				/* control */
  };

  static IDTOSTRING udatafieldcont[] =
  {
    0x00, "All bytes zero",
    0x01, "User Data",
    0x02, "User Data",
    -1, "Reserved"
  };

  static IDTOSTRING auxfieldcont[] =
  {
    0x00, "All bytes zero",
    0x01, "L-EC symbols",
    0x02, "User Data",
    -1, "Reserved"
  };

  int err;

  command.b2 = block>>24;
  command.b3 = block>>16;
  command.b4 = block>>8;
  command.b5 = block;
#if MAX_CDDALEN > 65536
  command.b7 = 255;
  command.b8 = 255;
#else
  command.b7 = MAX_CDDALEN>>8;
  command.b8 = MAX_CDDALEN & 0xff;
#endif
  if ((err = DoScsiCmd ((UBYTE *) cdda_buf[0], MAX_CDDALEN,
                        (UBYTE *) &command, sizeof (command),
                        (SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
  else
    {
      if (parsed == TRUE)
        {
          printf("User Data Field Contents (2048 bytes): %d = %s\n",
		 (cdda_buf[0])[0], id2string ((cdda_buf[0])[0], udatafieldcont));
          printf("Auxiliary Field Contents ( 288 bytes): %d = %s\n",
		 (cdda_buf[0])[0], id2string ((cdda_buf[0])[0], auxfieldcont));
          printf("Byte 1 (reserved): %d\n",(cdda_buf[0])[1]);
          printf("Byte 2 (reserved): %d\n",(cdda_buf[0])[2]);
          printf("Byte 3 (reserved): %d\n",(cdda_buf[0])[3]);
          printf("Absolute CD-ROM Address: %lu\n",
		 ((cdda_buf[0])[4] << 24) | ((cdda_buf[0])[5] << 16) | ((cdda_buf[0])[6] << 8) | ((cdda_buf[0])[7]));
        }
      else
        {
          rawhexasciioutput(cdda_buf[0], 8, 0);
        }
    }
}

/*********************************************************************
 *
 *	function to read subchannel data audio from Sony CDROM with
 *	starting block and number of blocks.
 */

void
read_subchannel (BOOLEAN parsed, UBYTE subchannel, UBYTE subchannelformat, UBYTE track)
{
  static SCSICMD10 command =
  {
    SCSI_CMD_READSUBCHANNEL,	/* 0x42 READ SUB-CHANNEL scsi command */
    0,				/* LUN | rsrvd. | MSF | rsrvd. */
    0x40,			/* return Sub-Q Channel data */
    0,				/* Sub-channel Data Format */
    PAD,
    PAD,
    0,				/* Track Number 1-99 */
    0,0,			/* allocated data length */
    PAD				/* control */
  };

  static IDTOSTRING audiostatus[] =
  {
    0x00, "Audio status byte not supported or not valid",
    0x11, "Audio play operation in progress.",
    0x12, "Audio play operation paused.",
    0x13, "Audio play operation successfully completed.",
    0x14, "Audio play operation stopped due to error.",
    0x15, "No current audio status to return",
    -1, "Reserved, unknown or no audio status"
  };

  static IDTOSTRING Qfield[] =
  {
    0x00, "Sub-channel Q mode information not supplied.",
    0x01, "Sub-channel Q encodes current position data.",
    0x02, "Sub-channel Q encodes media catalog number.",
    0x03, "Sub-channel Q encodes ISRC.",
    -1, "Reserved"
  };

  int err;

  command.b2 = subchannel;
  command.b3 = subchannelformat;
  command.b6 = track;

#if MAX_CDDALEN > 65536
  command.b7 = 255;
  command.b8 = 255;
#else
  command.b7 = MAX_CDDALEN & 0xff;
  command.b8 = MAX_CDDALEN>>8;	/* Allocation length = max. data length */
#endif

  if ((err = DoScsiCmd ((UBYTE *) cdda_buf[0], MAX_CDDALEN,
                        (UBYTE *) &command, sizeof (command),
                        (SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
  else
    {
      if (parsed == TRUE)
        /* parsed output */
        {
          if (subchannel == 0x40)
            {
              if (subchannelformat == 0)
                {
	/* CHANGE - this doesn't work with my Apple CD300!!!!! */
		  rawhexoutput(cdda_buf[0], ((cdda_buf[0])[2]*256 + (cdda_buf[0])[3])+4);
                }
              else if (subchannelformat == 1)
                {
		  printf("Byte 0 (reserved): %02x\n",(cdda_buf[0])[0]);
		  printf("Audio status: %s\n",id2string ((cdda_buf[0])[1],audiostatus));
		  printf("Sub-Channel Data Length: %d\n",((cdda_buf[0])[2]*256 + (cdda_buf[0])[3]));
		  printf("Sub-Channel Data Format code (should be 0x01!): %d\n",(cdda_buf[0])[4]);
		  printf("ADR: %s\n",id2string(((cdda_buf[0])[5]>>4 & 0x0f), Qfield));

		  printf("Audio with%s pre-emphasis.  ", ((cdda_buf[0])[5] & 0x01) ? "" : "out");
		  printf("Digital copy %s\n", ((cdda_buf[0])[5] & 0x02) ? "permitted" : "prohibited");
		  printf("%s track.  ", ((cdda_buf[0])[5] & 0x04) ? "Data" : "Audio");
		  printf("%s channel audio.\n", ((cdda_buf[0])[5] & 0x08) ? "Four" : "Two");

		  printf("Track Number: %d\n", (cdda_buf[0])[6]);
		  printf("Index Number: %d\n", (cdda_buf[0])[7]);
		  printf("Absolute CD-ROM Address: %lu\n", ((cdda_buf[0])[8] << 24) | ((cdda_buf[0])[9] << 16) | ((cdda_buf[0])[10] << 8) | ((cdda_buf[0])[11]));
		  printf("Track Relative CD-ROM Address: %lu\n", ((cdda_buf[0])[12] << 24) | ((cdda_buf[0])[13] << 16) | ((cdda_buf[0])[14] << 8) | ((cdda_buf[0])[15]));
                }
              else if (subchannelformat == 2)
                {
		  printf("Byte 0 (reserved): %02x\n",(cdda_buf[0])[0]);
		  printf("Audio status %s\n",id2string ((cdda_buf[0])[1],audiostatus));
		  printf("Sub-Channel Data Length %d\n",((cdda_buf[0])[2]*256 + (cdda_buf[0])[3]));
		  printf("Sub-Channel Data Format code (should be 0x02!): %d\n",(cdda_buf[0])[4]);
		  printf("Byte 5 (reserved): %02x\n",(cdda_buf[0])[5]);
		  printf("Byte 6 (reserved): %02x\n",(cdda_buf[0])[6]);
		  printf("Byte 7 (reserved): %02x\n",(cdda_buf[0])[7]);
		  printf("Byte 8 (reserved): %02x\n",(cdda_buf[0])[8] & 0x7f);
		  printf("MCVal is %s\n", ((cdda_buf[0])[8] & 0x80) ? "true" : "false");
		  printf("Media Catalog Number (UPC/Bar Code):\n");
		  rawhexasciioutput(&(cdda_buf[0])[9], 15, 1);
                }
              else if (subchannelformat == 3)
                {
		  printf("Byte 0 (reserved): %02x\n",(cdda_buf[0])[0]);
		  printf("Audio status %s\n",id2string ((cdda_buf[0])[1],audiostatus));
		  printf("Sub-Channel Data Length %d\n",((cdda_buf[0])[2]*256 + (cdda_buf[0])[3]));
		  printf("Sub-Channel Data Format code (should be 0x03!): %d\n",(cdda_buf[0])[4]);
		  printf("ADR: %s\n",id2string(((cdda_buf[0])[5]>>4 & 0x0f) ,Qfield));

		  printf("Audio with%s pre-emphasis.  ", ((cdda_buf[0])[5] & 0x01) ? "" : "out");
		  printf("Digital copy %s\n", ((cdda_buf[0])[5] & 0x02) ? "permitted" : "prohibited");
		  printf("%s track.  ", ((cdda_buf[0])[5] & 0x04) ? "Data" : "Audio");
		  printf("%s channel audio.\n", ((cdda_buf[0])[5] & 0x08) ? "Four" : "Two");

		  printf("Track Number: %d\n", (cdda_buf[0])[6]);
		  printf("Byte 7 (reserved): %02x\n",(cdda_buf[0])[7]);
		  printf("Byte 8 (reserved): %02x\n",(cdda_buf[0])[8] & 0x7f);
		  printf("TCVal is %s\n", ((cdda_buf[0])[8] & 0x80) ? "true" : "false");
		  printf("Track International-Standard-Recording-Code (ISRC):\n");
		  rawhexasciioutput(&(cdda_buf[0])[9], 15, 1);
                }
              else
                {
                  rawhexasciioutput(cdda_buf[0], ((cdda_buf[0])[2]*256 + (cdda_buf[0])[3])+4, 0);
                }
            }
          else
            {
              rawhexasciioutput(cdda_buf[0], ((cdda_buf[0])[2]*256 + (cdda_buf[0])[3])+4, 0);
            }
        }
      else
        /* raw output */
        {
	  rawhexasciioutput(cdda_buf[0], ((cdda_buf[0])[2]*256 + (cdda_buf[0])[3])+4, 0);
        }
    }

}

/*********************************************************************
 *
 * subroutine used to printout raw hex data bytes
 *
 */
void
rawhexoutput (UBYTE *p, UWORD numbytes)
{
  UWORD i;

  for (i = 0; i < numbytes; i++)
    {
      printf (" %02x", p[i]);
    }
  printf ("\n");
}

/*********************************************************************
 *
 * subroutine used to printout raw hex data bytes with the
 * corresponding ASCII values and an index
 *
 */
void
rawhexasciioutput (UBYTE *p, UWORD numbytes, UBYTE leadspace)
{
  UWORD i, j;
  UBYTE *boff, *aoff;
  int xxxlen = strlen (" xx");			/* byte */

  buffer[5+leadspace] = '=';

  for (i = 0; i < numbytes; i += BYTES_PER_LINE)
    {
      memset (buffer, ' ', sizeof (buffer));	/* put spaces in buffer */
      boff = &buffer[7+leadspace];
      aoff = boff + (xxxlen * BYTES_PER_LINE) + 1;

      sprintf (buffer+leadspace, "%04X = ", i);		/* add offset */

      for (j = 0; (j < BYTES_PER_LINE && (i+j) < numbytes); j++, boff += xxxlen, p++, aoff++)
        {
          sprintf (boff, " %02X", *p);
          *aoff = (isascii (*p) && isprint (*p)) ? *p : '.';
        }

      buffer[strlen (buffer)] = ' ';
      *++aoff = '\n';
      *++aoff = '\0';
      printf ("%s", buffer);
    }
}

/*********************************************************************
 *
 *	function to read digital audio from Sony / Toshiba CD-ROM with
 *	starting block and number of blocks.
 *	This version uses asynchronous reads.
 *
 *	outputs LRLRLR pairs of 16 bit digital stereo audio samples,
 *	2352 / 2368 / 2448 / 96 bytes per CD-ROM block
 *
 *      or (depending on 'whichchannel')
 *
 *      8 bit digital audio samples, either the left or right channel.
 */

#ifdef _DCC
__stkargs
#endif
void
read_cddaasync (ULONG startblock, ULONG numblocks, BYTE whichchannel,
		BYTE use16bit, unsigned int subcode)
{
  static struct CMD_RDCDDA
  {
    UBYTE cmd;			/* READ CD-DA scsi command 0xD8 */
    UBYTE pad_a;		/* Bits 7-5 Logical Unit Number | Bits 4-1 Reserved | Bit 0 EVPD */
    ULONG lba;			/* logical block address MSB, , ,LSB */
    ULONG lbn;			/* number of blocks to transfer MSB, , ,LSB */
    UBYTE subcode;		/* special sub code selector:
     				 * 0: normal 2352
				 * 1: 2368
				 * 2: 2448
				 * 3: 96 bytes */
    UBYTE cntrl;		/* Control */
  } command[NDBLBUF];

  /*
   * TOSHIBA XM3401 specific
   */
  static SCSICMD6 modecommand;
  static struct cddamodedata
  {
    UWORD pad0;
    UBYTE pad1;
    UBYTE bdlength;
    ULONG density;   /* = densitycode << 24 */
    ULONG blocklen;
  } newmodedata;


  int err, i = 0, j = 0, k = 0, l = 0;
  ULONG nblocks = numblocks, xblocks;

  finddrivebrand();		/* determine CD-ROM drive */

  /*
   * TOSHIBA XM3401 specific
   */
  if (whatdrive == TOSHIBA3401)
    {
      /* Read old mode data */
      modecommand.opcode  = SCSI_CMD_MSE;
      modecommand.b1      = 0;
      modecommand.b2      = 1;
      modecommand.b3      = 0;
      modecommand.b4      = MAX_DATA_LEN;
      modecommand.control = 0;
      if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			    (UBYTE *) &modecommand, sizeof (modecommand),
			    (SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
	{
	  fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
	  return;
	}
      /* Write new mode data */
      newmodedata.pad0     = 0;
      newmodedata.pad1     = 0;
      newmodedata.bdlength = 8;
      newmodedata.density  = 0x82L << 24;
      newmodedata.blocklen = 2352;
      modecommand.opcode = SCSI_CMD_MSL;
      modecommand.b1     = 0x10;
      modecommand.b2     = 0;
      modecommand.b4     = sizeof(struct cddamodedata);
      if ((err = DoScsiCmd ((UBYTE *) &newmodedata, sizeof(struct cddamodedata),
			    (UBYTE *) &modecommand, sizeof (modecommand),
			    (SCSIF_WRITE | SCSIF_AUTOSENSE))) != 0)
	{
	  fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
	  return;
	}
    } /* TOSHIBA */

  /* reset SCSI status to no command pending
   */
  for (i = 0; i < NDBLBUF; i++) scsi_status[i] = 0;

  for (i = 0; ; (++i >= NDBLBUF)? i = 0 : i)
  {
    if (breakcheck ())	/* ^C ? */
	{
	  /* wait for pending SCSI commands
	   */
	  for (j = 0, i++; j < NDBLBUF; j++, (++i >= NDBLBUF)? i = 0 : i)
	  {
	    if (scsi_status[i] == 1) WaitScsiCmd(i);
	  }
	  return;
	}
    if (nblocks > 0)
	{
	  if (scsi_status[i] == 1)	/* SCSI command pending? */
	  {
	    if ((err = WaitScsiCmd (i)) != 0)	/* read failed, reschedule */
		{
		  scsi_status[i] = 0;
		  do
		  {
		    fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));

		    if (breakcheck ())	/* ^C ? */
			{
			  for (j = 0, i++; j < NDBLBUF; j++, (++i >= NDBLBUF)? i = 0 : i)
		      {
		        if (scsi_status[i] == 1) WaitScsiCmd(i);
			  }
			  return;
			}
		      
		      SendScsiCmd (i, (UBYTE *) cdda_buf[i], MAX_CDDALEN,
				   (UBYTE *) & command[i], sizeof (command[i]),
				   (SCSIF_READ | SCSIF_AUTOSENSE));
		    }
		  while ((err = WaitScsiCmd (i)) != 0);
		}
	      scsi_status[i] = 0;	/* no SCSI command pending anymore */

	      if (use16bit == TRUE && whichchannel == 'S')
		/* output 16 bit stereo samples */
		{
		  fwrite (cdda_buf[i], scsi_cmd[i].scsi_Actual, 1, stdout);
		}
	      else if (whichchannel == 'L')
		/* output left channel */
		{
		  if (use16bit == FALSE)
		    /* output raw 8 bit left channel */
		    {
		      for (j = 0; j < (scsi_cmd[i].scsi_Actual / 4); j++)
			{
			  (mono_buf[i])[j] = (BYTE) (((cdda_buf[i])[(4 * j)] +
						      ((cdda_buf[i])[(4 * j) + 1] * 256)) / 256);
			}
		      fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 4), 1, stdout);
		    }
		  else
		    /* output raw 16 bit left channel */
		    {
		      for (j = 0, k = 0; j < (scsi_cmd[i].scsi_Actual); j += 4)
			{
			  (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j]);
			  (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 1]);
			}
		      fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 2), 1, stdout);
		    }
		}
	      else
		/* output right channel */
		{
		  if (use16bit == FALSE)
		    /* output raw 8 bit left channel */
		    {
		      for (j = 0; j < (scsi_cmd[i].scsi_Actual / 4); j++)
			{
			  (mono_buf[i])[j] = (BYTE) (((cdda_buf[i])[(4 * j) + 2] +
						      ((cdda_buf[i])[(4 * j) + 3] * 256)) / 256);
			}
		      fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 4), 1, stdout);
		    }
		  else
		    /* output raw 16 bit left channel */
		    {
		      for (j = 0, k = 0; j < (scsi_cmd[i].scsi_Actual); j += 4)
			{
			  (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 2]);
			  (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 3]);
			}
		      fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 2), 1, stdout);
		    }
		}
	    }	/* (scsi_status[i] == 1) ? */

	  if (whatdrive == TOSHIBA3401)
            {
              command[i].cmd = SCSI_CMD_READ12;   /* TOSHIBA XM3401 */
            }
	  else
	    {
              command[i].cmd = SCSI_CMD_READCDDA; /* Sony CDU-5*1 */
            }
          command[i].pad_a = 0;
          command[i].lba = startblock;
          command[i].lbn = xblocks = (nblocks > NUM_OF_CDDAFRAMES)? NUM_OF_CDDAFRAMES : nblocks;
	  command[i].subcode = subcode;
	  command[i].cntrl = 0;

	  startblock += xblocks;
	  nblocks -= xblocks;

	  scsi_status[i] = 1;	/* Status: SCSI command pending */

	  SendScsiCmd (i, (UBYTE *) cdda_buf[i], MAX_CDDALEN,
		       (UBYTE *) & command[i], sizeof (command[i]),
		       (SCSIF_READ | SCSIF_AUTOSENSE));

	}
      else	/* nblocks <= 0 */
	{
	  /* wait for all pending requests */
	  for (l = 0; l < NDBLBUF; l++)
	    {
	      if (scsi_status[i] == 1)	/* SCSI command pending ? */
		{
		  if ((err = WaitScsiCmd (i)) != 0) /* SCSI command failed? */
		    {
		      scsi_status[i] = 0;
		      do	/* reschedule SCSI command */
			{
			  fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));

			  if (breakcheck ()) /* ^C ? */
			    {
			      /* wait for pending SCSI commands
			       */
			      for (j = 0, i++; j < NDBLBUF; j++, (++i >= NDBLBUF)? i = 0 : i)
				{
				  if (scsi_status[i] == 1)
				    WaitScsiCmd(i);
				}
			      return;
			    }
		      
			  SendScsiCmd (i, (UBYTE *) cdda_buf[i], scsi_cmd[i].scsi_Length,
				       (UBYTE *) & command[i], sizeof (command[i]),
				       (SCSIF_READ | SCSIF_AUTOSENSE));
		      
			}
		      while ((err = WaitScsiCmd (i)) != 0);
		    }
		  scsi_status[i] = 0;	/* Status: no SCSI command pending */

		  if (use16bit == TRUE && whichchannel == 'S')
		    /* output 16 bit stereo samples */
		    {
		      fwrite (cdda_buf[i], scsi_cmd[i].scsi_Actual, 1, stdout);
		    }
		  else if (whichchannel == 'L')
		    /* output left channel */
		    {
		      if (use16bit == FALSE)
			/* output raw 8 bit left channel */
			{
			  for (j = 0; j < (scsi_cmd[i].scsi_Actual / 4); j++)
			    {
			      (mono_buf[i])[j] = (BYTE) (((cdda_buf[i])[(4 * j)] +
							  ((cdda_buf[i])[(4 * j) + 1] * 256)) / 256);
			    }
			  fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 4), 1, stdout);
			}
		      else
			/* output raw 16 bit left channel */
			{
			  for (j = 0, k = 0; j < (scsi_cmd[i].scsi_Actual); j += 4)
			    {
			      (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j]);
			      (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 1]);
			    }
			  fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 2), 1, stdout);
			}
		    }
		  else
		    /* output right channel */
		    {
		      if (use16bit == FALSE)
			/* output raw 8 bit left channel */
			{
			  for (j = 0; j < (scsi_cmd[i].scsi_Actual / 4); j++)
			    {
			      (mono_buf[i])[j] = (BYTE) (((cdda_buf[i])[(4 * j) + 2] +
							  ((cdda_buf[i])[(4 * j) + 3] * 256)) / 256);
			    }
			  fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 4), 1, stdout);
			}
		      else
			/* output raw 16 bit left channel */
			{
			  for (j = 0, k = 0; j < (scsi_cmd[i].scsi_Actual); j += 4)
			    {
			      (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 2]);
			      (mono_buf[i])[k++] = (BYTE) ((cdda_buf[i])[j + 3]);
			    }
			  fwrite (mono_buf[i], (scsi_cmd[i].scsi_Actual / 2), 1, stdout);
			}
		    }		/* output right channel */

		  scsi_status[i] = 0;

		}	/* (scsi_status[i] == 1) ? */

	      (++i >= NDBLBUF)? i = 0 : i;

	    }  /* for (l = 0; l < NDBLBUF; l++) */

	  break;
	}
    }
  if (whatdrive == TOSHIBA3401)
    {
      /*
       * TOSHIBA XM3401 specific
       */
      /* Write old mode data */
      newmodedata.density  = ((struct cddamodedata *) scsi_data)->density;
      newmodedata.blocklen = ((struct cddamodedata *) scsi_data)->blocklen;
      if ((err = DoScsiCmd ((UBYTE *) &newmodedata, sizeof(struct cddamodedata),
			    (UBYTE *) &modecommand, sizeof (modecommand),
			    (SCSIF_WRITE | SCSIF_AUTOSENSE))) != 0)
	{
	  fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
	}
    }
}

/*********************************************************************
 *
 *	function to read sectors from a starting sector #
 *	- similar adjacent lines are suppressed on printout.
 *
 *	- uses trackdisk.device
 */

void
read_sec (void)

{
  UBYTE *sec_click_ptr;		/* click = 16 bytes	*/
  UBYTE *pref;
  UBYTE *p;
  UWORD j;
  UWORD k;
  int err;

  /*
   *  keep printing sectors until ^C , or until error
   */
  io_ptr[0]->io_Command = CMD_READ;
  io_ptr[0]->io_Length = TD_SECTOR;
  io_ptr[0]->io_Data = (APTR) ip_buf;
  io_ptr[0]->io_Offset = secno * TD_SECTOR;	/* will be updated... */


  /*
   *  keep reading sectors : stop on ^C on bad sector #
   */
  for (;; ++secno)
    {
      UBYTE *ss;
      UWORD m_sec_offs;

      if (breakcheck ())	/* ^C ? */
	break;

      io_ptr[0]->io_Offset = secno * TD_SECTOR;	/* sector offset */

      DoIO ((struct IORequest *) io_ptr[0]);
      if ((err = io_ptr[0]->io_Error) == 0)
	{
	  printf ("\n");
	  /*
	   * scan this sector ...
  	  */
	  for (sec_click_ptr = pref = ip_buf, m_sec_offs = 0;
	       m_sec_offs < TD_SECTOR;
	       m_sec_offs += BYTES_PER_LINE, sec_click_ptr += BYTES_PER_LINE)
	    {
	      int xxxlen = strlen (" xx");	/* byte */

	      /*
	       * don't print line if same contents as previous
	       */
	      if (gcomp (sec_click_ptr, pref, BYTES_PER_LINE) == TRUE)
		{
		  if (m_sec_offs > 1)
		    continue;	/* same */
		}
	      memset (buffer, ' ', sizeof (buffer));	/* put spaces in buffer */

	      sprintf (buffer, "%05X:%03X = ", secno, m_sec_offs);

	      /* set up for loop */

	      k = strlen (buffer);
	      ss = buffer + k;
	      k += (BYTES_PER_LINE * xxxlen) + 1;
	      for (p = sec_click_ptr, j = 0;
		   j < BYTES_PER_LINE;
		   ss += xxxlen, ++j, ++k)
		{
		  UBYTE dd = *p++;
		  UBYTE que = (isascii (dd) && isprint (dd)) ? dd : '.';
		  sprintf (ss, " %02X", dd);	/* 2 hex charas  */
		  buffer[k] = que;
		}

	      buffer[strlen (buffer)] = ' ';
	      buffer[k++] = '\n';
	      buffer[k++] = '\0';

	      printf ("%s", buffer);
	      pref = sec_click_ptr;

	    }
	}
      else
	{
	  /* else DoIO error */

	  fprintf (stderr, "Error :  err = %ld , sec = %ld dec , $%lX , [%s]\n",
		   err, secno, secno, sense_errs (0, err));

	  return;
	}
    }
}

/*********************************************************************
 *
 *	function to read sectors from a starting sector #
 *	- similar adjacent lines are suppressed on printout.
 *
 *	- uses scsi device directly
 */

void
read_sec_scsi (void)
{
  static struct CMD_XREAD
  {
    UBYTE cmd;
    UBYTE lba[3];
    UBYTE numb_secs;
    UBYTE pad;
  } command =
  {
    SCSI_CMD_RD,
    0, 0, 0,
    0,
    PAD
  };

  UBYTE *sec_click_ptr;		/* click = 16 bytes	*/
  UBYTE *pref;
  UBYTE *p;
  UWORD j;
  UWORD k;
  int err;

  /*
   *  keep printing sectors until ^C , or until error
   */


  /*
   *  keep reading sectors : stop on ^C on bad sector #
   */
  for (;; ++secno)
    {
      UBYTE *ss;
      UWORD m_sec_offs;

      command.lba[2] = secno;
      command.lba[1] = secno >> 8;
      command.lba[0] = (secno >> 8) & 0x1F;

      command.numb_secs = 1;

      if (breakcheck ())	/* ^C ? */
	break;

      io_ptr[0]->io_Offset = secno * TD_SECTOR;	/* sector offset */

      if ((err = DoScsiCmd ((UBYTE *) ip_buf, 512,
			    (UBYTE *) & command, sizeof (command),
			    (SCSIF_READ | SCSIF_AUTOSENSE))) == 0)
	{
	  printf ("\n");
	  /*
	   * scan this sector ...
  	  */
	  for (sec_click_ptr = pref = ip_buf, m_sec_offs = 0;
	       m_sec_offs < TD_SECTOR;
	       m_sec_offs += BYTES_PER_LINE, sec_click_ptr += BYTES_PER_LINE)
	    {
	      int xxxlen = strlen (" xx");	/* byte */

	      /*
	       * don't print line if same contents as previous
	       */
	      if (gcomp (sec_click_ptr, pref, BYTES_PER_LINE) == TRUE)
		{
		  if (m_sec_offs > 1)
		    continue;	/* same */
		}
	      memset (buffer, ' ', sizeof (buffer));	/* put spaces in buffer */

	      sprintf (buffer, "%05X:%03X = ", secno, m_sec_offs);

	      /* set up for loop */

	      k = strlen (buffer);
	      ss = buffer + k;
	      k += (BYTES_PER_LINE * xxxlen) + 1;
	      for (p = sec_click_ptr, j = 0;
		   j < BYTES_PER_LINE;
		   ss += xxxlen, ++j, ++k)
		{
		  UBYTE dd = *p++;
		  UBYTE que = (isascii (dd) && isprint (dd)) ? dd : '.';
		  sprintf (ss, " %02X", dd);	/* 2 hex charas  */
		  buffer[k] = que;
		}

	      buffer[strlen (buffer)] = ' ';
	      buffer[k++] = '\n';
	      buffer[k++] = '\0';

	      printf ("%s", buffer);
	      pref = sec_click_ptr;

	    }
	}
      else
	{
	  /* else DoIO error */

	  fprintf (stderr, "Error :  sec = %ld dec , $%lX , [%s]\n",
		   secno, secno, sense_errs (0, err));
	  return;
	}
    }
}

/*********************************************************************
 *
 *	function to prevent/allow allow medium removal
 *
 */

void

medium_removal(int lock)
{
  static SCSICMD6 command =
  {
    SCSI_CMD_PAMR,		/* 0x1E SCSI Prevent Allow Medium Removal */
    0,				/* Bit 765 = LUN */
    PAD,			/* reserved */
    PAD,			/* reserved */
    0,				/* Bit 0 = prevent */
    PAD,			/* reserved */
  };

  int err;

  command.b4 = lock;

  if ((err = DoScsiCmd (0, 0,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error :   err=%ld , [%s]\n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to stop/start motor on SCSI device
 *	or eject/insert a medium
 *
 */

void
motor (int motorstatus)
{
  static SCSICMD6 command =
  {
    SCSI_CMD_SSU,		/* 0x1B SCSI Start / Stop Unit */
    0,
    PAD,
    PAD,
    0,				/* start/stop eject/insert */
    PAD,
  };

  int err;

  command.b4 = motorstatus;

  if ((err = DoScsiCmd (0, 0,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error :   err=%ld , [%s]\n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to seek to a cylinder
 *
 */

void
seek (void)

{
  static struct CMD_SEEK
  {
    UBYTE cmd;
    UBYTE pad_a;
    ULONG lba;
    UBYTE pad[4];
  } command =
  {
    SCSI_CMD_SKX,
    PAD,
    0,
    PAD, PAD, PAD, PAD
  };

  int err;
  /*
   *	load sector # (log block addr)
   */

  command.lba = secno;

  if ((err = DoScsiCmd (0, 0,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error :  err = %ld , sec = %ld dec , $%lX , [%s]\n",
	       err, secno, secno, sense_errs (0, err));
    }
}


/*********************************************************************
 *
 *	what CD-ROM drive?
 *
 */

void
finddrivebrand (void)
{
  static SCSICMD6 command =
  {
    SCSI_CMD_INQ,		/* 0x12 INQUIRY */
    PAD,			/* Bits 7-5 Logical Unit Number | Bits 4-1 Reserved | Bit 0 EVPD */
    PAD,			/* Page Code */
    PAD,			/* Reserved */
    0,				/* Allocation length */
    PAD				/* Control */
  };

  int err;

  command.b4 = MAX_DATA_LEN;	/* Allocation length = max. data length */

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) == 0)
    {
      int rem = scsi_cmd[0].scsi_Actual;

      if (rem >= 8)
	{
	  if (!memcmp("SONY    CD-ROM CDU-8003",
		       &scsi_data[8], strlen("SONY    CD-ROM CDU-8003")))
	    {
	      whatdrive = APPLECD300;
	    }
	  else if (!memcmp("SONY    CD-ROM CDU-8002",
			    &scsi_data[8], strlen("SONY    CD-ROM CDU-8002")))
	    {
	      whatdrive = APPLECD150;
	    }
	  else if (!memcmp("TOSHIBA",
			    &scsi_data[8], strlen("TOSHIBA")))
	    {
	      whatdrive = TOSHIBA3401;
	    }
	}
      else
	whatdrive = UNKNOWN;
    }
  else
    /* error */
    {
      fprintf (stderr, "Error : err=%ld , %s\n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to set the output volume of a CD-ROM drive
 *
 *	if (vol0 = vol1 = vol2 = vol3) == -1
 *	 display current volume settings, don't change anything
 *
 */

void
set_volume (int vol0, int vol1, int vol2, int vol3)
{
  static int err, i, j;
  static SCSICMD6 modecommand;
  static struct volmodedata
    {
      UBYTE head[4];
      UBYTE page;	/* page code 0x0E */
      UBYTE plength;	/* page length */
      UBYTE b2;		/* bit 2: Immed, bit 1: SOTC */
      UBYTE b3;		/* reserved */
      UBYTE b4;		/* reserved */
      UBYTE b5;		/* bit 7: APRVal, bit 3-0: format of LBAs / Sec. */
      UWORD bps;	/* logical blocks per second audio playback */
      UBYTE out0;	/* lower 4 bits: output port 0 channel selection */
      UBYTE vol0;	/* output port 0 volume */
      UBYTE out1;	/* lower 4 bits: output port 1 channel selection */
      UBYTE vol1;	/* output port 1 volume */
      UBYTE out2;	/* lower 4 bits: output port 2 channel selection */
      UBYTE vol2;	/* output port 2 volume */
      UBYTE out3;	/* lower 4 bits: output port 3 channel selection */
      UBYTE vol3;	/* output port 3 volume */
    } modedata;

  for (i = 0; i < 4; i++)
    modedata.head[i] = 0;

  modecommand.opcode	= SCSI_CMD_MSE;
  modecommand.b1	= 0;
  modecommand.b2	= 0x0E;
  modecommand.b3	= 0;
  modecommand.b4	= MAX_DATA_LEN;
  modecommand.control	= 0;

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			(UBYTE *) &modecommand, sizeof (modecommand),
			(SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
      return;
    }

  for (j = (scsi_data[0]+1), i = scsi_data[3] + 4; i < j; i += scsi_data[i+1] + 2)
    {
      if (vol0 == -1 && vol1 == -1 && vol2 == -1 && vol3 == -1)
	{
	  if ((scsi_data[i] & 0x3f) == 0x0e)
	    {
	      printf ("Immed    : %s\n", (scsi_data[i+2] & 4)? "yes" : "no");
	      printf ("SOTC     : %s\n", (scsi_data[i+2] & 2)? "yes" : "no");
	      printf ("APRVal   : %svalid\n", (scsi_data[i+5] & 0x80)? "" : "in");
	      if (scsi_data[i+5] & 0x80)
		{
		  printf ("LBA Format: ");
		  if ((scsi_data[i+5] & 0x0f) == 0)
		    printf ("1 second\n");
		  else if ((scsi_data[i+5] & 0x0f) == 8)
		    printf ("1/256 second\n");
		  else
		    printf ("%hu (reserved)\n", scsi_data[i+5] & 0x0f);
		  printf ("LBPS      : %u\n", scsi_data[i+6]<<8 + scsi_data[i+7]);
		}
	    }
	  if (scsi_data[i+8] & 0x0f)
	    printf ("Output 0 : %hu\n", scsi_data[i+8] & 0x0f);
	  else
	    printf ("Output 0 : muted\n");
	  printf ("Volume 0 : %hu\n", scsi_data[i+9]);
	  if (scsi_data[i+10] & 0x0f)
	    printf ("Output 1 : %hu\n", scsi_data[i+10] & 0x0f);
	  else
	    printf ("Output 1 : muted\n");
	  printf ("Volume 1 : %hu\n", scsi_data[i+11]);
	  if (scsi_data[i+12] & 0x0f)
	    printf ("Output 2 : %hu\n", scsi_data[i+12] & 0x0f);
	  else
	    printf ("Output 2 : muted\n");
	  printf ("Volume 2 : %hu\n", scsi_data[i+13]);
	  if (scsi_data[i+14] & 0x0f)
	    printf ("Output 3 : %hu\n", scsi_data[i+14] & 0x0f);
	  else
	    printf ("Output 3 : muted\n");
	  printf ("Volume 3 : %hu\n", scsi_data[i+15]);
	}
      
      /* should be 16 bytes */
      memcpy (&modedata.page, &scsi_data[i], 16);
    }
  if (vol0 > -1 || vol1 > -1 || vol2 > -1 || vol3 > -1)
    {
      modedata.page = 0x0e;
      modedata.plength = 0x0e;

      if (vol0 >= 0)
	modedata.vol0 = vol0;
      if (vol1 >= 0)
	modedata.vol1 = vol1;
      if (vol2 >= 0)
	modedata.vol2 = vol2;
      if (vol3 >= 0)
	modedata.vol3 = vol3;

      modecommand.opcode	= SCSI_CMD_MSL;
      modecommand.b1		= 0x10;
      modecommand.b2		= 0;
      modecommand.b3		= 0;
      modecommand.b4		= sizeof (modedata);
      modecommand.control	= 0;

      if ((err = DoScsiCmd ((UBYTE *) &modedata, sizeof(modedata),
			    (UBYTE *) &modecommand, sizeof (modecommand),
			    (SCSIF_WRITE | SCSIF_AUTOSENSE))) != 0)
	{
	  fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
	  return;
	}
    }
}

/*********************************************************************
 *
 *	function to make an inquiry
 *
 */

void
inquiry (BOOLEAN parsedoutput)
{
  static SCSICMD6 command =
  {
    SCSI_CMD_INQ,		/* 0x12 INQUIRY */
    PAD,			/* Bits 7-5 Logical Unit Number | Bits 4-1 Reserved | Bit 0 EVPD */
    PAD,			/* Page Code */
    PAD,			/* Reserved */
    0,				/* Allocation length */
    PAD				/* Control */
  };
  static int err;

  command.b4 = MAX_DATA_LEN;	/* Allocation length = max. data length */

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) == 0)
    {
      int rem = scsi_cmd[0].scsi_Actual;

      if (parsedoutput == FALSE)
	{
	  rawhexasciioutput (scsi_data, rem, 0);
	}
      else
	/* parsed output */
	{
	  static IDTOSTRING devicetype[] =
	  {
	    0x00, "Direct-access device (e.g., magnetic disk)",
	    0x01, "Sequential-access device (e.g., magnetic tape)",
	    0x02, "Printer device",
	    0x03, "Processor device",
	    0x04, "Write-once device (e.g., some optical disks)",
	    0x05, "CD-ROM device",
	    0x06, "Scanner device",
	    0x07, "Optical memory device (e.g., some optical disks)",
	    0x08, "Medium Changer device (e.g., jukeboxes)",
	    0x09, "Communications device",
	    0x0A, "Defined by ASC IT8 (Graphic Arts Pre-Press Devices)",
	    0x0B, "Defined by ASC IT8 (Graphic Arts Pre-Press Devices)",
	    -1, "Reserved, unknown or no device type"
	  };

	  static IDTOSTRING ansiversion[] =
	  {
	    0x00, "The device might or might not comply to an ANSI-approved standard.",
	    0x01, "The device complies to ANSI X3.131-1986 (SCSI-1).",
	    0x02, "The device complies to (SCSI-2).",
	    -1, "Reserved",
	  };

	  static IDTOSTRING responseformat[] =
	  {
	    0x00, "SCSI-1",
	    0x01, "CCS",
	    0x02, "SCSI-2",
	    -1, "Reserved",
	  };

	  printf ("Peripherial qualifier: %ld\n", (scsi_data[0] & 0xE0) >> 5);
	  printf ("Peripherial device type: $%lx, %s\n", (scsi_data[0] & 0x1F), id2string ((scsi_data[0] & 0x1F), devicetype));

	  printf ("Removable medium: %s\n", (scsi_data[1] & 0x80) ? "yes" : "no");
	  printf ("Device type modifier: %lx\n", scsi_data[1] & 0x7F);
	  printf ("ISO Version: %lx\n", (scsi_data[2] & 0xC0) >> 6);
	  printf ("ECMA Version: %lx\n", (scsi_data[2] & 0x38) >> 3);

	  printf ("ANSI-Approved Version: %ld, %s\n", scsi_data[2] & 0x07, id2string ((scsi_data[2] & 0x07), ansiversion));

	  printf ("AENC: %s\n", (scsi_data[3] & 0x80) ? "yes" : "no");
	  printf ("TrmIOP: does%s support TERMINATE I/O PROCESs message\n", (scsi_data[3] & 0x40) ? "" : "n't");

	  printf ("Response data format: $%lx, conforms to %s\n", scsi_data[3] & 0x0F, id2string ((scsi_data[3] & 0x0F), responseformat));
	  printf ("Additional length: $%lx\n", scsi_data[4]);
	  printf ("INQUIRY[5-6] (Reserved): $%lx, $%lx\n", scsi_data[5], scsi_data[6]);
	  printf ("RelAdr: does%s support relative addressing\n", (scsi_data[7] & 0x80) ? "" : "n't");
	  printf ("WBus32: does%s support 32 wide data transfers\n", (scsi_data[7] & 0x40) ? "" : "n't");
	  printf ("WBus16: does%s support 16 wide data transfers\n", (scsi_data[7] & 0x20) ? "" : "n't");
	  printf ("Sync: does%s support synchronous transfers\n", (scsi_data[7] & 0x10) ? "" : "n't");
	  printf ("Linked: does%s support linked commands\n", (scsi_data[7] & 0x08) ? "" : "n't");
	  printf ("CmdQue: does%s support tagged command queueing\n", (scsi_data[7] & 0x02) ? "" : "n't");
	  printf ("SftRe: responds to RESET condition with %s RESET alternative\n", (scsi_data[7] & 0x01) ? "soft" : "hard");
	  printf ("Vendor identification: %.8s\n", &scsi_data[8]);
	  printf ("Product identification: %.16s\n", &scsi_data[16]);
	  printf ("Product revision level: %.4s\n", &scsi_data[32]);
	  printf ("Vendor specific: %.20s\n", &scsi_data[36]);
	  printf ("Reserved: %.35s\n", &scsi_data[56]);
	}
    }
  else
    /* error */
    {
      fprintf (stderr, "Error : err=%ld , %s\n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to read disk capacity
 *
 */

void
read_capacity (BOOLEAN parsed)
{
  static struct CMD_READ_CAPACITY
  {
    UBYTE cmd;
    UBYTE pad_a;
    ULONG lba;
    UBYTE pad_b[2];
    UBYTE pmi;
    UBYTE pad_c;
  } command =
  {
    SCSI_CMD_RCP,	/* READ CAPACITY = READ CD-ROM CAPACITY */
    PAD,		/* LUN | rsrvd. | RelAddr */
    0,			/* start from sec 0 */
    PAD, PAD,
    0,			/* PMI */
    PAD
  };

  int err;

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) == 0)
    {
      if (parsed == TRUE)
        /* output parsed data */
        {
          ULONG sec_no = *((ULONG *) & scsi_data[0]);
          ULONG sec_size = *((ULONG *) & scsi_data[4]);

          printf ("Max Sec = %7ld , sec size = %4ld (capacity = %7ld KB)\n",
                  sec_no, sec_size, (sec_no * sec_size) / 1024);
        }
      else
        /* output raw data */
        {
          rawhexasciioutput (scsi_data, 8, 0);
        }
    }
  else
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to read audio CD TOC (table of contents)
 *
 */

void
read_toc (int toclong)
{
  static IDTOSTRING Qfield[] =
  {
    0x00, "Sub-channel Q mode information not supplied.",
    0x01, "Sub-channel Q encodes current position data.",
    0x02, "Sub-channel Q encodes media catalog number.",
    0x03, "Sub-channel Q encodes ISRC.",
    -1, "Reserved"
  };

  static IDTOSTRING Qfieldshort[] =
  {
    0x00, "not.suppl.",
    0x01, "cur.posdt.",
    0x02, "med.cat.#.",
    0x03, "ISRC",
    -1, "reserved"
  };

  static SCSICMD10 command =
  {
    SCSI_CMD_READTOC,		/* SCSI command read table of contents */
    0,
    PAD, PAD, PAD, PAD,
    0,				/* starting track */
    0x03, 0x24,			/* max. TOC data length on current CD-ROMs 804 bytes
				   or 100 TOC track descriptors */
    PAD
  };

  int err, tocsize;
  UBYTE *tocptr;

  if ((err = DoScsiCmd ((UBYTE *) toc_buf, MAX_TOC_LEN,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) == 0)
    {
      tocsize = (toc_buf[0] << 8) | toc_buf[1];		/* first word encodes length */

      if (toclong == 0)
        /* display TOC raw form */
        {
          rawhexasciioutput(toc_buf, (toc_buf[0]<<8 | toc_buf[1]), 0);
        }
      else if (toclong == 1)
	/* display TOC short form */
	{
          printf ("TOC len: %d\n", tocsize);
          printf ("First/last track: %d, %d\n", toc_buf[2], toc_buf[3]);

	  if (tocsize >= 2)	/* TOC Data Length - FTN - LTN */
	    tocsize -= 2;

	  for (tocptr = &toc_buf[4]; tocptr < (&toc_buf[4] + tocsize); tocptr += 8)
	    {
	      printf ("#%3.3d: ADR:%10.10s %-15.15s Dig.copy.%5.5s. %-9.9s %cChan. %ld\n",
		      tocptr[2],
		      id2string (((tocptr[1] >> 4) & 0x0F), Qfieldshort),
		      (tocptr[1] & 0x01) ? "pre-emphasis" : "no-pre-emphasis",
		      (tocptr[1] & 0x02) ? "prmtd" : "prohb.",
		      (tocptr[1] & 0x04) ? "Data tr." : "Audio tr.",
		      (tocptr[1] & 0x08) ? '4' : '2',
		      ((tocptr[4] << 24) | (tocptr[5] << 16) | (tocptr[6] << 8) | (tocptr[7]))
		);
	    }
	}
      else if (toclong == 2)
	/* display TOC long form */
	{
          printf ("TOC len: %d\n", tocsize);
          printf ("First/last track: %d, %d\n", toc_buf[2], toc_buf[3]);

	  if (tocsize >= 2)	/* TOC Data Length - FTN - LTN */
	    tocsize -= 2;

	  for (tocptr = &toc_buf[4]; tocptr < (&toc_buf[4] + tocsize); tocptr += 8)
	    {

	      printf ("Track number: %d\n", tocptr[2]);

	      printf (" ADR: $%lx: %s\n", ((tocptr[1] >> 4) & 0x0F), id2string (((tocptr[1] >> 4) & 0x0F), Qfield));

	      printf (" Audio with%s pre-emphasis.  ", (tocptr[1] & 0x01) ? "" : "out");
	      printf (" Digital copy %s\n", (tocptr[1] & 0x02) ? "permitted" : "prohibited");
	      printf (" %s track.  ", (tocptr[1] & 0x04) ? "Data" : "Audio");
	      printf (" %s channel audio.  ", (tocptr[1] & 0x08) ? "Four" : "Two");

	      printf (" Absolute address: %ld\n", ((tocptr[4] << 24) | (tocptr[5] << 16) | (tocptr[6] << 8) | (tocptr[7])));
	    }
	}
      else
	/* toclong is neither 0,1 or 2 - this should never happen! */
	{
	  fprintf (stderr, "Error : internal error in read_toc!\n");
	}
    }
  else
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to play audio
 *
 */

void
play_audio (int starttrack, int startindex, int endtrack, int endindex)
{
  static SCSICMD10 command =
  {
    SCSI_CMD_PLAYAUDIOTRACKINDEX,	/* Play audio track */
    PAD,			/* LUN */
    PAD,			/* Reserved */
    PAD,			/* Reserved */
    0,				/* Starting Track */
    0,				/* Starting Index */
    PAD,			/* Reserved */
    0,				/* Ending Track */
    0,				/* Ending Index */
    PAD				/* Control */
  };

  int err;

  command.b4 = starttrack;	/* set audio track to play */
  command.b5 = startindex;
  command.b7 = endtrack;	/* ending track */
  command.b8 = endindex;

  finddrivebrand ();		/* figure out drivetype */

  if (whatdrive == APPLECD150)
    {
      command.opcode = 0xC9;	/* Apple CD-150 / Pioneer Opcode for playing audio tracks */
    }

  if ((err = DoScsiCmd ((UBYTE *) scsi_data, MAX_DATA_LEN,
			(UBYTE *) & command, sizeof (command),
			(SCSIF_READ | SCSIF_AUTOSENSE))) != 0)
    {
      fprintf (stderr, "Error : err=%ld , %s \n", err, sense_errs (0, err));
    }
}

/*********************************************************************
 *
 *	function to compare two binary strings
 *
 *	returns FALSE if different
 */

int
gcomp (char *p1, char *p2, int len)
{
  while (len--)
    {
      if (*p1++ != *p2++)
	return (FALSE);
    }
  return (TRUE);
}

/*********************************************************************
 *
 * searches DeviceList for a device name with a given string in it.
 * - if found returns with a pointer to it, else NULL
 */

extern struct ExecBase *SysBase;

UBYTE *
GetDevName (char *grep)
{
  LIST *lh = (LIST *) SysBase->DeviceList.lh_Head;
  NODE *ln;

  for (ln = lh->lh_Head; ln->ln_Succ; ln = ln->ln_Succ)
    {
      UBYTE *p = ln->ln_Name;

      while (*p != '.')
	{
	  if (strncmp (p, grep, 4) == 0)
	    {
	      return (ln->ln_Name);
	    }
	  ++p;
	}
    }

  return (NULL);		/* not found */
}

/*********************************************************************
 *
 *	Break (^C) function
 *
 */

int
breakcheck (void)
{
  int zz = SetSignal (0L, SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C;
  if (zz)
    {
      fprintf (stderr, "\n***BREAK: ^C\n");
    }
  return (zz);
}

#ifdef __SASC
/*********************************************************************
 *
 *  tell SAS to turn of CTRL-C checking
 */
void __regargs
__chkabort (void)
{
}

#endif

/*********************************************************************
 *
 *	function to send a scsi command (uses asynchronous I/O)
 *
 */
void
SendScsiCmd (int req, UBYTE * data, int datasize, UBYTE * cmd, int cmdsize, UBYTE flags)
{

  io_ptr[req]->io_Length = sizeof (SCSICMD);
  io_ptr[req]->io_Data = (APTR) & scsi_cmd[req];
  io_ptr[req]->io_Command = HD_SCSICMD;

  scsi_cmd[req].scsi_Data = (APTR) data;
  scsi_cmd[req].scsi_Length = datasize;
  scsi_cmd[req].scsi_SenseActual = 0;
  scsi_cmd[req].scsi_SenseData = scsi_sense[req];
  scsi_cmd[req].scsi_SenseLength = SENSE_LEN;
  scsi_cmd[req].scsi_Command = cmd;
  scsi_cmd[req].scsi_CmdLength = cmdsize;
  scsi_cmd[req].scsi_Flags = flags;

  (void) SendIO ((struct IORequest *) io_ptr[req]);

}

/*********************************************************************
 *
 *	function to wait for an asynchronous scsi command
 *
 */
int
WaitScsiCmd (int req)
{
  int i;

  WaitIO ((struct IORequest *) io_ptr[req]);

  if (scsi_cmd[req].scsi_SenseActual)
    {
      fprintf (stderr, "SENSE_DATA:");
      for (i = 0; i < scsi_cmd[req].scsi_SenseActual; i++)
	{
	  fprintf (stderr, " %02x", scsi_cmd[req].scsi_SenseData[i]);
	}
      fprintf (stderr, "\n");
    }
  return (io_ptr[req]->io_Error);
}

/*********************************************************************
 *
 *	function to use a scsi command
 *
 */
int
DoScsiCmd (UBYTE * data, int datasize, UBYTE * cmd, int cmdsize, UBYTE flags)
{
  int i;

  io_ptr[0]->io_Length = sizeof (SCSICMD);
  io_ptr[0]->io_Data = (APTR) & scsi_cmd[0];
  io_ptr[0]->io_Command = HD_SCSICMD;

  scsi_cmd[0].scsi_Data = (APTR) data;
  scsi_cmd[0].scsi_Length = datasize;
  scsi_cmd[0].scsi_SenseActual = 0;
  scsi_cmd[0].scsi_SenseData = scsi_sense[0];
  scsi_cmd[0].scsi_SenseLength = SENSE_LEN;
  scsi_cmd[0].scsi_Command = cmd;
  scsi_cmd[0].scsi_CmdLength = cmdsize;
  scsi_cmd[0].scsi_Flags = flags;

  (void) DoIO ((struct IORequest *) io_ptr[0]);

  if (scsi_cmd[0].scsi_SenseActual)
    {
      fprintf (stderr, "SENSE_DATA:");
      for (i = 0; i < scsi_cmd[0].scsi_SenseActual; i++)
	{
	  fprintf (stderr, " %02x", scsi_cmd[0].scsi_SenseData[i]);
	}
      fprintf (stderr, "\n");
    }
  return (io_ptr[0]->io_Error);
}

/*********************************************************************
 *
 *	function to return an error string
 *
 *
 */

UBYTE *
err_str (int err)
{

  static UBYTE *errors[] =
  {
    " cannot issue SCSI command to self ",
    " DMA error ",
    " illegal or unexpected SCSI phase ",
    " SCSI parity error ",
    " Select timed out ",
    " status and/or sense error "
  };

  err -= 40;

  if ((err < 0) || (err > 5))
    return ("Error out-of-range");
  else
    return (errors[err]);
}

/*********************************************************************
 *
 *	usage function
 *
 *
 */

void
usage (void)
{
  static char *zz[] =
  {
    "Usage: SCSIutil [-dscsi_dev] <scsi_id> <command>\n",
    " -c[r]                 : Read capacity [raw]\n",
    " -d<l|r|s> sec blks sc : Read 16 bit digital audio(start sector/# blocks)(2)\n",
    " -D<l|r> sec blks      : Read  8 bit digital audio(left or right channel)(3)\n",
    " -e <0|1>              : Change medium (0=eject, 1=load)\n",
    " -i[r]                 : Inquiry [raw]\n",
    " -h[r] blk             : Read CD-ROM data block address header\n",
    " -l <0|1>              : Allow/Prevent medium removal\n",
    " -m <0|1>              : Stop/Start motor {0=stop, 1=start}\n",
    " -o[r] contr page      : Mode sense (contr = 0-3))\n",
    " -p st si et ei        : Play audio CD track (1-99), index (1-99)\n",
    " -r[t] sec_no          : Read sectors [use trackdisk.device]\n",
    " -s sec_no             : Seek to sector (5)\n",
    " -t[r|l]               : Display TOC of an audio CD [raw|long]\n",
    " -u[r] chan fmt track  : Read CD sub-channel information [raw] (6)\n",
    " -v [vl0 vl1 vl2 vl3]  : Set output volume channels 0-3 (7)\n",
#ifdef USE8SVX
    " -8<l|r|s> sec blks   : Read digital audio -> 8SVX (left, right or stereo) (7)\n",
#endif	/* USE8SVX */
    "\n",
    "Note 1: usually scsi_id = (BOARD * 100) + (LUN * 10) + SCSI_TARGET_ID\n",
    "     2: with 's' returns LRLRLR pairs of stereo audio, 2352 bytes per block\n",
    "        sc = Apple subcode (0=2352, 1=2368, 2=2448, 3=96 byte/block)\n",
    "     3: converted to 8 bit audio (-d and -D work with Sony CDU 561 & 8003)\n",
    "     4: contr 0: current, 1: changeable, 2: default, 3: saved values\n",
    "     5: to park heads, try sec_no of -1\n",
    "     6: Q-channel = 64, fmt: 0=Sub-Q Channel data,1=current CD-ROM pos.,\n",
    "        2=Media Catalog Number (UPC/Bar Code),3=Track ISRC\n",
    "     7: use -1 to leave volume of channel as it is, without argument shows\n",
    "        current volume settings\n",
#ifdef USE8SVX
    "     8: output 8SVX IFF (in case of stereo needs to read the CD twice)\n",
#endif	/* USE8SVX */
    ""				/* TERM */
  };

  int j = 0;

  fprintf (stderr, "SCSIutil V%s [%s : %s] - written by Gary Duncan\n",
	   VERSION, __DATE__, __TIME__, pname);
  fprintf (stderr, "         (gduncan@philips.oz.au) and Heiko Rath (hr@brewhr.swb.de)\n");

  while (*zz[j++])
    fprintf (stderr, "%s", zz[j - 1]);
}

/*********************************************************************
 *
 *	sense_errs function ; prints sense errors
 *
 *
 */

UBYTE *
sense_errs (int req, int err)
{
  typedef struct
  {
    BYTE code;
    BYTE sense;
    UBYTE *ptr;
  } S_ERRS;

/*
 *	only the likely, interesting ones filled in, e.g media errors
 */
  static S_ERRS x[] =
  {
    0x00, 0x00, "No error",
    0x01, 0x04, "?",
    0x02, 0x04, "?",
    0x03, 0x04, "?",
    0x04, 0x02, "?",
    0x06, 0x04, "?",
    0x09, 0x04, "?",
    0x10, 0x03, "?",
    0x10, 0x04, "?",
    0x11, 0x03, "?",
    0x12, 0x03, "?",
    0x13, 0x03, "?",
    0x14, 0x03, "?",
    0x15, 0x04, "Seek error ",
    0x17, 0x01, "?",
    0x18, 0x01, "?",
    0x19, 0x03, "?",
    0x1A, 0x05, "?",
    0x20, 0x05, "Invalid command op code",
    0x21, 0x05, "Illegal sector address",
    0x24, 0x05, "?",
    0x25, 0x05, "Invalid LUN",
    0x26, 0x05, "Invalid field in parameter list",
    0x29, 0x06, "?",
    0x2A, 0x06, "?",
    0x31, 0x03, "?",
    0x32, 0x01, "?",
    0x32, 0x03, "?",
    0x40, 0x04, "?",
    0x41, 0x04, "?",
    0x42, 0x04, "Power-on diagnostic failure",
    0x43, 0x04, "?",
    0x45, 0x04, "Select / reselect failure ",
    0x47, 0x04, "SCSI Interface Parity Error",
    0x48, 0x0B, "?",
    0x49, 0x0B, "Illegal message drive can't support",
    -1, -1, "ILLEGAL sense!!"
  };

  int j = 0;
  UBYTE *p;
  char sense;
  char code;

  /*
   *	verify that sense data looks valid
   */
  if (((scsi_cmd[req].scsi_Status & 2) == 0) ||
      (scsi_cmd[req].scsi_SenseActual < OFFS_KEY))
    {
      return ("");
    }
  sense = scsi_cmd[req].scsi_SenseData[OFFS_KEY] & 0xF;
  code = scsi_cmd[req].scsi_SenseData[OFFS_CODE];

  do
    {
      p = x[j].ptr;
      if ((x[j].code == code) && (x[j].sense == sense))
	break;
  } while (x[j++].code != -1);

  return (p);
}

/*********************************************************************
 *
 *	id2string function ; return pointer to string for matching id
 *
 */

UBYTE *
id2string (int id, IDTOSTRING * idtable)
{
  int j = 0;
  UBYTE *p;

  do
    {
      p = idtable[j].ptr;
      if (idtable[j].code == id)
	break;
  } while (idtable[j++].code != -1);
  return p;
}
