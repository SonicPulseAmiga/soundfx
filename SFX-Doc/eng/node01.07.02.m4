divert(-1)
define(`__TITLE',`Cycle')
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
				<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
					<center>
						__IMAGE(`mod_cycle')
					</center>
					<br>
					<p align="justify">This modulator generates an oscillation. You can choose it's waveform, phase and
					frequency. The latter could be adjusted in different ways :
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>variation   					</th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">hz				</td><td>frequency in hz : 1.5 hz</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td valign="top">time			</td><td>duration of one period in time units or samples : 5 ms</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td valign="top">repeats			</td><td>number of periods (cycles) : 4 rpts</td></tr>
					</table>
					</p>
					<p align="justify">The waveforms Rnd and SRnd produce random peaks, where for SRnd the peaks will be
					smoothed as well. The parameter frequency determines the number of random values per second
					(or how long one random value will be hold) and the parameter phase is not used for these both waveforms.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
