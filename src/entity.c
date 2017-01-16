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

#include "player1.xpm"
#include "player2.xpm"
#include "zap1.xpm"
#include "zap2.xpm"
#include "bat1.xpm"
#include "bat2.xpm"
#include "bunny1.xpm"
#include "bunny2.xpm"
#include "cat1.xpm"
#include "cat2.xpm"

/** How long the anthrogun stuns for. */
#define ANTHROGUN_HITSTUN 2

/** Global entity data. */
Entity entities[MAX_ENTITIES];

/** The player entity. */
Entity* playerentity = NULL;

EntityInfo entityinfo[NUM_ENTITYTYPES] =
{
	// None
	{
	},
	
	// Player
	{
		{
			player1_xpm,
			player2_xpm
		},
		true,
		20
	},
	
	// Anthrobolt
	{
		{
			zap1_xpm,
			zap2_xpm
		},
		false,
		1
	},
	
	// Bat
	{
		{
			bat1_xpm,
			bat2_xpm
		},
		false,
		5
	},
	
	// Bunny
	{
		{
			bunny1_xpm,
			bunny2_xpm
		},
		true,
		3
	},
	
	// Cat
	{
		{
			cat1_xpm,
			cat2_xpm
		},
		true,
		10
	}
};

void LoadSprites()
{
	int i, j, xw, xh, xc, xp, k, l, color, code, x, y;
	EntityInfo* info;
	char** xpmdata, *line, *p, letter;
	uint32_t* pixels, *o;
	int colormap[256];
	
	// Go through entity types
	for (i = 1; i < NUM_ENTITYTYPES; i++)
	{
		info = &entityinfo[i];
		
		for (j = 0; j < 2; j++)
		{
			// Get input and output
			xpmdata = info->xpm[j];
			pixels = info->pixels[j];
			
			// Clear the color map
			for (k = 0; k < 256; k++)
				colormap[k] = 0xFF00FF;
		
			// Read image details
			SDL_sscanf(xpmdata[0], "%d %d %d %d", &xw, &xh, &xc, &xp);
			
			// Cap image size
			if (xw > TILE_SIZE)
				xw = TILE_SIZE;
			if (xh > TILE_SIZE)
				xh = TILE_SIZE;
			
			// Read in all color details
			for (k = 0; k < xc; k++)
			{
				line = xpmdata[1 + k];
				
				// .	c #1B1B19
				// Read characters in the color
				code = line[0];
				
				// Only support hex colors
				for (p = &line[xp]; *p; p++)
					if (*p == '#')
					{
						// Skip the hash
						p++;
						break;
					}
				
				// Read in color
				color = 0;
				for (; *p; p++)
				{
					// Shift up
					color <<= 4;
					
					// Decode hex
					letter = *p;
					if (letter >= '0' && letter <= '9')
						color |= letter - '0';
					else if (letter >= 'a' && letter <= 'f')
						color |= 10 + (letter - 'a');
					else if (letter >= 'A' && letter <= 'F')
						color |= 10 + (letter - 'A');
				}
				
				// Store it
				colormap[code] = color;
			}
			
			// Read in image data
			for (y = 0; y < xh; y++)
			{
				// Read color pixels
				p = xpmdata[1 + xc + y];
				o = &pixels[y * TILE_SIZE];
				
				// Decode characters, but only single character ones
				for (x = 0; x < xw; x++)
					*(o++) = colormap[p[x * xp] & 0xFF];
			}
		}
	}
}

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

/**
 * Called when an entity hits another entity or a tie.
 *
 * @param source The source object.
 * @param hitentity The entity that was hit, may be {@code NULL}.
 * @param hittile The hit tile, may be {@code NULL}.
 * @param tx The X position of the tile.
 * @param ty The y position of the tile.
 * @since 2017/01/15
 */
void HitSomething(Entity* source, Entity* hitentity, LevelTile* hittile,
	int32_t tx, int32_t ty)
{
	int32_t xx, yy;
	EntityInfo* hitinfo;
	Entity *hurttarget;
	
	// Get info on hit object
	hurttarget = NULL;
	hitinfo = (hitentity == NULL ? NULL : &entityinfo[hitentity->type]);
	
	// Depends on the type
	switch (source->type)
	{
			// Anthrobolt
		case ENTITYTYPE_ANTHROBOLT:
			// Remove bolt
			source->type = ENTITYTYPE_NOTHING;
			
			// Stun, deferal, and bump object
			if (hitentity != NULL && hitentity == playerentity)
			{
				// Hit bolt where the entity is
				xx = hitentity->x;
				yy = source->y;
				
				// Never replace a longer stun
				if (hitentity->stun < ANTHROGUN_HITSTUN)
					hitentity->stun = ANTHROGUN_HITSTUN;
				
				// Push the object
				WalkEntity(hitentity, (source->angle == FACETYPE_RIGHT ?
					TILE_SIZE : -TILE_SIZE), 0, false);
				
				// Hurt this one
				hurttarget = hitentity;
			}
			
			// If it hit a tile, destroy it
			if (hittile)
			{
				xx = tx * TILE_SIZE;
				yy = ty * TILE_SIZE;
			}
			
			// Spawn an explosion
			break;
			
			// Turn around if ran into a wall or entity
		case ENTITYTYPE_BAT:
		case ENTITYTYPE_BUNNY:
		case ENTITYTYPE_CAT:
			// Turn around
			source->angle = (source->angle == FACETYPE_RIGHT ?
				FACETYPE_LEFT : FACETYPE_RIGHT);
			
			// If a player was hit, hurt them
			if (hitentity != NULL && hitentity == playerentity)
				hurttarget = hitentity;
			break;
			
			// Unknown
		default:
			break;
	}
	
	// Increase the object's pain
	// If it is hurt too much, remove it
	if (hurttarget != NULL)
		if ((hurttarget->pain++) >= entityinfo[hurttarget->type].painthreshold)
			SDL_memset(hurttarget, 0, sizeof(*hurttarget));
}

