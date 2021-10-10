divert(-1)
define(`__TITLE',`Operators')
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
					<p align="justify">An operator is a module which processes or generates samples. There
					are 3 different kinds of operators :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>variation      			</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">effects		</td><td>process one or many source samples to one or many results.</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">generators	</td><td>generate new sounds (synthesizer), do not rely on a source sample.</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">analyzers	</td><td>analyze samples (you would never have guessed that ;-)), do not generate any new samples</td></tr>
					</table>
					</p>
					<p align="justify">Most operators are built in a similar fashion. I'd therefore like to
					explain some things you'll encounter in most operators here and leave it out in the pages about
					the operators in special.
					</p>
					<p align="justify">All parameters you change are held in memory as long as the computer runs, so
					when you want to use the operator (effect) again (even if you left the program
					inbetween) you'll get the parameters as you left them. Should the buffer you
					have used be closed, __SFX changes these settings as those buffers are not
					existing anymore.
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
					<tr><td>   2.1.1</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.01.html">__NODETITLE(`node02.01.01.html')</a></td></tr>
					<tr><td>   2.1.2</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.02.html">__NODETITLE(`node02.01.02.html')</a></td></tr>
					<tr><td>   2.1.3</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.03.html">__NODETITLE(`node02.01.03.html')</a></td></tr>
					<tr><td>   2.1.4</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.04.html">__NODETITLE(`node02.01.04.html')</a></td></tr>
					<tr><td>   2.1.5</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.05.html">__NODETITLE(`node02.01.05.html')</a></td></tr>
					<tr><td>   2.1.6</td><td><img src="../_img/space.gif" height="0" width="30"><a href="node02.01.06.html">__NODETITLE(`node02.01.06.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
