divert(-1)
define(`__TITLE',`RAW_S')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
include(`lang_eng.m4')
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
Saves only the "raw" sample-data.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
</table>
A RAW sample realy isn't a format. It's 'raw' sound data. This is an advantage
in one way as it's easy to handle. The downside is that no other information
but the sample itself is saved (no loop points,bit resolution...).
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">type of compression
<ul>
<li>PCM8 : not compressed 8bit</li>
<li>PCM16 : not compressed 16bit</li>
<li>PCM24 : not compressed 24bit</li>
<li>PCM32 : not compressed 32bit</li>
<li>PCM16c : not compressed 16bit combined</li>
<li>µ-Law : µ-Law (14:8) compressed 14bit</li>
<li>µ-Law Inv : µ-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)</li>
<li>A-Law : A-Law (14:8) compressed 14bit</li>
<li>A-Law Inv : A-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Endian</nobr></td><td align="left"></ul>
should SFX convert endians. Intel-proecessor based systems store 16 bit data inverted, this oprions fixes that.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sign</nobr></td><td align="left">store the Sample as a signed or unsigned sample.
<ul>
<li>signed : Amiga, Sgi</li>
<li>unsigned : Mac, Atari, PC</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Channel</nobr></td><td align="left"></ul>
with how many channels should the sample get stored and in which way.
</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Notes)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
none
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
