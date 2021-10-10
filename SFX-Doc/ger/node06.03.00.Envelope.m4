divert(-1)
define(`__TITLE',`Hüllkurve')
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
					<p align="justify">Eine Hüllkurve (engl. Envelope) ist ein segmentierte Kurve mit
					einem minimalen Pegel von z.B. 0.0 und einem maximalen Pegel von z.B. 1.0.
					Eine solche Kurve dient der	<a href="node06.03.00.Modulation.html">Modulation</a>
					von Effekt-Parametern. Nachfolgend ein Beispiel :
						<!-- @TODO: add graphics -->
					Wenn man z.B. von einer solchen Kurve die Lautstärke eines Sample modulieren
					läßt, dann wird diese anfangs lauter, erreicht ihr Maximum und fällt dann
					langsam wieder ab.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
