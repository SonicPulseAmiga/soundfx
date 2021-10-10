divert(-1)
define(`__TITLE',`RAW_L')
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
	__SECTION_HEADER(RAW_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Loads RAW Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,A-LAW,µ-LAW)</td></tr>
</table>
A RAW sample realy isn't a format. It's 'raw' sound data. This is an advantage
in one way as it's easy to handle. The downside is that no other information
but the sample itself is saved (no loop points,bit resolution...).
__SFX at least trys to scans the sample for sign-type, bit-resolution and
endian-type (16 bit).<br>
As a new feature since version 3.70, you can program the RAW-loader by yourself.
If you work often with e.g. data from audio-cd's, then name those files "<name>.cdda".
To program the loader, you set all parameters in the left half of the RAW-loader :
<table border="0">
<tr><td>Type</td><td>=PCM16</td></tr>
<tr><td>Endian</td><td>=no</td></tr>
<tr><td>Sign</td><td>=signed</td></tr>
<tr><td>Channel</td><td>=mono/stereo interleaved</td></tr>
<tr><td>SRate</td><td>=44100</td></tr>
<tr><td>Offs</td><td>=0</td></tr>
<tr><td>Check File Type</td><td>Off</td></tr>
</table>
Save this as "cdda.cfg". No click on Add (on the right half) to create a new
type (thefore CheckFileType has to be enabled). Enter ".cdda" into
the field which contains "extension/header". Now click on that popup-symbol
and select the "cdda.cfg". Everytime a files end on ".cdda" the settings from
"cdda.cfg" are used now.
If you want to check the file contents and not the ending, use a "#" instead
of a "." as the first char (e.g. "#ALAW").
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
<li>µ-Law Inv : µ-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)</li>
<li>A-Law : A-Law (14:8) compressed 14bit</li>
<li>A-Law Inv : A-Law (14:8) compressed 14bit, with inverted bits (ISDN-Master)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Endian</nobr></td><td align="left"></ul>
should SFX convert endians. Intel-proecessor based systems store 16 bit data inverted, this oprions fixes that.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sign</nobr></td><td align="left">load the Sample as a signed or unsigned sample.
<ul>
<li>signed : Amiga, Sgi</li>
<li>unsigned : Mac, Atari, PC</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Channel</nobr></td><td align="left"></ul>
with how many channels is the sample stored and in which way.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SRate</nobr></td><td align="left">which samplingrate should be used
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Offs</nobr></td><td align="left">how many bytes should be skiped in the begin (to skip a header of known length).
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Check File Type</nobr></td><td align="left">should SFX check the file extension and investigate the data statistically
to find out the format and adjust the loading parameter accordingly.
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
The offs parameter is <b>not</b> for seeking into the sample, although it can be used that way.
For 16bit sample you need to take care, that you only skip an even number of bytes then.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
