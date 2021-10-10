divert(-1)
define(`__TITLE',`Modulator')
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
						__IMAGE(`fx_mod')
					</center>
					<br>
					<p align="justify">Dieser Bereich dient dem Einstellen von modulierbaren Parameter in __SFX. In der ersten Zeile
					geben sie einen Start- und einen Endwert ein. Mit dem '&lt;-&gt;' Knopf k&ouml;nnen sie die Werte tauschen.<br>
					Jetzt noch einige Worte zu den Parametern selber. Seit der Version 3.4 kann man
					echte Einheiten in __SFX verwenden. Das bedeutet, wenn Sie z.B. etwas mit Amplify
					doppelt so laut machen wollen, können Sie alle der folgenden Varianten verwenden :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Beispiel    	</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>2.0			</td><td>Faktor</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>200 %		</td><td>absolut, Prozent</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>2000 %%		</td><td>absolut, Promille</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>+ 100 %		</td><td>relativ, Prozent</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>+ 1000 %%	</td><td>relativ, Promille</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>+ 6 db		</td><td>relativ, Dezibel</td></tr>
					</table>
					Sie sehen also - da geht einiges. Hier nun die derzeitig von __SFX erkannten Einheiten (schreiben
					Sie mir wenn Sie welche vermissen) :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Gruppe   			</th><th>Beschreibung				</th><th>Format				</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Amplitude			</td><td>Faktor						</td><td>Wert				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>absolut, Prozent			</td><td>Wert %				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Promille			</td><td>Wert %%			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>relativ, Prozent			</td><td>+/- Wert %			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>relativ, Promille			</td><td>+/- Wert %%		</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>relativ, Dezibel			</td><td>+/- Wert db		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Pegel				</td><td>Wert lv			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>relative Frequenz	</td><td>Faktor						</td><td>Wert				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Prozent			</td><td>Wert %				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>absolut, Promille			</td><td>Wert %%			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>relativ, Prozent			</td><td>+/- Wert %			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>relativ, Promille			</td><td>+/- Wert %%		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>relativ, Semitones			</td><td>+/- Wert st		</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>relativ, Cents				</td><td>+/- Wert ct		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>relativ, Semitones	&amp; Cents	</td><td>+/- Wert:Wert st:ct</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>absolute Frequenz	</td><td>Herz						</td><td>Wert hz			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Ton						</td><td>Note -/# Oktave  (e.q. C-3, E#2)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>relative Zeit		</td><td>Faktor						</td><td>Zeit				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Prozent			</td><td>Wert %				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>absolut, Promille			</td><td>Wert %%			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Wiederholungen				</td><td>Wert rpts			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>absolute Zeit		</td><td>Stunde						</td><td>Wert h				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Minute						</td><td>Wert m				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Sekunde					</td><td>Wert s				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Millisekunde				</td><td>Wert ms			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Sekunde &amp; Millisekunde	</td><td>Wert:Wert s:ms		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Minute &amp; Sekunde		</td><td>Wert:Wert m:s		</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Stunde &amp; Minute &amp; Sekunde</td><td>Wert:Wert:Wert h:m:s	</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td colspan="2">... ich denke sie haben es kapiert			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Samples					</td><td>Wert sv			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Movie(Kino)frames (24 fps)	</td><td>Wert mf			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>PAL-Videoframes (25 fps)	</td><td>Wert pf			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>NTSC-Videoframes (30 fps)	</td><td>Wert nf			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Verhältnis, Anteil	</td><td>Faktor						</td><td>Wert				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Prozent			</td><td>Wert %				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>absolut, Promille			</td><td>Wert %%			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Anzahl				</td><td>absolut					</td><td>Wert				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>relativ					</td><td>+/- Wert			</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Phase/Winkel			</td><td>Faktor						</td><td>Wert				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>absolut, Prozent			</td><td>Wert %				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>absolut, Promille			</td><td>Wert %%			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Grad						</td><td>Wert °				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Minuten					</td><td>Wert '				</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>Sekunden					</td><td>Wert "				</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Minuten &amp; Sekunden 	</td><td>Wert:Wert ':"		</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td colspan="2">... und so weiter							</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;				</td><td>Radian						</td><td>Wert rad			</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;				</td><td>englische Grad				</td><td>Wert grd			</td></tr>
					</table>
					Nicht alle dieser Einheiten können für alle Parameter benutzt werden und umgekehrt
					können manchmal ungebr&auml;uchliche Einheiten verwendet werden. Auf den letzteren Fall
					weise ich in den zugehörigen Operatorbescheibungen hin.
					</p>
					<align="justify">Die zeite Zeile : Bei der Programmierung von __SFX habe ich großen Wert auf hohe Variabilität gelegt.
					Parameter sollten frei zugänglich und (wenn gew&uuml;nscht) komplex veränderbar sein. So kam es zur Entwicklung von &#39;Blend Shapes&#39;. Dies
					sind Kurven die einen Parameter modulieren. Ein &#39;Blend Shape&#39; gibt immer Werte von 0.0 - 1.0 zurück.
					Dadurch kann es den Parameter vom Start- zum Endwert variieren. Der Startwert wird
					bei Modulation=0.0 verwendet und der Endwert bei Modulation=1.0.
					Folgende Variationsm&ouml;glichkeiten gibt es :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">None	</td><td>Diese Shape gibt immer 0.0 zurück - es wird also nichts
																				geändert. Verwenden Sie diese Variante wenn Sie mit einem
																				konstanten Wert arbeiten möchten, und tragen Sie diesen im
																				1. Parameterfeld ein.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top"><a href="node01.07.01.html">Curve</a>	</td><td>gekrümmter Verlauf</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top"><a href="node01.07.02.html">Cycle</a>	</td><td>Schwingung</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top"><a href="node01.07.03.html">Vector</a>	</td><td>Hüllkurven</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top"><a href="node01.07.04.html">User</a>	</td><td>benutzerdefiniert</td></tr>
					</table>
					</p>
					<align="justify">Beispiele sagen nat&uuml;rlich mehr als tausend Worte. Darum nachfolgend einige für den Amplify-Operator :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Beispiel        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">1	</td><td>Sie wollen die Lautstärke des gesamten Samples um 5% erhöhen.<br>
																			Par.0 :	105 % (100%+5%)<br>
																			Par.1 : egal<br>
																			Modus :	None</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">2	</td><td>Sie wollen die Lautstärke des Samples anfangs um 10 % erhöhen und am
																			Ende auf 60% bringen. Der Lautstärkeabfall soll immer schneller werden.<br>
																			Par.0 :	110 % (100%+10%)<br>
																			Par.1 :	 60<br>
																			Modus :	Curve, Exp="2.0"</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">3	</td><td>Sie möchten einen Tremolloeffekt (zyklische Schwankungen der
																			Lautstärke - "Hubschrauber") erzeugen.<br>
																			Par.0 : 120 %<br>
																			Par.1 :  80 %<br>
																			Modus : Cycle, Sin, Frequency, Frq="1 Hz"</td></tr>
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
