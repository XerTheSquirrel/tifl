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

#include "entity.h"
#include "level.h"

/** Global entity data. */
Entity entities[MAX_ENTITIES];

/** The player entity. */
Entity* playerentity = NULL;

/**
 * Traces a line from one point to another and returns the first solid tile
 * that was hit. Tracing is done only in horizontal and vertical lines.
 *
 * All units are in pixels.
 *
 * @param x1 Start X.
 * @param y1 Start Y.
 * @param x2 End X.
 * @param y2 End Y.
 * @param hitx The X tile that was hit.
 * @param hity The Y tile that was hit.
 * @since 2017/01/14
 */
static void TraceLine(int x1, int y1, int x2, int y2, LevelTile** hitx,
	LevelTile** hity)
{
	int vect, end, at, x, y;
	LevelTile* checktile;
	const TileInfo* tinfo;
	boolean first;
	
	// Base position
	x = x1 / TILE_SIZE;
	y = y1 / TILE_SIZE;
	
	// Trace X?
	if (hitx != NULL)
	{
		*hitx = NULL;
	
		// Determine vector for X traveral
		vect = (x1 < x2 ? 1 : -1);
	
		// End tile point
		end = x2 / TILE_SIZE;
	
		// Trace X
		for (at = x1 / TILE_SIZE;; at += vect)
		{
			// Out of bounds?
			if (at >= 0 && at < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT)
			{
				// Get into
				checktile = &leveldata[(y * LEVEL_WIDTH) + at];
				tinfo = &tileinfo[checktile->type];
	
				// Solid tile, stop
				if (tinfo->issolid)
				{
					*hitx = checktile;
					break;
				}
			}
			
			// Stop?
			if (at == end)
				break;
		}
	}
	
	// Trace Y?
	if (hity != NULL)
	{
		*hity = NULL;
		
		// Determine vector for Y traveral
		vect = (y1 < y2 ? 1 : -1);
	
		// End tile point
		end = y2 / TILE_SIZE;
	
		// Trace Y
		for (at = y1 / TILE_SIZE;; at += vect)
		{
			// Must be in bounds
			if (at >= 0 && at < LEVEL_HEIGHT && x >= 0 && x < LEVEL_WIDTH)
			{
				// Get into
				checktile = &leveldata[(at * LEVEL_WIDTH) + x];
				tinfo = &tileinfo[checktile->type];
		
				// Solid tile, stop
				if (tinfo->issolid)
				{
					*hity = checktile;
					break;
				}
			}
			
			// Stop?
			if (at == end)
				break;
		}
	}
}

void WalkEntity(Entity* entity, int32_t relx, int32_t rely, boolean impulse)
{
	int32_t px, py, groundy, downy, newx, newy, movx, usex;
	LevelTile* lground, *rground;
	LevelTile* lohit, *hihit, *check, *ramwall;
	boolean onground, leftgsolid, rightgsolid, cliffside;
	TileInfo* tinfo;
	int di, dx, dy, i, hx, hy;
	
	// Entity position
	px = entity->x;
	py = entity->y;
	
	// If the entity is below the screen, the only way is down
	if (py < 0)
	{
		relx = 0;
		if (rely > 0)
			rely = 0;
	}
	
	// The force going down to detect if landing on the ground
	downy = -(rely >= 0 ? -1 : rely);
	
	// Trace lines to determine if a solid tile is being stood on
	TraceLine(px, py, px, py - downy, NULL, &lground);
	TraceLine(px + (TILE_SIZE - 1), py, px + (TILE_SIZE - 1), py - downy, NULL,
		&rground);
	
	// Either side can be solid
	leftgsolid = (lground != NULL && tileinfo[lground->type].issolid);
	rightgsolid	= (rground != NULL && tileinfo[rground->type].issolid);
	
	// On ground if either a solid
	onground = leftgsolid || rightgsolid;
	
	// But could be on the edge of a cliff
	cliffside = (leftgsolid != rightgsolid);
	
	// The height of the ground that is being stood on
	groundy = -1;
	if (onground)
		groundy = py - (py % TILE_SIZE);
	
	// If on the ground and Y thrust is down, cancel it
	// Also cancel if it is an impulsed jump and we are not on the ground
	if ((onground && rely < 0) || (!onground && impulse && rely > 0))
		rely = 0;
	
	// If not on the ground make left/right movement weaker
	// Or at the edge of a cliff
	if ((!onground || cliffside) && impulse)
		relx /= 2;
	
	// Trace line to determine if a wall is hit in this direction
	newx = px + relx;
	movx = (relx < 0 ? newx : newx + (TILE_SIZE - 1));
	TraceLine(px, py, movx, py, &lohit, NULL);
	TraceLine(px, py + (TILE_SIZE - 1), movx, py + (TILE_SIZE - 1),
		&hihit, NULL);
	
	// Move if not bumped into it
	if (lohit == NULL && hihit == NULL)
		entity->x = newx;
	
	// Otherwise do not run into the wall
	else
	{
		// Determine closer wall to ram into
		if (lohit != NULL && hihit != NULL)
		{
			// Get positions of both blocks
			dx = ((lohit - leveldata) % LEVEL_WIDTH);
			dy = ((hihit - leveldata) % LEVEL_WIDTH);
			
			// If going left, use block with higher index
			if (relx < 0)
				ramwall = (dx > dy ? lohit : hihit);
			
			// Otherwise right uses lower index
			else
				ramwall = (dx < dy ? lohit : hihit);
		}
		
		// Hit lower
		else if (lohit != NULL)
			ramwall = lohit;
		
		// Hit higher
		else
			ramwall = hihit;
		
		// Get position of 
		di = ramwall - leveldata;
		dx = (di % LEVEL_WIDTH);
		dy = (di / LEVEL_WIDTH);
		
		// Pixel positions of the block
		hx = dx * TILE_SIZE;
		hy = hx + TILE_SIZE;
		
		// Running into it from the right side
		if (relx < 0)
			usex = hy;
		
		// Otherwise from the left
		else
			usex = hx - TILE_SIZE;
		
		// Use that instead
		entity->x = usex;
	}
	
	// If standing on the ground, stand on it completely
	newy = py + rely;
	entity->y = (onground && rely <= 0 ? groundy : newy);
}

