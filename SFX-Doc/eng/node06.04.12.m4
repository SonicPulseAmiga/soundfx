divert(-1)
define(`__TITLE',`Operators : Tuning')
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
					<p align="justify">
						<b>Q:</b> What is the difference between Detune, PitchShift and
								  Resample in the sense that all 3 could change the pitch
								  of a sound?<br>
						<b>A:</b> Detune and Resample are quite similar. They both
								  just output incoming samples at a different speed, thus
								  they change the length of the sample along with the 
								  pitch.<br>
								  The difference is that Resample is doing this with a
								  constant rate (e.g. output 3 values for each two incoming
								  ones), while Detune can do this with a varying factor.
								  Additionally Resample offers a few gimmicks to
								  cure diseases which can be caused by doing this namely aliasing. 
								  Therefore an example : think of a wave containing<br>
								  +-+-+-+-<br>
								  where + means maximum positive amplitude and - means
								  maximum negative amplitude.
								  Now you down sample it by factor two (skipping every
								  second value) and you would get<br>
								  ++++<br>
								  The high frequency has canceled itself or more
								  precise even became a dc-offset. And it gets worser if you
								  down-sample by fractional factors (e.g. 1.5). Then you
								  would get something like<br>
								  +--+-<br>
								  There is a very easy way to see and hear the effect.
								  Use Synthesize-Add and generate a sine-sweep from
								  1000 Hz to e.g. 100000 Hz but choose a sampling rate
								  of e.g. 44100 Hz. The result should be something like
								  puuuuuiiiiiiieeeee ;-) But it will be something like
								  puuuuiiiiieeeeeiiieeeeuuueeeeiii.<br>
								  Use Analyse-Spect-2D to visualize it and you will
								  understand.<br><br>

								  Practically you would use Resample if you have a
								  sampled recorded e.g. at 22050 Hz and you want to use
								  it in a software which requires samples recorded at
								  44100 Hz, as Resample changes the sample so that it
								  sounds the same when played back at 44100 Hz instead
								  of 22500 Hz.<br>
								  In contrast, with Detune you can do things like
								  simulating the effect of a power-outage or alien voices.<br><br>

								  Now about Pitchshift. Its speciality is that it changes
								  the pitch without changing the length, by using a lot
								  of magic (intelligently repeating or skipping small
								  fragments of the sample).
								  There is another related effect which will appear in __SFX
								  sooner or later (I wish sooner). It is called Timestretch
								  and it makes a sample longer or shorter without changing
								  the pitch. Basically this is a PitchShift followed by
								  a resample (thus can be simulated in __SFX already).<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
