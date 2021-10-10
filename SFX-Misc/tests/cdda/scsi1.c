/****h* cdda/scsi1.c [1.00] *
*
*  NAME
*    scsi1.c
*  COPYRIGHT
*    $VER: scsi1.c 1.00 (12.06.99) © by Stefan Kost 1999-1999
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    06.Jun.1999
*  MODIFICATION HISTORY
*    12.Jun.1999	V 1.00	most recent version
*    06.Jun.1999	V 0.00	initial version
*  NOTES
*
*******
*/

//-- includes -----------------------------------------------------------------

//-- Ansi

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-- AmigaOS

#include <exec/types.h>
#include <cmacros.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <devices/scsidisk.h>
//#include <devices/scsicmd.h>
#include "scsicmd.h"
#include <exec/exec.h>
#include <libraries/listtools.h>
#include <workbench/startup.h>

#include <proto/dos.h>			/* Prototypes für Libraryfunctions */
#include <proto/exec.h>
#include <proto/listtools.h>
#include <proto/utility.h>

//-- Local

#include "project.h"

//-- DebugTools

#define DEBUG_SRCPATH "data:source/sfx/sfx-misc/test/cdda/"
#include <debug/dt_decl.h>

//-- prototypes ---------------------------------------------------------------

void			AppInit(void);
void			AppDone(UBYTE retCode);
void			AppRun(void);

BOOL			ParseArguments(void);

BOOL 			SCSIDeviceInit(STRPTR devname,ULONG unit);
void			SCSIDeviceDone(void);

BOOL			SCSICmdSubmit(void);

//-- globals ------------------------------------------------------------------

UBYTE *VersTag="\0$VER:"PRJ_VERS;

static const char ARG_TEMPLATE[]="DEVICE,UNIT/N";
enum RDArgsEnum				/* Argument numbers corresponding to the command line parsing template. */
{	
	ARGID_DEVICE=0,
	ARGID_UNIT,
	ARG_COUNT
};

struct Library 	*ListToolsBase=NULL;
BOOL			 SCSIDevice=FALSE;

//-- Parameter

#define MAXDEVICENAME 80
char DeviceName[MAXDEVICENAME];	/* Name of the device driver to use, usually scsi.device. */
LONG UnitNumber;				/* SCSI disk unit to use, xyz decimal format, x is board number for multiple
								   SCSI controller boards (usually 0), y is LUN of subdevice (usually 0), z is
								   SCSI ID of device. */
int LUN;						/* Logical unit number derived from UnitNumber, 0 to 9. */

struct MsgPort *MyMessagePort=NULL;			/* For receiving replies to device requests. */
struct IOStdReq *MyIORequest=NULL;			/* Commands to scsi.device are put into this IO request record. */
struct SCSICmd *MySCSICommand=NULL;			/* The IO request points to this record which points to the various buffers
											   needed for running the command.  If the buffer pointer fields in this record
											   are NULL then the buffers haven't been allocated yet. */
#define SCSI_BUFFER_SIZES 1024				/* All the buffers (data, command, sense) in MySCSICommand are this size and
											   are allocated from DMA-able memory.  Though scsi.device is reported to use
											   programmed IO rather than DMA for transfers <256 bytes. */
#define SCSI_MAX_SIZE_6		254
#define SCSI_MAX_SIZE_10	1022
#define SCSI_MAX_SIZE_12	1022
#define SCSI_MAX_SIZE		1022
BOOL DeviceOpen=FALSE;						/* TRUE if the scsi.device was OpenDevice'd and needs to be closed when the
											   program exits. */

//-- definitions --------------------------------------------------------------

