divert(-1)
define(`__TITLE',`Middle')
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
	__SECTION_HEADER(Middle)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Searches for the middle of sample data and centers the sample on the x-axis.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>none</nobr></td><td align="left">&nbsp;</td></tr>
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
Whenever you digitize sound, it can happen that the sample data lies a bit
off the x-axis. The middle of the sample just ain't where it should be. On
the x-axis. This means that your sample contains an overall offset in its
data and should you apply effects to it, can drift away from the middle and
at some point become <a href="../node06.03.00.Overdrive.html">overdriven</a> onesidedly.
(One half runs up (upper) or down (lower)). This operator prevents this from
ever happening again. (Though some of you will overdrive samples a lot at
some time, this ain't the cause anymore. It's your fault:)<br>
Apart from overdriving this is important for restauration (<a href="DeCrackle.html">DeCrackle</a>, <a href="NoiseGate.html">NoiseGate</a>, ...)
so that these operation can correctly analyse the signals.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
