#!/usr/bin/perl
#
# scans sfx-IOArexx.c source for arexx function definitions to build online help files
#
# usage
#   cd sfx-doc
#   perl ./m4gen_rexx.pl ../sfx-main/SFX-IORexx.c ./ger/node03.01.__.m4
#
# todo
#   extend doc to include
#   - result
#   - params
#   - short desc (eng, ger)
#   examples
#	  "EnableChannel",            REXXID_EnableChannel,       3,  /** # BufferId ChannelNo # Activate the respective channel */
#	  "Exit",                     REXXID_Exit,                1,  /** # # End SoundFX without asking */
#	  "GetActiveBuffer",          REXXID_GetActiveBuffer,     1,  /** BufferID # # Returns the currently active buffer */
#

# commandline options
$fnc=$ARGV[0];
$fnm=$ARGV[1];

#print "$fnc,$fnm\n";

# statistics
$funct=0;   # total number of functions
$docct=0;   # documented number of functions

open(SRCC,"<$fnc");
open(SRCM,"<$fnm");
open(DSTM,">$fnm.new");

# scan to begin of function list in m4-source and copy verbatim to m4 output
$scan=1;
while($scan && ($line=<SRCM>)) {
	chomp($line);
	if($line=~m/^<!--LIST_BEG-->$/) { $scan=0; }
	print DSTM "$line\n";
}

# scan to begin of function list in c-source
$scan=1;
while($scan && ($line=<SRCC>)) {
	chomp($line);
	if($line=~m/^\} RexxCmds\[\]=/) { $scan=0; }
}

# read fuction list from c-source and append to m4 output
$scan=1;
while($scan && ($line=<SRCC>)) {
	chomp($line);
	if($line=~m/NULL/) { $scan=0; }
	elsif($line=~m/\s*\"(\w+)\",\s*\w+,\s*\d+,\s*(.*)$/) {
		$func=$1;
		$doc=$2;
		$funct++;
		print DSTM "<tr>";
		# is there extra documentation available ?
		if($doc=~m/\/\*\*\s*(.*?)\s*#\s*(.*?)\s*#\s*(.*?)\s*\*\//) {
			$result=$1;
			$param=$2;
			$info=$3;
			$docct++;
			if($result ne "") { print DSTM "<td align=\"right\">$result=</td>";	}
			else              { print DSTM "<td>&nbsp;</td>"; }
								print DSTM "<td><b>$func</b></td>";
			if($param ne "")  {	print DSTM "<td align=\"left\">$param</td>"; }
			else              { print DSTM "<td>&nbsp;</td>"; }
			if($info ne "")   {	print DSTM "</tr>\n<tr><td>&nbsp;</td><td colspan=\"2\"><font color=\"#555555\">\n$info\n</font></td>";	}
		}
		else                  { print DSTM "<td>&nbsp;</td><td><b>$func</b></td><td>&nbsp;</td>"; }
		print DSTM "</tr>\n<tr><td colspan=\"3\" height=\"2\"><img src=\"../_img/space.gif\" height=\"2\"></td></tr>\n";
	}
}

# skip old function list in m4 source
$scan=1;
while($scan && ($line=<SRCM>)) {
	chomp($line);
	if($line=~m/^<!--LIST_END-->$/) { $scan=0; }
}
print DSTM "$line\n";

# copy everything after function list verbatim to m4 output
while($line=<SRCM>) {
	print DSTM "$line";
}

close(SRCC);
close(SRCM);
close(DSTM);

# backup old m4 source and put new file inplace
`rm -f $fnm.bak && mv $fnm $fnm.bak && mv $fnm.new $fnm`;

# statistics
print "$docct/$funct functions documented\n";

exit;