void AppInit(void)
{
	if(!(ListToolsBase=OpenLibrary(ListToolsName,ListToolsVersion))) goto Error;

	if(!(MyMessagePort	=CreateMsgPort()))															{ printf("Out of memory for CreateMsgPort.\n");goto Error; }
	if(!(MyIORequest	=(struct IOStdReq *)CreateIORequest(MyMessagePort,sizeof(struct IOStdReq)))){ printf("Out of memory for CreateIORequest.\n");goto Error; }
	MSG1("Messagetype : %ld",MyIORequest->io_Message.mn_Node.ln_Type);
	if(!(MySCSICommand	=AllocMem(sizeof(struct SCSICmd),MEMF_PUBLIC|MEMF_CLEAR)))					{ printf ("Out of memory for SCSICmd record.\n");goto Error; }

	/* Set up for a direct SCSI command through the scsi.device. */
	MyIORequest->io_Command			=HD_SCSICMD;
	MyIORequest->io_Data			=MySCSICommand;
	MyIORequest->io_Length			=sizeof(struct SCSICmd);
	MySCSICommand->scsi_Data		=AllocMem(SCSI_BUFFER_SIZES,MEMF_24BITDMA|MEMF_PUBLIC);
	MySCSICommand->scsi_Command		=AllocMem(SCSI_BUFFER_SIZES,MEMF_24BITDMA|MEMF_PUBLIC);
	MySCSICommand->scsi_SenseData	=AllocMem(SCSI_BUFFER_SIZES,MEMF_24BITDMA|MEMF_PUBLIC);
	if(!MySCSICommand->scsi_Data || !MySCSICommand->scsi_Command || !MySCSICommand->scsi_SenseData) { printf ("Ran out of 24 bit DMA accessible memory for data buffers.\n");goto Error; }

	MySCSICommand->scsi_Length			=SCSI_MAX_SIZE;			/* can take this much answer data. */
//	MySCSICommand->scsi_CmdLength		=SCSI_MAX_SIZE;			/* command buffer size. */
	MySCSICommand->scsi_SenseLength		=SCSI_MAX_SIZE;			/* error message buffer size. */

	if(!ParseArguments()) goto Error;
	if(!(SCSIDevice=SCSIDeviceInit(DeviceName,UnitNumber))) goto Error;
	return;
Error:
	AppDone(10);
}

void AppDone(UBYTE retCode)
{
	if(SCSIDevice)		SCSIDeviceDone();

	if(MySCSICommand)
	{
		if(MySCSICommand->scsi_Data)		FreeMem(MySCSICommand->scsi_Data,SCSI_BUFFER_SIZES);
		if(MySCSICommand->scsi_Command)		FreeMem(MySCSICommand->scsi_Command,SCSI_BUFFER_SIZES);
		if(MySCSICommand->scsi_SenseData)	FreeMem(MySCSICommand->scsi_SenseData,SCSI_BUFFER_SIZES);
		FreeMem(MySCSICommand,sizeof(struct SCSICmd));
		MySCSICommand=NULL;
	}
	if(MyIORequest)		{ DeleteIORequest(MyIORequest);MyIORequest=NULL; }
	if(MyMessagePort)	{ DeleteMsgPort(MyMessagePort);MyMessagePort=NULL; }
	if(ListToolsBase)	CloseLibrary(ListToolsBase);

	exit(retCode);
}

