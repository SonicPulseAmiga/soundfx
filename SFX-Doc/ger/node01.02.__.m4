divert(-1)
define(`__TITLE',`Men&uuml;s')
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
					<p align="justify">Je nachdem welches Fenster in __SFX aktiv ist steht ihnen
					eines der in den folgenden Unterkapiteln beschriebenen Pulldown Menüs zur
					Verfügung.</p>
					<p align="justify">Ausgegraute Men&uuml;eintr&auml;ge signalisieren, das der
					Men&uuml;eintrag derzeit nicht angew&auml;hlt werden kann. Dies kann z.B.
					der Fall sein, wenn sie noch keine Samples geladen haben oder keinen
					Bereich markiert haben.</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border=0 cellpadding="0" width="100%">
					<tr><td>   1.2.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.02.01.html">__NODETITLE(`node01.02.01.html')</a></td></tr>
					<tr><td>   1.2.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.02.02.html">__NODETITLE(`node01.02.02.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
