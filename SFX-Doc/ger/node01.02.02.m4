divert(-1)
define(`__TITLE',`Modulmen&uuml;')
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
					<p align="justify">Dieses Men&uuml; finden sie in den Einstellungsfenstern der <a href="node02.__.__.html">Module</a>.</p>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Hauptmen&uuml;</th><th>Untermen&uuml;       </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Projekt       </td><td>Laden                </td><td>Laden von Einstellungen</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Speichern            </td><td>Speichern der aktuellen Einstellung</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Start                </td><td>Starten des aktuellen Modules</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>letzte Einstellungen </td><td>letzte Einstellungen reaktivieren</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Standardeinstellungen</td><td>auf initiale Einstellungen zurücksetzen</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Hilfe                </td><td>Aufruf des Hilfe zu diesem Modul</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Über                 </td><td>Aufruf des Informationsfensters</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Beenden              </td><td>Schließen des Moduls</td></tr>
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
