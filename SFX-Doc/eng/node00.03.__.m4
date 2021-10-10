divert(-1)
define(`__TITLE',`Copyright')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
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
					No warranties will be given for full functionality of the software. Furthermore
					I accept no liability for damage because of misuse.
					If you found any error in the program, then please contact me with a description
					of it. I will try to fix it soon as possible.
					</p>
					<p align="justify">
					The program package, except of the key-file, is freely distributable. It`s even
					desired to spread it, as long as the fees are not more than 5.- DM or $3.-.
					If you want to distribute the program as part of a compilation or series,
					please contact me and ask for permission.
					</p>
					<p align="justify">
					This demo-package may be relased on following disk-series or CDs without previous request.:<ul>
						<li>Aminet CD</li>
						<li>Fred Fish CD</li>
						<li>Saar PD-Serie</li>
						<li>Time PD-Serie</li>
						<li>Amiga-Magazin PD/CD</li>
						<li>AmigaPlus CD's</li>
					</ul>
					</p>
					<p align="justify">
					I strongly recomend you not to use a cracked version, because it might crash
					very often and possibly damage your hardisk !<br>
					If you realy think __SFX is too expensive for you, then better tell me.
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
					Try out his HighSpeed Math Libraries !</p>
					<hr>
					<p align="justify">
					<a href="http://www.intercom.it/~amigaws" target="new">titlebar.image</a><br>
					&copy; Copyright ?-2002 Massimo Tantignone, All Rights Reserved.<br>
					Have a look at VisualPrefs !</p>
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
