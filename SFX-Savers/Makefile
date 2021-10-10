# read subdirs from Makefile.subdirs and regenerate wirh 'make subdirs'
SUBDIRS = Clipboard_S \
FutureSound_S \
IFF-16SV_S \
IFF-8SVX_S \
IFF-AIFC_S \
IFF-AIFF_S \
IFF-MAUD_S \
MPEG_S \
RAW_S \
RIFF-WAV_S \
SDS-File_S \
SND-AU_S \
Studio16_S \
TX16W_S \
VOC_S


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
