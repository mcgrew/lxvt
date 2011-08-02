/*--------------------------------*-C-*---------------------------------*
 * File:    command.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1992        John Bovey <jdb@ukc.ac.uk>
 * Copyright (c) 1994        Robert Nation <nation@rocket.sanders.lockheed.com>
 * Copyright (c) 1995        Garrett D'Amore <garrett@netcom.com>
 * Copyright (c) 1995        Steven Hirsch <hirsch@emba.uvm.edu>
 * Copyright (c) 1995        Jakub Jelinek <jj@gnu.ai.mit.edu>
 * Copyright (c) 1997        MJ Olesen <olesen@me.queensu.ca>
 * Copyright (c) 1997        Raul Garcia Garcia <rgg@tid.es>
 * Copyright (c) 1997,1998   Oezguer Kesim <kesim@math.fu-berlin.de>
 * Copyright (c) 1998-2001   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 1998        Alfredo K. Kojima <kojima@windowmaker.org>
 * Copyright (c) 2001        Marius Gedminas <marius.gedminas@uosis.mif.vu.lt>
 * Copyright (c) 2003        Rob McMullen <robm@flipturn.org>
 * Copyright (c) 2004        Terry Griffin <griffint@pobox.com>
 * Copyright (c) 2005        Johann 'Mykraverk' Oskarsson
 *				<johann@myrkraverk.com>
 * Copyright (c) 2004-2006   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 * Copyright (c) 2005-2006   Gautam Iyer <gi1242@users.sourceforge.net>
 * Copyright (C) 2008		  Jehan Hysseo <hysseo@users.sourceforge.net>
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


#ifdef USE_DEADKEY
/*
** Deadkey sequences table.
*/
typedef struct _DeadKeyChar {
    KeySym	ks;	/* keysym */
    KeySym	dk;	/* accent */
    KeySym	ach;	/* accent keysym */
} DeadKeyChar;

static DeadKeyChar  dkc_tab[] = {
    {XK_A, XK_grave,	    XK_Agrave},
    {XK_A, XK_acute,	    XK_Aacute},
    {XK_A, XK_apostrophe,   XK_Aacute},
    {XK_A, XK_asciicircum,  XK_Acircumflex},
    {XK_A, XK_asciitilde,   XK_Atilde},
    {XK_A, XK_quotedbl,	    XK_Adiaeresis},

    {XK_a, XK_grave,	    XK_agrave},
    {XK_a, XK_acute,	    XK_aacute},
    {XK_a, XK_apostrophe,   XK_aacute},
    {XK_a, XK_asciicircum,  XK_acircumflex},
    {XK_a, XK_asciitilde,   XK_atilde},
    {XK_a, XK_quotedbl,	    XK_adiaeresis},

    {XK_C, XK_grave,	    XK_Ccedilla},
    {XK_C, XK_acute,	    XK_Ccedilla},

    {XK_c, XK_grave,	    XK_ccedilla},
    {XK_c, XK_acute,	    XK_ccedilla},

    {XK_E, XK_grave,	    XK_Egrave},
    {XK_E, XK_acute,	    XK_Eacute},
    {XK_E, XK_apostrophe,   XK_Eacute},
    {XK_E, XK_asciicircum,  XK_Ecircumflex},
    {XK_E, XK_quotedbl,	    XK_Ediaeresis},

    {XK_e, XK_grave,	    XK_egrave},
    {XK_e, XK_acute,	    XK_eacute},
    {XK_e, XK_apostrophe,   XK_eacute},
    {XK_e, XK_asciicircum,  XK_ecircumflex},
    {XK_e, XK_quotedbl,	    XK_ediaeresis},

    {XK_I, XK_grave,	    XK_Igrave},
    {XK_I, XK_acute,	    XK_Iacute},
    {XK_I, XK_apostrophe,   XK_Iacute},
    {XK_I, XK_asciicircum,  XK_Icircumflex},
    {XK_I, XK_quotedbl,	    XK_Idiaeresis},

    {XK_i, XK_grave,	    XK_igrave},
    {XK_i, XK_acute,	    XK_iacute},
    {XK_i, XK_apostrophe,   XK_iacute},
    {XK_i, XK_asciicircum,  XK_icircumflex},
    {XK_i, XK_quotedbl,	    XK_idiaeresis},

    {XK_N, XK_asciitilde,   XK_Ntilde},
    {XK_n, XK_asciitilde,   XK_ntilde},

    {XK_O, XK_grave,	    XK_Ograve},
    {XK_O, XK_acute,	    XK_Oacute},
    {XK_O, XK_apostrophe,   XK_Oacute},
    {XK_O, XK_asciicircum,  XK_Ocircumflex},
    {XK_O, XK_asciitilde,   XK_Otilde},
    {XK_O, XK_quotedbl,	    XK_Odiaeresis},

    {XK_o, XK_grave,	    XK_ograve},
    {XK_o, XK_acute,	    XK_oacute},
    {XK_o, XK_apostrophe,   XK_oacute},
    {XK_o, XK_asciicircum,  XK_ocircumflex},
    {XK_o, XK_asciitilde,   XK_otilde},
    {XK_o, XK_quotedbl,	    XK_odiaeresis},

    {XK_U, XK_grave,	    XK_Ugrave},
    {XK_U, XK_acute,	    XK_Uacute},
    {XK_U, XK_apostrophe,   XK_Uacute},
    {XK_U, XK_asciicircum,  XK_Ucircumflex},
    {XK_U, XK_quotedbl,	    XK_Udiaeresis},

    {XK_u, XK_grave,	    XK_ugrave},
    {XK_u, XK_acute,	    XK_uacute},
    {XK_u, XK_apostrophe,   XK_uacute},
    {XK_u, XK_asciicircum,  XK_ucircumflex},
    {XK_u, XK_quotedbl,	    XK_udiaeresis},

    {XK_Y, XK_acute,	    XK_Yacute},
    {XK_Y, XK_apostrophe,   XK_Yacute},

    {XK_y, XK_acute,	    XK_yacute},
    {XK_y, XK_apostrophe,   XK_yacute},
    {XK_y, XK_quotedbl,	    XK_ydiaeresis},

    {0, 0, 0},
};

# define DEADKEY_CHAR_NUMBER	    (int)((sizeof(dkc_tab) / sizeof(DeadKeyChar)))

#endif	/* USE_DEADKEY */



/*--------------------------------------------------------------------*
 *         BEGIN `INTERNAL' ROUTINE PROTOTYPES                        *
 *--------------------------------------------------------------------*/
void           rxvt_process_keypress         (rxvt_t*, XKeyEvent*);
void           rxvt_clean_cmd_page           (rxvt_t*);
int static inline rxvt_cmdbuf_has_input	     ( rxvt_t *r );
int            rxvt_find_cmd_child           (rxvt_t*);
void           rxvt_check_cmdbuf             (rxvt_t*);
int            rxvt_read_child_cmdfd         (rxvt_t*, unsigned int);
void           rxvt_process_children_cmdfd   (rxvt_t*, fd_set*);
int            rxvt_check_quick_timeout      (rxvt_t*);
int            rxvt_adjust_quick_timeout     (rxvt_t*, int, struct timeval*);
void	       rxvt_refresh_vtscr_if_needed  (rxvt_t*);
int            rxvt_cmd_getc                 (rxvt_t*);
#ifdef POINTER_BLANK
void           rxvt_pointer_blank            (rxvt_t*);
#endif
void           rxvt_mouse_report             (rxvt_t*, const XButtonEvent*);
void           rxvt_set_bg_focused           (rxvt_t*, Bool);
#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
void           rxvt_process_keyrelease       (rxvt_t*, XKeyEvent*);
#endif
void           rxvt_scrollbar_dispatcher     (rxvt_t*, XButtonEvent*);
void           rxvt_process_buttonpress      (rxvt_t*, XButtonEvent*);
#ifdef MOUSE_WHEEL
void           rxvt_process_wheel_button     (rxvt_t*, XButtonEvent*);
#endif
void           rxvt_process_buttonrelease    (rxvt_t*, XButtonEvent*);
void           rxvt_process_clientmessage    (rxvt_t*, XClientMessageEvent*);
void           rxvt_process_visibilitynotify (rxvt_t*, XVisibilityEvent*);
#ifdef MONITOR_ENTER_LEAVE
void           rxvt_process_enter            (rxvt_t*, XCrossingEvent*);
void           rxvt_process_leave            (rxvt_t*, XCrossingEvent*);
#endif
void	       rxvt_change_colors_on_focus	     (rxvt_t*);
void           rxvt_process_focus            (rxvt_t*, XFocusChangeEvent*);
int            rxvt_calc_colrow              (rxvt_t* r, unsigned int width, unsigned int height);
void           rxvt_resize_sub_windows       (rxvt_t* r);
void           rxvt_resize_on_configure      (rxvt_t* r, unsigned int width, unsigned int height);
#ifndef NO_FRILLS
Bool	       getWMStruts		     (Display *dpy, Window w, CARD32 *left, CARD32 *right, CARD32 *top, CARD32 *bottom);
#endif
void           rxvt_process_configurenotify  (rxvt_t*, XConfigureEvent*);
void           rxvt_process_selectionnotify  (rxvt_t*, XSelectionEvent*);
void           rxvt_process_propertynotify   (rxvt_t*, XEvent*);
void           rxvt_process_expose           (rxvt_t*, XEvent*);
void           rxvt_process_motionnotify     (rxvt_t*, XEvent*);
void           rxvt_process_x_event          (rxvt_t*, XEvent*);
void           rxvt_process_nonprinting      (rxvt_t*, unsigned char);
void           rxvt_process_escape_vt52      (rxvt_t*, unsigned char);
void           rxvt_process_escape_seq       (rxvt_t*);
void           rxvt_process_csi_seq          (rxvt_t*);
#ifndef NO_FRILLS
void           rxvt_process_window_ops       (rxvt_t*, const int*, unsigned int);
#endif
unsigned char* rxvt_get_to_st                (rxvt_t*, unsigned char*);
void           rxvt_process_dcs_seq          (rxvt_t*);
void           rxvt_process_osc_seq          (rxvt_t*);
void           rxvt_xwsh_seq                 (rxvt_t*, int, const char*);
void           rxvt_process_xwsh_seq         (rxvt_t*);
int            rxvt_privcases                (rxvt_t*, int, uint32_t);
void           rxvt_process_terminal_mode    (rxvt_t*, int, int, unsigned int, const int*);
void           rxvt_process_sgr_mode         (rxvt_t*, unsigned int, const int*);
void           rxvt_process_graphics         (rxvt_t*);
void	       rxvt_process_getc	     (rxvt_t*, unsigned char);
/*--------------------------------------------------------------------*
 *         END   `INTERNAL' ROUTINE PROTOTYPES                        *
 *--------------------------------------------------------------------*/



/* {{{ Process keysyms between 0xff00 and 0xffff */
/*
 * SET_TILDE_KEY_SEQ( kbuf, str): Copies a escape sequences (ending with ~) into
 * kbuf.
 *
 * Rxvt style modifiers: The trailing tilde is replaced with $, ^, or @
 * depending on the modifier pressed:
 *
 *	No modifier	~
 *	Shift		$
 *	Ctrl		^
 *	Ctrl+Shift	@
 *
 * The meta modifier is ignored.
 *
 * Xterm style modifiers (if XTERM_KEYS is defined): Adds a parameter ";%d"
 * before the trailing "~" if a modifier is pressed:
 *
 *	None                  Nothing is added.
 *	Shift                 2 = 1(None)+1(Shift)
 *	Alt                   3 = 1(None)+2(Alt)
 *	Alt+Shift             4 = 1(None)+1(Shift)+2(Alt)
 *	Ctrl                  5 = 1(None)+4(Ctrl)
 *	Ctrl+Shift            6 = 1(None)+1(Shift)+4(Ctrl)
 *	Ctrl+Alt              7 = 1(None)+2(Alt)+4(Ctrl)
 *	Ctrl+Alt+Shift        8 = 1(None)+1(Shift)+2(Alt)+4(Ctrl)
 *
 * This is better because DEC sequences that don't end in a tilde can also
 * contain a modifier parameter / number. NOTE: Xterm also has a bunch of
 * sequences when Meta is pressed, which we don't implement.
 * 	
 */
#ifdef XTERM_KEYS
# define SET_TILDE_KEY_SEQ( kbuf, str )	\
    set_xterm_key_seq( (kbuf), str, str ";%d%c", '~', ctrl, meta, shft )
#else
# define SET_TILDE_KEY_SEQ( kbuf, str )	\
    sprintf( kbuf, str "%c", shft ? (ctrl ? '@' : '$') : (ctrl ? '^' : '~') )
#endif

/*
 * Copies sequence into kbuf.
 *
 * If no modifiers (ctrl, meta, shft) are pressed, then the character suffix is
 * appended to umod_prefix, and copied into kbuf.
 *
 * Otherwise, mod_format is sprintf'ed into kbuf. The third arg to sprintf is
 * the modifier number (computed from which modifier is pressed in the same way
 * as xterm does). The fourth argument is the character suffix.
 */
/* NOPROTO */
void
set_xterm_key_seq( unsigned char *kbuf, const char *umod_prefix,
	const char *mod_prefix_format, char suffix,
	int ctrl, int meta, int shft)
{
    if( ctrl || meta || shft )
	sprintf( (char*) kbuf, mod_prefix_format,
		1 + (shft ? 1 : 0) + (meta ? 2 : 0) + (ctrl ? 4 : 0),
		suffix );
    else
	sprintf( (char*) kbuf, "%s%c", umod_prefix, suffix );
}



/* INTPRO */
int
rxvt_0xffxx_keypress (rxvt_t* r, KeySym keysym,
	int ctrl, int meta, int shft, unsigned char* kbuf)
{
    /*
     * 2006-04-08 gi1242:  The KeyPad is treated as follows:
     *
     * 	    1. If NumLock is on: Unmodified keys send the regular char. Modified
     * 	       keys send the DEC sequence in private mode, and the regular char
     * 	       in normal model.
     *
     * 	    2. If NumLock is off: Keys always send the DEC private mode
     * 	       sequence.
     */

    /*
     * B: beginning of a {} body
     */
    unsigned int	newlen = 1;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "0xffxx_keypress: (C%d, M%d, S%d) %lx ...", ctrl, meta, shft, keysym));

    switch (keysym)
    {

#ifndef NO_BACKSPACE_KEY
	case XK_BackSpace:
	    {
		unsigned char *bsKbuf = kbuf;

		if (
			meta
# ifdef META8_OPTION
			&& (r->h->meta_char == C0_ESC)
# endif
		   )
		{
		    /* 7bit mode. Prefix with C0_ESC */
		    *(bsKbuf++)	= C0_ESC;
		    newlen++;

		    meta = 0;
		}

		if (ISSET_PMODE(r, PrivMode_HaveBackSpace))
		{
		    bsKbuf[0] = (!!(ISSET_PMODE(r,PrivMode_BackSpace))
				    ^ !!ctrl)
				? '\b' : '\177';
		    bsKbuf[1] = '\0';
		}
		else
		    /*
		     * If PrivMode_HaveBackSpace is unset, then h->key_backspace
		     * will be a one character string.
		     */
		    STRCPY( bsKbuf, r->h->key_backspace);

# ifdef META8_OPTION
		if( meta )  /* We must be in 8bit mode if meta is set here */
		    *bsKbuf |= r->h->meta_char;
# endif

# ifdef MULTICHAR_SET
		if (ISSET_OPTION(r, Opt_mc_hack) && PVTS(r)->screen.cur.col > 0)
		{
		    int		 col, row;

		    newlen = STRLEN(kbuf);
		    col = PVTS(r)->screen.cur.col - 1;
		    row = PVTS(r)->screen.cur.row + PVTS(r)->saveLines;
		    if (IS_MULTI2(PVTS(r)->screen.rend[row][col]))
			MEMMOVE(kbuf + newlen, kbuf, newlen + 1);
		}
# endif	/* MULTICHAR_SET */

		break;
	    }
#endif	/* !NO_BACKSPACE_KEY */



	case XK_Tab:
	    /*
	     * 2006-04-07 gi1242: Shift tab is sometimes sent as ISO_Left_Tab,
	     * which has keysym 0xfe20 (outside the 0xff00 -- 0xffff range). So
	     * we have to process shift tab elsewhere.
	     */
	    if (shft)
		STRCPY(kbuf, "\033[Z");
	    else
	    {
#ifdef CTRL_TAB_MAKES_META
		if (ctrl)
		    meta = 1;
#endif	/* CTRL_TAB_MAKES_META */
#ifdef MOD4_TAB_MAKES_META
		if (ev->state & Mod4Mask)
		    meta = 1;
#endif	/* MOD4_TAB_MAKES_META */
		newlen = 0;
	    }
	    break;

	case XK_Return:
	    newlen = 0;
#ifdef META8_OPTION
	    if (r->h->meta_char == 0x80)
		/*
		 * 2006-12-12 gi1242: In 8 bit mode, xterm sends 0x8d for
		 * Alt+enter. Enter sends 0x0d.
		 */
		kbuf[newlen++] = meta ? (r->h->meta_char|C0_CR) : C0_CR;
	    else
#endif	/* META8_OPTION */
	    {
		if( meta )
		    kbuf[newlen++] = C0_ESC;
		kbuf[newlen++] = C0_CR;
	    }
	    kbuf[newlen] = '\0';
	    break;

#ifdef XK_KP_Left
	case XK_KP_Up:	    /* \033Ox or standard */
	case XK_KP_Down:    /* \033Or or standard */
	case XK_KP_Right:   /* \033Ov or standard */
	case XK_KP_Left:    /* \033Ot or standard */
	    /*
	     * If numlock is on, we got here by pressing Shift + Keypad 8.
	     * Cancel the Shift, and send the DEC terminal sequence.
	     */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			"txvr"[keysym - XK_KP_Left], ctrl, meta, shft );
		break;
	    }

	    /* translate to std. cursor key */
	    keysym = XK_Left + (keysym - XK_KP_Left);

	    /* FALLTHROUGH */
#endif	/* XK_KP_Left */

	case XK_Up:	/* "\033[A" */
	case XK_Down:	/* "\033[B" */
	case XK_Right:	/* "\033[C" */
	case XK_Left:	/* "\033[D" */
#ifdef XTERM_KEYS
	    /*
	     * Modified cursor keys are \e[1;%d%c, where %d is the modifier
	     * number and %c is D, A, C or B for Up, Dn, Left / Right
	     * resply.
	     */
	    set_xterm_key_seq( kbuf, "\033[", "\033[1;%d%c",
		    "DACB"[keysym - XK_Left], ctrl, meta, shft);

	    if (ISSET_PMODE(r, PrivMode_aplCUR) &&
		!ctrl && !meta && !shft)
		/*
		 * 2006-04-07 gi1242 XXX: Should we ignore the modifiers in
		 * the private mode?
		 */
		kbuf[1] = 'O';
#else
	    STRCPY(kbuf, "\033[Z");
	    kbuf[2] = ("DACB"[keysym - XK_Left]);
	    /* do Shift first */
	    if (shft)
		kbuf[2] = ("dacb"[keysym - XK_Left]);
	    else if (ctrl)
	    {
		kbuf[1] = 'O';
		kbuf[2] = ("dacb"[keysym - XK_Left]);
	    }
	    else if (ISSET_PMODE(r, PrivMode_aplCUR))
		kbuf[1] = 'O';
#endif

#ifdef MULTICHAR_SET
	    if (ISSET_OPTION(r, Opt_mc_hack))
	    {
		/*
		 * If we're on a multibyte char, and we move left / right, then
		 * duplicate the cursor key string.
		 */
		int	     col, row, m;

		col = PVTS(r)->screen.cur.col;
		row = PVTS(r)->screen.cur.row + PVTS(r)->saveLines;
		m = 0;
		if (keysym == XK_Right &&
		    IS_MULTI1(PVTS(r)->screen.rend[row][col]))
		    m = 1;
		else if (keysym == XK_Left)
		{
		    if (col > 0)
		    {
			if (IS_MULTI2(PVTS(r)->screen.rend[row][col - 1]))
			    m = 1;
		    }
		    else if (PVTS(r)->screen.cur.row > 0)
		    {
			col = PVTS(r)->screen.tlen[--row];
			if (col == -1)
			    col = r->TermWin.ncol - 1;
			else
			    col--;
			if (col > 0 &&
			    IS_MULTI2(PVTS(r)->screen.rend[row][col]))
			    m = 1;
		    }
		}

		if (m)
		{
		    /* Duplicate the escape sequence in kbuf */
		    int len = STRLEN( kbuf );

		    MEMMOVE( kbuf + len, kbuf, len + 1);
		}
	    }
#endif	/* MULTICHAR_SET */

	    break;


# ifdef XK_KP_Prior
	case XK_KP_Prior:	/* \033[Oy */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'y', ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
# endif	/* XK_KP_Prior */
	case XK_Prior:
	    SET_TILDE_KEY_SEQ( kbuf, "\033[5");
	    break;


# ifdef XK_KP_Next
	case XK_KP_Next:	/* \033Os */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			's', ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

# endif	/* XK_KP_Next */
	case XK_Next:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[6");
	    break;


	case XK_KP_Enter:   /* \033OM */

	    if (ISSET_PMODE(r, PrivMode_aplKP) &&
		(!r->numlock_state || ctrl || meta || shft))
	    {
#ifdef NUMLOCK_ALWAYS_IGNORES_SHIFT
		if( r->numlock_state ) shft = 0;
#endif
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'M',
			ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = '\r';
		kbuf[1] = '\0';
	    }
	    break;

#ifdef XK_KP_Begin
	case XK_KP_Begin:   /* \033Ou (This is keypad 5) */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'u',
			ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = '5';
		kbuf[1] = '\0';
	    }

	    break;
#endif	/* XK_KP_Begin */


#ifdef XK_KP_Insert
	case XK_KP_Insert:  /* \033Op */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'p',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
#endif /* XK_KP_Insert */

	case XK_Insert:
	    SET_TILDE_KEY_SEQ( kbuf, "\033[2" );
	    break;


#ifdef XK_KP_Delete
	case XK_KP_Delete:  /* \033On */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'n',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */
#endif

	case XK_Delete:
