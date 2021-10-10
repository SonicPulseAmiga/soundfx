/****h* cdda/scsidefs.h [1.00] *
*
*  NAME
*    scsidefs.h
*  COPYRIGHT
*    $VER: scsidefs.h 1.00 (09.07.99) © by Stefan Kost 1999-1999
*  FUNCTION
*
*  AUTHOR
*    Stefan Kost			sk		kost@imn.htwk-leipzig.de
*  CREATION DATE
*    06.Jun.1999
*  MODIFICATION HISTORY
*    09.Jul.1999	V 1.00	most recent version
*    06.Jun.1999	V 1.00	initial version
*  NOTES
*    based on Peter Urbanec's scsi/commands.h
*******
*/

#ifndef SCSIDEFS_H
#define SCSIDEFS_H

//-- includes -----------------------------------------------------------------

#include <devices/scsidisk.h>

//-- prototypes ---------------------------------------------------------------


//-- info ---------------------------------------------------------------------

//-- defines ------------------------------------------------------------------

//-- device types

#define SCSIDT_DIRECT_ACCESS			00	//	DA  - Direct Access Device (SBC)
#define SCSCDT_SEQUENTIAL_ACCESS		01	//	SA  - Sequential Access Device (SSC)
#define	SCSIDT_PRINTER					02	//	PRT - Printer Device (SSC)
#define SCSIDT_PROCESSOR				03	//	CPU - Processor Device (SPC-2)
#define SCSIDT_WRITE_ONCE				04	//	WO  - Write Once Read Multiple Device (SBC)
#define SCSIDT_CD_ROM					05	//	CD  - C/DVD Device (MMC-2)
#define SCSIDT_SCANNER					06	//	SC  - Scanner Device (SCSI-2)
#define SCSIDT_OPTICAL_MEMORY			07	//	OM  - Optical Memory Device (SBC)
#define SCSIDT_MEDIA_CHANGER			08	//	MC  - Media Changer Device (SMC)
#define SCSIDT_COMMUNICATION			09	//	COM - Communication Device (SCSI-2)
											//	    - Storage Array Device (SCC-2)
											//	    - Enclosure Services Device (SES)
											//	    - Simplified Direct-Access Device (RBC)
											//	    - Optical Card Reader/Writer Device (OCRW)
#define	SCSIDT_UNKNOWN					1F	//		- unknown

//-- unit conversion

#define SCSIUC_FPS	75						// 75 frames per second
#define SCSIUC_SPM	60						// 60 seconds per minutes
#define SCSIUC_FPM	(SCSIUC_FPS*SCSIUC_SPM)	// 60*75 frames per minute

//-- block constants

#define SCSIBC_SPB2	588						// 588 samples per stereo block
#define SCSIBC_SPB4	294						// 294 samples per quadro block

//-- commands
														// binary		grp	cmd
