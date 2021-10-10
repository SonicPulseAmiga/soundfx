divert(-1)
define(`__TITLE',`MPEG_L')
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
	__SECTION_HEADER(MPEG_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Läd MPEG Samples mit einem Decoder der zur mpega.library kompatibel ist.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja</td></tr>
</table>
Sie werden viele dieser Samples im Internet finden. Wegen der sehr guten
Packrate, ist dieses Format hervorragend geeignet um komplette Songs zu packen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Engine</nobr></td><td align="left">Erlaubt die Auswahl einer mpega.library kompatiblen Decoder-Bibliothek. Von der
mpega.library existieren Versionen (FPU und MAD) die eine höhere Qualität bieten,
aber langsamer sind. Sie können auch einen PPC basierenden Decoder verwenden
wenn sie eine solche CPU haben.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Layer I&II</nobr></td><td align="left">Diese Einstellungen beziehen sich auf die Layer I und Layer II Dateien.
Sie können Qualität des Entpackens für Mono und Stereo Dateien getrennt einstellen.
Wenn sie etwas Speicher sparen möchten, können sie weiterhin erzwingen, daß
Stereodateien nach Mono konvertieret werden sollen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Layer III</nobr></td><td align="left">Genau wie oben, nur jedoch für Layer III Dateien.
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
keine
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
