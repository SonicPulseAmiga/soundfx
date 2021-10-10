define(`__PACKAGE',`SoundFX')
define(`__PAGE_STYLE',`background="../_img/backgr.gif" bgcolor="#112288" leftmargin="5" rightmargin="5" topmargin="5" text="#FFFFFF" link="#FFFFFF" vlink="#EEEEEE" alink="#000000"')dnl

define(`__SECTION',`0')dnl
define(`__SECTION_LABEL',`
	<a name="format(`#%03d',__SECTION)">
	define(`__SECTION',incr(__SECTION))
')dnl

define(`quotedots',`patsubst(`$1',`\.',`\\.')')dnl
define(`quotews',`patsubst(`$1',`\ ',`\\ ')')dnl

define(`remzero1',`format(`%d',$1)')dnl
define(`remzero2',`ifelse(`$1',`  ',`',format(`.%d',$1))')dnl
define(`formatchapter',`regexp(`$1', `\(..\)\.\(..\)\.\(..\)', `remzero1(`\1')`'remzero2(`\2')`'remzero2(`\3')` '')')dnl

define(`nextlink',`regexp(esyscmd(`prevnext index.txt "$1" NEXT'), `\(.*\)\.m4', `\1.html')')dnl
define(`prevlink',`regexp(esyscmd(`prevnext index.txt "$1" PREV'), `\(.*\)\.m4', `\1.html')')dnl

define(`nodefilter1',`regexp(`$1',`node\(..\)\.',`node\1.  .  .html')')dnl
define(`nodefilter2',`regexp(`$1',`node\(..\...\)\.',`node\1.  .html')')dnl
define(`nodetitle',`regexp(esyscmd(`htmlinfo "$1" TITLE'),__PACKAGE` : \(.*\)',`\1')')dnl

define(`navtree',`
	ifelse(substr($1,4,8),`  .  .  ',`',`
		[<a href="node  .  .  .html">__PACKAGE</a>]
		ifelse(substr($1,7,5),`  .  ',`',`
			[<a href="nodefilter1($1)">nodetitle(nodefilter1($1))</a>]
			ifelse(substr($1,10,2),`  ',`',`
				[<a href="nodefilter2($1)">nodetitle(nodefilter2($1))</a>]
			')
		')
	')
')dnl

define(`__CHAPTER',`ifelse(substr(__NODE,4,8),`  .  .  ',`',formatchapter(substr(__NODE,4,eval(len(__file__)-7))))')dnl
define(`__NODEEX',`quotews(quotedots(__NODE))')dnl
