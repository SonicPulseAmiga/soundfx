divert(-1)
define(`__TITLE',`Saver')
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
					<p align="justify">Ein Saver ist ein Modul, welches Samples in einem bestimmten Format speichert. __SFX
					bietet ihnen nahezu alle gebr&auml;uchlichen Formate zum Speichern an.
					</p>
					<p align="justify">Fast alle Saver haben Einiges gemeinsam, was ich im folgenden beschreiben werde.
					Wenn sie dies in den <a href="node01.06.04.html">Einstellungen</a> die Option
					"Save Icons" ausgewählt haben, erzeugen die Saver ein Standart-Piktogramm zum Sample. Weiterhin
					generieren die Saver einen Dateikommentar mit Informationen wie Format, Kanäle und Länge.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td>   2.5.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.05.01.html">__NODETITLE(`node02.05.01.html')</a></td></tr>
					<tr><td>   2.5.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.05.02.html">__NODETITLE(`node02.05.02.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
