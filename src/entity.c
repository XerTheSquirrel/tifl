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

void WalkEntity(Entity* entity, fixedtype relx, fixedtype rely)
{
	fixedtype newx, newy;
	
	// Determine next position based on movement
	newx = entity->x + FixedMul(relx, AngleCos(entity->angle + ANG90)) +
		FixedMul(rely, AngleCos(entity->angle));
	newy = entity->y + FixedMul(relx, AngleSin(entity->angle + ANG90)) +
		FixedMul(rely, AngleSin(entity->angle));
	
	// New position is OK, use it
	entity->x = newx;
	entity->y = newy;
}

