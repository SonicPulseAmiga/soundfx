divert(-1)
define(`__TITLE',`Logic')
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
	__SECTION_HEADER(Logic)
	<tr bgcolor=__COL_SEC_BACK><td>
		<table border="0" width="100%" cellpadding="5" cellspacing="0"><tr><td>
Does a logical operation to the sampledata whith the choosen function.
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
<tr bgcolor=__COL_TBL_ROW2><td align="right" valign="top"><nobr>Logic Operant (<a href="../node03.02.01.html">P2</a>)</nobr></td><td align="left">value what is to be used for the operation
</td></tr>
<tr bgcolor=__COL_TBL_ROW1><td align="right" valign="top"><nobr>Type</nobr></td><td align="left">What function should be used
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
For people who like to experiment. Effect parameter should be kept low.<br>
One can also use this operator to "encode" or encrypt samples. For this
you need a sample TO encrypt and a sample THAT encrypts. The later is the
key. Set the LogicOperand parameter to 32767 lv <-> -32768 lv and activate
the blendshape "User/Normal". Choose your key sample for the modulator.
The Effect parameter is set to 100 %. Choose the function "Xor". After
the operation, if you'll listen to the new data, there won't be much in
terms of listenable sound. Repeat the same operation and you'll get your
sample back the way it was.
		</td></tr></table>
	</td></tr>
__SECTION_STYLE_END
<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
