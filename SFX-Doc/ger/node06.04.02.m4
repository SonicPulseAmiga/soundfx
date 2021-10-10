divert(-1)
define(`__TITLE',`Probleme')
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
						<b>F:</b> Wenn ich Samples von Festplatte abspiele, dann wird der Sound mit Knacksern wiedergegeben.<br>
						<b>A:</b> Ich empfehle eine separate Partition für die ausgelagerten Dateien zu benutzen (in prefs/vmem auswählen).
								  Weiterhin sollte man auf dieser eine gro&szlig;e Blockgrö&szlig;e verwenden
								  (kann z.B. über HDToolBox geändert werden). Ich empfehle 8192..16384 Bytes. WARNUNG : Die Änderung der
								  Blockgrö&szlig;e zerstört alle Daten auf der Partition.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
