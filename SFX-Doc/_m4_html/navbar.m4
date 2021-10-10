define(`__NAVBAR',`
	__SECTION_LABEL
	__SECTION_STYLE_BEG<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
			<td align="left"><font size="-1">
				navtree(__NODE)
			</font></td>
			<td align="right" valign="center">
				<a href="__PATH`'prevlink(__NODE)">
				<img src="__PATH../_img/a_le.__IMGEXT" width="13" height="13" border="0" alt="[prev]"></a>
				<img src="__PATH../_img/space.__IMGEXT" width="2">
				<a href="__PATH`'nextlink(__NODE)">
				<img src="__PATH../_img/a_ri.__IMGEXT" width="13" height="13" border="0" alt="[next]"></a>
			</td>
		</tr></table>
	</td></tr>__SECTION_STYLE_END
	<br>
')dnl
