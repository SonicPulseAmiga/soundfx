divert(-1)
define(`__TITLE',`Vektor')
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
						__IMAGE(`mod_vector')
					</center>
					<br>
					<p align="justify">Dieser Modulator erzeugt max. 20 segmentige Hüllkurven.
					Mit "+" und "-" können Sie Punkte hinzufügen oder entfernen.
					Mit "FlipX" und "FlipY" können Sie die Kurve umklappen.
					In "Nr" können Sie direkt einen Punkt anwählen, um ihn in den nächsten zwei
					Feldern zu positionieren. Sie können die Punkte natürlich auch mit der Maus
					verschieben.<br>
					Dieser Modulator unterstützt Presets. Damit können sie ihre erzeugten Hüllkurven
					abspeichern und wieder abrufen (diese sind dann in allen operatoren verfügbar).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
