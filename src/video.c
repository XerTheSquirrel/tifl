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
#include "entity.h"
#include "level.h"

boolean gamekeydown[NUM_EVENTTYPE];

/** The game window. */
static SDL_Window* gamewindow;

/** The actual render surface which is of a fixed format. */
static SDL_Surface* rendersurface;

int currentframe;

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
	
	// Ok
	return 0;
}

void DrawSolid(uint32_t* pixels, int color, int x, int y, int w, int h)
{
	uint32_t* pp;
	int ey, i;
	
	// Off the left side?
	if (x < 0)
	{
		w = (w + x);
		x = 0;
	}
	
	// Off the right side?
	else if (x + w >= BASIC_SCREEN_WIDTH)
		w = BASIC_SCREEN_WIDTH - x;
	
	// Nothing to draw?
	if (w <= 0)
		return;
	
	// Draw pixels
	for (ey = y + h; y < ey; y++)
	{
		pp = &pixels[(y * BASIC_SCREEN_WIDTH) + x];
		
		for (i = 0; i < w; i++)
			*(pp++) = color;
	}
}

void DrawImageTile(uint32_t* pixels, uint32_t* src, int x, int y)
{
	uint32_t* pp;
	uint32_t* ss;
	int w, h, i, ey, b;
	
	// Bounds
	w = TILE_SIZE;
	h = TILE_SIZE;
	
	// Debug
	DrawSolid(pixels, 0xFF0000, x, y, TILE_SIZE, TILE_SIZE);
	
	// Off the left side?
	if (x < 0)
	{
		w = (w + x);
		x = 0;
	}
	
	// Off the right side?
	else if (x + w >= BASIC_SCREEN_WIDTH)
		w = BASIC_SCREEN_WIDTH - x;
	
	// Nothing to draw?
	if (w <= 0)
		return;
	
	// Draw pixels
	for (ey = y + h, b = 0; y < ey; y++, b++)
	{
		pp = &pixels[(y * BASIC_SCREEN_WIDTH) + x];
		ss = &src[b * TILE_SIZE];
		
		for (i = 0; i < w; i++)
			*(pp++) = *(ss++);
	}
}

void DrawLevel(uint32_t* pixels)
{
	int32_t vx, ve, xi, x, y, bx, by, i, color;
	LevelTile* tile;
	Entity* entity;
	const TileInfo* tinfo;
	
	// Draw nothing if there is no player
	if (playerentity == NULL)
	{
		pixels[0] = 0xFF0000;
		return;
	}
	
	// Determine the scroll offset of the game, based on the player position
	// Add half the tile size so the screen centers in the middle of the sprite
	vx = playerentity->x + (TILE_SIZE >> 1);
	
	// Move the entire screen left so it centers on the player
	vx -= HALF_VIEW_WIDTH_PIXELS;
	
	// Never exceed the left side of the level, nothing is there
	if (vx < 0)
		vx = 0;
	
	// Never exceed the right side of the level either
	ve = vx + VIEW_WIDTH_PIXELS;
	if (ve >= LEVEL_WIDTH_PIXELS)
	{
		vx = LEVEL_WIDTH_PIXELS - VIEW_WIDTH_PIXELS;
		ve = LEVEL_WIDTH_PIXELS;
	}
	
	// Draw the level
	for (xi = vx; xi <= ve; xi += TILE_SIZE)
	{
		// Actual X tile to draw
		x = xi / TILE_SIZE;
		
		// Brush here
		bx = (xi - vx) - (vx % TILE_SIZE);
		by = BASIC_SCREEN_HEIGHT - TILE_SIZE;
		
		// Draw column
		for (y = 0; y < LEVEL_HEIGHT; y++, by -= TILE_SIZE)
		{
			tile = &leveldata[(y * LEVEL_WIDTH) + x];
			tinfo = &tileinfo[tile->type];
			
			// Lighten slightly for some tiles
			color = tinfo->color;
			if ((x + y) & 1)
				color |= 0x0F0F0F;
			
			// Draw tile
			DrawSolid(pixels, color, bx, by, TILE_SIZE, TILE_SIZE);
		}
	}
	
	// Draw entities
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		entity = &entities[i];
		
		// Ignore nothing
		if (entity->type == ENTITYTYPE_NOTHING)
			continue;
		
		// Determine draw position of the entity
		bx = (entity->x - vx);
		by = BASIC_SCREEN_HEIGHT - ((entity->y) + TILE_SIZE);
		
		// Off the screen?
		if (bx <= -TILE_SIZE || by <= -TILE_SIZE ||
			bx > BASIC_SCREEN_WIDTH || by > BASIC_SCREEN_HEIGHT)
			continue;
		
		// Draw it
		DrawImageTile(pixels,
			entityinfo[entity->type].pixels[
				((currentframe + i) % GAME_FRAMES_PER_SECOND) <
					(GAME_FRAMES_PER_SECOND / 8)],
			bx, by);
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
	
	// Clear the view
	SDL_memset(pixels, 0,
		sizeof(int32_t) * BASIC_SCREEN_WIDTH * BASIC_SCREEN_HEIGHT);
	
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
						// Walk left
					case SDLK_LEFT:
					case SDLK_q:
						eventtype = EVENTTYPE_WALK_LEFT;
						break;
						
						// Walk right
					case SDLK_RIGHT:
					case SDLK_e:
						eventtype = EVENTTYPE_WALK_RIGHT;
						break;
						
						// Jump
					case SDLK_UP:
					case SDLK_SPACE:
					case SDLK_w:
						eventtype = EVENTTYPE_ROCKET;
						break;
						
						// Attack
					case SDLK_LCTRL:
					case SDLK_RCTRL:
					case SDLK_LALT:
					case SDLK_RALT:
					case SDLK_TAB:
					case SDLK_BACKSPACE:
						eventtype = EVENTTYPE_ATTACK;
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

