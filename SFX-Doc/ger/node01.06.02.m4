divert(-1)
define(`__TITLE',`Einstellungen f&uuml;r die Samplefenster')
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
						__IMAGE(`prefs_sample')
					</center>
					<br>
					<p align="justify">In diesem Fenster k�nnen Sie verschiedene Voreinstellungen bez�glich den Samplefenstern
					machen. Folgend die Beschreibung der einzelnen Funktionen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gadget					 </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Loader/Saver Path	 </td><td>Diese Pfade werden als Vorgabe in die Filerequester zum Laden und Speichern eingetragen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis Font	 </td><td>Mit diesem Font werden die Lineale am Sample beschriftet.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Safe Check	 </td><td>Dies beschreibt ob __SFX beim Schlie&szlig;en eines Sample r�ckfragen soll,
																					um zu vermeiden das sie ungespeicherte Samples l�schen :<ul>
																						<li>never : Requester erscheint niemals</li>
																						<li>if unsaved : Requester erscheint wenn das Sample noch nicht gespeichert wurde</li>
																						<li>always : Requester erscheint immer</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Storage		 </td><td>Hiermit k�nnen Sie einstellen, ob das Sample im Speicher
																					gehalten wird oder auf die Festplatte ausgelagert werden soll.
																					__SFX entscheidet dies normalerweise automatisch.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Draw Mode	 </td><td>�ber dieses Cycle-Gadget kann man ausw�hlen, wie die Wellenform des Sample
																					gezeichnet werden soll. Es sehen folgende Modi zur Verf�gung :<ul>
																						<li>1. Lines</li>
																						<li>2. Dots</li>
																						<li>3. DotAbs</li>
																						<li>4. Filled</li>
																						<li>5. FilledAbs</li>
																						<li>6. FilledHQ (sehr exaktes aber langsames zeichnen)</li>
																					</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Quick Draw	 </td><td>Wenn dies an ist, wird w�hrend des Scrollens das Raster und die Max-, RMS- und AvgLines weggelassen.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Raster X/Y	 </td><td>Mit diesem Checkboxen k�nnen Sie das Zeichnen des Rasters ausschalten.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Axis X/Y	 </td><td>Und mit diesen k&ouml;nnen Sie die Achsen abschalten. Damit vergr&ouml;ssern Sie den Zeichenbereich f�r die Wellenform.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Unit X/Y	 </td><td>Mit diesen Gadgets k�nnen Sie die Einheiten f�r jede Achse
																				 	ausw�hlen. Diese wird auch von der <a href="node01.04.__.html">Statusbar</a> benutzt.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Max-Lines	 </td><td>Die MaxLines zeigen die maximale Aussteuerung eines Samples an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">RMS-Lines	 </td><td>Diese Linien zeigen die wirkliche akustische Lautst�rke. Die
																					Berechnung dieser und auch der n�chsten, kann etwas dauern (bei l�ngern Samples).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Avg-Lines	 </td><td>Und diese zeigen die durchschnittliche Lautst�rke an.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">No Lines	     </td><td>Sollen die Max-, RMS- und AvgLines bei langen Samples erst einmal weggelassen werden?</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Size Threshold</td><td>Stellen sie ein was f�r sie ein langes Sample ist (Anzahl der Samplewerte)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Window Size	 </td><td>Die Gr��e eines Samplefensters kann hier absolut oder relative (als Promille) zum Screen angegeben werden.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Info Strings	 </td><td>Hier k�nnen sie die Kommentarinformationen die mit den Samples gespeichert werden �ndern.</td></tr>
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
