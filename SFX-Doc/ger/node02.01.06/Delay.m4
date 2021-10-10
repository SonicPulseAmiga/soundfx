divert(-1)
define(`__TITLE',`Delay')
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
	__SECTION_HEADER(Delay)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt Verz�gerungen, Echos, Flanger und vieles mehr
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einflie�t
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Feedback (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">wie viel vom Ergebnis in den Operator zur�ckgef�hrt wird. Dies kann auch
negativ sein um ein inverses Feedback zu erzeugen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Delay (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">modulierbare Verz�gerung
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">abschlie�ende Lautst�rkeanpassung
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dry</nobr></td><td align="left">gibt an, wie das Verh�ltnis des trockenen Signals aus dem Effect-Parameter berechnet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">wie sollen (sanfte) Zwischenwerte berechnet werden
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
Kurze Delayzeiten (ca. 10 ms) mit hohen Feedbackanteil bewirken einen
metallischen Klangcharakter bei dem Sample.<br>
Wenn man einen Klang hat, der ziemlich abruppt abbricht, kann man ihn mit
einem langen Delay ausklingen lassen. Daf�r sollte man z.B. mit einer
Vektor-H�llkurve den Feedbackanteil gegen Ende hochdrehen.
In __SFX's Delay k�nnen Sie auch die Delayzeit modulieren und diese sogar
als Note eingeben. Ich wei� das das erstmal seltsam klingt, aber es macht
durchaus Sinn. Wenn Sie einen hohen Feedbackanteil (> 89 %) und einen
Effektanteil von 100 % w�hlen, resoniert das Sample auf der Frequenz, die
der Delayzeit entspricht. Wenn Sie daf�r nun 'C-3' eingeben, berechnet
__SFX die richtige Delayzeit.<br>
Und es gibt noch eine n�tzliche Anwendung f�r diesen Operator. Wenn Sie ein
Sample haben welches brummt und Sie die Frequenz der St�rung kennen, w�hlen
Sie Dry='Dry=-Eff', Eff=-100 %, Fb=97 % und Delay=>frq>. Dies wird das
Brummen und alle seine	oberen Harmonien ausl�schen. Leider dauert es
einige Zyclen bis das Brummen leiser wird. Deshalb versuchen Sie bitte
ein bischen Brummen am Beginn der Aufnahme zu haben, welches Sie sp�ter
einfach wegscheiden k�nnen.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
