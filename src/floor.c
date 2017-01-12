// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#include <SDL_stdinc.h>

#include "floor.h"
#include "entity.h"

boolean floorwasfinished;

int onfloor = -1;

int floorcolor = 0xFF0000;

int ceilingcolor = 0x0000FF;

/** Floor data. */
FloorTile floordata[FLOOR_SIZE][FLOOR_SIZE];

boolean IsFloorFinished()
{
	return floorwasfinished;
}

void FloorNext()
{
	Entity player, *entity;
	int i;
	
	// Go to the next floor
	onfloor = (onfloor + 1) % FLOOR_LEVELS;
	
	// Clear the floor data
	SDL_memset(floordata, 0, sizeof(floordata));
	
	// Remember the player entity information so the player has the same stats
	// on the next level
	SDL_memset(&player, 0, sizeof(player));
	if (playerentity != NULL)
		SDL_memmove(&player, playerentity, sizeof(*playerentity));
	
	// Wipe all entities away
	SDL_memset(entities, 0, sizeof(entities));
	
	// Load new map tile data
	
	// Load in entities
	
	// Restore player data at the player start position
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		// Only use player starts
		entity = &entities[i];
		if (entity->type != ENTITYTYPE_PLAYER_START)
		{
			// Ignore
			if (i < MAX_ENTITIES - 1)
				continue;
			
			// However, if this point was reached then the level has no
			// player start, force one to exist
			entity->x = 0;
			entity->y = 0;
			entity->type = ENTITYTYPE_PLAYER_START;
		}
		
		// Set player here
		playerentity = entity;
		
		// Only set for a single player
		break;
	}
	
	// Floor not finished anymore
	floorwasfinished = false;
}