void WalkEntity(Entity* entity, int32_t relx, int32_t rely, boolean impulse)
{
	int32_t px, py, groundy, downy, newx, newy, movx, usex, di, dx, dy, i, hx,
		hy, hittx, hitty, ax, ay, bx, by;
	LevelTile* lground, *rground;
	LevelTile* lohit, *hihit, *check, *ramwall;
	boolean onground, leftgsolid, rightgsolid, cliffside;
	TileInfo* tinfo;
	LevelTile* hitsometile = NULL;
	Entity* hitsomeentity = NULL, *checkent;
	EntityInfo* einfo;
	
	// If this is an impulse and it is stunned, do nothing
	if (impulse && entity->stun)
		return;
	
	// Get info
	einfo = &entityinfo[entity->type];
	
	// If impulse, change facing direction
	if (impulse && relx != 0)
	{
		if (relx > 0)
			entity->angle = FACETYPE_RIGHT;
		else if (relx < 0)
			entity->angle = FACETYPE_LEFT;
	}
	
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
	
	// Must feel gravity for these cases
	if (einfo->feelsgravity)
	{
		// If on the ground and Y thrust is down, cancel it
		// Also cancel if it is an impulsed jump and we are not on the ground
		if ((onground && rely < 0) || (!onground && impulse && rely > 0))
			rely = 0;
	
		// If not on the ground make left/right movement weaker
		// Or at the edge of a cliff
		if ((!onground || cliffside) && impulse)
			relx /= 2;
	}
	
	// Check if bumping head on a block
	if (rely > 0)
	{
		// Trace up
		TraceLine(px, py, px, py + (TILE_SIZE + 1), NULL, &lohit);
		TraceLine(px + (TILE_SIZE - 1), py, px + (TILE_SIZE - 1),
			py + (TILE_SIZE + 1), NULL, &hihit);
		
		// Not bumped into
		if (lohit == NULL && hihit == NULL)
			newy = py + rely;
		else
		{
			// Hit lower
			if (lohit != NULL)
				ramwall = lohit;
		
			// Hit higher
			else
				ramwall = hihit;
		
			// Get position of the block
			di = ramwall - leveldata;
			dy = (di / LEVEL_WIDTH);
		
			// Do not go past it
			newy = (dy - 1) * TILE_SIZE;
			
			// Report that it was hit
			hitsometile = ramwall;
			hittx = di % LEVEL_WIDTH;
			hitty = dy;
		}
	}
	
	// Not going up
	else
		newy = py + rely;
	
	// Only check if moving
	if (relx != 0)
	{
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
			
			// Report that it was hit
			hitsometile = ramwall;
			hittx = dx;
			hitty = di / LEVEL_WIDTH;
		}
	}
	
	// If standing on the ground, stand on it completely
	entity->y = (onground && rely <= 0 ? groundy : newy);
	
	// Check to see if an entity was hit
	px = entity->x;
	py = entity->y;
	dx = px + TILE_SIZE;
	dy = py + TILE_SIZE;
	for (i = 0; i < MAX_ENTITIES; i++)
	{
		// Ignore nothing
		checkent = &entities[i];
		if (checkent->type == ENTITYTYPE_NOTHING)
			continue;
		
		// Ignore self
		if (checkent == entity)
			continue;
		
		// Get object shape?
		ax = checkent->x;
		ay = checkent->y;
		bx = ax + TILE_SIZE;
		by = ay + TILE_SIZE;
		
		// No chance of collision?
		if (dx < ax || dy < ay || px >= bx || py >= by)
			continue;
		
		// Hit it
		hitsomeentity = checkent;
		break;
	}
	
	// Register a single hit only
	if (hitsometile != NULL || hitsomeentity != NULL)
		HitSomething(entity, hitsomeentity, hitsometile, hittx, hitty);
}

Entity* BlankEntity()
{
	int i;
	
	// Find the next empty entity
	for (i = 0; MAX_ENTITIES; i++)
		if (entities[i].type == ENTITYTYPE_NOTHING)
			return &entities[i];
	
	// None found
	return NULL;
}

