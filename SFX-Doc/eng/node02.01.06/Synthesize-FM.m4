divert(-1)
define(`__TITLE',`Synthesize-FM')
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
	__SECTION_HEADER(Synthesize-FM)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Waveform generation by fm-synthesis like on a Yamaha CX-7.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SLen</nobr></td><td align="left">Length of the sound
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/SRat</nobr></td><td align="left">Playbackrate of the sample. Can be entered as rate, note or choosen from the <a href="../node01.05.04.html">period-window</a>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/Volume (Scale)</nobr></td><td align="left">Volume of the waveform
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Frq (Pitch)</nobr></td><td align="left">Basispitch of the sound to be generated. This can happen directly or through
the	<a href="../node01.05.04.html">period-window</a>.
It's advisable (read:really good) to choose a "C" as a note so you can use
the result in any music program easily.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/Operator</nobr></td><td align="left">Choose for which operator (wave generator) you want to edit wave, ampitude and frequency.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wave (Oszillator)</nobr></td><td align="left">What waveform will be used for the oscillator is determined here :
<ul>
<li>Sin : Sine</li>
<li>Tri : Triangle</li>
<li>Saw : Sawtooth</li>
<li>Sqr : Square</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave/Pha. (Phase)</nobr></td><td align="left"></ul>
Phaseshift (0-360 Degrees)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Frequency</nobr></td><td align="left">This defines the operators frequency relative to the basis pitch.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Amplitude</nobr></td><td align="left">This defines the operators amplitude.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Modulation-Matrix</nobr></td><td align="left">A checked box means that the amplitude of the src-operator modulates the
frequency of the dest operator. As you can easilly see, there are lots
of variations possible.
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
As a speciality of this operator you can import presets saved by FMSynth
(fileversion 1.3) as well. I would like to say thank you to the author
Christian Stiens for the source at this place.<br>
FM-synthesis is a complex matter. Just have a look at the included presets
and modify them. If you generated some good one, just send them to me, so
I can include them in further versions.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
