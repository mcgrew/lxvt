/*--------------------------------*-C-*---------------------------------*
 * File:    misc.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1996        mj olesen <olesen@me.QueensU.CA>
 * Copyright (c) 1997,1998   Oezguer Kesim <kesim@math.fu-berlin.de>
 * Copyright (c) 1998-2000   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 2004        Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *----------------------------------------------------------------------*/

#include "../config.h"
#include "rxvt.h"



/* EXTPROTO */
char*
rxvt_r_basename(const char *str)
{
    char*   base = STRRCHR(str, '/');

    return (char *)(base ? base + 1 : str);
}


/*
 * check that the first characters of S1 match S2
 *
 * No Match
 *      return: 0
 * Match
 *      return: STRLEN (S2)
 */
/* EXTPROTO */
int
rxvt_str_match(const char *s1, const char *s2)
{
    if( IS_NULL(s1) || IS_NULL(s2))
	return 0;
    else
    {
	int             n = STRLEN(s2);

	return ( (STRNCMP(s1, s2, n) == 0) ? n : 0 );
    }
}


/* EXTPROTO */
const char*
rxvt_str_skip_space(const char *str)
{
    if (str)
    while (*str && isspace((int) *str))
        str++;
    return str;
}


/*
 * remove leading/trailing space and strip-off leading/trailing quotes.
 * in place.
 */
/* EXTPROTO */
char*
rxvt_str_trim(char *str)
{
    char           *r, *s;
    int             n;

    if (!str || !*str)	    /* shortcut */
	return str;

    /* skip leading spaces */
    for (s = str; *s && isspace((int) *s); s++)
	;
    /* goto end of string */
    for (n = 0, r = s; *r++; n++)
	;
    r -= 2;
    /* dump return */
    if (n > 0 && *r == '\n')
	n--, r--;
    /* backtrack along trailing spaces */
    for (; n > 0 && isspace((int) *r); r--, n--)
	;
    /* skip matching leading/trailing quotes */
    if( *s == '"' && *r == '"' && n > 1 )
    {
	s++;
	n -= 2;
    }
    /* copy back over: forwards copy */
    for (r = str; n; n--)
	*r++ = *s++;
    *r = '\0';

    return str;
}


/*
 * in-place interpretation of string:
 *
 *      backslash-escaped:      "\a\b\E\e\n\r\t", "\octal"
 *      Ctrl chars:     ^@ .. ^_, ^?
 *
 * returns the converted string length
 */
/* EXTPROTO */
int
rxvt_str_escaped(char *str)
{
    char            ch, *s, *d;
    int             i, num;

    if (IS_NULL(str) || *str == '\0') return 0;

    d = s = str;

    /*
     * 2006-03-23 gi1242: Disabled, as the user has no 'easy' way to send
     * strings beginning with M-x to mrxvt.
     *
     * 2006-05-24 gi1242: With macros, the user has an 'easy' way to send
     * strings to mrxvt. However emacs users should use emacs macros do do such
     * things, and not require code bloat in mrxvt for these random features.
     * Besides, mrxvt users should use Vim anyway ... :)
     */
#if 0
    if (*s == 'M' && s[1] == '-')
    {
	/* Emacs convenience, replace leading `M-..' with `\E..' */
	*d++ = C0_ESC;
	s += 2;
	if (toupper((int) *s) == 'X')
	    /* append carriage-return for `M-xcommand' */
	    for (*d++ = 'x', append = '\r', s++;
		isspace((int) *s);
		s++)
		;
    }
#endif
    for (; (ch = *s++);)
    {
	if (ch == '\\')
	{
	    ch = *s++;
	    if (ch >= '0' && ch <= '7')		/* octal */
	    {
		num = ch - '0';
		for (i = 0; i < 2; i++, s++)
		{
		    ch = *s;
		    if (ch < '0' || ch > '7')
			break;
		    num = num * 8 + ch - '0';
		}
		ch = (char)num;
	    }
	    else if (ch == 'a')
		ch = C0_BEL;	/* bell */
	    else if (ch == 'b')
		ch = C0_BS; /* backspace */
	    else if (ch == 'E' || ch == 'e')
		ch = C0_ESC;	/* escape */
	    else if (ch == 'n')
		ch = '\n';  /* newline */
	    else if (ch == 'r')
		ch = '\r';  /* carriage-return */
	    else if (ch == 't')
		ch = C0_HT; /* tab */

	    else if (ch != '\\' && ch != '^' )
		*d++ = '\\'; /* Copy over backslash */
	}

	else if (ch == '^')
	{
	    ch = *s++;

	    if( ch == '?' )
		ch = 127;
	    else if( toupper(ch) >= 'A' && toupper(ch) <= 'Z' )
		ch = toupper(ch) - '@';
	    else
		*d++ = '^'; /* Copy over caret */
	}

	*d++ = ch;
    }

#if 0 /* Users can terminate their own stinking strings */
    /* ESC] is an XTerm escape sequence, must be terminated */
    if (*str == '\0' && str[1] == C0_ESC && str[2] == ']')
	append = CHAR_ST;

    /* add trailing character as required */
    if (append && d[-1] != append)
	*d++ = append;
#endif
    *d = '\0';

    return (d - str);
}

