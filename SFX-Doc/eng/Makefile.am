SUFFIXES=.html .m4

#DEPS=$(shell ( find . -name "*.m4" -printf "%p ";echo ) | sed -e 's/\.\/n/n/g' | sed -e 's/\ \ \./\\\ \\\ ./g')
DEPS=$(shell ( find . -name "*.m4" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')

#DEPS0=$(shell ( find . -name "node00.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS1=$(shell ( find . -name "node01.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS2=$(shell ( find . -name "node02.*.html" -printf "%p ";find ./node02.*/ -name "*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS3=$(shell ( find . -name "node03.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS4=$(shell ( find . -name "node04.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS5=$(shell ( find . -name "node05.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')
#DEPS6=$(shell ( find . -name "node06.*.html" -printf "%p ";echo ) | sed -e 's/\ \ \./\\\ \\\ ./g')

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

#chapters:
#	@head -3 node00.__.__.html >chap00.html; \
#	for file in $(DEPS0); do \
#	cat "$$file" | perl ../bodyfilter.pl >>chap00.html; \
#	done; \
#	tail -2 node00.__.__.html >>chap00.html
	

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
	perl ../spellcheck.pl "$$file" english; \
	done
