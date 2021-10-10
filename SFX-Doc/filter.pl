#!/gg/bin/perl
#
# filters
#   m4 commands
#   html tags
#   macros
#
# invoke as
#   pipe cat ger/node01.06.03.m4 | perl filter.pl

while($line=<STDIN>) {
	$line=filter_m4($line);
	$line=filter_html($line);
	$line=filter_macros($line);
	print $line;
}
exit;


# filter all initial m4 commands
sub filter_m4() {
	($line)=@_;

	if($line=~m/^divert/) { return("\n"); }
	if($line=~m/^define/) { return("\n"); }
	if($line=~m/^include/) { return("\n"); }

	return($line);
}

# filter html tags
sub filter_html() {
	($line)=@_;

	$line=~s/(<.*?>)/' ' x length($1)/eg;
	$line=~s/\t/    /g;

	$line=~s/\&nbsp\;/ /g;
	$line=~s/\&auml\;/ä/g;
	$line=~s/\&ouml\;/ö/g;
	$line=~s/\&uuml\;/ü/g;
	$line=~s/\&Auml\;/Ä/g;
	$line=~s/\&Ouml\;/Ö/g;
	$line=~s/\&Uuml\;/Ü/g;
	$line=~s/\&szlig\;/ß/g;
	$line=~s/\&#39\;/\'/g;
	$line=~s/\&quot\;/\"/g;
	$line=~s/\&amp\;/\&/g;
	$line=~s/\&lt\;/\</g;
	$line=~s/\&gt\;/\>/g;
	return($line);
}

# filter macros
sub filter_macros() {
	($line)=@_;

	$line=~s/(__.*?)(\s)/(' ' x length($1)).$2/eg;

	return($line);
}
