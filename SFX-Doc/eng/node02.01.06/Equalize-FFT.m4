divert(-1)
define(`__TITLE',`Equalize-FFT')
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
	__SECTION_HEADER(Equalize-FFT)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Modify the amplitude of the frequency components of a sample
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Frequency-Curve</nobr></td><td align="left">Here you can draw the shape of the frequency spectrum.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Arrow-Gadgets</nobr></td><td align="left">for moving the curve
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>F-Gadget</nobr></td><td align="left">Flip, mirrors the curve
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Band</nobr></td><td align="left">number of the band you're currently working on
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Val</nobr></td><td align="left">value of current band
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Frq</nobr></td><td align="left">shows the frequency range for the current band.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Range</nobr></td><td align="left">Simple tool to do a straight line between two bands. For those who can't draw these lines
(like me) just click on the first band then range and then the second band.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Mode</nobr></td><td align="left">Gives you the choice of moving just the current band or all when using the arrow buttons..
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Steps</nobr></td><td align="left">SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
longer calculation. Steps can no be bigger than the half of the number of bands.
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
This operator uses the <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
for its calculations.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
