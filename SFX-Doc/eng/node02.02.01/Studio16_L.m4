divert(-1)
define(`__TITLE',`Studio16_L')
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
	__SECTION_HEADER(Studio16_L)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Loads Studio16 samples.
<table border="0" cellspacing="1" cellpadding="2" width="100%>
<tr bgcolor=__COL_TBL_ROW1><td>Loops      </td><td>no</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td>Channels   </td><td>yes (mono/stereo/[quadro])</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td>Compression</td><td>no (PCM-16)</td></tr>
</table>
Those samples are used with the Studio16 Software, which is bundled with
soundcards of the company Surrize.<br>
Many thanks to Kenneth "Kenny" Nilsen for his work and help.
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
This format does not support multi-channel-samples (stereo or quadro).
__SFX offers a workaround for it. Just save the separate channels in studio16 as
name_l.ext and name_r.ext for stereo (where name is the filename and ext is the extension)
and name_l.ext, name_r.ext, name_f.ext and name_b.ext for quadro.
Then load one of them into __SFX. This loader will then look for the other channels
and load them as well.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
