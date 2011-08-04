/*--------------------------------*-C-*---------------------------------*
 * File:	rxvtmem.c
 *----------------------------------------------------------------------*
 *
 * All portions of code are copyright by their respective author/s.
 * Copyright (c) 1997-2001   Geoff Wing <gcw@pobox.com>
 * Copyright (c) 2004        Jingmin Zhou <jimmyzhou@users.sourceforge.net>
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

#include "rxvtmem.h"


static const char*  abort_msg = APL_NAME ": memory allocation failure.  Aborting";

/* -------------------------------------------------------------------- *
 *                      MEMORY ALLOCATION WRAPPERS						*
 * -------------------------------------------------------------------- */

/* EXTPROTO */
void*
rxvt_malloc(size_t size)
{
    void*   p;

    /* see AC_FUNC_MALLOC macro in autoconf documentation */
    if (0 == size)
	size = 1;

    p = malloc(size);
    if (p)
	return p;

    rxvt_msg (DBG_FATAL, DBG_MEMORY, abort_msg);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}


/* EXTPROTO */
void*
rxvt_calloc(size_t number, size_t size)
{
    void*   p;

    p = calloc(number, size);
    if (p)
	return p;

    rxvt_msg (DBG_FATAL, DBG_MEMORY, abort_msg);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}


/* EXTPROTO */
void*
rxvt_realloc(void *ptr, size_t size)
{
    void*   p;

    p = realloc(ptr, size);
    if (p)
	return p;

    rxvt_msg (DBG_FATAL, DBG_MEMORY, abort_msg);
    exit(EXIT_FAILURE);
    /* NOT REACHED */
}


/* EXTPROTO */
void
rxvt_free(void* ptr)
{
#if 0
    assert (NOT_NULL(ptr)); /* generate core dump */
#endif
    free (ptr);
}

/*----------------------- end-of-file (C source) -----------------------*/