//-- general ------------------------------------------------------------------
#define SCSI_TEST_UNIT_READY					0x00	// 000 00000	0	 0
#define SCSI_REQUEST_SENSE						0x03	// 000 00011	0	 3
#define SCSI_INQUIRY							0x12	// 000 10010	0	12
#define SCSI_MODE_SELECT_6						0x15	// 000 10101	0	15
#define SCSI_COPY								0x18	// 000 11000	0	18
#define SCSI_MODE_SENSE_6						0x1A	// 000 11010	0	1A
#define SCSI_RECEIVE_DIAGNOSTIC_RESULTS			0x1C	// 000 11100	0	1C
#define SCSI_SEND_DIAGNOSTIC					0x1D	// 000 11101	0	1D
//-----------------------------------------------------------------------------
#define SCSI_COMPARE							0x39	// 001 11001	1	19
#define SCSI_COPY_AND_VERIFY					0x3A	// 001 11010	1	1A
#define SCSI_WRITE_BUFFER						0x3B	// 001 11011	1	1B
#define SCSI_READ_BUFFER						0x3C	// 001 11100	1	1C
//-----------------------------------------------------------------------------
#define SCSI_CHANGE_DEFINITION					0x40	// 010 00000	2	 0
#define SCSI_LOG_SELECT							0x4C	// 010 01100	2	 C
#define SCSI_LOG_SENSE							0x4D	// 010 01101	2	 D
#define SCSI_MODE_SELECT_10						0x55	// 010 10101	2	15
#define SCSI_MODE_SENSE_10						0x5A	// 010 11010	2	1A
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- direct access devices ----------------------------------------------------
#define SCSI_DA_TEST_UNIT_READY					0x00	// 000 00000	0	 0
#define SCSI_DA_REZERO_UNIT						0x01	// 000 00001	0	 1
#define SCSI_DA_REQUEST_SENSE					0x03	// 000 00011	0	 3
#define SCSI_DA_FORMAT_UNIT						0x04	// 000 00100	0	 4
#define SCSI_DA_REASSIGN_BLOCKS					0x07	// 000 00111	0	 7
#define SCSI_DA_READ_6							0x08	// 000 01000	0	 8
#define SCSI_DA_SEEK_6							0x0B	// 000 01011	0	 B
#define SCSI_DA_INQUIRY							0x12	// 000 10010	0	12
#define SCSI_DA_MODE_SELECT_6					0x15	// 000 10101	0	15
#define SCSI_DA_RESERVE							0x16	// 000 10110	0	16
#define SCSI_DA_RELEASE							0x17	// 000 10111	0	17
#define SCSI_DA_COPY							0x18	// 000 11000	0	18
#define SCSI_DA_MODE_SENSE_6					0x1A	// 000 11010	0	1A
#define SCSI_DA_START_STOP_UNIT					0x1B	// 000 11011	0	1B
#define SCSI_DA_RECEIVE_DIAGNOSTIC_RESULTS		0x1C	// 000 11100	0	1C
#define SCSI_DA_SEND_DIAGNOSTIC					0x1D	// 000 11101	0	1D
#define SCSI_DA_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1E	// 000 11110	0	1D
//-----------------------------------------------------------------------------
#define SCSI_DA_READ_CAPACITY					0x25	// 001 00101	1	 7
#define SCSI_DA_READ_10							0x28	// 001 01000	1	 8
#define SCSI_DA_SEEK_10							0x2B	// 001 01011	1	 B
#define SCSI_DA_VERIFY							0x2F	// 001 01111	1	 F
#define SCSI_DA_SEARCH_DATA_HIGH				0x30	// 001 10000	1	10
#define SCSI_DA_SEARCH_DATA_EQUAL				0x31	// 001 10001	1	11
#define SCSI_DA_SEARCH_DATA_LOW					0x32	// 001 10010	1	12
#define SCSI_DA_SET_LIMITS						0x33	// 001 10011	1	13
#define SCSI_DA_PRE_FETCH						0x34	// 001 10100	1	14
#define SCSI_DA_SYNCHRONIZE_CACHE				0x35	// 001 10101	1	15
#define SCSI_DA_LOCK_UNLOCK_CACHE				0x36	// 001 10110	1	16
#define SCSI_DA_READ_DEFECT_DATA				0x37	// 001 11001	1	19
#define SCSI_DA_COMPARE							0x39	// 001 11010	1	1A
#define SCSI_DA_COPY_AND_VERIFY					0x3A	// 001 11011	1	1B
#define SCSI_DA_READ_BUFFER						0x3C	// 001 11100	1	1C
#define SCSI_DA_READ_LONG						0x3E	// 001 11110	1	1E
//-----------------------------------------------------------------------------
#define SCSI_DA_CHANGE_DEFINITION				0x40	// 010 00000	2	 0
#define SCSI_DA_LOG_SELECT						0x4C	// 010 01100	2	 C
#define SCSI_DA_LOG_SENSE						0x4D	// 010 01101	2	 D
#define SCSI_DA_MODE_SELECT_10					0x55	// 010 10101	2	15
#define SCSI_DA_MODE_SENSE_10					0x5A	// 010 11010	2	1A
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- sequential access devices ------------------------------------------------
#define SCSI_SA_CHANGE_DEFINITION				0x40
#define SCSI_SA_COMPARE							0x39
#define SCSI_SA_COPY							0x18
#define SCSI_SA_COPY_AND_VERIFY					0x3a
#define SCSI_SA_ERASE							0x19
#define SCSI_SA_INQUIRY							0x12
#define SCSI_SA_LOAD_UNLOAD						0x1b
#define SCSI_SA_LOCATE							0x2b
#define SCSI_SA_LOG_SELECT						0x4c
#define SCSI_SA_LOG_SENSE						0x4d
#define SCSI_SA_MODE_SELECT_6					0x15
#define SCSI_SA_MODE_SELECT_10					0x55
#define SCSI_SA_MODE_SENSE_6					0x1a
#define SCSI_SA_MODE_SENSE_10					0x5a
#define SCSI_SA_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1e
#define SCSI_SA_READ							0x08
#define SCSI_SA_READ_BLOCK_LIMITS				0x05
#define SCSI_SA_READ_BUFFER						0x3c
#define SCSI_SA_READ_POSITION					0x34
#define SCSI_SA_READ_REVERSE					0x0f
#define SCSI_SA_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_SA_RECOVER_BUFFERED_DATA			0x14
#define SCSI_SA_RELEASE_UNIT					0x17
#define SCSI_SA_REQUEST_SENSE					0x03
#define SCSI_SA_RESERVE_UNIT					0x16
#define SCSI_SA_REWIND							0x01
#define SCSI_SA_SEND_DIAGNOSTIC					0x1d
#define SCSI_SA_SPACE							0x11
#define SCSI_SA_TEST_UNIT_READY					0x00
#define SCSI_SA_VERIFY							0x13
#define SCSI_SA_WRITE							0x0a
#define SCSI_SA_WRITE_BUFFER					0x3b
#define SCSI_SA_WRITE_FILEMARKS					0x10
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- printer devices ----------------------------------------------------------
#define SCSI_PRT_CHANGE_DEFINITION				0x40
#define SCSI_PRT_COMPARE						0x39
#define SCSI_PRT_COPY							0x18
#define SCSI_PRT_COPY_AND_VERIFY				0x3a
#define SCSI_PRT_FORMAT							0x04
#define SCSI_PRT_INQUIRY						0x12
#define SCSI_PRT_LOG_SELECT						0x4c
#define SCSI_PRT_LOG_SENSE						0x4d
#define SCSI_PRT_MODE_SELECT_6					0x15
#define SCSI_PRT_MODE_SELECT_10					0x55
#define SCSI_PRT_MODE_SENSE_6					0x1a
#define SCSI_PRT_MODE_SENSE_10					0x5a
#define SCSI_PRT_PRINT							0x0a
#define SCSI_PRT_READ_BUFFER					0x3c
#define SCSI_PRT_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_PRT_RECOVER_BUFFERED_DATA			0x14
#define SCSI_PRT_RELEASE_UNIT					0x17
#define SCSI_PRT_REQUEST_SENSE					0x03
#define SCSI_PRT_RESERVE_UNIT					0x16
#define SCSI_PRT_SEND_DIAGNOSTIC				0x1d
#define SCSI_PRT_SLEW_AND_PRINT					0x0b
#define SCSI_PRT_STOP_PRINT						0x1b
#define SCSI_PRT_SYNCHRONIZE_BUFFER				0x10
#define SCSI_PRT_TEST_UNIT_READY				0x00
#define SCSI_PRT_WRITE_BUFFER					0x3b
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- processor devices --------------------------------------------------------
#define SCSI_CPU_TEST_UNIT_READY				0x00
#define SCSI_CPU_REQUEST_SENSE					0x03
#define SCSI_CPU_RECEIVE						0x08
#define SCSI_CPU_SEND							0x0a
#define SCSI_CPU_INQUIRY						0x12
#define SCSI_CPU_COPY							0x18
#define SCSI_CPU_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_CPU_SEND_DIAGNOSTIC				0x1d
#define SCSI_CPU_COMPARE						0x39
#define SCSI_CPU_COPY_AND_VERIFY				0x3a
#define SCSI_CPU_WRITE_BUFFER					0x3b
#define SCSI_CPU_READ_BUFFER					0x3c
#define SCSI_CPU_CHANGE_DEFINITION				0x40
#define SCSI_CPU_LOG_SELECT						0x4c
#define SCSI_CPU_LOG_SENSE						0x4d
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- write once devices -------------------------------------------------------
#define SCSI_WO_CHANGE_DEFINITION				0x40
#define SCSI_WO_COMPARE							0x39
#define SCSI_WO_COPY							0x18
#define SCSI_WO_COPY_AND_VERIFY					0x3a
#define SCSI_WO_INQUIRY							0x12
#define SCSI_WO_LOCK_UNLOCK_CACHE				0x36
#define SCSI_WO_LOG_SELECT						0x4c
#define SCSI_WO_LOG_SENSE						0x4d
#define SCSI_WO_MEDIUM_SCAN						0x38
#define SCSI_WO_MODE_SELECT_6					0x15
#define SCSI_WO_MODE_SELECT_10					0x55
#define SCSI_WO_MODE_SENSE_6					0x1a
#define SCSI_WO_MODE_SENSE_10					0x5a
#define SCSI_WO_PRE_FETCH						0x34
#define SCSI_WO_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1e
#define SCSI_WO_READ_6							0x08
#define SCSI_WO_READ_10							0x28
#define SCSI_WO_READ_12							0xa8
#define SCSI_WO_READ_BUFFER						0x3c
#define SCSI_WO_READ_CAPACITY					0x25
#define SCSI_WO_READ_LONG						0x3e
#define SCSI_WO_REASSIGN_BLOCKS					0x07
#define SCSI_WO_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_WO_RELEASE							0x17
#define SCSI_WO_REQUEST_SENSE					0x03
#define SCSI_WO_RESERVE							0x16
#define SCSI_WO_REZERO_UNIT						0x01
#define SCSI_WO_SEARCH_DATA_EQUAL_10			0x31
#define SCSI_WO_SEARCH_DATA_EQUAL_12			0xb1
#define SCSI_WO_SEARCH_DATA_HIGH_10				0x30
#define SCSI_WO_SEARCH_DATA_HIGH_12				0xb0
#define SCSI_WO_SEARCH_DATA_LOW_10				0x32
#define SCSI_WO_SEARCH_DATA_LOW_12				0xb2
#define SCSI_WO_SEEK_6							0x0b
#define SCSI_WO_SEEK_10							0x2b
#define SCSI_WO_SEND_DIAGNOSTIC					0x1d
#define SCSI_WO_SET_LIMITS_10					0x33
#define SCSI_WO_SET_LIMITS_12					0xb3
#define SCSI_WO_START_STOP_UNIT					0x1b
#define SCSI_WO_SYNCHRONIZE_CACHE				0x35
#define SCSI_WO_TEST_UNIT_READY					0x00
#define SCSI_WO_VERIFY_10						0x2f
#define SCSI_WO_VERIFY_12						0xaf
#define SCSI_WO_WRITE_6							0x0a
#define SCSI_WO_WRITE_10						0x2a
#define SCSI_WO_WRITE_12						0xaa
#define SCSI_WO_WRITE_AND_VERIFY_10				0x2e
#define SCSI_WO_WRITE_AND_VERIFY_12				0xae
#define SCSI_WO_WRITE_BUFFER					0x3b
#define SCSI_WO_WRITE_LONG						0x3f
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- cd-rom devices -----------------------------------------------------------
#define SCSI_CD_TEST_UNIT_READY					0x00	// 000 00000	0	 0
#define SCSI_CD_REZERO_UNIT						0x01	// 000 00001	0	 1
#define SCSI_CD_REQUEST_SENSE					0x03	// 000 00011	0	 3
#define SCSI_CD_REASSIGN_BLOCKS					0x07	// 000 00111	0	 7
#define SCSI_CD_READ_6							0x08	// 000 01000	0	 8
#define SCSI_CD_WRITE_6							0x0A	// 000 01010	0	 A
#define SCSI_CD_SEEK_6							0x0B	// 000 01011	0	 B
#define SCSI_CD_SYNCHRONIZE_BUFFER				0x10	// 000 10000	0	10
#define SCSI_CD_INQUIRY							0x12	// 000 10010	0	12
#define SCSI_CD_MODE_SELECT_6					0x15	// 000 10101	0	15
#define SCSI_CD_RESERVE							0x16	// 000 10110	0	16
#define SCSI_CD_RELEASE							0x17	// 000 10111	0	17
#define SCSI_CD_COPY							0x18	// 000 11000	0	18
#define SCSI_CD_MODE_SENSE_6					0x1A	// 000 11010	0	1A
#define SCSI_CD_START_STOP_UNIT					0x1B	// 000 11011	0	1B
#define SCSI_CD_RECEIVE_DIAGNOSTIC_RESULT		0x1C	// 000 11100	0	1C
#define SCSI_CD_SEND_DIAGNOSTIC					0x1D	// 000 11101	0	1D
#define SCSI_CD_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1E	// 000 11110	0	1D
//-----------------------------------------------------------------------------
#define SCSI_CD_READ_CD_ROM_CAPACITY			0x25	// 001 00101	1	 7
#define SCSI_CD_READ_10							0x28	// 001 01000	1	 8
#define SCSI_CD_WRITE_10						0x2A	// 001 01010	1	 A
#define SCSI_CD_SEEK_10							0x2B	// 001 01011	1	 B
#define SCSI_CD_READ_UPDATED_BLOCK				0x2D	// 001 01101	1	 D
#define SCSI_CD_WRITE_AND_VERIFY_10				0x2E	// 001 01110	1	 E
#define SCSI_CD_VERIFY_10						0x2F	// 001 01111	1	 F
#define SCSI_CD_SEARCH_DATA_HIGH_10				0x30	// 001 10000	1	10
#define SCSI_CD_SEARCH_DATA_EQUAL_10			0x31	// 001 10001	1	11
#define SCSI_CD_SEARCH_DATA_LOW_10				0x32	// 001 10010	1	12
#define SCSI_CD_SET_LIMITS_10					0x33	// 001 10011	1	13
#define SCSI_CD_PRE_FETCH						0x34	// 001 10100	1	14
#define SCSI_CD_SYNCHRONIZE_CACHE				0x35	// 001 10101	1	15
#define SCSI_CD_LOCK_UNLOCK_CACHE				0x36	// 001 10110	1	16
#define SCSI_CD_MEDIUM_SCAN						0x38	// 001 11000	1	18
#define SCSI_CD_COMPARE							0x39	// 001 11001	1	19
#define SCSI_CD_COPY_AND_VERIFY					0x3A	// 001 11010	1	1A
#define SCSI_CD_WRITE_BUFFER					0x3B	// 001 11011	1	1B
#define SCSI_CD_READ_BUFFER						0x3C	// 001 11100	1	1C
#define SCSI_CD_UPDATE_BLOCK					0x3D	// 001 11101	1	1D
#define SCSI_CD_READ_LONG						0x3E	// 001 11110	1	1E
#define SCSI_CD_WRITE_LONG						0x3F	// 001 11111	1	1F
//-----------------------------------------------------------------------------
#define SCSI_CD_CHANGE_DEFINITION				0x40	// 010 00000	2	 0
#define SCSI_CD_READ_SUB_CHANNEL				0x42	// 010 00010	2	 2
#define SCSI_CD_READ_TOC						0x43	// 010 00011	2	 3
#define SCSI_CD_READ_HEADER						0x44	// 010 00100	2	 4
#define SCSI_CD_PLAY_AUDIO_10					0x45	// 010 00101	2	 5
#define SCSI_CD_PLAY_AUDIO_MSF					0x47	// 010 00111	2	 7
#define SCSI_CD_PLAY_AUDIO_TRACK_INDEX			0x48	// 010 01000	2	 8
#define SCSI_CD_PLAY_TRACK_RELATIVE_10			0x49	// 010 01001	2	 9
#define SCSI_CD_PAUSE_RESUME					0x4B	// 010 01011	2	 B
#define SCSI_CD_LOG_SELECT						0x4C	// 010 01100	2	 C
#define SCSI_CD_LOG_SENSE						0x4D	// 010 01101	2	 D
#define SCSI_CD_MODE_SELECT_10					0x55	// 010 10101	2	15
#define SCSI_CD_RESERVE_10						0x56	// 010 10110	2	16
#define SCSI_CD_RELEASE_10						0x57	// 010 10111	2	17
#define SCSI_CD_MODE_SENSE_10					0x5A	// 010 11010	2	1A
#define SCSI_CD_PERSISTENT_RESERVE_IN			0x5E	// 010 11110	2	1E
#define SCSI_CD_PERSISTENT_RESERVE_OUT			0x5F	// 010 11111	2	1F
//-----------------------------------------------------------------------------
#define SCSI_CD_REPORT_LUNS						0xA0	// 101 00000	5	 0
#define SCSI_CD_BLANK							0xA1	// 101 00001	5	 1
#define SCSI_CD_SEND_EVENT						0xA2	// 101 00010	5	 2
#define SCSI_CD_SEND_KEY						0xA3	// 101 00011	5	 3
#define SCSI_CD_REPORT_KEY						0xA4	// 101 00100	5	 4
#define SCSI_CD_PLAY_AUDIO_12					0xA5	// 101 00101	5	 5
#define SCSI_CD_LOAD_UNLOAD_CD_CVD				0xA6	// 101 00110	5	 6
#define SCSI_CD_SET_READ_AHEAD					0xA7	// 101 00111	5	 7
#define SCSI_CD_READ_12							0xA8	// 101 01000	5	 8
#define SCSI_CD_PLAY_TRACK_RELATIVE_12			0xA9	// 101 01001	5	 9
#define SCSI_CD_GET_PERFORMANCE					0xAC	// 101 01100	5	 C
#define SCSI_CD_VERIFY_12						0xAF	// 101 01111	5	 F
#define SCSI_CD_SEARCH_DATA_HIGH_12				0xB0	// 101 10000	5	10
#define SCSI_CD_SEARCH_DATA_EQUAL_12			0xB1	// 101 10001	5	11
#define SCSI_CD_SEARCH_DATA_LOW_12				0xB2	// 101 10010	5	12
#define SCSI_CD_SET_LIMITS_12					0xB3	// 101 10011	5	13
#define SCSI_CD_SET_STREAMING					0xB6	// 101 10110	5	16
#define SCSI_CD_READ_MSF						0xB9	// 101 11001	5	19
#define SCSI_CD_SCAN							0xBA	// 101 11010	5	1A
#define	SCSI_CD_SET_CDROM_SPEED					0xBB	// 101 11011	5	1B
#define SCSI_CD_PLAY_CD							0xBC	// 101 11100	5	1C
#define SCSI_CD_MECHANISM_STATUS				0xBD	// 101 11101	5	1D
#define SCSI_CD_READ_CD							0xBE	// 101 11110	5	1E
#define	SCSI_CD_SEND_DVD_STRUCTURE				0xBF	// 101 11111	5	1F
//-----------------------------------------------------------------------------

