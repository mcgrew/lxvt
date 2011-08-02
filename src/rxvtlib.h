/*--------------------------------*-H-*---------------------------------*
 * File:    rxvtlib.h
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997-2001   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 2004        Terry Griffin <griffint@pobox.com>
 * Copyright (c) 2005        Teun Burgers <burgers@ecn.nl>
 * Copyright (c) 2004-2005   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 * Copyright (c) 2005        Gautam Iyer <gi1242@users.sourceforge.net>
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

#ifndef __RXVTLIB_H__
#define __RXVTLIB_H__

/*
 * Boolean variables
 */
#ifdef __GNUC__
# define BOOLVAR(x, sz)	x:sz
#else
# define BOOLVAR(x, sz) x
#endif


/* type of unicode_t */
typedef uint32_t	unicode_t;


/*****************************************************************************
 *                                 SECTION 2                                 *
 *                      DO NOT TOUCH ANYTHING BELOW HERE                     *
 *****************************************************************************/

struct rxvt_vars;	/* defined later on */
struct rxvt_hidden;	/* not defined here */

typedef struct
{
    int32_t         row;
    int32_t         col;
} row_col_t;

typedef unsigned char text_t;
#if defined(TTY_256COLOR) || defined(MULTICHAR_SET)
# define rend_t	    uint32_t
#else
# define rend_t	    uint16_t
#endif

/*
 * TermWin elements limits
 *  ncol      : 1 <= ncol       <= MAX(int16_t)
 *  nrow      : 1 <= nrow       <= MAX(int16_t)
 *  saveLines : 0 <= saveLines  <= MAX(int16_t)
 *  nscrolled : 0 <= nscrolled  <= saveLines
 *  view_start: 0 <= view_start <= nscrolled
 */

typedef struct
{
    uint16_t	    fwidth,	/* font width  [pixels] */
		    fheight;	/* font height [pixels] */
#ifdef XFT_SUPPORT
    uint16_t	    pwidth,	/* propotionally spaced font width / height */
		    pheight;
#endif
    uint16_t	    propfont;	/* font proportional flags */
    uint16_t	    ncol;	/* window columns [characters] */
    uint16_t	    nrow;	/* window rows [characters] */
    uint16_t	    mapped; 	/* TermWin is mapped? */
    uint16_t	    int_bwidth; /* internal border width */
    uint16_t	    ext_bwidth; /* external border width */

#ifndef NO_LINESPACE
    uint16_t	    lineSpace;	/* space between rows */
#endif

    char	    BOOLVAR(focus,1);	/* window is focused? */
    char	    enter;  /* pointer is in window? */

    Window	    parent; /* parent window */
    GC		    gc;	    /* GC for drawing text */
    XFontStruct*    font;   /* main font structure */
#ifndef NO_BOLDFONT
    XFontStruct*    bfont;  /* bold font */
#endif
#ifdef MULTICHAR_SET
    XFontStruct*    mfont;  /* Multichar font structure */
#endif
#ifdef USE_XIM
    XFontSet	    fontset;
#endif
#ifdef XFT_SUPPORT
    XftPattern*	    xftpattern;
    XftFont	    *xftfont, *xftpfont, *xftPfont;
# ifdef MULTICHAR_SET
    XftPattern*	    xftmpattern;
    XftFont*	    xftmfont;
    int		    xftmsize;
#  ifdef HAVE_ICONV_H
    iconv_t	    xfticonv;
#  endif
# endif
# ifndef NO_BOLDFONT
    XftFont*	    xftbfont;
# endif
    char	    xftfnmono;	/* font is mono? */
    char	    xftmono;	/* font and mfont are mono? */
    int		    xftsize, xftpsize;
#endif	/* XFT_SUPPORT */

# define PARENT_NUMBER	    (6)
    int		    opacity;	/* transluscent window opaque degree */
    int		    opacity_degree; /* opaque change degree */

    int		    fade;   /* off-focus fading percentage */

#ifdef HAVE_X11_SM_SMLIB_H
    SmcConn	    sm_conn;
    IceConn	    ice_conn;
    int		    ice_fd;
    char*	    sm_client_id;
#endif

#ifndef NO_BELL
    unsigned long   vBellDuration;
#endif
} TermWin_t;



