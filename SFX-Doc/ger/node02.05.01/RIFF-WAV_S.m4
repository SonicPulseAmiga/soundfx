divert(-1)
define(`__TITLE',`RIFF-WAV_S')
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
	__SECTION_HEADER(RIFF-WAV_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert RIFF-WAV Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorw�rts)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan�le     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,�-LAW)</td></tr>
</table>
Dieses Format wurde mit Window auf dem PC eingef�hrt und ist stark an den
IFF-Standart angelehnt. Das WAV-Format ist das wichtigste Sampleformat auf
dem PC.
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
<li>�-Law : �-Law (14:8) gepackt 14bit</li>
<li>A-Law : A-Law (14:8) gepackt 14bit</li>
<li>IEEE-32 : Flie�komma 32bit</li>
<li>IEEE-64 : Flie�komma 64bit</li>
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