#ifndef NO_DELETE_KEY
	    STRCPY( kbuf, r->h->key_delete);
#else
	    SET_TILDE_KEY_SEQ( kbuf, "\033[3" );
#endif

#ifdef MULTICHAR_SET
	    if (ISSET_OPTION(r, Opt_mc_hack))
	    {
		int	     col, row;

		newlen = STRLEN(kbuf);
		col = PVTS(r)->screen.cur.col;
		row = PVTS(r)->screen.cur.row + PVTS(r)->saveLines;

		if (IS_MULTI1(PVTS(r)->screen.rend[row][col]))
		    MEMMOVE(kbuf + newlen, kbuf, newlen + 1);
	    }
#endif	/* MULTICHAR_SET */
	    break;


	case XK_KP_Multiply:	/* "\033Oj" : "*" */
	case XK_KP_Add:		/* "\033Ok" : "+" */
	case XK_KP_Separator:	/* "\033Ol" : "," */
	case XK_KP_Subtract:	/* "\033Om" : "-" */
	case XK_KP_Divide:	/* "\033Oo" : "/" */

	case XK_KP_Decimal:	/* "\033On" : "." */
	case XK_KP_0:		/* "\033Op" : "0" */
	case XK_KP_1:		/* "\033Oq" : "1" */
	case XK_KP_2:		/* "\033Or" : "2" */
	case XK_KP_3:		/* "\033Os" : "3" */
	case XK_KP_4:		/* "\033Ot" : "4" */
	case XK_KP_5:		/* "\033Ou" : "5" */
	case XK_KP_6:		/* "\033Ov" : "6" */
	case XK_KP_7:		/* "\033Ow" : "7" */
	case XK_KP_8:		/* "\033Ox" : "8" */
	case XK_KP_9:		/* "\033Oy" : "9" */

	    /*
	     * If numlock is on, and no modifiers are presed, then we should
	     * send the actual number key. If modifiers are pressed, send the
	     * DEC sequence (after canceling shift ofcourse).
	     *
	     * If numlock is off, then send the DEC terminal sequence.
	     */
	    if (ISSET_PMODE(r, PrivMode_aplKP) &&
		(!r->numlock_state || ctrl || meta || shft))
	    {
		if(
			r->numlock_state
#ifndef NUMLOCK_ALWAYS_IGNORES_SHIFT
			&& keysym >= XK_KP_Decimal && keysym <= XK_KP_9
#endif
		  )
		    /*
		     * Cancel shift if numlock is pressed, but only cancel it
		     * for keys where shift has some meaning: 0-9 and Decimal.
		     * If NUMLOCK_ALWAYS_IGNORES_SHIFT is defined, then shift is
		     * always cancelled when numlock is on.
		     */
		    shft = 0;

		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'j' + (keysym - XK_KP_Multiply), ctrl, meta, shft );
	    }
	    else
	    {
		kbuf[0] = ('*' + (keysym - XK_KP_Multiply));
		kbuf[1] = '\0';
	    }
	    break;

	case XK_Find:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[1");
	    break;

#ifdef DXK_Remove	/* support for DEC remove like key */
	case DXK_Remove:
	    /* FALLTHROUGH */
#endif	/* DXK_Remove */
	case XK_Execute:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[3");
	    break;

	case XK_Select:
	    SET_TILDE_KEY_SEQ(kbuf, "\033[4");
	    break;

#ifdef XK_KP_End
	case XK_KP_End:	/* \033Oq */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'q',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

#endif	/* XK_KP_End */

	case XK_End:
	    if (ISSET_OPTION(r, Opt2_linuxHomeEndKey))
		SET_TILDE_KEY_SEQ( kbuf, KS_END_LINUX);
	    else
		SET_TILDE_KEY_SEQ( kbuf, KS_END);
	    break;

#ifdef XK_KP_Home
	case XK_KP_Home: /* \033Ow */
	    if( r->numlock_state ) shft = 0;

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c", 'w',
			ctrl, meta, shft );
		break;
	    }
	    /* FALLTHROUGH */

#endif	/* XK_KP_Home */
	case XK_Home:
	    if (ISSET_OPTION(r, Opt2_linuxHomeEndKey))
		SET_TILDE_KEY_SEQ( kbuf, KS_HOME_LINUX);
	    else
		SET_TILDE_KEY_SEQ( kbuf, KS_HOME);
	    break;

#define FKEY_SEQ_START( n, fkey)    \
	sprintf( (char*) kbuf, "\033[%2d", (n) + (int) (keysym - (fkey)) );


	case XK_KP_F1:	/* "\033OP" */
	case XK_KP_F2:	/* "\033OQ" */
	case XK_KP_F3:	/* "\033OR" */
	case XK_KP_F4:	/* "\033OS" */
	    /* XXX 2006-04-08 gi1242: Should we cancel shift here? */

	    if (ISSET_PMODE(r, PrivMode_aplKP))
	    {
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'P' + (keysym - XK_KP_F1), ctrl, meta, shft);
		break;
	    }
	    else
		keysym = XK_F1 + (keysym - XK_KP_F1);
	    /* FALLTHROUGH */

	case XK_F1: /* "\033[11~" */
	case XK_F2: /* "\033[12~" */
	case XK_F3: /* "\033[13~" */
	case XK_F4: /* "\033[14~" */
	    if (TERMENV_XTERM == PVTS(r)->termenv)
	    {
		/* Xterm sends \eOP, \eOQ, \eOR, \eOS for f1 -- f4 */
		set_xterm_key_seq( kbuf, "\033O", "\033O%d%c",
			'P' + (keysym - XK_F1), ctrl, meta, shft);
		break;
	    }
	    /* else FALL THROUGH */
	case XK_F5: /* "\033[15~" */
	    FKEY_SEQ_START( 11, XK_F1);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F6:	/* "\033[17~" */
	case XK_F7:	/* "\033[18~" */
	case XK_F8:	/* "\033[19~" */
	case XK_F9:	/* "\033[20~" */
	case XK_F10:	/* "\033[21~" */
	    FKEY_SEQ_START(17, XK_F6);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F11:	/* "\033[23~" */
	case XK_F12:	/* "\033[24~" */
	case XK_F13:	/* "\033[25~" */
	case XK_F14:	/* "\033[26~" */
	    FKEY_SEQ_START(23, XK_F11);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F15:	/* "\033[28~" */
	case XK_F16:	/* "\033[29~" */
	    FKEY_SEQ_START(28, XK_F15);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_Help:	/* "\033[28~" */
	    FKEY_SEQ_START(28, XK_Help);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_Menu:	/* "\033[29~" */
	    FKEY_SEQ_START(29, XK_Menu);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

	case XK_F17:	/* "\033[31~" */
	case XK_F18:	/* "\033[32~" */
	case XK_F19:	/* "\033[33~" */
	case XK_F20:	/* "\033[34~" */
	case XK_F21:	/* "\033[35~" */
	case XK_F22:	/* "\033[36~" */
	case XK_F23:	/* "\033[37~" */
	case XK_F24:	/* "\033[38~" */
	case XK_F25:	/* "\033[39~" */
	case XK_F26:	/* "\033[40~" */
	case XK_F27:	/* "\033[41~" */
	case XK_F28:	/* "\033[42~" */
	case XK_F29:	/* "\033[43~" */
	case XK_F30:	/* "\033[44~" */
	case XK_F31:	/* "\033[45~" */
	case XK_F32:	/* "\033[46~" */
	case XK_F33:	/* "\033[47~" */
	case XK_F34:	/* "\033[48~" */
	case XK_F35:	/* "\033[49~" */
	    FKEY_SEQ_START(31, XK_F17);
	    SET_TILDE_KEY_SEQ( kbuf + 4, "" );
	    break;

#undef FKEY_SEQ_START
	default:
	    newlen = 0;
	    break;

    }	/* switch (keysym) */


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Returning %d\n", newlen ? (int) STRLEN(kbuf) : -1 ));

    return newlen ? STRLEN(kbuf) : -1;
    /*
     * B: end of a {} body
     */
}
/* }}} */


/*{{{ Convert the keypress event into a string */
/* INTPROTO */
void
rxvt_process_keypress (rxvt_t* r, XKeyEvent *ev)
{
    int		    ctrl, meta, alt, shft, len;
    KeySym	    keysym;
#ifdef USE_DEADKEY
    static KeySym   accent = 0;
#endif	/* USE_DEADKEY */
#ifdef DEBUG
    static int	    debug_key = 1;  /* accessible by a debugger only */
#endif	/* DEBUG */
#ifdef USE_XIM
    int		    valid_keysym = 0;
#endif	/* USE_XIM */
    /*
    ** kbuf should be static in order to avoid performance penalty
    ** on allocation in the stack. And we only define it inside this
    ** function to avoid potential abuse of this buffer at somewhere
    ** else.
    */
    static unsigned char kbuf[KBUFSZ];	/* was r->h->kbuf */


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "KeyPress event\n"));
    shft = (ev->state & ShiftMask);
    ctrl = (ev->state & ControlMask);
    meta = (ev->state & r->h->ModMetaMask);
    alt = (ev->state & r->h->ModAltMask);

#if 0 /* {{{ Old numlock handling (conflicts with Xterm) */
    /*
     * use Num_Lock to toggle Keypad on/off.  If Num_Lock is off,
     * allow an escape sequence to toggle the Keypad. Always permit
     * `shift' to override the current setting
     */
    if (r->numlock_state || (ev->state & r->h->ModNumLockMask))
    {
	r->numlock_state = (ev->state & r->h->ModNumLockMask);
	PrivMode((!r->numlock_state), PrivMode_aplKP);
    }
#endif /*}}}*/

    /*
     * 2006-04-07 gi1242: Don't toggle the Keypad on pressing num lock. Only
     * store the numlock state so that we can cancel shift if numlock is
     * pressed, and we get an un-shifted keypad key.
     */
    r->numlock_state = ( ev->state & r->h->ModNumLockMask );

#ifdef USE_XIM
    if (NOT_NULL(r->h->Input_Context))
    {
	Status	      status_return;

	kbuf[0] = '\0';
	len = XmbLookupString(r->h->Input_Context, ev, (char *)kbuf,
		  KBUFSZ, &keysym, &status_return);
	valid_keysym = ((status_return == XLookupKeySym) ||
			(status_return == XLookupBoth));
    }
    else
    {
	valid_keysym = 1;
#endif	/* USE_XIM */

	/*
	******************************************************
	** Begin of part that is used when XIM is disabled
	*/
	len = XLookupString(ev, (char *)kbuf, KBUFSZ, &keysym, NULL);
	/*
	** map unmapped Latin[2-4]/Katakana/Arabic/Cyrillic/Greek
	** entries -> Latin1. good for installations with correct
	** fonts, but without XLOCALE
	*/
	if (!len)
	{
	    if ((keysym >= 0x0100) && (keysym < 0x0800))
	    {
		kbuf[0] = (keysym & 0xFF);
		kbuf[1] = '\0';
		len = 1;
	    }
	    else
		kbuf[0] = '\0';
	}
	/*
	** End of part that is used when XIM is disabled
	******************************************************
	*/

#ifdef USE_XIM
    }
#endif	/* USE_XIM */


#ifdef USE_DEADKEY
    if (0 != accent)
    {
	if (
		!ctrl && !meta /* NO ctrl or meta */
		&& (
		    XK_A == keysym || XK_a == keysym
		    || XK_C == keysym || XK_c == keysym
		    || XK_E == keysym || XK_e == keysym
		    || XK_I == keysym || XK_i == keysym
		    || XK_N == keysym || XK_n == keysym
		    || XK_O == keysym || XK_o == keysym
		    || XK_U == keysym || XK_u == keysym
		    || XK_Y == keysym || XK_y == keysym
		   )
	   )
	{
	    register int    idx;
	    KeySym  dk = 0;

	    /* dead key + space -> dead key itself */
	    switch (accent)
	    {
		case XK_dead_grave:	    /* ` */
		    dk = XK_grave;
		    break;

		case XK_dead_acute:	    /* ' */
		    dk = XK_acute;
		    break;

		case XK_dead_circumflex:    /* ^ */
		    dk = XK_asciicircum;
		    break;

		case XK_dead_diaeresis:	    /* " */
		    dk = XK_quotedbl;
		    break;

		case XK_dead_tilde:	    /* ~ */
		    dk = XK_asciitilde;
		    break;

		default:
		    assert(0);
	    }	/* switch(accent) */

	    for (idx = 0; idx < DEADKEY_CHAR_NUMBER; idx++)
	    {
		if (keysym == dkc_tab[idx].ks && dk == dkc_tab[idx].dk)
		{
		    kbuf[0] = (unsigned char) dkc_tab[idx].ach;
		    break;
		}
	    }
	    assert (0 != kbuf[0]);  /* impossible */

	    len = 1;
	    accent = 0;	/* clear accent anyway */
	}   /* if */
	else if (
		    !ctrl && !meta	    /* NO ctrl or meta */
		    && (XK_space == keysym || accent == keysym)
		)
	{
	    KeySym  dk = 0;

	    /*
	     * dead key + space -> dead key itself
	     * dead key ^ 2 -> dead key itself
	     * change the keysym so as to print out the dead key
	     */
	    switch (accent)
	    {
		case XK_dead_grave:	    /* ` */
		    keysym = dk = XK_grave;
		    break;

		case XK_dead_acute:	    /* ' */
		    keysym = dk = XK_apostrophe;
		    break;

		case XK_dead_circumflex:    /* ^ */
		    keysym = dk = XK_asciicircum;
		    break;

		case XK_dead_diaeresis:	    /* " */
		    keysym = dk = XK_quotedbl;
		    break;

		case XK_dead_tilde:	    /* ~ */
		    keysym = dk = XK_asciitilde;
		    break;

		default:
		    assert(0);

	    }	/* switch(accent) */
	    kbuf[0] = (unsigned char) dk;

	    len = 1;
	    accent = 0;	/* clear accent anyway */
	}
	else if (
		    !ctrl && !meta && 0 == len
		    && (XK_Shift_L == keysym || XK_Shift_R == keysym)
		)
	{
	    ;	/* do NOT clear accent when only shft is pressed */
	}
	else
	{
	    accent = 0;	/* clear accent anyway */
	}
    }	/* 0 != accent */
#endif	/* USE_DEADKEY */


    /*
     * V: beginning of valid_keysym (1)
     */
#ifdef USE_XIM
    if (valid_keysym)
#endif	/* USE_XIM */
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ctrl-meta-shft-keysym: %d-%d-%d-%x\n", ctrl, meta, shft, (int) keysym));

	/*
	 * 2006-02-24 gi1242: Allow macros with no modifier pressed so that the
	 * macro functionality will also generalize the keysym functionality.
	 *
	 * Since we use a binary search to look up macros, even if the user has
	 * a very large list of macros the price we pay for looking them up will
	 * be quite small. Thus using UNSHIFTED_MACROS does not hurt performance
	 * much.
	 */
#ifndef UNSHIFTED_MACROS
	if (ctrl || meta || alt || shft)
#endif
	    if( rxvt_process_macros( r, keysym, ev ) )
		return;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "bypass rxvt_process_macros\n"));

	/*
	 * 2006-02-24 gi1242: If you want to "hard code" a few macros into
	 * mrxvt, do them here. The code for unshifted-scrollkeys etc used to be
	 * here, and has now been removed.
	 *
	 * Rather than hardcode macros in, it is better to use the macro feature
	 * and define your macros in the system wide config file.
	 */


	/*
	 * At this point, all the keystrokes that have special meaning to us
	 * have been handled. If we are in the hold mode, this is the keystroke
	 * to exit. Otherwise, return here.
	 */
	if (PVTS(r)->hold > 1)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "exit after hold\n"));
	    if (keysym && len)
		rxvt_remove_page (r);
	    return;
	}


	/*
	 * A: begin 0xFFxx keysym
	 */
	if (keysym >= 0xff00 && keysym <= 0xffff)
	{
	    int	    newlen = rxvt_0xffxx_keypress (r, keysym,
				ctrl, meta, shft, kbuf);
	    if (-1 != newlen)
		len = newlen;
    
#if 0 /* was ifdef META8_OPTION. */
	    /* 2006-05-23 gi1242 Better to use modifier instead */
	    /*
	     * Pass meta for all function keys, if 'meta' option set
	     */
	    if (meta && (r->h->meta_char == 0x80) && len > 0)
		kbuf[len - 1] |= 0x80;
#endif	/* META8_OPTION */

	}
	/*
	 * A: end of 0xFFxx keysym
	 */


	else if (ctrl && keysym == XK_minus)
	{
	    len = 1;
	    kbuf[0] = '\037';	/* Ctrl-Minus generates ^_ (31) */
	}

	else if ( shft && keysym == XK_ISO_Left_Tab )
	{
	    /* Most keyboards send this keysym pressing Shift+Tab. */
	    STRCPY( kbuf, "\033[Z");
	    len = 3;
	}


#if defined(XK_dead_grave) && defined(XK_dead_horn)
	/*
	** ========================================================
	** C: beginning of 0xFE50 - 0xFE62, dead keys
	*/
	else if (!ctrl && !meta &&  /* NO ctrl or meta */
	    keysym >= XK_dead_grave && keysym <= XK_dead_horn)
	{
# ifdef USE_DEADKEY
	    if (
		    XK_dead_grave == keysym ||	    /* ` */
		    XK_dead_acute == keysym ||	    /* ' */
		    XK_dead_circumflex == keysym || /* ^ */
		    XK_dead_diaeresis == keysym ||  /* " */
		    XK_dead_tilde == keysym	    /* ~ */
	       )
	    {
		len = 0;
		accent = keysym;
	    }
# endif	/* USE_DEADKEY */
	}
	/*
	** C: end of 0xFE50 - 0xFE62, dead keys
	** ========================================================
	*/
#endif /* XK_dead_grave || XK_dead_horn */


	else if (len)
	{
	    if ( meta )
	    {
		/*
		 * If meta is pressed, then either set the 8th bit for all chars
		 * in kbuf, or prefix it with C0_ESC, depending on the meta8
		 * option.
		 *
		 * NOTE: This is not done for 0xffxx keys. Those should be
		 * indicated with a modifier parameter, as Xterm does.
		 */
#ifdef META8_OPTION
		if (r->h->meta_char == 0x80)	/* Set 8th bit */
		{
		    unsigned char  *ch;
	
		    for (ch = kbuf; ch < kbuf + len; ch++)
			*ch |= 0x80;

		    meta = 0;
		}
		else				/* Prefix with C0_ESC */
#endif	/* META8_OPTION */
		{
		    const unsigned char ch = C0_ESC;

		    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Sending meta for keysym %lx\n", keysym));
		    memmove(&kbuf[1], kbuf, len);
		    kbuf[0] = ch;
		    len ++;
		}
	    } /* if(meta) */

	}   /* else */

    }
    /*
     * V: End of valid_keysym (1)
     */



    if (len <= 0)
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Returning on unmpapped keysym %lx\n", keysym));
	return;		/* not mapped */
    }

    if ( alt && r->h->rs[Rs_altPrefix] )
    {
	    unsigned l = STRLEN(r->h->rs[Rs_altPrefix]);
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Sending alt prefix for keysym %lx\n", keysym));
	    memmove(&kbuf[l], kbuf, len);
	    memcpy(kbuf, r->h->rs[Rs_altPrefix], l);
	    len += l;
    }

    if (ISSET_OPTION(r, Opt_scrollTtyKeypress))
    {
	if (PVTS(r)->view_start)
	{
	    PVTS(r)->view_start = 0;
	    PVTS(r)->want_refresh = 1;
	}
    }

#ifdef DEBUG
    if (debug_key)	    /* Display keyboard buffer contents */
    {
	unsigned char*	p;
	int	        i;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "key 0x%04X [%d]: `", (unsigned int) keysym, len));
	for (i = 0, p = kbuf; i < len; i++, p++)
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, (*p >= ' ' && *p < '\177' ? "%c" : "\\%03o"), *p));
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "'\n"));
    }
#endif	/* DEBUG */

    if (0 == STRCMP ("UTF-8", r->h->locale))
    {
	rxvt_msg (DBG_INFO, DBG_COMMAND, "UTF-8 string?");
    }

    rxvt_tt_write(r, kbuf, (unsigned int)len);
}
/*}}} */


/*{{{ rxvt_cmd_write(), rxvt_cmd_getc() */
/* attempt to `write' count to the input buffer */
/* EXTPROTO */
void
rxvt_cmd_write( rxvt_t* r, const unsigned char *str,
	unsigned int count)
{
    unsigned int    n, s;
    unsigned char   *outbuf_base = PVTS(r)->outbuf_base,
                    *outbuf_end = PVTS(r)->outbuf_end,
                    *outbuf_start	= PVTS(r)->outbuf_escstart ? 
			    PVTS(r)->outbuf_escstart :
			    PVTS(r)->outbuf_start;

    n = outbuf_start - outbuf_base;
    s = outbuf_base + (BUFSIZ - 1) - outbuf_end;

    /*
     * If there is not enough space to write our data, try using the unused
     * space in the beginning.
     */
    if (n > 0 && s < count)
    {
	MEMMOVE(outbuf_base, outbuf_start,
	    (unsigned int)(outbuf_end - outbuf_start));

	outbuf_start  -= n;
	outbuf_end -= n;
	s += n;

	if( PVTS(r)->outbuf_escstart )
	    PVTS(r)->outbuf_escstart -= n;
	if( PVTS(r)->outbuf_escfail )
	    PVTS(r)->outbuf_escfail -= n;

	PVTS(r)->outbuf_start = outbuf_start;
    }

    if (count > s)
    {
	rxvt_msg (DBG_ERROR, DBG_COMMAND, "data loss: cmd_write too large");
	count = s;
    }

    for (; count--;)
	*outbuf_end++ = *str++;

    PVTS(r)->outbuf_end = outbuf_end;

    assert (PVTS(r)->outbuf_base <= PVTS(r)->outbuf_end);
}


/*
 * Does a waitpid() on each child, and marks it as dead if it's dead. This
 * function is safe to call from anywhere.
 */
