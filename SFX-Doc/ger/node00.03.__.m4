divert(-1)
define(`__TITLE',`Copyright')
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
					<p align="justify">
					__SFX<br>
                    &copy; Copyright 1993-esyscmd(`date +%Y') Stefan Kost. All Rights Reserved.
					</p>
					<p align="justify">
					Es werden keine Garantien für die vollständige Funktion der Software gegeben.
					Weiterhin wird keine Haftung für Schäden übernommen die durch die unsachgemäße
					Benutzung der Software entsteht.<br>
					Falls Sie einen Fehler in der Software gefunden haben, dann schreiben Sie mir
					bitte eine genaue Beschreibung desselben. Ich bin bemüht diese schnellstens
					zu entfernen.
					</p>
					<p align="justify">
					Die Software ist, bis auf das Keyfile, frei kopierbar - es ist sogar erwünscht
					diese zu verbreiten, solange dafür keine Gebühren größer 5,-DM verlangt werden.
					Wenn das Programm in Programmsammlungen aufgenommen werden soll, so kontaktieren
					 Sie mich bitte vorher.
					</p>
					<p align="justify">
					Die Demo-Version darf ohne vorherige Anfrage auf folgenden Serien / CD`s veröffentlicht werden.:<ul>
						<li>Aminet CD</li>
						<li>Fred Fish CD</li>
						<li>Saar PD-Serie</li>
						<li>Time PD-Serie</li>
						<li>Amiga-Magazin PD/CD</li>
						<li>AmigaPlus CD's</li>
					</ul>
					</p>
					<p align="justify">
					Ich rate auf keinen Fall eine gecrackte Version zu benutzen, da diese wahrscheinlich
					sehr oft abstürzen wird und möglicherweise Ihre Festplatte beschädigt !<br>
					Wenn Sie __SFX tatsächlich für zu teuer halten, sagen sie mir lieber Bescheid.
					</p>
					<hr>
					<p align="justify">
					<a href="http://www.boing.nu/pm/" target="new">popupmenu.library</a><br>
					&copy; Copyright ?-2000 Henrik Isaksson, All Rights Reserved.</p>
					<hr>
					<p align="justify">
					openurl.library<br>
					&copy; Copyright ?-2000 Troels Walsted Hansen, All Rights Reserved.</p>
					<hr>
					<p align="justify">
					<a href="http://www.stormamiga-lib.de" target="new">stormamiga.lib</a><br>
					&copy; Copyright ?-2001 Matthias Henze, All Rights Reserved.<br>
					Probieren sie auch die HighSpeed Math Libraries !</p>
					<hr>
					<p align="justify">
					<a href="http://www.intercom.it/~amigaws" target="new">titlebar.image</a><br>
					&copy; Copyright ?-2002 Massimo Tantignone, All Rights Reserved.<br>
					Probieren Sie VisualPrefs !</p>
					<hr>
					<p align="justify">
					<a href="http://www.shredzone.de" target="new">identify.library</a><br>
					&copy; Copyright 1996-2001 Richard Körber, All Rights Reserved.<br>
					Thanks to Dan Jedlicka for the example code.</p>
					<hr>
					<p align="justify">
					ShowTip<br>
					&copy; Copyright 2002 Dan Jedlicka, All Rights Reserved.<br></p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
