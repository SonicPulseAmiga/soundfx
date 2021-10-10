divert(-1)
define(`__TITLE',`Filter')
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
					<p align="justify">Filter sind Operatoren die von einem Klang bestimmte Frequenzen selektieren und
					diese unterdrücken. Das Gegenteil eines Filters ist ein Booster. Dieser verstärkt die ausgewählten
					Frequenzen. In __SFX ist beides in einem Operator kombiniert; mit positivem Effektanteil wird
					gefiltert und mit negativem 'geboostet'.<br>
					Der Name der Filtermodule in __SFX setzt sich aus zwei Teilen zusammen, dem
					Filterverfahren und dem zu bearbeitenden Frequenzbereich. Nachfolgend ein Überblick über die Verfahren :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Name            </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>CRS </td><td><b>Cr</b>oss<b>S</b>ection - Mittelwertfilter (simple FIR-Filter)<br>
																											Dies sind die einfachsten, aber leider auch am schwersten einstellbaren Filter.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>FIR </td><td><b>F</b>inite <b>I</b>mpulse <b>R</b>esponse - Endliche Impulsantwort</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>IIR </td><td><b>I</b>nfinite <b>I</b>mpulse <b>R</b>esponse - Unendliche Impulsantwort</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>BISQ</td><td><b>BiSq</b>uad - Kombination aus FIR und IIR</td></tr>
					</table>
					Folgende Grafiken zeigen die bearbeiteten Frequenzbereiche :
						<!-- @TODO: add graphics -->
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
