divert(-1)
define(`__TITLE',`Synthesize-FM')
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
	__SECTION_HEADER(Synthesize-FM)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Generierung von Wellenformen mittels FM-Synthese nach dem Vorbild eines Yamaha DX-7.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/SLen</nobr></td><td align="left">Länge des Sounds
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/SRat</nobr></td><td align="left">Abspielrate des Samples. Diese kann als Rate direkt oder als Note eingegeben werden bzw. im
<a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/Volume (Scale)</nobr></td><td align="left">Lautstärke für die Wellenform
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Miscellaneous/Frq (Pitch)</nobr></td><td align="left">Dient der Einstellung der Basistonhöhe. Diese kann direkt oder im
<a href="../node01.05.04.html">Periodenauswahlfenster</a> ausgewählt werden.
Es empfiehlt sich die Frequenz eines "C" zu nehmen, um die generierten Klänge
in einem Musikprogramm verwenden zu können.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Miscellaneous/Operator</nobr></td><td align="left">Wählen sie für welchen Operator (Wellenform-Generator) sie die Wellenform, die Amplitude und die Frequenz einstellen möchten.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wave (Oszillator)</nobr></td><td align="left">Welche Wellenform für den Oszilator verwendet werden soll. Zur Auswahl
stehen folgenden Wellenformen :
<ul>
<li>Sin : Sinus</li>
<li>Tri : Dreieck</li>
<li>Saw : Sägezahn</li>
<li>Sqr : Rechteck</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Wave/Pha. (Phase)</nobr></td><td align="left"></ul>
Phasenverschiebung (0-360 Grad)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Frequency</nobr></td><td align="left">Hier wird die Frequenz des Operators relativ zur Basistonhöhe definiert.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Amplitude</nobr></td><td align="left">Hier wird die Amplitude des Operators definiert.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Modulation-Matrix</nobr></td><td align="left">Ein Häkchen bedeutet das die Amplitude des Quelloperators die Frequenz
des Zieloperators moduliert. Wie sie leicht sehen können, gibt es hier
eine Unmenge von Möglichkeiten.
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
Als Besonderheit können sie mit diesem Operator auch Presets importieren
die mit dem Programm FMSynth (Fileversion 1.3) erzeugt worden. An dieser
Stelle recht herzlichen Dank an den Autor Christian Stiens für den
Sourcecode.<br>
FM-Synthese ist eine recht komplexe Angelegenheit. Schauen sie sich die
beiliegenden Presets an und basteln sie an diesen rum. Gute Prests können
sie mir gerne zusenden, damit ich sie in weiteren Versionen mit beilegen
kann.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