void AppRun(void)
{
	UBYTE *Cmd,*Ans,Ans2[SCSI_MAX_SIZE];
	UBYTE pdt;
	register UWORD i;

	if(SCSIDeviceInit(DeviceName,UnitNumber))
	{
		Cmd=(UBYTE *)MySCSICommand->scsi_Command;
		Ans=(UBYTE *)MySCSICommand->scsi_Data;
		MySCSICommand->scsi_Flags			=SCSIF_READ|SCSIF_AUTOSENSE;

		SCSICmd_6(Cmd,SCSI_INQUIRY,LUN,0,SCSI_MAX_SIZE_6,0);
		MySCSICommand->scsi_CmdLength		=6;
		if(SCSICmdSubmit())
		{
			if(MySCSICommand->scsi_Actual)
			{
//				printf ("  Received %ld inquiry bytes.\n",(ULONG)MySCSICommand->scsi_Actual);
				pdt=Ans[ 0]&0x1F;
				printf("  Vendor Identification  : ");for(i= 8;i<16;i++) putchar(Ans[i]);putchar('\n');
				printf("  Product Identification : ");for(i=16;i<32;i++) putchar(Ans[i]);putchar('\n');
				printf("  Product Revision Level : ");for(i=32;i<36;i++) putchar(Ans[i]);putchar('\n');
				printf("  Firmware Timestamp     : ");for(i=36;i<56;i++) putchar(Ans[i]);putchar('\n');
				if(pdt==SCSIDT_CD_ROM)
				{
					SCSICmd_6(Cmd,SCSI_TEST_UNIT_READY,LUN,0,0,0);
					MySCSICommand->scsi_CmdLength		=6;
					if(SCSICmdSubmit())
					{
//						printf("  Medium Available\n");
						SCSICmd_10(Cmd,SCSI_CD_READ_TOC,LUN,0,SCSI_MAX_SIZE_10,0);
						MySCSICommand->scsi_CmdLength		=10;
						if(SCSICmdSubmit())
						{
							if(MySCSICommand->scsi_Actual)
							{
								memcpy(Ans2,Ans,MySCSICommand->scsi_Actual);
								Cmd[1]|=2;
								if(SCSICmdSubmit())
								{
									if(MySCSICommand->scsi_Actual)
									{
										UBYTE t,*td1,*td2;
										ULONG nfrm,ofrm=0,min,sec,frm;
										char *strCtrl0[]={ "without preemphasis","with preemphasis" };
										char *strCtrl1[]={ "Digital Copy prohibited","Digital Copy permited" };
										char *strCtrl3[]={ "Two Channel","Four Channel" };

										printf("  Table Of Contents\n");
										printf("    TOC data length : %ld\n",(((UWORD)Ans[0])<<8|Ans[1]));
										printf("    first track nr  : %ld\n",Ans[2]);
										printf("    last track nr   : %ld\n",Ans[3]);
										printf("    track  adr  abs_addr  min:sec:frm\n");
										td1=&Ans[4];td2=&Ans2[4];
										nfrm=td1[7]+(td1[6]*100)+(td1[5]*6000);
										frm=nfrm-ofrm;
										min=(ULONG)(frm/6000);frm-=(min*6000);
										sec=(ULONG)(frm/100) ;frm-=(sec*100);
										printf("                          %3d:%3d:%3d\n",min,sec,frm);
										td1=&td1[8];ofrm=nfrm;
										for(t=Ans[2];t<=Ans[3];t++)
										{
											nfrm=td1[7]+(td1[6]*75)+(td1[5]*4500);
											frm=nfrm-ofrm;
											min=(ULONG)(frm/4500);frm-=(min*4500);
											sec=(ULONG)(frm/  75);frm-=(sec*  75);
											if(td2[1]&0x04) printf("    %5d  %3d  %8ld   %02d: %02d: %02d  data\n",td2[2],((td2[1]&0xF0)>>4),(((ULONG)td2[4])<<24|((ULONG)td2[5])<<16|((ULONG)td2[6])<<8|td2[7]),min,sec,frm);
											else			printf("    %5d  %3d  %8ld   %02d: %02d: %02d  audio %s,%s,%s\n",td2[2],((td2[1]&0xF0)>>4),(((ULONG)td2[4])<<24|((ULONG)td2[5])<<16|((ULONG)td2[6])<<8|td2[7]),min,sec,frm,strCtrl0[(td2[1]&0x01)],strCtrl1[(td2[1]&0x02)>>1],strCtrl3[(td2[1]&0x08)>>3]);
											td1=&td1[8];td2=&td2[8];ofrm=nfrm;
										}
									}			
									else printf("ERROR : No data received at all from the 'read toc' command!\n");
								}
							}			
							else printf("ERROR : No data received at all from the 'read toc' command!\n");
						}
					}
					else printf("  No Medium Available\n");
				}
				else printf("ERROR : device has to be a cd-rom\n");
			}
			else printf("ERROR : No data received at all from the 'inquiry' command!\n");
		}
		SCSIDeviceDone();
	}
}

/******************************************************************************
 * Read the command line arguments and set up various things.
 * Returns TRUE if successful.
 */

