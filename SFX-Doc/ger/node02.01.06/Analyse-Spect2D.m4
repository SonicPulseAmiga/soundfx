divert(-1)
define(`__TITLE',`Analyse-Spect2D')
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
	__SECTION_HEADER(Analyse-Spect2D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt eine 2-dimesionale Darstellung des Frequenzspectrums eines Samples.
Dies zeigt ihnen, aus welchen Frequenzen ein Klang über die Zeit hinweg aufgebaut ist.
Weiterhin können sie damit Anomalien und Störungen, wie Klick und Knackgeräusche aufspüren.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Palette</nobr></td><td align="left"><ul>
<li>gray : zur Darstellung wird eine Graustufenpalette verwendet.</li>
<li>color : zur Darstellung wird eine Farbpalette mit hohem Kontrast verwendet.</li>
&nbsp;</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left"></ul>
welche Fensterfunktion verwendet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lines</nobr></td><td align="left">wieviele Zeitscheiben SFX berechnen soll.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>MaxLin.</nobr></td><td align="left">wieviele Zeitscheiben auf den Bildschirm passen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
die Frequenzauflösung ist dann aber auch nicht so fein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Gamma</nobr></td><td align="left">nichtlineare Verstärkung. Werte von 100 % nach 0 % heben leise Details hervor.
Werte größer 100 % verbergen diese. Der Standartwert von 75 % ist eine gute Wahl
um leise Signalanteile sichtbar zu machen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Mode</nobr></td><td align="left"><ul>
<li>high 2 : vier Ergebnisse werden zu Einem gemittelt</li>
<li>high 1 : zwei Ergebnisse werden zu Einem gemittelt</li>
<li>normal : jeder Wert wird genutzt um ein Ergebnis zu erzeugen</li>
<li>smooth1 : jeder 2. Wert wird genutzt, die Zwischenwerte werden gemittelt</li>
<li>smooth2 : jeder 4. Wert wird genutzt, die Zwischenwerte werden gemittelt</li>
&nbsp;</ul>
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
Wenn das Spektrum fertig berechnet wurde, wird ein neues Fenster geöffnet und der
Graph gezeichnet. Wenn das Fenster aktiv ist und das Sample abgespielt wird, zeichnet
__SFX die Abspielposition auch in Spectrogram ein.<br>
Weiterhin können sie mit der Taste "C" einen der folgenden Modi auswählen :
kein Fadenkreuz, einfaches Fadenkreuz, harmonisches Fadenkreuz. Im letzteren Modus
folgen mehrere horizontale Linien dem Mauszeiger. Jede verdoppelt die Frequenz der Tieferliegenden.
Damit kann man Signalharmonien finden.<br>
Zur Berechnung wird die <a href="../node06.03.00.FourierTrans.html">Fast-Fourier-Transformation</a>
verwendet.<br>
Wenn sie die erzeugten Graphen als Bilder speichern wollen, dann empfehle ich ihnen dafür einen
image-grabber wie SGrab zu verwenden, welchen sie aus dem Aminet beziehen können.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
