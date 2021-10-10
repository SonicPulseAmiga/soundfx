divert(-1)
define(`__TITLE',`Filter')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
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
					<p align="justify">Filter are operators which select certain frequencies from a sound and the suppress them.
					The opposite of filters are boosters. Those would amplify frequencies. In __SFX both are combined
					into one operator; with a positive effect propotion it filters and with a negative iz boosts.<br>
					The names of filter-modules in __SFX consist of two parts, the filtering method and the frequencies the select.
					Below an overview of the methods :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>name            </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>CRS </td><td><b>Cr</b>oss<b>S</b>ection - median filter (simple FIR-filter)<br>
																	These are the most simple, but unfortunately the least controlable filter.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>FIR </td><td><b>F</b>inite <b>I</b>mpulse <b>R</b>esponse</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>IIR </td><td><b>I</b>nfinite <b>I</b>mpulse <b>R</b>esponse</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>BISQ</td><td><b>BiSq</b>uad - combination of FIR and IIR</td></tr>
					</table>
					The ggraphics below are showing the processed frequencies :
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
