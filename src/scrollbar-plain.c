/*--------------------------------*-C-*---------------------------------*
 * File:    scrollbar-plain.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997,1998   mj olesen <olesen@me.QueensU.CA>
 * Copyright (c) 1999-2001   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 2004        Marc Lehmann <pcg@goof.com>
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



/*----------------------------------------------------------------------*/
#ifdef HAVE_SCROLLBARS
#ifdef PLAIN_SCROLLBAR


#define CHOOSE_GC_FG(R, PIXCOL)	\
    XSetForeground ((R)->Xdisplay, (R)->scrollBar.gc, (PIXCOL))


/* EXTPROTO */
void
rxvt_scrollbar_init_plain (rxvt_t* r)
{
    XGCValues       gcvalue;
    unsigned long   gcmask;


    UNSET_GC(r->scrollBar.gc);

    /* Initialize the colors */
#ifdef KEEP_SCROLLCOLOR
    r->scrollBar.plain_fg = ISSET_PIXCOLOR( r->h, Color_scroll) ?
	r->pixColorsFocus[Color_scroll] : r->pixColorsFocus[Color_bg];

    r->scrollBar.plain_bg = ISSET_PIXCOLOR( r->h, Color_scroll) ?
	r->pixColorsFocus[Color_trough] : r->pixColorsFocus[Color_bg];
#else
    /*
     * Draw in uglyass reverse video
     */
    r->scrollBar.plain_fg = r->pixColorsFocus[Color_bg];
    r->scrollBar.plain_bg = r->pixColorsFocus[Color_fg];
#endif

    /* Create GC */
    gcmask = GCForeground;
    gcvalue.foreground = r->scrollBar.plain_fg;

    {
	gcmask  |= GCBackground;
	gcvalue.background = r->scrollBar.plain_bg;
	XSetWindowBackground (r->Xdisplay, r->scrollBar.win,
	    r->scrollBar.plain_bg);
    }

    r->scrollBar.gc = XCreateGC (r->Xdisplay, r->scrollBar.win,
	    gcmask, &gcvalue);
    assert (IS_GC(r->scrollBar.gc));
}


void
rxvt_scrollbar_exit_plain (rxvt_t* r)
{
    /* No resources to free */
}


/* EXTPROTO */
int
rxvt_scrollbar_show_plain(rxvt_t *r, int update __attribute__((unused)), int last_top, int last_bot, int scroller_len)
{
    int		xsb = 0;
    int		sbwidth = r->scrollBar.width - 1;
    int		clear = 0;  /* Call XClearArea? */


    /* scrollbar slider */
	clear = 0;

    scroller_len -= 1;

    xsb = ISSET_OPTION(r, Opt_scrollBar_right) ? 1 : 0;
    if (last_top < r->scrollBar.top)
	XClearArea(r->Xdisplay, r->scrollBar.win,
	    r->sb_shadow + xsb, last_top,
	    sbwidth + 1-xsb, (r->scrollBar.top - last_top), False);

    if (r->scrollBar.bot < last_bot)
	XClearArea(r->Xdisplay, r->scrollBar.win,
	    r->sb_shadow + xsb, r->scrollBar.bot,
	    sbwidth + 1-xsb, (last_bot - r->scrollBar.bot + clear), False);


    CHOOSE_GC_FG(r, r->scrollBar.plain_fg);
    if (clear)	{
	/* transparent background or background image */
	XClearArea (r->Xdisplay, r->scrollBar.win,
	    r->sb_shadow + xsb, r->scrollBar.top,
	    sbwidth, scroller_len,
	    False);
	XDrawRectangle(r->Xdisplay, r->scrollBar.win, r->scrollBar.gc,
	    r->sb_shadow + xsb, r->scrollBar.top,
	    sbwidth - xsb, scroller_len);
    }
    else    {
	XFillRectangle(r->Xdisplay, r->scrollBar.win, r->scrollBar.gc,
	    r->sb_shadow + xsb, r->scrollBar.top,
	    sbwidth, scroller_len);
    }

    return 1;
}

#endif	/* PLAIN_SCROLLBAR */
#endif	/* HAVE_SCROLLBARS */
/*----------------------- end-of-file (C source) -----------------------*/
