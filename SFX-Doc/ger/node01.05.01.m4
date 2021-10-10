divert(-1)
define(`__TITLE',`Samplefenster')
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
						__IMAGE(`win_sample')
					</center>
					<br>
					<p align="justify">
					<b>Das Fenster :</b><br>
					Falls ein Sample geladen oder erzeugt wurde, so sieht man dieses in einem eigenen
					Fenster. Die Änderung von Position und die Größe des Fensters ist über die
					dafür vorgesehenen Gadgets möglich. Um Positionen und Aussteuerungen des Samples
					besser abschätzen zu können werden mehrere Hilfslinien eingezeichnet. Es
					können weitere Hilfslinien eingezeichnet werden, um die maximale, durchschnittliche
					und reale (akustische) Amplitude anzuzeigen.<br>
					Falls ein Loop angeschaltet ist und Start- und Längenwerte gesetzt sind, werden
					diese als vertikale Linien mit Boxen an den oberen Enden eingezeichnet. Wenn
					ein Bereich markiert ist, sehen Sie dies in Form eines hervorgehobenen Rechteckes.
					In der Titelleiste eines Samplefensters wird der Name, die Samplingrate
					und die Länge des Puffers angezeigt. Während des Abspielens eines Samples sehen
					Sie dort die Abspielposition.
					</p>
					<p align="justify">
					<b>Aktionen im Fenster :</b><br>
					Wenn man die Maus umherbewegt, wird der Mauszeiger seine Gestalt ändern um die
					möglichen Aktionen anzuzeigen.<br>
					Die Looplinien können durch Anklicken der Box und Bewegen der Maus bei gedrükkter
					linker Maustaste verschoben werden.<br>
					Wenn Sie außerhalb der Loop-Boxen und markierter Bereiche in das Samplefenster
					klicken und bei gedrückter linker Maustaste die Maus bewegen, wird ein Bereich
					markiert. Wenn Sie in einen markierten Bereich klicken (nicht am Rand), können
					Sie diesen herumschieben, solange sie die Maustaste gedrückt halten. Wenn Sie
					in den Bereich auf die Ränder klicken, können Sie den Bereich in die entsprechende
					Richtung verändern. Hier ein 'Bild' um das etwas zu verdeutlichen (wobei
					die Form des Mauszeigers dies schon verdeutlichen sollte) :
					</p>
					<center>
						<table border="0" cellspacing="1" cellpadding="4">
							<tr bgcolor=__COL_TBL_HEAD>
								<td>
									<table border="2" cellspacing="1" cellpadding="4">
										<tr height="15%">
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">1</td>
											<td width="60%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">3</td>
										</tr>
										<tr height="70%">
											<td width="20%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">4</td>
											<td width="60%" bgcolor=__COL_TBL_ROW1 align="center" valign="center"><br><br>5<br><br><br></td>
											<td width="20%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">6</td>
										</tr>
										<tr height="15%">
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">7</td>
											<td width="60%" bgcolor=__COL_TBL_ROW2 align="center" valign="center">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;8&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
											<td width="20%" bgcolor=__COL_TBL_ROW1 align="center" valign="center">9</td>
										</tr>
									</table>
								</td>
								<td>
									1 linken und oberen Rand verändern<br>
									2 oberen Rand verändern<br>
									3 rechten und oberen Rand verändern<br>
									4 linken Rand verändern<br>
									5 herumbewegen<br>
									6 rechten Rand verändern<br>
									7 linken und unteren Rand verändern<br>
									8 unteren Rand verändern<br>
									9 rechten und unteren Rand verändern<br>
								</td>
							</tr>
						</table>
					</center>
					<p align="justify">
					Dieser Bereich kann nun geschnitten oder vergrößert werden. Falls ein Bereich
					vergrößert wurde, so kann man den Ausschnitt mit den Scrollbalken verschieben.
					Dieser wird auch während des Verschiebens ständig neugezeichnet.
					Während dem Verschieben von Looppointern, dem Markieren von Bereichen und dem
					Ändern des Ausschnittes werden in der <a href="node01.04.__.html">Statusleiste</a> Information über Start-,
					Endpunkte und Länge angezeigt.
					Wenn Sie das Sample größer als 1:1 vergrößert haben und in der <a href="node01.03.09.html">Bereichs-Toolbar</a>
					"Trace" angewählt wurde können Sie mit der Maus (linke Taste) in das Samplefenster
					zeichnen und somit diverse Fehler (Knackser) manuell beseitigen. Die
					Darstellung wird erst erneuert wenn Sie die Maustaste loslassen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
