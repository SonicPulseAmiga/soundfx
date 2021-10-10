divert(-1)
define(`__TITLE',`Modulator')
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
					<p align="justify">
					These parameters of a <a href="node02.01.02.html">modulator</a> can be changed with ARexx.
					You can find the required &lt;prefix&gt; (e.g. P1) inside the documentation of the operators.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>parameter             </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;S       </td><td>starting value (modulation returns 0.0)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;E       </td><td>ending value (modulation returns 1.0)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;ModShape</td><td>kind of modulation ("None","Curve","Cycle","Vector","User")</td></tr>
					</table>
					Depending on the kind of modulation ,more parameters are accessible.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>parameter               </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;CurveExp  </td><td>bend (0.0...1.0...infinite)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;CycleOszi </td><td>"Saw","Sin","Sqr","Tri"</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;CycleMode </td><td>"Hz","Time","Repeats"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;CycleFrq  </td><td>frequency</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;CyclePhase</td><td>starting phase (angle)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;VectorAnz </td><td>number of points</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;VectorPos </td><td>ix 0..(number-1), pos 0.0...1.0/td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;VectorLev </td><td>ix 0..(number-1), lev 0.0...1.0</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;UserType  </td><td>"Normal","Abs","AmpEnv","FrqEnv"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;prefix&gt;UserMode  </td><td>"Single","Repeat","Stretch"</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;prefix&gt;UserModBuf</td><td>id of modulation buffer</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
