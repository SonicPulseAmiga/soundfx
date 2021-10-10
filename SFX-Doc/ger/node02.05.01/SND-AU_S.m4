divert(-1)
define(`__TITLE',`SND-AU_S')
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
	__SECTION_HEADER(SND-AU_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert SND-AU Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan‰le     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
</table>
Diese Samples stammen von SUN-, NEXT- oder DEC-Rechnern bzw. auf Rechnern
die unter UNIX arbeiten. Das Formate ist recht einfach aufgebaut - ein
einfacher Header und dann die Sounddaten. Diese sind meistens µ-Law gepackt.
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
<li>A-Law : A-Law (14:8) gepackt 14bit</li>
<li>IEEE-32 : Flieﬂkomma 32bit</li>
<li>IEEE-64 : Flieﬂkomma 64bit</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Hdr</nobr></td><td align="left"></ul>
Dateikopf
<ul>
<li>SND : SUN's</li>
<li>DEC : DEC-workstation's</li>
<li>I_SND,I_DEC : PC mit UNIX (LINUX)</li>
</ul>
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
