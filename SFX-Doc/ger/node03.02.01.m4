divert(-1)
define(`__TITLE',`Modulator')
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
					Folgende Parameter eines <a href="node02.01.02.html">Modulators</a> lassen sich per ARexx �ndern.
					Der ben�tigte &lt;Pr�fix&gt; (z.B. P1) ist in den Beschreibungen der Operatoren enthalten.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Parameter             </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;S       </td><td>Startwert (Modulation gibt 0.0 zur�ck)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;E       </td><td>Endwert (Modulation gibt 1.0 zur�ck)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;ModShape</td><td>Art der Modulation ("None","Curve","Cycle","Vector","User")</td></tr>
					</table>
					Je nach Art der Modulation ist die Angabe weiterer Parameter m�glich.<br>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>Parameter               </th><th>Beschreibung</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;CurveExp  </td><td>Kr�mmung (0.0...1.0...unendlich)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;CycleOszi </td><td>"Saw","Sin","Sqr","Tri"</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;CycleMode </td><td>"Hz","Time","Repeats"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;CycleFrq  </td><td>Frequenz</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;CyclePhase</td><td>Startphasenwinkel</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;VectorAnz </td><td>Anzahl der Punkte</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;VectorPos </td><td>ix 0..(anz-1), pos 0.0...1.0/td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;VectorLev </td><td>ix 0..(anz-1), lev 0.0...1.0</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;UserType  </td><td>"Normal","Abs","AmpEnv","FrqEnv"</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&lt;Pr�fix&gt;UserMode  </td><td>"Single","Repeat","Stretch"</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&lt;Pr�fix&gt;UserModBuf</td><td>ID des Modulationssamples</td></tr>
					</table>
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
