define(`__NAVBAR',`
	__SECTION_LABEL
	<table border="1" width="100%" cellpadding="2" cellspacing="0"><tr bgcolor="#001177"><td>
		<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
			<td align="left"><font size="-1">
				navtree(__NODE)
			</font></td>
			<td align="right">
				<a href="prevlink(__NODE)"><img src="../_img/a_le.png" width="13" height="13" border="0"></a><img src="../_img/space.gif" width="2">
				<a href="nextlink(__NODE)"><img src="../_img/a_ri.png" width="13" height="13" border="0"></a>
			</td>
		</tr></table>
	</td></tr></table>
	<br>
')dnl