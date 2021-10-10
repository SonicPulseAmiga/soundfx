divert(-1)
define(`__TITLE',`Resample')
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
	__SECTION_HEADER(Resample)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Changes the <a href="../node06.03.00.Samplingrate.html">sampling-rate</a> and sample length while preserving the original sound.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SLen old</nobr></td><td align="left">old sample length
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>SLen new</nobr></td><td align="left">new sample length. Factor and new rate are calculated and entered
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SRat old</nobr></td><td align="left">old sampling-rate
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>SRat new</nobr></td><td align="left">new sampling-rate. Factor and new length are	calculated and entered
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Factor</nobr></td><td align="left">factor of change in length and rate. A factor of 1.0 changes nothing.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Lock</nobr></td><td align="left">Determines which parameter should be locked. If you want e.g. resample several
samples with different rates all to the same rate, you would choose "SRat" then.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">how to calculate (smooth) inbetween data
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Aliasing Filter</nobr></td><td align="left">If enabled, the sound gets filtered before resampling. This is important when
lowering the sampling-rate.
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
When you sample a sound and use it in a music program it is sometimes out of tune.
This operator can correct this. For this you'll have to enter the playback rate as for
example the rate you hear the note "C" on with this sample. Now you enter "Resample"
and set the resampling rate to "C" -> 16780  and generate the new sample. The new
sample will play a "C" at the correct rate now.<br>
With "Resample" it is also possible to change the length of the sample for when you
want to modulate something with this sample and need to get to a correct length to
do so. Use interpolation with this so that the waveform won't gets too "edgy".
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
