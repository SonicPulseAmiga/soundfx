divert(-1)
define(`__TITLE',`Stapelverarbeitungsstatusfenster')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`win_batchprogress')
					</center>
					<br>
					<p align="justify">In diesem Fenster wird der Fortgang der Stapelverarbeitung angezeigt.
					Dies geschieht über drei Bereiche. Der obere gibt einen Gesamt&uuml;berblick. Im
					Feld &quot;Stack&quat; wird angezeigt wieviele Samples noch zu bearbeiten sind. Diese Zahl
					kann im Verlauf noch wachsen, wenn weitere Unterverzeichnisse gefunden werden. Das Feld
					&quot;Done&quot; z&auml;hlt die Anzahl der fertig bearbeiteten Dateien und das Feld &quot;File&quot;
					informiert &uuml;ber das aktuelle Sample. Die darunterliegenden Statusbalken zeigen den Verlauf
					f&uuml;r die aktuelle Datei und f&uuml;r die aktuelle Operation an.
					</p>
					<p align="justify">Die Operation kann jederzeit mit einem Klick auf "Stop", dem Drücken
					der Tasten "S","s","ESC" oder einem Klick auf das "Close"-Gadget des Fensters abgebrochen werden.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
