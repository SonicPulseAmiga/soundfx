divert(-1)
define(`__TITLE',`MPEG_S')
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
	__SECTION_HEADER(MPEG_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Saves highly compressed MPEG samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes</td></tr>
</table>
Due to the high compression ratio, this will take a while. In fact it is
highly recomended to have an 68060 for this.<br>
This module uses external encoders (separate programs).
Therfore I've tried to make it highly configurable.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Encoder</nobr></td><td align="left">Choose the executable of the encoder you want to use. It has been testet with the
supplied 8Hz, as well as with  Pegase, Lame and Ncoder.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Parameter</nobr></td><td align="left">This is the parameter template which is passed on the command line to the encoder
(the encoder will be run as a background process and be feed with data from __SFX).
These placeholders are currently suported :
<ul>
<li>%b : the bitrate</li>
<li>%c : the parameter string for mono/stereo files (see below)</li>
<li>%i : the input filename</li>
<li>%o : the output filename</li>
<li>%r0 : the samplingrate in Hz</li>
<li>%r1 : the samplingrate in kHz (at the moment just 32, 44.1, 48)</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>MonoStr</nobr></td><td align="left"></ul>
The parameter string for mono-files which is used above with "%c".
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>StereoStr</nobr></td><td align="left">The parameter string for stereo-files which is used above with "%c".
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave</nobr></td><td align="left">This determines in which format the sample data is passed to the encoder.
<ul>
<li>CDDA</li>
<li>RIFF-WAV</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>BitRate</nobr></td><td align="left"></ul>
Strength of compression. Says how many bits per second are allowed. The
lower the bitrate is, the lower is the quality.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Pipe</nobr></td><td align="left">If you have problems with the pipe: device, you may try an alternative one
like apipe: or awnpipe:.
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
There are presets for most common encoders supplied. Still it is neccesary
that you adapt the path of the respective encoder executable or copy
the binaries to the sfx/_saver folder under the respective name.<br>
If you want to adapt it for further encoders, I recomend studing the
supplied ones.<br>
The saver uses a pipe device to send data to the encoder engine.
Please make sure the selected pipe-device is mounted.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