/*
 * screen accounting:
 * screen_t elements
 *   text:      Contains all text information including the scrollback buffer.
 *              Each line is length TermWin.ncol
 *   tlen:      The length of the line or -1 for wrapped lines.
 *   rend:      Contains rendition information: font, bold, colour, etc.
 *   
 *   NOTE: Each line for both text and rend are only allocated on demand, and
 *         text[x] is allocated <=> rend[x] is allocated  for all x.
 *
 *   row:       Cursor row position                   : 0 <= row < TermWin.nrow
 *   col:       Cursor column position                : 0 <= col < TermWin.ncol
 *   tscroll:   Scrolling region top row inclusive    : 0 <= row < TermWin.nrow
 *   bscroll:   Scrolling region bottom row inclusive : 0 <= row < TermWin.nrow
 *
 * selection_t elements
 *   clicks:    1, 2 or 3 clicks - 4 indicates a special condition of 1 where
 *              nothing is selected
 *   beg:       row/column of beginning of selection  : never past mark
 *   mark:      row/column of initial click           : never past end
 *   end:       row/column of one character past end of selection
 * * Note: -TermWin.nscrolled <= beg.row <= mark.row <= end.row < TermWin.nrow
 * * Note: col == -1 ==> we're left of screen
 *
 * Layout of text/rend information in the screen_t text/rend structures:
 *   Rows [0] ... [TermWin.saveLines - 1]
 *     scrollback region : we're only here if TermWin.view_start != 0
 *   Rows [TermWin.saveLines] ... [TermWin.saveLines + TermWin.nrow - 1]
 *     normal `unscrolled' screen region
 */
typedef struct
{
    text_t**	    text;	/* _all_ the text */
    int16_t*	    tlen;	/* length of each text line */
    rend_t**	    rend;	/* rendition, uses RS_ flags */
    row_col_t       cur;	/* cursor position on the screen */
    uint16_t	    tscroll;	/* top of settable scroll region */
    uint16_t	    bscroll;	/* bottom of settable scroll region */
    uint16_t	    charset;	/* character set number [0..3] */
    unsigned int    flags;	/* see below */
    row_col_t	    s_cur;	/* saved cursor position */
    uint16_t	    s_charset;	/* saved character set number [0..3] */
    char	    s_charset_char;
    rend_t	    s_rstyle;	/* saved rendition style */
} screen_t;


typedef struct
{
    unsigned char*  text;   /* selected text */
    uint32_t	    len;    /* length of selected text */
    enum {
	SELECTION_CLEAR = 0,/* nothing selected */
	SELECTION_INIT,	    /* marked a point */
	SELECTION_BEGIN,    /* started a selection */
	SELECTION_CONT,	    /* continued selection */
	SELECTION_DONE	    /* selection put in CUT_BUFFER0 */
    } op;		    /* current operation */
    short           screen; /* screen being used */
    short           clicks; /* number of clicks */
    row_col_t       beg;    /* beginning of selection <= mark */
    row_col_t       mark;   /* point of initial click <= end */
    row_col_t       end;    /* one character past end point */
} selection_t;

typedef enum
{
    OLD_SELECT, OLD_WORD_SELECT, NEW_SELECT
} sstyle_t;

/* ------------------------------------------------------------------------- */

/* screen_t flags */
#define Screen_Relative		(1<<0)	/* relative origin mode flag */
#define Screen_VisibleCursor	(1<<1)	/* cursor visible? */
#define Screen_Autowrap		(1<<2)	/* auto-wrap flag */
#define Screen_Insert		(1<<3)	/* insert mode (vs. overstrike) */
#define Screen_WrapNext		(1<<4)	/* need to wrap for next char? */
#define Screen_DefaultFlags	(Screen_VisibleCursor|Screen_Autowrap)


