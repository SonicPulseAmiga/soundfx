/****h* SoundFX/ToOctaMed.rexx [1.10] *
*
*  NAME
*    ToOctaMed.rexx
*  COPYRIGHT
*    $VER: ToOctaMed.rexx 1.10 (23.09.98) © by David O'Reilly & Stefan Kost 1998-1998
*  FUNCTION
*    exports the current sample to OctaMed Sound Studio
*  AUTHOR
*    David O'Reilly			dr
*    Stefan Kost			sk
*  CREATION DATE
*    17.Jun.1998
*  MODIFICATION HISTORY
*    23.Sep.1998	V 1.10	adapted to SoundFX (sk)
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
	ADDRESS VALUE sfxport
END

SFX_GetActiveBuffer
buf1=RESULT
IF buf1>-1 THEN DO
	SFX_GetBufferName buf1
	smp_name=RESULT
	SFX_SelSaver 'IFF-AIFF'
	oldsaver=RESULT
	SFX_SaveSample 't:tmp_sfx2om.aiff'

	SFX_SelLoader oldsaver

	ADDRESS 'OCTAMED_REXX'

	IN_LOAD 't:tmp_sfx2om.aiff NOSL'
	IN_SETNAME smp_name

	ADDRESS COMMAND 'delete >nil: t:tmp_sfx2om.aiff'
END
ELSE SFX_Message '"No sample to operate on !"'
ADDRESS VALUE sfxport
SFX_Activate
EXIT
