divert(-1)
define(`__TITLE',`Distortion')
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
	__SECTION_HEADER(Distortion)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Creates distortion and fuzz effects.
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Distortion Shape (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">this shape determines the kind and the amount of distortion
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Map</nobr></td><td align="left">the shape can be mapped in various ways :
<ul>
<li>full range : as it is [-max to max]</li>
<li>mirrored : copied and rotated around the origin [-max to 0]=-[0 to max], which yields same shapes for positive and negative sample-values</li>
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Wrap</nobr></td><td align="left"></ul>
Choose how to handle clipping. The modi below are available :
<ul>
<li>NoClip: don't care</li>
<li>Clip : overdriven values are clipped</li>
<li>Wrap1 : overdriven values are pushed into the opposite side until they don't clip anymore.</li>
<li>Wrap2 : overdriven values are overturned (folded) until they don't clip anymore.</li>
</ul>
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
The shape acts as a kind of lookup table. If the shape would be a straight
line (from bottom left to top right), nothing would change in the sound.
But the more different the shape looks like, the more distorted the sound will be.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
