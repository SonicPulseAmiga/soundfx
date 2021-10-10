divert(-1)
define(`__TITLE',`Wo l&auml;uft SoundFX ?')
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
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">Das Programm l‰uft auf allen Amigarechnern mit der
					Betriebssystemversion &gt;= 3.0.
					Ich erstelle derzeit keine Version f&uuml; Systeme mit einem einfachen 68000 mehr (kann dies aber
					jederzeit wieder tun, falls das wirklich jemand braucht).
					Da die Berechnung mancher Effekte sehr rechenaufwendig und die Benutzeroberfl‰che
					recht anspruchsvoll ist, wird eine Turbokarte mit Mathematik-Coprozessor empfohlen.
					Auﬂerdem kann durch die 16/32-bit Verarbeitung ein recht hoher Speicherbedarf entstehen.
					Weiterhin hilft	eine Grafikkarte nicht den ‹berblick zu verlieren.
					</p>
					<p align="justify">__SFX kann auch auf MorphOS Systemen (mit 68k Emulation)
					und auf Amiga-Emulatoren (Amithlon und UAE)	verwendet werden.
					</p>
					<p align="justify">Im Idealfall sollte der Rechner wie meiner aussehen - dann
					funktioniert __SFX auch gut ;-). Das w‰hre dann ein A2000 mit 060'er Board
					(64 Mb RAM) und SCSI Controller, Grafikkarte (PicassoIV), Soundkarte
					(Prelude &amp; Repulse) und OS3.5.
					</p>
					<p align="justify">
					Damit es sich richtig gut arbeiten l‰ﬂt (und auch etwas besser aussieht) empfehle ich Ihnen
					noch folgende Programme zu installieren :<ul>
						<li>MagicMenu</li>
						<li>ReqAttack</li>
						<li>VisulaPrefs (dies ist so gut das es auch eine Registrierung wert ist)</li>
					</ul></p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
