divert(-1)
define(`__TITLE',`DataTypes_L')
define(`__NODE',__file__)
define(`__PATH',`../')
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
	__SECTION_HEADER(DataTypes_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Loads Sample-Files via AMIGA OS DataTypes. This loader would load every
sample, if you have a datatype for its format installed. You can try this,
when the Universal-Loader fails.<br>
The main disadvantag of the system shipped with OS3.x, is that it only
supports 8bit mono samples. Fortunately __SFX can use the extensions
introduced by the sounddt41 (which can be found on aminet).
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>yes (forward)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/quadro)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>yes (PCM-8,PCM-16)</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>none</nobr></td><td align="left">&nbsp;</td></tr>
</table>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Notes)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
none
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
