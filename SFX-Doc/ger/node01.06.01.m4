divert(-1)
define(`__TITLE',`Einstellungen f&uuml;r das GUI')
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
						__IMAGE(`prefs_gui')
					</center>
					<br>
					<p align="justify">In diesem Fenster können Sie verschiedene Voreinstellungen bezüglich des __SFX
					GUI's machen. Folgend die Beschreibung der einzelnen Funktionen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget        				</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Palette	(links)	</td><td>Hier wählen Sie den zu ändernden Paletteneintrag aus.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Red,Green,Blue	</td><td>Mit diesen Gadgets können sie die Farbanteile des gewählten Eintrages ändern.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Screen			</td><td>Im nachfolgenden Auswahlfenster können Sie eine Bildschirmauflösung
																						auswählen, wobei nur für __SFX geeigneten Modi angezeigt werden.<br>
																						Bitte beachten sie, das in HighColor (15/16 bit) und TrueColor (24 bit) Grafikmodi
																						die Darstellung der Samples leicht unterschiedlich aussehen kann
																						(markierte Bereiche, Loops).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Scr. Font		</td><td>Hier können Sie einen Schriftsatz auswählen. Jetzt
																						stehen auch nichtproportionale Schriften zur verfügung.
																						Der standartmäßig eingestellte Zeichensatz (Trinomic.font)
																						ist notwendig, wenn Sie __SFX auf einem HiresNoLace-Screen
																						(640x256) benutzen wollen. Bei einer Auflösung von 1024x768 benutze
																						ich XHelvetica in der Größe 11.<br>
																						Verwenden Sie nur dann eine größere Schrift, wenn Sie
																						auch eine entsprechende Bildschirmauflösung eingestellt
																						haben.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Stat. Font		</td><td>Dieser Schriftsatz wird für die Felder in der Statusleiste verwendet.
																						Ich empfehle hierfür einen recht kleinen Font wie z.B. Trinomic in der Größe 6
																						oder XHelvetica in der Größe 9 zu verwenden.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Palette	(rechts)</td><td>Hier wählen Sie den Paletteneintrag aus der dem Stift zugewiesen werden soll.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Pens			</td><td>W&auml;len sie hier den Farbstift den sie &auml;ndern m&ouml;chten</td></tr>
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