void
rxvt_mark_dead_childs( rxvt_t *r )
{
    int	    i, j;
    short   ndead_childs = r->ndead_childs;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "rxvt_mark_dead_childs(): %d children are dead\n", r->ndead_childs));


    /*
     * Check processes running in each tab.
     */
	int	    status, pid;

	errno = 0;  /* Clear errno */
	if(
	     !PVTS(r)->dead	    &&
	     (pid = waitpid( PVTS(r)->cmd_pid, &status, WNOHANG)) != 0
	  )
	{
	    if( pid == -1 && errno == ECHILD )
	    {
		/*
		 * Command in ith tab is not our child. The only way this can
		 * happen is if we lost an SIGCHLD signal (e.g. if we receive a
		 * SIGCHLD when it is blocked, say by our own SIGCHLD handler).
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ECHILD error on waitpid\n" ));

		/* We have no way of getting the child's exit status now */
		PVTS( r )->status = 0;
	    }
	    else
	    {
		/* Process in tab i is dead. */
		r->ndead_childs--;			/* Reduce number of
							   unprocessed dead vt's
							   by one */
		ndead_childs--;				/* This variable is safe
							   from being modified
							   in a signal */
		PVTS( r )->status = status;		/* Save exit status */
	    }

	    /*
	     * Regardless of losing SIGCHLD, we mark this tab as dead.
	     */
	    PVTS( r )->dead	    = 1;	/* Mark it as dead */
	    PVTS( r )->hold	    = 1;	/* Hold it until it is cleaned
						   up */
	    r->cleanDeadChilds	    = 1;	/* Child has been marked as
						   dead, but not cleaned out. */
	}

    /*
     * Check processes we launched via rxvt_async_exec(). We don't care about
     * status info here, so this is much simpler.
     */
    for( i=0, j=0; i < r->nAsyncChilds; )
    {
	if( waitpid( r->asyncChilds[i], NULL, WNOHANG ) != 0 )
	{
	    r->ndead_childs--;
	    ndead_childs--;

	    i++;
	}
	
	else
	    r->asyncChilds[j++] = r->asyncChilds[i++];
    }
    r->nAsyncChilds -= (i-j);


    if( r->ndead_childs < 0 )
    {
	/*
	 * Oops. Some child died, but we never got a dead child signal on it. As
	 * long as we got here, we're fine.
	 *
	 * NOTE: It is OK for ndead_childs < 0. r->ndead_childs is updated for
	 * processes that die when we are in this function. ndead_childs is not.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,   "Lost child signal." ));
	r->ndead_childs = 0;
    }

    else if ( ndead_childs > 0 )
    {
	/*
	 * This is problematic. The number of processes that were promised as
	 * "dead" on entry to this function is not actually the number of
	 * processes that are dead!
	 *
	 * NOTE: It is OK for r->ndead_childs > 0, as r->ndead_childs could be
	 * externally modified while we are in this function.
	 *
	 * We should only get here when one of our child processes that is NOT
	 * running in a tab dies. For instance, when we print something with our
	 * "PrintPipe" macro. If we get here for any other reason, we're in deep
	 * trouble.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Spurious dead child signal received\n"));

	/*
	 * We should reset r->ndead_childs to 0. But there is a possible race
	 * condition with doing that. Suppose we received a dead child signal
	 * *after* looping over all childs, but just before getting here!
	 *
	 * To avoid this we only reduce r->ndead_childs by the number of
	 * processes we failed to catch as dead. Further, when we get EIO errors
	 * reading from a child, we call this function to see if the child is
	 * dead or not.
	 */
	r->ndead_childs -= ndead_childs;
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Exit rxvt_mark_dead_childs(): %d children are dead\n", r->ndead_childs));
}


/*
 * Cleans out tabs which have died but have not been cleaned up. (i.e. dead &&
 * hold == 1)
 *
 * NOTE: This function MUST NOT be called from rxvt_cmd_getc(), because it could
 * redirect command buffer input of the tabs.
 */
/* INTPROTO */
void
rxvt_clean_cmd_page (rxvt_t* r)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_clean_cmd_page()\n" ));
    if( r->ndead_childs )
	rxvt_mark_dead_childs( r );

   /*
    * We had better not get here unless we need to clean up dead children. Make
    * sure we don't proceed when we receive spurious dead child messages (e.g.
    * from when the print pipe dies).
    */
    if( !r->cleanDeadChilds )
	return;

    /*
     * We start from the last child because we need to move ahead after
     * removing dead child. This makes it much safer.
     *
     * Why do we need to restart dead value from LTAB(r) again? Because a
     * child may have died when we do something following and changed the
     * value of r->ndead_childs! This child may be later than any dead children
     * we have examined.
     */
	if( PVTS(r)->dead && PVTS(r)->hold == 1 )
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, 
			"exit %s (status %d). holdOption: %d\n",
			WIFEXITED(PVTS(r)->status) ? "success" : "failure",
			PVTS(r)->status,
			PVTS(r)->holdOption));
	    /*
	     * Process in tab i has died, and needs to be cleaned up.
	     */
	    if( SHOULD_HOLD( r ) )
	    {
		const int	maxLen = 1024;
		const char	*msg;

		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    "Hold child after it died\n"));

		/* increase hold number, so next iteration will skip it */
		PVTS(r)->hold++;

		/*
		 * Process any pending data from the child.
		 */
		do
		  {
		    unsigned char *last_escfail = NULL;

		    /*
		     * Process information in the child's output buffer.
		     */
		    while( PVTS(r)->outbuf_start < PVTS(r)->outbuf_end )
		    {
			rxvt_process_getc( r, *(PVTS(r)->outbuf_start++) );

			/* Incomplete escape sequence. */
			if( PVTS(r)->outbuf_escfail )
			{
			    /*
			     * See if reading from the child's fd will complete
			     * this escape seqeunce.
			     */
			    if( IS_NULL( last_escfail ) )
				last_escfail = PVTS(r)->outbuf_escfail;

			    else
			    {
				/* Really incomplete escape sequence */
				rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Incomplete escape sequence '%.*s'\n", 
					      PVTS(r)->outbuf_escfail - PVTS(r)->outbuf_escstart + 1, 
					      PVTS(r)->outbuf_escstart+1));

				SET_NULL( last_escfail );
				SET_NULL( PVTS(r)->outbuf_escstart );
				SET_NULL( PVTS(r)->outbuf_escfail );

				/* Skip the escape char */
				PVTS(r)->outbuf_start++;
			    }
			} /* if( PVTS(r)->outbuf_escfail ) */
		    }

		    /*
		     * Write out pending data in the child's input buffer.
		     */
		    if (PVTS(r)->inbuf_start < PVTS(r)->inbuf_end)
			rxvt_cmd_write(r, NULL, 0);

		    /* Make place for new data */
		    rxvt_check_cmdbuf( r );

		    /* Read any remaining data from childs fd */
		    rxvt_read_child_cmdfd( r,
			    BUFSIZ - 1 -
				(PVTS(r)->outbuf_end
					- PVTS(r)->outbuf_base) );
		  }
		while( rxvt_cmdbuf_has_input( r ) );

		/*
		 * print holdExitText on screen if defined.
		 */
		msg = getProfileOption( r,
				Rs_holdExitTxt );
		if( NOT_NULL( msg ) && *msg )
		{
		    unsigned char   buffer[maxLen];
		    int		    len;

		    rxvt_percent_interpolate( r, msg, STRLEN(msg),
			    (char*) buffer, maxLen );

		    len = rxvt_str_escaped( (char*) buffer );

		    rxvt_cmd_write(r, buffer, len );

		    if( PVTS(r)->outbuf_start < PVTS(r)->outbuf_end )
			rxvt_process_getc( r, *(PVTS(r)->outbuf_start++) );
		}

		/*
		 * Update title to show tab has finished.
		 */
		msg = getProfileOption( r,
				Rs_holdExitTtl );
		if( NOT_NULL( msg ) && *msg )
		{
		    unsigned char    tabTitle[maxLen];

		    rxvt_percent_interpolate( r, msg, STRLEN(msg),
			    (char*) tabTitle, maxLen );
		    rxvt_str_escaped( (char*) tabTitle );

		    rxvt_set_term_title( r, tabTitle );
		}
	    } /* if( SHOULD_HOLD( r ) ) */
	    else
		rxvt_remove_page( r );
	}

    r->cleanDeadChilds = 0; /* Dead child cleanup complete. */
}


/* Returns true if there is input pending in PVTS(r)->cmdbuf */
/* INTPROTO */
int static inline
rxvt_cmdbuf_has_input( rxvt_t *r )
{
    return PVTS(r)->outbuf_escfail ?
	PVTS(r)->outbuf_escfail < PVTS(r)->outbuf_end	    :
	PVTS(r)->outbuf_start < PVTS(r)->outbuf_end;
}

/* INTPROTO */
/* rxvt_check_cmdbuf (r, p) manage the free space in the buffer of the page p.
 * It will move the used space in it to the beginning when needed.
 */
void
rxvt_check_cmdbuf (rxvt_t* r)
{
    assert( PVTS(r)->outbuf_base <= PVTS(r)->outbuf_end );

    if(
	  IS_NULL( PVTS(r)->outbuf_escstart )		    &&
	  PVTS(r)->outbuf_start == PVTS(r)->outbuf_end
      )
    {
	/*
	 * If there is no data in the buffer, reset it to the beginning
	 * of the buffer.
	 */
	PVTS(r)->outbuf_start   = PVTS(r)->outbuf_end
				    = PVTS(r)->outbuf_base;

    }

    else if(
	     (PVTS(r)->outbuf_end - PVTS(r)->outbuf_base)
		== (BUFSIZ-1)						 &&
	     (
	       PVTS(r)->outbuf_escstart ?
		(PVTS(r)->outbuf_escstart > PVTS(r)->outbuf_base) :
		(PVTS(r)->outbuf_start > PVTS(r)->outbuf_base)
	     )
	   )
    {
	/*
	 * If there is space at beginning of the buffer, but not space at the
	 * end of the buffer, move the content of buffer forward to free space
	 */
	unsigned char	*start;
	unsigned int	n, len;

	start = PVTS(r)->outbuf_escstart ?
	    PVTS(r)->outbuf_escstart : PVTS(r)->outbuf_start;


	n   = start - PVTS(r)->outbuf_base;
	len = PVTS(r)->outbuf_end - start;

	assert( n == BUFSIZ - 1 - len );
	assert( start < PVTS(r)->outbuf_end );

	MEMMOVE( PVTS(r)->outbuf_base, start, len );

	PVTS(r)->outbuf_start   -= n;
	PVTS(r)->outbuf_end  -= n;
	if( PVTS(r)->outbuf_escstart )
	    PVTS(r)->outbuf_escstart -= n;
	if( PVTS(r)->outbuf_escfail )
	    PVTS(r)->outbuf_escfail -= n;
    }
}


/*
 * This function returns the number of bytes being read from a child
 * - r is the mrxvt state;
 * - page is the tab number you want to read the child output from;
 * - count is the maximum number of bytes you want to read.
 */
/* INTPROTO */
int
rxvt_read_child_cmdfd (rxvt_t* r, unsigned int count)
{
    int		    n = 0, bread = 0;
    struct	    timeval  tp;

    while( count )
    {
	int readErrno;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "read maximal %u bytes\n", count));

	/*
	 * 2006-08-23 gi1242: O_NDELAY is set here, so we need not worry about
	 * calls to read() blocking.
	 */
	errno = PVTS(r)->gotEIO = 0;
	n = read( PVTS(r)->cmd_fd, PVTS(r)->outbuf_end, count );
	readErrno = errno;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "read %d bytes\n", n));

	if (n > 0)
	{
	    /* Update count and buffer pointer */
	    count -= n;
	    bread += n;
	    PVTS(r)->outbuf_end += n;
	}

	else if (0 == n)
	{
	    /* rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Should not happen?\n")); */
	    /* 2006-08-23 gi1242: Could happen if we have no more data. */
	    break;
	}

	else /* if (n < 0) */
	{
	    /*
	     * We do not update count and buffer pointer and continue
	     * trying read more data in the next loop iteration.
	     */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s\n", strerror(readErrno)));

	    assert( readErrno != EBADF && readErrno != EFAULT &&
		    readErrno != EISDIR );

	    /* See if this process is dead */
	    switch (readErrno)
	    {
		case EIO:
		    r->gotEIO = PVTS(r)->gotEIO = 1;
		case EINTR:
		    rxvt_mark_dead_childs(r);
		    break;
	    }

	    /*
	     * 2006-08-31 gi1242: Old code would only break out on EAGAIN or
	     * EINVAL.
	     */
	    break;

	}
    }	/* while (count) */

    if (bread != 0)
    {
	gettimeofday( &tp, NULL);
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
		    "output produced on epoch %i\n", tp.tv_sec));
    }
    PVTS(r)->nbytes_last_read = bread;
    return bread;
}


/* INTPROTO */
void
rxvt_process_children_cmdfd( rxvt_t* r, fd_set* p_readfds )
{
    /*
     * Handle the children that have generate input. Notice in this loop we only
     * process input, but do NOT determine the child we want to return.
     */
	unsigned int	count, bufsiz;

	/* check next file descriptor if this one has nothing to read in. */
	if (!FD_ISSET(PVTS(r)->cmd_fd, p_readfds))
	{
	    PVTS(r)->nbytes_last_read = 0;
	    PVTS(r)->scrolled_lines	 = 0;
	    return;
	}

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "reading from shell\n"));

	/* check our command buffer before reading data */
	rxvt_check_cmdbuf( r );

	/* The buffer size is the buffer length - used length */
	count = bufsiz = (BUFSIZ - 1) -
	    (PVTS(r)->outbuf_end - PVTS(r)->outbuf_base);

	/* read data from the command fd into buffer */
	count -= rxvt_read_child_cmdfd (r, count);

#if 0
	/* check if a child died */
	if( PVTS(r)->dead && errno == EIO )
	    *PVTS(r)->outbuf_end = (char) 0;
#endif
}


/* Check quick_timeout before select */
/* INTPROTO */
int
rxvt_check_quick_timeout (rxvt_t* r)
{
    int			quick_timeout = 0;

#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
    if (r->h->mouse_slip_wheel_speed)
    {
	struct rxvt_hidden*	h = r->h;
	quick_timeout = 1;
	/* Only work for current active tab */
	if (!h->mouse_slip_wheel_delay-- &&
	    rxvt_scr_page( r,
		h->mouse_slip_wheel_speed >0 ? UP : DN,
		abs(h->mouse_slip_wheel_speed) ))
	{
	    h->mouse_slip_wheel_delay = SCROLLBAR_CONTINUOUS_DELAY;
	    h->refresh_type |= SMOOTH_REFRESH;
	    PVTS(r)->want_refresh = 1;
	}
    }
#endif /* MOUSE_WHEEL && MOUSE_SLIP_WHEELING */

#ifdef SELECTION_SCROLLING
    if (r->h->pending_scroll_selection)
    {
	struct rxvt_hidden*	h = r->h;
	quick_timeout = 1;
	/* Only work for current active tab */
	if (!h->scroll_selection_delay-- &&
	    rxvt_scr_page(r, h->scroll_selection_dir,
		h->scroll_selection_lines))
	{
	    rxvt_selection_extend(r, h->selection_save_x,
		h->selection_save_y, h->selection_save_state);
	    h->scroll_selection_delay = SCROLLBAR_CONTINUOUS_DELAY;
	    h->refresh_type |= SMOOTH_REFRESH;
	    PVTS(r)->want_refresh = 1;
	}
    }
#endif	/* SELECTION_SCROLLING */

    return quick_timeout;
}

/* Adjust quick_timeout after select */
/* INTPROTO */
int
rxvt_adjust_quick_timeout (rxvt_t* r, int quick_timeout, struct timeval* value)
{
    struct rxvt_hidden*	h = r->h;
    struct timeval	tp;
    int			set_quick_timeout = 0;
    int32_t		fsdiff = 60000000l;	/* or say LONG_MAX */


    assert (NOT_NULL(value));
    value->tv_usec = TIMEOUT_USEC;
    value->tv_sec = 0;


    if( !r->TermWin.mapped || r->h->refresh_type == NO_REFRESH )
	quick_timeout = 0;
    else
    {
	quick_timeout |= (PVTS(r)->want_refresh || h->want_clip_refresh);
    }

#if defined(POINTER_BLANK) || defined(CURSOR_BLINK)
    {
	int32_t	csdiff, psdiff, bsdiff;

	csdiff = psdiff = bsdiff = 60000000L;   /* or, say, LONG_MAX */

# if defined(CURSOR_BLINK)
	/*
	 * Cursor only blinks when terminal window is focused.
	 */
	if (ISSET_OPTION(r, Opt_cursorBlink) && r->TermWin.focus)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** get cursor time on select\n"));
	    (void)gettimeofday(&tp, NULL);

	    csdiff = (tp.tv_sec - h->lastcursorchange.tv_sec) * 1000000L
		 + tp.tv_usec - h->lastcursorchange.tv_usec;
	    if (csdiff > h->blinkInterval)
	    {
		/* XXX: settable blink times */
		h->lastcursorchange.tv_sec = tp.tv_sec;
		h->lastcursorchange.tv_usec = tp.tv_usec;
		h->hidden_cursor = !h->hidden_cursor;
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s\n", h->hidden_cursor ?  "** hide cursor" : "** show cursor"));

		PVTS(r)->want_refresh = 1;
		csdiff = 0;
	    }
	    else
		csdiff = h->blinkInterval - csdiff;
	    set_quick_timeout = 1;
	}
# endif	/* CURSOR_BLINK */

# if defined(POINTER_BLANK)
	/*
	 * If we haven't moved the pointer for a while
	 */
	if (ISSET_OPTION(r, Opt_pointerBlank) &&
	    (h->pointerBlankDelay > 0) &&
	    (0 == PVTS(r)->hidden_pointer))
	{
	    int32_t	pdelay;

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** get pointer time on select\n"));
	    (void) gettimeofday(&tp, NULL);
	    psdiff = (tp.tv_sec - h->lastmotion.tv_sec) * 1000000L
		 + tp.tv_usec - h->lastmotion.tv_usec;
	    pdelay = h->pointerBlankDelay * 1000000L;
	    /* only work for current active tab */
	    if (psdiff >= pdelay)
		rxvt_pointer_blank(r);
	    else
	    {
		set_quick_timeout = 1;
		psdiff = pdelay - psdiff;
	    }
	}
# endif	/* POINTER_BLANK */
	if (!quick_timeout && set_quick_timeout)
	{
	    MIN_IT(csdiff, fsdiff);
	    MIN_IT(csdiff, bsdiff);
	    MIN_IT(csdiff, psdiff);
	    value->tv_sec =  csdiff / 1000000L;
	    value->tv_usec = csdiff % 1000000L;
	    quick_timeout = 1;
	}
    }
#endif	/* POINTER_BLANK || CURSOR_BLINK */

    quick_timeout |= r->gotEIO;
    r->gotEIO = 0;

    return quick_timeout;
}


/* Refresh the VT screen and scrollbar if needed */
/* INTPROTO */
void
rxvt_refresh_vtscr_if_needed( rxvt_t *r )
{
    /*
     * If parts of the screen have changed, we should not honor the GC clipping.
     */
    if( PVTS(r)->want_refresh )
	r->h->refresh_type &= ~CLIPPED_REFRESH;

    if(
	 (PVTS(r)->want_refresh || r->h->want_clip_refresh)
	 && r->h->refresh_type != NO_REFRESH
      )
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%lu: produced %d bytes (%d in buffer)\n", time(NULL), PVTS(r)->nbytes_last_read, PVTS(r)->outbuf_end - PVTS(r)->outbuf_base ));

	rxvt_scr_refresh(r, r->h->refresh_type);

#ifdef USE_XIM
	rxvt_IM_send_spot (r);
#endif	/* USE_XIM */

    }   /* if (PVTS(r)->want_refresh) */
}


/*
 * rxvt_cmd_getc() - Return next input character.
 *
 * If *p_page == -1, then *p_page is set to a tab which returned input, and the
 * character is returned. Calling rxvt_cmd_getc() with *p_page = -1 is a good
 * thing, and should be done when possible.
 *
 * If *p_page != -1, we will either return a character from the tab *p_page, or
 * fail by setting *p_page to -1 and return 0. If the tab *p_page is dead on
 * entry, we will fail only when there is no data available. If the tab *p_page
 * is alive on entry, then we will fail for whatever reason we like (e.g. X
 * events are pending).
 */

