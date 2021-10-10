divert(-1)
define(`__TITLE',`better Basedrums')
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
					<ul>
						<li>laden Sie eine Basedrum die ihnen nicht "bassig" genug ist</li>
						<li>starten Sie Synthesize_Add<ul>
							<li>stellen Sie für die Länge den Längenwert des Basedrumsamples ein</li>
							<li>stellen Sie für die Frequenz (Pitch) den niedrigsten Ton auf  dem Keyboard ein (65. ...)</li>
							<li>setzen Sie den zweiten Frequenzmodulationswert auf 0.01 (oder noch kleiner)</li>
							<li>wählen Sie als Modus "Curve" mit einem Wert von 2.0</li>
							<li>lassen Sie sich das Sample generieren -> jetzt haben Sie einen tiefen Sinus der gegen Ende noch tiefer wird.</li>
							<li>probieren Sie auch mal andere Wellenformen aus (z.B. Saw)</li></ul>
						</li>
						<li>starten Sie Amplify<ul>
							<li>Source : Sinus-Sample</li>
							<li>stellen Sie für die Faktoren 0 und 100 ein</li>
							<li>Modulationsmodus  : "User2"</li>
							<li>Modulationssample : Basedrum-Sample</li>
							<li>"Start" -> jetzt haben Sie die Hüllkurve der Basedrum für den Sinus übernommen.</li></ul>
						</li>
						<li>Mixen Sie jetzt beide Samples zusammen<ul>
							<li>Src1: Basedrum</li>
							<li>Src2: Sinus</li>
							<li>Mix : ca. 50-70 % (ausprobieren)</li></ul>
						</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
