divert(-1)
define(`__TITLE',`Filter-FIRMatrix')
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
	__SECTION_HEADER(Filter-FIRMatrix)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Filters or boosts the signal via a convolution-matrix.
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Area for averaging-calculations. The wider that range, the higher the
cut-off frequency gets (you've got to look at this relatively,as the
Matrix permits many different characteristics to be set).
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Resonance (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Strength of resonance (also Peak or Q-Factor).
As a strong resonance thins out the signal, there is an amplification-factor
that runs parallel with the resonance, thus gets modulated too. A resonance
of 0.0 should have an Amp=100 %. Higher resonances should get higher
amplify values. You'll have to experiment to find the perfect values (try resonance+100 %).
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Matrix</nobr></td><td align="left">List of factors for use with the multiplications in the cross section.
Values shold not exceed 15.0.
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
A Matrix-<a href="../node06.03.00.Filter.html">filter</a>, such as this,
is a FIR-Filter where you can enter the coefficients yourself, e.g. if
you have designed them with an other program.<br>
With the matrix you can simulate various Filtercharacteristica. If you
want to get e.g. a highpass-filter, just set the first value to e.g. 5
and the next ones till cut-off to -1 (e.g. if cut-off=7, then the next
six values).
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
