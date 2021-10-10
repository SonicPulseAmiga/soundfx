divert(-1)
define(`__TITLE',`IFF-8SVX_S')
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
	__SECTION_HEADER(IFF-8SVX_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Saves IFF-8SVX samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16,PCM-24,PCM-32,FDPCM-8:4,FDPCM-16:6,EDPCM-8:4,EDPCM-16:5)</td></tr>
</table>
This is the most wide spread sound-file format on the Amiga. It is build
like any other IFF file making it a very flexible format whilst retaining
compatibility. The IFF-8SV format is one of the few that saves loops.<br>
__SFX also supports quadrosamples, 16-bit and combined samples. I have got
the description of the combined samples from the freeware program <b>SoundBox</b>
by Richard Körber. This format saves the full 16-bit data of a sample.
Is this sample loaded into a standard program (supporting only plain IFF-8SVX
files) then it loads as a standard 8-bit sample. If a program however knows
this format it loads it as a 16-bit sample.
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
<li>FDPCM8_4 : FibonacciDelta (2:1) compressed 8bit</li>
<li>FDPCM16_6 : FibonacciDelta (8:3) compressed 16bit</li>
<li>EDPCM8_4 : ExponentialDelta (2:1) compressed 8bit</li>
<li>EDPCM16_5 : ExponentialDelta (16:5) compressed 16bit</li>
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
When __SFX saves a sample in the 16-bit format it creates a "BITS" chunk of
the following structure :<br>
<code>struct chunk_bits {</code><br>
<code>  char  id[4];  // "BITS"</code><br>
<code>  ULONG len;    // 4L</code><br>
<code>  ULONG bits;   // 8/16 bit so far supported</code><br>
<code>};</code><br>
In addition the "CHAN" chunk has been extended. With a data value of 30, it
is a quadrosample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
