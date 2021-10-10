#!/gg/bin/perl
#
# filters
#   empty lines
#
# invoke as
#   pipe cat ger/node01.06.03.html | perl stripspaces.pl

while($line=<STDIN>) {
	chomp($line);
	if($line!~m/^\s*$/) {
		print "$line\n";
	}
}
exit;
