define(`__PACKAGE',`SoundFX')

# blue colors
# define(`__COL_SEC_BACK',`"`#'001177"')dnl
# define(`__COL_TBL_HEAD',`"`#'3355AA"')dnl
# define(`__COL_TBL_ROW1',`"`#'224499"')dnl
# define(`__COL_TBL_ROW2',`"`#'113388"')dnl
# define(`__COL_BRIGHT',`"`#'EFEFFF"')dnl
# define(`__COL_DARK',`"`#'000010"')dnl

# online-Style : red/gray colors
#define(`__COL_BACK',`"`#'4E3E3E"')dnl
#define(`__COL_SEC_BACK',`"`#'5E4E4E"')dnl
#define(`__COL_TBL_HEAD',`"`#'BEAEAE"')dnl
#define(`__COL_TBL_ROW1',`"`#'8E7E7E"')dnl
#define(`__COL_TBL_ROW2',`"`#'7E6E6E"')dnl
#define(`__COL_FRAME',`"`#'0A0000"')dnl
#define(`__COL_TEXT',`"`#'FFF6F6"')dnl
#define(`__COL_BRIGHT',`"`#'FFF6F6"')dnl
#define(`__COL_DARK',`"`#'0A0000"')dnl

#define(`__PAGE_STYLE',`background="__PATH../_img/backgr6.__IMGEXT" bgcolor=__COL_BACK'` leftmargin="5" rightmargin="5" topmargin="5" text=__COL_BRIGHT link="`#'CEAEFE" vlink="`#'AE8ECE" alink=__COL_BRIGHT')dnl

# Printout-Style : black/white colors
define(`__COL_BACK',`"`#'EEEEEE"')dnl
define(`__COL_SEC_BACK',`"`#'FFFFFF"')dnl
define(`__COL_TBL_HEAD',`"`#'DDDDDD"')dnl
define(`__COL_TBL_ROW1',`"`#'BBBBBB"')dnl
define(`__COL_TBL_ROW2',`"`#'AAAAAA"')dnl
define(`__COL_FRAME',`"`#'000000"')dnl
define(`__COL_TEXT',`"`#'000000"')dnl
define(`__COL_BRIGHT',`"`#'FFFFFF"')dnl
define(`__COL_DARK',`"`#'000000"')dnl

define(`__PAGE_STYLE',`bgcolor=__COL_BACK'` leftmargin="5" rightmargin="5" topmargin="5" text=__COL_TEXT link="`#'2040FF" vlink="`#'0020E0" alink=__COL_BRIGHT')dnl

# words

define(`__SFX',`<b>SoundFX</b>')dnl

# defs

define(`__IMGEXT',`gif')dnl
define(`__IMGWIDTH',`regexp(esyscmd(`dtinfo $1'),`.*width: \([0-9]*\)',`\1')')dnl
define(`__IMGHEIGHT',`regexp(esyscmd(`dtinfo $1'),`.*height: \([0-9]*\)',`\1')')dnl
define(`__IMAGE',`<img src="__PATH../_img/$1.__IMGEXT" width="__IMGWIDTH(__PATH../_img/$1.__IMGEXT)" height="__IMGHEIGHT(__PATH../_img/$1.__IMGEXT)" border="0"/>')dnl

dnl define(`__SECTION_STYLE_BEG',`<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr bgcolor=__COL_FRAME><td><table border="0" width="100%" cellpadding="3" cellspacing="1">')
dnl define(`__SECTION_STYLE_END',`</table></td></tr></table>')

define(`__SECTION_STYLE_BEG',`<table border="0" width="100%" cellpadding="3" cellspacing="1" bgcolor=__COL_FRAME>')
define(`__SECTION_STYLE_END',`</table>')


define(`__SECTION_HEADER',`
	<tr bgcolor=__COL_TBL_HEAD><td>
		<table border="0" width="100%" cellpadding="0" cellspacing="0"><tr>
			<td align="left">
				<b>$1</b>
			</td>
			<td align="right">
				<a href="format(`#%03d',decr(decr(__SECTION)))">
				<img src="__PATH../_img/a_up.__IMGEXT" width="13" height="13" border="0" alt="[up]"></a>
				<img src="__PATH../_img/space.__IMGEXT" width="2">
				<a href="format(`#%03d',__SECTION)">
				<img src="__PATH../_img/a_do.__IMGEXT" width="13" height="13" border="0" alt="[down]"></a>
			</td>
		</tr></table>
	</td></tr>
')dnl

define(`__SECTION',`0')dnl
define(`__SECTION_LABEL',`
	<a name="format(`#%03d',__SECTION)"></a>
	define(`__SECTION',incr(__SECTION))
')dnl

define(`quotedots',`patsubst(`$1',`\.',`\\.')')dnl
define(`quotews',`patsubst(`$1',`\ ',`\\ ')')dnl

define(`remzero1',`format(`%d',$1)')dnl
define(`remzero2',`ifelse(`$1',`__',`',format(`.%d',$1))')dnl
define(`formatchapter',`regexp(`$1', `\(..\)\.\(..\)\.\(..\)', `remzero1(`\1')`'remzero2(`\2')`'remzero2(`\3')` '')')dnl

define(`nextlink',`regexp(esyscmd(`prevnext -n index.txt "$1"'), `\.\/\(.*\)\.m4', `\1.html')')dnl
define(`prevlink',`regexp(esyscmd(`prevnext -p index.txt "$1"'), `\.\/\(.*\)\.m4', `\1.html')')dnl

define(`nodefilter1',`regexp(`$1',`node\(..\)\.',`node\1.__.__.html')')dnl
define(`nodefilter2',`regexp(`$1',`node\(..\...\)\.',`node\1.__.html')')dnl
define(`nodefilter3',`regexp(`$1',`node\(..\...\...\)\.',`node\1.html')')dnl

dnl egrep "define\(.__TITLE." <file>.m4 | sed "s/.*\,.\(.*\)../\1/"
define(`__NODETITLE',`regexp(esyscmd(`htmlinfo "$1" TITLE QUOTEM4'),__PACKAGE` : [0-9.]* \(.*\)',`\1')')dnl

define(`nthelp',`patsubst(substr(`$1',6,8),`[0-9]',`-')')

define(`navtree',`
	ifelse(nthelp(`$1'),`__.__.__',`',`
		[<a href="'__PATH`node__.__.__.html">__PACKAGE</a>]
		ifelse(nthelp(`$1'),`--.__.__',`',`
			[<a href="'__PATH`nodefilter1($1)">__NODETITLE(nodefilter1($1))</a>]
			ifelse(nthelp(`$1'),`--.--.__',`',`
				[<a href="'__PATH`nodefilter2($1)">__NODETITLE(nodefilter2($1))</a>]
				ifelse(substr($1,14,1),`/',`',`
					ifelse(substr($1,12,2),`00',`
						[<a href="'__PATH`nodefilter3($1)">__NODETITLE(nodefilter3($1))</a>]
					',`')
				')
			')
		')
	')
')dnl

define(`__CHAPTER',`ifelse(substr(__NODE,4,8),`__.__.__',`',formatchapter(substr(__NODE,4,eval(len(__file__)-7))))')dnl
define(`__NODEEX',`quotews(quotedots(__NODE))')dnl