/* rxvt_vars.Options */
enum Option {
	Opt_console		    = 1,
	Opt_loginShell		    ,
	Opt_iconic		    ,
	Opt_visualBell		    ,
	Opt_mapAlert		    ,
	Opt_reverseVideo	    ,
	Opt_utmpInhibit		    ,
	Opt_meta8		    ,
	Opt_scrollTtyOutputInhibit  ,
	Opt_scrollTtyKeypress	    ,
	Opt_mc_hack		    ,
	Opt_tripleclickwords	    ,
	Opt_mouseWheelScrollPage    ,
	Opt_pointerBlank	    ,
	Opt_cursorBlink		    ,
#ifdef XFT_SUPPORT
        Opt_xft		    	    ,
#endif
        Opt2_protectSecondary	    ,
#ifdef XFT_SUPPORT
# ifdef MULTICHAR_SET
	Opt2_xftNomFont	    	    ,
	Opt2_xftSlowOutput	    ,
# endif
	Opt2_xftAntialias	    ,
	Opt2_xftHinting	    	    ,
	Opt2_xftAutoHint            ,
	Opt2_xftGlobalAdvance	    ,
#endif
	Opt2_borderLess		    ,
	Opt2_overrideRedirect	    ,
	Opt2_veryBold		    ,
#ifndef NO_BRIGHTCOLOR
	Opt2_boldColors	    	    ,
	Opt_veryBright		    ,
#endif
	Opt2_noSysConfig    	    ,
	Opt2_disableMacros	    ,
#ifdef HAVE_X11_SM_SMLIB_H
	Opt2_enableSessionMgt	    ,
#endif
	Opt2_linuxHomeEndKey	    ,
	Opt2_smoothResize	    ,
	Opt2_smartResize   	    ,
	Opt2_maximized		    ,
	Opt2_fullscreen		    ,
};

#define OPT(O)	((uint64_t)(1ULL << (O)))

#ifdef XFT_SUPPORT
# define DEFAULT_OPTIONS   \
    (OPT(Opt2_veryBold) | OPT(Opt2_smartResize) | OPT(Opt2_xftAntialias))
#else
# define DEFAULT_OPTIONS   \
    (OPT(Opt2_veryBold) | OPT(Opt2_smartResize))
#endif


/* Macros to manipulate options (given an option array) */
#define ISSET_ARRAYOPT( array, option )	    \
    ( (array) & OPT(option) )

#define NOTSET_ARRAYOPT( array, option )    \
    !ISSET_ARRAYOPT( array, option )

#define SET_ARRAYOPT( array, option )	    \
    ( (array) |= OPT(option) )

#define UNSET_ARRAYOPT( array, option )	    \
    ( (array) &= ~OPT(option) )

#define TOGGLE_ARRAYOPT( array, option )	    \
    ( (array) ^= OPT(option) )

/* Macros to manipulate standard options */
#define ISSET_OPTION(R, OPT)	\
    ISSET_ARRAYOPT( (R)->Options, (OPT) )

#define NOTSET_OPTION(R, OPT)	\
    !ISSET_OPTION( (R), (OPT) )

#define SET_OPTION(R, OPT)  	\
    SET_ARRAYOPT( (R)->Options, (OPT) )

#define UNSET_OPTION(R, OPT)  	\
    UNSET_ARRAYOPT( (R)->Options, (OPT) )

#define TOGGLE_OPTION(R, OPT)	\
    TOGGLE_ARRAYOPT( (R)->Options, (OPT) )



#define PROPFONT_NORMAL		    (1<<0)
#define PROPFONT_BOLD		    (1<<1)
#define PROPFONT_MULTI		    (1<<2)

/*
 * Weather or not we should use xftPfont / xftpfont while drawing text. This has
 * ABSOLUTELY NOTHING to do with PROPFONT_* or the propfont junk. This is used
 * for drawing tab titles / menubar / etc.
 */
#define NO_PFONT		    (0)
#define USE_PFONT		    (1)
#define USE_BOLD_PFONT		    (2)

/* ------------------------------------------------------------------------- */

typedef enum
{
    TERMENV_XTERM = 0,
    TERMENV_RXVT,
    TERMENV_VT102,
    TERMENV_VT100,
    TERMENV_ANSI,
    TERMENV_DUMB,
} termenv_t;

