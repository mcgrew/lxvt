/*--------------------------------*-C-*---------------------------------*
 * File:    tabbar.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 2002        Alexis <materm@tuxfamily.org>
 * Copyright (c) 2004        Terry Griffin <griffint@pobox.com>
 * Copyright (c) 2004        Sergey Popov <p_sergey@jungo.com>
 * Copyright (c) 2004-2006   Jingmin Zhou <jimmyzhou@users.sourceforge.net>
 * Copyright (c) 2005        Mark Olesen <Mark.Olesen@gmx.net>
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
#ifdef HAVE_WORDEXP_H
# include <wordexp.h>
#endif


/******************************************************************************
*			Begin internal routine prototypes.		      *
******************************************************************************/
/******************************************************************************
*			End internal routine prototypes.		      *
******************************************************************************/

extern char **cmd_argv;


#ifdef HAVE_TABS

/*
 * Initialize global data structure of all tabs
 */
/* INTPROTO */
void
rxvt_tabbar_init (rxvt_t* r)
{
    LTAB(r) = -1;   /* the last tab */
    r->tabBar.atab = 0;	/* the active tab */
    FVTAB(r) = 0;   /* first visiable tab */
    LVTAB(r) = 0;   /* last visiable tab */
    r->tabBar.ptab = 0;	    /* previous active tab */
}
#endif


/* INTPROTO */
void
rxvt_kill_page (rxvt_t* r, short page)
{
    kill (PVTS(r, page)->cmd_pid, SIGHUP);
}


/*
 * Reduce r->num_fds so that select() is more efficient
 */
/* EXTPROTO */
void
rxvt_adjust_fd_number( rxvt_t* r )
{
    int	    num_fds = STDERR_FILENO;
    int	    i;

    for( i=0; i <= LTAB(r); i++ )
	MAX_IT( num_fds, PVTS(r, i)->cmd_fd );
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "LTAB=%d, stderr_fd=%d, num_fds=%d. ", LTAB(r), STDERR_FILENO, num_fds));

    MAX_IT( num_fds, r->Xfd );
#ifdef USE_FIFO
    MAX_IT( num_fds, r->fifo_fd );
#endif/*USE_FIFO*/
#ifdef HAVE_X11_SM_SMLIB_H
    MAX_IT( num_fds, r->TermWin.ice_fd );
#endif

#if 0
    MAX_IT( num_fds, r->num_fds-1 );
#endif
#ifdef OS_IRIX
    /* Alex Coventry says we need 4 & 7 too */
    MAX_IT( num_fds, 7 );
#endif
    r->num_fds = num_fds + 1;	/* counts from 0 */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "Adjust num_fds to %d\n", r->num_fds));
}


/*
 * Append a new tab after the last tab. If command is not NULL, run that
 * command in the tab. If command begins with '!', then run the shell first.
 */
