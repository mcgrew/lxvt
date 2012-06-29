/*--------------------------------*-H-*---------------------------------*
 * File:    protos.h
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997-2001   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 2004        Sergey Popov <p_sergey@jungo.com>
 * Copyright (c) 2004        Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 * Copyright (C) 2008		  Jehan Hysseo <hysseo@users.sourceforge.net>
 * Copyright (C) 2012        Thomas McGrew <tjmcgrew@gmail.com>
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

#ifndef __PROTOS_H__
#define __PROTOS_H__


/* Begin prototypes of command.c */
void		 rxvt_cmd_write                   __PROTO((rxvt_t* r, int page, const unsigned char* str, unsigned int count));
void    rxvt_xterm_seq                __PROTO((rxvt_t* r, int page, int op, const char* str, unsigned char resp __attribute__((unused))));
void             rxvt_tt_printf                   __PROTO((rxvt_t* r, int page, const char* fmt,...));
void             rxvt_tt_write                    __PROTO((rxvt_t* r, int page, const unsigned char* d, int len));
void             rxvt_pointer_unblank             __PROTO((rxvt_t* r, int page));
void             rxvt_resize_on_subwin            __PROTO((rxvt_t* r, resize_reason_t reason));
void           	 rxvt_resize_on_font           	  __PROTO((rxvt_t* r, char* fontname));
/* End prototypes of command.c */


/* Begin prototypes of encoding.c */
#ifdef MULTICHAR_SET
void             rxvt_set_multichar_encoding      __PROTO((rxvt_t* r, const char* str));
#endif
void             rxvt_decode_dummy                __PROTO((unsigned char* str, int len));
void             rxvt_set_default_locale          __PROTO((rxvt_t *r));
char*            rxvt_get_encoding_from_locale    __PROTO((rxvt_t *r));
void             rxvt_set_default_font_x11        __PROTO((rxvt_t *r));
char*            rxvt_fallback_mfont_x11          __PROTO((rxvt_t *r));
#ifdef XFT_SUPPORT
void             rxvt_set_default_font_xft        __PROTO((rxvt_t *r));
char*            rxvt_fallback_mfont_xft          __PROTO((rxvt_t *r));
#endif
char*            rxvt_encoding_name               __PROTO((rxvt_t *r));
/* End prototypes of defaultfont.c */


/* Begin prototypes of grkelot.c */
#ifdef GREEK_SUPPORT
void             greek_init                       __PROTO((void));
void             greek_end                        __PROTO((void));
void             greek_reset                      __PROTO((void));
void             greek_setmode                    __PROTO((int greek_mode));
int              greek_getmode                    __PROTO((void));
int              greek_xlat                       __PROTO((char* s, int num_chars));
#endif
/* End prototypes of grkelot.c */

/* Begin prototypes of macros.c */
int		 rxvt_parse_macros		  __PROTO((rxvt_t*, const char *, const char *, macro_priority_t priority));
void		 rxvt_cleanup_macros		  __PROTO((rxvt_t*));
Bool		 rxvt_set_action		  (action_t*, char*);
int		 rxvt_process_macros		  __PROTO((rxvt_t*, KeySym, XKeyEvent*));
void	 	 rxvt_free_macros		  __PROTO((rxvt_t*));
int		 rxvt_dispatch_action		  (rxvt_t*, action_t*, XEvent *ev);
/* End prototypes of macros.c */

