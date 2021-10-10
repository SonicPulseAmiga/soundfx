divert(-1)
define(`__TITLE',`Synthesize-Add')
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
	__SECTION_HEADER(Synthesize-Add)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Generierung von Wellenformen mittels additativer Klangsynthese,
inclusive Frequenz- & Amplitudenmodulation.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave (Oszillator)</nobr></td><td align="left">Welche Wellenform für den Oszilator verwendet werden soll. Zur Auswahl
stehen folgenden Wellenformen :
<ul>
<li>Sin : Sinus</li>
<li>Tri : Dreieck</li>
<li>Saw : Sägezahn</li>
<li>Sqr : Rechteck</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wave/Pha. (Phase)</nobr></td><td align="left"></ul>
Phasenverschiebung (0-360 Grad)
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Curve Editing/Range</nobr></td><td align="left">Hiermit kann ein linearer Verlauf zwischen 2 Reglern erzeugt werden. Dazu
klickt man den 1. Regler an, dann auf Range und jetzt wählt man den 2. Regler aus.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Curve Editing/Mode</nobr></td><td align="left">Hier kann man auswählen, wie die Regler mit den vertikalen Pfeilen
verschoben oder geflipt werden sollen.
<ul>
<li>Cur : der aktuelle Regler</li>
<li>All : alle Regler</li>
<li>Pos : alle positiven Regler</li>
<li>Neg : alle negativen Regler</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Curve Editing/Nr</nobr></td><td align="left"></ul>
Nummer des Obertones
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Curve Editing/Val</nobr></td><td align="left">Lautstärke für den Oberton
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SLen</nobr></td><td align="left">Länge des Sounds
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/OnePer</nobr></td><td align="left">Berechnet die Länge für eine Periode bei aktueller Rate und trägt das Ergebnis bei SLen ein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SRat</nobr></td><td align="left">Abspielrate des Samples. Diese kann als Rate direkt oder als Note eingegeben werden bzw. im
<a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Volume (Scale)</nobr></td><td align="left">Lautstärke für die Wellenform
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/MaxVol</nobr></td><td align="left">Berechnet den Lautstärkewert für eine optimale Dynamikausnutzung.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Frq (Pitch)</nobr></td><td align="left">Dient der Einstellung der Basistonhöhe. Diese kann direkt oder im
<a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
Es empfiehlt sich die Frequenz eines "C" zu nehmen, um die generierten Klänge
in einem Musikprogramm verwenden zu können.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Harmonics (SVal)</nobr></td><td align="left">In diesem Feld sind 64-Regler für den Lautstärken der Obertöne. Wenn der
Regler in der Mitte ist (Wert=0), dann geht der Ton nicht mit in die
Berechnung ein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Harmonics/horiz. Pfeile</nobr></td><td align="left">Verschieben die Liste horizontal in Einer- oder Fünferschritten.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Harmonics/vert. Pfeile</nobr></td><td align="left">Verschieben die Liste oder den aktuellen Regler vertikal in Einer- oder Fünferschritten.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Harmonics/F-Gadget</nobr></td><td align="left">Flip, spiegelt die Liste oder den aktuellen Regler vertikal.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Frequency (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Faktoren zur Frequenzmodulation
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Amplitude (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Faktoren zur Amplitudenmodulation
</td></tr>
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
Ein jeder Ton besteht aus einer Grundschwingung und mehreren Obertönen, deren
Frequenzen ein Vielfaches der Grundfrequenz betragen. Mit diesem Operator
können Sie die komplexesten Wellenformen entwerfen, indem Sie die einzelnen
<a href="../node06.03.00.Harmonics.html">Obertöne</a> eingeben. Am Bestem
laden Sie eine der abgespeicherten Dateien und schauen bzw. hören sich das
Ergebnis an. Ein jeder Oberton wird durch seine Wertigkeit (=Lautstärke)
definiert. Diese wird von dem Wert "Val" representiert. Dieser Wert sollte
mit zunemenden Obertönen kleiner werden. Positive Werte werden aufaddiert und
Negative werden abgezogen.<br>
Sehr interessante Ergebnisse erhält man, wenn man ein Grundsample für z.B.
C-2 erzeugt und ein weiteres mit folgender Frequenz : [C-2]+(([C#2]-[C-2])/4)
Folgend ein paar Beispiele :<table>
<tr><td>C-0</td><td>65.4063913</td><td>67.35102453</td></tr>
<tr><td>C-1</td><td>130.8127827</td><td>132.7574159</td></tr>
<tr><td>C-2</td><td>261.6255653</td><td>265.5148317</td></tr>
<tr><td>C-3</td><td>523.2511306</td><td>531.0296635</td></tr></table>
Diese zwei Samples mischt man jetzt mit <a href="Mix.html">Mix</a> zu gleichen
Teilen zusammen. Dadurch haben wir eine leichte Schwebung in das Sample
gebracht ; es klingt nun wesentlich lebendiger und fetter.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