/* INTPROTO */
int
rxvt_cmd_getc(rxvt_t *r)
{
    fd_set	    readfds;
    int		    quick_timeout, select_res;
#ifdef POINTER_BLANK
    int		    want_motion_time = 0;
#endif
#ifdef CURSOR_BLINK
    int		    want_keypress_time = 0;
#endif
#if defined(POINTER_BLANK) || defined(CURSOR_BLINK)
    struct timeval  tp;
#endif
    struct timeval  value;
    struct rxvt_hidden *h = r->h;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "Entering rxvt_cmd_getc\n"));


    /* loop until we can return something */
    for(;;)
    {
#if defined(POINTER_BLANK) || defined(CURSOR_BLINK)
	/* presume == 0 implies time not yet retrieved */
	tp.tv_sec = tp.tv_usec = 0;
#endif	/* POINTER_BLANK || CURSOR_BLINK */
#ifdef CURSOR_BLINK
	want_keypress_time = 0;
#endif	/* CURSOR_BLINK */
#ifdef POINTER_BLANK
	if (ISSET_OPTION(r, Opt_pointerBlank))
	    want_motion_time = 0;
#endif	/* POINTER_BLANK */


	    /* Process all pending X events */
	    while( XPending(r->Xdisplay) )
	    {
		XEvent	  xev;

		XNextEvent(r->Xdisplay, &xev);


#ifdef CURSOR_BLINK
		if (ISSET_OPTION(r, Opt_cursorBlink) &&
		    KeyPress == xev.type)
		{
		    if (h->hidden_cursor)
		    {
			rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** hide cursor on keypress\n"));
			h->hidden_cursor = 0;
			PVTS(r)->want_refresh = 1;
		    }
		    want_keypress_time = 1;
		}
#endif	/* CURSOR_BLINK */

#ifdef POINTER_BLANK
		if (ISSET_OPTION(r, Opt_pointerBlank) &&
		    (h->pointerBlankDelay > 0))
		{
		    if (MotionNotify == xev.type ||
			ButtonPress == xev.type ||
			ButtonRelease == xev.type )
		    {
			/* only work for current active tab */
			if (PVTS(r)->hidden_pointer)
			    rxvt_pointer_unblank(r);
			want_motion_time = 1;
		    }
		    /* only work for current active tab */
		    if (KeyPress == xev.type && !PVTS(r)->hidden_pointer)
			rxvt_pointer_blank(r);
		}
#endif	/* POINTER_BLANK */

#ifdef USE_XIM
		if (NOT_NULL(r->h->Input_Context))
		{
		    if (!XFilterEvent(&xev, xev.xany.window))
			rxvt_process_x_event(r, &xev);
		    h->event_type = xev.type;
		}
		else
#endif	/* USE_XIM */
		{
		    rxvt_process_x_event(r, &xev);
		}
	    }   /* while ((XPending(r->Xdisplay)) */


	/*
	 * We are done processing our X events. Check to see if we have any data
	 * pending in our input buffer.
	 */
	if( rxvt_cmdbuf_has_input (r) )
	{
	    /*
	     * In case -1 == selpage we are free to return data from any tab we
	     * choose. Note, that rxvt_find_cmd_child() will favor returning the
	     * active tab.
	     */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_find_cmd_child: find\n"));

	    return *(PVTS(r)->outbuf_start)++;
	}


	/*
	 * The command input buffer is empty and we have no pending X events.
	 * We call select() to wait until some data is available.
	 */
#ifdef CURSOR_BLINK
	if (want_keypress_time)
	{
	    /* reset last cursor change time on keypress event */
	    (void) gettimeofday (&tp, NULL);
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** init cursor time on keypress\n"));
	    h->lastcursorchange.tv_sec = tp.tv_sec;
	    h->lastcursorchange.tv_usec = tp.tv_usec;
	}
#endif	/* CURSOR_BLINK */

#ifdef POINTER_BLANK
	if (ISSET_OPTION(r, Opt_pointerBlank) && want_motion_time)
	{
	    (void) gettimeofday (&tp, NULL);
	    h->lastmotion.tv_sec = tp.tv_sec;
	    h->lastmotion.tv_usec = tp.tv_usec;
	}
#endif	/* POINTER_BLANK */

	quick_timeout = rxvt_check_quick_timeout (r);
	quick_timeout = rxvt_adjust_quick_timeout (r, quick_timeout, &value);

	/* Now begin to read in from children's file descriptors */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Waiting for %lumu for child\n", 
		      quick_timeout ? value.tv_sec * 1000000LU + value.tv_usec : ULONG_MAX));


	/* Prepare to read in from children's file descriptors */
	FD_ZERO(&readfds);
	FD_SET(r->Xfd, &readfds);

	    /* remember to skip held childrens */
	    if ( PVTS(r)->hold > 1 )
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    " not listening on vt.cmd_fd (dead)\n"));
	    }
	    else if ( PVTS(r)->gotEIO )
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    " not listening on vt.cmd_fd (EIO)\n"));
		PVTS(r)->gotEIO = 0;
	    }
	    else
        {
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			" listening on vt.cmd_fd = %d\n",
			PVTS(r)->cmd_fd));
	    FD_SET(PVTS(r)->cmd_fd, &readfds);

	    /* Write out any pending output to child */
	    if( PVTS(r)->inbuf_start < PVTS(r)->inbuf_end )
		rxvt_tt_write(r, NULL, 0);
        }

#ifdef HAVE_X11_SM_SMLIB_H
	if (-1 != r->TermWin.ice_fd)
	    FD_SET(r->TermWin.ice_fd, &readfds);
#endif

	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
		    "Calling select( num_fds=%d, timeout=%06du, &readfds)",
		    r->num_fds,
		    quick_timeout ? value.tv_sec * 1000000 + value.tv_usec : -1
		    ));
	select_res = select( r->num_fds, &readfds, NULL, NULL,
			(quick_timeout ? &value : NULL) );
	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
		    "done (timeout %06du). Return %d\n",
		    quick_timeout ? value.tv_sec * 1000000 + value.tv_usec : -1,
		    select_res ));

	if( select_res > 0 )
	{
	    /* Select succeeded. Check if we have new Xevents first. */
	    if( 0 && XPending( r->Xdisplay ) > 25)
	    {
		rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
			"%d xevents to processes. Continuing\n",
			XPending( r->Xdisplay ) ));
		continue;
	    }

	    /* Read whatever input we can from child fd's*/
	    rxvt_process_children_cmdfd (r, &readfds);

#ifdef HAVE_X11_SM_SMLIB_H
	    /*
	     * ICE file descriptor must be processed after we process all file
	     * descriptors of children. Otherwise, if a child exit,
	     * IceProcessMessages may hang and make the entire terminal
	     * unresponsive.
	     */
	    if(
		 -1 != r->TermWin.ice_fd &&
		 FD_ISSET (r->TermWin.ice_fd, &readfds)
	      )
		rxvt_process_ice_msgs (r);
#endif

	    /*
	     * Now figure out if we have something to return.
	     */
	    if( rxvt_cmdbuf_has_input(r) )
		return *(PVTS(r)->outbuf_start)++;

	} /* if( select_res >= 0 ) */

	/*
	 * If we get here, we either have a select() error, or no tabs had any
	 * input. Check to see if something died.
	 */
	if( r->ndead_childs || select_res == -1 )
	    rxvt_mark_dead_childs( r );

	if( r->cleanDeadChilds )
	{
	    /* Ok. Something died. */
	    return -1;
	} /* if( r->cleanDeadChilds ) */


	/*
	 * Nothing to return. Screen refresh, and call select() again.
	 */
	rxvt_refresh_vtscr_if_needed( r );

    }	/* for(;;) */

    /* NOT REACHED */
}
/*}}} */


/* EXTPROTO */
void
rxvt_pointer_unblank(rxvt_t* r)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s(r)\n", __func__));
    XDefineCursor(r->Xdisplay, PVTS(r)->vt, r->term_pointer);
    rxvt_recolour_cursor(r);
#ifdef POINTER_BLANK
    if (NOTSET_OPTION(r, Opt_pointerBlank))
	return;	/* no need to do anything */

    PVTS(r)->hidden_pointer = 0;

    if (r->h->pointerBlankDelay > 0)
    {
	struct timeval  tp;

	(void)gettimeofday(&tp, NULL);
	r->h->lastmotion.tv_sec = tp.tv_sec;
	r->h->lastmotion.tv_usec = tp.tv_usec;
    }
#endif
}


#ifdef POINTER_BLANK
/* INTPROTO */
void
rxvt_pointer_blank(rxvt_t* r)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Blanking pointer\n"));
    if (ISSET_OPTION(r, Opt_pointerBlank) &&
	IS_CURSOR(r->h->blank_pointer))
    {
	XDefineCursor(r->Xdisplay, PVTS(r)->vt,
	    r->h->blank_pointer);
	XFlush(r->Xdisplay);
	PVTS(r)->hidden_pointer = 1;
    }
}
#endif


/* INTPROTO */
void
rxvt_mouse_report(rxvt_t* r, const XButtonEvent *ev)
{
    int		 button_number, key_state = 0;
    int		 x, y;

    x = ev->x;
    y = ev->y;
    rxvt_pixel_position(r, &x, &y);

    if (r->h->MEvent.button == AnyButton)
    {
	button_number = 3;
    }
    else
    {
	button_number = r->h->MEvent.button - Button1;
	/* add 0x3D for wheel events, like xterm does */
	if (button_number >= 3)
	    button_number += (64 - 3);
    }

    if (ISSET_PMODE(r, PrivMode_MouseX10))
    {
	/*
	 * do not report ButtonRelease
	 * no state info allowed
	 */
	key_state = 0;
	if (button_number == 3)
	    return;
    }
    else
    {
	/* XTerm mouse reporting needs these values:
	 *   4 = Shift
	 *   8 = Meta
	 *  16 = Control
	 * plus will add in our own Double-Click reporting
	 *  32 = Double Click
	 */
	key_state = ((r->h->MEvent.state & ShiftMask) ? 4 : 0)
	     + ((r->h->MEvent.state & r->h->ModMetaMask) ? 8 : 0)
	     + ((r->h->MEvent.state & ControlMask) ? 16 : 0);
#ifdef MOUSE_REPORT_DOUBLECLICK
	key_state += ((r->h->MEvent.clicks > 1) ? 32 : 0);
#endif
    }

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Mouse ["));
    if (key_state & 16)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'C'));
    if (key_state & 4)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'S'));
    if (key_state & 8)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'A'));
    if (key_state & 32)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", '2'));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "]: <%d>, %d/%d\n", button_number, x + 1, y + 1));
#else
    rxvt_tt_printf(r, "\033[M%c%c%c",
	  (32 + button_number + key_state),
	  (32 + x + 1),
	  (32 + y + 1));
#endif
}


/*
** Individual X Event handlers
*/
#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
/* INTPROTO */
void
rxvt_process_keyrelease(rxvt_t* r, XKeyEvent *ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "KeyRelease event\n"));
    if (!(ev->state & ControlMask))
	r->h->mouse_slip_wheel_speed = 0;
    else
    {
	KeySym	      ks;

	ks = XKeycodeToKeysym(r->Xdisplay, ev->keycode, 0);
	if (ks == XK_Control_L || ks == XK_Control_R)
	    r->h->mouse_slip_wheel_speed = 0;
    }
}
#endif



/* INTPROTO */
void
rxvt_process_buttonpress(rxvt_t* r, XButtonEvent *ev)
{
    int			reportmode = 0, clickintime;
    struct rxvt_hidden*	h = r->h;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ButtonPress event\n"));
    h->bypass_keystate = ev->state & (h->ModMetaMask | ShiftMask);
    if (!h->bypass_keystate)
	reportmode = !!ISSET_PMODE(r, PrivMode_mouse_report);

    /*
     * VT window processing of button press
     */
    if (ev->window == PVTS(r)->vt)
    {
	clickintime = ev->time - h->MEvent.time < MULTICLICK_TIME;
	if (reportmode)
	{
	    /* mouse report from vt window */
	    /* save the xbutton state (for ButtonRelease) */
	    h->MEvent.state = ev->state;
#ifdef MOUSE_REPORT_DOUBLECLICK
	    if (ev->button == h->MEvent.button && clickintime)
	    {
		/* same button, within alloted time */
		h->MEvent.clicks++;
		if (h->MEvent.clicks > 1)
		{
		    /* only report double clicks */
		    h->MEvent.clicks = 2;
		    rxvt_mouse_report(r, ev);

		    /* don't report the release */
		    h->MEvent.clicks = 0;
		    h->MEvent.button = AnyButton;
		}
	    }
	    else
	    {
		/* different button, or time expired */
		h->MEvent.clicks = 1;
		h->MEvent.button = ev->button;
		rxvt_mouse_report(r, ev);
	    }
#else
	    h->MEvent.button = ev->button;
	    rxvt_mouse_report(r, ev);
#endif		    /* MOUSE_REPORT_DOUBLECLICK */
	}
	else
	{
	    if (ev->button != h->MEvent.button)
		h->MEvent.clicks = 0;
	    switch (ev->button)
	    {
		case Button1:
		    if (h->MEvent.button == Button1 && clickintime)
			h->MEvent.clicks++;
		    else
			h->MEvent.clicks = 1;
		    rxvt_selection_click(r, h->MEvent.clicks,
			ev->x, ev->y);
		    h->MEvent.button = Button1;
		    break;

		case Button3:
		    if (h->MEvent.button == Button3 && clickintime)
			rxvt_selection_rotate(r, ev->x, ev->y);
		    else
			rxvt_selection_extend(r, ev->x, ev->y, 1);
		    h->MEvent.button = Button3;
		    break;
	    }
	}
	h->MEvent.time = ev->time;
	return;
    }


}



#ifdef MOUSE_WHEEL
/* INTPROTO */
void
rxvt_process_wheel_button(rxvt_t* r, XButtonEvent *ev)
{
    int		 i, v;

    v = (ev->button == Button4) ? UP : DN;
    if (ev->state & ShiftMask)
	i = 1;
    else if (ISSET_OPTION(r, Opt_mouseWheelScrollPage))
	i = r->TermWin.nrow - 1;
    else
	i = 5;

# ifdef MOUSE_SLIP_WHEELING
    if (ev->state & ControlMask)
    {
	r->h->mouse_slip_wheel_speed += (v ? -1 : 1);
	r->h->mouse_slip_wheel_delay = SCROLLBAR_CONTINUOUS_DELAY;
    }
# endif

# ifdef JUMP_MOUSE_WHEEL
    rxvt_scr_page(r, v, i);

# else	/* !JUMP_MOUSE_WHEEL */
    for (; i--;)
    {
	rxvt_scr_page(r, v, 1);

#  ifdef XFT_SUPPORT
	/* disable screen refresh if XFT antialias is used to improve
	 * performance */
	if (!(ISSET_OPTION(r, Opt_xft) && ISSET_OPTION(r, Opt2_xftAntialias)))
#  endif    /* XFT_SUPPORT */
	    rxvt_scr_refresh(r, SMOOTH_REFRESH);
    }
# endif	/* JUMP_MOUSE_WHEEL */
}
#endif	/* MOUSE_WHEEL */


/* INTPROTO */
void
rxvt_process_buttonrelease(rxvt_t* r, XButtonEvent *ev)
{
    int		 reportmode = 0;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ButtonRelease event\n"));
    r->h->csrO = 0;	/* reset csr Offset */
    if (!r->h->bypass_keystate)
	reportmode = !!ISSET_PMODE(r, PrivMode_mouse_report);

#ifdef SELECTION_SCROLLING
    r->h->pending_scroll_selection=0;
#endif	/* SELECTION_SCROLLING */

    if (ev->window == PVTS(r)->vt)
    {
	if (NOT_WIN(ev->subwindow))
	{
	    if (reportmode)
	    {
		/* mouse report from vt window */
		/* don't report release of wheel "buttons" */
		if (ev->button >= 4)
		    return;
#ifdef MOUSE_REPORT_DOUBLECLICK
		/* only report the release of 'slow' single clicks */
		if (
			r->h->MEvent.button != AnyButton
			&& (
			    ev->button != r->h->MEvent.button
			    || (ev->time - r->h->MEvent.time > MULTICLICK_TIME / 2)
			   )
		   )
		{
		    r->h->MEvent.clicks = 0;
		    r->h->MEvent.button = AnyButton;
		    rxvt_mouse_report(r, ev);
		}
#else	/* MOUSE_REPORT_DOUBLECLICK */
		r->h->MEvent.button = AnyButton;
		rxvt_mouse_report(r, ev);
#endif	/* MOUSE_REPORT_DOUBLECLICK */
		return;
	    }
	    /*
	     * dumb hack to compensate for the failure of click-and-drag
	     * when overriding mouse reporting
	     */
	    if (ISSET_PMODE(r, PrivMode_mouse_report) &&
		r->h->bypass_keystate &&
		Button1 == ev->button &&
		r->h->MEvent.clicks <= 1)
		rxvt_selection_extend(r, ev->x, ev->y, 0);

	    switch (ev->button)
	    {
		case Button1:
		case Button3:
		    rxvt_selection_make(r, ev->time);
		    break;
		case Button2:
		    rxvt_selection_request(r, ev->time, ev->x, ev->y);
		    break;
#ifdef MOUSE_WHEEL
		case Button4:
		case Button5:
		    rxvt_process_wheel_button (r, ev);
		    break;
#endif	/* MOUSE_WHEEL */
	    }
	}
    }
}



/* INTPROTO */
void
rxvt_process_clientmessage(rxvt_t* r, XClientMessageEvent* ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ClientMessage event\n"));
    if (
	  ev->format == 32
	  && (Atom) ev->data.l[0] == r->h->xa[XA_WMDELETEWINDOW]
       )
	rxvt_exit_request (r);

#ifdef OFFIX_DND
    /* OffiX Dnd (drag 'n' drop) protocol */
    if (
	    ev->xclient.message_type == h->xa[XA_DNDPROTOCOL]
	    && (
		ev->xclient.data.l[0] == DndFile
		|| ev->xclient.data.l[0] == DndDir
		|| ev->xclient.data.l[0] == DndLink
	       )
       )
    {
	/* Get Dnd data */
	Atom		ActualType;
	int		ActualFormat;
	unsigned char*	data;
	uint32_t	Size, RemainingBytes;

	XGetWindowProperty(r->Xdisplay, XROOT,
	   r->h->xa[XA_DNDSELECTION], 0L, 1000000L,
	   False, AnyPropertyType, &ActualType, &ActualFormat,
	   &Size, &RemainingBytes, &data);
	XChangeProperty(r->Xdisplay, XROOT, XA_CUT_BUFFER0,
	    XA_STRING, 8, PropModeReplace, data, STRLEN(data));

	rxvt_selection_paste(r, XROOT, XA_CUT_BUFFER0, True);
	XSetInputFocus(r->Xdisplay, XROOT, RevertToNone, CurrentTime);
    }
#endif	    /* OFFIX_DND */
}



/* INTPROTO */
void
rxvt_process_visibilitynotify(rxvt_t* r, XVisibilityEvent* ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "VisibilityNotify event\n"));
    switch (ev->state)
    {
	case VisibilityUnobscured:
	    r->h->refresh_type = FAST_REFRESH;
	    break;
	case VisibilityPartiallyObscured:
	    r->h->refresh_type = SLOW_REFRESH;
	    break;
	default:
	    r->h->refresh_type = NO_REFRESH;
	    break;
    }
}



#ifdef MONITOR_ENTER_LEAVE
/* INTPROTO */
void
rxvt_process_enter (rxvt_t* r, XCrossingEvent* ev)
{
    if (ev->window == r->TermWin.parent)
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Enter event\n"));
	r->TermWin.enter = 1;
    }
}


/* INTPROTO */
void
rxvt_process_leave (rxvt_t* r, XCrossingEvent* ev)
{
    if (ev->window == r->TermWin.parent)
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Leave event\n"));
	r->TermWin.enter = 0;
    }
}
#endif	/* MONITOR_ENTER_LEAVE */


/*
 * Change the bg / faded colors because of a focus change
 */
/* INTPROTO */
void
rxvt_change_colors_on_focus( rxvt_t *r )
{
    rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND, "%s(r)\n", __func__ ));
    if( rxvt_set_vt_colors( r ) )
    {
	rxvt_scr_clear(r);
	rxvt_scr_touch(r, True);
    }
}


/* INTPROTO */
void
rxvt_process_focus (rxvt_t* r, XFocusChangeEvent* ev)
{
    if( ev->mode == NotifyGrab || ev->mode == NotifyUngrab )
	return;

    if (ev->window == r->TermWin.parent)
    {
	while ( XCheckTypedWindowEvent( r->Xdisplay, ev->window, FocusIn,  (XEvent *)ev) ||
		XCheckTypedWindowEvent( r->Xdisplay, ev->window, FocusOut, (XEvent *)ev));
	Bool focus = ev->type == FocusIn;

	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND, "%s( r, ev) %d. ev->mode=%d\n",
		    __func__, focus, ev->mode ));

	if (r->TermWin.focus == focus)
		return;

	r->TermWin.focus = focus;
	PVTS(r)->want_refresh = 1; /* Cursor needs to be refreshed */

#ifdef CURSOR_BLINK
	if (!focus)
	    r->h->hidden_cursor = 0;
#endif

#ifdef USE_XIM
	if (NOT_NULL(r->h->Input_Context))
	{
	  if (focus)
	    XSetICFocus(r->h->Input_Context);
	  else
	    XUnsetICFocus(r->h->Input_Context);
	}
#endif

	rxvt_change_colors_on_focus (r);
    }
}


/*
 * Recalculate and set the szHint upon changes of menubar/scrollbar/tabbar or
 * font. szHint.width and height are not modified, but the XResizeWindow is
 * called. The szHint.width and height will be correctly updated on
 * ConfigureNotify events.
 *
 * reason:  reason we want to resize the window.
 */
