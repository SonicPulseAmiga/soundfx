SUBDIRS = ger eng

bin_PROGRAMS = htmlinfo prevnext
htmlinfo_SOURCES = htmlinfo.c
prevnext_SOURCES = prevnext.c

index.html:
	echo "m4 -I_m4_html index.m4 >index.html"; \
	m4 -I_m4_html index.m4 >index.html
