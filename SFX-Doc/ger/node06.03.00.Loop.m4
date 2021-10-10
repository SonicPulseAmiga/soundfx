divert(-1)
define(`__TITLE',`Loop')
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
					<p align="justify">
						Loops dienen der Wiederholung eines Teilstückes aus einem <a href="node06.03.00.Sample.html">Sample</a>.
						Dies benutzt man z.B. in der Ausklangphase eines Intruments um den Ton länger zu halten.<br>
						Der Start- und Endpunkt eines Loops sollten auf einem Nulldurchgang (oder mindestens auf ähnlichen Werten) liegen,
						da es sonst zu Knackgeräuschen kommt.
						Auf der <a href="node01.03.09.html">Bereichs-Toolbar</a> finden Sie die Funktionen zum Justieren der Loopmarker.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
