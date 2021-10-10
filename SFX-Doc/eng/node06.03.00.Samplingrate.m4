divert(-1)
define(`__TITLE',`Samplingrate')
define(`__NODE',__file__)
define(`__PATH',`')
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
			__SECTION_HEADER(__CHAPTER`'__TITLE)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify"> The samplingrate tells how many digital values are played back per second.
					The unit of the samplingrate is Hz (oszillations per second).
					The half of the samplingrate (nyquist-frequency), determines the highest freuenquency that is contained in a sample.
					There is a simple explanation for it: to detect a frequency, you need at least one period of the wave
					and that is at least two values.<br>
					As humans hear only up to about 20 kHz, samplingrates of much more than 40 kHz are not usually neccesary.
					Below common samplingrates are listed :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>samplingrate  </th><th>application</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 8000 Hz</td><td>soundcards (typical for SND-AU samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">11025 Hz</td><td>soundcards (typical for old samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">22050 Hz</td><td>soundcards (typical frequency with many samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">28867 Hz</td><td>max. playbackrate of the Paula-chip in normal mode</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">32000 Hz</td><td>consumer DATs and sampler</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">44100 Hz</td><td>CD-player, soundcards</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">48000 Hz</td><td>DAT-recorder/player</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">57734 Hz</td><td>max. playback of the Paula-chip in productivity mode</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">96000 Hz</td><td>profesional studio equipment</td></tr>
					</table>
					The Amiga audiohardware support a samplingrate of up to about 28 kHz under normal screen-modes
					and up to about 56kHz under screen-modes with doubled DMA-rate, e.g.  "Productivity"
					(activate such modes only, if you are sure that you monitor can handle it or if you use
					a graphic-card and there is nothing connected to the normal monitor-output).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
