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

bin_PROGRAMS = drxvt
drxvt_SOURCES =	    command.h		\
	       	    debug.h		\
	       	    encoding.h		\
	       	    feature.h		\
	       	    init.h		\
	       	    protos.h		\
	       	    rxvt.h		\
	       	    rxvtlib.h		\
		    rxvtmem.h


drxvt_SOURCES +=    command.c		\
		    debug.c		\
		    encoding.c		\
		    init.c		\
		    macros.c		\
		    main.c		\
		    misc.c		\
		    netdisp.c		\
		    ptytty.c		\
		    rxvt.c		\
		    rxvtmem.c		\
		    screen.c		\
		    strings.c		\
		    tabbar.c		\
		    transparent.c	\
		    xdefaults.c

#install-exec-hook:
#   -chown root $(DESTDIR)$(bindir)/drxvt
#   -chmod go-r $(DESTDIR)$(bindir)/drxvt
#   -chmod u+s $(DESTDIR)$(bindir)/drxvt

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


drxvt_SOURCES +=    $(UTMP_SRC)			\
		    $(SESSION_SRC)		\
		    $(XFTACS_SRC)

RXVT_H =	    ../config.h			\
		    command.h			\
		    debug.h			\
		    feature.h			\
		    protos.h			\
		    rxvt.h			\
		    rxvtlib.h

AUTOMAKE_OPTIONS = no-dependencies

# set dependencies
$(bin_PROGRAMS) : 
command.o:	    $(RXVT_H)				command.c
encoding.o:	    $(RXVT_H) encoding.h		encoding.c
grkelot.o:	    $(RXVT_H) grkelot.h		grkelot.c
macros.o:	    $(RXVT_H)				macros.c
init.o:		    $(RXVT_H) $(XFTACS_H) init.h	init.c
logging.o:	    $(RXVT_H)				logging.c
main.o:		    $(RXVT_H) $(XFTACS_H)		main.c
misc.o:		    $(RXVT_H) 				misc.c
netdisp.o:	    $(RXVT_H) 				netdisp.c
ptytty.o:	    $(RXVT_H) 				ptytty.c
rxvt.o:		    $(RXVT_H) 				rxvt.c
rxvtmem.o:	    $(RXVT_H) 				rxvtmem.h rxvtmem.c
screen.o:	    $(RXVT_H) $(XFTACS_H)		screen.c
session.o:	    $(RXVT_H) 				session.c
strings.o:	    $(RXVT_H) 				strings.c
tabbar.o:	    $(RXVT_H) 				tabbar.c
xdefaults.o:	    $(RXVT_H) 				xdefaults.c
transparent.o:	    $(RXVT_H) 				transparent.c
xftacs.o:			xftacs.h		xftacs.c
