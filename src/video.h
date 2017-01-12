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

#define BASIC_SCREEN_WIDTH	320
#define BASIC_SCREEN_HEIGHT	240

/**
 * Initializes the video system.
 *
 * @return Zero on success, otherwise failure.
 * @since 2017/01/12
 */
int VideoInit(void);

#endif

