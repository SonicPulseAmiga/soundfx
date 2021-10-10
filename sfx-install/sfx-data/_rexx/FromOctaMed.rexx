/****h* SoundFX/FromOctaMed.rexx [1.10] *
*
*  NAME
*    FromOctaMed.rexx
*  COPYRIGHT
*    $VER: FromOctaMed.rexx 1.10 (23.09.98) © by David O'Reilly & Stefan Kost 1998-1998
*  FUNCTION
*    imports the current sample from OctaMed Sound Studio
*  AUTHOR
*    David O'Reilly			dr
*    Stefan Kost			sk
*  CREATION DATE
*    17.Jun.1998
*  MODIFICATION HISTORY
*    23.Sep.1998	V 1.10	adapted to SoundFX (sk)
*                           better type detection (sk)
*    17.Jun.1998	V 1.00	initial version (dr)
*  NOTES
*
*******
*/

OPTIONS RESULTS
sfxport=ADDRESS()
IF (LEFT(sfxport, 8) ~= "SFX_REXX") THEN DO	/* not started by SoundFX ? */
	PARSE ARG opts
	sfxport=WORD(opts,1)
	IF ~SHOW("Ports",sfxport) THEN DO
		IF ~SHOW("Ports","SFX_REXX") THEN EXIT 10
    	sfxport='SFX_REXX'
	END
END

ADDRESS 'OCTAMED_REXX'

IN_GETTYPE
smp_type=RESULT

IF smp_type=EMPTY THEN DO
	ADDRESS VALUE sfxport
	SFX_Message '"Current Octamed instrument is empty !"'
	EXIT
END
ELSE IF smp_type=SYNTH THEN DO
	ADDRESS VALUE sfxport
	SFX_Message '"Current Octamed instrument is a synth sound !"'
	EXIT
END
ELSE IF smp_type=UNKNOWN THEN DO
	ADDRESS VALUE sfxport
	SFX_Message '"Current Octamed instrument is of unknown type !"'
	EXIT
END

IN_SAVE 't:tmp_om2sfx.aiff aiff'
IN_GETNAME
smp_name=RESULT

TRACE ALL

ADDRESS VALUE sfxport

SFX_SelLoader 'IFF-AIFF'
oldloader=RESULT
SFX_LoadSample 't:tmp_om2sfx.aiff'
buf1=RESULT
SFX_RenameBuffer buf1 smp_name

SFX_SelLoader oldloader

SFX_Activate

ADDRESS COMMAND 'delete >nil: t:tmp_om2sfx.aiff'
EXIT