/* EXTPROTO */
void
rxvt_append_page( rxvt_t* r, int profile,
	const char TAINTED *title, const char *command )
{
    int	    num_cmd_args = 0; /* Number of args we got from parsing command */
    char**  argv;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "rxvt_append_page( r, %d, %s, %s )\n", profile, title ? title : "(nil)", command ? command : "(nil)" ));

    if( profile < 0 || profile >= MAX_PROFILES )
    {
	rxvt_msg (DBG_WARN, DBG_TABBAR,  "Warning: Profile '%d' out of range; use profile '0' instead.", profile );
	profile = 0;
    }

    //LTAB(r)++;

    /*
     * Use command specified with -e only if we're opening the first tab, or the
     * --cmdAllTabs option is specified, and we're not given a command to
     *  execute (e.g. via the NewTab cmd macro).
     */
    if(
	    cmd_argv	    /* Argument specified via -e option */
	    && command == NULL /* No command specified (e.g. via NewTab macro) */
#ifdef HAVE_TABS
	    && (
		//LTAB(r) == 0			    /* First tab */
		LTAB (r) == - 1
		|| ISSET_OPTION(r, Opt2_cmdAllTabs)  /* -at option */
	       )
#endif
      )
	argv = cmd_argv;
    else
    {
	/* load tab command if necessary*/
	if( command == NULL )
	    command = getProfileOption( r, profile, Rs_command );

	if( command != NULL && *command != '!' )
	{
	    const char *cmd = command;

	    /* If "command" starts with '!', we should run it in the shell. */
	    if( cmd[0] == '\\' && cmd[1] == '!' )
		cmd++;

	    argv = rxvt_string_to_argv( cmd, &num_cmd_args );
	}
	else
	    argv = NULL;
    }
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "\tForking command=%s, argv[0]=%s\n", command ? command : "(nil)", ( argv && argv[0] ) ? argv[0] : "(nil)" ));

    /*
     * Set the tab title.
     */
    if( title == NULL || *title == '\0' )
    {
	title = getProfileOption( r, profile, Rs_tabtitle );
	if( title == NULL || *title == '\0' )
	{
	    if( command && *command != '\0' )
		title = command;
	    else if( argv && argv[0] && *argv[0] != '\0' )
		title = argv[0];
	}
    }
    if (!rxvt_create_termwin( r, 
#ifdef HAVE_TABS
		LTAB(r) + 1,
#else
		0,
#endif
		profile, title ))
    {
	rxvt_dbgmsg ((DBG_ERROR, DBG_TABBAR,
		    "\tThe initialization of the new tab failed.\n"));
	return;
    }

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "Last page is %d.\n", LTAB(r)));

    /*
     * Run the child process.
     *
     * 2006-02-17 gi1242: Bug -- If the child produces some output and exits
     * quickly, then some of that output is sometimes lost.
     */
    if( getProfileOption( r, profile, Rs_cwd ) != NULL )
    {
	const char  *cwdOption	= getProfileOption( r, profile, Rs_cwd );
	char	    cwd[PATH_MAX] = "",
		    child_cwd[PATH_MAX] = "";
	int	    len = 0;


	getcwd (cwd, PATH_MAX);

	if( !STRCMP( cwdOption, "." ) )
	{
	    if( ATAB(r) != LTAB(r) )
	    {
		/*
		 * Copy working directory of the current tab into child_cwd.
		 */
		char	proc_cwd[32];		/* 16 is enough */

		sprintf( proc_cwd, "/proc/%d/cwd", AVTS(r)->cmd_pid );
		if( (len = readlink( proc_cwd, child_cwd, PATH_MAX-1) ) > 0 )
		    /* readlink does not null terminate */
		    child_cwd[len] = 0;
	    }
	}

	else
	{
#ifdef HAVE_WORDEXP_H
	    wordexp_t p;
	    int wordexp_result = wordexp(cwdOption, &p, 0);
	    char *filename;

	    if( wordexp_result == 0 )
	    {
		if( p.we_wordc > 1)
		    rxvt_msg( DBG_ERROR, DBG_TABBAR,
			    "Too many words when expanding %s\n", cwdOption );
		else if( NOT_NULL( *p.we_wordv ) )
		{
		    filename = *p.we_wordv;

		    len  = STRLEN( filename );
		    MIN_IT( len, PATH_MAX - 1 );
		    STRNCPY( child_cwd, filename, len );
		    child_cwd[len] = 0;
		}

		wordfree( &p );
	    }

	    else
	    {
		rxvt_dbgmsg(( DBG_VERBOSE, DBG_TABBAR,
			    "wordexp error code '%i', expanding '%s'\n",
			    wordexp_result, filename ));
	    }
#endif /* HAVE_WORDEXP_H */
	}

	if( len > 0 && chdir( child_cwd ) == 0 )
	{
	    /* Now in working directory of ATAB */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,
			"Running child in directory: %s\n", child_cwd ));

	    /* Run command in this new directory. */
	    LVTS(r)->cmd_fd =
		rxvt_run_command( r, LTAB(r), (const char**) argv );

	    /* Restore old working directory. */
	    chdir( cwd );
	}

	else
	{
	    /* Exec command in original directory. */
	    rxvt_dbgmsg(( DBG_DEBUG, DBG_TABBAR,
			"Running child in original directory\n"));

	    LVTS(r)->cmd_fd =
		rxvt_run_command( r, LTAB(r), (const char**) argv );
	}
    }
    else
	LVTS(r)->cmd_fd = rxvt_run_command (r, LTAB(r), (const char**) argv);


    /*
     * In case we allocated memory for argv using rxvt_string_to_argv (because a
     * command was specified), then free it.
     */
    if( num_cmd_args > 0)
    {
	char **s;

	for( s = argv; *s != NULL; s++) rxvt_free(*s);
	rxvt_free( argv );
    }

    /*
     * If run command failed, rollback
     */
    assert( -1 != LVTS(r)->cmd_fd );
    if (-1 == LVTS(r)->cmd_fd)
    {
	rxvt_dbgmsg ((DBG_WARN, DBG_TABBAR,
		    "\tThe command failed.\n"));
	rxvt_destroy_termwin (r, LTAB(r));
	return;
    }
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,"page %d's cmd_fd is %d\n", LTAB(r), LVTS(r)->cmd_fd));


    /* adjust number of file descriptors to listen */
    rxvt_adjust_fd_number (r);

    /*
     * Initialize the screen data structures
     */
    rxvt_scr_reset (r, LTAB(r));
    rxvt_scr_refresh (r, LTAB(r), FAST_REFRESH);

    /*
     * Now we actually execute the command after executing shell, but we need
     * careful check first.
     */
    if( command != NULL && *command == '!' )
    {
	command++;  /* Skip leading '!' */
	rxvt_tt_write( r, LTAB(r), (const unsigned char*) command,
		STRLEN(command) );
	rxvt_tt_write( r, LTAB(r), (const unsigned char*) "\n", 1 );
    }

