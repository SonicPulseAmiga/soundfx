divert(-1)
define(`__TITLE',`Quellenauswahl')
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
						__IMAGE(`fx_srcsel1')
					</center>
					<br>
					<center>
						__IMAGE(`fx_srcsel2')
					</center>
					<br>
					<p align="justify">Hiermit kann eine zu bearbeitende Quelle ausgew&auml;hlt werden. Das Cylegadget hinter dem Namen des Sourcepuffers, ermöglicht
					die Auswahl des zu bearbeitenden Bereiches. __SFX schlägt ihn automatisch den wahrscheinlich gewünschten Modus
					vor, d.h. wenn Sie z.B. einem Bereich markiert haben, ist Range
					voreingestellt. Folgende Varianten sind möglich :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">All		</td><td>das gesamte Sample wird bearbeitet</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Window	</td><td>nur der aktuell sichtbare (gezoomte) Bereich
																				wird bearbeitet</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Range	</td><td>nur der aktuell markierte Bereich wird bearbeitet</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