BOOL ParseArguments(void)
{
	struct RDArgs *rda;
	LONG args[ARG_COUNT]={0};
	BOOL ret=FALSE;

	if(rda=ReadArgs((STRPTR)ARG_TEMPLATE,args,NULL))
	{
		if(args[ARGID_DEVICE])
		{
			MSG1("ARG_DEVICE : found : %s",args[ARGID_DEVICE]);
			strncpy(DeviceName,(char *)(args[ARGID_DEVICE]),MAXDEVICENAME-1);
			DeviceName[MAXDEVICENAME-1]='\0';
			ret=TRUE;
		}
		else
		{
			register ULONG i;
			struct Node *node;
			char *src,*dst;

			MSG("ARG_DEVICE : not found");
			printf("Usage : "PRJ_NAME" %s\n",ARG_TEMPLATE);
			printf("Please specify a valid device name. Here is a list of possible ones :\n");
			foreach(node,&SysBase->DeviceList)
			{
				/* Copy the potential device name into our device name buffer and
				   convert it to upper case for comparison purposes.  Also truncates
				    names that are too long. */
				for(src=node->ln_Name,dst=DeviceName,i=0;*src && (i<MAXDEVICENAME-2);++src, ++dst,++i) *dst = toupper(*src);
				*dst='\0';
				if(strstr(DeviceName,"SCSI")) printf("\t%s\n",node->ln_Name);
			}
			DeviceName[0]='\0';
			ret=FALSE;
		}
		if(args[ARGID_UNIT])
		{
			MSG1("ARG_UNIT : found : %ld",args[ARGID_UNIT]);
			UnitNumber=*((LONG *)(args[ARGID_UNIT]));
			LUN=(UnitNumber/10)%10;
		}
		else
		{
			MSG("ARG_UNIT : not found");
			if(DeviceName[0])		/* find out what is on each LUN and unit */
			{
				UBYTE *Cmd,*Ans;
				UBYTE pq,pdt;
				register UWORD i;
//				char *strPeriQual[]={ "device is connected to target","device is supported, but not connected to target","reserved","target does not support devices","vendor specific","vendor specific","vendor specific","vendor specific" };
				char *strPeriType[]={ "direct access device","sequential access device","printer device","processor device","write-once device","cd-rom device","scanner device","optical memory device","medium changer device","communication device" };

				for(UnitNumber=0;UnitNumber<=7;UnitNumber++)
				{
//					MSG1("  evaluating unit %1ld",UnitNumber);
					printf("Unit %1ld\n",UnitNumber);
					if(SCSIDeviceInit(DeviceName,UnitNumber))
					{
						Cmd=(UBYTE *)MySCSICommand->scsi_Command;
						MySCSICommand->scsi_Flags			=SCSIF_READ|SCSIF_AUTOSENSE;
						SCSICmd_6(Cmd,SCSI_INQUIRY,LUN,0,SCSI_MAX_SIZE_6,0);
						MySCSICommand->scsi_CmdLength		=6;
						if(SCSICmdSubmit())
						{
							if(MySCSICommand->scsi_Actual)
							{
//								printf ("  Received %ld inquiry bytes.\n",(ULONG)MySCSICommand->scsi_Actual);
								Ans=(UBYTE *)MySCSICommand->scsi_Data;
/*								printf("  peripheral qualifier   : %ld %s\n",(pq =(Ans[ 0]&0xE0)>>5),strPeriQual[pq]);*/
								printf("  peripheral device type : %ld %s\n",(pdt=(Ans[ 0]&0x1F)   ),((pdt<0x0A) ? strPeriType[pdt] : "reserved/unknown"));
/*								printf("  removable media        : %ld\n",(Ans[ 1]&0x80)>>7);
								printf("  device type            : %ld\n",(Ans[ 1]&0x7F)   );
								printf("  ISO version            : %ld\n",(Ans[ 2]&0xC0)>>5);
								printf("  ECMA version           : %ld\n",(Ans[ 2]&0x38)>>3);
								printf("  ANSI-Approved version  : %ld\n",(Ans[ 2]&0x07)   );
								printf("  async event notificat. : %ld\n",(Ans[ 3]&0x80)>>7);
								printf("  terminate IO proc.     : %ld\n",(Ans[ 3]&0x40)>>6);
								printf("  response data format   : %ld\n",(Ans[ 3]&0x0F)   );
								printf("  addition length        : %ld\n", Ans[ 4]         );
								printf("  RelAdr                 : %ld\n",(Ans[ 7]&0x80)>>7);
								printf("  WBus32                 : %ld\n",(Ans[ 7]&0x40)>>6);
								printf("  WBus16                 : %ld\n",(Ans[ 7]&0x20)>>5);
								printf("  Sync                   : %ld\n",(Ans[ 7]&0x10)>>4);
								printf("  Linked                 : %ld\n",(Ans[ 7]&0x08)>>3);
//								printf("  RrmIOP                 : %ld\n",(Ans[ 7]&0x04)>>2);
								printf("  CmdQueue               : %ld\n",(Ans[ 7]&0x02)>>1);
								printf("  SoftReset              : %ld\n",(Ans[ 7]&0x01)   );
*/
								printf("  Vendor Identification  : ");for(i= 8;i<16;i++) putchar(Ans[i]);putchar('\n');
								printf("  Product Identification : ");for(i=16;i<32;i++) putchar(Ans[i]);putchar('\n');
								printf("  Product Revision Level : ");for(i=32;i<36;i++) putchar(Ans[i]);putchar('\n');
								printf("  Firmware Timestamp     : ");for(i=36;i<56;i++) putchar(Ans[i]);putchar('\n');
								SCSICmd_6(Cmd,SCSI_TEST_UNIT_READY,LUN,0,0,0);
								MySCSICommand->scsi_CmdLength		=6;
								if(SCSICmdSubmit())
								{
									printf("  Medium Available\n");
								}
							}
							else printf("ERROR : No data received at all from the 'inquiry' command!\n");
						}
						SCSIDeviceDone();
					}
//					else UnitNumber=7;
				}
			}
			else UnitNumber=LUN=0;
			ret=FALSE;
		}
		FreeArgs(rda);
	}
	else printf("Usage : "PRJ_NAME" %s\n",ARG_TEMPLATE);
  	return(ret);
}

