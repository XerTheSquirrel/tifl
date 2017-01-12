// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_GLOBAL_H
#define TIFL_GLOBAL_H

#include <stdint.h>

/**
 * Boolean type.
 *
 * @since 2017/01/12
 */
typedef enum
{
	false,
	true
} boolean;

/**
 * Kills the program.
 *
 * @param format Formatted text.
 * @since 2017/01/12
 */
void Die(const char* format, ...);

#endif