/* Begin prototypes of init.c */
int              rxvt_init_vars                   __PROTO((rxvt_t* r));
void             rxvt_init_secondary              __PROTO((rxvt_t* r));
void		 rxvt_set_jumpscroll		  __PROTO((rxvt_t* r));
const char    ** rxvt_init_resources              __PROTO((rxvt_t* r, int argc, const char* const *argv));
#ifdef XFT_SUPPORT
void		 rxvt_fade_color                  __PROTO((rxvt_t* r, const XColor *xcol, unsigned long *pix_return, XftColor *xft_return));
#else
void	         rxvt_fade_color                  __PROTO((rxvt_t* r, const XColor *xcol, unsigned long *pix_return, void *xft_return));
#endif
int		 rxvt_set_fgbg_colors		  __PROTO((rxvt_t* r, int page));
int		 rxvt_set_vt_colors		  __PROTO((rxvt_t* r, int page));
void		 rxvt_copy_color                  __PROTO((rxvt_t*, int, int));
void		 rxvt_set_color                   __PROTO((rxvt_t*, int, const XColor*));
void             rxvt_init_env                    __PROTO((rxvt_t *r));
void             rxvt_init_xlocale                __PROTO((rxvt_t *r));
void             rxvt_init_command                __PROTO((rxvt_t* r));
void             rxvt_init_fifo                   __PROTO(( rxvt_t *r ));
CARD32           rxvt_get_desktop                 __PROTO((rxvt_t* r));
void             rxvt_create_show_windows         __PROTO((rxvt_t* r, int argc, const char* const *argv));
void             rxvt_destroy_termwin             __PROTO((rxvt_t* r, int page));
int             rxvt_create_termwin              __PROTO((rxvt_t* r, int page, const char TAINTED * title));
const char*	 getProfileOption		  __PROTO(( rxvt_t *r, int resource ));
Status		 ewmh_message			  __PROTO(( Display *, Window, Window, Atom, long, long, long, long, long));
int		 rxvt_async_exec		  __PROTO((rxvt_t*, const char *));
int              rxvt_run_command                 __PROTO((rxvt_t* r, int page, const char** argv));
termenv_t        rxvt_get_termenv                 __PROTO((const char* str));
char**		 rxvt_string_to_argv    	  __PROTO((const char*, int*));
/* End prototypes of init.c */


/* Begin prototypes of logging.c */
#ifdef UTMP_SUPPORT
void             rxvt_makeutent                   __PROTO((rxvt_t* r, int page, const char* pty, const char* hostname));
void             rxvt_cleanutent                  __PROTO((rxvt_t* r, int page));
#endif
/* End prototypes of logging.c */


/* Begin prototypes of rxvtmem.c */
#ifndef MTRACE
void*            rxvt_malloc                      __PROTO((size_t size)) __attribute__((malloc));
void*            rxvt_calloc                      __PROTO((size_t number, size_t size)) __attribute__((malloc));
void*            rxvt_realloc                     __PROTO((void *ptr, size_t size)) __attribute__((warn_unused_result));
void             rxvt_free                        __PROTO((void *ptr));
#else
#define rxvt_malloc malloc
#define rxvt_calloc calloc
#define rxvt_realloc realloc
#define rxvt_free free
#endif
/* End prototypes of rxvtmem.c */


