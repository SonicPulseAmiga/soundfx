divert(-1)
define(`__TITLE',`Operators : Filters, EQ')
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
						<b>Q:</b> How can I change the band assignments in the <a href="node02.01.06/Equalize-FFT.html">Equalize-FFT</a> operator?
								  The first band goes up to 648 hz and the last to 22050 hz, that makes not much sense to me.<br>
						<b>A:</b> The current equalizer is based on the fft-algorithm. This splits the whole frequency-space into fixed ranges.
								  The overall area covered is 0 Hz to samplingfreqency/2.<br>
								  For a future __SFX version I plan to include a full-parametric equalizer (n middle bands
								  with editable gain and width plus one low and one high shelf with editable gain and cut-off).<br><br>

						<b>Q:</b> I could not manage to get TB303 like effect on rhythm. This means to sweep a sharp sound from Low to Hi
								  - it is a classic effect that is used in Trance or House songs when whole rhythm is resonating...<br>
								  Can I get this in __SFX? <br>
						<b>A:</b> I belive you can. At first I recommend to use the <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a> as this
								  is fast and powerful. A filter has three important parameters :<ol>
									<li> model : lowpass, highpass, bandpass, bandstop, ... - you would choose lowpass in your case</li>
									<li> the cut-off frequency  : this is the frequency where the amplitude has already dropped by 3db</li>
									<li> the resonance : this attenuates frequencies around the cut-off</li>
								  </ol>
								  __SFX allows to modulate most parameters and not keeps them static as most other applications do.
								  Both Cut-Off and Resonance are modulatable. Such a parameter can be controlled
								  in nearly every thinkable way. Therefore you have those blend-shapes (the second line
								  of each parameter). You basically enter a start and end value and the shape
								  alternates between them (see <a href="node02.01.02.html">__NODETITLE(`node02.01.02.html')</a>.<br>
						<b>A:</b> (Jan Krutisch) I guess a good way to start here is to use <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a> as an effect (as 
								  Stefan suggested) and let the cutoff be modulated by the signals  amplitude. Since I have not used
								  __SFX for some time now, I could not tell you exactly how this is achieved, but you have to set the
								  modulation to  "USER". Then you can choose between frequency and amplitude modulation.
								  The only thing you  have to do is to set the two values for cutoff to reasonable values
								  (experimentation rules!!!) and set the resonance to a fairly high value.
								  Voila! Instant jumpy filters attack.<br><br>

						<b>Q:</b> Is there any way to increase resolution when doing FFT analysis. I mean
								  doing zooms and such to find exact peaks?<br>
						<b>A:</b> Not really. That is the unfortunate limitation of the FFT. If you are
								  interested in lower frequencies, you can zoom using a trick. Just low-pass-filter the
								  signal and resample it (you can use the builtin aliasing-filter of the
								  <a href="node02.01.06/Resample.html">Resample</a>-operator). Then start <a href="node02.01.06/AnalyseSpect-2D.html">AnalyseSpect-2D</a>.<br>
								  What I could do, is to try to build an spectrum-analyser on the base of
								  bandpass-filters. This analyser could then (nearly) endlessly zoom-into
								  the signal.<br><br>

						<b>Q:</b> When I create a 1 second noise sample using the <a href="node02.01.06/Noise.html">Noise</a> operator and then
								  perform a spectrum analysis, the result is anything but flat. What is
								  wrong, the noise algorithm or the FFT?<br>
						<b>A:</b> It can not be perfectly flat. Depending on the quality of the
								  random-number generator the noise is more or less "white".<br><br>

						<b>Q:</b> I have recorded a sample from a bigger distance and want to increase
								  the volume. But if I do so, I get a background sound.
								  It seems that this sound is above 14kHz. My sample is speech only, so
								  I think it is no problem to cut this background sound of with __SFX.<br>
						<b>A:</b> I assume, that you have recored in 16bit with 44100/48000 Hz.
								  The easiest is to use a low-pass filter. Because you want good cancellation,
								  I suggest to use <a href="node02.01.06/Filter-FIRLowPass.html">Filter-FIRLowPass</a> (and not the <a href="node02.01.06/Filter-StateVariable.html">Filter-StateVariable</a>).
								  Start the filter and enter 13000 Hz for the cut-off, set modulation to none,
								  as you do not want to create artistic sounds. Number should be
								  something like 64.<br>
								  Use the <a href="node02.01.06/Analyse-Spect-2D.html">AnalyseSpect-2D</a> afterwards to verify that high frequencies have been canceled out.
								  You can even apply the filter several time to increase steepness and dampening.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
