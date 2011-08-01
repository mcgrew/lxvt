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

/* INTPROTO */
void
rxvt_kill_page (rxvt_t* r)
{
    kill (PVTS(r)->cmd_pid, SIGHUP);
}


/*
 * Reduce r->num_fds so that select() is more efficient
 */
/* EXTPROTO */
void
rxvt_adjust_fd_number( rxvt_t* r )
{
    int	    num_fds = STDERR_FILENO;

	MAX_IT( num_fds, PVTS(r)->cmd_fd );
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "stderr_fd=%d, num_fds=%d. ", STDERR_FILENO, num_fds));

    MAX_IT( num_fds, r->Xfd );
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
	const char *command )
{
    int	    num_cmd_args = 0; /* Number of args we got from parsing command */
    char**  argv;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "rxvt_append_page( r, %d, %s )\n", profile, command ? command : "(nil)" ));

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

    if (!rxvt_create_termwin( r, 
		profile ))
    {
	rxvt_dbgmsg ((DBG_ERROR, DBG_TABBAR,
		    "\tThe initialization of the new tab failed.\n"));
	return;
    }

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

#ifdef HAVE_WORDEXP_H
	    wordexp_t p;
	    int wordexp_result = wordexp(cwdOption, &p, 0);
	    char *filename = NULL;

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

	if( len > 0 && chdir( child_cwd ) == 0 )
	{
	    /* Now in working directory of ATAB */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,
			"Running child in directory: %s\n", child_cwd ));

	    /* Run command in this new directory. */
	    PVTS(r)->cmd_fd =
		rxvt_run_command( r, (const char**) argv );

	    /* Restore old working directory. */
	    chdir( cwd );
	}

	else
	{
	    /* Exec command in original directory. */
	    rxvt_dbgmsg(( DBG_DEBUG, DBG_TABBAR,
			"Running child in original directory\n"));

	    PVTS(r)->cmd_fd =
		rxvt_run_command( r, (const char**) argv );
	}
    }
    else
	PVTS(r)->cmd_fd = rxvt_run_command (r, (const char**) argv);


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
    assert( -1 != PVTS(r)->cmd_fd );
    if (-1 == PVTS(r)->cmd_fd)
    {
	rxvt_dbgmsg ((DBG_WARN, DBG_TABBAR,
		    "\tThe command failed.\n"));
	rxvt_destroy_termwin (r);
	return;
    }
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,"cmd_fd is %d\n", PVTS(r)->cmd_fd));


    /* adjust number of file descriptors to listen */
    rxvt_adjust_fd_number (r);

    /*
     * Initialize the screen data structures
     */
    rxvt_scr_reset (r);
    rxvt_scr_refresh (r, FAST_REFRESH);

    /*
     * Now we actually execute the command after executing shell, but we need
     * careful check first.
     */
    if( command != NULL && *command == '!' )
    {
	command++;  /* Skip leading '!' */
	rxvt_tt_write( r, (const unsigned char*) command,
		STRLEN(command) );
	rxvt_tt_write( r, (const unsigned char*) "\n", 1 );
    }
}


/*
 * Called by the handler of SIGCHLD; destroy the terminal and its tab
 */
/* EXTPROTO */
void
rxvt_remove_page (rxvt_t* r)
{
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"rxvt_remove_page (r)\n"));

    /* clean utmp/wtmp entry */
#ifdef UTMP_SUPPORT
    rxvt_privileges (RESTORE);
    rxvt_cleanutent (r); 
    rxvt_privileges (IGNORE);
#endif

    /* free virtual terminal related resources */
    assert (PVTS(r)->ttydev);
    rxvt_free (PVTS(r)->ttydev);
    assert (PVTS(r)->cmd_fd >= 0);
    close (PVTS(r)->cmd_fd);

    if (PVTS(r)->inbuf_base)
    {
	rxvt_free (PVTS(r)->inbuf_base);
	PVTS(r)->inbuf_base = NULL;
    }

    /* free screen structure */
    rxvt_scr_release (r);

    /* destroy the virtual terminal window */
    rxvt_destroy_termwin (r);

	rxvt_clean_exit (r);

    /* update selection */
    if (r->selection.op)
	rxvt_process_selectionclear (r);
}


/*----------------------- end-of-file (C source) -----------------------*/
