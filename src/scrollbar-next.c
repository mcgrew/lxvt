/*--------------------------------*-C-*---------------------------------*
 * File:    scrollbar-next.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997,1998   mj olesen <olesen@me.QueensU.CA>
 * Copyright (c) 1998        Alfredo K. Kojima <kojima@windowmaker.org>
 * Copyright (c) 1999-2001   Geoff Wing <gcw@pobox.com>
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
#ifdef NEXT_SCROLLBAR


#define CHOOSE_GC_FG(R, PIXCOL)	\
    XSetForeground ((R)->Xdisplay, (R)->scrollBar.gc, (PIXCOL))

#define TILEGC	    (r->scrollBar.next_stippleGC)

#define DIMPLE	    (r->scrollBar.next_dimple)
#define UPARROW	    (r->scrollBar.next_upArrow)
#define HIUPARROW   (r->scrollBar.next_upArrowHi)
#define DNARROW	    (r->scrollBar.next_downArrow)
#define HIDNARROW   (r->scrollBar.next_downArrowHi)


#define n_stp_width 8
#define n_stp_height	2
const unsigned char n_stp_bits[] = { 0x55, 0xaa };

/*
 * N*XTSTEP like scrollbar - written by Alfredo K. Kojima
 */
#define DIMPLE_WIDTH   6
#define DIMPLE_HEIGHT  6
#define ARROW_WIDTH   11
#define ARROW_HEIGHT  13

const char   *const SCROLLER_DIMPLE[] = {
    ".%###.",
    "%#%%%%",
    "#%%...",
    "#%..  ",
    "#%.   ",
    ".%.  ."
};
const char   *const SCROLLER_ARROW_UP[] = {
    "...........",
    "...........",
    ".....%.....",
    ".....#.....",
    "....%#%....",
    "....###....",
    "...%###%...",
    "...#####...",
    "..%#####%..",
    "..#######..",
    ".%#######%.",
    "...........",
    "..........."
};
const char   *const SCROLLER_ARROW_DOWN[] = {
    "...........",
    "...........",
    ".%#######%.",
    "..#######..",
    "..%#####%..",
    "...#####...",
    "...%###%...",
    "....###....",
    "....%#%....",
    ".....#.....",
    ".....%.....",
    "...........",
    "..........."
};
const char   *const HI_SCROLLER_ARROW_UP[] = {
    "           ",
    "           ",
    "     %     ",
    "     %     ",
    "    %%%    ",
    "    %%%    ",
    "   %%%%%   ",
    "   %%%%%   ",
    "  %%%%%%%  ",
    "  %%%%%%%  ",
    " %%%%%%%%% ",
    "           ",
    "           "
};
const char   *const HI_SCROLLER_ARROW_DOWN[] = {
    "           ",
    "           ",
    " %%%%%%%%% ",
    "  %%%%%%%  ",
    "  %%%%%%%  ",
    "   %%%%%   ",
    "   %%%%%   ",
    "    %%%    ",
    "    %%%    ",
    "     %     ",
    "     %     ",
    "           ",
    "           "
};


/* INTPROTO */
static Pixmap
rxvt_render_pixmap(rxvt_t *r, const char *const *data, int width, int height)
{
    char	    a;
    int		    x, y;
    Pixmap	    d;
    unsigned long   pointcolour;

    d = XCreatePixmap (r->Xdisplay, r->scrollBar.win, width, height,
	    XDEPTH);
    if (NOT_PIXMAP(d))
	return None;

    for (y = 0; y < height; y++) {
	for (x = 0; x < width; x++) {
	    if ((a = data[y][x]) == ' ' || a == 'w')
		pointcolour = r->scrollBar.next_white;
	    else if (a == '.' || a == 'l')
		pointcolour = r->scrollBar.next_bg;
	    else if (a == '%' || a == 'd')
		pointcolour = r->scrollBar.next_dark;
	    else	/* if (a == '#' || a == 'b' || a) */
		pointcolour = r->scrollBar.next_fg;
	    CHOOSE_GC_FG(r, pointcolour);
	    XDrawPoint(r->Xdisplay, d, r->scrollBar.gc, x, y);
	}
    }

    return d;
}


