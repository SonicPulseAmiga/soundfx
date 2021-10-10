#!/usr/bin/perl
#
# scans a c-source for function definitions
# use like
#   for file in *.c; do echo $file; cat $file | perl ../scanfuncs.pl | sort >$file.funcs; done

while($line=<STDIN>) {
	if($line=~m/^[^\s\/\}\#\*].*?(\w+)\s*\(.*\)\s*\{*\s*[^;]$/) {
		$func=$1;
		if($func=~m/^LIB(.*)$/) {
			print "$1\n";
		}
		else {
			print "$func\n";
		}
	}
}
exit;
