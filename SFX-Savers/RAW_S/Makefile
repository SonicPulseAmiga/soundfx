#

export PRJ_NAME		= $(shell grep "define PRJ_NAME" ./src/project.h | cut -d'"' -f2)
export PRJ_VERSION	= $(shell grep "define PRJ_VERSION" ./src/project.h | cut -f3)
export PRJ_REVISION	= $(shell grep "define PRJ_REVISION" ./src/project.h | cut -f2)

.PHONY: all install \
release rel-68020 rel-68020-881 rel-68030 rel-68030-881 rel-68040-881 rel-68060-881 debug \
docs clean add-version \
opt-rel.68020 opt-rel.68020-881 opt-rel.68030 opt-rel.68030-881 opt-rel.68040-881 opt-rel.68060-881 opt-dbg.68060-881

all:
	$(MAKE) -C src
	@cp ./src/$(PRJ_NAME).sfxmod ../../sfx/_savers/
	@-User:tools/Monitors/Scout/Scout command "removelibrary $(PRJ_NAME).sfxmod"

install:
	@cp ./src/$(PRJ_NAME).sfxmod ../../sfx/_savers/
	@-User:tools/Monitors/Scout/Scout command "removelibrary $(PRJ_NAME).sfxmod"

release: rel-68020 rel-68020-881 rel-68030 rel-68030-881 rel-68040-881 rel-68060-881 docs

rel-68020: opt-rel.68020
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod ../_VERSIONS.68020/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68020/

rel-68020-881: opt-rel.68020-881
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod  ../_VERSIONS.68020-881/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68020-881/

rel-68030: opt-rel.68030
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod ../_VERSIONS.68030/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68030/

rel-68030-881: opt-rel.68030-881
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod ../_VERSIONS.68030-881/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68030-881/

rel-68040-881: opt-rel.68040-881
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod ../_VERSIONS.68040-881/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68040-881/

rel-68060-881: opt-rel.68060-881
	$(MAKE) -C src clean all
	@cp ./src/$(PRJ_NAME).sfxmod ../_VERSIONS.68060-881/
	@cp ./src/$(PRJ_NAME).sfxmod.dbg ../_VERSIONS.dbg.68060-881/
	@cp ./src/$(PRJ_NAME).sfxmod ../../sfx/_savers/

debug: opt-dbg.68060-881
	$(MAKE) -C src clean all
	@cp src/$(PRJ_NAME).sfxmod ../../sfx/_savers/

docs:
	$(MAKE) -C src docs

clean:
	$(MAKE) -C src clean

opt-rel.68020:
	@cp ../../sfx-module/options/scoptions.rel.68020 ./src/scoptions
	$(MAKE) add-version

opt-rel.68020-881:
	@cp ../../sfx-module/options/scoptions.rel.68020-881 ./src/scoptions
	$(MAKE) add-version

opt-rel.68030:
	@cp ../../sfx-module/options/scoptions.rel.68030 ./src/scoptions
	$(MAKE) add-version

opt-rel.68030-881:
	@cp ../../sfx-module/options/scoptions.rel.68030-881 ./src/scoptions
	$(MAKE) add-version

opt-rel.68040-881:
	@cp ../../sfx-module/options/scoptions.rel.68040-881 ./src/scoptions
	$(MAKE) add-version

opt-rel.68060-881:
	@cp ../../sfx-module/options/scoptions.rel.68060-881 ./src/scoptions
	$(MAKE) add-version

opt-dbg.68060-881:
	@cp ../../sfx-module/options/scoptions.dbg.68060-881 ./src/scoptions
	$(MAKE) add-version

add-version:
	@echo >>./src/scoptions "PROGRAMNAME="$(PRJ_NAME)".sfxmod"
	@echo >>./src/scoptions "LIBRARYVERSION="$(PRJ_VERSION)
	@echo >>./src/scoptions "LIBRARYREVISION="$(PRJ_REVISION)
