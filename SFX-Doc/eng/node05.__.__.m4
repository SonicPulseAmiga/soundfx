divert(-1)
define(`__TITLE',`Workshop')
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
					<p align="justify">In the next chapter I introduce you to __SFX with the help of examples.
					You can find most of the final samples inside the drawer "Workshop" in the program drawer.
					</p>
					<p align="justify">At first a few general remarks :<ul>
						<li>!!!! TRY AND PLAY AROUND !!!! - you can not destroy anything</li>
						<li>intensive use of __SFX is the best way to understand how it works</li>
						<li>do not use only default-settings of the operators</li>
						<li>use the modulation-features - some effects are only interesting, if you modulate something e.g. Detune, Smear<br></li>
						<li>if you have some questions/problems - write to <a href="node00.05.__.html">me</a> !!!
							- only so I can understand where descriptions are not sufficient, where weaknesses are.
						</li>
					</ul>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="4" cellspacing="0" width="100%"><tr><td><table border="0" cellpadding="0" width="100%">
					<tr><td>   5.1  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.01.__.html">__NODETITLE(`node05.01.__.html')</a></td></tr>
					<tr><td>   5.2  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.02.__.html">__NODETITLE(`node05.02.__.html')</a></td></tr>
					<tr><td>   5.3  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.03.__.html">__NODETITLE(`node05.03.__.html')</a></td></tr>
					<tr><td>   5.4  </td><td><img src="../_img/space.gif" height="0" width="20"><a href="node05.04.__.html">__NODETITLE(`node05.04.__.html')</a></td></tr>
				</table></td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
