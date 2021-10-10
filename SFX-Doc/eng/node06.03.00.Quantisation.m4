divert(-1)
define(`__TITLE',`Quantisierung')
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
						To manipulate a signal with the computer you need it in digital form.
						Therfore the signal gets meassured in short intervalls. The values are then rounded and stored.
						During this process the signal becomes quantized twice (time, amplitude). The rate which we use for
						taking probes is called <a href="node06.03.00.Samplingrate.html">samplingrate</a> and the precission
						corresponds to the <a href="node06.03.00.Bitresolution.html">bitresolution</a> of the sample.
						One can apply the following rule of the thumb to both values, the higher the better the result,
						but the higher the memory-consumtion as well.<br>
						If the quantisation of time (<a href="node06.03.00.Samplingrate.html">samplingrate</a>) is too low,
						not all frequencies belonging to the signal can be recorded properly. Unfortunately this even mirrors those
						artefact into other frequencies (<a href="node06.03.00.Aliasing.html">aliasing</a>).<br>
						During the convertion another error happens - the difference of the real value and the rounded version.
						This error appears as quantisation-noise. The higher the <a href="node06.03.00.Bitresolution.html">bitresolution</a>,
						the less noise there is.
						If you load a 16bit-sample into __SFX and play it back with 8bit and 14/16bit, you will hear the diffenrence.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