/* EXTPROTO */
void
rxvt_scrollbar_init_next (rxvt_t *r)
{
    XGCValues	    gcvalue;
    unsigned long   gcmask;
    XColor	    xcol;
    Pixmap	    stipple;


    UNSET_GC(TILEGC);
    UNSET_PIXMAP(DIMPLE);


    gcvalue.graphics_exposures = False;


    /* Initialize the colors */
    r->scrollBar.next_fg = r->pixColorsFocus[Color_Black];
    xcol.red = 0xaeba;
    xcol.green = 0xaaaa;
    xcol.blue = 0xaeba;
    if (!rxvt_alloc_color(r, &xcol))
	xcol.pixel = r->pixColorsFocus[Color_AntiqueWhite];
    r->scrollBar.next_bg = xcol.pixel;
    r->scrollBar.next_white = r->pixColorsFocus[Color_White];
    xcol.red = 0x51aa;
    xcol.green = 0x5555;
    xcol.blue = 0x5144;
    if (!rxvt_alloc_color(r, &xcol))
	xcol.pixel = r->pixColorsFocus[Color_Grey25];
    r->scrollBar.next_dark = xcol.pixel;

    
    /* Create scrollBar GC */
    gcvalue.foreground = r->scrollBar.next_fg;
    gcvalue.background = r->scrollBar.next_bg;
    gcmask = GCForeground;
    gcmask |= GCBackground;
    r->scrollBar.gc = XCreateGC (r->Xdisplay, r->scrollBar.win,
			gcmask, &gcvalue);
    assert (IS_GC(r->scrollBar.gc));


    /* Create stipple GC */
    stipple = XCreateBitmapFromData(r->Xdisplay, r->scrollBar.win,
		(char *)n_stp_bits, n_stp_width, n_stp_height);
    if (IS_PIXMAP(stipple)) {
	gcvalue.foreground = r->scrollBar.next_dark;
	gcvalue.background = r->scrollBar.next_bg;
	gcvalue.fill_style = FillOpaqueStippled;
	gcvalue.stipple = stipple;
	gcmask = GCForeground | GCStipple | GCFillStyle;
	gcmask |= GCBackground;

	TILEGC = XCreateGC(r->Xdisplay, r->scrollBar.win,
		    gcmask, &gcvalue);
	assert (IS_GC(TILEGC));
    }


    /* Create pixmaps */
    DIMPLE = rxvt_render_pixmap(r, SCROLLER_DIMPLE,
		DIMPLE_WIDTH, DIMPLE_HEIGHT);
    assert (IS_PIXMAP(DIMPLE));
    UPARROW = rxvt_render_pixmap(r, SCROLLER_ARROW_UP,
		ARROW_WIDTH, ARROW_HEIGHT);
    assert (IS_PIXMAP(UPARROW));
    HIUPARROW = rxvt_render_pixmap(r, HI_SCROLLER_ARROW_UP,
		ARROW_WIDTH, ARROW_HEIGHT);
    assert (IS_PIXMAP(HIUPARROW));
    DNARROW = rxvt_render_pixmap(r, SCROLLER_ARROW_DOWN,
		ARROW_WIDTH, ARROW_HEIGHT);
    assert (IS_PIXMAP(DNARROW));
    HIDNARROW = rxvt_render_pixmap(r, HI_SCROLLER_ARROW_DOWN,
		ARROW_WIDTH, ARROW_HEIGHT);
    assert (IS_PIXMAP(HIDNARROW));

	XSetWindowBackground (r->Xdisplay, r->scrollBar.win,
	    r->scrollBar.next_bg);
}


/* EXTPROTO */
void
rxvt_scrollbar_exit_next (rxvt_t *r)
{
    if (IS_GC(r->scrollBar.next_stippleGC)) {
	XFreeGC (r->Xdisplay, r->scrollBar.next_stippleGC);
	UNSET_GC(r->scrollBar.next_stippleGC);
    }
    if (IS_PIXMAP(r->scrollBar.next_dimple))	{
	XFreePixmap (r->Xdisplay, r->scrollBar.next_dimple);
	UNSET_PIXMAP(r->scrollBar.next_dimple);
    }
    if (IS_PIXMAP(r->scrollBar.next_upArrow))	{
	XFreePixmap (r->Xdisplay, r->scrollBar.next_upArrow);
	UNSET_PIXMAP(r->scrollBar.next_upArrow);
    }
    if (IS_PIXMAP(r->scrollBar.next_upArrowHi))	{
	XFreePixmap (r->Xdisplay, r->scrollBar.next_upArrowHi);
	UNSET_PIXMAP(r->scrollBar.next_upArrowHi);
    }
    if (IS_PIXMAP(r->scrollBar.next_downArrow))	{
	XFreePixmap (r->Xdisplay, r->scrollBar.next_downArrow);
	UNSET_PIXMAP(r->scrollBar.next_downArrow);
    }
    if (IS_PIXMAP(r->scrollBar.next_downArrowHi))   {
	XFreePixmap (r->Xdisplay, r->scrollBar.next_downArrowHi);
	UNSET_PIXMAP(r->scrollBar.next_downArrowHi);
    }
}


/* Draw bevel & arrows */
/* INTPROTO */
static void
next_draw_bevel (rxvt_t *r, Drawable d, int x1, int y1, int w, int h)
{
    int		 x2, y2;

    x2 = x1 + w - 1;	    /* right  point */
    y2 = y1 + h - 1;	    /* bottom point */

    /* white top and left */
    CHOOSE_GC_FG(r, r->scrollBar.next_white);
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x1, y1, x2, y1);
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x1, y1, x1, y2);

    /* black bottom and right */
    CHOOSE_GC_FG(r, r->scrollBar.next_fg);
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x1, y2, x2, y2);
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x2, y1, x2, y2);

    /* dark inside bottom and right */
    CHOOSE_GC_FG(r, r->scrollBar.next_dark);
    x1++, y1++, x2--, y2--; /* move in one point */
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x1, y2, x2, y2);
    XDrawLine(r->Xdisplay, d, r->scrollBar.gc, x2, y1, x2, y2);
}


