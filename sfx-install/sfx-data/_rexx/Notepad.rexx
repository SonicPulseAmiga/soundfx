/*
   $VER: Notepad.rexx 1.0 for SoundFX (13.2.2002)
   © by Dan Jedlicka

   Opens a text editor on SoundFX screen so that you can write down
   your notes without having to switch to other screens.

   The default editor is EditPad (available as a system tool since
   AmigaOS 3.5). You can of course configure it for other editors
   that can be opened on named public screens (eg. CED).

 */



OPTIONS RESULTS
sfx_port="SFX_REXX"

/* Configure your editor here. */
editor_path="SYS:Tools"
editor_app="EditPad"

olddir=PRAGMA("DIRECTORY", editor_path)
oldstack=PRAGMA("STACK", 8192)

/* Run the editor. */
IF SHOW("PORT", sfx_port) THEN DO
   IF EXISTS(editor_app) THEN DO
      ADDRESS COMMAND "run >NIL: EditPad PUBSCREEN=SFX_PUBSCREEN"
      /*  For CED the line would be similar:
          ADDRESS COMMAND "run >NIL: CED PUBSCREEN=SFX_PUBSCREEN"
       */
   END
END

olddir=PRAGMA("DIRECTORY",olddir)
oldstack=PRAGMA("STACK",oldstack)
