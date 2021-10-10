divert(-1)
define(`__TITLE',`Einstellungen f&uuml;r den virtuellen Speicher')
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
						__IMAGE(`prefs_vmem')
					</center>
					<br>
					<p align="justify">In diesem Fenster k�nnen Sie verschiedene Voreinstellungen zum virtuellen Speicher
					machen. Folgend die Beschreibung der einzelnen Funktionen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget					</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Enable		</td><td>Soll __SFX �berhaupt virtuellen Speicher benutzen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Swappath	</td><td>Hier kann der Standard-Pfad f�r die Auslagerung eingetragen
																					werden bzw. durch einen Klick auf das PopUp-Symbol ausgew�hlt werden.
																					__SFX wird dort soviel Platz benutzen, wie ben�tigt wird.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">BlkSize		</td><td>Puffergr��e f�r den Laufwerkszugriff in Bytes.
																					Dies hat nichts mit der Blockgr��e ihrer Festplatte zu tun.</td></tr>
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
