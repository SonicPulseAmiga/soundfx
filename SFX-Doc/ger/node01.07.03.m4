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
					<p align="justify">Dieser Modulator erzeugt max. 20 segmentige H�llkurven.
					Mit "+" und "-" k�nnen Sie Punkte hinzuf�gen oder entfernen.
					Mit "FlipX" und "FlipY" k�nnen Sie die Kurve umklappen.
					In "Nr" k�nnen Sie direkt einen Punkt anw�hlen, um ihn in den n�chsten zwei
					Feldern zu positionieren. Sie k�nnen die Punkte nat�rlich auch mit der Maus
					verschieben.<br>
					Dieser Modulator unterst�tzt Presets. Damit k�nnen sie ihre erzeugten H�llkurven
					abspeichern und wieder abrufen (diese sind dann in allen operatoren verf�gbar).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
