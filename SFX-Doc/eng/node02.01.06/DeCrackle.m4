divert(-1)
define(`__TITLE',`DeCrackle')
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
	__SECTION_HEADER(DeCrackle)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dampens strong leveljumps (crackles)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dif.</nobr></td><td align="left">Leveljump-threshold. If the detected leveljump lays this much above
the average leveljumps in current area, it will dampened.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Amp.</nobr></td><td align="left">Amplitude-threshold. If the current amplitudes lays this much above
the average amplitudes in current area, it will dampened.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Adjust</nobr></td><td align="left">How strong should the crack be dampened. 100 % means fully cancelation.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Size</nobr></td><td align="left">The maximum length of an anormal signal to be considered as a crackle.
Crackles are normaly relative short. This Parameter is use to separate
crackles from percussive sounds.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Test</nobr></td><td align="left">Starts the operator without modifying the sample and shows the results of the crackle-analysis.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Stat.</nobr></td><td align="left">The amount of crackles detected (absolut and relative to the length) for each channel of the sample.
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
This operator detects cracks in samples and makes them quieter.
Such cracks are contained in samples recorded from a longplayer
or can be caused by r/w-errors on a disk.<br>
Before using this operator, it is recommended to apply the
<a href="Middle.html">Middle</a> operator, followed by the
<a href="ZeroPass.html">ZeroPass</a> operator and finally the
<a href="Amplify.html">Amplify</a> operator with the MaxVol function,
to prepare the sample.<br>
If the result obtained by this operator sounds damp and misses attacks,
then raise the Dif. and Amp. values, so that fewer signals are
interpreted as crackles. If obvious crackles are not supressed zoom into
one and look at their length. Then adjust the size parameter accordingly.
You can use the Test function to tune the results.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
