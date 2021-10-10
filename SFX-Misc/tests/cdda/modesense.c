/*
 * Copyright (C) 1990 Commodore-Amiga, Inc.
 * All rights reserved
 */

/*
 * The following program demonstrates the use of the HD_SCSICmd to send a
 * MODE SENSE to a unit on the requested device (default scsi.device).  This
 * code can be easily modified to send other commands to the drive.
 *
 * compile under Lattice using 'lc -cfist -L modesense'
 */

#include <stdio.h>
#include <stdlib.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <devices/timer.h>
#include <devices/scsidisk.h>
#include <libraries/dosextens.h>

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>

#define BUFSIZE 256

UBYTE *buffer;			/* a data buffer used for mode sense data */
struct IOStdReq SCSIReq;	/* a standard IORequest structure */
struct SCSICmd Cmd;		/* where the actual SCSI command goes */
UBYTE  Sense[20];		/* buffer for request sense data */
struct MsgPort Port;		/* our ReplyPort */

void ShowSenseData(void);

static UBYTE TestReady[] = { 0,0,0,0,0,0 };	/* not used but here for  */
static UBYTE StartUnit[] = { 0x1b,0,0,0,1,0 };	/* illustration of other  */
static UBYTE StopUnit[] =  { 0x1b,0,0,0,0,0 };	/* commands.		  */

static UBYTE ModeSense[]={ 0x1a,0,0xff,0,254,0 }; /* the command being sent */