typedef struct
{
    /* moved from TermWin_t */
    uint16_t	    saveLines;	/* number of lines to save */
    uint16_t	    num_scr;	/* number of lines scrolled */
    uint16_t	    nscrolled;	/* number of line actually scrolled */
    uint16_t	    view_start;	/* scrollback view starts here */

    /* screen structure initialized? */
    unsigned char   BOOLVAR(init_screen, 1);

    Window          vt; /* terminal window */
#ifdef XFT_SUPPORT
    XftDraw*	    xftvt;
#endif

    /* Apparently, people like fg/bg colors for individual terminal */
    unsigned long   p_fg,
		    p_bg,
		    p_fgfade,
		    p_bgfade;
#ifdef XFT_SUPPORT
    XftColor	    p_xftfg,
		    p_xftbg,
		    p_xftfgfade,
		    p_xftbgfade;
#endif

    /* moved from rxvt_t */
    text_t**	    drawn_text;	/* text drawn on screen (characters) */
    rend_t**	    drawn_rend;	/* text drawn on screen (rendition) */
    text_t**	    buf_text;
    rend_t**	    buf_rend;
    screen_t        screen;
#if NSCREENS
    screen_t        swap;
#endif
    /* move from hidden */
    rend_t	    rstyle;

    uint16_t	    prev_ncol; /* previous columns */
    uint16_t	    prev_nrow; /* previous rows */

    /* moved from rxvt_t */
    int             cmd_fd; /* pty file descriptor; connected to command */
    int             tty_fd; /* tty file descriptor; connected to child */
    /* moved from hidden */
    unsigned char   BOOLVAR( current_screen, 1);    /* PRIMARY / SECONDARY */
#ifdef POINTER_BLANK
    unsigned char   BOOLVAR( hidden_pointer, 1);    /* pointer is hidden? */
#endif

    pid_t	    cmd_pid;
    char*	    ttydev;
#ifndef RESET_TTY_TO_COMMON_DEFAULTS
    struct stat     ttyfd_stat; /* original status of our tty */
#endif
#ifndef NO_SETOWNER_TTYDEV
    unsigned char   next_tty_action;
#endif

    /* moved from hidden */
    uint32_t	    PrivateModes;
    uint32_t	    SavedModes;

#ifdef UTMP_SUPPORT
#ifndef UTEMPTER_SUPPORT
# ifdef HAVE_STRUCT_UTMP
    struct utmp     ut;
# endif
# if defined(HAVE_STRUCT_UTMPX) && !defined(HAVE_STRUCT_UTMP)
    struct utmpx    utx;
# endif
# if (defined(HAVE_STRUCT_UTMP) && defined(HAVE_UTMP_PID)) || defined(HAVE_STRUCT_UTMPX)
    char            ut_id[5];
# endif
    int             utmp_pos;
#endif	/* UTEMPTER_SUPPORT */
    unsigned char   next_utmp_action;
#endif

#if 0
    char**	    command_argv;
    int		    command_argc;
#endif

    /* move from rxvt_hidden */
    ttymode_t	    tio;
    unsigned int    ttymode;
    char	    rvideo;
#ifdef MULTICHAR_SET
    char	    chstat,
    /* set ==> we only got half a glyph */
		    lost_multi,
    /* set ==> currently using 2 bytes per glyph */
		    multi_byte;
#endif
    char	    charsets[4];

    /* need to hold the terminal?
     *   hold == 0: not hold
     *   hold == 1: hold
     *   hold >  1: can destroy the terminal now
     */
    unsigned char   BOOLVAR(hold,2),
		    BOOLVAR(holdOption,3),  /* Copy of the profile option for
					       this tab.*/
		    BOOLVAR(gotEIO,1),	    /* Read on this terminal's fd
					       got EIO */
		    BOOLVAR(dead,1);	    /* the terminal is dead or alive? */
    int		    status;		    /* Status of child process after it
					       exits */

    /* the terminal TERM type */
    termenv_t	    termenv;

    int		    scrolled_lines;	/* If this tab is producing lots of
					   data, this is the number of lines
					   that have scrolled without a refresh
					   request */

    /*
     * Moved from hidden: want_refresh needs to be local to each tab.
     * want_full_refresh, refresh_type (except for SMOOTH_REFRESH), and
     * want_clip_refresh should be window (not tab) variables.
     */
    unsigned char   BOOLVAR( want_refresh, 1);		/* Awaiting screen
							   refresh */

    int		    nbytes_last_read;	/* Number of bytes read on the last
					   read() call to child's fd */

    /*
     * Data we want to write into cmd_fd is buffered in here, before being
     * written by rxvt_tt_write(). [Child's input buffer]
     */
    unsigned char   *inbuf_base,		/* pointer to physical buffer */
		    *inbuf_start,		/* beginning of area to write */
		    *inbuf_end;		/* end of area to write */
	 int inbuf_room; /* Remaining room at the end of the buffer */

    /*
     * Data read from cmd_fd is buffered in here [Child's output buffer]
     */
    unsigned char   *outbuf_escstart,	/* Start of an escape sequence */
		    *outbuf_escfail,	/* Position where processing of an
					   escape sequence last failed */
		    *outbuf_start,	/* current char */
		    *outbuf_end;	/* End of read child's output */
    unsigned char   outbuf_base[BUFSIZ];
} term_t;


