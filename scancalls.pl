#!/usr/bin/perl
#
# scans a c-source for function calls
# use like
#   for file in *.c; do echo $file; cat $file | perl ../scancalls.pl | sort | uniq >$file.calls; done

#$linenr=0;
while($line=<STDIN>) {
	#$linenr++;

	# remove one line comment
	$line=~s/^(.*)\/\/.*$/$1/;
	# remove block comment
	# @TODO

	# we currently do see use of function pointer as calls
	# because we can't differentiat this from variable assignments
	# $line=~m/\=\s*(\w+)\s*\;/ || $line=~m/\,\s*(\w+)\s*\,/

	while($line=~m/^\s/ && $line=~m/[\s\=\+\-\*\/\&\|\^\!\?\:\>\<\;\,\)\(](\w+)\s*\(.*?\)/) {
		$call=$1;
		#print "$linenr:[$call][$line]\n";
		$line=~s/([\s\=\+\-\*\/\&\|\^\!\?\:\>\<\;\,\)\(])\w+\s*(\(.*?\))/$1;$2/;
		if($call!~m/^(for|if|return|switch|while)$/) {
			if($call=~m/^LIB(.*)$/) {
				print "$1\n";
			}
			else {
				print "$call\n";
			}
		}
	}
}
exit;
