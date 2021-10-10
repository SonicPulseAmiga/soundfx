divert(-1)
define(`__TITLE',`Kanäle')
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
					Ein Sound kann aus mehreren Einzelsounds bestehen, die gleichzeitig
					auf verschiedenen Lautsprechern abgespielt werden um ein räumliches Höhrgefühl zu erzeugen.
					Nachfolgend beschreibe ich einige Varianten :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Name          </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Mono          </td><td>nur ein Kanal und somit keine Rauminformation.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Stereo        </td><td>zwei separate Kanäle (recht und links)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Quattro       </td><td>vier separate Kanäle<ul>
																			<li>vorne links, vorne rechts, hinten links, hinten rechts</li>
																			<li>links, rechts, vorne, hinten</li>
																		</ul></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Pseudo Quattro</td><td>besteht aus drei oder vier Kanälen<ul>
																			<li>3 : vorne links, vorne rechts, hinten mitte</li>
																			<li>4 : vorne links, vorne rechts, hinten links, hinten rechts</li>
																		</ul>
																		Dies kann über eine spezielle Verschaltung der 3 oder 4 Lautsprecher aus einem
																		Stereosignal gewonnen werden.
																		</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Surround      </td><td>besteht aus 4 oder 5 Kanälen<ul>
																		<li>4 : vorne links, vorne mitte, vorne rechts, hinten mitte</li>
																		<li>5 : vorne links, vorne mitte, vorne rechts, hinten links, hinten rechts</li>
																	</ul>
																	Die erste Variante kann über eine spezielle Verschaltung der 4 Lautsprecher aus einem
																	Stereosignal gewonnen werden. Wesentlich bessere Ergebnisse erhält man
																	jedoch mit einem Decoder.
																	</td></tr>
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
