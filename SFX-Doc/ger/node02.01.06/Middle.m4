divert(-1)
define(`__TITLE',`Middle')
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
	__SECTION_HEADER(Middle)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Sucht den Mittelpunkt der Sampledaten und zentriert dieses dann an der x-Achse.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>keine</nobr></td><td align="left">&nbsp;</td></tr>
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
Wenn man Töne selbst digitalisiert hat, liegen die Sampledaten oft einwenig
neben der x-Achse. Dies bedeutet, daß ein konstanter Gleichspannungsanteil
(Offset) auf dem Signal liegt. Hier sollte man diesen Operator anwenden, da
sonst das Signal bei einer Weiterverarbeitung immer weiter von der Mitte
weggleitet und es dadurch zu einseitigen <a href="../node06.03.00.Overdrive.html">Übersteuerungen</a> kommt.
Dieser Operator vermeidet das es dazu kommen kann (Auch wenn manche ihre Samples
ordentlich übersteueren, daran liegt es dann nicht mehr. Selber schuld:)<br>
Neben dem Übersteuern ist dies auch für Restauration (<a href="DeCrackle.html">DeCrackle</a>, <a href="NoiseGate.html">NoiseGate</a>, ...)
sehr wichtig, damit diese Operatoren die Signale richtig analysieren können.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
