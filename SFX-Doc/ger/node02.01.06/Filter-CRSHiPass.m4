divert(-1)
define(`__TITLE',`Filter-CRSHiPass')
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
	__SECTION_HEADER(Filter-CRSHiPass)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Bearbeitet tiefe Frequenzen des Samples, d.h. unterdrückt oder verstärkt sie und läßt Hohe durch.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einfließt. Negative Effektanteile bewirken keine Dämpfung,
sondern eine Verstärkung der zu bearbeitenden Frequenzen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Bereich für die Durchschnittsberechnung. Je breiter dieser ist, desto höher
ist die Cut-Off-Frequenz.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Resonance (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Stärke der Resonanz (auch Peak oder Q-Faktor).
Da eine starke Resonanz das Signal ausdünnt, gibt es einen Amplifyparameter
der parallel zur Resonanz mitläuft,also auch moduliert wird.
Bei einer Resonanz von 0 sollte Amp=100 % sein. Bei einer höheren Resonanz
sollten größere Werte verwendet werden. Diese lassen sich allerdings nur
durch Probieren herausfinden (versuchen sie mal Resonance+100%).
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
Diese <a href="../node06.03.00.Filter.html">Filter</a> basieren auf einem
recht einfachen Modell und sind daher nicht sonderlich genau, dafür aber
recht schnell zu berechnen.<br>
Und seien sie vorsichtig. Wenn sie nur noch ein lautes metallisches Geräusch
hören, haben sie die Resonanz zu weit aufgedreht.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
