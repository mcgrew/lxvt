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
