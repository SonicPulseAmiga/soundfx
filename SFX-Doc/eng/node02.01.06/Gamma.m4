divert(-1)
define(`__TITLE',`Gamma')
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
	__SECTION_HEADER(Gamma)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Gamma-correction for sampledata
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Gamma (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Factor for non-linear amplification/dampening.<br>
A value of 1.0 has no effect. A larger value dampens the data (makes quiet
signals even quieter). A smaller value amplifys (makes quiete values louder).
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
You may need this operator in the folowing case:
You've got a sample which uses the full amplituderange, but is still too
quiet because of it's <a href="../node06.03.00.Dynamic.html">Dynamic</a>. To make it louder you must amplify it
without to change the volume of the maxima and minima - only amplify
the values in the middle. This is exact that, what this operator does.
(It is basically the same as a gamma-operator for image-processing.)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
