divert(-1)
define(`__TITLE',`Die ARexx Schnittstelle')
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
					<p align="justify">Der ARexx-Port von __SFX heißt "REXX_SFX". Über
					diesen können sie den Funktionsumfang von __SFX erheblich erweitern.
					So ist es z.B. möglich __SFX fernzusteuern oder Sample mit anderen Programmen auszutauschen.
					Sie können sogar eigenen Effekte in ARexx programmieren.<br>
					<b>Wichtig:</b> seit Version 3.70 besitzen alle Befehle, den Prefix "SFX_" um
					Namenskollisieonen zu vermeiden.
					</p>
					<p align="justify">
					Schauen Sie sich die Skripte, die mit Ihrer __SFXInstallation mitgeliefert werden an.
					Dise finden sie im &quot;_rexx&quot; Verzeichnis.<br>
					Wenn sie eigenen Skripte schreiben, empfehle ich Ihnen diese mit folgendem Code zu beginnen,
					damit der Arexx-Port korrekt gesetzt wird:
<pre>
IF (LEFT(ADDRESS(), 8) ~= "SFX_REXX") THEN DO	/* not started by SoundFX ? */
	PARSE ARG opts
	sfxport=WORD(opts,1)
	IF SHOW("Ports",sfxport) THEN DO
		ADDRESS VALUE sfxport
	END
	ELSE DO
		IF ~SHOW("Ports","SFX_REXX") THEN EXIT 10
    	ADDRESS 'SFX_REXX'
	END
END
</pre>
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
					<tr><td>   3.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node03.01.__.html">__NODETITLE(`node03.01.__.html')</a></td></tr>
					<tr><td>   3.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node03.02.__.html">__NODETITLE(`node03.02.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
