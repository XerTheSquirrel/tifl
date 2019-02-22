// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Stephanie Gawroriski <xer@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_GLOBAL_H
#define TIFL_GLOBAL_H

#include <stdint.h>

/** The number of frames per second in the game. */
#define GAME_FRAMES_PER_SECOND 20

/** Milliseconds per game frame. */
#define MILLISECONDS_PER_FRAME 50

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

