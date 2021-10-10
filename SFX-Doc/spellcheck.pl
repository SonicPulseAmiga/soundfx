#!/gg/bin/perl

if((!defined($ARGV[0])) || ($ARGV[0] eq "")) {
  die("USAGE spellcheck.pl <infile> [<language>]");
}
$ifilename=$ARGV[0];
if(defined($ARGV[1]) || ($ARGV[1] ne "")) {
  $opt_language="-d $ARGV[1]";
}
else {
  $opt_language="";
}

#print "checking \"$ifilename\"\n";
#@lines=`cat $ifilename | perl ../filter.pl | ispell -a $opt_language`;
@lines=`cat $ifilename | perl ../filter.pl | aspell -a $opt_language`;

# DEBUG
#print $#lines."\n";

#-- init linenumber
$lineno=1;
#-- init stack
@stack=();
$stackix=0;

#-- process results
foreach $line (@lines) {
  chomp($line);
  if($line eq "") {
	&analyse_stack();
  }
  else {
	#-- collect results
	if($line!~m/^[@*+]/) {
	  $stack[$stackix++]=$line;
#	  print "[$line]\n";
	}
  }
}
&analyse_stack();
exit;

sub analyse_stack {
  if($stackix>0) {
	foreach $entry (@stack) {
	  print "$ifilename:$lineno:";
	  if($entry=~m/^& (\w*) (\d*) (\d*): (.*)$/) {
		$word=$1;
		$ct=$2;
		$col=$3+1;
		$suggest=$4;
		# it seems that even xemacs not alway likes :col:
		print "$col: misspelled word \"$word\" => $suggest\n";
		#print " misspelled word \"$word\" => $suggest\n";
		next;
	  }
      if($entry=~m/^# (\w*) (\d*)$/) {
		 $word=$1;
		 $col=$2+1;
		 print "$col: unknown word \"$word\"\n";
		 next;
	  }
      print "\n";
	}
  }
  #-- increment linenumber
  $lineno++;
  #-- reset stack
  @stack=();
  $stackix=0;
}
