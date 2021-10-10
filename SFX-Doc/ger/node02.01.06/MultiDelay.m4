divert(-1)
define(`__TITLE',`MultiDelay')
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
	__SECTION_HEADER(MultiDelay)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Erzeugt bis zu 8 Echoverzögerungen
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Delay (Del1...8)</nobr></td><td align="left">Verzögerung
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Volume (Amp1...8)</nobr></td><td align="left">wie laut soll dieses Delay sein
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Fb Local (FbL1...8)</nobr></td><td align="left">wie viel vom Ergebnis in das Delay zurückgeführt wird. Dies kann auch
negativ sein um ein inverses Feedback zu erzeugen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Fb Global (FbG1...8)</nobr></td><td align="left">wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
negativ sein um ein inverses Feedback zu erzeugen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">abschließende Lautstärkeanpassung
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Dry</nobr></td><td align="left">wie laut soll das Ausgangssample mit eingemischt werden
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Num</nobr></td><td align="left">wie vile Delays sollen verwendet werden
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
Als Neuheit seit V 3.4 können Sie auch hier Noten als Delayzeit eigeben.
Laden Sie doch mal das Preset "Resonate-CEG". Mit diesen Einstellungen
lassen Sie ein Sample im C-Dur Akkord resonieren. Der Effekt wird noch
deutlicher, wenn Sie den Operator zweimal ausführen. Das Ausgangssample
sollte unbedingt vorher mit <a href="Middle.html">Middle</a> bearbeitet werden.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
