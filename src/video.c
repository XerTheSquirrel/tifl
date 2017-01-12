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
	
	// For simplicity, use a surface with the given format.
	rendersurface = SDL_CreateRGBSurfaceWithFormat(0, BASIC_SCREEN_WIDTH,
		BASIC_SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB32);
	if (rendersurface == NULL)
		Die("Could not create the backrendered surface.");
	
	// Forego blending
	SDL_SetSurfaceBlendMode(rendersurface, SDL_BLENDMODE_NONE);
	
	// Ok
	return 0;
}

void VideoDraw(void)
{
	SDL_Rect destrect;
	uint32_t* pixels;
	int i, winw, winh;
	SDL_Surface* gamesurface;
	int bww, bwh, bhw, bhh;
	
	// If the window size changes, the surface is invalidated.
	gamesurface = SDL_GetWindowSurface(gamewindow);
	SDL_SetSurfaceBlendMode(gamesurface, SDL_BLENDMODE_NONE);
	
	// Lock
	SDL_LockSurface(rendersurface);
	pixels = (uint32_t*)rendersurface->pixels;
	
	// Base draw of pixel data
	for (int i = 0; i < 320 * 240; i++)
		pixels[i] = /*0xFF000000 |*/ (i * 2);
	
	// Need target window size for scaling
	SDL_GetWindowSize(gamewindow, &winw, &winh);
	
	// Scale it evenly, from 320x240 to the window size with bars.
	// Most displays now are 1:1 so just perform basic aspect correction
	destrect.x = 0;
	destrect.y = 0;
	double scalew = (double)winw / (double)BASIC_SCREEN_WIDTH,
		scaleh = (double)winh / (double)BASIC_SCREEN_HEIGHT;
	
	// Scale by width and height values
	bww = (int)(scalew * BASIC_SCREEN_WIDTH);
	bwh = (int)(scalew * BASIC_SCREEN_HEIGHT);
	bhw = (int)(scaleh * BASIC_SCREEN_WIDTH);
	bhh = (int)(scaleh * BASIC_SCREEN_HEIGHT);
	
	// Scale by height if the width exceeds the buffer size
	// Or the height exceeds the buffer size
	if (bww > winw || bwh > winh)
	{
		destrect.w = bhw;
		destrect.h = bhh;
	}
	
	// Otherwise by width
	else
	{
		destrect.w = bww;
		destrect.h = bwh;
	}
	
	// Center the display
	destrect.x = (winw >> 1) - (destrect.w >> 1);
	destrect.y = (winh >> 1) - (destrect.h >> 1);
	
	// Unlock
	SDL_UnlockSurface(rendersurface);
	
	// Blit it to the actual surface
	SDL_BlitScaled(rendersurface, NULL, gamesurface, &destrect);
	
	// Update drawing
	SDL_UpdateWindowSurface(gamewindow);
}

boolean NextEvent(Event* out)
{
	SDL_Event event;
	
	// Get next event
	SDL_memset(&event, 0, sizeof(event));
	if (SDL_PollEvent(&event) < 1)
		return false;
	
	// Clear event
	SDL_memset(out, 0, sizeof(*out));
	
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