/* Begin prototypes of main.c */
void             rxvt_privileges                  __PROTO((int mode));
RETSIGTYPE       rxvt_Child_signal                __PROTO((int sig __attribute__((unused))));
RETSIGTYPE       rxvt_Exit_signal                 __PROTO((int sig));
void		 rxvt_exit_request                __PROTO((rxvt_t*));
void             rxvt_clean_exit                  __PROTO((rxvt_t* r));
void             rxvt_privileged_utmp             __PROTO((rxvt_t* r, int page, char action));
void             rxvt_privileged_ttydev           __PROTO((rxvt_t* r, int page, char action));
void             rxvt_tt_winsize                  __PROTO((int fd, unsigned short col, unsigned short row, pid_t pid));
void             rxvt_init_font_x11               __PROTO((rxvt_t *r));
int              rxvt_change_font_x11             __PROTO((rxvt_t* r, const char* fontname));
#ifdef XFT_SUPPORT
int              rxvt_init_font_xft               __PROTO((rxvt_t *r));
int              rxvt_change_font_xft             __PROTO((rxvt_t* r, const char* fontname));
#endif
void             rxvt_set_win_title               __PROTO((rxvt_t* r, Window win, const char* str));
void             rxvt_set_term_title              __PROTO((rxvt_t* r, const unsigned char* str));
void             rxvt_set_icon_name               __PROTO((rxvt_t* r, const unsigned char* str));
void             rxvt_set_window_color            __PROTO((rxvt_t* r, int page, int idx, const char* color));
void             rxvt_recolour_cursor             __PROTO((rxvt_t *r));
#ifdef XFT_SUPPORT
int              rxvt_alloc_xft_color             __PROTO((rxvt_t *r, const XColor *xcol, XftColor* xftcolor));
#endif
int              rxvt_parse_alloc_color           __PROTO((rxvt_t* r, XColor *screen_in_out, const char* colour));
int              rxvt_alloc_color                 __PROTO((rxvt_t* r, XColor *screen_in_out, const char* colour));
void             rxvt_IM_send_spot                __PROTO((rxvt_t *r));
void             rxvt_IM_set_fontset              __PROTO((rxvt_t* r, int idx));
void             rxvt_IM_init_callback            __PROTO((Display *unused __attribute__((unused)), XPointer client_data __attribute__((unused)), XPointer call_data __attribute__((unused))));
void             rxvt_IM_resize                   __PROTO((rxvt_t *r));
rxvt_t         * rxvt_get_r                       __PROTO((void));
/* End prototypes of main.c */


/* Begin prototypes of menubar.c */
#ifdef HAVE_MENUBAR
void             rxvt_menubar_resize              __PROTO((rxvt_t*));
void             rxvt_menubar_create              __PROTO((rxvt_t*));
void             rxvt_menubar_clean_exit          __PROTO((rxvt_t*));
int              rxvt_menubar_hide                __PROTO((rxvt_t*));
int              rxvt_menubar_show                __PROTO((rxvt_t*));
void             rxvt_menubar_expose              __PROTO((rxvt_t*));
void             rxvt_menubar_dispatcher          __PROTO((rxvt_t*, unsigned char*));
void             rxvt_menubar_control             __PROTO((rxvt_t*, XButtonEvent*));
void             rxvt_menubar_load_file           __PROTO((rxvt_t*, const unsigned char*));
int              rxvt_is_menubar_win              __PROTO((rxvt_t*, Window));
unsigned short   rxvt_menubar_height              __PROTO((rxvt_t*));
unsigned short   rxvt_menubar_rheight             __PROTO((rxvt_t*));
int              rxvt_menubar_visible             __PROTO((rxvt_t*));
void		 rxvt_menu_show			  __PROTO((rxvt_t*));
int		 rxvt_menu_select		  __PROTO((rxvt_t*, XButtonEvent*));
#endif
/* End prototypes of menubar.c */


/* Begin prototypes of misc.c */
char           * rxvt_r_basename                  __PROTO((const char* str));
int              rxvt_str_match                   __PROTO((const char* s1, const char* s2));
char*            rxvt_str_trim                    __PROTO((char* str));
int              rxvt_str_escaped                 __PROTO((char* str));
int		 rxvt_percent_interpolate         __PROTO((rxvt_t*, int, const char *, int, char *, int));
char**           rxvt_splitcommastring            __PROTO((const char* cs));
void             rxvt_draw_shadow                 __PROTO((Display*, Window, GC , unsigned long topShadow, unsigned long botShadow, int x, int y, int w, int h));
char*            rxvt_File_find                   __PROTO((const char *file, const char *ext, const char *path));
void             rxvt_draw_triangle               __PROTO(( Display*, Window, GC, unsigned long topShadow, unsigned long botShadow, int x, int y, int w, int type));
/* End prototypes of misc.c */


/* Begin prototypes of netdisp.c */
#ifdef NET_DISPLAY
char           * rxvt_network_display             __PROTO((const char* display));
#endif
/* End prototypes of netdisp.c */


