divert(-1)
define(`__TITLE',`Filter-FIRMutate')
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
	__SECTION_HEADER(Filter-FIRMutate)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dampens/boosts the signal. The filter coefficients are taken from src 2.
Therefore src 2 controls all the parameters like filter type (lowpass,
highpass, ...), the cut-off frequencies, the filter slope and so on. 
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">how much the operator effects the outcome
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Filter-Offset (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Modulates the point in the scr 2 sample, where the operator starts
retrieving the filter coefficients.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Filter-Stretch (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Modifies the mapping from samplevalues to coefficients.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Nr. (Length)</nobr></td><td align="left">How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">how to calculate (smooth) inbetween data
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">final amplification
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
A <a href="../node06.03.00.Filter.html">filter</a> such as this, is more an experimental thing.
There is nearly no way to know the result before. Good results are achived
by e.g. changing the filter-offset very little (e.g. linear from 0.0 to 0.1)
or using a relativ short sample for scr 2. Furthermore it sounds interessting
to blend the filter-stretch from e.g. 0.125 to 8.0.<br>
Curved-interpolation is useful, when using very short sample for src 2 or
small filter-offset changes.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
