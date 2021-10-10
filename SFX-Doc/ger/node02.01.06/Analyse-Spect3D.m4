divert(-1)
define(`__TITLE',`Analyse-Spect3D')
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
	__SECTION_HEADER(Analyse-Spect3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt eine 3-dimesionale Darstellung des Frequenzspectrums eines Samples
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dir</nobr></td><td align="left"><ul>
<li>front : legt den Beginn des Samples nach vorn</li>
<li>back : legt den Beginn des Samples nach hinten</li>
&nbsp;</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left"></ul>
welche Fensterfunktion verwendet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Lines</nobr></td><td align="left">Wieviele Zeitscheiben SFX berechnen soll.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>MaxLin.</nobr></td><td align="left">Wieviele Zeitscheiben auf den Bildschirm passen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Bands</nobr></td><td align="left">Wieviele Bänder tatsächlich genutz werden sollen. Mit wenigen Bändern dauert die Berechnung nicht so lange,
die Frequenzauflösung ist dann aber auch nicht so fein.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Gamma</nobr></td><td align="left">Nichtlineare Verstärkung. Werte von 100 % nach 0 % heben leise Details hervor.
Werte größer 100 % verbergen diese. Der Standartwert von 75 % ist eine gute Wahl
um leise Signalanteile sichtbar zu machen.
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
Graph gezeichnet. 
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
