divert(-1)
define(`__TITLE',`Gamma')
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
	__SECTION_HEADER(Gamma)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Gammakorrektur für Samples
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Gamma (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Wert für nichtlineare Verstärkung/Abschwächung.<br>
Bei einem Wert von 1.0 passiert nichts. Bei größeren Werten wird abgeschwächt
(leise Signale werden noch leiser). Bei kleineren Werten wird verstärkt
(leise Signale werden lauter).
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
Diesen Operator benötigt man z.B. für folgenden Fall:
Man hat ein Sample welches voll ausgesteuert ist, aber wegen seiner <a href="../node06.03.00.Dynamic.html">Dynamik</a>
recht leise ist. Jetzt würde man das Ganze gern so verstärken, daß die
lauten und die leisen Werte so bleiben wie sie sind und die mittleren Werte
angehoben werden. Genau dies macht dieser Operator. (Im Prinzip das Gleiche
wie der Gamma-Operator bei Bildbearbeitungsprogrammen.)
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
