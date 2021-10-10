divert(-1)
define(`__TITLE',`Interpolator')
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
					Folgende Parameter eines <a href="node02.01.03.html">Interpolators</a> lassen sich per ARexx ändern.
					Der benötigte &lt;Präfix&gt; (z.B. I1) ist in den Beschreibungen der Operatoren enthalten.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Parameter             </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Präfix&gt;IntType </td><td>"None","Lin","Si","Lagrange"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Präfix&gt;IntRange</td><td>Größe des Interpolationsbereiches</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
