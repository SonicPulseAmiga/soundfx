divert(-1)
define(`__TITLE',`Copyright')
define(`__NODE',__file__)
include(`global.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<html>
	__HEADER
	<body __PAGE_STYLE>
		__NAVBAR
		<!-- Contents -->
		<a name="#001">
		<table border="1" width="100%" cellpadding="2" cellspacing="0">
			<tr bgcolor="#3355AA"><td>
				<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
					<td align="left">
						<b>__CHAPTER`'__TITLE</b>
					</td>
					<td align="right">
						<a href="#000"><img src="../_img/a_up.png" width="13" height="13" border="0"></a><img src="../_img/space.gif" width="2">
						<a href="#002"><img src="../_img/a_do.png" width="13" height="13" border="0"></a>
					</td>
				</tr></table>
			</td></tr>
			<tr bgcolor="#001177"><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">
						Programm:<ul>
							<li>SoundFX</li>
							<li>sfxsupport.library</li>
							<li>_loaders/*.{sfxmod|modinfo}</li>
							<li>_operators/*.{sfxmod|modinfo}</li>
							<li>_players/*.{sfxmod|modinfo}</li>
							<li>_rexx/*.{sfxmod|modinfo}</li>
							<li>_savers/*.{sfxmod|modinfo}</li>
						</ul>
						Dokumentation:<ul>
							<li>guides/*.(guide|html)</li>
							<li>_loaders/*.(guide|html)</li>
							<li>_operators/*.(guide|html)</li>
							<li>_players/*.(guide|html)</li>
							<li>_rexx/*.(guide|html)</li>
							<li>_savers/*.(guide|html)</li>
						</ul>
						Daten:<ul>
							<li>envelopes/*.*</li>
							<li>workshop/*.*</li>
						</ul>
						Installscripts:<ul>
							<li>Install-SFX*</li>
						</ul>
						Keyfile:<ul>
							 <li>keyfiles/sfx.key</li>
							 <li>sfx.key</li>
							 <li>devs:keyfiles/sfx.key</li>
						</ul>
						&copy; Copyright 1993-2000 Stefan Kost. All Rights Reserved.
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
					Ich rate auf keinen Fall eine gecrackte Version zu benutzen, da diese warscheinlich
					sehr oft abstürzen wird und möglicherweise Ihre Festplatte beschädigt !<br>
					Wenn Sie SoundFX tatsächlich für zu teuer halten, sagen sie mir lieber Bescheid.
					</p>
					<hr>
					<p align="justify">
					amigaguide.library<br>
					&copy; Copyright 1992 Commodore-Amiga, Inc.  All Rights Reserved.<br>
					Reproduced and distributed under license from Commodore.<br><br>
					Amigaguide software is provided "as-is" and subject to change. No warranties
					are made. All use is at your own risk. No liability or responsibility is assumed.
					</p>
				</td></tr></table>
			</td></tr>
		</table>
		<br>
		<a name="#002">
		<table border="1" width="100%" cellpadding="2" cellspacing="0">
			<tr bgcolor="#3355AA"><td>
				<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
					<td align="left">
						<b>Inhalt</b>
					</td>
					<td align="right">
						<a href="#001"><img src="../_img/a_up.png" width="13" height="13" border="0"></a><img src="../_img/space.gif" width="2">
						<a href="#003"><img src="../_img/a_do.png" width="13" height="13" border="0"></a>
					</td>
				</tr></table>
			</td></tr>
			<tr bgcolor="#001177"><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td><b>0</b></td><td><img src="../_img/space.gif" width="10"><a href="node00.  .  .html"><b>Einf&uuml;hrung</b></a></td></tr>
					<tr><td>   0.1  </td><td><img src="../_img/space.gif" width="20"><a href="node00.01.  .html">Was ist SoundFX ?</a></td></tr>
					<tr><td>   0.2  </td><td><img src="../_img/space.gif" width="20"><a href="node00.02.  .html">Wo l&auml;uft SoundFX ?</a></td></tr>
					<tr><td>   0.3  </td><td><img src="../_img/space.gif" width="20"><a href="node00.03.  .html">Copyright</a></td></tr>
					<tr><td>   0.4  </td><td><img src="../_img/space.gif" width="20"><a href="node00.04.  .html">Registration / Autor</a></td></tr>
					<tr><td><b>1</b></td><td><img src="../_img/space.gif" width="10"><a href="node01.  .  .html"><b>Bedienung</b></a></td></tr>
					<tr><td>   1.1  </td><td><img src="../_img/space.gif" width="20"><a href="node01.01.  .html">Allgemeines</a></td></tr>
					<tr><td>   1.2  </td><td><img src="../_img/space.gif" width="20"><a href="node01.02.  .html">Men&uuml;</a></td></tr>
					<tr><td>   1.3  </td><td><img src="../_img/space.gif" width="20"><a href="node01.03.  .html">Toolbars</a></td></tr>
					<tr><td>   1.3.1</td><td><img src="../_img/space.gif" width="30"><a href="node01.03.01.html">Loader</a></td></tr>
					<tr><td>   1.4  </td><td><img src="../_img/space.gif" width="20"><a href="node01.04.  .html">Statusbar</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		</table>
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
