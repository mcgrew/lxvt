/*--------------------------------*-C-*---------------------------------*
 * File:    transparent.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 2004-2005   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 * Copyright (c) 2005        Gautam Iyer <gi1242@users.sourceforge.net>
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


/* ------------------- Begin Internal Prototypes ---------------------- */
#ifdef TINTING_SUPPORT
# ifdef HAVE_LIBXRENDER
void		xrenderShadeIntersect	     (rxvt_t*, Picture, unsigned long,
					      int, int, int, unsigned, unsigned,
					      int, int, unsigned, unsigned);
# else
static void     shade_ximage		     (rxvt_t*, XImage*);
# endif
#endif

/* -------------------------- End Internal Prototypes ----------------------- */

/* {{{1 General functions */
/* {{{2 rxvt_set_opacity(r) */
/* INTPROTO */
int
rxvt_set_opacity (rxvt_t* r)
{
    int		    k;
    unsigned int    n;
    Window	    wintree[PARENT_NUMBER];
    Window	    root;
    Window*	    list;
    CARD32	    opacity;


    /* do not set opacity */
    if (IS_NULL(r->h->rs[Rs_opacity]) ||
	NOT_ATOM(r->h->xa[XA_NET_WM_WINDOW_OPACITY]))
	return 0;


    opacity = (CARD32) (r->TermWin.opacity * (0xffffffff / 100));

    /*
    ** look for parent tree of top level window because the window
    ** manager may embed our window into some frame windows
    */
    wintree[0] = r->TermWin.parent;
    for (k = 1; k < PARENT_NUMBER; k++)
    {
	XQueryTree (r->Xdisplay, wintree[k-1], &root,
	    &(wintree[k]), &list, &n);
	XFree (list);
	if (wintree[k] == XROOT)
	    break;
    }

    /* Set opacity for all windows */
    if (k != PARENT_NUMBER)
    {
	while (k-- > 0)
	    XChangeProperty (r->Xdisplay, wintree[k],
		r->h->xa[XA_NET_WM_WINDOW_OPACITY], XA_CARDINAL, 32,
		PropModeReplace, (unsigned char*) &opacity, 1L);
    }

    XSync(r->Xdisplay, False);

    return 1;
}

/* {{{2 rxvt_process_reparentnotify( r, ev) */
/* EXTPROTO */
void
rxvt_process_reparentnotify (rxvt_t* r, XEvent* ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TRANSPARENT, "ReparentNotify event\n"));
    
    rxvt_set_opacity (r);

}

/* {{{1 Tinting functions */
#ifdef TINTING_SUPPORT
# ifdef HAVE_LIBXRENDER
/* {{{2 XRender tinting functions */
/* {{{3 xrenderShadeParentPixmap( r, pmap, nx, ny, nw, nh, shadeSubwin)
 *
 * Shade the pixmap as though it were the parent window's pixmap. The
 * coordinates nx, ny, nw, nh determine the location in pmap that needs to be
 * shaded (e.g. if the window is partially off screen, we only need to shade the
 * portion of pmap that's on screen).
 *
 * If shadeSubwin is true, then areas corresponding to the tab / scroll / menu
 * bar are tinted with their respective background colors.
 *
 * pmap should have dimensions equal to the parent window.
 */