/* Possible values for macros.modFlags */
#define MACRO_CTRL	(1U << 0)
#define MACRO_META	(1U << 1)
#define MACRO_ALT	(1U << 2)
#define MACRO_SHIFT	(1U << 3)
#define MACRO_PRIMARY	(1U << 4)

/* Number of bits used by modifiers in modFlags */
#define MACRO_N_MOD_BITS    (5)
#define MACRO_MODMASK	\
    ( MACRO_CTRL | MACRO_META | MACRO_ALT | MACRO_SHIFT | MACRO_PRIMARY )
#define MACRO_MAX_CHAINLEN  (0xf)

/* Get / set the macro number from modFlags */
#define MACRO_GET_NUMBER(x)	\
    (( (x) & ~MACRO_MODMASK ) >> MACRO_N_MOD_BITS)


/*
 * Action to take when a macro is called / menu item is selected.
 */
typedef struct
{
    unsigned short  type;   /* must not be changed; first element */
    unsigned short  len;    /* strlen (str) */
    unsigned char   *str;   /* action to take */
} action_t;

/* Values for macro_t.type. Must sync this with macroNames from macros.c */
enum
{
    MacroFnDummy=0,
    MacroFnEsc,
    MacroFnStr,
    MacroFnExec,
    MacroFnClose,
    MacroFnScroll,
    MacroFnCopy,
    MacroFnPaste,
    MacroFnPasteFile,
    MacroFnFont,
    MacroFnToggleVeryBold,
    MacroFnToggleBoldColors,
    MacroFnToggleVeryBright,
    MacroFnToggleHold,
    MacroFnToggleFullscreen,
    MacroFnRaise,
    MacroFnSetTitle,
    MacroFnSaveConfig,
    MacroFnToggleMacros,
    NMACRO_FUNCS
} macroFnNames;

typedef unsigned char macro_priority_t;
typedef struct
{
    KeySym		keysym;
    unsigned char	modFlags;	/* First 4 bits are the action order
					   number. Last four bits are the
					   modifiers */
    macro_priority_t	priority;
    action_t		action;
} macros_t;

/*
 * profile_t structure. Contains information about each profile (formerly the
 * vt%d.xx resources.
 */
typedef struct _profile_t
{
    unsigned long	fg,
			bg;
#ifdef XFT_SUPPORT
    XftColor		xftfg,
			xftbg;
#endif
    unsigned long	fg_fade,
			bg_fade;
# ifdef XFT_SUPPORT
    XftColor		xftfg_fade,
			xftbg_fade;
# endif

    int			saveLines;

    unsigned char	BOOLVAR( holdOption, 3 );

    /*
     * Each profile also has a tab title, and command associated to it. However
     * since that's already stored in our resource options, we don't need
     * pointers for it here.
     */
} profile_t;


