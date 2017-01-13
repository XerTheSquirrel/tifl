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
	int i, x, y;
	
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
	// For now just make a border around the level
	for (x = 0; x < FLOOR_SIZE; x++)
	{
		floordata[x][0].type = FLOORTYPE_GREEN;
		floordata[x][FLOOR_SIZE - 1].type = FLOORTYPE_GREEN;
	}
	for (y = 0; y < FLOOR_SIZE; y++)
	{
		floordata[0][y].type = FLOORTYPE_GREEN;
		floordata[FLOOR_SIZE - 1][y].type = FLOORTYPE_GREEN;
	}
	
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
			entity->x = FLOOR_SIZE << (FIXEDSHIFT - 1);
			entity->y = FLOOR_SIZE << (FIXEDSHIFT - 1);
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

void TraceTile(fixedtype x, fixedtype y, angletype angle, FloorTile** hittile,
	fixedtype* raydistance, boolean* horizhit)
{
	fixedtype travx, travy, dx, dy, cx, cy;
	int idx, idy, tracers;
	FloorTile* tile;
	
	// Clear trace target
	*hittile = NULL;
	*raydistance = 0;
	*horizhit = false;
	
	// Determine the traversal direction in unit circles
	travx = AngleCos(angle);
	travy = AngleSin(angle);
	
	// Traverse tiles in this given direction
	// Also do not trace so many blocks that exceed the level size!
	tracers = 0;
	for (dx = x, dy = y; tracers < 90; dx += travx, dy += travy, tracers++)
	{
		// Get tile coordinates
		idx = (dx >> FIXEDSHIFT);
		idy = (dy >> FIXEDSHIFT);
		
		// If the tile is outside of the level bounds, do nothing
		if (idx < 0 || idx >= FLOOR_SIZE || idy < 0 || idy >= FLOOR_SIZE)
			continue;
		
		// Get tile
		tile = &floordata[idx][idy];
		
		// Can never hit nothing
		if (tile->type == FLOORTYPE_NOTHING)
			continue;
		
		// Set as hit
		*hittile = tile;
		
		// Calculate distance to wall
		*raydistance = FixedMul(OctoDist(x, y, dx, dy), FIXED_C(64));
		
		*horizhit = (((idx + idy) & 1) == 0);
		
		// Stop
		return;
	}
}

