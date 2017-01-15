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

/** Global entity data. */
Entity entities[MAX_ENTITIES];

/** The player entity. */
Entity* playerentity = NULL;

void WalkEntity(Entity* entity, fixedtype relx, fixedtype rely,
	boolean impulse)
{
	fixedtype newx, newy;
	int i;
	boolean onground;
	
	// Impulsed, check if it is on the ground
	if (impulse)
	{
		i = FixedDiv(entity->y, FIXED_TILE_SIZE);
	}
	
	// Target X and Y position
	newx = entity->x + relx;
	newy = entity->y + rely;
	
	// Move is OK
	entity->x = newx;
	entity->y = newy;
}

