/* $VER: GrabImages V0.1 (01.Mar.2001) © by Stefan Kost 2001 */

/* to convert the iffs to gifs use the following command :
**
** foreach file (*.iff)
** ? tapgif FROM $file TO `basename $file .iff`.gif
** ? end
** copy FROM *.gif TO ../_img/ QUIET
*/

SIGNAL ON SYNTAX
OPTIONS RESULTS

/* load an initial sample */

ADDRESS 'REXX_SFX'

SFX_LoadSample 'swap:samplesnew/909break'
buf1=RESULT

/* where to save to */

/* curdir=PRAGMA("DIRECTORY")*/
curdir='dev:sc/source/sfx/sfx-doc/'

/* grab pictures */

ADDRESS 'SGRAB'

/*
SAY "Toolbars"
SAY "  Loader"
GRABSCREEN curdir||'_imgnew/tb_loader.iff' SCREEN 'SoundFX' X '0' Y '14' W '181' H '31'
SAY "  Saver"
GRABSCREEN curdir||'_imgnew/tb_saver.iff' SCREEN 'SoundFX' X '181' Y '14' W '181' H '31'
SAY "  Operator"
GRABSCREEN curdir||'_imgnew/tb_operator.iff' SCREEN 'SoundFX' X '362' Y '14' W '176' H '31'
SAY "  Rexx"
GRABSCREEN curdir||'_imgnew/tb_rexx.iff' SCREEN 'SoundFX' X '538' Y '14' W '176' H '31'
SAY "  Player"
GRABSCREEN curdir||'_imgnew/tb_player.iff' SCREEN 'SoundFX' X '714' Y '14' W '256' H '31'
SAY "  Buffer"
GRABSCREEN curdir||'_imgnew/tb_buffer.iff' SCREEN 'SoundFX' X '0' Y '45' W '231' H '31'
SAY "  Edit"
GRABSCREEN curdir||'_imgnew/tb_edit.iff' SCREEN 'SoundFX' X '231' Y '45' W '181' H '31'
SAY "  Zoom"
GRABSCREEN curdir||'_imgnew/tb_zoom.iff' SCREEN 'SoundFX' X '412' Y '45' W '131' H '31'
SAY "  Range"
GRABSCREEN curdir||'_imgnew/tb_range.iff' SCREEN 'SoundFX' X '543' Y '45' W '353' H '31'

SAY "Windows"
SAY "  Info"
PULL goon
GRABWINDOW curdir||'_imgnew/win_info.iff' WINDOW 'Informationen'
*/
/*
SAY "  Record"
PULL goon
GRABWINDOW curdir||'_imgnew/win_record.iff' WINDOW 'Aufnahme'
*/
SAY "  SampleOptions"
PULL goon
GRABWINDOW curdir||'_imgnew/win_sampleoptions.iff' WINDOW 'Sampleoptionen'

/*
SAY "Prefs"
SAY "  GUI"
PULL goon
GRABWINDOW curdir||'_imgnew/prefs_gui.iff' WINDOW 'GUI'
SAY "  Sample"
PULL goon
GRABWINDOW curdir||'_imgnew/prefs_sample.iff' WINDOW 'Sample'
SAY "  VMem"
PULL goon
GRABWINDOW curdir||'_imgnew/prefs_vmem.iff' WINDOW 'Virtueller'
SAY "  Misc"
PULL goon
GRABWINDOW curdir||'_imgnew/prefs_misc.iff' WINDOW 'Versch'
*/

/* clean up */

ADDRESS 'REXX_SFX'

SFX_RemoveBuffer buf1

SAY "ready"
EXIT

SYNTAX:
	SAY "Error #"||RC||":" ERRORTEXT(RC) "in line "||SIGL
EXIT