/* INTPROTO */
void
rxvt_resize_on_subwin (rxvt_t* r, resize_reason_t reason)
{
    /*
     * Store the old width and height
     */
    unsigned oldWidth  = r->szHint.width;
    unsigned oldHeight = r->szHint.height;

    switch (reason)
    {
	case RESIZE_FONT:
	    /* Calculate the base width and height */
	    r->szHint.base_width  = 2 * r->TermWin.int_bwidth;
	    r->szHint.base_height = 2 * r->TermWin.int_bwidth;

	    /* Set the terminal incremental width and height */
#ifndef NO_FRILLS
	    if(ISSET_OPTION(r, Opt2_smoothResize))
	    {
		r->szHint.width_inc = 1;
		r->szHint.height_inc = 1;
	    }
	    else
#endif
	    {
		r->szHint.width_inc = r->TermWin.fwidth;
		r->szHint.height_inc = r->TermWin.fheight;
	    }

	    /* Set the terminal minimal width and height */
	    r->szHint.min_width = r->szHint.base_width + r->TermWin.fwidth;
	    r->szHint.min_height = r->szHint.base_height + r->TermWin.fheight;

	    /* Calculate new height and width */
	    r->szHint.width  = r->szHint.base_width +
		    Width2Pixel(r->TermWin.ncol);
	    r->szHint.height = r->szHint.base_height +
		    Height2Pixel(r->TermWin.nrow);

	    /*
	     * Just incase the window is not resized by the WM, then we still need
	     * to refresh the screen.
	     */
	    r->h->want_resize |= FORCE_REFRESH;
	    break;

	default:
	    assert (0);	/* should not reach here */
	    return ;
    }

    /* Reset WMNormal Hints. We need not worry about r->szHint.flags */
    XSetWMNormalHints (r->Xdisplay, r->TermWin.parent, &(r->szHint));

    r->h->window_vt_y = r->szHint.base_height - 2*r->TermWin.int_bwidth;

    /*
     * Now we can resize the window The resize request might not always succeed.
     * If the window is already maximized, then (most?) WMs do nothing on the
     * below call. So when we recieve a ConfigureNotify event, we'll think we've
     * not moved or resized, and will erroriously not update our TermWin.ncols
     * etc.
     */
    XResizeWindow(r->Xdisplay, r->TermWin.parent,
	    r->szHint.width, r->szHint.height);

    /*
     * We get around this error by setting want_resize. In configure notify
     * events, if this variable is set, we resize the window regardless of
     * wether it apears to be in the same position or not.
     */
    r->h->want_resize |= FORCE_RESIZE;

#ifndef NO_FRILLS
    while(ISSET_OPTION(r, Opt2_smartResize))
    {
	/*
	 * Let's attempt to move the window so we don't push part of it off
	 * screen.
	 */

	CARD32	 left, right, top, bottom;
	int	 wx, wy;	  /* pos of window with decorations */
	unsigned wwidth, wheight; /* dim of window with decorations */

	int newx = r->szHint.x,   /* new location to move to */
	    newy = r->szHint.y;

	unsigned    dspw = DisplayWidth(  r->Xdisplay, XSCREEN),
		    dsph = DisplayHeight( r->Xdisplay, XSCREEN);

	int  dx = (int) r->szHint.width - (int) oldWidth,
	     dy = (int) r->szHint.height- (int) oldHeight;


	if( !getWMStruts( r->Xdisplay, r->TermWin.parent,
		&left, &right, &top, &bottom)) break;

	wx = r->szHint.x - left;
	wy = r->szHint.y - top;
	wwidth  = r->szHint.width  + left + right;
	wheight = r->szHint.height + top + bottom;

	/*
	 * Now the position of the window with decorations is in wx, wy,
	 * wwidth, wheight.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Strut: (%lu, %lu %lu, %lu), " "Geometry: %ux%u+%d+%d, Change: (%d, %d)\n", left, right, top, bottom, wwidth, wheight, wx, wy, dx, dy ));

	/*
	 * Try and move us back on screen, if we resized off it.
	 */
	if( wwidth >= dspw || wheight >= dsph ) break;

	if( wx < 0 && wx + dx >= 0) newx = left;
	else if( wx + (int) wwidth > (int) dspw
		&& wx + (int) wwidth - dx <= (int) dspw )
	    newx = dspw - wwidth + left;

	if( wy < 0 && wy + dy >= 0) newy = top;
	else if( wy + (int) wheight > (int) dsph
		&& wy + (int) wheight - dy <= (int) dsph)
	    newy = dsph - wheight + top;

	if(
		(newx != r->szHint.x || newy != r->szHint.y) /* moved */
		&& newx - (int) left >=0	/* onscreen */
		&& newx - (int) left + (int) wwidth <= dspw
		&& newy - (int) top >=0
		&& newy - (int) top + (int) wheight <= dsph
	  )
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Smart move to %ux%u+%d+%d\n", r->szHint.width, r->szHint.height, newx, newy));
	    XMoveWindow( r->Xdisplay, r->TermWin.parent,
		    newx, newy);
	}
	break;
    }
#endif

    /* 
     * According to X Reference, we should only trust the size of a Window
     * through ConfigureNotify event, or through XGet... functions. So here we
     * must not change the window size till we receive a ConfigureNotify event.
     */
    r->szHint.width = oldWidth;
    r->szHint.height = oldHeight;
}


/* Resize windows on changing fonts */
/* INTPROTO */
void
rxvt_resize_on_font (rxvt_t* r, char* fontname)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_resize_on_font\n"));

#ifdef XFT_SUPPORT
    /* if use freetype font, disallow resize by now ;-) */
    if (ISSET_OPTION(r, Opt_xft) && r->TermWin.xftfont)
	if (!rxvt_change_font_xft (r, fontname))
	    return ;
    if (NOTSET_OPTION(r, Opt_xft))
#endif	/* XFT_SUPPORT */
    /* X11 font resize */
    if (!rxvt_change_font_x11 (r, fontname))
	return ;

    rxvt_resize_on_subwin (r, RESIZE_FONT);
}



/*
** Recalculate the window col/row upon window resizing
*/
/* INTPROTO */
int
rxvt_calc_colrow (rxvt_t* r, unsigned int width, unsigned int height)
{
    unsigned int    ncol, nrow;


    assert (0 != r->TermWin.fwidth);
    assert (0 != r->TermWin.fheight);

    /* Do nothing if size does not change, unless we requested the resize. */
    if ( !r->h->want_resize &&
	    r->szHint.width == width && r->szHint.height == height)
	return 0;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Recalc row/col of (%d, %d)\n", width, height));
    ncol = Pixel2Width (width - r->szHint.base_width);
    nrow = Pixel2Height (height - r->szHint.base_height);
    MAX_IT(ncol, 1);
    MAX_IT(nrow, 1);

    r->h->prev_ncol = r->TermWin.ncol;
    r->h->prev_nrow = r->TermWin.nrow;
    r->TermWin.ncol = ncol;
    r->TermWin.nrow = nrow;

    /* 
     * According to X Reference, we should only trust the size of a Window
     * through ConfigureNotify event, or through XGet... functions. This
     * function should only be called from ConfigureNotify event handler
     */
    r->szHint.width = width;
    r->szHint.height = height;

    return ((r->h->prev_ncol != r->TermWin.ncol) ||
	    (r->h->prev_nrow != r->TermWin.nrow));
}


/* INTPROTO */
void
rxvt_resize_sub_windows (rxvt_t* r)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_resize_sub_windows\n"));

	XMoveResizeWindow (r->Xdisplay, PVTS(r)->vt,
	    r->h->window_vt_x, r->h->window_vt_y,
	    VT_WIDTH(r), VT_HEIGHT(r));
}


/* Resize windows on configurenotify event */
/* INTPROTO */
void
rxvt_resize_on_configure (rxvt_t* r, unsigned int width, unsigned int height)
{
    unsigned int    old_width = r->szHint.width,
		    old_height = r->szHint.height;
    int		    fix_screen;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "%s(r, width=%u, height=%u)\n", __func__, width, height));
    /* update ncol/nrow of new window */
    fix_screen = rxvt_calc_colrow (r, width, height);

    /*
     * Check for refresh. If called XResizeWindow, and the WM politely refused
     * to resize window, then we should still process the resize event.
     */
    if (r->h->want_resize ||
	    old_width != r->szHint.width || old_height != r->szHint.height)
	rxvt_resize_sub_windows (r);

    if (fix_screen)
    {
	    int		curr_screen = -1;
	    uint16_t	old_ncol;

	    /*
	     * Update previous columns and rows for each VT. We need these
	     * copies so that rxvt_scr_reset can work for each VT. If all VT
	     * only share one copy, the first VT that calls rxvt_scr_reset will
	     * update it, thus other VT will not correctly adjust their
	     * column/row.
	     */
	    PVTS(r)->prev_nrow = r->h->prev_nrow;
	    PVTS(r)->prev_ncol = r->h->prev_ncol;
	    old_ncol = PVTS(r)->prev_ncol;

	    rxvt_scr_clear(r);

	    curr_screen = rxvt_scr_change_screen(r, PRIMARY);
	    /* scr_reset only works on the primary screen */
	    rxvt_scr_reset(r);
	    if (curr_screen >= 0)
	    {
		/* this is not the first time through */
		rxvt_scr_change_screen(r, curr_screen);
		rxvt_selection_check(r, (old_ncol != r->TermWin.ncol ? 4 : 0));
	    }
    }

#ifdef USE_XIM
    rxvt_IM_resize(r);
#endif
}


/*
 * Get the size of the borders the Window Manager might have put around the
 * window. Returns true if it succeeded reading the struts. Else returns false,
 * and puts 0 in left / right / top / bottom.
 */
#ifndef NO_FRILLS
Bool getWMStruts( Display *dpy, Window w,
	CARD32 *left, CARD32 *right, CARD32 *top, CARD32 *bottom)
{

    Atom atom;
    static const int natoms = 2;
    static const char *atomName[2] = {
	"_KDE_NET_WM_FRAME_STRUT",
	"_NET_FRAME_EXTENTS"};
    int i;

    Atom	    type;
    int		    format;
    unsigned long   nitems, bytes_after;

    unsigned char  *prop; /* left, right, top and bottom borders */

    /*
     * Initialize return values to 0, just incase we can't read the window
     * property.
     */
    *left = *right = *top = *bottom = 0;

    /* Now try and read the property */
    for( i=0; i < natoms; i++)
    {
	atom = XInternAtom( dpy, atomName[i], False);
	if (NOT_ATOM(atom))
	    continue;

	if( XGetWindowProperty( dpy, w, atom,
		0,		/* offset */
		4,		/* length */
		False,		/* Delete? */
		XA_CARDINAL,	/* Type */
		&type, &format, &nitems, &bytes_after, /* return values */
		&prop) != Success )
	    continue;

	if(
	    type == XA_CARDINAL && bytes_after == 0 &&
	    format == 32 && nitems == 4
	  )
	{
	    CARD32  *struts = (CARD32*) prop;

	    *left   = struts[0];
	    *right  = struts[1];
	    *top    = struts[2];
	    *bottom = struts[3];

	    rxvt_dbgmsg((DBG_DEBUG, DBG_COMMAND,
			"%s: %lu, %lu, %lu, %lu (format=%d, nitems=%lu)\n",
			atomName[i], *left, *right, *top, *bottom,
			format, nitems ));
	    /*
	     * Don't check future atomName's
	     */
	    i = natoms;
	}

	XFree( prop);
    }
    return i == natoms + 1;
}
#endif

/* INTPROTO */
void
rxvt_process_configurenotify (rxvt_t* r, XConfigureEvent* ev)
{
    unsigned int    height, width;

    if (ev->window != r->TermWin.parent)
	return;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ConfigureNotify event\n"));
    do		    /* Wrap lots of configures into one */
      {
	width = ev->width;
	height = ev->height;
      }
    while ( XCheckTypedWindowEvent(r->Xdisplay, ev->window,
		    ConfigureNotify, (XEvent*) ev));

    /*
     * Remember the position of the window. Make sure that we remember the
     * position relative to the root window (for root transparency). Notice this
     * position will be offset by the Window manager.
     */
    XTranslateCoordinates(r->Xdisplay, r->TermWin.parent, XROOT,
	0, 0, &r->szHint.x, &r->szHint.y, &ev->window);

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Geometry: %ux%u+%d+%d\n", width, height, r->szHint.x, r->szHint.y));

    if (
	    r->h->want_resize
	    || r->szHint.width != width || r->szHint.height != height
       )
    {

	rxvt_resize_on_configure (r, width, height);
    }
#ifdef DEBUG
    else
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Size: Not resizing\n"));
    }
#endif

    if( (r->h->want_resize & ~FORCE_REFRESH) )
    {
	r->h->want_resize = 0;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Forcing refresh (from want_resize)\n"));

	/*
	 * Otherwise we should refresh the screen. A lazy refresh is enough, as
	 * we will recieve expose events shortly.
	 */
	/* rxvt_src_clear required? */
	rxvt_scr_touch( r, False);
    }
}


/* INTPROTO */
void
rxvt_process_selectionnotify (rxvt_t* r, XSelectionEvent* ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "SelectionNotify event\n"));
    if (r->h->selection_wait == Sel_normal)
	rxvt_selection_paste(r, ev->requestor, ev->property, True);
}


/* INTPROTO */
void
rxvt_process_propertynotify (rxvt_t* r, XEvent* ev)
{
#ifdef DEBUG
    char *name;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Property notify event:"));
#endif

    assert( ev->type == PropertyNotify );
    /*
     * Wrap all property notify events in one loop. This is *required* so that
     * we don't run the transparency updates too often (or worse still, with a
     * bad pixmap).
     */
    do
      {
#ifdef DEBUG
	name = XGetAtomName( r->Xdisplay, ev->xproperty.atom);
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s(%08lx) ", name, ev->xproperty.atom ));
	XFree( name);
#endif

	if (
		ev->xproperty.atom == r->h->xa[XA_VT_SELECTION]
		&& ev->xproperty.state == PropertyNewValue
	   )
	{
	    rxvt_selection_property(r, ev->xproperty.window, ev->xproperty.atom);
	}

      }
    while( XCheckTypedEvent( r->Xdisplay, PropertyNotify, ev) );

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "\n"));
#endif
}


/*
 * ev must either be Expose or GraphicsExpose.
 */
/* INTPROTO */
void
rxvt_process_expose (rxvt_t* r, XEvent* ev)
{
    Window	win = (ev->type == Expose ?
			ev->xexpose.window : ev->xgraphicsexpose.drawable );

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Got %s event.\n", ev->type == Expose ? "Expose" : "GraphicsExpose"));
    if (win == PVTS(r)->vt)
    {
	/*
	 * We need to be clever about exposes (especially under oober slow Xft).
	 * We group exposes, and clip to the appropriate locations.
	 */

	if (NOT_REGION(r->h->refreshRegion))
	{
	    r->h->refreshRegion = XCreateRegion();
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Created clip region %p\n", r->h->refreshRegion));
	    /* Will be freed when rxvt_scr_refresh completes */
	}

	do
	  {
	    XRectangle rect;

	    if( ev->type == Expose)
	    {
		rect.x	    = ev->xexpose.x;
		rect.y	    = ev->xexpose.y;
		rect.width  = ev->xexpose.width;
		rect.height = ev->xexpose.height;
	    }
	    else
	    {
		rect.x	    = ev->xgraphicsexpose.x;
		rect.y	    = ev->xgraphicsexpose.y;
		rect.width  = ev->xgraphicsexpose.width;
		rect.height = ev->xgraphicsexpose.height;
	    }

	    /*
	     * BUG# 1473987: We sometimes receive expose events when the window
	     * contents are not completely cleared.
	     *
	     * If this does not fix the bug, then we will have to issue an
	     * XClearArea() regardless of weather send_event is true or false.
	     */
#if 0
	    /*
	     * 2006-05-20 gi1242: This did not fix the bug, so we comment out
	     * the test for send_event.
	     */
	    if( ev->xexpose.send_event )
#endif
		XClearArea( r->Xdisplay, win, rect.x, rect.y,
			rect.width, rect.height, False );

	    XUnionRectWithRegion( &rect, r->h->refreshRegion,
		    r->h->refreshRegion);
	    rxvt_scr_expose(r,
		    rect.x, rect.y, rect.width, rect.height,
		    False); /* Don't generate a screen refresh */


	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Adding [%ux%u+%d+%d] to expose region\n", rect.width, rect.height, rect.x, rect.y));
	  }
	while(
		XCheckTypedWindowEvent( r->Xdisplay, win, Expose, ev)
		|| XCheckTypedWindowEvent( r->Xdisplay, win, GraphicsExpose, ev)
	     );

	r->h->refresh_type |= CLIPPED_REFRESH;
	r->h->want_clip_refresh = 1;
    }
    else
    {
	XEvent	    unused_xevent;

	/* fetch all expose events if possible */
	while(	XCheckTypedWindowEvent(r->Xdisplay, win,
		    Expose, &unused_xevent) ||
		XCheckTypedWindowEvent( r->Xdisplay, win,
		    GraphicsExpose, &unused_xevent)
	     );

	/* Not reached */
    }
}


/* INTPROTO */
void
rxvt_process_motionnotify (rxvt_t* r, XEvent* ev)
{
    Window	    unused_root, unused_child;
    int		    unused_root_x, unused_root_y;
    unsigned int    unused_mask;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "MotionNotify event\n"));
#ifdef POINTER_BLANK
    if (ISSET_OPTION(r, Opt_pointerBlank) && PVTS(r)->hidden_pointer)
	rxvt_pointer_unblank (r);
#endif

    if (ISSET_PMODE(r, PrivMode_mouse_report) &&
	!(r->h->bypass_keystate))
	return;

    if (ev->xany.window == PVTS(r)->vt)
    {
	if ((ev->xbutton.state & (Button1Mask | Button3Mask)))
	{
	    while (XCheckTypedWindowEvent(r->Xdisplay, PVTS(r)->vt,
		MotionNotify, ev))
		;
	    XQueryPointer(r->Xdisplay, PVTS(r)->vt,
		&unused_root, &unused_child,
		&unused_root_x, &unused_root_y,
		&(ev->xbutton.x), &(ev->xbutton.y),
		&unused_mask);
#ifdef MOUSE_THRESHOLD
	    /* deal with a `jumpy' mouse */
	    if ((ev->xmotion.time - r->h->MEvent.time) > MOUSE_THRESHOLD)
	    {
#endif
		rxvt_selection_extend(r,
		    (ev->xbutton.x), (ev->xbutton.y),
		    (ev->xbutton.state & Button3Mask) ? 2 : 0);
#ifdef SELECTION_SCROLLING
		if (
			ev->xbutton.y<r->TermWin.int_bwidth
			|| Pixel2Row(ev->xbutton.y)>(r->TermWin.nrow-1)
		   )
		{
		    int dist;

		    r->h->pending_scroll_selection=1;

		    /*
		    ** don't clobber the current delay if we are
		    ** already in the middle of scrolling.
		    */
		if (r->h->scroll_selection_delay<=0)
		    r->h->scroll_selection_delay = SCROLLBAR_CONTINUOUS_DELAY;

		    /*
		    ** save the event params so we can highlight
		    ** the selection in the pending-scroll loop
		    */
		r->h->selection_save_x = ev->xbutton.x;
		r->h->selection_save_y = ev->xbutton.y;
		r->h->selection_save_state = (ev->xbutton.state & Button3Mask) ? 2 : 0;

		    /* calc number of lines to scroll */
		    if (ev->xbutton.y<r->TermWin.int_bwidth)
		    {
			r->h->scroll_selection_dir = UP;
			dist = r->TermWin.int_bwidth - ev->xbutton.y;
		    }
		    else
		    {
			r->h->scroll_selection_dir = DN;
			dist = ev->xbutton.y -
			(r->TermWin.int_bwidth + VT_HEIGHT(r));
		    }
		    r->h->scroll_selection_lines=(Pixel2Height(dist)/
			SELECTION_SCROLL_LINE_SPEEDUP)+1;
		    MIN_IT(r->h->scroll_selection_lines,
			SELECTION_SCROLL_MAX_LINES);
		}
		else
		{
		    /*
		    ** we are within the text window, so we
		    ** shouldn't be scrolling
		    */
		    r->h->pending_scroll_selection = 0;
		}
#endif
#ifdef MOUSE_THRESHOLD
	    }
#endif
	}
    }
}

/*{{{ process an X event */
/* INTPROTO */
void
rxvt_process_x_event(rxvt_t* r, XEvent *ev)
{
    int		    i, want_timeout = 0;
    struct timeval  tp;
    struct rxvt_hidden *h = r->h;
#ifdef DEBUG_X
    const char *const eventnames[] =
    {		    /* mason - this matches my system */
	"",
	"",
	"KeyPress",
	"KeyRelease",
	"ButtonPress",
	"ButtonRelease",
	"MotionNotify",
	"EnterNotify",
	"LeaveNotify",
	"FocusIn",
	"FocusOut",
	"KeymapNotify",
	"Expose",
	"GraphicsExpose",
	"NoExpose",
	"VisibilityNotify",
	"CreateNotify",
	"DestroyNotify",
	"UnmapNotify",
	"MapNotify",
	"MapRequest",
	"ReparentNotify",
	"ConfigureNotify",
	"ConfigureRequest",
	"GravityNotify",
	"ResizeRequest",
	"CirculateNotify",
	"CirculateRequest",
	"PropertyNotify",
	"SelectionClear",
	"SelectionRequest",
	"SelectionNotify",
	"ColormapNotify",
	"ClientMessage",
	"MappingNotify"
    };
    struct tm	  *ltt;
#endif

    /*
     * check if we need to get the time for any timeouts
     */

    for (i = NUM_TIMEOUTS; i--; )
	if (h->timeout[i].tv_sec)
	{
	    want_timeout = 1;
	    break;
	}

#ifndef DEBUG_X
    if (want_timeout)
#endif
	(void)gettimeofday(&tp, NULL);

#ifdef DEBUG_X
    ltt = localtime(&(tp.tv_sec));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Event: %-16s %-7s %08lx (%4d-%02d-%02d %02d:%02d:%02d.%.6ld) %s %lu\n", eventnames[ev->type], (ev->xany.window == r->TermWin.parent ? "parent" : (ev->xany.window == PVTS(r)->vt ? "vt" : "UNKNOWN")), (ev->xany.window == r->TermWin.parent ? r->TermWin.parent : (ev->xany.window == PVTS(r)->vt ? PVTS(r)->vt : 0)), ltt->tm_year + 1900, ltt->tm_mon + 1, ltt->tm_mday, ltt->tm_hour, ltt->tm_min, ltt->tm_sec, tp.tv_usec, ev->xany.send_event ? "S" : " ", ev->xany.serial));
#endif

    /* X event timeouts */
    if (want_timeout)
    {
	for (i = NUM_TIMEOUTS; i--; )
	{
	    if (
		    h->timeout[i].tv_sec == 0		||
		    tp.tv_sec < h->timeout[i].tv_sec	||
		    (
		      tp.tv_sec == h->timeout[i].tv_sec	    &&
		      tp.tv_usec < h->timeout[i].tv_usec
		    )
	       )
		continue;

	    /* We timed out on the "i"th timeout */
	    h->timeout[i].tv_sec = 0;
	    switch(i)
	    {
		case TIMEOUT_INCR:
		    rxvt_msg (DBG_ERROR, DBG_COMMAND,
			"data loss: timeout on INCR selection paste");
		    h->selection_wait = Sel_none;
		    break;

		default:
		    break;
	    }
	}
    }


    switch (ev->type)
    {
	case KeyPress:
	    rxvt_process_keypress (r, (XKeyEvent *) ev);
	    break;

#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
	case KeyRelease:
	    rxvt_process_keyrelease (r, (XKeyEvent*) ev);
	    break;
#endif

	case ButtonPress:
	    rxvt_process_buttonpress(r, (XButtonEvent *) ev);
	    break;

	case ButtonRelease:
	    rxvt_process_buttonrelease(r, (XButtonEvent *) ev);
	    break;

	case ClientMessage:
	    rxvt_process_clientmessage(r, (XClientMessageEvent*) ev);
	    break;

	case MappingNotify:
	    XRefreshKeyboardMapping(&(ev->xmapping));
	    break;

	/*
	 * XXX: this is not the _current_ arrangement
	 * Here's my conclusion:
	 * If the window is completely unobscured, use bitblt's
	 * to scroll. Even then, they're only used when doing partial
	 * screen scrolling. When partially obscured, we have to fill
	 * in the GraphicsExpose parts, which means that after each refresh,
	 * we need to wait for the graphics expose or Noexpose events,
	 * which ought to make things real slow!
	 */
	case VisibilityNotify:
	    rxvt_process_visibilitynotify (r, (XVisibilityEvent*) ev);
	    break;

#ifdef MONITOR_ENTER_LEAVE
	case EnterNotify:
	    rxvt_process_enter (r, (XCrossingEvent*) ev);
	    break;

	case LeaveNotify:
	    rxvt_process_leave (r, (XCrossingEvent*) ev);
	    break;
#endif	/* MONITOR_ENTER_LEAVE */

	case FocusIn:
	case FocusOut:
	    rxvt_process_focus (r, (XFocusChangeEvent*) ev);
	    break;

	case ConfigureNotify:
	    rxvt_process_configurenotify (r, (XConfigureEvent*) ev);
	    break;

	case SelectionClear:
	    rxvt_process_selectionclear(r);
	    break;

	case SelectionNotify:
	    rxvt_process_selectionnotify (r, (XSelectionEvent*) ev);
	    break;

	case SelectionRequest:
	    rxvt_process_selectionrequest (r, (XSelectionRequestEvent*) ev);
	    break;

	case UnmapNotify:
	    r->TermWin.mapped = 0;
	    break;

	case MapNotify:
	    r->TermWin.mapped = 1;
	    break;

	case PropertyNotify:
	    rxvt_process_propertynotify (r, ev);
	    break;

	case ReparentNotify:
	    rxvt_process_reparentnotify (r, ev);
	    break;

	case GraphicsExpose:
	case Expose:
	    rxvt_process_expose (r, ev);
	    break;

	case MotionNotify:
	    rxvt_process_motionnotify (r, ev);
	    break;
    }
}

