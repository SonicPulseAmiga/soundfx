divert(-1)
define(`__TITLE',`DelayPlus')
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
	__SECTION_HEADER(DelayPlus)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Generates Delays, Echos, Flanger plus some really wiered fx and much more
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Feedback (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">how much of the result is feeback into the operator. This may be negative producing an inverted feedback.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Delay (<a href="../node03.02.01.html">P3</a>)</nobr></td><td align="left">modulatable delaytime.
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Cut-Off (<a href="../node03.02.01.html">P4</a>)</nobr></td><td align="left">The filter cut-off is the frequency where the filter becomes active.
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Resonance (<a href="../node03.02.01.html">P5</a>)</nobr></td><td align="left">Resonance attenuates the sound around the cut-off frequency. A value of
1.0 means no attenuation and higher values will lead to stronger attentuations.
If you turn up this too far, that the filter will begin to oszillate (quwiek).
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">final amplification
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">what kind of <a href="../node06.03.00.Filter.html">Filter</a> do you want it to be
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Dry</nobr></td><td align="left">determines how the propotion of the dry signal from the effect-parameter is calculated
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">how to calculate (smooth) inbetween data
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
see <a href="Delay.html">Delay</a> and <a href="Filter-StateVariable.html">Filter-StateVariable</a>
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
