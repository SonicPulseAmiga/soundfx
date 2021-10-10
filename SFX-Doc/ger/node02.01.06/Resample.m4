divert(-1)
define(`__TITLE',`Resample')
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
	__SECTION_HEADER(Resample)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Ändert <a href="../node06.03.00.Samplingrate.html">Samplingrate</a> und Samplelänge bei gleichbleibendem Klang.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SLen old</nobr></td><td align="left">alte Samplelänge
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>SLen new</nobr></td><td align="left">neue Samplelänge. Der Faktor und die neue Rate werden berechnet und eingetragen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>SRat old</nobr></td><td align="left">alte Samplingrate
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>SRat new</nobr></td><td align="left">neue Samplingrate. Der Faktor und die neue Länge werden berechnet und eingetragen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Factor</nobr></td><td align="left">Der Änderungsfaktor für Rate und Länge. Ein Faktor gleich 1.0 ändert nix.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Lock</nobr></td><td align="left">Bestimmt welcher Parameter festgehalten wird. Wenn man z.B. mehrere Samples mit
unterschiedlichen Raten auf ein Rate bringen möchte, wählt man "SRat" aus.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">wie sollen (sanfte) Zwischenwerte berechnet werden
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Aliasing Filter</nobr></td><td align="left">Wenn aktiviert, wird das Sample erst gefinltert und dann resampled. Dies ist wichtig
wenn die Samplingrate niedriger wird.
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
Wenn man einen Klang digitalisiert hat und dieser im Musikprogramm verstimmt klingt,
kann man dies hier korregieren. Dazu stellt man die Rate ein bei der man z.B.
ein "C" hört, jetzt ruft man Resample auf und stellt die Rate für das "C" ein
(z.B. C-3 -> 16780). Danach hört man bei dieser Rate das "C".<br>
Hiermit kann man auch Wellenformen, die zum Modulieren eines Parameters verwendet
werden sollen, optimal in ihrer Länge anpassen. Dabei sollte man Interpolation
einschalten, damit die Wellenform erhalten bleibt und nicht eckig wird.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
