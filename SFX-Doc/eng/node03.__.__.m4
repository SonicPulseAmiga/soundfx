divert(-1)
define(`__TITLE',`The ARexx Interface')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_eng.m4')
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
					<p align="justify">The ARexx-port of __SFX is called "REXX_SFX".
					Through this you can "remote-control" __SFX through ARexx-Scripts.
					This way you can use __SFX for processing samples for other programs FROM that
					other program (e.g. a Music program). You can even write own effects.<br>
					<b>Important:</b> from version 3.70 all commands are prefixed by "SFX_" to avoid
					command-collisions.
					</p>
					<p align="justify">
					Have a look at the scripts that come with your __SFX installation.
					They are installed in the directory named &quot;_rexx&quot;.<br>
					When writing own scripts, I recommend that you start them with code like this,
					to set the proper arexx-port:
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