/* EXTPROTO */
int
rxvt_scrollbar_show_next(rxvt_t *r, int update, int last_top, int last_bot, int scroller_len)
{
    int		height;
    Drawable	s;
    int		page = ATAB(r);


    height = r->scrollBar.end + NEXT_SB_TBTN_HEIGHT + NEXT_SB_PAD;

    if (PVTS(r, page)->nscrolled == 0 || !update) {
	XClearArea (r->Xdisplay, r->scrollBar.win,
	    0, 0, SB_WIDTH_NEXT + 1, height, False);

	CHOOSE_GC_FG(r, r->scrollBar.next_fg);
	XDrawRectangle(r->Xdisplay, r->scrollBar.win, r->scrollBar.gc,
	    0, NEXT_SB_BD_WIDTH, SB_WIDTH_NEXT,
	    height + NEXT_SB_BD_WIDTH);

	XFillRectangle(r->Xdisplay, r->scrollBar.win, TILEGC,
	    NEXT_SB_LPAD, 0, NEXT_SB_BTN_WIDTH, height);
    }

    if (PVTS(r, page)->nscrolled) {
	if (last_top < r->scrollBar.top || !update) {
	    /*
	    ** Area above the scroller
	    */
	    XFillRectangle(r->Xdisplay, r->scrollBar.win, TILEGC,
		NEXT_SB_LPAD, NEXT_SB_PAD + last_top,
		NEXT_SB_BTN_WIDTH, r->scrollBar.top - last_top);
	}

	if (r->scrollBar.bot < last_bot || !update) {
	    /*
	    ** Area above the buttons but below the scroller
	    */
	    XFillRectangle(r->Xdisplay, r->scrollBar.win, TILEGC,
		NEXT_SB_LPAD, r->scrollBar.bot + NEXT_SB_PAD,
		NEXT_SB_BTN_WIDTH, (last_bot - r->scrollBar.bot));
	}


	/*
	** Area of the scroller
	*/
	{
	    CHOOSE_GC_FG(r, r->scrollBar.next_bg);
	    XFillRectangle(r->Xdisplay, r->scrollBar.win,
		r->scrollBar.gc,
		NEXT_SB_LPAD, r->scrollBar.top + NEXT_SB_PAD,
		NEXT_SB_BTN_WIDTH, scroller_len/*-NEXT_SB_BTN_HEIGHT*/);
	}


	/*
	** Here comes the dimple in the scroller
	*/
	CHOOSE_GC_FG(r, r->scrollBar.next_white);
	XCopyArea(r->Xdisplay, DIMPLE, r->scrollBar.win,
		r->scrollBar.gc, 0, 0,
		DIMPLE_WIDTH, DIMPLE_HEIGHT,
		(SB_WIDTH_NEXT - DIMPLE_WIDTH) / 2,
		r->scrollBar.top + NEXT_BEVEL_ULEFT_WIDTH +
		(scroller_len - DIMPLE_HEIGHT) / 2);

	/*
	** Bevel around the scroller
	*/
	next_draw_bevel(r, r->scrollBar.win, NEXT_BEVEL_X,
	    r->scrollBar.top + NEXT_SB_PAD, NEXT_SB_BTN_WIDTH,
	    scroller_len);
	/*
	** Bevel around the buttons
	*/
	next_draw_bevel(r, r->scrollBar.win, NEXT_BEVEL_X,
	    height - NEXT_SB_DBTN_HEIGHT, NEXT_SB_BTN_WIDTH,
	    NEXT_SB_BTN_HEIGHT);
	next_draw_bevel(r, r->scrollBar.win, NEXT_BEVEL_X,
	    height - NEXT_SB_SBTN_HEIGHT, NEXT_SB_BTN_WIDTH,
	    NEXT_SB_BTN_HEIGHT);

	/*
	** Top button
	*/
	CHOOSE_GC_FG(r, r->scrollBar.next_white);
	s = (scrollbar_isUp()) ? HIUPARROW : UPARROW;
	XCopyArea(r->Xdisplay, s, r->scrollBar.win, r->scrollBar.gc,
	    0, 0,
	    ARROW_WIDTH, ARROW_HEIGHT, NEXT_BTN_FACE_X,
	    height-NEXT_SB_DBTN_HEIGHT+NEXT_BEVEL_ULEFT_WIDTH);

	/*
	** Bottom button
	*/
	s = (scrollbar_isDn()) ? HIDNARROW : DNARROW;
	XCopyArea(r->Xdisplay, s, r->scrollBar.win, r->scrollBar.gc,
	    0, 0,
	    ARROW_WIDTH, ARROW_HEIGHT, NEXT_BTN_FACE_X,
	    height-NEXT_SB_SBTN_HEIGHT+NEXT_BEVEL_ULEFT_WIDTH);
    }

    return 1;
}

#endif	/* NEXT_SCROLLBAR */
#endif	/* HAVE_SCROLLBARS */
/*----------------------- end-of-file (C source) -----------------------*/