/* Begin prototypes of ptytty.c */
int              rxvt_get_pty                     __PROTO((int *fd_tty, char** ttydev));
int              rxvt_get_tty                     __PROTO((const char* ttydev));
int              rxvt_control_tty                 __PROTO((int fd_tty, const char* ttydev));
/* End prototypes of ptytty.c */


/* Begin prototypes of screen.c */
void
rxvt_draw_string_x11 (rxvt_t* r, Window win, GC gc, Region refreshRegion,
	int x, int y, char* str, int len, int (*draw_string)());
#ifdef XFT_SUPPORT
void
rxvt_draw_string_xft (rxvt_t* r, Drawable d, GC gc, Region refreshRegion,
	rend_t rend, int pfont,
	XftDraw* win, XftColor* fore, int x, int y, char* str, int len,
	void (*xftdraw_string)());
#endif
void             rxvt_init_screen                 __PROTO((rxvt_t* r));
void             rxvt_scr_reset                   __PROTO((rxvt_t* r, int page));
void             rxvt_scr_release                 __PROTO((rxvt_t* r, int page));
void             rxvt_scr_poweron                 __PROTO((rxvt_t* r, int page));
void             rxvt_scr_cursor                  __PROTO((rxvt_t* r, int page, int mode));
int              rxvt_scr_change_screen           __PROTO((rxvt_t* r, int page, int scrn));
void             rxvt_scr_color                   __PROTO((rxvt_t* r, int page, unsigned int color, int fgbg));
void             rxvt_scr_rendition               __PROTO((rxvt_t* r, int page, int set, rend_t style));
int              rxvt_scroll_text                 __PROTO((rxvt_t* r, int page, int row1, int row2, int count, int spec));
void             rxvt_scr_add_lines               __PROTO((rxvt_t* r, int page, const unsigned char* str, int nlines, int len));
void             rxvt_scr_backspace               __PROTO((rxvt_t* r, int page));
void             rxvt_scr_tab                     __PROTO((rxvt_t* r, int page, int count));
void             rxvt_scr_backindex               __PROTO((rxvt_t* r, int page));
void             rxvt_scr_forwardindex            __PROTO((rxvt_t* r, int page));
void             rxvt_scr_gotorc                  __PROTO((rxvt_t* r, int page, int row, int col, int relative));
void             rxvt_scr_index                   __PROTO((rxvt_t* r, int page, enum page_dirn direction));
void             rxvt_scr_erase_line              __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_erase_screen            __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_E                       __PROTO((rxvt_t* r, int page));
void             rxvt_scr_insdel_lines            __PROTO((rxvt_t* r, int page, int count, int insdel));
void             rxvt_scr_insdel_chars            __PROTO((rxvt_t* r, int page, int count, int insdel));
void             rxvt_scr_scroll_region           __PROTO((rxvt_t* r, int page, int top, int bot));
void             rxvt_scr_cursor_visible          __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_autowrap                __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_relative_origin         __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_insert_mode             __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_set_tab                 __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_rvideo_mode             __PROTO((rxvt_t* r, int page, int mode));
void             rxvt_scr_report_position         __PROTO((rxvt_t* r, int page));
void             rxvt_scr_charset_choose          __PROTO((rxvt_t* r, int page, int set));
void             rxvt_scr_charset_set             __PROTO((rxvt_t* r, int page, int set, unsigned int ch));
int              rxvt_scr_get_fgcolor             __PROTO((rxvt_t *r));
int              rxvt_scr_get_bgcolor             __PROTO((rxvt_t *r));
void             rxvt_scr_expose                  __PROTO((rxvt_t* r, int page, int x, int y, int width, int height, Bool refresh));
void             rxvt_scr_touch                   __PROTO((rxvt_t* r, int page, Bool refresh));
int              rxvt_scr_move_to                 __PROTO((rxvt_t* r, int page, int y, int len));
int              rxvt_scr_page                    __PROTO((rxvt_t* r, int page, enum page_dirn direction, int nlines));
void             rxvt_scr_bell                    __PROTO((rxvt_t *r, int page));
void             rxvt_scr_printscreen             __PROTO((rxvt_t* r, int page, int fullhist, int pretty, int linecont, const char *pipeName));
void             rxvt_scr_refresh                 __PROTO((rxvt_t* r, int page, unsigned char refresh_type));
void             rxvt_scr_clear                   __PROTO((rxvt_t* r, int page));
void             rxvt_scr_dump                    __PROTO((rxvt_t* r, int page, int fd));
void             rxvt_selection_check             __PROTO((rxvt_t* r, int page, int check_more));
int              rxvt_selection_paste             __PROTO((rxvt_t* r, Window win, Atom prop, Bool delete_prop));
void             rxvt_selection_property          __PROTO((rxvt_t* r, Window win, Atom prop));
void             rxvt_selection_request           __PROTO((rxvt_t* r, int page, Time tm, int x, int y));
void             rxvt_selection_request_by_sel    __PROTO((rxvt_t* r, int page, Time tm, int x, int y, int sel));
void             rxvt_process_selectionclear      __PROTO((rxvt_t* r, int page));
void             rxvt_selection_make              __PROTO((rxvt_t* r, int page, Time tm));
void             rxvt_selection_click             __PROTO((rxvt_t* r, int page, int clicks, int x, int y));
void             rxvt_selection_extend            __PROTO((rxvt_t* r, int page, int x, int y, int flag));
void             rxvt_selection_rotate            __PROTO((rxvt_t* r, int page, int x, int y));
void             rxvt_paste_file                  __PROTO((rxvt_t* r, int page, Time tm, int x, int y, char* filename));
void             rxvt_process_selectionrequest    __PROTO((rxvt_t* r, int page, const XSelectionRequestEvent *rq));
void             rxvt_pixel_position              __PROTO((rxvt_t* r, int *x, int *y));
/* End prototypes of screen.c */


