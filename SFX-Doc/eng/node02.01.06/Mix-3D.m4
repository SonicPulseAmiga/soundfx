divert(-1)
define(`__TITLE',`Mix-3D')
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
	__SECTION_HEADER(Mix-3D)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Mixes 8 samples via a path in a cube
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Sources</nobr></td><td align="left">The source-samples which then go into the mix.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>X-Axis (<a href="../node03.02.01.html">P1</a>)</nobr></td><td align="left">location of the point on the X-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Y-Axis (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">location of the point on the Y-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Z-Axis (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">location of the point on the Z-axis
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Path</nobr></td><td align="left">This area shows the path of the curve inside the cube. During the
calculation a point will wander along the curve from one end to the other.
The distance of the point to the corners defines how much of the source
assigned to that corner will gets mixed into the result.<br>
With the "View" gadget, you can choose from where to look on the cube
and with "Prec." you choose how detailed the curve will be drawn.
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
Just mix various versions of one sample together and do this twice with different
curves. Then join the results to one stereo sample.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
