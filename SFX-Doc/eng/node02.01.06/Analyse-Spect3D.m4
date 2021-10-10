divert(-1)
define(`__TITLE',`Analyse-Spect3D')
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
	__SECTION_HEADER(Analyse-Spect3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Produces a 3-dimensional frequency-spectrum plot of a sample
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dir</nobr></td><td align="left"><ul>
<li>front : put samplestart at the front-side</li>
<li>back : put samplestart at the back-side</li>
&nbsp;</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left"></ul>
what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lines</nobr></td><td align="left">how many timesslices should SFX render.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>MaxLin.</nobr></td><td align="left">how many timeslices will fit on this screen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Gamma</nobr></td><td align="left">Nonlinear amplification. Values from 100 % towards 0 % means enhancing quiet details.
Values above 100 means hiding them. The default value of 75 % is a good choice to
make quiet signals visible too.
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
When calculations are complete a new window is opened on which the graph is drawn.
For the calculations the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
is used.<br>
If you want to store the generated graphs as images I recommend using a image grabber like SGrab,
which can be found on Aminet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
