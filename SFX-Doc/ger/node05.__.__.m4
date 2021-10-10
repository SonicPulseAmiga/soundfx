divert(-1)
define(`__TITLE',`Workshop')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<!-- NEW PAGE -->
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
					<p align="justify">In nachfolgenden Kapiteln werden Sie anhand verschiedener Beispiele in die
					Arbeit mit __SFX eingeführt.Die fertigen Samples finden Sie teilweise im Verzeichnis "Workshop" im
					Programmverzeichnis.
					</p>
					<p align="justify">Zu Beginn ein paar allgemeine Bemerkungen :<ul>
						<li>!!!! AUSPROBIEREN !!!! - es kann nichts kaputtgehen</li>
						<li>die Funktionsweise von __SFX versteht man am besten durch die intensive Benutzung des Programms</li>
						<li>Nicht nur die Standardeinstellungen der Operatoren verwenden</li>
						<li>Nutzen Sie die Modulationsmöglichkeiten - einige Effekte werden nur dadurch so richtig wirkungsvoll z.B. Detune, Smear<br></li>
						<li>wenn Sie Fragen/Probleme haben - schreiben Sie <a href="node00.05.__.html">mir</a>
							- nur so kann ich sehen, wo etwas zu kompliziert beschrieben ist, wo noch Schwachstellen sind.
						</li>
					</ul>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border="0" cellpadding="0" width="100%">
					<tr><td>   5.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.01.__.html">__NODETITLE(`node05.01.__.html')</a></td></tr>
					<tr><td>   5.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.02.__.html">__NODETITLE(`node05.02.__.html')</a></td></tr>
					<tr><td>   5.3  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.03.__.html">__NODETITLE(`node05.03.__.html')</a></td></tr>
					<tr><td>   5.4  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.04.__.html">__NODETITLE(`node05.04.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