/* Begin prototypes of scrollbar-next.c */
#ifdef NEXT_SCROLLBAR
void             rxvt_scrollbar_init_next         __PROTO((rxvt_t *r));
void             rxvt_scrollbar_exit_next         __PROTO((rxvt_t *r));
int              rxvt_scrollbar_show_next         __PROTO((rxvt_t* r, int update, int last_top, int last_bot, int scroller_len));
#endif
/* End prototypes of scrollbar-next.c */


/* Begin prototypes of scrollbar-rxvt.c */
#ifdef RXVT_SCROLLBAR
void             rxvt_scrollbar_init_rxvt         __PROTO((rxvt_t *r));
void             rxvt_scrollbar_exit_rxvt         __PROTO((rxvt_t *r));
int              rxvt_scrollbar_show_rxvt         __PROTO((rxvt_t* r, int update __attribute__((unused)), int last_top, int last_bot, int scroller_len));
#endif
/* End prototypes of scrollbar-rxvt.c */


/* Begin prototypes of scrollbar-xterm.c */
#ifdef XTERM_SCROLLBAR
void             rxvt_scrollbar_init_xterm        __PROTO((rxvt_t *r));
void             rxvt_scrollbar_exit_xterm        __PROTO((rxvt_t *r));
int              rxvt_scrollbar_show_xterm        __PROTO((rxvt_t* r, int update __attribute__((unused)), int last_top, int last_bot, int scroller_len));
#endif
/* End prototypes of scrollbar-xterm.c */


/* Begin prototypes of scrollbar-plain.c */
#ifdef PLAIN_SCROLLBAR
void             rxvt_scrollbar_init_plain        __PROTO((rxvt_t *r));
void             rxvt_scrollbar_exit_plain        __PROTO((rxvt_t *r));
int              rxvt_scrollbar_show_plain        __PROTO((rxvt_t* r, int update __attribute__((unused)), int last_top, int last_bot, int scroller_len));
#endif
/* End prototypes of scrollbar-plain.c */