/*}}} */

/* *INDENT-OFF* */
enum {
    C1_40 = 0x40,
	C1_41 , C1_BPH, C1_NBH, C1_44 , C1_NEL, C1_SSA, C1_ESA,
    C1_HTS, C1_HTJ, C1_VTS, C1_PLD, C1_PLU, C1_RI , C1_SS2, C1_SS3,
    C1_DCS, C1_PU1, C1_PU2, C1_STS, C1_CCH, C1_MW , C1_SPA, C1_EPA,
    C1_SOS, C1_59 , C1_SCI, C1_CSI, CS_ST , C1_OSC, C1_PM , C1_APC
};
/* *INDENT-ON* */

/*
 * Set outbuf_escfail to the place where processing an escape sequence failed.
 * nchars is the minimum number of chars needed to be read
 */
/* INTPROTO */
void
rxvt_set_escfail( rxvt_t *r, int nchars )
{
    assert( PVTS(r)->outbuf_escstart );

    rxvt_check_cmdbuf( r );
    PVTS(r)->outbuf_escfail = PVTS(r)->outbuf_start + nchars - 1;

    if( PVTS(r)->outbuf_escfail > PVTS(r)->outbuf_base + BUFSIZ-3 )
    {
	/*
	 * Escape sequence was longer than BUFSIZ. Just skip the escape
	 * character and go on like normal
	 */
	PVTS(r)->outbuf_start = PVTS(r)->outbuf_escstart + 1;
	SET_NULL( PVTS(r)->outbuf_escstart );
	SET_NULL( PVTS(r)->outbuf_escfail );
    }

    else
	/* Fall back to start of escape sequence */
	PVTS(r)->outbuf_start = PVTS(r)->outbuf_escstart;
}

/*{{{ process non-printing single characters */
/* INTPROTO */
void
rxvt_process_nonprinting(rxvt_t* r, unsigned char ch)
{
    switch (ch)
    {
	case C0_ENQ:	/* terminal Status */
	    if (r->h->rs[Rs_answerbackstring])
		rxvt_tt_write(r,
		    (const unsigned char *)r->h->rs[Rs_answerbackstring],
		    (unsigned int)STRLEN(r->h->rs[Rs_answerbackstring]));
	    else
		rxvt_tt_write(r, (unsigned char *)VT100_ANS,
		    (unsigned int)STRLEN(VT100_ANS));
	    break;

	case C0_BEL:	/* bell */
	    rxvt_scr_bell(r);
	    break;

	case C0_BS:	/* backspace */
	    rxvt_scr_backspace(r);
	    break;

	case C0_HT:	/* tab */
	    rxvt_scr_tab(r, 1);
	    break;

	case C0_CR:	/* carriage return */
	    rxvt_scr_gotorc(r, 0, 0, R_RELATIVE);
	    break;

	case C0_VT:	/* vertical tab, form feed */
	case C0_FF:
	case C0_LF:	/* line feed */
	    rxvt_scr_index(r, UP);
	    break;

	case C0_SO:	/* shift out - acs */
	    rxvt_scr_charset_choose(r, 1);
	    break;

	case C0_SI:	/* shift in - acs */
	    rxvt_scr_charset_choose(r, 0);
	    break;
    }
}
/*}}} */


/*{{{ process VT52 escape sequences */
/* INTPROTO */
void
rxvt_process_escape_vt52(rxvt_t* r, unsigned char ch)
{
    int	    row, col;

    switch (ch)
    {
	case 'A':	/* cursor up */
	    rxvt_scr_gotorc(r, -1, 0, R_RELATIVE | C_RELATIVE);
	    break;

	case 'B':	/* cursor down */
	    rxvt_scr_gotorc(r, 1, 0, R_RELATIVE | C_RELATIVE);
	    break;

	case 'C':	/* cursor right */
	    rxvt_scr_gotorc(r, 0, 1, R_RELATIVE | C_RELATIVE);
	    break;

	case 'D':	/* cursor left */
	    rxvt_scr_gotorc(r, 0, -1, R_RELATIVE | C_RELATIVE);
	    break;

	case 'H':	/* cursor home */
	    rxvt_scr_gotorc(r, 0, 0, 0);
	    break;

	case 'I':	/* cursor up and scroll down if needed */
	    rxvt_scr_index(r, DN);
	    break;

	case 'J':	/* erase to end of screen */
	    rxvt_scr_erase_screen(r, 0);
	    break;

	case 'K':	/* erase to end of line */
	    rxvt_scr_erase_line(r, 0);
	    break;

	case 'Y':	    /* move to specified row and col */
	    /* full command is 'ESC Y row col' where row and col
	    ** are encoded by adding 32 and sending the ascii
	    ** character.  eg. SPACE = 0, '+' = 13, '0' = 18,
	    ** etc.
	    */
	    row = rxvt_cmd_getc(r);
	    if( row == -1 )
	    {
		rxvt_set_escfail( r, 2 );
		break;
	    }
	    row -= ' ';

	    col = rxvt_cmd_getc(r);
	    if( col == -1 )
	    {
		rxvt_set_escfail( r, 1 );
		break;
	    }
	    row -= ' ';

	    rxvt_scr_gotorc(r, row, col, 0);
	    break;

	case 'Z':	/* identify the terminal type */
	    /* I am a VT100 emulating a VT52 */
	    rxvt_tt_printf(r, "\033/Z");
	    break;

	case '<':	/* turn off VT52 mode */
	    PrivMode(0, PrivMode_vt52);
	    break;

	case 'F':    	/* use special graphics character set */
	case 'G':	   /* use regular character set */
	    /* unimplemented */
	    break;

	case '=':    	/* use alternate keypad mode */
	case '>':	   /* use regular keypad mode */
	    /* unimplemented */
	    break;
    }
}
/*}}} */


/*{{{ process escape sequences */
/* INTPROTO */
void
rxvt_process_escape_seq(rxvt_t* r)
{
    int             c,
		    ch = rxvt_cmd_getc(r);

    if( ch == -1 )
    {
	rxvt_set_escfail( r, 1 );
	return;
    }

    if (ISSET_PMODE(r, PrivMode_vt52))
    {
	rxvt_process_escape_vt52(r, ch);
	return;
    }

    switch (ch)
    {
	/* case 1:	do_tek_mode (); break; */
	case '#':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else if( c == 8 )
		rxvt_scr_E(r);

	    break;

	case '(':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else
		rxvt_scr_charset_set(r, 0, (unsigned int) c );
	    break;

	case ')':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else
		rxvt_scr_charset_set(r, 1, (unsigned int) c);

	    break;

	case '*':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else
		rxvt_scr_charset_set(r, 2, (unsigned int) c);

	    break;

	case '+':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else
		rxvt_scr_charset_set(r, 3, (unsigned int) c);

	    break;

#ifdef MULTICHAR_SET
	case '$':
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );
	    else
		rxvt_scr_charset_set(r, -2, (unsigned int) c);

	    break;
#endif

#ifndef NO_FRILLS
	case '6':
	    rxvt_scr_backindex(r);
	    break;
#endif
	case '7':
	    rxvt_scr_cursor(r, SAVE);
	    break;
	case '8':
	    rxvt_scr_cursor(r, RESTORE);
	    break;
#ifndef NO_FRILLS
	case '9':
	    rxvt_scr_forwardindex(r);
	    break;
#endif
	case '=':
	case '>':
	    PrivMode((ch == '='), PrivMode_aplKP);
	    break;

	case C1_40:
	    c = rxvt_cmd_getc( r );

	    if( c == -1 )
		rxvt_set_escfail( r, 1 );

	    /* 2006-08-31 gi1242 XXX Why is there no code here? */

	    break;

	case C1_44:
	    rxvt_scr_index(r, UP);
	    break;

	/* 8.3.87: NEXT LINE */
	case C1_NEL:	    /* ESC E */
	    rxvt_scr_add_lines(r, (const unsigned char *)"\n\r", 1, 2);
	    break;

	/* kidnapped escape sequence: Should be 8.3.48 */
	case C1_ESA:	    /* ESC G */
	    rxvt_process_graphics(r);
	    break;

	/* 8.3.63: CHARACTER TABULATION SET */
	case C1_HTS:	    /* ESC H */
	    rxvt_scr_set_tab(r, 1);
	    break;

	/* 8.3.105: REVERSE LINE FEED */
	case C1_RI:	    /* ESC M */
	    rxvt_scr_index(r, DN);
	    break;

	/* 8.3.142: SINGLE-SHIFT TWO */
	/*case C1_SS2: scr_single_shift (2);   break; */

	/* 8.3.143: SINGLE-SHIFT THREE */
	/*case C1_SS3: scr_single_shift (3);   break; */

	/* 8.3.27: DEVICE CONTROL STRING */
	case C1_DCS:	    /* ESC P */
	    /* rxvt_process_dcs_seq(r); */
	    rxvt_process_xwsh_seq (r);
	    break;

	/* 8.3.110: SINGLE CHARACTER INTRODUCER */
	case C1_SCI:	    /* ESC Z */
	    rxvt_tt_write(r, (const unsigned char *)ESCZ_ANSWER,
		  (unsigned int)(sizeof(ESCZ_ANSWER) - 1));
	    break;	    /* steal obsolete ESC [ c */

	/* 8.3.16: CONTROL SEQUENCE INTRODUCER */
	case C1_CSI:	    /* ESC [ */
	    rxvt_process_csi_seq(r);
	    break;

	/* 8.3.90: OPERATING SYSTEM COMMAND */
	case C1_OSC:	    /* ESC ] */
	    rxvt_process_osc_seq(r);
	    break;

	/* 8.3.106: RESET TO INITIAL STATE */
	case 'c':
	    rxvt_scr_poweron(r);
	    break;

	/* 8.3.79: LOCKING-SHIFT TWO (see ISO2022) */
	case 'n':
	    rxvt_scr_charset_choose(r, 2);
	    break;

	/* 8.3.81: LOCKING-SHIFT THREE (see ISO2022) */
	case 'o':
	    rxvt_scr_charset_choose(r, 3);
	    break;
    }
}
/*}}} */


/*{{{ process CONTROL SEQUENCE INTRODUCER (CSI) sequences `ESC[' */
/* *INDENT-OFF* */
enum {
    CSI_ICH = 0x40,
    CSI_CUU, CSI_CUD, CSI_CUF, CSI_CUB, CSI_CNL, CSI_CPL, CSI_CHA,
    CSI_CUP, CSI_CHT, CSI_ED , CSI_EL , CSI_IL , CSI_DL , CSI_EF , CSI_EA ,
    CSI_DCH, CSI_SEE, CSI_CPR, CSI_SU , CSI_SD , CSI_NP , CSI_PP , CSI_CTC,
    CSI_ECH, CSI_CVT, CSI_CBT, CSI_SRS, CSI_PTX, CSI_SDS, CSI_SIMD, CSI_5F,
    CSI_HPA, CSI_HPR, CSI_REP, CSI_DA , CSI_VPA, CSI_VPR, CSI_HVP, CSI_TBC,
    CSI_SM , CSI_MC , CSI_HPB, CSI_VPB, CSI_RM , CSI_SGR, CSI_DSR, CSI_DAQ,
    CSI_70 , CSI_71 , CSI_72 , CSI_73 , CSI_74 , CSI_75 , CSI_76 , CSI_77 ,
    CSI_78 , CSI_79 , CSI_7A , CSI_7B , CSI_7C , CSI_7D , CSI_7E , CSI_7F 
};

#define make_byte(b7,b6,b5,b4,b3,b2,b1,b0)	    \
    (((b7) << 7) | ((b6) << 6) | ((b5) << 5) | ((b4) << 4)  \
     | ((b3) << 3) | ((b2) << 2) | ((b1) << 1) | (b0))
#define get_byte_array_bit(array, bit)		    \
    (!!((array)[(bit) / 8] & (128 >> ((bit) & 7))))

const unsigned char csi_defaults[] = {
    make_byte(1,1,1,1,1,1,1,1),	/* @, A, B, C, D, E, F, G, */
    make_byte(1,1,0,0,1,1,0,0),	/* H, I, J, K, L, M, N, O, */
    make_byte(1,0,1,1,1,1,1,0),	/* P, Q, R, S, T, U, V, W, */
    make_byte(1,1,1,0,0,0,1,0),	/* X, Y, Z, [, \, ], ^, _, */
    make_byte(1,1,1,0,1,1,1,0),	/* `, a, b, c, d, e, f, g, */
    make_byte(0,0,1,1,0,0,0,0),	/* h, i, j, k, l, m, n, o, */
    make_byte(0,0,0,0,0,0,0,0),	/* p, q, r, s, t, u, v, w, */
    make_byte(0,0,0,0,0,0,0,0)	/* x, y, z, {, |, }, ~,	*/
};
/* *INDENT-ON* */


/* INTPROTO */
void
rxvt_process_csi_seq(rxvt_t* r)
{
    int             ch;
    unsigned char   priv, i;
    unsigned int    nargs, p;
    int		    n, ndef;
    int		    arg[ESC_ARGS];


    for (nargs = ESC_ARGS; nargs > 0;)
	arg[--nargs] = 0;

    priv = 0;
    ch = rxvt_cmd_getc(r);
    if( ch == -1 )
    {
	rxvt_set_escfail( r, 1 );
	return;
    }

    if (ch >= '<' && ch <= '?')		/* '<' '=' '>' '?' */
    {
	priv = ch;
	ch = rxvt_cmd_getc(r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}
    }

    /* read any numerical arguments */
    for (n = -1; ch < CSI_ICH; )
    {
	if (isdigit(ch))
	{
	    if (n < 0)
		n = ch - '0';
	    else
		n = n * 10 + ch - '0';
	}
	else if (ch == ';')
	{
	    if (nargs < ESC_ARGS)
		arg[nargs++] = n;
	    n = -1;
	}
	else if (ch == '\b')
	{
	    rxvt_scr_backspace(r);
	}
	else if (ch == C0_ESC)
	{
	    rxvt_process_escape_seq(r);
	    return;
	}
	else if (ch < ' ')
	{
	    rxvt_process_nonprinting(r, ch);
	}

	ch = rxvt_cmd_getc(r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}
    }

    if (ch > CSI_7F)
	return;

    if (nargs < ESC_ARGS)
    arg[nargs++] = n;

    i = ch - CSI_ICH;
    ndef = get_byte_array_bit(csi_defaults, i);
    for (p = 0; p < nargs; p++)
	if (arg[p] == -1)
	    arg[p] = ndef;

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "CSI "));
    for (p = 0; p < nargs; p++)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%d%s", arg[p], p < nargs - 1 ? ";" : ""));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c\n", ch));
#endif


    /* private mode handling */
    if (priv)
    {
	switch (priv)
	{
	case '>':
	    if (ch == CSI_DA)	/* secondary device attributes */
	    rxvt_tt_printf(r, "\033[>%d;%-.8s;0c", 'R', VERSION);
	    break;
	case '?':
	    if (ch == 'h' || ch == 'l' || ch == 'r' || ch == 's' || ch == 't')
	    {
		rxvt_process_terminal_mode(r, ch, priv, nargs, arg);
	    }
	    break;
	}
	return;
    }


    switch (ch)
    {
	/* ISO/IEC 6429:1992(E) CSI sequences (defaults in parentheses) */
	case CSI_CUU:	    /* 8.3.22: (1) CURSOR UP */
	case CSI_VPR:	    /* 8.3.161: (1) LINE POSITION FORWARD */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CUD:	    /* 8.3.19: (1) CURSOR DOWN */
	case CSI_VPB:	    /* 8.3.160: (1) LINE POSITION BACKWARD */
	    rxvt_scr_gotorc(r, arg[0], 0, RELATIVE);
	    break;

	case CSI_CUB:	    /* 8.3.18: (1) CURSOR LEFT */
	case CSI_HPB: 	    /* 8.3.59: (1) CHARACTER POSITION BACKWARD */
#ifdef ISO6429
	    arg[0] = -arg[0];
#else		    /* emulate common DEC VTs */
	    arg[0] = arg[0] ? -arg[0] : -1;
#endif
	    /* FALLTHROUGH */
	case CSI_CUF:	    /* 8.3.20: (1) CURSOR RIGHT */
	case CSI_HPR:	    /* 8.3.60: (1) CHARACTER POSITION FORWARD */
#ifdef ISO6429
	    rxvt_scr_gotorc(r, 0, arg[0], RELATIVE);
#else		    /* emulate common DEC VTs */
	    rxvt_scr_gotorc(r, 0, arg[0] ? arg[0] : 1, RELATIVE);
#endif
	    break;

	case CSI_CPL:	    /* 8.3.13: (1) CURSOR PRECEDING LINE */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CNL:	    /* 8.3.12: (1) CURSOR NEXT LINE */
	    rxvt_scr_gotorc(r, arg[0], 0, R_RELATIVE);
	    break;

	case CSI_CHA:	    /* 8.3.9: (1) CURSOR CHARACTER ABSOLUTE */
	case CSI_HPA:	    /* 8.3.58: (1) CURSOR POSITION ABSOLUTE */
	    rxvt_scr_gotorc(r, 0, arg[0] - 1, R_RELATIVE);
	    break;

	case CSI_VPA:	    /* 8.3.159: (1) LINE POSITION ABSOLUTE */
	    rxvt_scr_gotorc(r, arg[0] - 1, 0, C_RELATIVE);
	    break;

	case CSI_CUP:	    /* 8.3.21: (1,1) CURSOR POSITION */
	case CSI_HVP:	    /* 8.3.64: (1,1) CHARACTER AND LINE POSITION */
	    rxvt_scr_gotorc(r, arg[0] - 1, nargs < 2 ? 0 : (arg[1] - 1), 0);
	    break;

	case CSI_CBT:	    /* 8.3.7: (1) CURSOR BACKWARD TABULATION */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CHT:	    /* 8.3.10: (1) CURSOR FORWARD TABULATION */
	    rxvt_scr_tab(r, arg[0]);
	    break;

	case CSI_ED:	    /* 8.3.40: (0) ERASE IN PAGE */
	    rxvt_scr_erase_screen(r, arg[0]);
	    break;

	case CSI_EL:	    /* 8.3.42: (0) ERASE IN LINE */
	    rxvt_scr_erase_line(r, arg[0]);
	    break;

	case CSI_ICH:	    /* 8.3.65: (1) INSERT CHARACTER */
	    rxvt_scr_insdel_chars(r, arg[0], INSERT);
	    break;

	case CSI_IL:	    /* 8.3.68: (1) INSERT LINE */
	    rxvt_scr_insdel_lines(r, arg[0], INSERT);
	    break;

	case CSI_DL:	    /* 8.3.33: (1) DELETE LINE */
	    rxvt_scr_insdel_lines(r, arg[0], DELETE);
	    break;

	case CSI_ECH:	    /* 8.3.39: (1) ERASE CHARACTER */
	    rxvt_scr_insdel_chars(r, arg[0], ERASE);
	    break;

	case CSI_DCH:	    /* 8.3.26: (1) DELETE CHARACTER */
	    rxvt_scr_insdel_chars(r, arg[0], DELETE);
	    break;

	case CSI_SD:	    /* 8.3.114: (1) SCROLL DOWN */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_SU:	    /* 8.3.148: (1) SCROLL UP */
	    rxvt_scroll_text(r, PVTS(r)->screen.tscroll,
		PVTS(r)->screen.bscroll, arg[0], 0);
	    break;

	case CSI_DA:	    /* 8.3.24: (0) DEVICE ATTRIBUTES */
	    rxvt_tt_write(r, (const unsigned char *)VT100_ANS,
		(unsigned int)(sizeof(VT100_ANS) - 1));
	    break;

	case CSI_SGR:	    /* 8.3.118: (0) SELECT GRAPHIC RENDITION */
	    rxvt_process_sgr_mode(r, nargs, arg);
	    break;

	case CSI_DSR:	    /* 8.3.36: (0) DEVICE STATUS REPORT */
	    switch (arg[0])
	    {
		case 5:		/* DSR requested */
		    rxvt_tt_printf(r, "\033[0n");
		    break;
		case 6:		/* CPR requested */
		    rxvt_scr_report_position(r);
		    break;
#if defined (ENABLE_DISPLAY_ANSWER)
		case 7:		/* unofficial extension */
		    rxvt_tt_printf(r, "%-.250s\n",
			r->h->rs[Rs_display_name]);
		    break;
#endif
		case 8:		/* unofficial extension */
		    rxvt_xterm_seq(r, XTerm_title, APL_NAME "-" VERSION,
			CHAR_ST);
		    break;
	    }	/* switch (arg[0]) */
	    break;

	case CSI_TBC:	    /* 8.3.155: (0) TABULATION CLEAR */
	    switch (arg[0])
	    {
		case 0:		/* char tab stop cleared at active position */
		    rxvt_scr_set_tab(r, 0);
		    break;
		/* case 1: */	    /* line tab stop cleared in active line */
		/* case 2: */	    /* char tab stops cleared in active line */
		case 3:		/* all char tab stops are cleared */
		/* case 4: */	    /* all line tab stops are cleared */
		case 5:		/* all tab stops are cleared */
		    rxvt_scr_set_tab(r, -1);
		    break;
	    }	/* switch (arg[0]) */
	    break;

	case CSI_CTC:	    /* 8.3.17: (0) CURSOR TABULATION CONTROL */
	    switch (arg[0])
	    {
		case 0:		/* char tab stop set at active position */
		    rxvt_scr_set_tab(r, 1);
		    break;	/* = ESC H */
		/* case 1: */	    /* line tab stop set at active line */
		case 2:		/* char tab stop cleared at active position */
		    rxvt_scr_set_tab(r, 0);
		    break;	/* = ESC [ 0 g */
		/* case 3: */	    /* line tab stop cleared at active line */
		/* case 4: */	    /* char tab stops cleared at active line */
		case 5:		/* all char tab stops are cleared */
		    rxvt_scr_set_tab(r, -1);
		    break;	/* = ESC [ 3 g */
		/* case 6: */	    /* all line tab stops are cleared */
	    }	/* switch (arg[0]) */
	    break;

	case CSI_RM:	    /* 8.3.107: RESET MODE */
	    if (arg[0] == 4)
		rxvt_scr_insert_mode(r, 0);
	    break;

	case CSI_SM:	    /* 8.3.126: SET MODE */
	    if (arg[0] == 4)
		rxvt_scr_insert_mode(r, 1);
	    break;

	/* PRIVATE USE beyond this point.  All CSI_7? seqeunces here */ 
	case CSI_72:	    /* DECSTBM: set top and bottom margins */
	    if (nargs == 1)
		rxvt_scr_scroll_region(r, arg[0] - 1, MAX_ROWS - 1);
	    else if (nargs == 0 || arg[0] >= arg[1])
		rxvt_scr_scroll_region(r, 0, MAX_ROWS - 1);
	    else 
		rxvt_scr_scroll_region(r, arg[0] - 1, arg[1] - 1);
	    break;

	case CSI_73:
	    rxvt_scr_cursor(r, SAVE);
	    break;
	case CSI_75:
	    rxvt_scr_cursor(r, RESTORE);
	    break;

#ifndef NO_FRILLS
	case CSI_74:
	    rxvt_process_window_ops(r, arg, nargs);
	    break;
#endif

	case CSI_78:	    /* DECREQTPARM */
	    if (arg[0] == 0 || arg[0] == 1)
		rxvt_tt_printf(r, "\033[%d;1;1;112;112;1;0x",
		    arg[0] + 2);
	    /* FALLTHROUGH */
	default:
	    break;
    }
}
/*}}} */


