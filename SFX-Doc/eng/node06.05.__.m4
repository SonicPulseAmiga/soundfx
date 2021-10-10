divert(-1)
define(`__TITLE',`Support')
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
					<p align="justify">Please read the chapter <a href="node06.04.__.html">FAQ</a>
					before writing to me. Perhaps there is already a solution to you problem.<br>
					If you write to __AUTHOR, please include the following details:<ul>
						<li>your computer configuration, preferably use "sys:Tools/ShowConfig"</li>
						<li>which sfx-version and what sfx-cpu-version you use, "version FILE=SoundFX FULL"</li>
						<li>where did you detect the error. The more accurate it is, the  easier it is
							for me to fix it. Please do not send inquiries of the kind "... does not work". Describe
							as detailed as possible, what you did and what you want to achieve. Every tiny
							detail may matter.<br>
							You may send samples along with the report (if they may be the cause), but please
							try to restrict them to a maximum length of 0.5 Mb. If a certain operator causes
							problems, just send me a preset with the respective settings.</li>
					</ul></p>
					<p align="justify">As mentioned, I try to make the program as good as possible and you could help me.
					I try to answer every email, but not all letters (I read ALL letter and try to fix
					the reported bugs).
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(`BugTracker')
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">If you have an internet connection, I recommend to check the bug-tracker at
					<a href="http://www.sonicpulse.de/phpbt/" target="new">www.sonicpulse.de/phpbt/</a> and to
					enter your bug-reports, feature-request or help-request there.
					This eases collecting information about the case and helps we to keep the overview.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(`Tools for bug reports')
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<p align="justify">
						Finally I may make sense to use tools like Snoopdos, Enforcer, Mungwall, ... and
						to include their output in the mail.
					</p>
					<p align="justify">
						Snoopdos is the right tool to find out if files are missing
						(incomplete installation, harddisk-erros).
					</p>
					<p align="justify">
						If the program behaves badly or even crashes, a toolchain out of<ul>
							<li>SegTracker + MuForce + MuGuardianAngel</li>
							<li>SegTracker + WipeOut + CyberGuard</li>
							<li>SegTracker + Mungwall + Enforcer</li>
						</ul>
						can help to locate the source of the problem.<br>
						SegTracker needs to be started early in the startup-sequence. It needs no parameters.
						To check for errors I start MuForce and MuGuardianAngel as below
<pre>
run &gt;NIL: sashimi CONSOLE WINDOW="CON:0/14/800/200/Sashimi [Ctrl]+E=Empty [Ctrl]+F=File [Ctrl]+D=Reset console/CLOSE/WAIT/INACTIVE/NOFNC/JUMP/FASTUPDATE" ON NOPROMPT TIMERON
run &gt;nil: &lt;nil: MuForce AREGCHECK DREGCHECK STACKCHECK CAPTURESUPER LED 0 RAWIO DISABLEBELL ON
run &gt;nil: &lt;nil: MuGuardianAngel AREGCHECK DREGCHECK STACKCHECK SHOWHUNK LED 0 DISABLEBELL WAITFORMUFORCE
</pre><br>
						The first line starts Sashimi - a tool that captures the logoutput of the next two tools and
						displays this in a shell-window. If you use a shell-extension like KingCON you can later
						save the output and send that to me.<br>
						It is important that SegTracker run. You see this that when problems are reported there
						are lines in the report with HUNK and OFFSET values. This is the data I need to find the
						place of the error in the source-code.
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
