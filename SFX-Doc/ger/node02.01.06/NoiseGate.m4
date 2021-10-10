divert(-1)
define(`__TITLE',`NoiseGate')
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
	__SECTION_HEADER(NoiseGate)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Blendet Stellen welche leiser als der Schwellwert sind aus..
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Threshold (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">Amplitude die als Schwellwert für das Ausblenden dient
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Attack</nobr></td><td align="left">Ansprechdauer; der Sound wird nicht abgehackt sonden aus- und eingeblendet.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Shape</nobr></td><td align="left">Hüllform, wie übergeblendet werden soll.
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
Der Operator kann bei Soloaufnahmen (z.B. Sprache, Guitarre, ..) angewandt
werden, deren Pausen verrauscht sind.<br>
Bei percussiven Material empfehle ich kurzere Attackwerte (z.B. 0.5 ms),
sonst kann der Attack auch mal etwas länger sein (z.B. 1.0 ms).<br>
Bevor man diesen Operator nutzt empfiehlt es sich, erst den
<a href="Middle.html">Middle</a> Operator anzuwenden, um das Sample vorzubereiten.<br>
Bedenken sie immer, dass dieser Operator bei echten 16 bit Samples
besser wirkt, als bei 8 bit Samples.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
