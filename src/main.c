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
#include "entity.h"
#include "floor.h"

/** The angle the player turns at. */
#define PLAYER_TURN_ANGLE 0x02d82d80

/** The speed the player moves at. */
#define PLAYER_MOVE_SPEED ((1 << (FIXEDSHIFT - 1)) - (1 << (FIXEDSHIFT - 3)))

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
	boolean strafing;
	
	// Start
	for (uint32_t frameid = 0;; frameid++)
	{
		// Mark start
		entertime = SDL_GetTicks();
		
		// If the current floor is finished, go to the next floor
		if (IsFloorFinished())
			FloorNext();
		
		// Pump game events
		PumpEvents();
		
		// Quitting?
		if (gamekeydown[EVENTTYPE_QUIT])
			return;
		
		// Player interaction
		if (playerentity != NULL)
		{
			strafing = gamekeydown[EVENTTYPE_STRAFE_MODE];
			
			// Turn/strafe left?
			if (!strafing && gamekeydown[EVENTTYPE_TURN_LEFT])
				playerentity->angle -= PLAYER_TURN_ANGLE;
			else if ((strafing && gamekeydown[EVENTTYPE_TURN_LEFT]) ||
				gamekeydown[EVENTTYPE_STRAFE_LEFT])
				WalkEntity(playerentity, -PLAYER_MOVE_SPEED, 0);
			
			// Turn/strafe right?
			if (!strafing && gamekeydown[EVENTTYPE_TURN_RIGHT])
				playerentity->angle += PLAYER_TURN_ANGLE;
			else if ((strafing && gamekeydown[EVENTTYPE_TURN_RIGHT]) ||
				gamekeydown[EVENTTYPE_STRAFE_RIGHT])
				WalkEntity(playerentity, PLAYER_MOVE_SPEED, 0);
			
			// Walk forward?
			if (gamekeydown[EVENTTYPE_WALK_FORWARD])
				WalkEntity(playerentity, 0, PLAYER_MOVE_SPEED);
			
			// Walk backward?
			if (gamekeydown[EVENTTYPE_WALK_BACKWARD])
				WalkEntity(playerentity, 0, -PLAYER_MOVE_SPEED);
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
	
	// Implicitly start the next floor
	FloorNext();
	
	// Enter game loop
	loop();
	
	// Done
	return 0;
}

