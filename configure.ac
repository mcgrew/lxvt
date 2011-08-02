dnl# All portions of code are copyright by their respective author/s.
dnl#
dnl#  Copyright (c) 1998-2001   Geoff Wing <gcw@pobox.com>
dnl#  Copyright (c) 2003-2004   Marc Lehmann <pcg@goof.com>
dnl#  Copyright (c) 2004        Hiramatsu Yoshifumi
dnl#  Copyright (c) 2004-2006   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
dnl#  Copyright (c) 2006	Gautam Iyer <gi1242@users.sourceforge.net>
dnl#	Copyright (c) 2007	Jehan Hysseo <hysseo@users.sourceforge.net>
dnl#
dnl# This program is free software; you can redistribute it and/or
dnl# modify it under the terms of the GNU General Public License as
dnl# published by the Free Software Foundation; either version 2 of
dnl# the License, or (at your option) any later version.
dnl# 
dnl# This program is distributed in the hope that it will be useful,
dnl# but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl# GNU General Public License for more details.
dnl# 
dnl# You should have received a copy of the GNU General Public License
dnl# along with this program; if not, write to the Free Software
dnl# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl#

dnl 2006-02-20 gi1242: If the version contains a letter, then it sometimes
dnl produces spurious results when programs use an escape sequence to query the
dnl terminal version.
AC_INIT(drxvt, 0.6.0, [dylan-rxvt@dylex.net])
AM_INIT_AUTOMAKE(drxvt, 0.6.0)

AM_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR([src/main.c])


AM_MAINTAINER_MODE

dnl Check OS
AC_CANONICAL_HOST
local_os_type="unknown"
case "$host" in
*-cygwin*)
    AC_DEFINE(OS_CYGWIN, [], [set OS to cygwin])
    local_os_type="cygwin"
    ;;
*-linux*)
    AC_DEFINE(OS_LINUX, [], [set OS to linux])
	dnl hack for some 64-bit systems
	AC_DEFINE(_GNU_SOURCE, [], [set GNU source flag for linux])
    local_os_type="linux"
    ;;
*-irix*)
    AC_DEFINE(OS_IRIX, [], [set OS to irix])
    local_os_type="irix"
    ;;
*-sunos*)
    AC_DEFINE(OS_SUNOS, [], [set OS to sunos])
    local_os_type="sunos"
    ;;
*-solaris*)
    AC_DEFINE(OS_SOLARIS, [], [set OS to solaris])
    local_os_type="solaris"
    ;;
*-aix*)
    AC_DEFINE(OS_AIX, [], [set OS to aix])
    local_os_type="aix"
    ;;
*-hpux*)
    AC_DEFINE(OS_HPUX, [], [set OS to hp-ux])
    local_os_type="hpux"
    ;;
*-freebsd*)
    AC_DEFINE(OS_BSD, [], [set OS to bsd])
    AC_DEFINE(OS_FREEBSD, [], [set OS to freebsd])
    local_os_type="freebsd"

    dnl our hack for freebsd system
    CFLAGS="$CFLAGS -I/usr/local/include"
    LIBS="$LIBS -L/usr/local/lib"
    ;;
*-netbsd*)
    AC_DEFINE(OS_BSD, [], [set OS to bsd])
    AC_DEFINE(OS_NETBSD, [], [set OS to netbsd])
    local_os_type="netbsd"

    dnl our hack for netbsd system
    CFLAGS="$CFLAGS -I/usr/local/include"
    LIBS="$LIBS -L/usr/local/lib"
    ;;
