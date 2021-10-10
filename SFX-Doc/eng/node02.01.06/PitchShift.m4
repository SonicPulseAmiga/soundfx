divert(-1)
define(`__TITLE',`PitchShift')
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
	__SECTION_HEADER(PitchShift)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Changes the pitch of a sample without making it shorter or longer.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">how much the operator effects the outcome
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>PitchShift Factor (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">factor for change in pitch
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Window</nobr></td><td align="left">windowrange; use values in the range of 5 to 100 ms for good results
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Smooth</nobr></td><td align="left">how much percentage of the windowrange should used for crossfade;
usually between 25 % and 50 %
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">how to calculate (smooth) inbetween data
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
Before I give some more detailed tips, I will generally describe how this all works.
If you want to pitch up a sound, you can achive this by playing the sound faster
and thus compressing the wave (on the time axis). Unfortunately this makes the
sound shorter as well. To compensate this, __SFX will repeat small chunks of
sound to stretch the sample. While doing this __SFX has to take care that those
chunks fit relative seamingless together to avoid crackles. The winsize parameter
determines how far SFX searches maximaly for a good transition. The size depends
on the material to pitch-shift. I recomend smaller values (30-50 ms) for percussive
samples (this avoids that attacks are repeated audible) and longer values (100-200
ms) for synth/pad/string sounds (to avoid loops).<br>
If the <a href="../noed06.03.00.Modulation.html">modulator</a> is a sine wave and
a small pitchfactor is used (+/- 10 ct), you'll get a vibrato effect.<br>
If you'd like to manipulate synthetic waveforms that have a constant period you
should enter the period in Winsize. This'll produce clean pitchshifts.<br>
Factors shouldn't exceed 4.0 with sampled sounds as such high factors result in
bad pitchshifts (this is due to the way the pitchshifter works). Synthetic waveforms
can be pitchshifted however the far you like.<br>
If the result contains crackles try to slightly change the window-size and/or raise
the smooth value.<br>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
