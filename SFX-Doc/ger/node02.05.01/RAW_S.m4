divert(-1)
define(`__TITLE',`RAW_S')
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
	__SECTION_HEADER(RAW_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert nur die "rohen" Sampledaten.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kanäle     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
</table>
RAW-Sample haben eigentlich gar kein Format. Hier werden nur die "rohen"
Sounddaten abgespeichert. Das hat den Vorteil, daß dieses Format sehr einfach
zu handhaben ist, aber auch den Nachteil, daß keinerlei zusätzliche Daten wie
Samplingrate, Loops, Bitauflösung usw. gespeichert werden können.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">Art der Kompression
<ul>
<li>PCM8 : ungepackt 8bit</li>
<li>PCM16 : ungepackt 16bit</li>
<li>PCM24 : ungepackt 24bit</li>
<li>PCM32 : ungepackt 32bit</li>
<li>PCM16c : ungepackt 16bit kombiniert</li>
<li>µ-Law : µ-Law (14:8) gepackt 14bit</li>
<li>µ-Law Inv : µ-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)</li>
<li>A-Law : A-Law (14:8) gepackt 14bit</li>
<li>A-Law Inv : A-Law (14:8) gepackt 14bit, mit gespiegelten Bits (ISDN-Master)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Endian</nobr></td><td align="left"></ul>
ob eine Endiankonvertierung durchgeführt werden soll. Intel-Prozessor basierte
Systeme speichern 16bit Wörter umgekehrt und diese Option  korregiert das.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sign</nobr></td><td align="left">ob das Sample als vorzeichenbehaftetes oder nicht-vorzeichenbehaftetes gespeichert werden soll
<ul>
<li>signed : Amiga, Sgi</li>
<li>unsigned : Mac, Atari, PC</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Channel</nobr></td><td align="left"></ul>
wieviele Kanäle gespeichert werden sollen und wie sie aufgebaut sind.
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
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