*-openbsd*)
    AC_DEFINE(OS_BSD, [], [set OS to bsd])
    AC_DEFINE(OS_OPENBSD, [], [set OS to openbsd])
    local_os_type="openbsd"

	dnl get openbsd release number, remove `.' from release number
	OBSD_REL=`/usr/bin/uname -r | sed 's/\.//g'`
	AC_DEFINE_UNQUOTED(OS_OPENBSD_REL, ${OBSD_REL}, [set openbsd release number])

    dnl our hack for openbsd system
    CFLAGS="$CFLAGS -I/usr/local/include"
    LIBS="$LIBS -L/usr/local/lib"
    ;;
*-osf*)
    AC_DEFINE(OS_OSF, [], [set OS to osf])
    local_os_type="osf"
    ;;
*-darwin*)
    AC_DEFINE(OS_BSD, [], [set OS to bsd])
    AC_DEFINE(OS_DARWIN, [], [set OS to darwin])
    local_os_type="darwin"
    ;;
*-sco*)
    AC_DEFINE(OS_SCO, [], [set OS to sco])
    AC_DEFINE(OS_SVR4, [], [set OS to svr4])
    local_os_type="sco"
    AC_MSG_ERROR([Mrxvt does not support f*cking SCO's products!])
esac


dnl Configuration options
support_everything=no
support_minimal=no
support_debug=no
support_frills=yes
support_selectionscrolling=yes
support_mousewheel=yes
support_mouseslipwheel=no
support_half_shadow=yes
support_utmp=yes
support_wtmp=yes
support_lastlog=yes
support_sessionmgr=yes
support_utempter=no
support_linespace=yes
support_24bits=no
support_256colors=yes
support_cursor_blink=yes
support_pointer_blank=yes
support_smart_resize=no
dnl: support_fading=yes
support_xrender=yes
support_xft=no
support_xim=no
support_cjk=no
support_ttygid=yes
support_backspace_key=yes
support_delete_key=yes
support_resources=yes
support_xgetdefault=no
support_swapscreen=yes
support_old_selection=no



AC_ARG_ENABLE(
    everything,
    AC_HELP_STRING([--enable-everything],[enable all options [[no]]]),
    [support_everything=$enableval],
    [support_everything=no]
)
if test "x$support_everything" = "xyes"; then
    support_debug=yes
    support_frills=yes
    support_selectionscrolling=yes
    support_mousewheel=yes
    support_mouseslipwheel=yes
    support_half_shadow=yes
    support_utmp=yes
    support_wtmp=yes
    support_lastlog=yes
    support_sessionmgr=yes
    support_utempter=yes
    support_linespace=yes
    support_24bits=yes
    support_256colors=yes
    support_cursor_blink=yes
    support_pointer_blank=yes
    support_smart_resize=yes
    dnl: support_fading=yes
    support_xrender=yes
    support_xft=yes
    support_xim=yes
    support_cjk=yes
    support_ttygid=yes
    support_backspace_key=yes
    support_delete_key=yes
    support_resources=yes
    support_xgetdefault=no
    support_swapscreen=yes
    support_old_selection=no
fi

AC_ARG_ENABLE(
    minimal,
    AC_HELP_STRING([--enable-minimal],[enable minimal options [[no]]]),
    [support_minimal=$enableval],
    [support_minimal=no]
)
if test "x$support_minimal" = "xyes"; then
    support_debug=no
    support_frills=no
    support_selectionscrolling=no
    support_mousewheel=no
    support_mouseslipwheel=no
    support_half_shadow=no
    support_utmp=no
    support_wtmp=no
    support_lastlog=no
    support_sessionmgr=no
    support_utempter=no
    support_linespace=no
    support_24bits=no
    support_256colors=no
    support_cursor_blink=no
    support_pointer_blank=no
    support_smart_resize=no
    dnl: support_fading=no
    support_xrender=no
    support_xft=no
    support_xim=no
    support_cjk=no
    support_ttygid=yes
    support_backspace_key=yes
    support_delete_key=yes
    support_resources=yes
    support_xgetdefault=no
    support_swapscreen=yes
    support_old_selection=no
fi


AC_ARG_ENABLE(
    debug,
    AC_HELP_STRING([--enable-debug],[enable debug mode - for developper [[no]]]),
    [support_debug=$enableval]
)
if test "x$support_debug" = "xyes"; then
    AC_DEFINE(DEBUG, [], [set DEBUG mode])
    CFLAGS="$CFLAGS -g"
else
    AC_DEFINE(NDEBUG, [], [unset DEBUG mode])
fi


AC_ARG_ENABLE(
    frills,
    AC_HELP_STRING([--enable-frills],[enable internal borders / smart resizing [[yes]]]),
    [support_frills=$enableval]
)
if test "x$support_frills" != "xyes"; then
    AC_DEFINE(NO_FRILLS, [], [enable internal borders / smart resizing])
fi


AC_ARG_ENABLE(
    selectionscrolling,
    AC_HELP_STRING([--enable-selectionscrolling],[enable continual scrolling on selection [[yes]]]),
    [support_selectionscrolling=$enableval]
)
if test "x$support_selectionscrolling" = "xyes"; then
    AC_DEFINE(SELECTION_SCROLLING, [], [enable continual scrolling on selection])
fi


AC_ARG_ENABLE(
    mousewheel,
    AC_HELP_STRING([--enable-mousewheel],[enable scrolling via mouse wheel or buttons 4 & 5 [[yes]]]),
    [support_mousewheel=$enableval]
)
if test "x$support_mousewheel" = "xyes"; then
    AC_DEFINE(MOUSE_WHEEL, [], [enable scrolling via mouse wheel])
fi


AC_ARG_ENABLE(
    mouseslipwheel,
    AC_HELP_STRING([--enable-mouseslipwheel],[enable slip scrolling via mouse wheel or buttons 4 & 5 [[no]]]),
    [support_mouseslipwheel=$enableval]
)
if test "x$support_mousewheel x$support_mouseslipwheel" = "xyes xyes"; then
    AC_DEFINE(MOUSE_SLIP_WHEELING, [], [enable slip scrolling via mouse wheel])
fi


AC_ARG_ENABLE(
    utmp,
    AC_HELP_STRING([--enable-utmp],[enable utmp/utmpx support [[yes]]]),
    [support_utmp=$enableval]
)
if test "x$support_utmp" = "xyes"; then
    AC_DEFINE(UTMP_SUPPORT, [], [set utmp support])
fi


AC_ARG_ENABLE(
    wtmp,
    AC_HELP_STRING([--enable-wtmp],[enable wtmp support (need utmp support) [[yes]]]),
    [support_wtmp=$enableval]
)
dnl depends on utmp support
if test "x$support_utmp x$support_wtmp" = "xyes xyes"; then
    AC_DEFINE(WTMP_SUPPORT, [], [set wtmp support])
fi


AC_ARG_ENABLE(
    lastlog,
    AC_HELP_STRING([--enable-lastlog],[enable lastlog support [[yes]]]),
    [support_lastlog=$enableval]
)
dnl depends on utmp support
if test "x$support_utmp x$support_lastlog" = "xyes xyes"; then
    AC_DEFINE(LASTLOG_SUPPORT, [], [set lastlog support])
fi
AM_CONDITIONAL(UTMP_SUPPORT, test "x$support_utmp" = "xyes")


AC_ARG_ENABLE(
    sessionmgr,
    AC_HELP_STRING([--enable-sessionmgr],[enable X session manager [[yes]]]),
    [support_sessionmgr=$enableval],
)


AC_ARG_ENABLE(
    utempter,
    AC_HELP_STRING([--enable-utempter],[enable utempter library for utmp/wtmp/lastlog [[no]]]),
    [support_utempter=$enableval],
)


AC_ARG_ENABLE(
    linespace,
    AC_HELP_STRING([--enable-linespace],[enable linespace support [[yes]]]),
    [support_linespace=$enableval]
)
if test "x$support_linespace" != "xyes"; then
    AC_DEFINE(NO_LINESPACE, [], [unset linespace support])
fi


AC_ARG_ENABLE(
    24bits,
    AC_HELP_STRING([--enable-24bits],[prefer 24 bits color [[no]]]),
    [support_24bits=$enableval]
)
if test "x$support_24bits" = "xyes"; then
    AC_DEFINE(PREFER_24BIT, [], [set 24 bits color support])
fi


AC_ARG_ENABLE(
    256colors,
    AC_HELP_STRING([--enable-256colors],[use 256 color [[yes]]]),
    [support_256colors=$enableval]
)
if test "x$support_256colors" = "xyes"; then
    AC_DEFINE(TTY_256COLOR, [], [set 256 color support])
fi


AC_ARG_ENABLE(
    cursor-blink,
    AC_HELP_STRING([--enable-cursor-blink],[enable cursor blinking [[yes]]]),
    [support_cursor_blink=$enableval]
)
if test "x$support_cursor_blink" = "xyes"; then
    AC_DEFINE(CURSOR_BLINK, [], [set cursor blinking support])
fi


AC_ARG_ENABLE(
    pointer-blank,
    AC_HELP_STRING([--enable-pointer-blank],[enable blank pointer [[yes]]]),
    [support_pointer_blank=$enableval]
)
if test "x$support_pointer_blank" = "xyes"; then
    AC_DEFINE(POINTER_BLANK, [], [set blank pointer support])
fi


dnl# Make an option instead of a feature
dnl# AC_ARG_ENABLE(
dnl#     smart-resize,
dnl#     AC_HELP_STRING([--enable-smart-resize],[enable smart resizing [[no]]]),
dnl#     [support_smart_resize=$enableval]
dnl# )
dnl# if test "x$support_smart_resize" = "xyes"; then
dnl#     AC_DEFINE(SMART_RESIZE, [], [set smart resizing support])
dnl# fi


dnl: No longer a compile time option, and OFF_FOCUS_FADING is now always
dnl: enabled. 2006-09-14 gi1242.
dnl:
dnl: AC_ARG_ENABLE(
dnl:     fading,
dnl:     AC_HELP_STRING([--enable-fading],[enable colors fading when off focus [[yes]]]),
dnl:     [support_fading=$enableval]
dnl: )
dnl: if test "x$support_fading" = "xyes"; then
dnl:     AC_DEFINE(OFF_FOCUS_FADING, [], [set fading support])
dnl: fi


AC_ARG_ENABLE(
    xrender,
    AC_HELP_STRING([--enable-xrender],[tint using XRender (slower but nicer results) [[yes]]]),
    [support_xrender=$enableval]
)


AC_ARG_ENABLE(
    xft,
    AC_HELP_STRING([--enable-xft],[enable freetype font support [[no]]]),
    [support_xft=$enableval]
)


AC_ARG_ENABLE(
    xim,
    AC_HELP_STRING([--enable-xim],[enable XIM support [[no]]]),
    [support_xim=$enableval]
)


AC_ARG_ENABLE(
    cjk,
    AC_HELP_STRING([--enable-cjk],[enable CJK multichar language support [[no]]]),
    [support_cjk=$enableval]
)


AC_ARG_ENABLE(
    ttygid,
    AC_HELP_STRING([--enable-ttygid],[enable ttygid to group tty [[yes]]]),
    [support_ttygid=$enableval]
)
if test "x$support_ttygid" = "xyes"; then
    AC_DEFINE(TTY_GID_SUPPORT, [], [set ttygid support])
fi


AC_ARG_ENABLE(
    backspace-key,
    AC_HELP_STRING([--enable-backspace-key],[enable handling of the backspace key [[yes]]]),
    [support_backspace_key=$enableval]
)
if test "x$support_backspace_key" != "xyes"; then
    AC_DEFINE(NO_BACKSPACE_KEY, [], [disable handling of backspace key])
fi


AC_ARG_ENABLE(
    delete-key,
    AC_HELP_STRING([--enable-delete-key],[enable handling of the delete key [[yes]]]),
    [support_delete_key=$enableval]
)
if test "x$support_delete_key" != "xyes"; then
    AC_DEFINE(NO_DELETE_KEY, [], [disable handling of the delete key])
fi

AC_ARG_ENABLE(
    resources,
    AC_HELP_STRING([--enable-resources],[enable all resource checking [[yes]]]),
    [support_resources=$enableval]
)
if test "x$support_resources" != "xyes"; then
    AC_DEFINE(NO_RESOURCES, [], [disable all resource checking])
fi


AC_ARG_ENABLE(
    swapscreen,
    AC_HELP_STRING([--enable-swapscreen],[enable swap screen support [[yes]]]),
    [support_swapscreen=$enableval]
)
if test "x$support_swapscreen" != "xyes"; then
    AC_DEFINE(NO_SECONDARY_SCREEN, [], [disable swap screen support])
fi


AC_ARG_ENABLE(
    old-selection,
    AC_HELP_STRING([--enable-old-selection],[enable old mouse selection code [[no]]]),
    [support_old_selection=$enableval]
)
if test "x$support_old_selection" = "xyes"; then
    AC_DEFINE(NO_NEW_SELECTION, [], [enable old mouse selection])
else
    AC_DEFINE(NO_OLD_SELECTION, [], [disable old mouse selection])
fi


AC_ARG_WITH(
    term,
    AC_HELP_STRING([--with-term=NAME],[set the terminal to NAME [[rxvt]]]),
    [term_name_option=$withval],
    [term_name_option=rxvt]
)
AC_DEFINE_UNQUOTED(TERMENV, "$term_name_option", [default TERM name])


AC_ARG_WITH(
    line-space,
    AC_HELP_STRING([--with-line-space=NUM],[set the space between lines [[0]]]),
    [line_space_option=$withval],
    [line_space_option=0]
)
AC_DEFINE_UNQUOTED(LINESPACE, $line_space_option, [the space between lines])


AC_ARG_WITH(
    int-border,
    AC_HELP_STRING([--with-int-border=NUM],[set the width of internal border, frills option must be enabled [[0]]]),
    [int_border_option=$withval],
    [int_border_option=0]
)
AC_DEFINE_UNQUOTED(INTERNALBORDERWIDTH, $int_border_option, [the width of internal border])


AC_ARG_WITH(
    ext-border,
    AC_HELP_STRING([--with-ext-border=NUM],[set the width of external border, frills option must be enabled [[0]]]),
    [ext_border_option=$withval],
    [ext_border_option=0]
)
AC_DEFINE_UNQUOTED(EXTERNALBORDERWIDTH, $ext_border_option, [the width of external border])


AC_ARG_WITH(
    save-lines,
    AC_HELP_STRING([--with-save-lines=NUM],[set the lines to save in scroll [[100]]]),
    [save_lines_option=$withval],
    [save_lines_option=100]
)
AC_DEFINE_UNQUOTED(SAVELINES, $save_lines_option, [the lines to save in scroll])


AC_ARG_WITH(
    max-profiles,
    AC_HELP_STRING([--with-max-profiles=NUM],[set the max number of terminal [[5]]]),
    [max_prof_option=$withval],
    [max_prof_option=5]
)
AC_DEFINE_UNQUOTED(MAX_PROFILES, $max_prof_option, [the max number of profiles])


withval=""
AC_ARG_WITH(
    terminfo,
    AC_HELP_STRING([--with-terminfo=PATH],[set the path to the terminfo tree to PATH]),
    [terminfo_option=$withval],
    [terminfo_option="/usr/share/terminfo"]
)
if test "x$withval" != "x"; then
    AC_DEFINE_UNQUOTED(RXVT_TERMINFO, "$terminfo_option", [terminfo path])
fi


AC_ARG_WITH(
    encoding,
    AC_HELP_STRING([--with-encoding=NAME],[set language default encoding (eucj|sjis|big5|gb|gbk|kr|noenc) to NAME (default is noenc)]),
    [encoding_option=$withval],
    [encoding_option="noenc"]
)



dnl Checks for programs.
AC_PROG_CC(gcc cc egcs)
dnl AX_CFLAGS_WARN_ALL
if test "x$GCC" = "xyes"; then
    CFLAGS="$CFLAGS -Wall -fPIC"
fi
AC_PROG_MAKE_SET
AC_PROG_INSTALL
#AM_PROG_LIBTOOL



dnl Checks for libraries.

dnl FreeBSD needs to link libutil for openpty and libxpg4 for
dnl setlocale. Why this looks only necessary after FreeBSD
dnl 5.2.1?!!!
if test "x$local_os_type" = "xfreebsd" -o \
        "x$local_os_type" = "xopenbsd" -o \
        "x$local_os_type" = "xnetbsd"; then
    AC_CHECK_LIB(xpg4, setlocale)
    AC_CHECK_LIB(util, openpty)
fi


AC_PATH_X
if test "x$no_x" = "xyes"; then
    AC_MSG_ERROR([Mrxvt requires the X Window System libraries and headers.])
fi

dnl Warn if the detected path differs from the supplied
if test "x$x_includes" != "x" -a \
        "x$ac_x_includes" != "x" -a \
        "$x_includes" != "$ac_x_includes"; then
	AC_MSG_WARN([X includes path detected differs from the supplied!])
fi
if test "x$x_libraries" != "x" -a \
        "x$ac_x_libraries" != "x" -a \
        "$x_libraries" != "$ac_x_libraries"; then
	AC_MSG_WARN([X libraries path detected differs from the supplied!])
fi

dnl In case x_includes and x_libraries are not empty
test "x$x_includes" = "x" && x_includes="$ac_x_includes"
test "x$x_libraries" = "x" && x_libraries="$ac_x_libraries"

test "x$x_includes" != "x" && CFLAGS="$CFLAGS -I$x_includes"
dnl To pass linking, we must define both LDFLAGS and LIBS
test "x$x_libraries" != "x" && LDFLAGS="$LDFLAGS -L$x_libraries"
test "x$x_libraries" != "x" && LIBS="$LIBS -L$x_libraries"


AC_CHECK_LIB(
    X11,
    XOpenDisplay,
    [],
    AC_MSG_ERROR([Mrxvt requires the libX11 libraries.])
)

AC_CHECK_HEADERS(X11/XKBlib.h)

dnl AC_CHECK_LIB(
dnl     Xext,
dnl     XextAddDisplay,
dnl     [],
dnl     AC_MSG_ERROR([Mrxvt requires the libXext libraries.])
dnl )


if test "x$support_xim" = "xyes"; then
    AC_CHECK_LIB(
        X11,
        XRegisterIMInstantiateCallback,
        AC_DEFINE(USE_XIM, [], [set XIM support]),
        [support_xim=no]
    )
fi


if test "x$support_sessionmgr" = "xyes"; then
    AC_CHECK_LIB(
        SM,
        SmcOpenConnection,
        [],
        [support_sessionmgr=no]
    )
    AC_CHECK_LIB(
        ICE,
        IceConnectionNumber,
        [],
        [support_sessionmgr=no]
    )
fi
AM_CONDITIONAL(SESSION_SUPPORT, test "x$support_sessionmgr" = "xyes")


if test "x$support_utempter" = "xyes"; then
    AC_CHECK_LIB(
        utempter,
        utempter_add_record,
        [],
        [support_utempter=no]
    )
fi
if test "x$support_utempter" = "xyes"; then
    AC_DEFINE(UTEMPTER_SUPPORT, [], [set utempter library support])
fi


if test "x$support_xft" = "xyes"; then
    dnl Backup variables
    OLD_LIBS="$LIBS"
    OLD_CFLAGS="$CFLAGS"
    OLD_LDFLAGS="$LDFLAGS"

    dnl Get new variables.
    if which pkg-config >/dev/null 2>&1 && pkg-config --exists xft; then
        LIBS="$LIBS `pkg-config --libs xft`"
        CFLAGS="$CFLAGS `pkg-config --cflags xft`"
        LDFLAGS="$LDFLAGS `pkg-config --libs xft`"
    else
	if which xft-config >/dev/null 2>&1; then
	    dnl If xft-config is not found, try pkg-config instead
	    LIBS="$LIBS `xft-config --libs`"
	    CFLAGS="$CFLAGS `xft-config --cflags`"
	    LDFLAGS="$LDFLAGS `xft-config --libs`"
	else
	    support_xft="no"
	fi
    fi

    dnl Solaris has wired include directory for freetype.h
    if test "x$local_os_type" = "xsolaris"; then
        CFLAGS="$CFLAGS -I/usr/sfw/include -I/usr/sfw/include/freetype2"
    fi

    AC_CHECK_HEADERS(fontconfig/fontconfig.h X11/Xft/Xft.h, ,
        [support_xft=no],
        [[#include <X11/Xlib.h>
        #include <fontconfig/fontconfig.h>
        ]]
    )
    AC_CHECK_LIB(
        fontconfig,
        FcPatternAddInteger,
        [],
        [support_xft=no]
    )
    AC_CHECK_LIB(
        Xft,
        XftDrawString16,
        [],
        [support_xft=no]
    )

    if test "x$support_xft x$support_cjk" = "xyes xyes"; then
        dnl BSD and Cygwin needs libiconv
        AC_CHECK_LIB(iconv, iconv_open)
        dnl hack for Mac OS X
        if test "x$local_os_type" = "xdarwin"; then
            LIBS="$LIBS -liconv"
        fi
        dnl hack for Cygwin
        if test "x$local_os_type" = "xcygwin"; then
            LIBS="$LIBS -liconv"
        fi
        dnl hack for Cygwin
        if test "x$local_os_type" = "xopenbsd"; then
            LIBS="$LIBS -liconv"
        fi
    fi

    if test "x$support_xft" = "xyes"; then
        AC_DEFINE(XFT_SUPPORT, [], [set XFT support])
    else
        dnl Restore backup copy variables
        LIBS="$OLD_LIBS"
        CFLAGS="$OLD_CFLAGS"
        LDFLAGS="$OLD_LDFLAGS"
    fi
fi
AM_CONDITIONAL(XFT_SUPPORT, test "x$support_xft" = "xyes")

if test "x$support_cjk" = "xyes"; then
    AC_DEFINE(MULTICHAR_SET, [], [set CJK multichar set support])
    AC_DEFINE_UNQUOTED(MULTICHAR_ENCODING, "$encoding_option", [default encoding])
fi



AC_HEADER_STDC
AC_HEADER_SYS_WAIT

AC_CHECK_HEADERS([stdlib.h], [], [],
    [#include <stdio.h>
    ])

AC_CHECK_HEADERS([sys/socket.h], [], [],
    [#include <stdio.h>
    #ifdef STDC_HEADERS
    # include <stdlib.h>
    # include <stddef.h>
    #else
    # ifdef HAVE_STDLIB_H
    #  include <stdlib.h>
    # endif
    #endif
    ])

AC_CHECK_HEADERS( \
    arpa/inet.h \
    assert.h \
    fcntl.h \
    iconv.h \
    lastlog.h \
    limits.h \
    locale.h \
    langinfo.h \
    netinet/in.h \
    pty.h \
    sgtty.h \
    stdarg.h \
    stdlib.h \
    string.h \
    sys/byteorder.h \
    sys/ioctl.h \
    sys/select.h \
    sys/sockio.h \
    sys/stropts.h \
    sys/strredir.h \
    sys/stat.h \
    sys/time.h \
    sys/times.h \
    sys/types.h \
    sys/wait.h \
    time.h \
    termios.h \
    unistd.h \
    utmp.h \
    utmpx.h \
    wordexp.h \
    wchar.h \
)

if test "x$support_sessionmgr" = "xyes"; then
    AC_CHECK_HEADERS(X11/SM/SMlib.h)
fi

AC_CHECK_HEADERS([net/if.h], [], [],
    [#include <stdio.h>
    #ifdef STDC_HEADERS
    # include <stdlib.h>
    # include <stddef.h>
    #else
    # ifdef HAVE_STDLIB_H
    #  include <stdlib.h>
    # endif
    #endif
    #if HAVE_SYS_SOCKET_H
    # include <sys/socket.h>
    #endif
    ])

AC_CHECK_HEADERS([net/if_arp.h], [], [],
    [#include <stdio.h>
    #ifdef STDC_HEADERS
    # include <stdlib.h>
    # include <stddef.h>
    #else
    # ifdef HAVE_STDLIB_H
    #  include <stdlib.h>
    # endif
    #endif
    #if HAVE_SYS_SOCKET_H
    # include <sys/socket.h>
    #endif
    ])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME


AC_STRUCT_TM

AC_TYPE_MODE_T
AC_TYPE_UID_T
AC_TYPE_PID_T
AC_TYPE_SIGNAL


AC_CACHE_CHECK(for working nl_langinfo, rxvt_cv_func_nl_langinfo,
[AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <langinfo.h>]], [[nl_langinfo(CODESET);]])],[rxvt_cv_func_nl_langinfo=yes],[rxvt_cv_func_nl_langinfo=no])])
if test "x$rxvt_cv_func_nl_langinfo" = "xyes"; then
  AC_DEFINE(HAVE_NL_LANGINFO, 1, Define if nl_langinfo(CODESET) works)
fi

AC_CACHE_CHECK([for XPointer], rxvt_cv_xpointer,
[AC_TRY_COMPILE([#include <X11/Xlib.h>], [XPointer dummy;],
rxvt_cv_xpointer=yes, rxvt_cv_xpointer=no)])
if test "x$rxvt_cv_xpointer" = "xyes"; then
  AC_DEFINE(HAVE_XPOINTER, 1, Define if you have XPointer typedef)
fi


if test "x$support_utmp" = "xyes"; then
    AC_CACHE_CHECK([for struct utmp], rxvt_cv_struct_utmp,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmp.h>],
        [struct utmp ut;],
        rxvt_cv_struct_utmp=yes, rxvt_cv_struct_utmp=no)])
    if test "x$rxvt_cv_struct_utmp" = "xyes"; then
        AC_DEFINE(HAVE_STRUCT_UTMP, 1, Define if utmp.h has struct utmp)
    fi

    AC_CACHE_CHECK(for ut_host in utmp struct, rxvt_cv_struct_utmp_host,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmp.h>],
        [struct utmp ut; ut.ut_host;],
        rxvt_cv_struct_utmp_host=yes, rxvt_cv_struct_utmp_host=no)])
    if test "x$rxvt_cv_struct_utmp_host" = "xyes"; then
        AC_DEFINE(HAVE_UTMP_HOST, 1, Define if struct utmp contains ut_host)
    fi

    AC_CACHE_CHECK(for ut_pid in utmp struct, rxvt_cv_struct_utmp_pid,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmp.h>],
        [struct utmp ut; ut.ut_pid;],
        rxvt_cv_struct_utmp_pid=yes, rxvt_cv_struct_utmp_pid=no)])
    if test "x$rxvt_cv_struct_utmp_pid" = "xyes"; then
        AC_DEFINE(HAVE_UTMP_PID, 1, Define if struct utmp contains ut_pid)
    fi

    AC_CACHE_CHECK([for struct utmpx], rxvt_cv_struct_utmpx,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmpx.h>],
        [struct utmpx ut;],
        rxvt_cv_struct_utmpx=yes, rxvt_cv_struct_utmpx=no)])
    if test "x$rxvt_cv_struct_utmpx" = "xyes"; then
        AC_DEFINE(HAVE_STRUCT_UTMPX, 1, Define if utmpx.h has struct utmpx)
    fi

    AC_CACHE_CHECK(for ut_host in utmpx struct, rxvt_cv_struct_utmpx_host,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmpx.h>],
        [struct utmpx utx; utx.ut_host;],
        rxvt_cv_struct_utmpx_host=yes, rxvt_cv_struct_utmpx_host=no)])
    if test "x$rxvt_cv_struct_utmpx_host" = "xyes"; then
        AC_DEFINE(HAVE_UTMPX_HOST, 1, Define if struct utmpx contains ut_host)
    fi

    AC_CACHE_CHECK(for ut_session in utmpx struct, rxvt_cv_struct_utmpx_session,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmpx.h>],
        [struct utmpx utx; utx.ut_session;],
        rxvt_cv_struct_utmpx_session=yes, rxvt_cv_struct_utmpx_session=no)])
    if test "x$rxvt_cv_struct_utmpx_session" = "xyes"; then
        AC_DEFINE(HAVE_UTMPX_SESSION, 1, Define if struct utmpx contains ut_session)
    fi
fi


if test "x$support_utmp x$support_lastlog" = "xyes xyes"; then
    AC_CACHE_CHECK(for struct lastlog, rxvt_cv_struct_lastlog,
        [AC_TRY_COMPILE([#include <sys/types.h>
        #include <utmp.h>
        #ifdef HAVE_LASTLOG_H
        #include <lastlog.h>
        #endif
        ],
        [struct lastlog ll;],
        rxvt_cv_struct_lastlog=yes, rxvt_cv_struct_lastlog=no)])
    if test "x$rxvt_cv_struct_lastlog" = "xyes"; then
        AC_DEFINE(HAVE_STRUCT_LASTLOG, 1, Define if utmp.h or lastlog.h has struct lastlog)
    fi
fi




AC_CHECK_SIZEOF(char, 1)
AC_CHECK_SIZEOF(short, 2)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AC_CHECK_SIZEOF(long long, 8)
AC_CHECK_SIZEOF(int *, 4)


if test $ac_cv_sizeof_char -ge 2; then
  RINT16T=char
  AC_DEFINE(RINT16T,  char,[typedef])
  AC_DEFINE(RUINT16T, unsigned char,[typedef])
else
  if test $ac_cv_sizeof_short -ge 2; then
    RINT16T=short
    AC_DEFINE(RINT16T,  short,[typedef])
    AC_DEFINE(RUINT16T, unsigned short,[typedef])
  else
    if test $ac_cv_sizeof_int -ge 2; then
      RINT16T=int
      AC_DEFINE(RINT16T,  int,[typedef])
      AC_DEFINE(RUINT16T, unsigned int,[typedef])
    else    dnl What a fantastic system ?
      RINT16T=long
      AC_DEFINE(RINT16T,  long,[typedef])
      AC_DEFINE(RUINT16T, unsigned long,[typedef])
    fi
  fi
fi
if test $ac_cv_sizeof_char -ge 4; then
  RINT32T=char
  AC_DEFINE(RINT32T,  char,[typedef])
  AC_DEFINE(RUINT32T, unsigned char,[typedef])
else
  if test $ac_cv_sizeof_short -ge 4; then
    RINT32T=short
    AC_DEFINE(RINT32T,  short,[typedef])
    AC_DEFINE(RUINT32T, unsigned short,[typedef])
  else
    if test $ac_cv_sizeof_int -ge 4; then
      RINT32T=int
      AC_DEFINE(RINT32T,  int,[typedef])
      AC_DEFINE(RUINT32T, unsigned int,[typedef])
    else    dnl What a fantastic system ?
      RINT32T=long
      AC_DEFINE(RINT32T,  long,[typedef])
      AC_DEFINE(RUINT32T, unsigned long,[typedef])
    fi
  fi
fi
if test $ac_cv_sizeof_char -eq $ac_cv_sizeof_int_p; then
  RINTPT=char
  AC_DEFINE(RINTPT,  char,[typedef])
  AC_DEFINE(RUINTPT, unsigned char,[typedef])
else
  if test $ac_cv_sizeof_short -eq $ac_cv_sizeof_int_p; then
    RINTPT=short
    AC_DEFINE(RINTPT,  short,[typedef])
    AC_DEFINE(RUINTPT, unsigned short,[typedef])
  else
    if test $ac_cv_sizeof_int -eq $ac_cv_sizeof_int_p; then
      RINTPT=int
      AC_DEFINE(RINTPT,  int,[typedef])
      AC_DEFINE(RUINTPT, unsigned int,[typedef])
    else    dnl what next? long long? quad?
      RINTPT=long
      AC_DEFINE(RINTPT,  long,[typedef])
      AC_DEFINE(RUINTPT, unsigned long,[typedef])
    fi
  fi
fi



dnl Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_LSTAT
dnl AC_FUNC_MALLOC
AC_FUNC_SETPGRP
AC_FUNC_VPRINTF


dnl Now to check functions
AC_CHECK_FUNCS(
    getcwd \
    getpt \
    _getpty \
    getpwuid \
    grantpt \
    isastream \
    nanosleep \
    openpty \
    putenv \
    revoke \
    select \
    setlocale \
    seteuid \
    setpgrp \
    setpgid \
    setreuid \
    setsid \
    setutent \
    socket \
    strcasestr \
    strchr \
    strdup \
    strndup \
    strrchr \
    ttyslot \
    unlockpt \
    unsetenv \
    updwtmp \
    updwtmpx \
    xsetlocale \
)


dnl Now to set tty options
dnl echo "*** ac_cv_func_getpt = $ac_cv_func_getpt"
dnl Check PTY/TTY types
if test "x$ac_cv_func_openpty" = "xyes"; then
    rxvt_cv_ptys=OPENPTY
else if test "x$ac_cv_func__getpty" = "xyes"; then
    rxvt_cv_ptys=SGI4
else if test -c /dev/ttyp20; then
    rxvt_cv_ptys=SCO
else if test -c /dev/ptym/clone; then
    rxvt_cv_ptys=HPUX
else if test "x$ac_cv_func_getpt" = "xyes"; then
    rxvt_cv_ptys=GLIBC
else if test -c /dev/ptc -a -c /dev/pts; then
    rxvt_cv_ptys=PTC
else if test -c /dev/ptc -a -d /dev/pts; then
    rxvt_cv_ptys=PTC
else if test -c /dev/ptmx -a -c /dev/pts/0; then
    rxvt_cv_ptys=STREAMS
else if test "x$ac_cv_func_grantpt x$ac_cv_func_unlockpt" = "xyes xyes"; then
    rxvt_cv_ptys=STREAMS
else
    rxvt_cv_ptys=BSD
fi
fi
fi
fi
fi
fi
fi
fi
fi


dnl echo "*** rxvt_cv_ptys = $rxvt_cv_ptys"
if test "x$rxvt_cv_ptys" = "xOPENPTY"; then
  AC_DEFINE(PTYS_ARE_OPENPTY, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xSCO"; then
  AC_DEFINE(PTYS_ARE_NUMERIC, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xSTREAMS"; then
  AC_DEFINE(PTYS_ARE_PTMX, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xPTC"; then
  AC_DEFINE(PTYS_ARE_PTC, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xSGI4"; then
  AC_DEFINE(PTYS_ARE__GETPTY, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xCONVEX"; then
  AC_DEFINE(PTYS_ARE_GETPTY, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xGLIBC"; then
  AC_DEFINE(PTYS_ARE_GETPT, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xHPUX"; then
  AC_DEFINE(PTYS_ARE_CLONE, 1, Define for this pty type)
fi
if test "x$rxvt_cv_ptys" = "xBSD" -o "x$rxvt_cv_ptys" = "xHPUX" -o "x$rxvt_cv_ptys" = "xGLIBC"; then
  AC_DEFINE(PTYS_ARE_SEARCHED, 1, Define for this pty type)
fi

AC_CHECKING(for pty ranges)
ptys=`echo /dev/pty??`
pch1=`echo $ptys | tr ' ' '\012' | sed -e 's/^.*\(.\).$/\1/g' | sort -u | tr -d '\012'`
pch2=`echo $ptys | tr ' ' '\012' | sed -e 's/^.*\(.\)$/\1/g' | sort -u | tr -d '\012'`
if test x$pch1 != x; then
  AC_DEFINE_UNQUOTED(PTYCHAR1, "$pch1", Define for first char in devptyXX)
fi
if test x$pch2 != x; then
  AC_DEFINE_UNQUOTED(PTYCHAR2, "$pch2", Define for second char in devptyXX)
fi



dnl Now to generate Makefiles
AC_SUBST(CC)
AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile share/Makefile])
AC_OUTPUT


dnl Now to dump configuration options
dnl echo "CFLAGS: $CFLAGS"
dnl echo "LDFLAGS: $LDFLAGS"
dnl echo "LIBS: $LIBS"

echo
echo
echo "Configurations:"
echo
echo
echo "Maintenance:"
echo "Debug mode                  : $support_debug"
echo "Rarely used features        : $support_frills"
echo 
echo "Scroll features:"
echo "Scroll on selection         : $support_selectionscrolling"
echo "Mouse wheel scroll          : $support_mousewheel"
if test "x$support_mousewheel" = "xyes"; then
    echo "Slip mouse wheel scroll     : $support_mouseslipwheel"
fi
echo
echo "Logging features:"
echo "Support utmp records        : $support_utmp"
echo "Support wtmp records        : $support_wtmp"
echo "Support lastlog records     : $support_lastlog"
echo "Support X session manager   : $support_sessionmgr"
echo "Use utempter library        : $support_utempter"
echo
echo "Visual features:"
echo "Support line space          : $support_linespace"
echo "24 bits color mode          : $support_24bits"
echo "256 color mode              : $support_256colors"
echo "Blink cursor                : $support_cursor_blink"
echo "Blank pointer               : $support_pointer_blank"
echo "Smart resize                : $support_smart_resize"
dnl: echo "Off focus fading            : $support_fading"
echo
echo "Multi-language and font features:"
echo "Freetype Font               : $support_xft"
echo "X Input Method              : $support_xim"
echo "CJK multichar language      : $support_cjk"
if test "x$support_cjk" = "xyes"; then
    echo "Default encoding method     : $encoding_option"
fi
echo
echo "System features:"
echo "TTY/PTY type                : $rxvt_cv_ptys"
echo "TTY gid                     : $support_ttygid"
echo "Backspace key               : $support_backspace_key"
echo "Delete key                  : $support_delete_key"
echo "X resources                 : $support_resources"
echo "X default resources         : $support_xgetdefault"
echo "Swap screen                 : $support_swapscreen"
echo "Old selection               : $support_old_selection"
echo
echo "Options:"
echo "Terminal name               : $term_name_option"
echo "Maximal profiles            : $max_prof_option"
if test "x$support_linespace" = "xyes"; then
    echo "Line space                  : $line_space_option"
fi
if test "x$support_frills" = "xyes"; then
    echo "Internal border width       : $int_border_option"
    echo "External border width       : $ext_border_option"
fi
if test "x$support_scroll" = "xyes"; then
    echo "Saved lines in scroll       : $save_lines_option"
fi
echo
