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

/** The actual render surface which is of a fixed format. */
static SDL_Surface* rendersurface;

int VideoInit(void)
{
	// Initialize video and event handling parts
	if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK |
		SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) != 0)
		Die("Could not initialize SDL video.");
	
	// Setup game window
	gamewindow = SDL_CreateWindow("Trouble In Furry Land",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		BASIC_SCREEN_WIDTH, BASIC_SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if (gamewindow == NULL)
		Die("Could not create the main window.");
	
	// Need the game surface
	gamesurface = SDL_GetWindowSurface(gamewindow);
	
	// For simplicity, use a surface with the given format.
	rendersurface = SDL_CreateRGBSurfaceWithFormat(0, BASIC_SCREEN_WIDTH,
		BASIC_SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB32);
	if (rendersurface == NULL)
		Die("Could not create the backrendered surface.");
	
	// Ok
	return 0;
}

void VideoDraw(void)
{
	SDL_Rect dest;
	uint32_t* pixels;
	int i;
	
	// Lock
	SDL_LockSurface(rendersurface);
	pixels = (uint32_t*)rendersurface->pixels;
	
	// Base draw of pixel data
	for (int i = 0; i < 320 * 240; i++)
		pixels[i] = 0xFF000000 | (i * 2);
	
	// Need target window size for scaling
	dest.x = 0;
	dest.y = 0;
	SDL_GetWindowSize(gamewindow, &dest.w, &dest.h);
	
	// Lock the target surface
	SDL_LockSurface(gamesurface);
	
	// Blit it to the actual surface
	SDL_BlitScaled(rendersurface, NULL, gamesurface, &dest);
	
	// Unlock
	SDL_UnlockSurface(rendersurface);
	SDL_UnlockSurface(gamesurface);
	
	// Update drawing
	SDL_UpdateWindowSurface(gamewindow);
}

boolean NextEvent(Event* out)
{
	SDL_Event event;
	
	// Get next event
	if (SDL_PollEvent(&event) < 1)
		return false;
	
	// Depends on the event type
	switch (event.type)
	{
			// Quit
		case SDL_QUIT:
			out->type = EVENTTYPE_QUIT;
			break;
		
			// Unknown, ignore
		default:
			return false;
	}
	
	// Event made
	return true;
}