/* Begin prototypes of scrollbar-sgi.c */
#ifdef SGI_SCROLLBAR
void             rxvt_scrollbar_init_sgi          __PROTO((rxvt_t *r));
void             rxvt_scrollbar_exit_sgi          __PROTO((rxvt_t *r));
int              rxvt_scrollbar_show_sgi          __PROTO((rxvt_t* r, int update __attribute__((unused)), int last_top, int last_bot, int scroller_len));
#endif
/* End prototypes of scrollbar-sgi.c */


/* Begin prototypes of scrollbar.c */
#ifdef HAVE_SCROLLBARS
int              rxvt_scrollbar_visible           __PROTO((rxvt_t* r));
void             rxvt_scrollbar_init              __PROTO((rxvt_t* r));
void             rxvt_scrollbar_create            __PROTO((rxvt_t* r));
void             rxvt_scrollbar_clean_exit        __PROTO((rxvt_t* r));
void             rxvt_scrollbar_resize            __PROTO((rxvt_t* r));
int              rxvt_scrollbar_hide              __PROTO((rxvt_t* r));
int              rxvt_scrollbar_show              __PROTO((rxvt_t* r));
int              rxvt_scrollbar_update            __PROTO((rxvt_t* r, int update));
unsigned short   rxvt_scrollbar_width             __PROTO((rxvt_t* r));
unsigned short   rxvt_scrollbar_rwidth            __PROTO((rxvt_t* r));
int              rxvt_is_scrollbar_win            __PROTO((rxvt_t* r, Window w));
unsigned long	 rxvt_scrollbar_bg		  __PROTO((rxvt_t* r));
#endif
/* End prototypes of scrollbar.c */


/* Begin prototypes of strings.c */
#ifdef HAVE_WCHAR_H
wchar_t*         rxvt_mbstowcs                    __PROTO((const char* str));
char*            rxvt_wcstoutf8                   __PROTO((const wchar_t* str));
#endif	/* HAVE_WCHAR_H */
/* End prototypes of strings.c */

/* Begin prototypes of transparent.c */
int		rxvt_set_opacity	     	 __PROTO((rxvt_t*));
void		rxvt_process_reparentnotify   	 __PROTO((rxvt_t*, XEvent*));
#ifdef TRANSPARENT
void		rxvt_toggle_transparency	 __PROTO((rxvt_t*));
void		refreshRootBGVars		 __PROTO((rxvt_t*));
int             rxvt_check_our_parents           __PROTO((rxvt_t*));
#endif
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
void		rxvt_refresh_bg_image		 __PROTO((rxvt_t*, int, Bool));
#endif
#ifdef TINTING_SUPPORT
# if defined(HAVE_LIBXRENDER)
void		xrenderShadeParentPixmap     	 __PROTO((rxvt_t*, Pixmap, int, int, unsigned, unsigned, Bool));
# else
void            rxvt_shade_pixmap                __PROTO((rxvt_t*, Drawable, int, int, unsigned, unsigned));
# endif
#endif
/* End prototypes of transparent.c */

/* Begin prototypes of xdefaults.c */
int              rxvt_save_options                __PROTO((rxvt_t* r, const char* filename));
void             rxvt_get_options                 __PROTO((rxvt_t* r, int argc, const char* const *argv));
void             rxvt_extract_resources           __PROTO((rxvt_t* r, Display *display __attribute__((unused)), const char* name));
/* End prototypes of xdefaults.c */


#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
/* Begin prototypes of pixmap.c */
int              rxvt_scale_pixmap                __PROTO((rxvt_t* r, int page, const char* geom));
void             rxvt_resize_pixmap               __PROTO((rxvt_t* r, int page));
Pixmap           rxvt_load_bg_pixmap              __PROTO((rxvt_t* r, int page, const char* file));
Pixmap           rxvt_load_pixmap                 __PROTO((rxvt_t* r, const char* file, long* pw, long* ph));
/* End prototypes of pixmap.c */


