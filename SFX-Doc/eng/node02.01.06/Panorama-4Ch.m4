divert(-1)
define(`__TITLE',`Panorama-4Ch')
define(`__NODE',__file__)
define(`__PATH',`../')
include(`global.m4')
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
	__SECTION_HEADER(Function)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Distribute a mono-signal between 4 <a href="../node06.03.00.Channel.html">channels</a>.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Left-Right Position (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Propotion for left and right. 0 % (or 0.0) means left and 100 % (or 1.0) right.</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Front-Back Position (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Propotion for front and back. 0 % (or 0.0) means front and 100 % (or 1.0) back.</td></tr>
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
The result could be transformed back into a stero-sample by using the <a href="SurroundEncoder.html">SurroundEncoder</a>.
The resulting sample enfolds its depth by playing it via a surround-decoder.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
