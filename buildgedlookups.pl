#!/usr/bin/perl
#
# scans a c-source for function definitions to build golded lookup files
# use like
#   for file in *.c; do echo $file; cat $file | perl ../buildgedlookups.pl; done

open(DICT,">>golded:add-ons/c++/syntax/dictionaries/soundfx");
open(INFO,">>golded:add-ons/c++/quickinfo/soundfx.words");

while($line=<STDIN>) {
	chomp($line);
	if($line=~m/^[^\s\/\}\#\*].*?(\w+)\s*\(.*\)\s*\{*\s*[^;]$/) {
		$func=$1;
		if($func=~m/^LIB(.*)$/) {
			$func=$1;
        }
		print DICT "$func\n";
        if($line=~m/^(.*?)\s*\{$/) {
            $line=$1;
        }
        print INFO "$func( $line\n";
	}
}

close(DICT);
close(INFO);


exit;
