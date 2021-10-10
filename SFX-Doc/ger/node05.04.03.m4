divert(-1)
define(`__TITLE',`Enhancer')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
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
					<ul>
						<li>Sample laden</li>
						<li>falls es ein 'altes Tracker-Sample' ist erst mal resamplen und zwar auf mind. 22050 Hz (besser 44100 od 48000 Hz) (Interpolation einschalten !!)</li>
						<li>mit PitchShift bearbeiten<ul>
							<li>Effektanteil="50 %"</li>
							<li>Faktor einmal 2.0 und einmal mit 0.5</li></ul>
						</li>
						<li>die beide Ergebnisse mit 1:1 zusammenmixen</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
