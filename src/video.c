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
#include <SDL_stdinc.h>

#include "global.h"
#include "video.h"
#include "floor.h"
#include "entity.h"

/** The field of view angle. */
#define FIELD_OF_VIEW_ANGLE ANG60

/** The half field of view angle. */
#define HALF_FIELD_OF_VIEW ANG30

boolean gamekeydown[NUM_EVENTTYPE];

/** The distance to the projection plane. */
static fixedtype PROJECTION_PLANE_DISTANCE;

/** The angle between each projection ray. */
static fixedtype ANGLE_BETWEEN_RAYS;

/** The game window. */
static SDL_Window* gamewindow;

/** The actual render surface which is of a fixed format. */
static SDL_Surface* rendersurface;

int VideoInit(void)
{
	// Clear keys
	SDL_memset(gamekeydown, 0, sizeof(gamekeydown));	
	
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
		BASIC_SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
	if (rendersurface == NULL)
		Die("Could not create the backrendered surface.");
	
	// Forego blending
	SDL_SetSurfaceBlendMode(rendersurface, SDL_BLENDMODE_NONE);
	
	// Initialize some variables
	PROJECTION_PLANE_DISTANCE = FixedDiv(FIXED_C(HALF_SCREEN_WIDTH),
		AngleTan(FIELD_OF_VIEW_ANGLE >> 1));
	ANGLE_BETWEEN_RAYS = FIELD_OF_VIEW_ANGLE / BASIC_SCREEN_WIDTH;
	
	// Ok
	return 0;
}

void DrawLevel(uint32_t* pixels)
{
	int x, y, idist, q, baseq, endq, slicez;
	uint32_t* dp;
	uint32_t color, maskaway;
	fixedtype px, py;
	fixedtype raydistance;
	angletype traceangle, baseangle, diffangle;
	FloorTile* hittile;
	boolean horizhit;
	
	// Draw nothing if there is no player
	if (playerentity == NULL)
	{
		pixels[0] = 0xFF0000;
		return;
	}
	
	// Draw the background ceiling
	dp = pixels;
	maskaway = 0xFFFFFF;
	color = ceilingcolor;
	for (y = 0; y < HALF_SCREEN_HEIGHT; y++)
	{
		for (x = 0; x < BASIC_SCREEN_WIDTH; x++)
			*(dp++) = color;
		maskaway -= 0x020202;
		color = (ceilingcolor & maskaway);
	}
	
	// Draw the background floor
	maskaway = 0x0F0F0F;
	color = floorcolor & maskaway;
	for (y = HALF_SCREEN_HEIGHT; y < BASIC_SCREEN_HEIGHT; y++)
	{
		for (x = 0; x < BASIC_SCREEN_WIDTH; x++)
			*(dp++) = color;
		maskaway += 0x020202;
		color = (floorcolor & maskaway);
	}
	
	// Project any walls in view
	baseangle = playerentity->angle - HALF_FIELD_OF_VIEW;
	traceangle = baseangle;
	diffangle = 0;
	px = playerentity->x;
	py = playerentity->y;
	for (int i = 0; i < BASIC_SCREEN_WIDTH; i++,
		traceangle += ANGLE_BETWEEN_RAYS, diffangle += ANGLE_BETWEEN_RAYS)
	{
		// Trace ray
		TraceTile(px, py, traceangle, &hittile, &raydistance, &horizhit);
		
		// Not hit
		if (hittile == NULL)
			continue;
		
		// Determine color
		switch (hittile->type)
		{
			case FLOORTYPE_RED:
				color = 0xFF0000;
				break;
				
			case FLOORTYPE_GREEN:
				color = 0x00FF00;
				break;
				
			case FLOORTYPE_YELLOW:
				color = 0xFFFF00;
				break;
				
			case FLOORTYPE_BLUE:
				color = 0x0000FF;
				break;
				
			default:
				color = 0;
				break;
		}
		
		// If horizontal line was hit, cut color
		if (horizhit)
			color &= 0xF0F0F0;
		
		// Correct the ray angle due to the distortion
		//raydistance = FixedMul(raydistance, AngleCos(traceangle - baseangle));
		//raydistance = FixedMul(raydistance, AngleCos(traceangle + baseangle + HALF_FIELD_OF_VIEW));
		
		// Determine slice size
		slicez = FixedMul(FixedDiv((64 << FIXEDSHIFT), raydistance),
			PROJECTION_PLANE_DISTANCE) >> (FIXEDSHIFT);
		
		// Where to start drawing this slice?
		baseq = (BASIC_SCREEN_HEIGHT >> 1) - (slicez >> 1);
		endq = baseq + slicez;
		
		// Make sure they are in order
		if (baseq > endq)
		{
			q = endq;
			endq = baseq;
			endq = q;
		}
		
		// Never exceed vertical bounds
		if (baseq < 0)
			baseq = 0;
		if (endq > BASIC_SCREEN_HEIGHT)
			endq = BASIC_SCREEN_HEIGHT;
		
		// Draw slice
		for (q = baseq; q < endq; q++)
			pixels[(q * BASIC_SCREEN_WIDTH) + i] = color;
	}
}

