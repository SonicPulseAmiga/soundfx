divert(-1)
define(`__TITLE',`benutzerdefiniert')
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
						__IMAGE(`mod_user')
					</center>
					<br>
					<p align="justify">Dieser Modulator erlaubt es einen anderen Samplepuffer als Steuerungsquelle
					zu benutzen. Dabei gibt es folgende Typen :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Normal	</td><td>Wenn die Amplitude des Modulationspuffers ihr negatives
																				Maximum erreicht hat, entspricht das dem Wert 0.0 und beim
																				positiven Maximum wird 1.0 zur�ckgegeben.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Abs		</td><td>Analog zu "Normal" 0.0 wird hier mit dem absoluten Betrag des
																				Samplepuffers gearbeitet. Somit ergibt ein Samplewert auf der
																				Nulllinie einen R�ckgabewert von 0 und eine Maximum (+ oder -)
																				eine 1.0.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">AmpEnv	</td><td>Dieses Shape gibt die Lautst�rkeh�llkurve des modulierenden
																				Samples zur�ck (Stellen Sie sich vor sie spannen einen
																				Gummifaden �ber das Sample).</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">FrqEnv	</td><td>Dieses Shape gibt die Frequenzh�llkurve des modulierenden
																				Samples zur�ck.</td></tr>
					</table>
					</p>
					<p align="justify">F&uuml;r AmpEnv und FrqEnv werden eventuell mal unterschiedliche Algorithmen zur Verf&uuml;gung stehen.
					Diese k&ouml;nnen sie dann &uuml;ber das Cycle-Gadget "Env" ausw&auml;len.
					</p>
					<p align="justify">Die Samplepuffer, die zur Modulation benutzt werden, k�nnen ja durchaus eine
					andere L�nge als das Ergebnissample haben. Wie der Modulationspuffer bez�glich
					seiner L�nge interpretiert wird, kann man wie folgt entscheiden :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Single	</td><td>Falls das Sample k�rzer ist, wird der Rest mit Leerraum
																				aufgef�llt.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Repeat	</td><td>Falls das Sample k�rzer ist, wird es so oft wiederholt, wie
																				es ben�tigt wird.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Stretch	</td><td>Das Sample wird auf die ben�tigte L�nge gedehnt oder gestaucht.</td></tr>
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
