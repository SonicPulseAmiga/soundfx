divert(-1)
define(`__TITLE',`Kurve')
define(`__NODE',__file__)
define(`__PATH',`')
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
			__SECTION_HEADER(__CHAPTER`'__TITLE)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`mod_curve')
					</center>
					<br>
					<p align="justify">Dieser Modulator erzeugt einen gekrümmten Verlauf. Die Krümmung wird über den
					Parameter "Exponent" gesteuert und auch graphisch dargestellt oder kann mit der
					Maus verändert werden, indem man die Kurve herumschiebt, bis sie einem gefällt.
					Nachfolgend einige Beispiele :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        					</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Linear (exp=1.0)	</td><td>Gibt am zu Samplebeginn 0.0 und am Ende 1.0 zurück. Dazwischen
																							wird geradlinig = linear übergeblendet.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">SpeedUp (exp>1.0)	</td><td>&Auml;hnlich dem vorhergehenden, unterscheidet sich dies dadurch,
																							das es einen beschleunigten Verlauf erzeugt, das heißt - die
																							Werte ändern sich anfangs langsamer und gegen Ende schneller.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">SlowDown (exp<1.0)	</td><td>Analog zu SpeedUp liefert dies einen gebremsten Verlauf.</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
