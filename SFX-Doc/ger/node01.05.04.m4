divert(-1)
define(`__TITLE',`Periodenauswahlfenster')
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
						__IMAGE(`win_period')
					</center>
					<br>
					<p align="justify">In diesem Fenster können Sie die <a href="node06.03.00.Samplingrate.html">Samplingrate</a> auswählen. Folgenden Möglichkeiten
					stehen ihnen zur Verfügung :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante	       				</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Mausauswahl		</td><td>Klicken Sie dazu einfach die gewünschte Note an. Deren
																						Rate und deren Note werden in den unteren Gadgets angezeigt.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Tastaturauswahl	</td><td>Wählen Sie mit F1-F5 die Oktave aus. Mit den folgenden
																						Tasten wählen Sie die Töne aus :<center><font face="courier">
																						 s d   g h j<br>
																						y x c v b n m
																						</font></center></td></tr>
					</table>
					</p>
					<p align="justify">Unter der Keyboardgrafik wird in 3 Feldern die ausgewählte Rate, Note und Frequenz
					angezeigt. Mit dem darunterliegenden Cycle-Gadget können Sie außerdem aus den gebräuchlichsten Raten auswählen.
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Samplingrate 				</th><th>Anwendungsgebiet</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right"> 8000 Hz</td><td align="left">Soundkarten (typisch für SND-AU Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">11025 Hz</td><td align="left">Soundkarten (typisch bei alten Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">22050 Hz</td><td align="left">Soundkarten (typische Frequenz bei vielen Samples)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">28867 Hz</td><td align="left">max. Abspielrate des Paulachips im normalen Modus</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">32000 Hz</td><td align="left">Consumer DATs und Sampler</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">44100 Hz</td><td align="left">CD-Player</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">48000 Hz</td><td align="left">DAT-Recorder/Player</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td align="right">57734 Hz</td><td align="left">max. Abspielrate des Paulachips im Productivity-Modus</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td align="right">96000 Hz</td><td align="left">High-Quality Audiobearbeitung</td></tr>
					</table>
					</p>
					<p align="justify">Mit dem Cycle-Gadget PlayMode können Sie einstellen, ob während der Ratenauswahl
					das Sample abgespielt werden soll oder nicht. Wenn Sie also PlayMode=PlayAll
					einstellen und dann in der Keyboardleiste herumklicken, hören Sie den Ton in
					der entsprechenden Tonhöhe sofort. Dies funktioniert natürlich nur wenn Sie die
					Rate eines bereits vorhandenen Samples einstellen (bei der Tonhöhenauswahl für
					die Operatoren (z.B. Noise) ist ja noch nichts berechnet worden).
					</p>
					<p align="justify">Durch einen Klick auf Okay werden die Werte übernommen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
