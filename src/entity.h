// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_ENTITY_H
#define TIFL_ENTITY_H

#include "global.h"

/**
 * The type of entity this is.
 *
 * @since 2017/01/12
 */
typedef enum EntityType
{
	/** Nothing. */
	ENTITYTYPE_NOTHING,
	
	/** Player start position. */
	ENTITYTYPE_PLAYER_START,
	
	/** The number of entity types. */
	NUM_ENTITYTYPES
} EntityType;

/**
 * Entity data.
 *
 * @since 2017/01/12
 */
typedef struct Entity
{
	/** The type of entity this is. */
	EntityType type;
	
	/** Postition of the entity. */
	int32_t x, y;
	
	/** The angle the entity is facing. */
	angletype angle;
} Entity;

/** Maximum number of entities. */
#define MAX_ENTITIES 128

/** Global entity data. */
extern Entity entities[MAX_ENTITIES];

/** The player entity. */
extern Entity* playerentity;

/**
 * Walks the given entity in the given direction.
 *
 * The movement is based on the angle the entity is facing.
 *
 * @param entity The entity to walk.
 * @param relx The forward facing x movement (left/right).
 * @param rely The forward facing y movement (forwards/backwards).
 */
void WalkEntity(Entity* entity, fixedtype relx, fixedtype rely);

#endif

