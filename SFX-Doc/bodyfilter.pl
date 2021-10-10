#!/gg/bin/perl
#
# filters
#   lines before and after html body
#
# invoke as
#   pipe cat ger/node01.06.03.html | perl bodyfilter.pl



while(($line=<STDIN>) && ($line!~m/^\s\<body\ .*\>/i)) { }
while(($line=<STDIN>) && ($line!~m/^\s\<\/body\>/i)) {
	print $line;
}
exit;
