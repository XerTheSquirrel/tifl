// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_VIDEO_H
#define TIFL_VIDEO_H

#include "global.h"
#include "event.h"

#define BASIC_SCREEN_WIDTH	320
#define BASIC_SCREEN_HEIGHT	240

#define HALF_SCREEN_WIDTH	160
#define HALF_SCREEN_HEIGHT	120

/** The current game frame. */
extern int currentframe;

/**
 * Initializes the video system.
 *
 * @return Zero on success, otherwise failure.
 * @since 2017/01/12
 */
int VideoInit(void);

/**
 * Draws the game.
 *
 * @since 2017/01/12
 */
void VideoDraw(void);

/**
 * Pumps input events and such.
 *
 * @since 2017/01/12
 */
void PumpEvents();

#endif

