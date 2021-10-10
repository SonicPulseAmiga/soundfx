divert(-1)
define(`__TITLE',`Main menu')
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
					<p align="justify">This menu is available, when no other dialog window is active.</p>
					<table border="0" cellspacing="1" cellpadding="2" width="100%">
						<tr bgcolor=__COL_TBL_HEAD><th>main menu     </th><th>sub menu              </th><th>description</th></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Project       </td><td>New                   </td><td>opens a dialog for generating an empty sample</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Load                  </td><td>load samples with the current loader (see <a href="node01.03.01.html">choosing a loader</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Save                  </td><td>save a sample with the current saver (see <a href="node01.03.02.html">choosing a saver</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Close                 </td><td>removes selected samples after prompting the user</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Execute               </td><td>start the current operator (see <a href="node01.03.03.html">choosing an operator</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Execute Rexx          </td><td>start the current rexx-script (see <a href="node01.03.04.html">choosing a rexx-script</a>)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Play All              </td><td>play the whole sample</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Play Range            </td><td>play the selected area</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Stop                  </td><td>stop playing</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Record                </td><td>opens the <a href="node01.05.09.html">recording window</a> (requires AHI)</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Batch Processor       </td><td>opens the <a href="node01.05.10.html">batch processor window</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Info                  </td><td>opens the <a href="node01.05.02.html">information window</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>MRU (5x)              </td><td>the 5 samples you have loaded last, can be reloaded herewith<br>
																											these entries are stored in the file &quot;data/MRU.cfg&quot;.
																										 </td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Quit                  </td><td>end the program after prompting the user</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Edit          </td><td>...                   </td><td>similar to the <a href="node01.03.07.html">edit toolbar</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Range         </td><td>...                   </td><td>for setting, adapting and reseting ranges</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Zoom          </td><td>...                   </td><td>similar to the <a href="node01.03.08.html">zoom toolbar</a></td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>CleanUp       </td><td>Current               </td><td>reorder the active sample-window</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>All                   </td><td>reorder all sample windows</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>All normal            </td><td>reorder all sample windows and resize them to standard size</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>All zoomed            </td><td>reorder all sample windows and resize them to small size</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Utilities     </td><td>Swap byte order       </td><td>repair files saved with wrong byte order</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Swap sign             </td><td>repair files saved with wrong sign</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Interleave channels   </td><td>repair files saved with wrong channel format</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>De-interleave channels</td><td>repair files saved with wrong channel format</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>Prefs         </td><td>GUI                   </td><td>preferences for the GUI</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Sample                </td><td>preferences for the sample window</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Virtual memory        </td><td>preferences for virtual memory</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Miscellaneous         </td><td>miscellaneous preferences</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Use                   </td><td>remembers the current settings as long as the computer is switched on</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Save                  </td><td>save the current settings permanently</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Load last used        </td><td>load the last used settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>&nbsp;        </td><td>Load last saved       </td><td>load the last saved settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW2><td>&nbsp;        </td><td>Reset to defaults     </td><td>set default settings</td></tr>
						<tr bgcolor=__COL_TBL_ROW1><td>Help          </td><td>...                   </td><td>invoke the online-help with the chosen topic, go to the support
																									 page at the internet, write an e-mail to the author or display
																									 version info of the software.</td></tr>
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
