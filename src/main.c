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
#include "random.h"

/** The speed the player moves at. */
#define PLAYER_MOVE_SPEED 8

/** The player jump height. */
#define PLAYER_JUMP_SPEED (GRAVITY_FORCE * 2)

/** Weaker rocket blast. */
#define PLAYER_JUMP_SPEED_WEAK ((GRAVITY_FORCE * 2) - (GRAVITY_FORCE / 4))

/** Very weak. */
#define PLAYER_JUMP_SPEED_BARE (GRAVITY_FORCE + 1)

/** Gravity force. */
#define GRAVITY_FORCE 8

/** The sped the anthrobolt moves at. */
#define ANTHROBOLT_SPEED 16

/** The number of frames to be stunned after firing the anthrogun. */
#define ANTHROGUN_PLAYER_STUN 3

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

static void RunEntities()
{
	int i;
	Entity* entity;
	EntityInfo* info;
	
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		entity = &entities[i];
		
		// Ignore nothing
		if (entity->type == ENTITYTYPE_NOTHING)
			continue;
		
		// Get into
		info = &entityinfo[entity->type];
		
		// Decrease stun, if stunned
		if (entity->stun > 0)
			entity->stun--;
		
		// Push down if it feels gravity
		if (info->feelsgravity)
			WalkEntity(&entities[i], 0, -GRAVITY_FORCE, false);
		
		// Non-player actions
		if (entity != playerentity)
		{
			// Remove if it falls off the level
			if (entity->x < 0 || entity->x >= LEVEL_WIDTH_PIXELS ||
				entity->y < -TILE_SIZE || entity->y >= LEVEL_HEIGHT)
			{
				entity->type = ENTITYTYPE_NOTHING;
				continue;
			}
			
			// Run AI action for it
			switch (entity->type)
			{
					// Anthrobolt, continues moving in the direction it is
					// facing
				case ENTITYTYPE_ANTHROBOLT:
					WalkEntity(entity, (entity->angle == FACETYPE_RIGHT ?
						ANTHROBOLT_SPEED : -ANTHROBOLT_SPEED), 0, false);
					break;
				
					// Unknown, do nothing
				default:
					break;
			}
		}
	}
}

static void SpawnPlayerAttack()
{
	Entity* entity;
	
	// Do not attack if dead
	if (playerentity == NULL)
		return;
	
	// If the player is stunned, cannot fire
	if (playerentity->stun > 0)
		return;
	
	// Get next entity to use, if possible
	entity = BlankEntity();
	if (entity == NULL)
		return;
	
	// Set information
	entity->type = ENTITYTYPE_ANTHROBOLT;
	
	// The bolt is aimed at the same angle as the player and spawns right
	// in front of them
	entity->angle = playerentity->angle;
	entity->y = playerentity->y;
	if (entity->angle == FACETYPE_RIGHT)
		entity->x = playerentity->x + TILE_SIZE;
	else
		entity->x = playerentity->x - TILE_SIZE;
	
	// Stun the player for a bit to prevent super rapid fire and movement
	// when firing
	playerentity->stun = ANTHROGUN_PLAYER_STUN;
}

/**
 * The game loop.
 *
 * @since 2017/01/12
 */
void loop()
{
	uint32_t entertime, leavetime, difference;
	int i, rocketcount;
	
	// Start
	rocketcount = 0;
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
			// Walk left
			if (gamekeydown[EVENTTYPE_WALK_LEFT])
				WalkEntity(playerentity, -PLAYER_MOVE_SPEED, 0, true);
				
			// Walk right
			if (gamekeydown[EVENTTYPE_WALK_RIGHT])
				WalkEntity(playerentity, PLAYER_MOVE_SPEED, 0, true);
			
			// Rocket
			if (gamekeydown[EVENTTYPE_ROCKET])
				if (playerentity->y >= WEAK_ROCKET_BOOST_HEIGHT)
					WalkEntity(playerentity, 0, PLAYER_JUMP_SPEED_BARE, true);
				else if (playerentity->y >= HALF_LEVEL_HEIGHT_PIXELS)
					WalkEntity(playerentity, 0, PLAYER_JUMP_SPEED_WEAK, true);
				else
					WalkEntity(playerentity, 0, PLAYER_JUMP_SPEED, true);
			
			// Attack
			if (gamekeydown[EVENTTYPE_ATTACK])
				SpawnPlayerAttack();
		}
		
		// Interact with all entities
		RunEntities();
		
		// Player feel off the screen on the bottom, so respawn them
		if (playerentity != NULL && playerentity->y < -(TILE_SIZE * 4))
			RespawnPlayer();
		
		// If off the left side, lower the level
		if (playerentity->x < 0)
			InitializeLevel(currentlevelnum - 1);
		
		// Otherwise off the right side, raise the level
		else if (playerentity->x > RIGHT_SIDE_TRANSITION)
			InitializeLevel(currentlevelnum + 1);
		
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
		
		// Frame up
		currentframe++;
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
	
	// Load sprite data
	LoadSprites();
	
	// Setup level and respawn the player
	currentframe = 0;
	InitializeLevel(0);
	RespawnPlayer();
	
	// Enter game loop
	loop();
	
	// Done
	return 0;
}

