divert(-1)
define(`__TITLE',`Delay')
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
	__SECTION_HEADER(Delay)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Generates Delays, Echos, Flanger and much more
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Ampf</nobr></td><td align="left">final amplification
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Dry</nobr></td><td align="left">determines how the propotion of the dry signal from the effect-parameter is calculated
</td></tr>
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Interpolation (<a href="../node03.02.02.html">I1</a>)</nobr></td><td align="left">how to calculate (smooth) inbetween data
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
Short delay values (about 10 ms) are known to put a metallic character to
the sample.<br>
When a sampled sound ends too abruptley, you can let it fade out with a
long delay effect. For this control the feedback by e.g. vector-envelope,
which raises the feedback towards the end.
In __SFX's Delay you can even modulate the delaytime and you can enter
the delaytime as notes. I know it sounds strange, but it makes sense.
If you choose a high feedbackpropotion (> 90 %) and set effektpropotion
to 100 %, the sample will resonate on the frequency which corresponds to
the delaytime. If you enter a 'C-3', __SFX will calculate the right
delaytime so that it resonates on that note.<br>
And there is another useful application of this operator. If you have a
sample containing hum and you know it's frequency,	then choose Dry='Dry=-Eff',
Eff=-100 %, Fb=97 % and Delay=<frq>. This will wipeout the frequency and all
it's high harmonics. Unfortunately it may take some cycles before the
humming fades away. Therefore try to have a bit humming in the begin,
which you can just cut later.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
