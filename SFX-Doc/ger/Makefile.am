SUFFIXES=.html .m4

#DEPS=$(shell ( find . -name "*.m4" -printf "%p ";echo ) | sed -e 's/\.\/n/n/g' | sed -e 's/\ \ \./\\\ \\\ ./g')
DEPS=$(shell ( find . -name "*.m4" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')

HTMLDOCOPTS=--bodycolor FFFFFF --browserwidth 850 --fontsize 10 --header ...

all: index html ps pdf

index:
	@rm -f index.txt indext.txt; \
	find . -name "*.m4" -print | sed s/_/\ /g | sort >indext.txt; \
	cat indext.txt indext.txt | sed s/\ /_/g >index.txt; \
	rm -f indext.txt

html:
	@for file in $(DEPS); do \
	outfile=`echo "$$file" | sed -e 's/.m4/.html/g'`; \
	echo "m4 -I../_m4_html $$file >$$outfile"; \
	m4 -I../_m4_html "$$file" | perl ../stripspaces.pl >"$$outfile"; \
	done

ps:
#	-htmldoc --webpage    $(HTMLDOCOPTS) --size letter -f sfx-letter.ps `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
#	-htmldoc --webpage    $(HTMLDOCOPTS) --size a4     -f sfx-a4.ps     `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
	-htmldoc --continuous $(HTMLDOCOPTS) --size letter -f sfx-letter.ps `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
	-htmldoc --continuous $(HTMLDOCOPTS) --size a4     -f sfx-a4.ps     `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`

pdf:
#	-htmldoc --webpage    $(HTMLDOCOPTS) --size letter -f sfx-letter.pdf `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
#	-htmldoc --webpage    $(HTMLDOCOPTS) --size a4     -f sfx-a4.pdf     `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
	-htmldoc --continuous $(HTMLDOCOPTS) --size letter -f sfx-letter.pdf `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`
	-htmldoc --continuous $(HTMLDOCOPTS) --size a4     -f sfx-a4.pdf     `find . -name "*.html" -print | sed s/_/\ /g | sort | sed s/\ /_/g`

spellcheck:
	@for file in $(DEPS); do \
	perl ../spellcheck.pl "$$file" german; \
	done
