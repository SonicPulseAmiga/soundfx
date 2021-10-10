# read subdirs from Makefile.subdirs and regenerate wirh 'make subdirs'
SUBDIRS = CDDA-Direct_L \
Clipboard_L \
DataTypes_L \
FutureSound_L \
IFF-16SV_L \
IFF-8SVX_L \
IFF-AIFC_L \
IFF-AIFF_L \
IFF-MAUD_L \
MPEG_L \
Maestro_L \
RAW_L \
RIFF-WAV_L \
SDS-File_L \
SND-AU_L \
Studio16_L \
TX16W_L \
VOC_L


all:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS)) || break;\
	done

release:
	@rm _VERSIONS.*/*.sfxmod*
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) release) || break;\
	done

debug:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) debug) || break;\
	done

docs:
	@for dir in $(SUBDIRS); do\
		(cd $$dir; $(MAKE) -$(MAKEFLAGS) docs) || break;\
	done

subdirs:
	@rm -f Makefile.subdirs
	find . -type d -maxdepth 1 -regex "\.\/[^\_].*" -printf "%f \\\\\n" | sort >Makefile.subdirs
