divert(-1)
define(`__TITLE',`Operators : Delay')
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
						<b>Q:</b> In the <a href="node02.01.06/MultiDelay.html">MultiDelay</a> operator, what exactly is the difference between local and global
								  feedback.<br>
						<b>A:</b> Let me try an illustration :
<pre>
           +--------------------------+
           |                          |
           |              * fbglob +--+
           v          +---------+  |  |
source >---+---+---+--| Delay 1 |--+------+-----> result
               |   ^  +---------+  |  |   |
               |   +---------------+  |   |
               |      * fblocal       |   |
               |                      |   |
               |          * fbglob +--+   |
               |      +---------+  |  |   |
               +---+--| Delay 2 |--+------+
               |   ^  +---------+  |  |   |
               |   +---------------+  |   |
               |      * fblocal       |   |
               :                      :   :
               |          * fbglob +--+   |
               |      +---------+  |      |
               +---+--| Delay 8 |--+------+
                   ^  +---------+  |    
                   +---------------+    
                      * fblocal         
</pre>
								  So local feedback is a factor which determines how much of one delays
								  output is feed back to its own input. Global feedback is the factor
								  which determines how much of one delays output is added to a sum
								  which is feed back into all delay-inputs.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
