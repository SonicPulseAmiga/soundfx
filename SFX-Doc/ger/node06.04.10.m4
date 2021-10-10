divert(-1)
define(`__TITLE',`Operatoren : Qualit�t')
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
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">
						<b>F:</b> Ich habe ein Sprachsample bei dem ich ein 50 Hz Brummen entfernen
								  m�chte. Welchen Filter soll ich anwenden und welche Parameter
								  sind einzustellen?<br>
						<b>A:</b> Das ist einfach. Probieren sie es aus, auch wenn es etwas
								  merkw�rdig klingt. Starten sie den Delay-Effekt. Dort gibt es
								  ein Preset "DeHummer_50Hz".
								  Es resoniert auf 50 Hz und unterdr�ckt genau diese Resonanz.
								  Das klappt einfach prima. Wenn trotzdem noch etwas von dem
								  St�rger�usch zu h�ren ist, wiederholen sie die Berechnung
								  einfach noch einmal (oder noch �fters, was jedoch selten
								  notwendig sein sollte).<br><br>

						<b>F:</b> Hat jemand Erfahrung mit dem Entknacksen von Schallplatten?
								  Ich habe mit DeCrackle rumexperimentiert (Dif. 200 %, Amp.
								  200 %, Adjust 95 %) und gute Resultate f�r laute Knackser
								  erziehlt.
								  Ich finde jedoch keine brauchbaren Parameter f�r Filter-FIRLowPass
								  oder DeNoise-FIR um den sttischen leisen Rauschteppich in
								  den Griff zu bekommen.<br>
						<b>A:</b> Es ist meiner Meinung nach wohl nicht m�glich perfekte Ergebnisse
								  durch automatisches Entfernen der Knackser zu erhalten.
								  Es gibt so viele Wellenformen mit Spitzen, die Bestandteil
								  der Musik sind aber �hnliche Charakteristiken wie Knackser
								  aufweisen.<br>
								  Auf alle F�lle sind Filter die im Frequenzbereich operieren
								  nicht die L�sung. Ein Klick oder Knackser ist ein Impulssignal
								  und enth�lt alle Frqquenzen. Das Entfernen der h�hen Frequenzen
								  w�rde es nur verlagern.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
