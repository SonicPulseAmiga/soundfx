divert(-1)
define(`__TITLE',`Modulation')
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
					<p align="justify">
						Als Modulation, bezeichnet man einen Vorgang bei dem ein Parameter durch ein Signal variiert wird.
						Dies kann z.B. zyklisch durch eine Sinusschwingung oder auch durch eine <a href="node06.03.00.Envelope.html">H�llkurve</a> geschehen.
						In Synthesizern findet man h�ufig sogenannte LFOs (Low-Frequenz-Oszillator). Diese dienen als Modulationsquelle, d.h. sie erzeugen
						ein langsam schwingendes Signal, welches einen anderen Parameter (z.B. die Tonh�he) �ndert.
						Eine H�llkurve wird z.B. zur Formung des Lautst�rkeverlaufes benutzt.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
