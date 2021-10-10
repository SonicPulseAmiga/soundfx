divert(-1)
define(`__TITLE',`Module')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
include(`header.m4')
include(`footer.m4')
include(`navbar.m4')
divert`'dnl
<!-- NEW PAGE -->
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
					<p align="justify">__SFX ist stark modularisiert. So sind z.B. alle Effekte als
					extra Module (Plug-Ins) ausgelagert. Sie werden erst nachgeladen, wenn sie auch wirklich
					ben&ouml;tigt werden.
					</p>
					<p align="justify">Normalerweise erkennt __SFX selbständig, wenn neue Plug-Ins hinzugekommen
					oder entfernt worden sind. Falls dies doch einmal fehlschlägt (z.B. weil die Uhr ihres
					Rechner falsch l&auml;uft/lief), dann können sie eine Aktualisierung erzwingen in dem sie im
					Unterverzeichnis "data" alle Dateien mit der Endung ".db" l&ouml;schen. In einer Shell
					wechseln sie dazu in das Verzeichnis in welchem sie __SFX installiert haben und verwenden
					folgenden Befehl : "delete data/#?.db".
					</p>
					<p align="justify">Nahezu alle Module haben eigenen Einstellungen. Diese sind mit dem
					jeweiligen Modul beschrieben. Alle diese Fenster haben die gleichen
					<a href="node01.02.02.html">Men&uuml;punkte</a>.<br>
					Die Standarteinstellung der Module können sie anpassen, indem sie ihre Einstellungen als
					"default.cfg" abspeichern.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border="0" cellpadding="0">
					<tr><td>   2.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node02.01.__.html">__NODETITLE(`node02.01.__.html')</a></td></tr>
					<tr><td>   2.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node02.02.__.html">__NODETITLE(`node02.02.__.html')</a></td></tr>
					<tr><td>   2.3  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node02.03.__.html">__NODETITLE(`node02.03.__.html')</a></td></tr>
					<tr><td>   2.4  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node02.04.__.html">__NODETITLE(`node02.04.__.html')</a></td></tr>
					<tr><td>   2.5  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node02.05.__.html">__NODETITLE(`node02.05.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
