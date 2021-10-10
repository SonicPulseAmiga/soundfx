divert(-1)
define(`__TITLE',`Modules')
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
					<p align="justify">__SFX is highly modularized. That means that e.g. all effects
					are separate modules (plug-ins), which will be loaded only if you are going to use them.
					</p>
					<p align="justify">Usually __SFX detects automaticall, that new plug-ins have been
					installed or removed. If this should fail somehow (e.g. because the clock of you computer
					is/was wrong), you can force an update by deleting all files ending on ".db" in the
					subdirectory "data". In a shell you would change into the directory where __SFX is
					installed and use the following command : "delete data/#?.db".
					</p>
					<p align="justify">Nearly every modul has its own settings. These are described along with
					the module. All these window share the same <a href="node01.02.02.html">menuitems</a>.<br>
					You can adjust the standard-settings of each modul by saving your settings as
					"default.cfg".
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
