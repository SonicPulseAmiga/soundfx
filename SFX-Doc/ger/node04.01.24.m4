divert(-1)
define(`__TITLE',`Dieses Sample wurde nicht korrekt gespeichert ! ...')
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
					<!-- errDamagedFormat -->
					<p align="justify">Beim Laden dieses Samples hat __SFX Fehler im Format der Datei gefunden. __SFX
					wird versuchen, so viel wie möglich von der Datei zu laden. Wenn dies erfolgreich gelingt, sollten sie
					die Datei neu speichern.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
