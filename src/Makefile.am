# All portions of code are copyright by their respective author/s.
#
#   Copyright (c) 2002   Alexis <materm@tuxfamily.org>
#   Copyright (c) 2004   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
#   Copyright (c) 2006	 Gautam Iyer <gi1242@users.sourceforge.net>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 675 Mass
# Ave, Cambridge, MA 02139, USA.

DEFS = -DPKG_CONF_DIR=\"$(sysconfdir)/$(PACKAGE_NAME)\"

bin_PROGRAMS = lxvt
lxvt_SOURCES =	    command.h		\
	       	    debug.h		\
	       	    encoding.h		\
	       	    feature.h		\
	       	    init.h		\
	       	    menubar.h		\
	       	    protos.h		\
	       	    rxvt.h		\
	       	    rxvtlib.h		\
		    rxvtmem.h


lxvt_SOURCES +=    command.c		\
		    debug.c		\
		    encoding.c		\
		    init.c		\
		    macros.c		\
		    main.c		\
		    menubar.c		\
		    misc.c		\
		    netdisp.c		\
		    ptytty.c		\
		    rxvt.c		\
		    rxvtmem.c		\
		    screen.c		\
		    scrollbar.c		\
		    strings.c		\
		    tabbar.c		\
		    xdefaults.c

lxvt_SOURCES +=    close_term.xbm	\
		    term.xbm

if PLAIN_SCROLLBAR
SCROLLBAR_PLAIN_SRC = scrollbar-plain.c
else
SCROLLBAR_PLAIN_SRC = 
endif

if XTERM_SCROLLBAR
SCROLLBAR_XTERM_SRC = scrollbar-xterm.c
else
SCROLLBAR_XTERM_SRC = 
endif

if NEXT_SCROLLBAR
SCROLLBAR_NEXT_SRC = scrollbar-next.c
else
SCROLLBAR_NEXT_SRC = 
endif

if RXVT_SCROLLBAR
SCROLLBAR_RXVT_SRC = scrollbar-rxvt.c
else
SCROLLBAR_RXVT_SRC = 
endif

if SGI_SCROLLBAR
SCROLLBAR_SGI_SRC = scrollbar-sgi.c
else
SCROLLBAR_SGI_SRC = 
endif

if UTMP_SUPPORT
UTMP_SRC = logging.c
else
UTMP_SRC =
endif

if SESSION_SUPPORT
SESSION_SRC = session.c
else
SESSION_SRC = 
endif

if XFT_SUPPORT
XFTACS_SRC = xftacs.c xftacs.h
XFTACS_H   = xftacs.h
else
XFTACS_SRC =
XFTACS_H   =
endif


lxvt_SOURCES +=    $(SCROLLBAR_PLAIN_SRC)	\
		    $(SCROLLBAR_XTERM_SRC)	\
		    $(SCROLLBAR_NEXT_SRC)	\
		    $(SCROLLBAR_RXVT_SRC)	\
		    $(SCROLLBAR_SGI_SRC)	\
		    $(UTMP_SRC)			\
		    $(SESSION_SRC)		\
		    $(XFTACS_SRC)

LXVT_H =	    ../config.h			\
		    command.h			\
		    debug.h			\
		    feature.h			\
		    menubar.h			\
		    protos.h			\
		    rxvt.h			\
		    rxvtlib.h

AUTOMAKE_OPTIONS = no-dependencies

# set dependencies
$(bin_PROGRAMS) : 
command.o:	    $(LXVT_H)				command.c
encoding.o:	    $(LXVT_H) encoding.h		encoding.c
macros.o:	    $(LXVT_H)				macros.c
init.o:		    $(LXVT_H) $(XFTACS_H) init.h	init.c
logging.o:	    $(LXVT_H)				logging.c
main.o:		    $(LXVT_H) $(XFTACS_H)		main.c
menubar.o:	    $(LXVT_H) 				menubar.c
misc.o:		    $(LXVT_H) 				misc.c
netdisp.o:	    $(LXVT_H) 				netdisp.c
ptytty.o:	    $(LXVT_H) 				ptytty.c
rxvt.o:		    $(LXVT_H) 				rxvt.c
rxvtmem.o:	    $(LXVT_H) 				rxvtmem.h rxvtmem.c
screen.o:	    $(LXVT_H) $(XFTACS_H)		screen.c
scrollbar-next.o:   $(LXVT_H) 				scrollbar-next.c
scrollbar-plain.o:  $(LXVT_H) 				scrollbar-plain.c
scrollbar-rxvt.o:   $(LXVT_H) 				scrollbar-rxvt.c
scrollbar-sgi.o:    $(LXVT_H) 				scrollbar-sgi.c
scrollbar-xterm.o:  $(LXVT_H) 				scrollbar-xterm.c
scrollbar.o:	    $(LXVT_H) 				scrollbar.c
session.o:	    $(LXVT_H) 				session.c
strings.o:	    $(LXVT_H) 				strings.c
#tabbar.o:	    $(LXVT_H) $(LXVT_PIXMAPS)		tabbar.c
tabbar.o:	    $(LXVT_H)       		tabbar.c
xdefaults.o:	    $(LXVT_H) 				xdefaults.c
xftacs.o:			xftacs.h		xftacs.c
