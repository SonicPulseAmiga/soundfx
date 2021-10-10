divert(-1)
define(`__TITLE',`Lautstärke')
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
					<p align="justify">Die Lautstärke eines Klanges kann verschieden angegeben werden :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Art</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>maximale Lautstärke /</nobr> <nobr>Spitzenlautstärke</nobr></td><td>größter absoluter Amplitudenausschlag</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td><nobr>durschnittliche Lautstärke</nobr></td><td>Durchschnitt aller absoluten Amplitudenausschläge</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td><nobr>akustische Lautstärke</nobr></td><td>Energie des Klanges</td></tr>
					</table>
					__SFX zeigt ihnen alle diese Pegel im Samplefenster an wenn sie dieses in den <a href="node01.05.03.html">Sampleoptionen</a>
					(oder generell in den <a href="node01.06.02.html">Einstellungen für die Samples</a>) aktiviert haben.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
