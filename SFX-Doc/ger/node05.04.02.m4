divert(-1)
define(`__TITLE',`Ghost Echo')
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
						<li>Sample laden (z.B. Sprachsample oder Drumloop)</li>
						<li>Reverse aufrufen<ul>
							<li>Effektanteil=100%</li></ul>
						</li>
						<li>Optionsfenster aufrufen<ul>
							<li>das Ergebnissample ca. um 5000-10000 Werte verlängern</li></ul>
						</li>
						<li>Echo aufrufen<ul>
							<li>Standard-Einstellungen benutzen</li>
							<li>eventuell Echoanz erhöhen</li>
							<li>eventuell Amp. auf kleineren Wert setzen (bei Übersteuerung)</li></ul>
						</li>
						<li>Reverse aufrufen<ul>
							<li>Effektanteil=100%</li></ul>
						</li>
						<li>und fertig - jetzt hört man ein Sprachsample bei dem die Reflexionen vor dem eigentlichen Schallereignis kommen</li>
					</ul>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
