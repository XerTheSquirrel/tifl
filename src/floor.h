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
	/** Nothing. */
	FLOORTYPE_NOTHING,
	
	/** Red. */
	FLOORTYPE_RED,
	
	/** Green. */
	FLOORTYPE_GREEN,
	
	/** Yellow. */
	FLOORTYPE_YELLOW,
	
	/** Blue. */
	FLOORTYPE_BLUE,
	
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

/** The color of the floor. */
extern int floorcolor;

/** The color of the ceiling. */
extern int ceilingcolor;

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

/**
 * Traces a ray to the given tile.
 *
 * @param x The starting X position.
 * @param y The starting Y position.
 * @param hittile The tile that was hit.
 * @param raydistance The distance to the tile.
 * @param horizhit Was the wall hit on the horizontal side?
 */
void TraceTile(fixedtype x, fixedtype y, angletype angle, FloorTile** hittile,
	fixedtype* raydistance, boolean* horizhit);

#endif

