divert(-1)
define(`__TITLE',`Zyklus')
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
						__IMAGE(`mod_cycle')
					</center>
					<br>
					<p align="justify">Dieser Modulator erzeugt eine Schwingung. Dabei l‰ﬂt sich die Wellenform, Phase
					und Frequenz ausw‰hlen. Letzteres kann in verschiedenen Arten eingestellt
					werden :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        					</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Hz				</td><td>Frequenz in hz : 1.5 hz</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Zeit			</td><td>Dauer einer Periode in Zeiteinheiten oder Samples : 5 ms</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Wiederholungen	</td><td>Anzahl von Perioden (Zyklen) : 4 rpts</td></tr>
					</table>
					</p>
					<p align="justify">Bei den Wellenformen Rnd und SRnd werden zuf‰llige Signalpegel erzeugt, die bei SRnd
					gegl‰ttet (smoothed) werden. Der Parameter Frequenz gibt die Anzahl der Zufallswerte pro Sekunde
					(oder wie lange ein Zufallswert gehalten wird) an und der Parameter Phase ist bei diesen Beiden
					Wellenformen ohne Nutzen.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
