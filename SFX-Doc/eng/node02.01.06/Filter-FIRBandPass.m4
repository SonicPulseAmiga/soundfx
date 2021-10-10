divert(-1)
define(`__TITLE',`Filter-FIRBandPass')
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
	__SECTION_HEADER(Filter-FIRBandPass)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Works on frequencies except a specific frequency-band, means supresses or boosts them and let the band pass through unaltered.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">how much the operator effects the outcome. Negative values produce the
opposite	effect - they boost frequencys.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Low Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">lower bound of the band, relative frequencies ranging from 0 Hz to half of sampling-rate
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>High Cut-Off (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">upper bound of the band
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Nr. (Length)</nobr></td><td align="left">How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">what windowfunction to use
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
Please don't wonder because of these long calculation-times. If you are using
e.g. 64 coefficients, SFX needs to do 128 multiplications and 128 additions
for each samplevalue. Since SFX lets you modulate filterspecifications (and
not using fixed ones like other programs do), it has to redesign the filter
each samplevalue. Therefore again a bunch of calculation steps are neccessary.<br>
For <a href="../node06.03.00.Filter.html">FIR-filters</a> a mathematical
coprocessor really helps !
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
