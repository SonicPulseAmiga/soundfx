divert(-1)
define(`__TITLE',`Equalize-3Band')
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
	__SECTION_HEADER(Equalize-3Band)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Raises or lowers high, mid and low frequencies. Works like the tone control of you hifi.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lower Cut-Off (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">frequency which divides the lower from the middle band, relative frequency ranging from 0 Hz to half of sampling-rate
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Higher Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">frequency which divides the middle from the upper band, relative frequency ranging from 0 Hz to half of sampling-rate
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lower gain (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">amplification for the lower band
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Middle gain (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">amplification for the middle band
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Higher gain (<a href="../node03.02.01.html">P5</a>)</nobr></td><td align="left">amplification for the higher band
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">final amplification
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
On your hifi you normaly can not change the filter cut-offs. If in doubt, just leave them as they are.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
