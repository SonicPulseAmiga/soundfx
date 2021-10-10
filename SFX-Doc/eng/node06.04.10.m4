divert(-1)
define(`__TITLE',`Operators : Quality')
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
						<b>Q:</b> I have a speech sample and want to remove the 50 Hz hum frequency.
								  Which filter should I use and which parameters?<br>
						<b>A:</b> This is an easy one. Try it, even though it sounds odd. Use the
								  Delay-FX. There should be a preset "DeHummer_50Hz".
								  It resonates on 50 Hz and suppresses the resonation. Works just
								  wonderful. If there is a remaining hum, just apply it twice (or
								  even more often, which is selden required though).<br><br>

						<b>Q:</b> Has anyone experience with Decrackle of records? I&#39;ve experimented
								  with Decrackle (Dif. 200 %, Amp. 200 %, Adjust 95 %) and achieved
								  good results on bigger crackles.
								  But I can&#39;t find good parameters for Filter-FIRLowPass or
								  DeNoise-FIR to eliminate the permanent silent crackles.<br>
						<b>A:</b> I don&#39;t believe it is possible to get perfect results by trying to remove
								  crackles from LP automatically. There are many peak wave forms which are
								  part of the music but have very similar characteristics to clicks.<br>
								  In particular, filters acting in the frequency domain are NOT the way to
								  go. A click or crackle is an impulse signal and therefore contains
								  all frequencies. Removing the high frequencies just spreads it out.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