typedef struct rxvt_vars
{
    /*
     * These ``hidden'' items are not for public consumption and must not be
     * accessed externally
     *
     * 2006-02-19 gi1242: mrxvt was forked from rxvt. In rxvt, they compiled
     * common rxvt functions into a library "rxvtlib". Thus when loading
     * multiple versions of rxvt, this common code was linked to, saving some
     * memory.
     *
     * With tabs, we of course won't find this as useful. With grand dreams of
     * multiple windows, we'll never need it :).
     */
    struct rxvt_hidden *h;

    /*
     * Exposed items
     *   Changes to structure here require library version number change
     */
    TermWin_t	    TermWin;
    Display*	    Xdisplay;
    uint64_t	    Options;
    XSizeHints      szHint;

    /* macros */
    macros_t*	    macros;		    /* array of user defind macros */
    unsigned short  nmacros,		    /* Number of macros defined */
		    maxMacros;		    /* max # of macros that can be
					       stored in memory pointed to by
					       "macros" */

    Colormap        Xcmap;

    unsigned long   *pixColorsFocus,
		    *pixColorsUnfocus;	    /* Arrays of size TOTAL_COLORS.
					       Pixel colors to be used when
					       focused / unfocussed. */
    unsigned long   *pixColors;		    /* Array of size TOTAL_COLORS */

#ifdef XFT_SUPPORT
    XftColor*	    xftColors;		    /* Array of size TOTAL_COLORS */

    XftColor	    *xftColorsFocus,	    /* Same as above, but for Xft */
		    *xftColorsUnfocus;
# endif


    profile_t	    profile;

    Cursor	    term_pointer;	    /* cursor for vt window */
    int		    Xdepth;
    int		    Xfd;		    /* file descriptor of the X
					       connection */

    /*
     * term_t structures and pointers.
     */
    term_t          vts;

    unsigned char   BOOLVAR( cleanDeadChilds, 1 ),
					    /* True if we have marked some
					       children as dead, but not called
					       rxvt_clean_cmd_page() to clean
					       them out */
		    BOOLVAR( gotEIO, 1 );   /* True if some read() got EIO */

    short	    ndead_childs;	    /* number of children that have
					       died */
    short	    nAsyncChilds;	    /* Number of alive children launched
					       via rxvt_async_exec */
    short	    asyncChilds[MAX_CHILDS];
					    /* PIDS of children exec'ed via
					     * rxvt_async_exec */

    int		    num_fds;		    /* number of fd to monitor */


    selection_t     selection;
    sstyle_t	    selection_style;

    int		    numlock_state;
    char*	    tabstop;		    /* per location: 1 == tab-stop */
    enum enc_label  encoding_method;

    char**	    global_argv;
    int		    global_argc;

} rxvt_t;


typedef enum
{
    RESIZE_FONT = 0,
    X_CONFIGURE,
} resize_reason_t;


/* MACROS for colors of individual terminals */

#define VTFG(R)	\
    ((R)->profile.fg)
#define VTBG(R)	\
    ((R)->profile.bg)

#ifdef XFT_SUPPORT
# define VTXFTFG(R)	    \
    ((R)->profile.xftfg)
# define VTXFTBG(R)	    \
    ((R)->profile.xftbg)
#endif	/* XFT_SUPPORT */

# define VTFG_FADE(R)	    \
    ((R)->profile.fg_fade)
# define VTBG_FADE(R)	    \
    ((R)->profile.bg_fade)

# ifdef XFT_SUPPORT
#  define VTXFTFG_FADE(R)	    \
    ((R)->profile.xftfg_fade)
#  define VTXFTBG_FADE(R)	    \
    ((R)->profile.xftbg_fade)
# endif /* XFT_SUPPORT */

#define ISSET_VTFG(R)    \
    (NULL != ((R)->h->rs[Rs_foreground] ) )
#define ISSET_VTBG(R)    \
    (NULL != ((R)->h->rs[Rs_background] ) )



/* MACROS for vts structure */
#define PVTS(R)     (&(R)->vts)

#define SEL(R)	    ((R)->selection)

#define PSCR(R)     (PVTS(R)->screen)



/* macros for private/saved mode of term_t */
#define ISSET_PMODE(R, V)   \
    (PVTS((R))->PrivateModes & (V))
#define SET_PMODE(R, V)  \
    (PVTS((R))->PrivateModes |= (V))
#define UNSET_PMODE(R, V)  \
    (PVTS((R))->PrivateModes &= ~(V))
#define ISSET_SMODE(R, V)   \
    (PVTS((R))->SavedModes & (V))
#define SET_SMODE(R, V)  \
    (PVTS((R))->SavedModes |= (V))
#define UNSET_SMODE(R, V)  \
    (PVTS((R))->SavedModes &= ~(V))


/* Macro to determine weather we should the i-th tab or not */
#define SHOULD_HOLD( r )					\
    (								\
      ( HOLD_ALWAYSBIT & PVTS((r))->holdOption )		\
      ||							\
      (								\
	( HOLD_NORMALBIT & PVTS((r))->holdOption )		\
	&& !WIFEXITED( PVTS((r))->status )			\
      )								\
      ||							\
      (								\
	( HOLD_STATUSBIT & PVTS((r))->holdOption )		\
	&& WEXITSTATUS( PVTS((r))->status ) != 0		\
      )								\
    )


/*****************************************************************************
 *                                PROTOTYPES                                 *
 *****************************************************************************/
void	    rxvt_main_loop(rxvt_t *);
rxvt_t*	    rxvt_init (int, const char *const *);

#endif	    /* __RXVTLIB_H__ */
/*----------------------- end-of-file (H source) -----------------------*/
