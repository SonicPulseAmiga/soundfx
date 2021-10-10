divert(-1)
define(`__TITLE',`Hall')
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
	__SECTION_HEADER(Hall)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Verhallt das Signal. Es werden drei Reflektionsphasen nachgebildet - Frühreflektionen, Haupthall, diffuser Hall (Nachhall).
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Feedback, Early Reflections (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
negativ sein um ein inverses Feedback zu erzeugen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Volume, Early Reflections (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">wie laut die Frühreflektionen im Ergebnis zu hören sein sollen
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Delay, Early Reflections (ErDelS,ErDelE,ErNr)</nobr></td><td align="left">Anzahl der Echos und der abgedeckte Zeitbereich
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Feedback, Main Reflections (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">wie viel vom Ergebnis in den Operator zurückgeführt wird. Dies kann auch
negativ sein um ein inverses Feedback zu erzeugen.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Delay, Main Reflections (MrDelS,MrDelE,MrNr)</nobr></td><td align="left">Anzahl der Echos und der abgedeckte Zeitbereich
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Diff</nobr></td><td align="left">Länge des diffusen Halls.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">abschließende Lautstärkeanpassung
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
Ich weiß das das immer noch nicht perfekt ist. Der Algorithmus ist im Prinzip
der gleiche wie zuvor, nur das nun vile Parameter offengelegt worden sind.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