/* Begin prototypes of jpg.c */
#ifdef USE_JPEG
long             JpegReadFileToPixmap             __PROTO((Display* display, Window window, GC gc, char* filename, Pixmap* pixmap, long* w, long* h));
#endif
/* End prototypes of jpg.c */


/* Begin prototypes of png.c */
#ifdef USE_PNG
long             PngReadFileToPixmap             __PROTO((Display* display, Window window, GC gc, char* filename, Pixmap* pixmap, long* w, long* h));
#endif
/* End prototypes of png.c */
#endif /* BACKGROUND_IMAGE || TRANSPARENT */


/* Begin prototypes of tabbar.c */
void             rxvt_append_page               __PROTO((rxvt_t*, const char TAINTED *title, const char *command));
void             rxvt_kill_page                 __PROTO((rxvt_t*, short));
void             rxvt_remove_page               __PROTO((rxvt_t*, short));
void             rxvt_activate_page             __PROTO((rxvt_t*, short));
#ifdef HAVE_TABS
void             rxvt_tabbar_init               __PROTO((rxvt_t*));
#ifdef HAVE_TABBAR
void		 rxvt_tabbar_set_visible_tabs   __PROTO((rxvt_t*, Bool));
void		 refresh_tabbar_tab		__PROTO((rxvt_t*, int));
void             rxvt_tabbar_resize             __PROTO((rxvt_t*));
void             rxvt_tabbar_dispatcher         __PROTO((rxvt_t*, XButtonEvent*));
void		 rxvt_tabbar_button_release     __PROTO((rxvt_t*, XButtonEvent*));
int              rxvt_tabbar_visible            __PROTO((rxvt_t*));
void             rxvt_tabbar_expose             __PROTO((rxvt_t*, XEvent*));
int              rxvt_tabbar_hide               __PROTO((rxvt_t*));
int              rxvt_tabbar_show               __PROTO((rxvt_t*));
void             rxvt_tabbar_create             __PROTO((rxvt_t*));
void             rxvt_tabbar_clean_exit         __PROTO((rxvt_t*));
unsigned short   rxvt_tabbar_height             __PROTO((rxvt_t*));
unsigned short   rxvt_tabbar_rheight            __PROTO((rxvt_t*));
unsigned int     rxvt_tab_width                 __PROTO((rxvt_t*));
int              rxvt_is_tabbar_win             __PROTO((rxvt_t*, Window));
void             rxvt_tabbar_draw_buttons       __PROTO((rxvt_t*));
void		 rxvt_adjust_fd_number	        __PROTO((rxvt_t* r));
void             rxvt_tabbar_change_color       __PROTO((rxvt_t*, int, const char*));
void             rxvt_tabbar_highlight_tab      __PROTO((rxvt_t*, short, Bool));
void             rxvt_tabbar_move_tab           __PROTO((rxvt_t*, short));
#endif
#endif
void             rxvt_tabbar_set_title          __PROTO((rxvt_t*, short, const unsigned char TAINTED *));
void		 sync_tab_title		        __PROTO((rxvt_t*, int));
/* End prototypes of tabbar.c */

/* Begin prototypes of session.c */
#ifdef HAVE_X11_SM_SMLIB_H
void            rxvt_process_ice_msgs          __PROTO((rxvt_t* r));
void            rxvt_session_init              __PROTO((rxvt_t* r));
void            rxvt_session_exit              __PROTO((rxvt_t* r));
#endif	/* HAVE_X11_SM_SMLIB_H */
/* End prototypes of session.c */

/* Begin prototypes of debug.c */
int             rxvt_msg                       __PROTO((uint32_t, uint32_t, const char*, ...));
void            rxvt_parse_dbg_arguments       __PROTO((int argc, const char* const*));
/* End prototypes of debug.c */

#endif  /* __PROTOS_H__ */
/*----------------------- end-of-file (H source) -----------------------*/
