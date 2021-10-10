divert(-1)
define(`__TITLE',`Funktionen')
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
					Mittlerweile stellt __SFX ihnen mehr als 100 ARexx Funktionen zur Verfügung.
					Wenn sie Weitere benötigen, so lassen sie <a href="node00.05.__.html">mich</a> es wissen.
					Nachfolgend finden sie alle Funktionen im Überblick
					(bitte beachten sie, das alle Funktionsnamen mit &quot;SFX_&quot; beginnen):
					</p>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__SECTION_LABEL
		__SECTION_STYLE_BEG
			__SECTION_HEADER(__CONTENTS)
			<tr bgcolor=__COL_SEC_BACK><td>
				<table border="0" cellpadding="5" cellspacing="0"><tr><td>
					<table border="0" cellpadding="1" cellspacing="0">
<!--LIST_BEG-->
<tr><td>&nbsp;</td><td><b>Activate</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Brings __SFX screen to front
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>CleanUp</b></td><td align="left">Mode[0=Cur,1=All,2=AllNormal,3=AllZoomed]</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Reorder window(s) on __SFX screen
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>DisableChannel</b></td><td align="left">BufferId ChannelNo</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Deactivate a given channel
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCopy</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCopyB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region (sample-begin to region-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCopyE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region (region-begin to sample-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCut</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Cuts the selected region
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCutB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Cuts the selected region (sample-begin to region-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditCutE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Cuts the selected region (region-begin to sample-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditErase</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Erases the selected region
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditEraseB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Erases the selected region (sample-begin to region-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditEraseE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Erases the selected region (region-begin to sample-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditGrab</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region into a new buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditGrabB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region into a new buffer (sample-begin to region-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditGrabE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Copies the selected region into a new buffer (region-begin to sample-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditOverwrite</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Overwrite samples with contents of the copy-buffer starting from the begin of the selected region
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditOverwriteB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Overwrite samples with contents of the copy-buffer starting from the begin of the sample
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditPaste</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Inserts the contents of the copy-buffer at the region marker
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditPasteB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Inserts the contents of the copy-buffer at the region begin
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditPasteE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Inserts the contents of the copy-buffer at the region end
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditZero</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Silences the selected region
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditZeroB</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Silences the selected region (sample-begin to region-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EditZeroE</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Silences the selected region (region-begin to sample-end)
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>EnableChannel</b></td><td align="left">BufferId ChannelNo</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Activate a given channel
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>Exit</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Leave __SFX without asking
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BufferId=</td><td><b>GetActiveBuffer</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Return the Id of the currently active sample
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BuferName=</td><td><b>GetBufferName</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns a string containing the name of the sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">NumChannels=</td><td><b>GetChannels</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the number of channels for the given buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Length=</td><td><b>GetLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the length of the specified sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">List=</td><td><b>GetList</b></td><td align="left">ListName[Buffers,Loaders,Operators,Players,Savers]</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns a new-line delimited list of available modules in the respective category
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Value=</td><td><b>GetLoaderParam</b></td><td align="left">LoaderName ParamName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the value of the given parameter of the given loader
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">LoopEnd=</td><td><b>GetLoopEnd</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the end position of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">LoopLength=</td><td><b>GetLoopLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the length of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">LoopMode[0=Off,1=Forward]=</td><td><b>GetLoopMode</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the loop mode for the specified buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">LoopStart=</td><td><b>GetLoopStart</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the start position of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkXEnd=</td><td><b>GetMarkXEnd</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-end position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkXLength=</td><td><b>GetMarkXLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-length of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkXStart=</td><td><b>GetMarkXStart</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-start position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkYEnd=</td><td><b>GetMarkYEnd</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-end position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkYLength=</td><td><b>GetMarkYLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-length of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">MarkYStart=</td><td><b>GetMarkYStart</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-start position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Value=</td><td><b>GetOperatorParam</b></td><td align="left">OperatorName ParamName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the value of the given parameter of the given operator
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ProgDir=</td><td><b>GetProgDir</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the pathname of __SFX installation
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Mode=</td><td><b>GetQuietMode</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns wheter __SFX is in quiet mode
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">SampleRate=</td><td><b>GetRate</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the sampling rate of the specified sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>GetSample</b></td><td align="left">DstAddress</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Stores the samples of the currently active buffer into the givven memory location as PCM-8 mono data
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Value=</td><td><b>GetSampleValue</b></td><td align="left">BufferId ChannelId Position</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Retrieves one 16-bit sample value
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Value=</td><td><b>GetSaverParam</b></td><td align="left">SaverName ParamName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the value of the given parameter of the given saver
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">StorageType[1=Mem,2=Dev]=</td><td><b>GetStorageType</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the type of storage of the specified sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">UserInfo=</td><td><b>GetUserInfo</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns a text string with information of registered user
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">VersionInfo=</td><td><b>GetVersion</b></td><td align="left">ComponentName[SoundFX,...]</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns the version of the specified component in the form X.Y
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomXEnd=</td><td><b>GetZoomXEnd</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-end position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomXLength=</td><td><b>GetZoomXLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-length of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomXStart=</td><td><b>GetZoomXStart</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the x-start position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomYEnd=</td><td><b>GetZoomYEnd</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-end position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomYLength=</td><td><b>GetZoomYLength</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-length of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ZoomYStart=</td><td><b>GetZoomYStart</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Get the y-start position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>HideBuffer</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Hides a visible sample
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ProWinId=</td><td><b>InitProWin</b></td><td align="left">MaxLength Title</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Creates a new progress window
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">ChannelActive=</td><td><b>IsChannelActive</b></td><td align="left">BufferId ChannelId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Returns a alue &gt; 0 if the given channel is active
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BufferId=</td><td><b>LoadSample</b></td><td align="left">FileName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Loads the specified file with the currently selected loader
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>Message</b></td><td align="left">MessageText</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Displays the supplied text as a message box on __SFX screen
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BufferId=</td><td><b>NewBuffer</b></td><td align="left">Length SamplingRate Channels</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Prepares a new empty buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BufferId=</td><td><b>ProcessSample</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Apply the currently selected operator to the active sample
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>PutSample</b></td><td align="left">SrcAddress Length Name</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Loads PCM-8 mono samples from the given memory location into __SFX and names the new sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>PutSampleValue</b></td><td align="left">BufferId ChannelId Position Value</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Stores one 16-bit sample value
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>RedrawBuffer</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Refreshes the sample waveform graphics
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">Aborted=</td><td><b>RefrProWin</b></td><td align="left">ProWinId NewPosition</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Sets the new progress status and check if the user has aborted
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>RemoveBuffer</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Closes the specified sample-buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>RemoveProWin</b></td><td align="left">ProWinId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Closes the progress window
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>RenameBuffer</b></td><td align="left">BufferId NewName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Gives the specified sample buffer a new name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SaveSample</b></td><td align="left">FileName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Saves the currently selected sample under the specified filename with the currently selected saver
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">BufferId=</td><td><b>SearchBuffer</b></td><td align="left">Name</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Looks up a sample buffer by its name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SelLoader</b></td><td align="left">LoaderName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Activates the loader with the supplied name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SelOperator</b></td><td align="left">LoaderName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Activates the operator with the supplied name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SelPlayer</b></td><td align="left">LoaderName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Activates the player with the supplied name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SelSaver</b></td><td align="left">LoaderName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Activates the saver with the supplied name
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetActiveBuffer</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Makes the supplied sample-buffer the active one
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLength</b></td><td align="left">BufferId NewLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Changes the length of the specified buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoaderParam</b></td><td align="left">LoaderName ParamName Value</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Sets the value of the given parameter of the given loader
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoaderPreset</b></td><td align="left">LoaderName PresetName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Selects a preset for the given loader
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoopEnd</b></td><td align="left">BufferId NewLoopEnd</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the end position of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoopLength</b></td><td align="left">BufferId NewLoopLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the length of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoopMode</b></td><td align="left">BufferId LoopMode[0=Off,1=Forward]</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the respective loop mode for the specified buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetLoopStart</b></td><td align="left">BufferId NewLoopStart</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the start position of the loop
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkXEnd</b></td><td align="left">BufferId NewMarkXEnd</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-end position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkXLength</b></td><td align="left">BufferId NewMarkXLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-length of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkXStart</b></td><td align="left">BufferId NewMarkXStart</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-start position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkYEnd</b></td><td align="left">BufferId NewMarkYEnd</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-end position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkYLength</b></td><td align="left">BufferId NewMarkYLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-length of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetMarkYStart</b></td><td align="left">BufferId NewMarkYStart</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-start position of the mark
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetOperatorParam</b></td><td align="left">OperatorName ParamName Value</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Sets the value of the given parameter of the given operator
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetOperatorPreset</b></td><td align="left">OperatorName PresetName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Selects a preset for the given operator
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td align="right">OldMode=</td><td><b>SetQuietMode</b></td><td align="left">NewMode[0,1]</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
(De)activates the quite mode for arexx processing
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetRate</b></td><td align="left">BufferId NewSampleRate</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Changes the sampling rate of the specified buffer
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetSaverParam</b></td><td align="left">SaverName ParamName Value</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Sets the value of the given parameter of the given saver
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetSaverPreset</b></td><td align="left">SaverName PresetName</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Selects a preset for the given saver
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomXEnd</b></td><td align="left">BufferId NewZoomXEnd</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-end position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomXLength</b></td><td align="left">BufferId NewZoomXLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-length of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomXStart</b></td><td align="left">BufferId NewZoomXStart</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the x-start position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomYEnd</b></td><td align="left">BufferId NewZoomYEnd</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-end position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomYLength</b></td><td align="left">BufferId NewZoomYLength</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-length of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>SetZoomYStart</b></td><td align="left">BufferId NewZoomYStart</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Set the y-start position of the zoom
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>ShowBuffer</b></td><td align="left">BufferId</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Makes a hidden sample visible again
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>ToBack</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Sends __SFX screen to back
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<tr><td>&nbsp;</td><td><b>ToFront</b></td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td colspan="2"><font color="#555555">
Brings __SFX screen to front
</font></td></tr>
<tr><td colspan="3" height="2"><img src="../_img/space.gif" height="2"></td></tr>
<!--LIST_END-->
					</table>
				</td></tr></table>
			</td></tr>
		__SECTION_STYLE_END
		<br>
		__NAVBAR
		__FOOTER
	</body>
</html>