/* EXTPROTO */
void
xrenderShadeParentPixmap( rxvt_t *r, Pixmap pmap,
	int nx, int ny, unsigned nw, unsigned nh,
	Bool shadeSubwin)
{
    int		    rx = 0, ry = 0;	    /* coords of tabbar etc */
    unsigned	    rw = TWIN_WIDTH (r),    /* dims of tabbar etc */
		    rh = TWIN_HEIGHT (r);

    Picture			pic;
    XRenderPictFormat		*format;
    XRenderPictureAttributes	attrs;


    format = XRenderFindVisualFormat( r->Xdisplay, XVISUAL);
    pic = XRenderCreatePicture( r->Xdisplay, pmap, format, 0, &attrs);

    if( shadeSubwin )
    {
# ifdef HAVE_MENUBAR
	if( rxvt_menubar_visible( r ) )
	{
	    /* Shade menubar */
	    if (ISSET_OPTION(r, Opt_transparent_menubar))
		xrenderShadeIntersect( r, pic, r->menuBar.bg, r->TermWin.shade,
			nx, ny, nw, nh,
			0, 0, rw, rxvt_menubar_height(r));

	    /* Reset coordinates to shade tabbar / main window. */
	    ry += rxvt_menubar_height (r);
	    rh -= rxvt_menubar_height (r);
	}
# endif

#ifdef HAVE_TABS
	if( rxvt_tabbar_visible( r ) )
	{
	    /* Shade tabbar */
	    if (ISSET_OPTION(r, Opt_transparent_tabbar))
		xrenderShadeIntersect( r, pic, r->tabBar.ibg, r->TermWin.shade,
			nx, ny, nw, nh,
			0, ry,
			rw, rxvt_tabbar_height( r ));
	    /* Reset coordinates to shade main window. */
	    rh -= rxvt_tabbar_height( r );
		ry += rxvt_tabbar_height( r );
	}
#endif

# ifdef HAVE_SCROLLBARS
	if( rxvt_scrollbar_visible( r ) )
	{
	    /* Shade scrollbar */
	    if (ISSET_OPTION(r, Opt_transparent_scrollbar))
		xrenderShadeIntersect( r, pic, rxvt_scrollbar_bg(r),
			r->TermWin.shade,
			nx, ny, nw, nh,
			ISSET_OPTION(r, Opt_scrollBar_right) ? VT_WIDTH(r) : 0, ry,
			rxvt_scrollbar_width(r), VT_HEIGHT(r));

	    /* Reset coordinates to shade main window. */
	    rw -= rxvt_scrollbar_width(r);
	    if( NOTSET_OPTION(r, Opt_scrollBar_right) )
		rx += rxvt_scrollbar_width(r);
	}
# endif
    }

    /* Shade root background. */
    xrenderShadeIntersect( r, pic,
	    ISSET_PIXCOLOR( r->h, Color_tint ) ?
		    r->pixColorsFocus[Color_tint] : VTBG(r),
	    r->TermWin.shade,
	    nx, ny, nw, nh,
	    rx, ry, rw, rh);

    /*
     * Free everything and get lost.
     */
    XRenderFreePicture( r->Xdisplay, pic);
}

/* {{{3 xrenderShadeIntersect( r, pic, pixel, shade, (rect1), (rect2))
 * Shade the intersection of two rectangles
 */
/* INTPROTO */
void
xrenderShadeIntersect( rxvt_t *r, Picture pic, unsigned long pixel, int shade,
	int x1, int y1, unsigned w1, unsigned h1,
	int x2, int y2, unsigned w2, unsigned h2)
{
    int		x, y;
    unsigned	w, h;

    XColor	    color;
    XRenderColor    trans_color;


    if( x1 + w1 < x2 || x2 + w2 < x1 ||
	    y1 + h1 < y2 || y2 + h2 < y1)
	return;	/* disjoint */

    /*
     * Get intersection in (x, y, w, h)
     */
    x = (x1 < x2) ? x2 : x1;
    y = (y1 < y2) ? y2 : y1;
    w = ((x1 + w1 < x2 + w2) ? x1 + w1 : x2 + w2) - x;
    h = ((y1 + h1 < y2 + h2) ? y1 + h1 : y2 + h2) - y;

    /*
     * Get XRenderColor from pixel and shade.
     */
    color.pixel = pixel;
    XQueryColor (r->Xdisplay, XCMAP, &color);

    trans_color.red	= color.red;
    trans_color.green	= color.green;
    trans_color.blue	= color.blue;
    trans_color.alpha	= ( (100 - shade) * (0xffff / 100) );

    /*
     * Fill translucent. Don't know if PictOpOver is the best operation to use
     * or not ...
     */
    XRenderFillRectangle( r->Xdisplay, PictOpOver, pic, &trans_color,
	    x, y, w, h);
}

/* {{{3 Unimplemented stuff */
#if 0
int
xrenderCopyShadedPixmap( rxvt_t *r, Drawable src, Drawable dst,
	XRenderColor *color, int x, int y, unsigned width, unsigned height,
	int dx, dy)
{
}
#endif
/* }}}2 */
# else /* HAVE_LIBXRENDER */
/* {{{2 Fast tinting functions */
/* {{{3 rxvt_shade_pixmap(r, src, (rect)) */
/* EXTPROTO */
void
rxvt_shade_pixmap (rxvt_t* r, Drawable src,
	int sx, int sy, unsigned nw, unsigned nh)
{
    XImage*	ximage;

    GC gc = XCreateGC (r->Xdisplay, src, 0UL, 0);

    ximage = XGetImage(r->Xdisplay, src, sx, sy, nw, nh,
		AllPlanes, ZPixmap);
    if (NULL != ximage)
    {
	shade_ximage (r, ximage);

	XPutImage(r->Xdisplay, src, gc, ximage, 0, 0, sx, sy, nw, nh);
	XDestroyImage (ximage);
    }

    XFreeGC( r->Xdisplay, gc);
}


/* {{{3 shade_ximage(r, image)
 *
 * Tint XImage with color / degree from options.
 */
