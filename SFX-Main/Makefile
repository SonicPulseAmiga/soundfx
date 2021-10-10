# Makefile for SFX

.force: splint

splint:
	splint \
		-I/amiga/user/Dev/SC/os_guides/includes \
		-I/amiga/user/Dev/SC/source/SFX/SFX-Main \
		-nestcomment -weak \
		+gnuextensions \
		-D__regargs= \
		-D__stdargs= \
		-DUBYTE=unsigned\ char -DUWORD=unsigned\ short -DULONG=unsigned\ long \
		-DBYTE=signed\ char -DWORD=signed\ short -DLONG=signed\ long \
		-Du_char=unsigned\ char -Du_short=unsigned\ short -Du_long=unsigned\ long \
		*.c