/******************************************************************************
 * Opens the scsi.device driver and sets up the related data structures.
 * Returns TRUE if successful.
 */

BOOL SCSIDeviceInit(STRPTR devname,ULONG unit)
{
	int ErrorCode;

	if(  ErrorCode		=OpenDevice(devname,unit,(struct IORequest *)MyIORequest,0L))
	{
	    switch(ErrorCode)
	    {
			case HFERR_NoBoard	: printf("OpenDevice for %s unit %d failed : the is no board to serve unit\n",DeviceName,(int)UnitNumber);
				break;
			case HFERR_SelfUnit	: printf("OpenDevice for %s unit %d failed : unit is host-adapter\n",DeviceName,(int)UnitNumber);
				break;
//			case GVP_NOUNIT		: printf("is unused.\n");
//				break;
			default				: printf("OpenDevice for %s unit %d failed, code %d.\n",DeviceName,(int)UnitNumber,ErrorCode);
				break;
		}
		DeviceOpen=FALSE;
		goto Error;
	}
	else DeviceOpen=TRUE;
	return(TRUE);
Error:
	SCSIDeviceDone();
	return(FALSE);
}

void SCSIDeviceDone(void)
{
	if(DeviceOpen)	{ CloseDevice((struct IORequest *)MyIORequest);DeviceOpen=FALSE; }
}

/******************************************************************************
 * Submits the SCSI command in MyIORequest/MySCSICommand to the scsi.device
 * and prints out any resulting errors.  Returns TRUE if no errors.
 */

BOOL SCSICmdSubmit()
{
	int ErrorCode;
//	register UWORD i;
	UBYTE stat;
	static UBYTE *strStatus  []={ "Good","Check Condition","Condition Met","Busy","Intermediate","Intermediate Condition Met","Reservation Conflict","Command Terminated","Queue Full" };
	static UBYTE *strSenseKey[]={ "No Sense","Recovered Error","Not Ready","Medium Error","Hardware Error","Illegal Request","Unit Attention","-","-","-","-","Aborted Command","-","-","-","-" };

	MySCSICommand->scsi_Status		=0;	/* In case a *.device IO error happens. */
	MySCSICommand->scsi_SenseActual	=0;	/* Some scsi drivers don't set this. */

//	MSG1("before DoIO : %ld",MyIORequest->io_Message.mn_Node.ln_Type);
//	MyIORequest->io_Message.mn_Node.ln_Type=NT_UNKNOWN;
	if(!(ErrorCode=__DoIO((struct IORequest *)MyIORequest)))
	{
//		MSG1("after  DoIO : %ld",MyIORequest->io_Message.mn_Node.ln_Type);
		if((stat=SCSIStatus(MySCSICommand))!=SCSIStat_Good)
		{
			ERR2("ERROR : SCSI command failed, SCSI error code 0x%08lx : %s",(ULONG)stat,strStatus[stat]);
			if(MySCSICommand->scsi_SenseActual>0)
			{
				MSG1("  error code        : 0x%08lx",(int)(MySCSICommand->scsi_SenseData[0]&0x7F));
				MSG2("  sense key         : 0x%04lx : %s",(int)(MySCSICommand->scsi_SenseData[2]&0x0F),strSenseKey[(MySCSICommand->scsi_SenseData[2]&0x0F)]);
//				printf("  information bytes : ");for(i= 3;i< 7;i++) printf("0x%02x ",(int)MySCSICommand->scsi_SenseData[i]);putchar('\n');
				MSG1("  add. sense length : %ld",(int)MySCSICommand->scsi_SenseData[7]);
//				printf("  cmd. specific     : ");for(i= 8;i<11;i++) printf("0x%02x ",(int)MySCSICommand->scsi_SenseData[i]);putchar('\n');
				MSG2("  ASC,ASCQ          : 0x%08lx, 0x%08lx",(int)MySCSICommand->scsi_SenseData[12],(int)MySCSICommand->scsi_SenseData[13]);
			}
			return(FALSE);
		}
	}
	else
	{
		switch(MyIORequest->io_Error)
		{
			case HFERR_SelTimeout:	ERR("ERROR : DoIO returned HFERR_SelTimeout.\n");break;
			case HFERR_BadStatus :	ERR("ERROR : DoIO returned HFERR_BadStatus.\n");break;
			default:				ERR1("ERROR : DoIO returned code %d.\n",ErrorCode);break;
		}
		return(FALSE);	
	}
	return(TRUE);
}

/******************************************************************************
 * The usual main program entry point.
 */

void main(void)
{
	AppInit();
	AppRun();
	AppDone(0);
}
