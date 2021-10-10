divert(-1)
define(`__TITLE',`VOC_S')
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
	__SECTION_HEADER(VOC_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Saves SoundBlaster-VOC samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,ADPCM-8:4,ADPCM-8:3,ADPCM-8:2,A-LAW,µ-LAW)</td></tr>
</table>
The VOC format was introduced by "Creative Labs", creators of the
Soundblaster-cards on the PC. It was created for easy playback from disks
and hard disks or CDs giving it a host of advantages. However due to
inconsequent planning it became nessecary to 'add' features which slow down
handling of this format. Most programs aren't able to read but one
(the 1.1 version) of the VOC format. __SFX can read and write all known
versions of this format.
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
<li>ADPCM8_4 : AdaptiveDelta (2:1) compressed 8bit</li>
<li>ADPCM8_3 : AdaptiveDelta (3:1) compressed 8bit</li>
<li>ADPCM8_2 : AdaptiveDelta (4:1) compressed 8bit</li>
<li>µ-Law : µ-Law (14:8) compressed 14bit</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Header</nobr></td><td align="left"></ul>
file format version :
<ul>
<li>1.20 : Use blocktype 9 for soundheader.</li>
<li>1.15 : Use blocktype 8 and 1 for soundheader.</li>
<li>1.10	: Use blocktype 1 only for soundheader</li>
</ul>
In my experience most programs can't read the new VOC formats. I therefore
added the option to save the old format. Using version 1.10 might be the
safest. Please take these limitations into account:
<ul>
<li>1.15 : only 8-bit samples</li>
<li>1.10 : only mono and 8-bit samples</li>
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
