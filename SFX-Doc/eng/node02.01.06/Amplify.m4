divert(-1)
define(`__TITLE',`Amplify')
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
	__SECTION_HEADER(Amplify)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Changes the volume of a sample
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Amplification (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">This value controls the amount of amplification. The volume can be raised and/or lowered.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>MaxVol</nobr></td><td align="left">After a clicking this button, the current source will be scanned and the maximum
amplification without clipping will be calculated.
the result will be entered into Par0 and modulation will be set to "None".
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wrap</nobr></td><td align="left">Choose how to handle clipping. The modi below are available :
<ul>
<li>NoClip : don't test for overdriven values; will produce distorted sounds when raising the volume beyond the maximum</li>
<li>Clip : overdriven values are clipped</li>
<li>Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.</li>
<li>Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.</li>
</ul>
Just give it a try. Take a long sinewave  and slowly overdrive it.
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
Percussion sounds (bassdrums,snare,drums,...) can be lifted a bit
(ca. 120 %). This'll produce the typical <a href="../node06.03.00.Overdrive.html">overdrive</a> effect,
by clipping sample data (vertically).<br>
The amount of amplification without hitting the ceiling hard, can be estimated
taking a look at the min-and maxlines in the <a href="../node01.05.01.html">samplewindow</a>.<br>
This operator can also be used for amplitude and ring-modulation, creating
further possiblities for sound synthesis.
For example, make one sine wave with normal period and another
with double. Let the one sine be the source and
the other sine be the modulation waveform with User/
Normal set. Set Par0 to 0.0 and Par1 to 1.0. 
Generate the new sample and take a close look (with zoom maybe?:) at the result.
What you have done is called ring-modulation. When choosing the modulation
range to be from -1.0 to 1.0 then you will get amplitude modulation.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
