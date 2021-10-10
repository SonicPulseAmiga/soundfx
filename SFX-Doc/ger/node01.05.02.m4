divert(-1)
define(`__TITLE',`Informationsfenster')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`win_info')
					</center>
					<br>
					<p align="justify">
					Information gibt, wie der Name schon sagt, nützliche Informationen zum Programm aus,wie zum Beispiel :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Bereich        						</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Programmname			</td><td>Wenn hier nicht &quot;__SFX&quot; steht, benutzen sie das falsche Programm ;-)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Versionsnummer			</td><td>Diese Infos bitte immer mit angeben wenn sie mich wegen einem Problem kontaktieren</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Copyright &amp; Autor	</td><td>...</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Sampleliste				</td><td>eine Liste der geladenen Samples. Wenn Sie einen solchen Eintrag anklicken,
																								werden in den darunterliegenden Feldern zusätliche Informationen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Registrationsinfo.		</td><td>Ihre Registrationsnummer und Ihr Name (wenn dort ein
																								Name steht, dann hoffentlich Ihrer !!! ).</td></tr>
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