#ifdef HAVE_TABS
    /*
     * Now update active page information
     */
    PTAB(r) = ATAB(r); /* set last previous tab */
    ATAB(r) = LTAB(r); /* set the active tab */
#endif

    /* update mapped flag */
    AVTS(r)->mapped = 1;

    /* first tab is special since ptab = atab now */
    if (PTAB(r) != ATAB(r))
	PVTS(r, PTAB(r))->mapped = 0;

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name (r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}


/*
 * Called by the handler of SIGCHLD; destroy the terminal and its tab
 */
/* EXTPROTO */
void
rxvt_remove_page (rxvt_t* r, short page)
{
    register int    i;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"rxvt_remove_page (r, %d)\n", page));

    /* clean utmp/wtmp entry */
#ifdef UTMP_SUPPORT
    rxvt_privileges (RESTORE);
    rxvt_cleanutent (r, page); 
    rxvt_privileges (IGNORE);
#endif

    /* free virtual terminal related resources */
    assert (PVTS(r, page)->ttydev);
    rxvt_free (PVTS(r, page)->ttydev);
    assert (PVTS(r, page)->cmd_fd >= 0);
    close (PVTS(r, page)->cmd_fd);

    if (PVTS(r, page)->inbuf_base)
    {
	rxvt_free (PVTS(r, page)->inbuf_base);
	PVTS(r, page)->inbuf_base = NULL;
    }

    /* free screen structure */
    rxvt_scr_release (r, page);

    /* destroy the virtual terminal window */
    rxvt_destroy_termwin (r, page);

