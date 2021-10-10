divert(-1)
define(`__TITLE',`Interpolationstyp-Fenster')
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
						__IMAGE(`win_interpol')
					</center>
					<br>
					<p align="justify">In diesem Fenster kann man einen Interpolationstyp auswählen und eventuell einen
					Parameter ändern. Dabei wird das gewählte Verhalten graphisch dargestellt.
					Beim Digitalisieren einen Samples werden in sehr kurzen Abst&auml;nden Proben genommen. Dies ergibt dann
					die digitalisierte Wellenform. Einige Effekte ben&ouml;tigen nun aber auch mal Werte zwischen diesen Abtastpunkten.
					Auch hier zeigt sich __SFX flexibel und bietet ihnen eine Auswahl an :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Variante        		</th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">None	</td><td>keine Interpolation (der naheliegendste Wert wird verwendet)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Lin		</td><td>lineare Interpolation</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">Si		</td><td>Kurveninterpolation über <n> Werte</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">Lagrange</td><td>Kurveninterpolation über <n> Werte</td></tr>
					</table>
					F&uuml;r die letzteren Varianten ist es notwendig die Größe des Interpolationsbereiches einzustellen, d.h. wie viele
					Samplewerte aus der Umgebung analysiert werden sollen, um einen Zwischenwert zu errechnen. Nehmen sie auf keinen Fall zu
					gro&szlig;e Werte (größer als 10).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
