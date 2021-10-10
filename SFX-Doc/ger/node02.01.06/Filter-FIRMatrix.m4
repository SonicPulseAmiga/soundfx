divert(-1)
define(`__TITLE',`Filter-FIRMatrix')
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
	__SECTION_HEADER(Filter-FIRMatrix)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Filtert oder Boostet das Signal �ber eine Convolutionsmatrix.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">wie stark der Effekt in das Ergebnis einflie�t. Negative Effektanteile bewirken keine D�mpfung,
sondern eine Verst�rkung der zu bearbeitenden Frequenzen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Cut-Off (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">Bereich f�r die Durchschnittsberechnung. Je breiter dieser ist, desto h�her
ist die Cut-Off-Frequenz (ist relativ zu sehen, da sich �ber die Matrix
v�llig unterschiedliche Charakteristika einstellen lassen).
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Resonance (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">St�rke der Resonanz (auch Peak oder Q-Faktor).
Da eine starke Resonanz das Signal ausd�nnt, gibt es einen Amplifyparameter
der parallel zur Resonanz mitl�uft,also auch moduliert wird.
Bei einer Resonanz von 0 sollte Amp=100 % sein. Bei einer h�heren Resonanz
sollten gr��ere Werte verwendet werden. Diese lassen sich allerdings nur
durch Probieren herausfinden (versuchen sie mal Resonance+100%).
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Matrix</nobr></td><td align="left">Eine Liste der Faktoren die f�r die Multiplikationen im Querschnittsbereich
benutzt werden. Die einzelnen Werte sollten nicht gr��er als 15.0 gew�hlt werden.
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
Ein Matrix-<a href="../node06.03.00.Filter.html">Filter</a>, wie dieser,
ist ein FIR-Filter bei dem Sie die Koeffizienten selber eintragen k�nnen,
falls Sie diese z.B. mit einem andern Programm entworfen haben.<br>
�ber die Matrix lassen sich die unterschiedlichsten Filtercharakteristika
simulieren. Wenn Sie z.B. einen Hochpassfilter simulieren m�chten, setzen
Sie den ersten Wert auf z.B. 5 und die anderen Werte bis zur Cut-Off Grenze
auf -1 (also bei Cut-Off-Range=7, die n�chsten 6 Werte).
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
