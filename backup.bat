.key DUMMY/K
.bra [
.ket ]
;
; $VER: backup.bat 0.1 (10.06.02)
; copies all soundfx source files + executables
; 

set SRC .

;-- create versioned directory

grep >temp[$$]1 "#define PRJ_VERSION" $SRC/sfx-main/project.h
sc:c/sed >NIL: temp[$$]1 "#define PRJ_VERSION" ""
sc:c/sed >NIL: temp[$$]1 "	" ""

grep >temp[$$]2 "#define PRJ_REVISION" $SRC/sfx-main/project.h
sc:c/sed >NIL: temp[$$]2 "#define PRJ_REVISION" ""
sc:c/sed >NIL: temp[$$]2 "	" ""

copy temp{$$}1 temp[$$]2 env: QUIET

set DST ../SFX-$temp[$$]1.$temp[$$]2

mkdir -p $DST
echo "$DST"

; now copy files

;copy FROM $SRC/SFX-Library/#?.(c|h|library#?|fd) TO $DST ALL CLONE QUIET
;copy FROM $SRC/SFX-Library/(build|SCOPTIONS).#? TO $DST ALL CLONE QUIET
sh -c "find $SRC -name \"*.c\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"*.h\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"*.fd\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"*.rexx\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"build.*\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"SCOPTIONS.*\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"upd*\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"sfxsupport.library.dbg.*\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"sfx.dbg.*\" -exec cp --parents {} $DST/ \;"
sh -c "find $SRC -name \"Makefile*\" -exec cp --parents {} $DST/ \;"

delete >nil: env:temp[$$]2 env:temp[$$]3 temp[$$]1 temp[$$]2
