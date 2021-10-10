divert(-1)
define(`__TITLE',`Dynamic')
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
	__SECTION_HEADER(Dynamic)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Amplifies or deamplifies the volume of a sample depending on its amplitude.
Provides complex changes of the <a href="../node06.03.00.Dynamic.html">dynamics</a> of the sample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Effect (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">how much the operator effects the outcome
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ratio loud (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">amplitude-change of loud signals
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Ratio quiet (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">amplitude-change of quiet signals
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Threshold (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">determines the break point between the quiet and loud ratio - whenever the signals
amplitude exceeds the threshold the loud ratio will be applied otherwise the quiet
ratio will be used.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Knee</nobr></td><td align="left">the are two variant, one is edgy and the other fades smoothly
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Characteristics</nobr></td><td align="left">These graphs show the effect of the settings. Read it a a translation table - the volume
of the source sample denotes the x postion, then the curve can be used to find the
respective y postion which denotes the output volume. 
</td></tr>
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
Here are a few examples:<ul>
<li>Compressor: squeezes the sample together:)<br>Ratio loud &lt;100 %, Ratio quiet &gt;100 %</li>
<li>Expander: expands the sample<br>Ratio loud &gt;100, Ratio quiet &lt;100 %</li>
<li>Limiter: amplifies the quiet parts of the sample<br>Ratio loud =100, Ratio quiet &gt;100 %</li>
<li>Delimiter: amplifies the loud parts of the sample<br>Ratio loud &gt;100, Ratio quiet =100 %</li>
</ul>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
