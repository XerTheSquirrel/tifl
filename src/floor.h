// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_FLOOR_H
#define TIFL_FLOOR_H

#include "global.h"

/** The size of tiles in pixels. */
#define TILE_SIZE 64

/** The size of each floor. */
#define FLOOR_SIZE 64

/** The number of game levels available. */
#define FLOOR_LEVELS 64

/**
 * The floor tile type.
 *
 * @since 2016/01/12
 */
typedef enum FloorType
{
	/** Grass. */
	FLOORTYPE_FLOOR_GRASS,
	
	/** Start of wall types. */
	FLOORTYPE_START_WALLS,
	
	/** Wood. */
	FLOORTYPE_WALL_WOOD,
	
	/** The number of floor types. */
	NUM_FLOORTYPE
} FloorType;

/**
 * The floor tile data.
 *
 * @since 2017/01/12
 */
typedef struct FloorTile
{
	/** The type of floor this is. */
	FloorType type;
} FloorTile;

/** The floor data. */
extern FloorTile floordata[FLOOR_SIZE][FLOOR_SIZE];

/** The current floor the game is on. */
extern int onfloor;

/**
 * Checks if the current floor is finished.
 *
 * @return {@code true} if it is finished.
 * @since 2017/01/12
 */
boolean IsFloorFinished();

/**
 * Goes to the next floor.
 *
 * @since 2017/01/12
 */
void FloorNext();

#endif

