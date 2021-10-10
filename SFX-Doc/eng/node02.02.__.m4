divert(-1)
define(`__TITLE',`Loader')
define(`__NODE',__file__)
define(`__PATH',`')
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
			__SECTION_HEADER(__CHAPTER`'__TITLE)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">A loader is a module, which loads samples in a certain format. __SFX
					offers loader for the most common formats.
					</p>
					<p align="justify">If you have a sample which can not be loaded, then there are two main reasons for this :<br>
					1. I have coded the loader badly<br>
					2. I don't know about the format or don't support it yet<br>
					In the first case, please mail the offending sample to <a href="node00.05.__.html">me</a>.
					In the second case do so as well, but try to send as much information about the format with it.
					So if you can dig out some documentation about the format in the unexplored depths of the world wide web,
					chances are rising phenomenally that this format can be loaded in one of the next __SFX version.
					If the formats supports various variants (compression, different bit-depths, etc.) don't hesitate to send me
					a rich set of test files.
					</p>
					<p align="justify">Nearly all savers have a few things in common, which I will describe below.
					After loading all loaders generate a file-comment with information like format, channels and length,
					if the disk is not write-protected. If there is already a filecomment, if will not be overwritten.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0"><tr><td><table border=0 cellpadding="0">
					<tr><td>   2.2.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.02.01.html">__NODETITLE(`node02.02.01.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