void VideoDraw(void)
{
	SDL_Rect destrect;
	uint32_t* pixels;
	int i, winw, winh, x;
	SDL_Surface* gamesurface;
	int bww, bwh, bhw, bhh;
	
	static int q;
	
	// If the window size changes, the surface is invalidated.
	gamesurface = SDL_GetWindowSurface(gamewindow);
	SDL_SetSurfaceBlendMode(gamesurface, SDL_BLENDMODE_NONE);
	
	// Lock
	SDL_LockSurface(rendersurface);
	pixels = (uint32_t*)rendersurface->pixels;
	
	// Draw the level
	DrawLevel(pixels);
	
	// Need target window size for scaling
	SDL_GetWindowSize(gamewindow, &winw, &winh);
	
	// Scale it evenly, from 320x240 to the window size with bars.
	// Most displays now are 1:1 so just perform basic aspect correction
	destrect.x = 0;
	destrect.y = 0;
	int scalew = winw / BASIC_SCREEN_WIDTH,
		scaleh = winh / BASIC_SCREEN_HEIGHT;
	
	// Force positive scale
	if (scalew <= 0)
		scalew = 1;
	if (scaleh <= 0)
		scaleh = 1;
	
	// Scale by width and height values
	bww = (scalew * BASIC_SCREEN_WIDTH);
	bwh = (scalew * BASIC_SCREEN_HEIGHT);
	bhw = (scaleh * BASIC_SCREEN_WIDTH);
	bhh = (scaleh * BASIC_SCREEN_HEIGHT);
	
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

void PumpEvents()
{
	static int activekeyforcommand[NUM_EVENTTYPE];
	
	SDL_Event event;
	EventType eventtype;
	int sym;
	int* activekey;
	boolean* downstate;
	
	// Get next event
	while (SDL_PollEvent(&event) >= 1)
	{
		// Depends on the event type
		switch (event.type)
		{
				// Quit
			case SDL_QUIT:
				gamekeydown[EVENTTYPE_QUIT] = true;
				break;
				
				// Key up/down
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				// Depending on the key, choose an action
				eventtype = EVENTTYPE_NONE;
				switch ((sym = event.key.keysym.sym))
				{
						// Turn left
					case SDLK_LEFT:
					case SDLK_q:
						eventtype = EVENTTYPE_TURN_LEFT;
						break;
						
						// Turn right
					case SDLK_RIGHT:
					case SDLK_e:
						eventtype = EVENTTYPE_TURN_RIGHT;
						break;
						
						// Walk forward
					case SDLK_UP:
					case SDLK_w:
						eventtype = EVENTTYPE_WALK_FORWARD;
						break;
						
						// Walk backward
					case SDLK_DOWN:
					case SDLK_s:
						eventtype = EVENTTYPE_WALK_BACKWARD;
						break;
						
						// Strafe mode?
					case SDLK_LALT:
					case SDLK_RALT:
						eventtype = EVENTTYPE_STRAFE_MODE;
						break;
						
						// Strafe left
					case SDLK_a:
					case SDLK_TAB:
					case SDLK_COMMA:
						eventtype = EVENTTYPE_STRAFE_LEFT;
						break;
						
						// Strafe right
					case SDLK_d:
					case SDLK_BACKSPACE:
					case SDLK_PERIOD:
						eventtype = EVENTTYPE_STRAFE_RIGHT;
						break;
						
						// Unknown
					default:
						break;
				}
				
				// No event to trigger
				if (eventtype == EVENTTYPE_NONE)
					continue;
				
				activekey = &activekeyforcommand[eventtype];
				downstate = &gamekeydown[eventtype];
				
				// Only release a key if it is the one that was pressed down
				if (event.type == SDL_KEYUP)
				{
					if (*downstate == true && *activekey == sym)
					{
						*activekey = 0;
						*downstate = false;
					}
				}
				
				// Otherwise press the key if it is not active
				else
				{
					if (*downstate == false && *activekey == 0)
					{
						*activekey = sym;
						*downstate = true;
					}
				}
				break;
		
				// Unknown, ignore
			default:
				break;
		}
	}
}

