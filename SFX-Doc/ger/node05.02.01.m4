divert(-1)
define(`__TITLE',`interessante Strings/Synths')
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
					sehr interessante Ergebnisse erhält man bei Flächenklängen und folgender Bearbeitung<ul>
						<li>mit HiPass-Filter bearbeiten (CutOff ca. 0.10 - 0.15)<ul>
							<li>mit Amplify danach wieder auf MaxVol bringen</li>
							<li>Klänge sind nicht mehr so aufdringlich / vordergründig</li></ul>
						</li>
						<li>mit Chorus-Phaser-Operator bearbeiten</li>
						<li>um x Oktaven runtersamplen (Resample) und<ul>
							<li>dann Swap-Operator ausführt</li>
							<li>eine Octave hochsamplen</li>
							<li>die letzten beiden Schritte x-mal wiederholen</li></ul>
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
