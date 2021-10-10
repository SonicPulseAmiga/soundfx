divert(-1)
define(`__TITLE',`Studio16_S')
define(`__NODE',__file__)
define(`__PATH',`../')
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
	__SECTION_HEADER(Studio16_S)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Speichert Studio16 Samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>nein</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan�le     </td><td>ja (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>nein (PCM-16)</td></tr>
</table>
Solche Samples werden von der Studio16 Software benutzt, welche den Soundkarten
der Firma Sunrize beiliegt.<br>
Vielen Dank an Kenneth "Kenny" Nilsen f�r seine Arbeit und Hilfe.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>keine</nobr></td><td align="left">&nbsp;</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Hinweise)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Dieses Format unterst�tzt keine Mehrkanal-Samples  (Stereo oder Quadro).
__SFX bietet daf�r eine L�sung. Stereo-Samples werden als name_l.ext
und name_r.ext gespeichert (wobei name der Dateiname und ext die Dateierweiterung sind) and
Quadro-Samples als name_l.ext, name_r.ext, name_f.ext und name_b.ext.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
