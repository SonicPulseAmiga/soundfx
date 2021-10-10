divert(-1)
define(`__TITLE',`SND-AU_S')
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
	__SECTION_HEADER(SND-AU_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Saves SND-AU samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW,IEEE-32,IEEE-64)</td></tr>
</table>
These samples come mainly from the SUN, NEXT or DEC computers or in common :
most UNIX-based machines are using this format. The format is pretty simple -
a small header followed by the sound data. In most cases these are µ-Law packed.
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
<li>µ-Law : µ-Law (14:8) compressed 14bit</li>
<li>A-Law : A-Law (14:8) compressed 14bit</li>
<li>IEEE-32 : floating point 32bit</li>
<li>IEEE-64 : floating point 64bit</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Hdr</nobr></td><td align="left"></ul>
Fileheader
<ul>
<li>SND : SUN's</li>
<li>DEC : DEC-workstation's</li>
<li>I_SND,I_DEC : PC with UNIX (LINUX)</li>
</ul>
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
