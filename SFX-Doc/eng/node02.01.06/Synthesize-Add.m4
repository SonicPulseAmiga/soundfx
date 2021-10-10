divert(-1)
define(`__TITLE',`Synthesize-Add')
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
	__SECTION_HEADER(Synthesize-Add)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Waveform generation through additive and sound- synthesis, including
frequency-and amplitude modulation.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave (Oszillator)</nobr></td><td align="left">What waveform will be used for the oscillator is determined here :
<ul>
<li>Sin : Sine</li>
<li>Tri : Triangle</li>
<li>Saw : Sawtooth</li>
<li>Sqr : Square</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wave/Pha. (Phase)</nobr></td><td align="left"></ul>
Phaseshift (0-360 Degrees)
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Curve Editing/Range</nobr></td><td align="left">Tool to let you create a smooth linear line between two sliders. Click the
first then range and then the second.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Curve Editing/Mode</nobr></td><td align="left">Here you can choose how to move or flip the sliders with vertical arrows :
<ul>
<li>Cur : current slider</li>
<li>All : all sliders</li>
<li>Pos : all positive sliders</li>
<li>Neg : all negative sliders</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Curve Editing/Nr</nobr></td><td align="left"></ul>
Number of the high tone.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Curve Editing/Val</nobr></td><td align="left">Amplitude for the high tone
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SLen</nobr></td><td align="left">Length of the sound
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/OnePer</nobr></td><td align="left">Calculates the length of one period using the current rate and puts the result into SLen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SRat</nobr></td><td align="left">Playbackrate of the sample. Can be entered as rate, note or choosen from the <a href="../node01.05.04.html">period-window</a>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Volume (Scale)</nobr></td><td align="left">Volume of the waveform
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/MaxVol</nobr></td><td align="left">Calculates the volume for optimum dynamics.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Frq (Pitch)</nobr></td><td align="left">Basispitch of the sound to be generated. This can happen directly or through
the	<a href="../node01.05.04.html">period-window</a>.
It's advisable (read:really good) to choose a "C" as a note so you can use
the result in any music program easily.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Harmonics (SVal)</nobr></td><td align="left">This area has 64 sliders for all the obertone-parts. If the slider's in the
the middle (value=0), then this high tone won't be incorporated into the
resulting waveform.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Harmonics/horiz. arrows</nobr></td><td align="left">Horizontal moving of the lists in steps of one or five.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Harmonics/vert. arrows</nobr></td><td align="left">Vertical moving of the list or the current slider in steps on one or five.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Harmonics/F-Gadget	Flip.</nobr></td><td align="left">Vertically mirrors the list or the current slider.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Frequency (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Factors for frequency-modulation
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Amplitude (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Factors for amplitude-modulation
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
Every sound consists of one basic tone and many "high" tones. Frequencies
of these "high" or upper tones are a multiples of that of the basic tone.
With the help of this operator you can build very complex waveforms by
entering the different <a href="../node06.03.00.Harmonics.html">high tones</a>.
It might be a good idea to load the example files and take a look or listen
in to the resulting waveform. Every high tone has it's own volume setting.
The "val" will show it to you in numbers. This value should decline with
a rising number of high tones (chance for overdrive:). Positive values are
added and negative subtracted.<br>
You can produce intersting results for example by taking a basic sample made
at "C-2" and another at [C-2] + ( ([C#2]-[C-2]) / 4 ) Some examples :<table>
<tr><td>C-0</td><td>65.4063913</td><td>67.35102453</td></tr>
<tr><td>C-1</td><td>130.8127827</td><td>132.7574159</td></tr>
<tr><td>C-2</td><td>261.6255653</td><td>265.5148317</td></tr>
<tr><td>C-3</td><td>523.2511306</td><td>531.0296635</td></tr></table>
You now mix these two samples with <a href="Mix.html">Mix</a> with a 50/50 setting.
This gives you a sample that sounds a bit etheral, alive and fatter.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
