// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#include <SDL.h>

#include "global.h"
#include "video.h"

int VideoInit(void)
{
	// Initialize video
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		Die("Could not initialize SDL video.");
	
	// Ok
	return 0;
}