#ifndef NO_FRILLS
/* ARGSUSED */
/* INTPROTO */
void
rxvt_process_window_ops(rxvt_t* r, const int *args, unsigned int nargs)
{
    int			x, y;
    unsigned int	w, h;
    XWindowAttributes	wattr;
    Window		wdummy;


    if (nargs == 0)
	return;
    switch (args[0])
    {
	/* commands */
	case 1:		/* deiconify window */
	    XMapWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 2:		/* iconify window */
	    XIconifyWindow (r->Xdisplay, r->TermWin.parent,
		DefaultScreen(r->Xdisplay));
	    break;
	case 3:		/* set position (pixels) */
	    XMoveWindow (r->Xdisplay, r->TermWin.parent, args[1], args[2]);
	    break;
	case 4:		/* set size (pixels) */
	    w = (unsigned int) args[2];
	    h = (unsigned int) args[1];
	    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
	    r->h->want_resize = 1;
	    break;
	case 5:		/* raise window */
	    XRaiseWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 6:		/* lower window */
	    XLowerWindow (r->Xdisplay, r->TermWin.parent);
	    break;
	case 7:		/* refresh window */
	    rxvt_scr_touch (r, True);
	    break;
	case 8:		/* set size (chars) */
	    w = (unsigned int) (Width2Pixel(args[2]) + r->szHint.base_width);
	    h = (unsigned int) (Height2Pixel(args[1]) + r->szHint.base_height);
	    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
	    r->h->want_resize = 1;
	    break;
	default:
	    if (args[0] >= 24)
	    {
		/* set height (chars) */
		w = (unsigned int) r->szHint.width;
		h = (unsigned int) (args[1] * r->TermWin.fheight + r->szHint.base_height);
		XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
		r->h->want_resize = 1;
	    }
	    break;

	/* reports - some output format copied from XTerm */
	case 11:	    /* report window state */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    rxvt_tt_printf(r, "\033[%dt",
		wattr.map_state == IsViewable ? 1 : 2);
	    break;
	case 13:	    /* report window position */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    XTranslateCoordinates(r->Xdisplay, r->TermWin.parent,
		wattr.root, -wattr.border_width, -wattr.border_width,
		&x, &y, &wdummy);
	    rxvt_tt_printf(r, "\033[3;%d;%dt", x, y);
	    break;
	case 14:	    /* report window size (pixels) */
	    XGetWindowAttributes(r->Xdisplay, r->TermWin.parent, &wattr);
	    rxvt_tt_printf(r, "\033[4;%d;%dt", wattr.height,
		wattr.width);
	    break;
	case 18:	    /* report window size (chars) */
	    rxvt_tt_printf(r, "\033[8;%d;%dt", r->TermWin.nrow,
		r->TermWin.ncol);
	    break;
    }
}
#endif	/* NO_FRILLS */


/*----------------------------------------------------------------------*/
/*
 * get input up until STRING TERMINATOR (or BEL)
 * ends_how is terminator used.  returned input must be free()d
 */
/* INTPROTO */
unsigned char  *
rxvt_get_to_st(rxvt_t* r, unsigned char *ends_how)
{
    int		    seen_esc = 0;   /* seen escape? */
    unsigned int    n = 0;
    unsigned char*  s;
    int             ch;
    unsigned char   string[STRING_MAX];


    for(;;)
    {
	ch = rxvt_cmd_getc( r );
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return NULL;
	}

	if( !ch )
	    break;

	if(
	     ch == C0_BEL || ch == CHAR_ST ||
	     (ch == 0x5c && seen_esc)	/* 7bit ST */
	  )
	    break;

	if( ch == C0_ESC )
	{
	    seen_esc = 1;
	    continue;
	}
	else if (ch == '\t')
	    ch = ' ';	/* translate '\t' to space */
	else if (ch < 0x08 || (ch > 0x0d && ch < 0x20))
	{
#if 0
	    /*
	     * NO! See http://vt100.net/emu/dec_ansi_parser for reason. I am not
	     * sure what to do with CAN or SUB, but the rest should be *ignored*
	     * - I will try to find out what to do with CAN and SUB. - Johann
	     * 'Mykraverk' Oskarsson <johann@myrkraverk.com>
	     */
	    return NULL;    /* other control character - exit */
#endif

	    continue; /* do nothing */
	}

	if (n < sizeof(string) - 1)
	    string[n++] = ch;

	seen_esc = 0;
    } /* for(;;) */

    string[n++] = '\0';
    if (IS_NULL(s = (unsigned char UNTAINTED *) STRNDUP (string, n)))
	return NULL;

    *ends_how = (ch == 0x5c ? C0_ESC : ch);

    return s;
}


/*----------------------------------------------------------------------*/
/*
 * process DEVICE CONTROL STRING `ESC P ... (ST|BEL)' or `0x90 ... (ST|BEL)'
 */
/* INTPROTO */
void
rxvt_process_dcs_seq(rxvt_t* r)
{
    unsigned char   eh, *s;

    /* Not handled yet */
    s = rxvt_get_to_st (r, &eh);
    if (s)
	rxvt_free(s);
    return;
}

/*----------------------------------------------------------------------*/
/*
 * process OPERATING SYSTEM COMMAND sequence `ESC ] Ps ; Pt (ST|BEL)'
 */
/* INTPROTO */
void
rxvt_process_osc_seq (rxvt_t* r)
{
    int             ch;
    unsigned char   eh, *s;
    int		    arg;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_process_osc_seq( r ).\n"));

    arg = 0;
    for(;;)
    {
	ch = rxvt_cmd_getc(r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}

	if( isdigit(ch) )
	    arg = arg * 10 + (ch - '0');
	else
	    break;
    }


    if (ch == ';')
    {
	s = rxvt_get_to_st(r, &eh);
	if (s)
	{
	    /*
	     * rxvt_menubar_dispatch() violates the constness of the string, so
	     * do it here.
	     */
	    if (arg == XTerm_Menu)
	    {
		/* XXX: currently disabled due to security concerns */
#if 0
		rxvt_menubar_dispatch(r, (char*) s);
#endif
	    }
	    else
		rxvt_xterm_seq(r, arg, (char*) s, eh);

	    rxvt_free(s);
	}
    }
}


/*
 * Xwsh escape sequences: ESC P Ps.y;Pt ESC \
 *       1 = change title
 *       3 = change iconName
 *       4 = set text color by string
 *       5 = set page color by string
 *       6 = set selection text color by string
 *       7 = set selection page color by string
 *       8 = set cursor text color by string
 *       9 = set cursor page color by string
 *      10 = set half intensity by string
 *      11 = set bold intensity by string
 *     101 = bind string to key Ps+1 and pass as value
 *     103 = bind string to key Ps+1 and pass s function
 */
/* INTPROTO */
void
rxvt_xwsh_seq(rxvt_t* r, int op, const char *str)
{
    assert(NOT_NULL(str));
    switch (op)
    {
	case Xwsh_title:
	    rxvt_set_term_title (r, (const unsigned char*) str);
	    break;
	case Xwsh_iconName:
	    rxvt_set_icon_name (r, (const unsigned char*) str);
	    break;
	case Xwsh_textColor:
	    break;
	case Xwsh_pageColor:
	    break;
	case Xwsh_selTextColor:
	    break;
	case Xwsh_selPageColor:
	    break;
	case Xwsh_cursorTextColor:
	    break;
	case Xwsh_cursorPageColor:
	    break;
	case Xwsh_halfIntColor:
	    break;
	case Xwsh_boldIntColor:
	    break;
	case Xwsh_bindStrKeyVal:
	    break;
	case Xwsh_bindStrKeyFunc:
	    break;
    }
}


/* INTPROTO */
void
rxvt_process_xwsh_seq (rxvt_t* r)
{
    int             ch;
    unsigned char   string[STRING_MAX];
    int             arg;


    arg = 0;
    for(;;)
    {
	ch = rxvt_cmd_getc (r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}

	if( isdigit(ch) )
	    arg = arg * 10 + (ch - '0');
	else
	    break;
    }

    if (ch == '.')
    {
	ch = rxvt_cmd_getc(r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}

	if ( ch == 'y')
	{
	    int             n = 0;

	    for(;;)
	    {
		ch = rxvt_cmd_getc( r );
		if( ch == -1 )
		{
		    rxvt_set_escfail( r, 1 );
		    return;
		}

		if( ch == '\033' )
		    break;

		if (ch)
		{
		    if (ch == '\t')
			ch = ' ';   /* translate '\t' to space */
		    else if (ch < ' ')
			return;	/* control character - exit */

		    if (n < sizeof(string) - 1)
			string[n++] = ch;
		}
	    }

	    ch = rxvt_cmd_getc( r );
	    if( ch == -1 )
	    {
		rxvt_set_escfail( r, 1 );
		return;
	    }

	    if( ch == '\\' )
	    {
		string[n] = '\0';
		rxvt_xwsh_seq(r, arg, (char*) string);
	    }
	} /* if( ch == 'y' ) */
    } /* if( ch == '.' ) */
}


/*
 * XTerm escape sequences: ESC ] Ps;Pt (ST|BEL)
 *     0 = change iconName/title
 *     1 = change iconName
 *     2 = change title
 *     4 = change color
 *    12 = change text color
 *    13 = change mouse foreground color 
 *    17 = change highlight character colour
 *    18 = change bold character color
 *    19 = change underlined character color 
 *    46 = change logfile (not implemented)
 *    50 = change font
 *
 * rxvt extensions:
 *    10 = menu (may change in future) (DISABLED)
 *    20 = bg pixmap
 *    39 = change default fg color
 *    49 = change default bg color
 *    55 = dump scrollback buffer and all of screen
 */
/* EXTPROTO */
void
rxvt_xterm_seq(rxvt_t* r, int op, const char *str, unsigned char resp __attribute__((unused)))
{
    int		color;
    char	*buf, *name;


    assert(NOT_NULL(str));
    switch (op)
    {
	case XTerm_title:   /* Set tab / term title */
	case XTerm_name:    /* also set icon title */
	    rxvt_set_term_title(r, (const unsigned char*) str);
	    if( op == XTerm_title )
		break;	/* Don't set the icon name */
	    /* else FALL THROUGH */

	case XTerm_iconName:
	    rxvt_set_icon_name(r, (const unsigned char*) str);
	    /*
	     * 2006-05-30 gi1242: -sti means sync the icon name to the tab
	     * title, and NOT the other way around.
	     */
#if 0
	    if (ISSET_OPTION(r, Opt2_syncTabIcon))
		rxvt_set_term_title (r,
			(const unsigned char TAINTED*) str);
#endif
	    break;

	case XTerm_Color:
	    for (buf = (char *)str; buf && *buf;)
	    {
		if (IS_NULL(name = STRCHR(buf, ';')))
		    break;
		*name++ = '\0';
		color = atoi(buf);
		if (color < 0 || color >= TOTAL_COLORS)
		    break;
		if (NOT_NULL(buf = STRCHR(name, ';')))
		    *buf++ = '\0';
		rxvt_set_window_color(r, color + minCOLOR, name);
	    }
	    break;
#ifndef NO_CURSORCOLOR
	case XTerm_Color_cursor:
	    rxvt_set_window_color(r, Color_cursor, str);
	    break;
#endif
	case XTerm_Color_pointer:
	    rxvt_set_window_color(r, Color_pointer, str);
	    break;
#if !(defined(NO_BRIGHTCOLOR) && defined(NO_BOLD_UNDERLINE_REVERSE))
	case XTerm_Color_BD:
	    rxvt_set_window_color(r, Color_BD, str);
	    break;
#endif
#ifndef NO_BOLD_UNDERLINE_REVERSE
	case XTerm_Color_UL:
	    rxvt_set_window_color(r, Color_UL, str);
	    break;
	case XTerm_Color_RV:
	    rxvt_set_window_color(r, Color_RV, str);
	    break;
#endif

	case XTerm_Menu:
	    /*
	     * rxvt_menubar_dispatch() violates the constness of the string,
	     * so DON'T do it here.
	     *
	     * 2006-03-13 gi1242: Plus it's a security risk. Only interactive
	     * users should control menus.
	     */
	    break;

	case XTerm_restoreFG:
	    rxvt_set_window_color(r, Color_fg, str);
	    break;
	case XTerm_restoreBG:
	    rxvt_set_window_color(r, Color_bg, str);
	    break;
	case XTerm_logfile:
	    /* TODO Write code */
	    break;
	case XTerm_font:
	    rxvt_resize_on_font (r, (char*) str);
	    break;

	/* 2006-03-14 gi1242 XXX: Why is this commented again? */
#if 0
	case XTerm_dumpscreen:
	    {
		int	     fd;
		if ((fd=open(str, O_RDWR | O_CREAT | O_EXCL, 0600))>=0)
		{
		    rxvt_scr_dump(r, fd);
		    close(fd);
		}
	    }
	    break;
#endif

	case MRxvt_term:	/* Set window title */
	case MRxvt_tab:
	case MRxvt_tabterm:	/* Set window and tab title */
	    rxvt_set_term_title(r, (const unsigned char*) str);
	    break;

	/*
	 * 2006-02-20 gi1242: These escape sequences are disabled for a possible
	 * security flaw. A malicious user might be able to send a few escape
	 * sequences through a text file, giving him more control than he should
	 * have over the tabs.
	 *
	 * On that note, even the escape sequences for hiding or showing the
	 * tabbar / scroll bar / menubar have been disabled. These aren't really
	 * a security risk, however the user can access these functions through
	 * either keyboard macros, or the popup menu, so these are really just
	 * code bloat :).
	 *
	 * Only the local user should be able to create / close / move tabs.
	 *
	 * NOTE: The disabled code uses the old hotkey code (from mrxvt-0.4.2).
	 * To enable the escape sequences below, you have to uncomment the code
	 * below AND port it to use the macro feature from 0.5.0 upward.
	 */
#if 0 /* {{{ DISABLED FOR SECURITY REASONS */
	case MRxvt_hide:
	    {
		rxvt_hotkey_hide_tabbar (r, 0);
	    }
	    break;

	case MRxvt_tabbtn:
	    rxvt_hotkey_hide_button (r, 0);
	    break;

	case MRxvt_saveconfig:
	    rxvt_hotkey_save_config (r, 0);
	    break;

	case MRxvt_closewin:
	    if( *str )
	    {
		int tabno = atoi(str) - 1;

		if( tabno == -1 )
		{
		    rxvt_hotkey_kill_tab(r, NULL);
		}
		else if( tabno >=0 && tabno <=LTAB(r)
		    && ( NOTSET_OPTION(r, Opt2_protectSecondary) ||
			 PVTS(r, tabno)->current_screen == PRIMARY))
		{
		    rxvt_kill_page (r, tabno);
		}
		break;
	    }

	    rxvt_hotkey_close_window (r, NULL);
	    break;
#endif /* }}} */

	/*
	 * 2006-03-13 gi1242: The following are not security risks, but are
	 * unnecessary. Only an interactive user needs such control, and he can
	 * have it using macros / menus.
	 */
#if 0 /* {{{ DISABLED because macros are more useful */
	case MRxvt_verybold:
	    /* rxvt_hotkey_verybold (r, 0); */
	    rxvt_toggle_verybold(r);
	    break;

#endif /* }}} */

#ifdef MULTICHAR_SET
	case MRxvt_encode:
	    /* We only change encoding method, but not font ;-) */
	    rxvt_set_multichar_encoding (r, str);   
	    break;
#endif	/* MULTICHAR_SET */

	case MRxvt_opacity:
	    if (IS_ATOM(r->h->xa[XA_NET_WM_WINDOW_OPACITY]))
	    {
		int	oldopacity = r->TermWin.opacity;
		int	tmp;

		if ('-' == *str && (char) 0 == *(str+1))
		{
		    /* handle '-' only, remember it's opposite  */
		    tmp = (r->h->rs[Rs_opacityDegree]) ?
			    r->TermWin.opacity_degree : 1;
		}
		else if ('+' == *str && (char) 0 == *(str+1))
		{
		    /* handle '+' only, remember it's opposite  */
		    tmp = (r->h->rs[Rs_opacityDegree]) ?
			    -(r->TermWin.opacity_degree) : -1;
		}
		else
		{
		    /* other cases, remember it's opposite  */
		    tmp = 100 - atoi (str);
		    /* only change opacity when it is valid */
		    if (tmp < 0 || tmp > 100)
			tmp = oldopacity;
		}
		if ('-' == *str || '+' == *str)
		    /* use input as offset */
		    r->TermWin.opacity += tmp;
		else
		    /* use input as absolute value */
		    r->TermWin.opacity = tmp;
		if (r->TermWin.opacity < 0)
		    r->TermWin.opacity = 0;
		if (r->TermWin.opacity > 100)
		    r->TermWin.opacity = 100;
		/* only change opacity when it has changed */
		if (r->TermWin.opacity != oldopacity)
		    rxvt_set_opacity (r);
	    }
	    break;

	case MRxvt_termenv:
	    PVTS(r)->termenv = rxvt_get_termenv ((const char*) str);
	    break;

	default:
	    break;
    }
}
/*----------------------------------------------------------------------*/


/*{{{ process DEC private mode sequences `ESC [ ? Ps mode' */
/*
 * mode can only have the following values:
 *    'l' = low
 *    'h' = high
 *    's' = save
 *    'r' = restore
 *    't' = toggle
 * so no need for fancy checking
 */
/* INTPROTO */
int
rxvt_privcases(rxvt_t* r, int mode, uint32_t bit)
{
    int		 state;

    if (mode == 's')
    {
	SET_PMODE (r, ISSET_PMODE(r, bit));
	return -1;
    }
    else
    {
	if (mode == 'r')
	    /* no overlapping */
	    state = ISSET_SMODE(r, bit) ? 1 : 0;
	else
	    state = (mode == 't') ? !ISSET_PMODE(r, bit) : mode;
	PrivMode(state, bit);
    }
    return state;
}


