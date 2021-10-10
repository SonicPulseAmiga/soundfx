divert(-1)
define(`__TITLE',`Equalize-FFT-3D')
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
	__SECTION_HEADER(Equalize-FFT-3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Morphs between 8 equalizer curves in a cube into a result-curve,
which then modifies the amplitude of the frequency components of a sample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Frequency-Curves (Eqf1..8)</nobr></td><td align="left">Your source equalizer curves. When you click on the PopUp-Symbol a file
requester appears to let you choose an equalizer preset. These can be
made with the <a href="Equalize-FFT.html">Equalize-FFT</a> operator.
You can even select multiple presets at once. This will load multiple curves.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>X-Axis (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">location of the point on the X-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Y-Axis (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">location of the point on the Y-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Z-Axis (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">location of the point on the Z-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Path</nobr></td><td align="left">This area shows the path of the curve inside the cube. During the
calculation a point will wander along the curve from one end to the other.
The distance of the point to the corners defines how much of the equalizer
shape assigned to that corner effects the result equalizer shape.<br>
With the "View" gadget, you can choose from where to look on the cube
and with "Prec." you choose how detailed the curve will be drawn.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Just how many bands should SFX use. Less Bands means less math, but you lose out on accuracy.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Steps</nobr></td><td align="left">SFX does a transformation every <steps> sample-values. Lower values mean better quallity but
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
The results of the operator are very unpredictable. This means you are invited
to experiment (e.g. try a large noise sample and one of the included presets).
The FX is generally well suited to produce e.g. Sci-Fi sounds.<br>
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
