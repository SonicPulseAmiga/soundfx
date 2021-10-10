divert(-1)
define(`__TITLE',`Technischer Hintergrund')
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
					<p align="justify">__SFX wird mit folgenden Werkzeugen entwickelt :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Werkzeug</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>SAS C/C++</td><td>Hauptcompiler</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>GoldEd</td><td>Editor auf dem Amiga</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>JEdit</td><td>Java basierter Editor, wird über Netzwerklaufwerke genuzt</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>htmldoc</td><td>zur Erzeugung von ps/pdf Dateien</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>gnu m4</td><td>zur Erzeugung von html Dateien</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>debug tools, splint, muforce</td><td>bug-tracking</td></tr>
					</table>
					Vielen Dank an die Beteiligten Autoren!
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