/* we're not using priv _yet_ */
/* INTPROTO */
void
rxvt_process_terminal_mode(rxvt_t* r, int mode, int priv __attribute__((unused)), unsigned int nargs, const int *arg)
{
    unsigned int    i, j;
    int		    state;
    static const struct {
	const int	argval;
	const uint32_t	bit;
    } argtopriv[] = {
	{ 1, PrivMode_aplCUR },
	{ 2, PrivMode_vt52 },
	{ 3, PrivMode_132 },
	{ 4, PrivMode_smoothScroll },
	{ 5, PrivMode_rVideo },
	{ 6, PrivMode_relOrigin },
	{ 7, PrivMode_Autowrap },
	{ 9, PrivMode_MouseX10 },
	{ 25, PrivMode_VisibleCursor },
	{ 35, PrivMode_ShiftKeys },
	{ 40, PrivMode_132OK },
	{ 47, PrivMode_Screen },
	{ 66, PrivMode_aplKP },
#ifndef NO_BACKSPACE_KEY
	{ 67, PrivMode_BackSpace },
#endif
	{ 1000, PrivMode_MouseX11 },
	{ 1010, PrivMode_TtyOutputInh },
	{ 1011, PrivMode_Keypress },
	{ 1047, PrivMode_Screen },
	/*
	 * 2006-03-12 gi1242: xterm uses 1049 to switch to the alternate screen,
	 * clear it and save the cursor position. Since 1048 was already
	 * implemented with state unkept, we implement 1049 in the same way.
	 *
	 * XXX: I'm not sure if both 1048 and 1049 should be implemented with
	 * state kept.
	 */
    };

    if (nargs == 0)
	return;

    /* make lo/hi boolean */
    if (mode == 'l')
	mode = 0;	/* reset */
    else if (mode == 'h')
	mode = 1;	/* set */

    for (i = 0; i < nargs; i++)
    {
	state = -1;

	/* basic handling */
	for (j = 0; j < (sizeof(argtopriv)/sizeof(argtopriv[0])); j++)
	{
	    if (argtopriv[j].argval == arg[i])
	    {
		state = rxvt_privcases(r, mode, argtopriv[j].bit);
		break;
	    }
	}

	/* extra handling for values with state unkept  */
	if (state == -1 && (mode == 0 || mode == 1) )
	{
	    switch (arg[i])
	    {
		case 1048:	/* alternative cursor save */
		    rxvt_scr_cursor(r, mode == 0 ? RESTORE : SAVE );
		    break;

		case 1049:	/* Alternate screen with cursor save & clearing
				   (like xterm). XXX Should we implement with
				   state kept? */
		    if( mode == 0 )
		    {
			rxvt_scr_change_screen(r, PRIMARY);
			rxvt_scr_cursor( r, RESTORE );
		    }
		    else
		    {
			rxvt_scr_cursor( r, SAVE );
			rxvt_scr_change_screen(r, SECONDARY);
			rxvt_scr_erase_screen(r, 2);
		    }
		    /* FALLTHROUGH */
		default:
		    continue;	/* for(;i;) */
	    }
	    continue;
	}

	/* extra handling for values with valid 0 or 1 state */
	switch (arg[i])
	{
	    /* case 1:	- application cursor keys */
	    case 2:	    /* VT52 mode */
		  /* oddball mode.  should be set regardless of set/reset
		   * parameter.  Return from VT52 mode with an ESC < from
		   * within VT52 mode
		   */
		PrivMode(1, PrivMode_vt52);
		break;
	    case 3:	    /* 80/132 */
		if (ISSET_PMODE(r, PrivMode_132OK))
		{
		    unsigned int w = Width2Pixel((state ? 132 : 80))
					    + r->szHint.base_width;
		    unsigned int h = r->szHint.base_height;
		    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
		    r->h->want_resize = 1;
		}
		break;
	    case 4:	    /* smooth scrolling */
		if (state)
		{
		    /* Unset jump scroll */
		    r->h->refresh_limit = BUFSIZ-1;
		    r->h->skip_pages	= 1;
		}
		else
		{
		    /* Set jump scroll */
		    rxvt_set_jumpscroll(r);
		}
		break;
	    case 5:	    /* reverse video */
		rxvt_scr_rvideo_mode(r, state);
		break;
	    case 6:	    /* relative/absolute origins  */
		rxvt_scr_relative_origin(r, state);
		break;
	    case 7:	    /* autowrap */
		rxvt_scr_autowrap(r, state);
		break;
	    /* case 8:	- auto repeat, can't do on a per window basis */
	    case 9:	    /* X10 mouse reporting */
		if (state)	/* orthogonal */
		    UNSET_PMODE(r, PrivMode_MouseX11);
		break;
	    case 25:	    /* visible/invisible cursor */
		rxvt_scr_cursor_visible(r, state);
		break;
	    /* case 35:	- shift keys */
	    /* case 40:	- 80 <--> 132 mode */
	    case 47:	    /* secondary screen */
		rxvt_scr_change_screen(r, state);
		break;
	    /* case 66:	- application key pad */
	    /* case 67:	- backspace key */
	    case 1000:	    /* X11 mouse reporting */
		if (state)	/* orthogonal */
		    UNSET_PMODE(r, PrivMode_MouseX10);
		break;
#if 0
	    case 1001:
		break;	    /* X11 mouse highlighting */
#endif
	    case 1010:	    /* scroll to bottom on TTY output inhibit */
		if (state)
		    SET_OPTION(r, Opt_scrollTtyOutputInhibit);
		else
		    UNSET_OPTION(r, Opt_scrollTtyOutputInhibit);
		break;

	    case 1011:	    /* scroll to bottom on key press */
		if (state)
		    SET_OPTION(r, Opt_scrollTtyKeypress);
		else
		    UNSET_OPTION(r, Opt_scrollTtyKeypress);
		break;

	    case 1047:	    /* secondary screen w/ clearing */
		/*
		 * 2006-03-12 gi1242 XXX: Xterm does NOT clear the alternate
		 * screen with this escape sequence. I hope whomever wrote this
		 * knew what he was doing.
		 */
		if (PVTS(r)->current_screen != PRIMARY)
		    rxvt_scr_erase_screen(r, 2);
		rxvt_scr_change_screen(r, state);
	    /* FALLTHROUGH */
	    default:
		break;
	}
    }
}
/*}}} */

/*{{{ process sgr sequences */
/* INTPROTO */
void
rxvt_process_sgr_mode(rxvt_t* r, unsigned int nargs, const int *arg)
{
    unsigned int    i;
    short	    rendset;
    int		    rendstyle = 0;

    if (nargs == 0)
    {
	rxvt_scr_rendition(r, 0, ~RS_None);
	return;
    }
    for (i = 0; i < nargs; i++)
    {
	rendset = -1;
	switch (arg[i])
	{
	    case 0:
		rendset = 0, rendstyle = ~RS_None;
		break;
	    case 1:
		rendset = 1, rendstyle = RS_Bold;
		break;
	    case 4:
		rendset = 1, rendstyle = RS_Uline;
		break;
	    case 5:
		rendset = 1, rendstyle = RS_Blink;
		break;
	    case 7:
		rendset = 1, rendstyle = RS_RVid;
		break;
	    case 22:
		rendset = 0, rendstyle = RS_Bold;
		break;
	    case 24:
		rendset = 0, rendstyle = RS_Uline;
		break;
	    case 25:
		rendset = 0, rendstyle = RS_Blink;
		break;
	    case 27:
		rendset = 0, rendstyle = RS_RVid;
		break;
	}
	if (rendset != -1)
	{
	    rxvt_scr_rendition(r, rendset, rendstyle);
	    continue;	    /* for(;i;) */
	}

	switch (arg[i])
	{
	    case 30:
	    case 31:	    /* set fg color */
	    case 32:
	    case 33:
	    case 34:
	    case 35:
	    case 36:
	    case 37:
		rxvt_scr_color(r,
		    (unsigned int)(minCOLOR+(arg[i]-30)), Color_fg);
		break;
#ifdef TTY_256COLOR
	    case 38:
		if (nargs > i + 2 && arg[i + 1] == 5)
		{
		    rxvt_scr_color(r,
			(unsigned int)(minCOLOR+arg[i+2]), Color_fg);
		    i += 2;
		}
		break;
#endif
	    case 39:	    /* default fg */
		rxvt_scr_color(r, Color_fg, Color_fg);
		break;

	    case 40:
	    case 41:	    /* set bg color */
	    case 42:
	    case 43:
	    case 44:
	    case 45:
	    case 46:
	    case 47:
		rxvt_scr_color(r,
		    (unsigned int)(minCOLOR+(arg[i]-40)), Color_bg);
		break;
#ifdef TTY_256COLOR
	    case 48:
		if (nargs > i + 2 && arg[i + 1] == 5)
		{
		    rxvt_scr_color(r,
			(unsigned int)(minCOLOR+arg[i+2]), Color_bg);
		    i += 2;
		}
		break;
#endif
	    case 49:	    /* default bg */
		rxvt_scr_color(r, Color_bg, Color_bg);
		break;

#ifndef NO_BRIGHTCOLOR
	    case 90:
	    case 91:	    /* set bright fg color */
	    case 92:
	    case 93:
	    case 94:
	    case 95:
	    case 96:
	    case 97:
		rxvt_scr_color(r,
		    (unsigned int)(minBrightCOLOR+(arg[i]-90)), Color_fg);
		break;
	    case 100:
	    case 101:	    /* set bright bg color */
	    case 102:
	    case 103:
	    case 104:
	    case 105:
	    case 106:
	    case 107:
		rxvt_scr_color(r,
		    (unsigned int)(minBrightCOLOR+(arg[i]-100)), Color_bg);
		break;
#endif
	}
    }
}
/*}}} */

/*{{{ process Rob Nation's own graphics mode sequences */
/* INTPROTO */
void
rxvt_process_graphics(rxvt_t* r)
{
    int             ch, cmd = rxvt_cmd_getc(r);

    if( cmd == -1 )
    {
	rxvt_set_escfail( r, 1 );
	return;
    }

    if (cmd == 'Q')		    /* query graphics */
    {
	rxvt_tt_printf(r, "\033G0\n");	/* no graphics */
	return;
    }

    /* swallow other graphics sequences until terminating ':' */
    do
      {
	ch = rxvt_cmd_getc(r);
	if( ch == -1 )
	{
	    rxvt_set_escfail( r, 1 );
	    return;
	}
      }
    while( ch != ':' && ch >= ' ' && ch <= '~' ); /* 2007-01-06 gi1242: In case
						     the escape sequence is
						     malformed, swallow only
						     printable characters. */
}
/*}}} */

/* ------------------------------------------------------------------------- */

/*
 * Process the value returned by rxvt_cmd_getc() here. This processes escape
 * sequences, and adds text to the tabs output buffer.
 */
/* INTPROTO */
void
rxvt_process_getc( rxvt_t *r, unsigned char ch )
{
    int		    limit;	/* Number of lines to read before asking for a
				   refresh */

    limit = r->h->skip_pages * r->TermWin.nrow;
    if( limit < 0 )
	/* Integer overflow */
	limit = INT_MAX;
    

    /*
     * Process as much input from the tab as is available. Keep a count of the
     * (approximate) number of lines we have scrolled, so we know when to
     * refresh.
     */
    for(;;)
    {
	if (ch >= ' ' || ch == '\t' || ch == '\n' || ch == '\r')
	{
	    /*
	     * Read the longest text string we can from the input buffer.
	     */

	    int	    nlines = 0,		/* #lines read */
		    nchars,		/* #chars read before newline */
		    refreshnow = 0;	/* If we should request a refresh */

	    unsigned char   *str;


	    nchars = PSCR(r).cur.col;

	    /*
	     * point `str' to the start of the string, decrement first since
	     * it was post incremented in rxvt_cmd_getc()
	     */
	    str = --(PVTS(r)->outbuf_start);
	    while (PVTS(r)->outbuf_start < PVTS(r)->outbuf_end)
	    {
		ch = *(PVTS(r)->outbuf_start)++;
		
		if (ch == '\n')
		{
		    nchars = 0;
		    nlines++;
		    PVTS(r)->scrolled_lines++;
		}

		else if (ch < ' ' && ch != '\t' && ch != '\r')
		{
		    /*
		     * Unprintable. Reduce outbuf_start so that this character
		     * will be processed later.
		     */
		    PVTS(r)->outbuf_start--;
		    break;
		}

		else if( ++nchars > r->TermWin.ncol )
		{
		    PVTS(r)->scrolled_lines++;
		    nchars = 0;
		}

		if(
		     PVTS(r)->scrolled_lines >= limit
		  )
		{
		    refreshnow = 1;
		    break;
		}
	    }

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "\e[31mAdding %d chars %d lines\e[0m\n%.*s\n", PVTS(r)->outbuf_start - str, nlines, PVTS(r)->outbuf_start - str, str));

	    /*
	     * NOTE: nlines can not be MORE than the number of lines we will
	     * actually add!
	     */
	    rxvt_scr_add_lines(r, str, nlines,
		(PVTS(r)->outbuf_start - str));

	    /*
	     * Only refresh the screen if we've scrolled more than
	     * MAX_SKIPPED_PAGES pages.
	     *
	     * Refreshing should be correct for small scrolls, because
	     * nbytes_last_read will be small, forcing the refresh.
	     */
	    if (refreshnow)
	    {
		refreshnow = 0;

		/*
		 * Note: If the tab is not visible, then rxvt_scr_refresh
		 * returns immediately. Also rxvt_scr_refresh resets
		 * scrolled_lines.
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Requesting refresh." " scrolled %d lines\n", PVTS(r)->scrolled_lines ));
		rxvt_scr_refresh(r,
			(r->h->refresh_type & ~CLIPPED_REFRESH) );

		/* If we have X events to process, then do so now. */
		if( XPending( r->Xdisplay ) )
		    break;
	    }
	}

	/*
	 * Process escape sequence
	 */
	else if( ch == C0_ESC )
	{
	    /* Save the start of the escape sequence */
	    if( IS_NULL( PVTS(r)->outbuf_escstart ) )
		PVTS(r)->outbuf_escstart =
		    PVTS(r)->outbuf_start-1;

	    /* Forget the previous escape sequence failure (if any) */
	    SET_NULL( PVTS(r)->outbuf_escfail );

	    /* Attempt to process the escape sequence */
	    rxvt_process_escape_seq(r);

	    /* If we succeeded, then clear the start. */
	    if( IS_NULL( PVTS(r)->outbuf_escfail ) )
		SET_NULL( PVTS(r)->outbuf_escstart );
	    else
		/* Otherwise don't process any more data from this tab */
		break;
	}

	/*
	 * Anything else must be a non-printing character
	 */
	else
	{
	    rxvt_process_nonprinting(r, ch);
	}


	/*
	 * Check if we can keep reading on this tab.
	 *
	 * NOTE: We could check if we also have pending X events, but this will
	 * generate many many extra protocol requests, which can be quite a
	 * problem on a slow connection. Thus for now we only process X events
	 * on screen refreshes or in rxvt_cmd_getc().
	 */
	if( rxvt_cmdbuf_has_input( r ) )
	    ch = *PVTS(r)->outbuf_start++;
	else
	    break;
    } /* for(;;) */
}


/*{{{ Read and process output from the application */
/* LIBPROTO */
void
rxvt_main_loop(rxvt_t *r)
{
    int          	ch;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "Entering rxvt_main_loop()\n" ));

    /* Send the screen size. */
	rxvt_tt_winsize(PVTS(r)->cmd_fd,
	    r->TermWin.ncol, r->TermWin.nrow, PVTS(r)->cmd_pid);

    while (1)
    {
	/* wait for something */
	ch = 0;

	while(
	       r->ndead_childs == 0		&&	/* Nothing dead */
	       r->cleanDeadChilds == 0	&&	/* Nothing to be cleaned up */
	       ( (ch = rxvt_cmd_getc(r)) == -1 )	/* No input */
	     )
	    ;

	/*
	 * 2006-08-23 gi1242: If rxvt_cmd_getc is called, and then
	 * r->ndead_childs gets set during this call, then we should not discard
	 * the return value of rxvt_cmd_getc.
	 */

	if( ch != -1 )
	    /* rxvt_cmd_getc() returned something */
	    rxvt_process_getc( r, ch );

	/*
	 * See if we need a refresh. If we read a large number of bytes from
	 * this tabs cmd_fd, then we should let rxvt_process_getc() handle the
	 * refresh (because we might be "flat out" scrolling).
	 *
	 * If we read a small number of bytes on our last read(), then we should
	 * refresh the screen here, because if other tabs are busy, we can not
	 * count on rxvt_cmd_getc() refreshing the screen.
	 *
	 * XXX 2006-09-01 gi1242: If we have multiple visible terminal windows,
	 * then we should do this refresh for ALL tabs with visible terminal
	 * windows (and not only the active tab).
	 */

	if( PVTS(r)->nbytes_last_read <= r->h->refresh_limit )
	    rxvt_refresh_vtscr_if_needed( r );

	/*
	 * handle the case that some children have died regardless of what
	 * rxvt_cmd_getc returned
	 */
	if (r->ndead_childs > 0 || r->cleanDeadChilds )
	    rxvt_clean_cmd_page (r);

    } /* while(1) */
    /* NOT REACHED */
    assert (0);
}


/*
 * Send printf() formatted output to the command.
 * Only use for small amounts of data.
 */
/* EXTPROTO */
void
rxvt_tt_printf(rxvt_t* r, const char *fmt,...)
{
    va_list	 arg_ptr;
    unsigned char   buf[256];

    va_start(arg_ptr, fmt);
    vsnprintf((char *)buf, sizeof(buf)-1, fmt, arg_ptr);
    buf[sizeof(buf)-1] = (unsigned char) 0;
    va_end(arg_ptr);
    rxvt_tt_write(r, buf, (unsigned int)STRLEN(buf));
}


/* ---------------------------------------------------------------------- */
/* Addresses pasting large amounts of data and rxvt hang
 * code pinched from xterm (v_write()) and applied originally to
 * rxvt-2.18 - Hops
 * Write data to the child's pty as typed by the user, pasted with the mouse,
 * or generated by us in response to a query ESC sequence.
 * If len = 0, this function can be used to simply write any pending data
 * in the input buffer to the child.
 */
/* EXTPROTO */
void
rxvt_tt_write (rxvt_t* r, const unsigned char *str, int len)
{
#define MAX_PTY_WRITE 128   /* 1/2 POSIX minimum MAX_INPUT */
	int		written;
	int		p;
	/* start of physical buffer	*/
	unsigned char*	inbuf_base;
	/* start of current buffer pending */
	unsigned char*	inbuf_start;
	/* end of current buffer pending   */
	unsigned char*	inbuf_end;
	/* remaining room in physical buffer	  */
	int inbuf_room;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_tt_write (r, %s, %d)\n", str ? (char*) str: "nil", len));

	if (IS_NULL(PVTS(r)->inbuf_start) && len > 0)
	{
	    p = (len / MAX_PTY_WRITE + 1) * MAX_PTY_WRITE;
	    if (p <= 0) 
	    {
		/* possible integer overflow
		 * Or maybe len was already < 0. */
		rxvt_msg (DBG_ERROR, DBG_COMMAND,
			"\e[31Possible integer overflow or logical error in rxvt_tt_write: probable data loss.\e[0m\n");
		return;
	    }
	    inbuf_base = inbuf_start = inbuf_end = rxvt_malloc(p);
	    inbuf_room = p;
	}
	else
	{
	    inbuf_base = PVTS(r)->inbuf_base;
	    inbuf_start = PVTS(r)->inbuf_start;
	    inbuf_end = PVTS(r)->inbuf_end;
	    inbuf_room = PVTS(r)->inbuf_room;
	}

	/*
	 * Append to the block we already have. Always doing this simplifies the
	 * code, and isn't too bad, either. If this is a short block, it isn't
	 * too expensive, and if this is a long block, we won't be able to write
	 * it all anyway.
	 */
	if (len > 0)
	{
	    if (inbuf_room < len)
	    {
		/* run out of room */
		if (inbuf_start != inbuf_base)
		{
		    /* there is unused space, move everything down */
		    MEMMOVE(inbuf_base, inbuf_start,
			(unsigned int)(inbuf_end - inbuf_start));
		    inbuf_end -= inbuf_start - inbuf_base;
		    inbuf_start = inbuf_base;
		}
		if (inbuf_room < len)
		{
		    /* still won't fit: get more space, use most basic
		    ** realloc because an error is not fatal. */
		    unsigned int    size = inbuf_end - inbuf_base;
		    unsigned int    reallocto;

		    reallocto = ((size+len) / MAX_PTY_WRITE + 1) *MAX_PTY_WRITE;
		    inbuf_base = rxvt_realloc(inbuf_base, reallocto);
		    /* save across realloc */
		    if (inbuf_base)
		    {
			inbuf_start = inbuf_base;
			inbuf_end = inbuf_base + size;
			inbuf_room = reallocto - size;
		    }
		    else
		    {
			/* no memory: ignore entire write request */
			rxvt_msg (DBG_ERROR, DBG_COMMAND, 
				"data loss: cannot allocate buffer space");
			/* restore clobbered pointer */
			inbuf_base = inbuf_start;
		    }
		}
	    }
	    if (inbuf_room >= len)
	    {
		/* new stuff will fit */
		MEMCPY(inbuf_end, str, len);
		inbuf_end += len;
	    }
	}

	/*
	 * Write out as much of the buffer as we can. Be careful not to overflow
	 * the pty's input silo. We are conservative here and only write a small
	 * amount at a time.
	 *
	 * If we can't push all the data into the pty yet, we expect write to
	 * return a non-negative number less than the length requested (if some
	 * data written) or -1 and set errno to EAGAIN, EWOULDBLOCK, or EINTR
	 * (if no data written).
	 *
	 * (Not all systems do this, sigh, so the code is actually a little more
	 * forgiving.)
	 */

	if ((p = inbuf_end - inbuf_start) > 0)
	{
	    written = write (PVTS(r)->cmd_fd, inbuf_start, min (p, MAX_PTY_WRITE));
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "\t%d bytes have been written to vts.cmd_fd = %d\n", written, PVTS(r)->cmd_fd));
	    if (written < 0)
		written = 0;
	    inbuf_start += written;
	    if (inbuf_start >= inbuf_end)	/* we wrote it all */
		inbuf_start = inbuf_end = inbuf_base;
	}

	/*
	 * If we have lots of unused memory allocated, return it
	 */
	if (inbuf_room > MAX_PTY_WRITE * 8)
	{
	    /* arbitrary hysteresis, save pointers across realloc */
	    unsigned int    start = inbuf_start - inbuf_base;
	    unsigned int    size = inbuf_end - inbuf_base;
	    unsigned int    reallocto;

	    reallocto = (size / MAX_PTY_WRITE + 1) * MAX_PTY_WRITE;
	    inbuf_base = rxvt_realloc(inbuf_base, reallocto);
	    if (inbuf_base)
	    {
		inbuf_start = inbuf_base + start;
		inbuf_end = inbuf_base + size;
		inbuf_room = reallocto - size;
	    }
	    else
	    {
		/* should we print a warning if couldn't return memory? */
		/* restore clobbered pointer */
		inbuf_base = inbuf_start - start;
	    }
	}
	PVTS(r)->inbuf_base = inbuf_base;
	PVTS(r)->inbuf_start = inbuf_start;
	PVTS(r)->inbuf_end = inbuf_end;
	PVTS(r)->inbuf_room = inbuf_room;
}
/*----------------------- end-of-file (C source) -----------------------*/