#define SCSI_CD_V1_READ_CDDA					0xD8	// 110 11000	6	18	/* Sony CDU 561 / Sony CDU 8003 = Apple CD 300 */
#define SCSI_CD_V1_READ_CDDA_MSF				0xD9	// 110 11001	6	19	/* Plextor PX-#? */

//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- scanner devices ----------------------------------------------------------
#define SCSI_SC_CHANGE_DEFINITION				0x40
#define SCSI_SC_COMPARE							0x39
#define SCSI_SC_COPY							0x18
#define SCSI_SC_COPY_AND_VERIFY					0x3a
#define SCSI_SC_GET_DATA_BUFFER_STATUS			0x34
#define SCSI_SC_GET_WINDOW						0x25
#define SCSI_SC_INQUIRY							0x12
#define SCSI_SC_LOG_SELECT						0x4c
#define SCSI_SC_LOG_SENSE						0x4d
#define SCSI_SC_MODE_SELECT_6					0x15
#define SCSI_SC_MODE_SELECT_10					0x55
#define SCSI_SC_MODE_SENSE_6					0x1a
#define SCSI_SC_MODE_SENSE_10					0x5a
#define SCSI_SC_OBJECT_POSITION					0x31
#define SCSI_SC_READ							0x28
#define SCSI_SC_READ_BUFFER						0x3c
#define SCSI_SC_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_SC_RELEASE_UNIT					0x17
#define SCSI_SC_REQUEST_SENSE					0x03
#define SCSI_SC_RESERVE_UNIT					0x16
#define SCSI_SC_SCAN							0x1b
#define SCSI_SC_SET_WINDOW						0x24
#define SCSI_SC_SEND							0x2a
#define SCSI_SC_SEND_DIAGNOSTIC					0x1d
#define SCSI_SC_TEST_UNIT_READY					0x00
#define SCSI_SC_WRITE_BUFFER					0x3b
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- optical memory devices ---------------------------------------------------
#define SCSI_OM_CHANGE_DEFINITION				0x40
#define SCSI_OM_COMPARE							0x39
#define SCSI_OM_COPY							0x18
#define SCSI_OM_COPY_AND_VERIFY					0x3a
#define SCSI_OM_ERASE_10						0x2c
#define SCSI_OM_ERASE_12						0xac
#define SCSI_OM_FORMAT_UNIT						0x04
#define SCSI_OM_INQUIRY							0x12
#define SCSI_OM_LOCK_UNLOCK_CACHE				0x36
#define SCSI_OM_LOG_SELECT						0x4c
#define SCSI_OM_LOG_SENSE						0x4d
#define SCSI_OM_MEDIUM_SCAN						0x38
#define SCSI_OM_MODE_SELECT_6					0x15
#define SCSI_OM_MODE_SELECT_10					0x55
#define SCSI_OM_MODE_SENSE_6					0x1a
#define SCSI_OM_MODE_SENSE_10					0x5a
#define SCSI_OM_PRE_FETCH						0x34
#define SCSI_OM_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1e
#define SCSI_OM_READ_6							0x08
#define SCSI_OM_READ_10							0x28
#define SCSI_OM_READ_12							0xa8
#define SCSI_OM_READ_BUFFER						0x3c
#define SCSI_OM_READ_CAPACITY					0x25
#define SCSI_OM_READ_DEFECT_DATA_10				0x37
#define SCSI_OM_READ_DEFECT_DATA_12				0xb7
#define SCSI_OM_READ_GENERATION					0x29
#define SCSI_OM_READ_LONG						0x3e
#define SCSI_OM_READ_UPDATED_BLOCK				0x2d
#define SCSI_OM_REASSIGN_BLOCKS					0x07
#define SCSI_OM_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_OM_RELEASE							0x17
#define SCSI_OM_REQUEST_SENSE					0x03
#define SCSI_OM_RESERVE							0x16
#define SCSI_OM_REZERO_UNIT						0x01
#define SCSI_OM_SEARCH_DATA_EQUAL_10			0x31
#define SCSI_OM_SEARCH_DATA_EQUAL_12			0xb1
#define SCSI_OM_SEARCH_DATA_HIGH_10				0x30
#define SCSI_OM_SEARCH_DATA_HIGH_12				0xb0
#define SCSI_OM_SEARCH_DATA_LOW_10				0x32
#define SCSI_OM_SEARCH_DATA_LOW_12				0xb2
#define SCSI_OM_SEEK_6							0x0b
#define SCSI_OM_SEEK_10							0x2b
#define SCSI_OM_SEND_DIAGNOSTIC					0x1d
#define SCSI_OM_SET_LIMITS_10					0x33
#define SCSI_OM_SET_LIMITS_12					0xb3
#define SCSI_OM_START_STOP_UNIT					0x1b
#define SCSI_OM_SYNCHRONIZE_CACHE				0x35
#define SCSI_OM_TEST_UNIT_READY					0x00
#define SCSI_OM_UPDATE_BLOCK					0x3d
#define SCSI_OM_VERIFY_10						0x2f
#define SCSI_OM_VERIFY_12						0xaf
#define SCSI_OM_WRITE_6							0x0a
#define SCSI_OM_WRITE_10						0x2a
#define SCSI_OM_WRITE_12						0xaa
#define SCSI_OM_WRITE_AND_VERIFY_10				0x2e
#define SCSI_OM_WRITE_AND_VERIFY_12				0xae
#define SCSI_OM_WRITE_BUFFER					0x3b
#define SCSI_OM_WRITE_LONG						0x3f
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- medium changer devices ---------------------------------------------------
#define SCSI_MC_CHANGE_DEFINITION				0x40
#define SCSI_MC_EXCHANGE_MEDIUM					0xa6
#define SCSI_MC_INITIALIZE_ELEMENT_STATUS		0x07
#define SCSI_MC_INQUIRY							0x12
#define SCSI_MC_LOG_SELECT						0x4c
#define SCSI_MC_LOG_SENSE						0x4d
#define SCSI_MC_MODE_SELECT_6					0x15
#define SCSI_MC_MODE_SELECT_10					0x55
#define SCSI_MC_MODE_SENSE_6					0x1a
#define SCSI_MC_MODE_SENSE_10					0x5a
#define SCSI_MC_MOVE_MEDIUM						0xa5
#define SCSI_MC_POSITION_TO_ELEMENT				0x2b
#define SCSI_MC_PREVENT_ALLOW_MEDIUM_REMOVAL	0x1e
#define SCSI_MC_READ_BUFFER						0x3c
#define SCSI_MC_READ_ELEMENT_STATUS				0xb8
#define SCSI_MC_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_MC_RELEASE							0x17
#define SCSI_MC_REQUEST_VOLUME_ELEMENT_ADDRESS	0xb5
#define SCSI_MC_REQUEST_SENSE					0x03
#define SCSI_MC_RESERVE							0x16
#define SCSI_MC_REZERO_UNIT						0x01
#define SCSI_MC_SEND_DIAGNOSTIC					0x1d
#define SCSI_MC_SEND_VOLUME_TAG					0xb6
#define SCSI_MC_TEST_UNIT_READY					0x00
#define SCSI_MC_WRITE_BUFFER					0x3b
//-----------------------------------------------------------------------------
														// binary		grp	cmd
