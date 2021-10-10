divert(-1)
define(`__TITLE',`Analyse-Data')
define(`__NODE',__file__)
define(`__PATH',`../')
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
	__SECTION_HEADER(Analyse-Data)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt Histogramme der Amplituden und der Amplituden-Deltas, sowie verschiedenen Statistiken eines Samples
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>keine</nobr></td><td align="left">&nbsp;</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Hinweise)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Wenn die Berechnungen fertig sind, wird ein neues Fenster geöffnet, welches die Graphen und die Werte enthält.
Mit dem "Channel" Cycle-button kann man auswählen, für welchen Kanal man die Graphen sehen möchte.
Mit einem Klick in das Close-Gadget schließen Sie das Fenster.<br>
Die gezeigten Daten helfen ihnen beim Mastering z.B. die Lautstärken
verschiedener Stücke anzugleichen.
Wenn der Operator von ARexx oder vom Batchprozessor aufgerufen wurde, werden die Ergebnisse in
die Datei "Analyse-Data.log" im aktuellen Saverpfad (oder Zielpfad des Batchprozessors) geschrieben.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
