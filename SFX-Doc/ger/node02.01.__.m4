divert(-1)
define(`__TITLE',`Operatoren')
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
					<p align="justify">Ein Operator ist ein Modul welches Samples bearbeitet oder generiert. Es gibt prinzipiell
					3 Arten von Operatoren :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        			</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Effekte		</td><td>Bearbeiten ein oder mehrere Quellsamples zu ein oder mehrere Resultaten.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Generatoren	</td><td>Generieren neue Sounds (Synthesizer), ben&ouml;tigen keine Quellen</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Analysatoren</td><td>Analysieren Samples (wer h&auml;tte das gedacht), erzeugen keine neuen Samples</td></tr>
					</table>
					</p>
					<p align="justify">Die meisten Operatoren gleichen sich in Ihrem Aufbau.
					Deshalb werde ich allgemeine Details an dieser Stelle erklären und in der Dokumentation zu den jeweiligen
					Operatoren auslassen.
					</p>
					<p align="justify">Alle Parameter die Sie in einem Operator ändern,
					werden als aktuelle Einstellungen während der Rechner läuft gehalten.
					D.h., wenn Sie einen Operator ein weiteres mal benutzen (auch wenn
					Sie das Programm zwischenzeitlich beendet haben), haben alle Parameter
					die Werte des letzten Aufrufes. Falls Sie Samplepuffer zur Modulation
					benutzt haben und diese inzwischen geschlossen haben, ändert __SFX
					diese Einstellungen ab, da die Samples ja nicht mehr existieren.
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
					<tr><td>   2.1.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.01.html">__NODETITLE(`node02.01.01.html')</a></td></tr>
					<tr><td>   2.1.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.02.html">__NODETITLE(`node02.01.02.html')</a></td></tr>
					<tr><td>   2.1.3</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.03.html">__NODETITLE(`node02.01.03.html')</a></td></tr>
					<tr><td>   2.1.4</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.04.html">__NODETITLE(`node02.01.04.html')</a></td></tr>
					<tr><td>   2.1.5</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.05.html">__NODETITLE(`node02.01.05.html')</a></td></tr>
					<tr><td>   2.1.6</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.06.html">__NODETITLE(`node02.01.06.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
