divert(-1)
define(`__TITLE',`Equalize-FFT-3D')
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
	__SECTION_HEADER(Equalize-FFT-3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Morpht zwischen 8 Equalizerkurven in einem Würfel zu einer Ergebniskurve,,
welche die Lautstärke der Frequenzbestandteile des Samples ändert.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Frequency-Curves (Eqf1..8)</nobr></td><td align="left">Wenn Sie auf das PopUp-Symbol klicken, erscheint ein Dateirequester, aus
dem Sie ein Equalizerpreset auswählen können. Diese können mit dem
<a href="Equalize-FFT.html">Equalize-FFT</a> Operator erstellen.
Sie könenn sogar mehrere Pesets auswählen. Dann werden mehrere Kurven geladen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>X-Axis (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Position des Punktes auf der X-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Y-Axis (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Position des Punktes auf der Y-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Z-Axis (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Position des Punktes auf der Z-Achse
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Path</nobr></td><td align="left">In diesem Feld wird der Pfad, als Kurve im Würfel, dargestellt. Während
der Berechnung wird ein Punkt entlang der Kurve vom Begin bis zum Ende
wandern. Die Entfernung dies Punktes zu den Ecken bestimmt wie stark die
Equalizerkurven die den Ecken zugeordnet wurden in die Ergebniskurve
einfließen.<br>
Mit "View" läßt sich der Ansichtspunkt festlegen und mit "Prec." die
Genauigkeit mit der die Kurve gezeichnet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">welche Fensterfunktion verwendet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
die Frequenzauflösung ist dann aber auch nicht so fein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Steps</nobr></td><td align="left">Aller wieviel Samples eine Transformation erstellt werden soll. Je öfters diese berechnet
werden, desto genauer das Ergebnis und desto höher auch die Rechenzeit. Steps darf max. halb
so groß wie die Nummer der Bänder sein.
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
Die Resultate dieses Operators sind sehr unvorhersehbar. Das bedeutet das sie
ruhig etwas herumexperimentieren sollten (z.B. nehmen sie doch mal ein langes
Rausch-Sample und eines der mitgelieferten Presets).
The Effekt ist recht gut für z.B. Sci-Fi Sounds geeignet.<br>
Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a> verwendet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