/* INTPROTO */
static void
shade_ximage (rxvt_t* r, XImage* srcImage)
{
    int		    sh_r, sh_g, sh_b;
    uint32_t	    mask_r, mask_g, mask_b;
    uint32_t	    *lookup, *lookup_r, *lookup_g, *lookup_b;
    unsigned int    lower_lim_r, lower_lim_g, lower_lim_b;
    unsigned int    upper_lim_r, upper_lim_g, upper_lim_b;
    int i;
    Visual*	    visual = XVISUAL;

    XColor	    color;

    /* Former arguments */
    int		    shade, rm, gm, bm;


    if (ISSET_PIXCOLOR (r->h, Color_tint) && r->h->rs[Rs_shade])
    {
	color.pixel = r->pixColorsFocus[Color_tint];
	XQueryColor (r->Xdisplay, XCMAP, &color);

	shade = r->TermWin.shade;
    }
    else return;	    /* No shading defined */

    rm = color.red;
    gm = color.green;
    bm = color.blue;


    if (100 == shade)	    /* no need to shade */
	return;

    if( visual->class != TrueColor || srcImage->format != ZPixmap )
	return ;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_TRANSPARENT, "shade background image\n"));

    /* for convenience */
    mask_r = visual->red_mask;
    mask_g = visual->green_mask;
    mask_b = visual->blue_mask;

    /* boring lookup table pre-initialization */
    switch (srcImage->bits_per_pixel)
    {
	case 15:
	    if ((mask_r != 0x7c00) ||
		(mask_g != 0x03e0) ||
		(mask_b != 0x001f))
		return;
	    lookup = (uint32_t *) rxvt_malloc (sizeof (uint32_t)*(32+32+32));
	    lookup_r = lookup;
	    lookup_g = lookup+32;
	    lookup_b = lookup+32+32;
	    sh_r = 10;
	    sh_g = 5;
	    sh_b = 0;
	    break;

	case 16:
	    if ((mask_r != 0xf800) ||
		(mask_g != 0x07e0) ||
		(mask_b != 0x001f))
		return;
	    lookup = (uint32_t *) rxvt_malloc (sizeof (uint32_t)*(32+64+32));
	    lookup_r = lookup;
	    lookup_g = lookup+32;
	    lookup_b = lookup+32+64;
	    sh_r = 11;
	    sh_g = 5;
	    sh_b = 0;
	    break;

	case 24:
	    if ((mask_r != 0xff0000) ||
		(mask_g != 0x00ff00) ||
		(mask_b != 0x0000ff))
		return;
	    lookup = (uint32_t *) rxvt_malloc (sizeof (uint32_t)*(256+256+256));
	    lookup_r = lookup;
	    lookup_g = lookup+256;
	    lookup_b = lookup+256+256;
	    sh_r = 16;
	    sh_g = 8;
	    sh_b = 0;
	    break;

	case 32:
	    if ((mask_r != 0xff0000) ||
		(mask_g != 0x00ff00) ||
		(mask_b != 0x0000ff))
	    return;
	    lookup = (uint32_t *) rxvt_malloc (sizeof (uint32_t)*(256+256+256));
	    lookup_r = lookup;
	    lookup_g = lookup+256;
	    lookup_b = lookup+256+256;
	    sh_r = 16;
	    sh_g = 8;
	    sh_b = 0;
	    break;

	default:
	    return; /* we do not support this color depth */
    }

    /* prepare limits for color transformation (each channel is
    ** handled separately) */
    if (shade < 0)
    {
	shade = -shade;
	if (shade < 0)
	    shade = 0;
	if (shade > 100)
	    shade = 100;

	lower_lim_r = 65535-rm;
	lower_lim_g = 65535-gm;
	lower_lim_b = 65535-bm;

	lower_lim_r = 65535-(unsigned int)(((uint32_t)lower_lim_r)*((uint32_t)shade)/100);
	lower_lim_g = 65535-(unsigned int)(((uint32_t)lower_lim_g)*((uint32_t)shade)/100);
	lower_lim_b = 65535-(unsigned int)(((uint32_t)lower_lim_b)*((uint32_t)shade)/100);

	upper_lim_r = upper_lim_g = upper_lim_b = 65535;
    }
    else
    {
	if (shade < 0)
	    shade = 0;
	if (shade > 100)
	    shade = 100;

	lower_lim_r = lower_lim_g = lower_lim_b = 0;

	upper_lim_r = (unsigned int)((((uint32_t)rm)*((uint32_t)shade))/100);
	upper_lim_g = (unsigned int)((((uint32_t)gm)*((uint32_t)shade))/100);
	upper_lim_b = (unsigned int)((((uint32_t)bm)*((uint32_t)shade))/100);
    }

    /* switch red and blue bytes if necessary, we need it for some
    ** weird XServers like XFree86 3.3.3.1 */
    if ((srcImage->bits_per_pixel == 24) && (mask_r >= 0xFF0000 ))
    {
	unsigned int tmp;

	tmp = lower_lim_r;
	lower_lim_r = lower_lim_b;
	lower_lim_b = tmp;

	tmp = upper_lim_r;
	upper_lim_r = upper_lim_b;
	upper_lim_b = tmp;
    }

    /* fill our lookup tables */
    for (i = 0; i <= mask_r>>sh_r; i++)
    {
	uint32_t tmp;
	tmp = ((uint32_t)i)*((uint32_t)(upper_lim_r-lower_lim_r));
	tmp += ((uint32_t)(mask_r>>sh_r))*((uint32_t)lower_lim_r);
	lookup_r[i] = (tmp/65535)<<sh_r;
    }
    for (i = 0; i <= mask_g>>sh_g; i++)
    {
	uint32_t tmp;
	tmp = ((uint32_t)i)*((uint32_t)(upper_lim_g-lower_lim_g));
	tmp += ((uint32_t)(mask_g>>sh_g))*((uint32_t)lower_lim_g);
	lookup_g[i] = (tmp/65535)<<sh_g;
    }
    for (i = 0; i <= mask_b>>sh_b; i++)
    {
	uint32_t tmp;
	tmp = ((uint32_t)i)*((uint32_t)(upper_lim_b-lower_lim_b));
	tmp += ((uint32_t)(mask_b>>sh_b))*((uint32_t)lower_lim_b);
	lookup_b[i] = (tmp/65535)<<sh_b;
    }

    /* apply table to input image (replacing colors by newly
    ** calculated ones) */
    switch (srcImage->bits_per_pixel)
    {
    case 15:
	{
	    unsigned short *p1, *pf, *p, *pl;
	    p1 = (unsigned short *) srcImage->data;
	    pf = (unsigned short *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
	    while (p1 < pf)
	    {
		p = p1;
		pl = p1 + srcImage->width;
		for (; p < pl; p++)
		{
		    *p = lookup_r[(*p & 0x7c00)>>10] |
			lookup_g[(*p & 0x03e0)>> 5] |
			lookup_b[(*p & 0x001f)];
		}
		p1 = (unsigned short *) ((char *) p1 + srcImage->bytes_per_line);
	    }
	    break;
	}
    case 16:
	{
	    unsigned short *p1, *pf, *p, *pl;
	    p1 = (unsigned short *) srcImage->data;
	    pf = (unsigned short *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
	    while (p1 < pf)
	    {
		p = p1;
		pl = p1 + srcImage->width;
		for (; p < pl; p++)
		{
		    *p = lookup_r[(*p & 0xf800)>>11] |
			lookup_g[(*p & 0x07e0)>> 5] |
			lookup_b[(*p & 0x001f)];
		}
		p1 = (unsigned short *) ((char *) p1 + srcImage->bytes_per_line);
	    }
	    break;
	}
    case 24:
	{
	    unsigned char *p1, *pf, *p, *pl;
	    p1 = (unsigned char *) srcImage->data;
	    pf = (unsigned char *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
	    while (p1 < pf)
	    {
		p = p1;
		pl = p1 + srcImage->width * 3;
		for (; p < pl; p += 3)
		{
		    p[0] = lookup_r[(p[0] & 0xff0000)>>16];
		    p[1] = lookup_r[(p[1] & 0x00ff00)>> 8];
		    p[2] = lookup_r[(p[2] & 0x0000ff)];
		}
		p1 = (unsigned char *) ((char *) p1 + srcImage->bytes_per_line);
	    }
	    break;
	}
    case 32:
	{
	    uint32_t *p1, *pf, *p, *pl;
	    p1 = (uint32_t *) srcImage->data;
	    pf = (uint32_t *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);

	    while (p1 < pf)
	    {
		p = p1;
		pl = p1 + srcImage->width;
		for (; p < pl; p++)
		{
		    *p = lookup_r[(*p & 0xff0000)>>16] |
			lookup_g[(*p & 0x00ff00)>> 8] |
			lookup_b[(*p & 0x0000ff)] |
			(*p & ~0xffffff);
		}
		p1 = (uint32_t *) ((char *) p1 + srcImage->bytes_per_line);
	    }
	    break;
	}
    }

    rxvt_free (lookup);
}
/* }}}2 */
# endif	 /* HAVE_LIBXRENDER */
#endif	/* TINTING_SUPPORT */