/*
 * % interpolate expand src, and copy into dst. Returns length of the
 * interpolated string.
 */
/* EXTPROTO */
int
rxvt_percent_interpolate( rxvt_t *r,
	const char *src, int len, char *dst, int maxLen )
{
    int	i=0,	/* Unexpanded string index */
	j=0;	/* Expanded string index */

    rxvt_dbgmsg ((DBG_DEBUG, DBG_MISC, "rxvt_percent_interpolate( r, %s, %d, %s, %d )\n", src, len, "dst", maxLen));

    while( i < len-1 && j < maxLen-1 )
    {
	if( src[i] == '%' )
	{
	    switch( src[++i] )
	    {
		case '%':
		    /* Copy % over */
		    dst[j++] = src[i++];
		    break;

		case 'S':
		    /* Exit status of dead processes */
		    if( PVTS( r )->dead )
			j += snprintf( dst + j, maxLen - j, "%d",
				WEXITSTATUS( PVTS( r )->status ) );
		    else
			dst[ j++ ] = src[ i ];

		    i++;
		    break;

		case 'N':
		    /* Normal / abnormal exit status of dead processes */
		    if( PVTS( r )->dead )
			j += snprintf( dst + j, maxLen - j, "%s",
				WIFEXITED( PVTS( r )->status )
				    ? "normally" : "abnormally" );
		    else
			dst[ j++ ] = src[ i ];

		    i ++;
		    break;

		case 's':
		    /*
		     * Selection. TODO Also paste selection if it is not
		     * owned by mrxvt.
		     */
		    if( NOT_NULL( r->selection.text ) )
			j += snprintf( dst + j, maxLen -j,
				"%s", r->selection.text );
		    i++;
		    break;

		case 'p':
		    /* Pid of process in current tab */
		    j += snprintf( dst + j, maxLen - j, "%d",
			    PVTS(r)->cmd_pid );
		    i++;
		    break;

		case 'P':
		    /* PID of mrxvt */
		    j += snprintf( dst + j, maxLen - j, "%d", getpid() );
		    i++;
		    break;

		default:
		    rxvt_msg (DBG_ERROR, DBG_MISC, "Unrecognized flag %%%c in '%s'", src[i++], src );
		    break;
	    }
	}
	else
	    dst[j++] = src[i++];
    }

    /* Copy last char over */
    if( i == len-1 && j < maxLen-1 )
	dst[j++] = src[i++];

    /* NULL terminate dst */
    if( j > maxLen - 1 )
	j = maxLen - 1;
    if( j == 0 || dst[j-1] )
	dst[j++] = 0;

    /* % expansion done. Copy the string and length over */
    return j;
}

/*
 * Split a comma-separated string into an array, stripping leading and
 * trailing spaces (and paired quotes) from each entry.  Empty strings
 * are properly returned
 * Caller should free each entry and array when done
 */
/* EXTPROTO */
char**
rxvt_splitcommastring(const char *cs)
{
    int             l, n, p;
    const char     *s, *t;
    char          **ret;

    if( IS_NULL(s = cs))
	s = "";

    for( n=1, t=s; *t; t++)
	if (*t == ',')
	    n++;

    assert (n >= 0 && n+1 > 0);	/* possible integer overflow? */

    ret = rxvt_malloc( (n + 1) * sizeof(char *) );
    ret[n] = NULL;

    for( l = 0, t = s; l < n; l++ )
    {
	for( ; *t && *t != ','; t++ );
	p = t - s;

	ret[l] = rxvt_malloc(p + 1);
	STRNCPY(ret[l], s, p);
	ret[l][p] = '\0';
	rxvt_str_trim(ret[l]);
	s = ++t;
    }
    return ret;
}

/*----------------------- end-of-file (C source) -----------------------*/
