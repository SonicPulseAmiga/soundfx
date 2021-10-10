#!/gg/bin/perl
#
# gets the title from m4 source
#
# invoke as
#   pipe cat ger/node01.06.03.m4 | perl getm4title.pl

while($line=<STDIN>) {
	chomp($line);
	if($line=~m/^define..__TITLE.,.(.*)..$/) {
		$title=$1;
		$title=~s/&auml;/ä/g;
		$title=~s/&ouml;/ö/g;
		$title=~s/&uuml;/ü/g;
		$title=~s/&Auml;/Ä/g;
		$title=~s/&Ouml;/Ö/g;
		$title=~s/&Uuml;/Ü/g;
		$title=~s/&szlig;/ß/g;
		print "$title";
		exit(0);
	}
}
exit(1);
