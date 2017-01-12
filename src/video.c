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

/** The game window. */
static SDL_Window* gamewindow;

/** The game surface. */
static SDL_Surface* gamesurface;

int VideoInit(void)
{
	// Initialize video
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		Die("Could not initialize SDL video.");
	
	// Setup game window
	gamewindow = SDL_CreateWindow("Trouble In Furry Land",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		BASIC_SCREEN_WIDTH, BASIC_SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI |
		SDL_WINDOW_MAXIMIZED);
	if (gamewindow == NULL)
		Die("Could not create the main window.");
	
	// Need the game surface
	gamesurface = SDL_GetWindowSurface(gamewindow);
	
	// Ok
	return 0;
}

