divert(-1)
define(`__TITLE',`Operatoren : Delay')
define(`__NODE',__file__)
define(`__PATH',`')
include(`global.m4')
include(`lang_ger.m4')
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
						<b>F:</b> Was genau ist der Unterschied zwischen dem lokalen und dem globalen Feedback
								  im Operator <a href="node02.01.06/MultiDelay.html">MultiDelay</a>.<br>
						<b>A:</b> Lassen sie mich versuchen das zu illustrieren :
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
								  Lokales Feedback ist also ein Faktor, welcher bestimmt wie stark der Ausgang
								  eines Delays in seinen eigenen Eingang zurückgeführt wird. Globales Feedback
								  dagegen bestimmt wieviel vom Ausgang eines Delays zu einer Summe hinzugefügt
								  wird, welche in alle Delays zurückgeführt wird.<br>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
