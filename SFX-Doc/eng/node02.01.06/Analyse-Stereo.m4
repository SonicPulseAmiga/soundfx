divert(-1)
define(`__TITLE',`Analyse-Stereo')
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
	__SECTION_HEADER(Analyse-Stereo)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Produces a graph which shows the spatial distribution of a sample
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
__SECTION_LABEL
__SECTION_STYLE_BEG
	__SECTION_HEADER(Parameter)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
<table border="0" cellspacing="1" cellpadding="2" width="100%"><tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>none</nobr></td><td align="left">&nbsp;</td></tr>
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
This is know as well as a phase-plot.<br>
When calculations are complete a new window is opened on which the graph is drawn.
A signal where both channels are exactly the same, will appear as a line from the
middle to the top (center). When you listen to it with headphones, you will hear the
signal inside your head. The Phase of such a signal is absolut synchonous.
A complete anti-phase signal (one channel is the inverted copy of the other),
will appear as a line towards the bottom (wide). If listening to this on headphones,
the sound appears to come from outside. Such a signal is mono-incompatible,
which means, if one listens to this on a mono kitch radio he/she will hear absolutely nothing.
When analysing real-stereo files, the graph further shows with peaks towards left or right
how much "stereo" the signal is. Ideally the graph is a peaked ball around the center
with a needle towards the top.
<br>
If you want to store the generated graphs as images I recommend using a image grabber like SGrab,
which can be found on Aminet.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
