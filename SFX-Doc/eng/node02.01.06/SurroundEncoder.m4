divert(-1)
define(`__TITLE',`SurroundEncoder')
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
	__SECTION_HEADER(SurroundEncoder)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Encodes the audio data of a quadro sample into a stereo sample, which when
replayed via a Surround Decoder which regain all its depth.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Surround/Mode</nobr></td><td align="left">Invert is faster, but causes cacelation of signals on some room positions.
Phaseshift do not has those problems, but is slower. Windowfunction and number
of coefficients is only needed for Mode=Phaseshift.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Surround/Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Surround/Nr. (PhaseNr)</nobr></td><td align="left">How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Rearfilter</nobr></td><td align="left">Normally the sound which goes to the rear channel gets filtered. Here
you can decide if you want this to happen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Rearfilter/Window (<a href="../node03.02.03.html">W2</a>)</nobr></td><td align="left">what windowfunction to use
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Rearfilter/Nr. (RearNr)</nobr></td><td align="left">How many coefficients should be used. The more they are, the better results will be get.(max 1024, but 64 is usually enough)
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
Use e.g. the <a href="Panorama-4Ch.html">Panorama-4Ch</a> operator to generate
quadrosamples.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
