divert(-1)
define(`__TITLE',`Sample hat keine Samplingrate, SoundFX nimmt die Standardrate !')
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
					<!-- errNoSampleRate -->
					<p align="justify">Dieses Sample wurde wahrscheinlich nicht korrekt gespeichert.
					Das Sample kann nun zu hoch oder zu tief klingen. Bitte korrigieren Sie die Einstellungen in den
					<a href="node01.05.03.html">Sampleoptionen</a>.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