//-- communication devices ----------------------------------------------------
#define SCSI_COM_CHANGE_DEFINITION				0x40
#define SCSI_COM_GET_MESSAGE_6					0x08
#define SCSI_COM_GET_MESSAGE_10					0x28
#define SCSI_COM_GET_MESSAGE_12					0xa8
#define SCSI_COM_INQUIRY						0x12
#define SCSI_COM_LOG_SELECT						0x4c
#define SCSI_COM_LOG_SENSE						0x4d
#define SCSI_COM_MODE_SELECT_6					0x15
#define SCSI_COM_MODE_SELECT_10					0x55
#define SCSI_COM_MODE_SENSE_6					0x1a
#define SCSI_COM_MODE_SENSE_10					0x5a
#define SCSI_COM_READ_BUFFER					0x3c
#define SCSI_COM_RECEIVE_DIAGNOSTIC_RESULTS		0x1c
#define SCSI_COM_REQUEST_SENSE					0x03
#define SCSI_COM_SEND_DIAGNOSTIC				0x1d
#define SCSI_COM_SEND_MESSAGE_6					0x0a
#define SCSI_COM_SEND_MESSAGE_10				0x2a
#define SCSI_COM_SEND_MESSAGE_12				0xaa
#define SCSI_COM_TEST_UNIT_READY				0x00
#define SCSI_COM_WRITE_BUFFER					0x3b
//-----------------------------------------------------------------------------

//-- info ---------------------------------------------------------------------
/*
commandCode	-- scsi command code
	 G G G C C C C C
	+-----+---------+
	   |       |
	   |       +---- command
	   +------------ group		   
	group
    	=0 	 : cmd is  6 bytes
		=1,2 : cmd is 10 bytes
		=3,4 : reserved
		=5   : cmd is 12 bytes
		=6,7 : vendor specific
	command
		see above

lun -- logical unit number
	for subdevices off a main one, in high 3 bits.
	usually 0, sometimes used for jukebox devices.

reserved -- as it says, don't touch it
	set it to zero if not stated otherwise
*/

//-- definitions --------------------------------------------------------------

//-- status

#define SCSIStat_Good						 0
#define SCSIStat_CheckCondition				 1
#define SCSIStat_ConditionMet				 2
#define SCSIStat_Busy						 4
#define SCSIStat_Intermediate				 8
#define	SCSIStat_IntermediateConditionMet	10
#define SCSIStat_ReservationConflict		12
#define SCSIStat_CommandTerminated			17
#define SCSIStat_QueueFull					20

#endif /* SCSIDEFS_H */

//-- eof ----------------------------------------------------------------------
