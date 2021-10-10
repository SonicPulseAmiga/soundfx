divert(-1)
define(`__TITLE',`Fensterfunktions-Fenster')
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
						__IMAGE(`win_winfunc')
					</center>
					<br>
					<p align="justify">In diesem Fenster kann man eine Fensterfunktion auswählen und eventuell einen
					Parameter ändern. Dabei werden die Funktionen graphisch dargestellt. Der obere Graph zeigt dabei
					den Verlauf der Fensterfunktion im Zeitbereich und der untere Graph den Effekt im Frequenzbereich.
					Hiermit kann man erkennen, das einige Funktionen im Stopband besser filtern aber dann auch die
					Flanke weniger steil ausfällt.
					Dieses Fenster wird in der Regel von einem <a href="node02.01.__.html">Operator</a> (<a href="node02.01.04.html">Fensterfunktionsauswahl</a>) aus aufgerufen.
					</p>
					<p align="justify">Die Auswahl einer Fensterfunktion stellt immer einen Kompromi&szlig; dar.
					Hier ein Beispiel für FIR-Filter :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Fenster        			</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Rectangle	</td><td>+ hohe Flankensteilheit<br>
																						- schlechte Dämpfung</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Hamming		</td><td>- schlechte Flankensteilheit<br>
																						+ hohe Dämpfung</td></tr>
					</table>
					</p>
					<p align="justify">Durch mehrmalige Anwendung eines Filters werden jedoch beide Merkmale besser.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
