#

CFLAGS	= NOLINK
LDFLAGS = LINK
CC		= sc
LD		= sc

OBJECTS = $(PRJ_NAME).o lib-soundfx.o

.PHONY: all docs clean

all: $(PRJ_NAME).sfxmod.dbg $(PRJ_NAME).sfxmod

%.o: %.c
	$(CC) $(CFLAGS) $*.c

$(PRJ_NAME).sfxmod: ${OBJECTS}
	$(LD) $(LDFLAGS) STRIPDEBUG ${OBJECTS} PROGRAMNAME=$@ 

$(PRJ_NAME).sfxmod.dbg: ${OBJECTS}
	$(LD) $(LDFLAGS) ${OBJECTS} PROGRAMNAME=$@ 

docs:
	@m4gen $(PRJ_NAME) data:source/sfx/sfx-doc/ger/node02.05.01/ ger
	@m4gen $(PRJ_NAME) data:source/sfx/sfx-doc/eng/node02.05.01/ eng

clean:
	@rm -rf *.o *.lnk *.sfxmod *.sfxmod.dbg *.tmp
