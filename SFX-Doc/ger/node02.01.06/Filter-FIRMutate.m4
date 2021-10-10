divert(-1)
define(`__TITLE',`Filter-FIRMutate')
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
	__SECTION_HEADER(Filter-FIRMutate)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dämpft/Boostet das Signal. Die Filterkoeffizienten werden src 2 entnommen.
Damit kontrolliert src 2 alle Parameter wie Filtertyp (Tiefpass, Hochpass,
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einfließt
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Filter-Offset (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Moduliert den Punkt im src 2 sample, wo der Operator beginnt die
Filterkoeffizienten zu entnehmen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Filter-Stretch (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">Ändert die Abbildung von den Samplewerten zu den Filterkoeffizienten.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Window (<a href="../node03.02.03.html">W1</a>)</nobr></td><td align="left">welche Fensterfunktion verwendet wird.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Nr. (Length)</nobr></td><td align="left">Wieviele Koeffizienten benutzt werden sollen. Je mehr Koeffizienten benutzt werden,
desto besser (max. 1024, 64 ist in der Regel ausreichend)
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">wie sollen (sanfte) Zwischenwerte berechnet werden
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">abschließende Lautstärkeanpassung
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
Ein <a href="../node06.03.00.Filter.html">Filter</a> wie dieser, ist sehr experimentell.
Es ist nahezu unmöglich vorher zu wissen wie das Ergebnis klingen wird.
Gute Resultate erhält man, wenn man z.B. den Filter-Offset sehr wenig ändert
(z.B. linear von 0.0 zu 0.1) oder ein relativ kurzes Sample für src 2
verwendet. Weiterhin ist es interessant den filter-stretch Wert von z.B.
0.125 nach 8.0 überzublenden.<br>
Kurven-Interpolation ist nützlich, wenn man sehr kurze Samples für src 2
verwendet oder mit kleinen filter-offset Werten arbeitet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
