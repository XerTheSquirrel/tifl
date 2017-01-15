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
#include <stdio.h>
#include <stdarg.h>

#include "global.h"
#include "video.h"
#include "level.h"
#include "entity.h"

/** The speed the player moves at. */
#define PLAYER_MOVE_SPEED 419424

/** The player jump height. */
#define PLAYER_JUMP_SPEED (PLAYER_MOVE_SPEED)

void Die(const char* format, ...)
{
#define DIE_SIZE 512
	char die[DIE_SIZE];
	va_list args;
	
	// Format it
	va_start(args, format);
	SDL_vsnprintf(die, DIE_SIZE - 1, format, args);
	die[DIE_SIZE - 1] = 0;
	va_end(args);
	
	// Print to output
	fprintf(stderr, "%s\n", format);
	abort();
#undef DIE_SIZE
}

/**
 * The game loop.
 *
 * @since 2017/01/12
 */
void loop()
{
	uint32_t entertime, leavetime, difference;
	
	// Start
	for (uint32_t frameid = 0;; frameid++)
	{
		// Mark start
		entertime = SDL_GetTicks();
		
		// Pump game events
		PumpEvents();
		
		// Quitting?
		if (gamekeydown[EVENTTYPE_QUIT])
			return;
		
		// Respawn the player if he has no entity
		if (playerentity == NULL)
			RespawnPlayer();
		
		// Player interaction
		if (playerentity != NULL)
		{
			// If off the left side, lower the level
			if (playerentity->x < 0)
				InitializeLevel(currentlevelnum - 1);
			
			// Otherwise off the right side, raise the level
			else if (playerentity->x > RIGHT_SIDE_TRANSITION)
				InitializeLevel(currentlevelnum + 1);
			
			// Walk left
			if (gamekeydown[EVENTTYPE_WALK_LEFT])
				WalkEntity(playerentity, -PLAYER_MOVE_SPEED, 0, true);
				
			// Walk right
			if (gamekeydown[EVENTTYPE_WALK_RIGHT])
				WalkEntity(playerentity, PLAYER_MOVE_SPEED, 0, true);
			
			// Jump
			if (gamekeydown[EVENTTYPE_JUMP])
				WalkEntity(playerentity, 0, PLAYER_JUMP_SPEED, true);
		}
		
		// Mark end
		leavetime = SDL_GetTicks();
		difference = MILLISECONDS_PER_FRAME - (leavetime - entertime);
		
		// Enough time available to render the game?
		if (difference > 0 && difference <= MILLISECONDS_PER_FRAME)
			VideoDraw();
		
		// Can still sleep?
		leavetime = SDL_GetTicks();
		difference = MILLISECONDS_PER_FRAME - (leavetime - entertime);
		if (difference > 0 && difference <= MILLISECONDS_PER_FRAME)
			SDL_Delay(difference);
	}
}

/**
 * Main entry point.
 *
 * @param argc Argument count.
 * @param argv Arguments.
 * @return 0 on success, anything else on failure.
 * @since 2017/01/12
 */
int main(int argc, char** argv)
{
	// Initialize events
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
		Die("Failed to initialize event handler and timer.");
	
	// Initialize video
	if (VideoInit() != 0)
		Die("Failed to initialize video.");
	
	// Setup level and respawn the player
	InitializeLevel(0);
	RespawnPlayer();
	
	// Enter game loop
	loop();
	
	// Done
	return 0;
}

