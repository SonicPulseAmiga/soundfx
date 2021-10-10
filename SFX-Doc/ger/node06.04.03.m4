divert(-1)
define(`__TITLE',`Fehler')
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
						<b>F:</b> Wenn Ich versuche ein 10Mb gro&szlig;es Sample zu laden, bekomme ich manchmal einen "Out of memory"-Fehler, obwohl ich noch 13 Mb frei habe.<br>
						<b>A:</b> Sie brauchen die 10 Mb als ein Block. Geben sie mal "avail" in einem shell-Fenster ein. Dort sehen sie dann den grö&szlig;ten freien Block.<br><br>

						<b>F:</b> Ich habe das 10 Mb Sample nun geladen und noch 4 Mb frei. Jetzt versuche ich einen Bereich auszuschneiden (z.B. 512 kb) und bekomme einen "Out of memory"-Fehler.<br>
						<b>A:</b> Wenn sie einen Schnitt machen mu&szlig; __SFX die Sampledaten, die sie behalten möchten, umkopieren.<br><br>

						<b>F:</b> Wenn ich __SFX unter OS3.5 starte bekomme ich den folgenden Fehler "Can&#39;t open amigaguide.library &gt;=V34 !".<br>
						<b>A:</b> Bitte überprüfen Sie ihre OS3.5 Installation. Es scheint, da&szlig; die Datatypes manchmal nach "libs:datatypes"
								  und nicht nach "sys:classes/datatypes" installiert werden.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
