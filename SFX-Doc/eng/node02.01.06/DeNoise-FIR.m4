divert(-1)
define(`__TITLE',`DeNoise-FIR')
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
	__SECTION_HEADER(DeNoise-FIR)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Removes noise from a sample (multifrequency Noisegate)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"></table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Attack)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Notes)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
If threshold is set too high, too much of the sample will be suppressed.
The result might sound damp in this case<br>
The attack-value should be relative small. If it is too small,
the result might sound chopped.<br>
A good way to control the effect is to use the <a href="Analyse-Spect2D.html">Analyse-Spect2D</a>
operator with a low gamma-value (e.g. 0.2).
You should clearly see the noise in quiet sections.
After applying the DeNoise operator, check again with the Analyser. You should be able to see if the
noise-levels have dropped.<br>
__SFX divides the sample into several bands and denoises these.
Afterwards the signal will rebuild out of these. The diversion
is done by using <a href="../node06.03.00.Filter.html">FIR-Filter</a>.<br>
Before using this operator, it is recommended to apply the
<a href="Middle.html">Middle</a> operator, to prepare the sample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
