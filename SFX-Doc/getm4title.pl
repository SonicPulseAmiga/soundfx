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
		$title=~s/&auml;/�/g;
		$title=~s/&ouml;/�/g;
		$title=~s/&uuml;/�/g;
		$title=~s/&Auml;/�/g;
		$title=~s/&Ouml;/�/g;
		$title=~s/&Uuml;/�/g;
		$title=~s/&szlig;/�/g;
		print "$title";
		exit(0);
	}
}
exit(1);
