divert(-1)
define(`__TITLE',`SDS-File_L')
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
	__SECTION_HEADER(SDS-File_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
L?d Sample Dump Standard Dateien.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>ja (vorw?rts)</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Kan?le     </td><td>nein (mono)</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Kompression</td><td>ja (PCM-8,PCM-16,PCM-24,PCM-32)</td></tr>
</table>
Dieses Format erm?glicht es Ihnen Samples mit Ihrem Sampler (Profisampler,
keine Parallelportsampler) auszutauschen. Dazu ben?tigen sie weiterhin einen
SysEx Dumper. Senden Sie das Sample vom Sampler aus per MIDI/SCSI und speichern
Sie die empfangene SysEx Datei ab (bevorzugte Endung .SDS). Diese kann nun
von __SFX geladen werden.
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
keine
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
