divert(-1)
define(`__TITLE',`Bitauflösung')
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
					<p align="justify">Die Bitauflösung gibt an, mit welcher Genauigkeit die analogen Audiodaten ge-
					wandelt wurden. Je höher die Bitrate, desto geringer die Wandlungsfehler
					(Quantisierungsfehler) und desto authentischer das <a href="node06.03.00.Sample.html">Sample</a>.
					Gebräuchliche Bitauflösungen sind 8-, 12-, 16-bit und 24-bit. Folgend eine kleine Aufstellung der
					Auflösungen, des entsprechenden Wandlerbereiches und der üblichen Anwendungsbereiche :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Bits            </th><th colspan="3">Bereich                                             </th><th>Anwendung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">8 </td><td align="right">    -128</td><td>...</td><td align="right">    127</td><td>Heimbereich, Multimedia</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">12</td><td align="right">   -2048</td><td>...</td><td align="right">   2047</td><td>Heimbereich, Multimedia</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">14</td><td align="right">   -8192</td><td>...</td><td align="right">   8191</td><td>semiprofesioneller Bereich</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">16</td><td align="right">  -32768</td><td>...</td><td align="right">  32767</td><td>semiprofesioneller Bereich, Heimstudio</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">24</td><td align="right">-8388608</td><td>...</td><td align="right">8388608</td><td>Profistudio</td></tr>
					</table>
					Man sieht deutlich, schon die Hinzunahme eines Bits, ergibt eine gewaltige
					Erweiterung des Wertebereiches und damit eine enorm erhöhte Qualität.<br>
					Die Amiga-Audiohardware unterstützt normalerweise nur die Wiedergabe von maximal 8-bit.
					Durch einen Trick lassen sich aber auch so ca. 12-bit bzw. 14-bit erreichen.<br>
					Um den Unterschied zu höhren verfahren Sie wie folgt :<ul>
						<li>laden sie ein 16-bit Sample (bei einem 8-bit Sample klingen beide Player logischerweise gleich),
						Verwenden Sie ein Sample mit einer schönen Ausklangphase (z.B. Basedrum, welche zum Ende sehr tief wird).</li>
						<li>spielen sie das Sample bei großer Lautstärke mit beiden Playern ab (eventuell Kopfhörer verwenden).</li>
					</ul>
					Und haben sie den Unterschied am Ende bemerkt?
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
