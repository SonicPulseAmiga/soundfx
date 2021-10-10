define(`navtree',`
	ifelse(`patsubst(`substr($1,4,8)',`\d',`-')',`__.__.__',`',`
		[<a href="'__PATH`node__.__.__.html">__PACKAGE</a>]
		ifelse(`patsubst(substr($1,4,8),`\d',`-')',`--.__.__',`',`
			[<a href="'__PATH`nodefilter1($1)">nodetitle(nodefilter1($1))</a>]
			ifelse(`patsubst(substr($1,4,8),`\d',`-')',`--.--.__',`',`
				[<a href="'__PATH`nodefilter2($1)">nodetitle(nodefilter2($1))</a>]
				ifelse(substr($1,12,1),`/',`',`
					ifelse(substr($1,10,2),`00',`',`
						[<a href="'__PATH`nodefilter3($1)">nodetitle(nodefilter3($1))</a>]
					')
				')
			')
		')
	')
')dnl

define(`navtree',`
	ifelse(`translit(substr(`$1',4,8),`0-9',`----------')',`__.__.__',`',`
		[<a href="'__PATH`node__.__.__.html">__PACKAGE</a>]
		ifelse(`translit(substr(`$1',4,8),`0-9',`----------')',`--.__.__',`',`
			[<a href="'__PATH`nodefilter1($1)">nodetitle(nodefilter1($1))</a>]
		')
	')
')dnl