#ifdef HAVE_TABS
    if (LTAB (r) == 0)
    {
	/* quit the last the terminal, exit the application */
	rxvt_free (r->vts);
	SET_NULL (r->vts);
#endif
	rxvt_clean_exit (r);
#ifdef HAVE_TABS
    }

    /* update total number of tabs */
    LTAB(r)--;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "\tThe last tab is %d.", LTAB(r)));

    if (FVTAB(r) > page)
	FVTAB(r)--;
    if (LVTAB(r) > page)
    {
	LVTAB(r)--;
    }
    /* Reorganize the tabs array. */
    /* update TermWin and tab_widths */
    for (i = page; i <= LTAB(r); i++)
    {
	PVTS(r, i) = PVTS(r, i+1);
	PVTS(r, i)->vts_idx = i;
    }

    {
	term_t** temp_vts = rxvt_realloc (r->vts, (LTAB (r) + 1) * sizeof (term_t*));
	if (temp_vts)
	{
	    r->vts = temp_vts;
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,
			"\tThe tab array has been reallocated to (%d * sizeof (term_t*)).\n", LTAB(r) + 1));
	}
	else
	    rxvt_dbgmsg ((DBG_WARN, DBG_TABBAR,
			"\tAfter removing a tab, the tab array could not be reallocated. If you see often this message, this can be a problem.\n"));
	// if the realloc failed, this is not fatale as we can imagine it may be reallocated
	// at the next change on tabs but display all the same a warning.
    }

    /* update selection */
    if (page == r->selection.vt)
	rxvt_process_selectionclear (r, page);
    else if (r->selection.vt > page)
	r->selection.vt --;

    /*
     * Now we try to set correct atab, ptab, fvtab, and lvtab
     * Must be careful here!!!
     */
    /* update previous active tab */
    if (PTAB(r) > page)
	PTAB(r)--;
    /* in case PTAB is invalid */
    if (PTAB(r) > LTAB(r))
	PTAB(r) = LTAB(r);

    /* update active tab */
    if( ATAB(r) == page )
    {
	/* Fall back to previous active */
	ATAB(r) = PTAB(r);

	/* Make the previous active tab the previous / next tab if possible. */
	if( PTAB(r) > 0 )
	    PTAB(r)--;
	else if (PTAB(r) < LTAB(r) )
	    PTAB(r)++;
    }
    else if( ATAB(r) > page)
	ATAB(r)--;

    /* always set mapped flag */
    AVTS(r)->mapped = 1;

    /* Adjust the number of FD's we select() for.  */
    rxvt_adjust_fd_number(r);

    /* Switch fg/bg colors */
    rxvt_set_vt_colors( r, ATAB(r) );
    XMapRaised( r->Xdisplay, AVTS(r)->vt );

    /*
     * We don't need to touch the screen here. XMapRaised will generate a
     * MapNotify and Expose events, which will refresh the screen as needed.
     * Touching the screen unnecessarily causes a flicker (and is *horrible*
     * under slow connections).
     */
    /* rxvt_scr_touch (r, ATAB(r), True); */

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name(r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
#endif
}


/*
 * Set new title for a tab
 */
/* EXTPROTO */
void
rxvt_tabbar_set_title (rxvt_t* r, short page, const unsigned char TAINTED * str)
{
    char UNTAINTED *	    n_title;

    assert (str);
    assert (page >= 0 && page <= LTAB(r));
    assert (PVTS(r, page)->tab_title);

    n_title = STRNDUP (str, MAX_TAB_TXT);
    /*
     * If strdup succeeds, set new title
     */
    if (NULL != n_title)
    {
	rxvt_free (PVTS(r, page)->tab_title);
	PVTS(r, page)->tab_title = n_title;

    }

    /* synchronize terminal title with active tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle) &&
	(page == ATAB(r)))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon) &&
	(page == ATAB(r)))
	rxvt_set_icon_name(r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}


/*
 * Activate a page terminal
 */
/* EXTPROTO */
void
rxvt_activate_page (rxvt_t* r, short index)
{
#ifdef HAVE_TABS
    /* shortcut */
    if (/* !r->tabBar.state ||
	NOT_WIN(r->tabBar.win) || */
	index == ATAB(r))
	return;

    AVTS(r)->mapped = 0;
    r->tabBar.ptab = ATAB(r);
    ATAB(r) = index;
#endif
    AVTS(r)->mapped = 1;
    AVTS(r)->highlight = 0; /* clear highlight flag */
    
    /* Switch VT fg/bg colors */
    rxvt_set_vt_colors( r, ATAB(r) );
    XMapRaised( r->Xdisplay, AVTS(r)->vt );
    /*
     * We don't need to touch the screen here. XMapRaised will generate a
     * MapNotify and Expose events, which will refresh the screen as needed.
     * Touching the screen unnecessarily causes a flicker (and is *horrible*
     * under slow connections).
     */
    /* rxvt_scr_touch (r, ATAB(r), True); */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "active page is %d\n",ATAB(r)));

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name (r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}


/*
 * Synchronize the window title to the title of the tab "page".
 */
void
sync_tab_title( rxvt_t *r, int page )
{
    char wintitle[MAX_TAB_TXT];

    if(
	 IS_NULL( PVTS(r,page)->winTitleFormat )	||
	 rxvt_percent_interpolate( r, page,
	     PVTS(r,page)->winTitleFormat, STRLEN(PVTS(r,page)->winTitleFormat),
	     wintitle, MAX_TAB_TXT ) <= 1
      )
    {
	/* % interpolation failed / not possible */
	rxvt_set_term_title( r, (unsigned char*) PVTS(r, page)->tab_title );
    }
    else
	rxvt_set_term_title( r, (unsigned char*) wintitle );
}

/*----------------------- end-of-file (C source) -----------------------*/
