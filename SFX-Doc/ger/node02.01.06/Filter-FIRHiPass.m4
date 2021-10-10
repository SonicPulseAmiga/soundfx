divert(-1)
define(`__TITLE',`Filter-FIRHiPass')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
include(`lang_ger.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<html>
	__HEADER
	<body __PAGE_STYLE>
		__NAVBAR
		<!-- Contents -->
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Filter-FIRHiPass)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Bearbeitet tiefe Frequenzen des Samples, d.h. unterdrückt oder verstärkt sie und läßt Hohe durch.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
sondern eine Verstärkung der zu bearbeitenden Frequenzen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">alle darunterliegenden Frequenzen werden bearbeitet, relative Frequenz von 0 Hz bis zur halben Samplingrate
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Nr. (Length)</nobr></td><td align="left">Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
desto besser (max. 1024, 64 ist in der Regel ausreichend)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">welche Fensterfunktion verwendet wird.
</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Hinweise)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Bitte wundern Sie sich nicht über die teilweise recht langen Rechenzeiten.
Wenn Sie z.B. mit 64 Koeffizienten arbeiten, werden 128 Multiplikationen
und 128 Additionen pro Samplewert durchgeführt. Da Sie die Filterspezifikationen
in SFX modulieren lassen können (und nicht wie in anderen Programmen fest
einstellen muessen), muß der Filter bei jedem Rechenschritt neu entworfen
werden. Dazu sind nochmals eine ganze Menge von Rechenoperationen notwendig.<br>
Bei den <a href="../node06.03.00.Filter.html">FIR-Filtern</a> hift ein
mathematischer Co-Prozessor erheblich !
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
