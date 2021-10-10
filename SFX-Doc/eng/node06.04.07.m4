divert(-1)
define(`__TITLE',`Operators : Amplitude, Dynamics')
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
						<b>Q:</b> My aim is to do an envelope detector in a few steps (maybe Arexx Script).
								  Does anyone know how to rectify a sample (i.e. mirror the negative parts,
								  absolute value) - I mean not just the display, but the actual sample data.
								  Then I want to LPF the sample to end up with the envelope. What cutoff
								  frequency would be OK?<br>
								  How is this done when applying the AmpEnv modulation<br>
						<b>A:</b> The first is easy. Use <a href="node02.01.06/AmplifySplit.html">AmplifySplit</a>. There you can amplify upper and lower parts
								  individually. Thus you can amplify the upper by "1.0" and the lower by "-1.0"
								  and therfore just inverting the lower part. Then apply a lowpass with a cutoff
								  at about "150 Hz".<br>
								  Another good idea is to mix the signal with a delayed copy of
								  itself (choose a delay of e.g. "25 ms" in the <a href="node02.01.06/Mix.html">Mix</a> operator).<br>
								  But the <a href="node02.01.06/AmplifySplit.html">AmplifySplit</a> and LPF combination works just perfectly. The best
								  results are achieved with the <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a> (cutoff between "50" and
								  "200 Hz" and resonance=1). With these values you see really nice and smooth
								  envelope curves which can be used to modulate other effects.<br><br>

						<b>Q:</b> The envelope curve I end up getting lies on the upper
								  part of the window (i.e it only takes positive values), but I would like
								  it to take values from -(max) to +(max).<br>
						<b>A:</b> You can use the <a href="node02.01.06/.html">Slide</a> operator to shift it down by "50%" and then <a href="node02.01.06/Amplify.html">Amplify</a> to scale it to
								  "200 %". If you use it then in __SFX for modulation, just use the modulation mode "abs".<br>
								  Even easier is to use __SFX&#39;s ability to create those envelopes on-the-fly.
								  You know that __SFX is able to do that ? Choose "blend-shape=User", then
								  activate the settings and choose the source sample (where to grab that envelope
								  from) and "modulation type=AmpEnv" (AmplitudeEnvelope).<br><br>

						<b>Q:</b> In the operator <a href="node02.01.06/Dynamic.html">Dynamic</a>, what is the threshold for deciding what is a 
								  loud or quiet value? Do we have access to setting this threshold?<br>
						<b>A:</b> In former __SFX version this effect was called "CompressorExpander". Such an effect
								  need a threshold to operate on. I renamed the operator in __SFX to <a href="node02.01.06/Dynamic.html">Dynamic</a>
								  as it works different. The results are similar though.<br>
								  You give the <a href="node02.01.06/Dynamic.html">Dynamic</a> operator a factor for the loudest value
								  (full amplitude) and a factor for the quietest (zero). In between the operator
								  interpolates linear.<br><br>

						<b>Q:</b> If I give a negative value for quiet, will it sit on zero or be flipped 
								  on negative side?<br>
						<b>A:</b> __SFX will never reject a parameter because it looks unfamiliar. That is why
								  you can produce so many different results with just one operator.<br>
								  When you enter a negative value for the quiet, then it will inverse
								  quiet sounds and the interpolation will range from that negative value
                                  to the (probably) positive value for loud.<br><br>

						<b>Q:</b> While mixing a CD last weekend I found that some tracks are much more
								  silent than others. Is __SFX able to do a "maximize" function on a track?
								  I mean to make silent tracks louder and maybe loud tracks more silent
								  and if yes, how?<br>
						<b>A:</b> It is easy to make them all loud. You can use the <a href="node02.01.06/Amplify.html">Amplify</a>-Operator for that. Just press
								  MaxVol there and it calculates the optimal amplification. When you have
								  a batch of file to maximize, then use the batch processor :<ol>
									<li>Loader : Universal</li>
									<li>Operator : Amplify, Preset : MaxVol</li>
									<li>Saver : e.g. IFF-AIFC</li>
								  </ol>Then you just hit start and select source and destination directories.<br>
								  If all tracks are amplified to full extent, it is a bit more difficult.
								  It would mean to use <a href="node02.01.06/Analyse-Data.html">Analyse-Data</a> for each track and to write down the
								  "RMS-Volume". Then you could make the louder tracks quieter (with
								  amplify) to reduce their energy (RMS-Volume) or use the dynamic operator
								  to compress the silent tracks (e.g. loud values=1.0, quiet values 1.5).<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