void main(int argc, char **argv) {
    int unit,tval,i;
    char *dname = "scsi.device";
    UBYTE *tbuf;

    if( (argc < 2) || (argc > 3) ) {
	printf("Usage: %s unit [xxxx.device]\n",argv[0]);
	exit(100);
    }

    unit = atoi( argv[1] );
    if ( argc == 3 ) dname = argv[2];

    buffer = (UBYTE *) AllocMem( BUFSIZE, MEMF_PUBLIC|MEMF_CLEAR );
    if (!buffer) { printf("Couldn't get memory\n"); exit(100); }

    Port.mp_Node.ln_Pri = 0;			/* setup the ReplyPort */
    Port.mp_SigBit      = AllocSignal(-1);
    Port.mp_SigTask     = (struct Task *)FindTask(0);
    NewList( &(Port.mp_MsgList) );

    SCSIReq.io_Message.mn_ReplyPort = &Port;

    if (OpenDevice( dname, unit, &SCSIReq, 0))  {
	printf("Couldn't open unit %ld on %s\n",unit,dname);
	FreeMem( buffer,BUFSIZE );
	exit(100);
    }

    SCSIReq.io_Length  = sizeof(struct SCSICmd);
    SCSIReq.io_Data    = (APTR)&Cmd;
    SCSIReq.io_Command = HD_SCSICMD;	/* the command we are sending	*/

    Cmd.scsi_Data = (UWORD *)buffer;	/* where we put mode sense data	*/
    Cmd.scsi_Length = 254;		/* how much we will accept	*/
    Cmd.scsi_CmdLength = 6;		/* length of the command	*/
    Cmd.scsi_Flags = SCSIF_AUTOSENSE|SCSIF_READ;
					/* do automatic REQUEST_SENSE	*/
					/* set expected data direction 	*/
    Cmd.scsi_SenseData =(UBYTE *)Sense;	/* where sense data will go	*/
    Cmd.scsi_SenseLength = 18;		/* how much we will accept	*/
    Cmd.scsi_SenseActual = 0;		/* how much has been received	*/

    Cmd.scsi_Command=(UBYTE *)ModeSense;/* issuing a MODE_SENSE command	*/
    DoIO( &SCSIReq );			/* send it to the device driver	*/
    if(Cmd.scsi_Status) ShowSenseData(); /* if bad status then show it	*/

    else {
	printf("\nBlock descriptor header\n");
	printf("=======================\n");
	printf("Mode Sense data length  = %d\n",(short)buffer[0]);
	printf("Block descriptor length = %d\n",(short)buffer[3]);
	tbuf = &buffer[4];
	printf("Density code            = %d\n",(short)tbuf[0]);
	tval = (tbuf[1]<<16) + (tbuf[2]<<8) + tbuf[3];
	printf("Number of blocks        = %ld\n",tval);
	tval = (tbuf[5]<<16) + (tbuf[6]<<8) + tbuf[7];
	printf("Block size              = %ld\n",tval);

	tbuf += buffer[3];		/* move to page descriptors */

	while( (tbuf - buffer) < buffer[0] ) {
	    switch ( tbuf[0] & 0x7f ) {
	    case 1:
		printf("\nError Recovery Parameters\n");
		printf("=========================\n");
		printf("Page length             = %d\n",(short)tbuf[1]);
		printf("DISABLE CORRECTION      = %d\n",(short)tbuf[2]&1);
		printf("DISABLE XFER ON ERROR   = %d\n",(short)(tbuf[2]>>1)&1);
		printf("POST ERROR              = %d\n",(short)(tbuf[2]>>2)&1);
		printf("ENABLE EARLY CORRECTION = %d\n",(short)(tbuf[2]>>3)&1);
		printf("READ CONTINUOUS         = %d\n",(short)(tbuf[2]>>4)&1);
		printf("TRANSFER BLOCK          = %d\n",(short)(tbuf[2]>>5)&1);
		printf("AUTO READ REALLOCATION  = %d\n",(short)(tbuf[2]>>6)&1);
		printf("AUTO WRITE REALLOCATION = %d\n",(short)(tbuf[2]>>7)&1);
		printf("Retry count             = %d\n",(short)tbuf[3]);
		printf("Correction span         = %d\n",(short)tbuf[4]);
		printf("Head offset count       = %d\n",(short)tbuf[5]);
		printf("Data strobe offset count= %d\n",(short)tbuf[6]);
		printf("Recovery time limit     = %d\n",(short)tbuf[7]);
		tbuf += tbuf[1]+2;
		break;

	    case 2 :
		printf("\nDisconnect/Reconnect Control\n");
		printf("============================\n");
		printf("Page length             = %d\n",(short)tbuf[1]);
		printf("Buffer full ratio       = %d\n",(short)tbuf[2]);
		printf("Buffer empty ratio      = %d\n",(short)tbuf[3]);
		tval = (tbuf[4]<<8)+tbuf[5];
		printf("Bus inactivity limit    = %d\n",tval);
		tval = (tbuf[6]<<8)+tbuf[7];
		printf("Disconnect time limit   = %d\n",tval);
		tval = (tbuf[8]<<8)+tbuf[9];
		printf("Connect time limit      = %d\n",tval);
		tval = (tbuf[10]<<8)+tbuf[11];
		printf("Maximum burst size      = %d\n",tval);
		printf("Disable disconnection   = %d\n",(short)tbuf[12]&1);
		tbuf += tbuf[1]+2;
		break;

	    case 3:
		printf("\nDevice Format Parameters\n");
		printf("========================\n");
		printf("Page length             = %d\n",(short)tbuf[1]);
		tval = (tbuf[2]<<8)+tbuf[3];
		printf("Tracks per zone         = %d\n",tval);
		tval = (tbuf[4]<<8)+tbuf[5];
		printf("Alternate sectors/zone  = %d\n",tval);
		tval = (tbuf[6]<<8)+tbuf[7];
		printf("Alternate tracks/zone   = %d\n",tval);
		tval = (tbuf[8]<<8)+tbuf[9];
		printf("Alternate tracks/volume = %d\n",tval);
		tval = (tbuf[10]<<8)+tbuf[11];
		printf("Sectors per track       = %d\n",tval);
		tval = (tbuf[12]<<8)+tbuf[13];
		printf("Bytes per sector        = %d\n",tval);
		tval = (tbuf[14]<<8)+tbuf[15];
		printf("Interleave              = %d\n",tval);
		tval = (tbuf[16]<<8)+tbuf[17];
		printf("Track skew factor       = %d\n",tval);
		tval = (tbuf[18]<<8)+tbuf[19];
		printf("Cylinder skew factor    = %d\n",tval);

		tbuf += tbuf[1]+2;
		break;

	    case 4:
		printf("\nDrive Geometry Parameters\n");
		printf("=========================\n");
		printf("Page length             = %d\n",(short)tbuf[1]);
		tval = (tbuf[2]<<16)+(tbuf[3]<<8)+tbuf[4];
		printf("Number of cylinders     = %ld\n",tval);
		printf("Number of heads         = %d\n",(short)tbuf[5]);
		tval = (tbuf[6]<<16)+(tbuf[6]<<8)+tbuf[8];
		printf("Start write precomp     = %ld\n",tval);
		tval = (tbuf[9]<<16)+(tbuf[10]<<8)+tbuf[11];
		printf("Start reduced write     = %ld\n",tval);
		tval = (tbuf[12]<<8)+tbuf[13];
		printf("Drive step rate         = %d\n",tval);
		tval = (tbuf[14]<<16)+(tbuf[15]<<8)+tbuf[16];
		printf("Landing zone cylinder   = %ld\n",tval);

		tbuf += tbuf[1]+2;
		break;

	    default:
		printf("\nVendor Unique Page Code %2x\n",(short)tbuf[0]);
		printf("==========================\n");
		for( i=0; i<=tbuf[1]+1; i++ )
		    printf("%x ",(short)tbuf[i]);
		printf("\n");
		tbuf += tbuf[1]+2;
	    }
	}
    }
    CloseDevice( &SCSIReq );
    FreeMem( buffer, BUFSIZE );
    FreeSignal(Port.mp_SigBit);
}

void ShowSenseData(void)
{
int i;

    for(i=0; i<18; i++)
        printf("%x ",(int)Sense[i]);
    printf("\n");
}

