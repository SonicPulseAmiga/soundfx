/****h* SoundFX/FromSoundProbe.rexx [1.10] *
*
*  NAME
*    FromSoundProbe.rexx
*  COPYRIGHT
*    $VER: FromSoundProbe.rexx 1.10 (23.09.98) © by David O'Reilly & Stefan Kost 1998-1998
*  FUNCTION
*    imports the current sample from SoundProbe
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
END

ADDRESS 'SOUNDPROBE'

GETPROJECT
pid=RESULT
IF pid=0 THEN DO
	ADDRESS 'REXX_SFX' SFX_Message '"There is no current SoundProbe project !"'
	EXIT
END
GETATTR PROJECT pid STORAGEID buf
IF buf=0 THEN DO					/* test for data storage */
	ADDRESS 'REXX_SFX' SFX_Message '"SoundProbe project is empty!"'
	EXIT
END
	
GETATTR PROJECT pid NAME smp_name
SAVEAS 't:tmp_sp2sfx.aiff AIFF.spiolibrary'

ADDRESS VALUE sfxport

SFX_SelLoader 'IFF-AIFF'
oldloader=RESULT
SFX_LoadSample 't:tmp_sp2sfx.aiff'
buf1=RESULT
SFX_RenameBuffer buf1 smp_name

SFX_SelLoader oldloader

SFX_Activate

ADDRESS COMMAND 'delete >nil: t:tmp_sp2sfx.aiff'
EXIT
