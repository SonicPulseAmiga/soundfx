divert(-1)
define(`__TITLE',`Einstellungen')
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
					<p align="justify">Viele der Eigenschaften von __SFX k&ouml;nnen sie &uuml;ber die im
					folgenden beschriebenen Fenster an Ihre Vorlieben anpassen.
					</p>
					<p align="justify">Diese Einstellungen werden vorübergehend in ENV:sfx.cfg und
					permanent in ENVARC:sfx.cfg gespeichert.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border=0 cellpadding="0" width="100%">
					<tr><td>   1.6.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.06.01.html">__NODETITLE(`node01.06.01.html')</a></td></tr>
					<tr><td>   1.6.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.06.02.html">__NODETITLE(`node01.06.02.html')</a></td></tr>
					<tr><td>   1.6.3</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.06.03.html">__NODETITLE(`node01.06.03.html')</a></td></tr>
					<tr><td>   1.6.4</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node01.06.04.html">__NODETITLE(`node01.06.04.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
