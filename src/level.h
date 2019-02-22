// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Stephanie Gawroriski <xer@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_LEVEL_H
#define TIFL_LEVEL_H

#include "global.h"

/** The size of tiles. */
#define TILE_SIZE 20

/** The view width. */
#define VIEW_WIDTH 16

/** The width of the level. */
#define LEVEL_WIDTH (VIEW_WIDTH * 2)

/** The width of the level in pixels. */
#define LEVEL_WIDTH_PIXELS (LEVEL_WIDTH * TILE_SIZE)

/** The height of levels. */
#define LEVEL_HEIGHT 12

/** Half the height in pixels. */
#define HALF_LEVEL_HEIGHT_PIXELS ((LEVEL_HEIGHT * TILE_SIZE) / 2)

/** The height at which the rocket boost is not as great. */
#define WEAK_ROCKET_BOOST_HEIGHT (HALF_LEVEL_HEIGHT_PIXELS + \
	(HALF_LEVEL_HEIGHT_PIXELS / 2))

/** Height of the level in pixels. */
#define LEVEL_HEIGHT_PIXELS (LEVEL_HEIGHT * TILE_SIZE)

/** The view width in pixels. */
#define VIEW_WIDTH_PIXELS ((VIEW_WIDTH) * TILE_SIZE)

/** Half of the view width in pixels. */
#define HALF_VIEW_WIDTH_PIXELS (VIEW_WIDTH_PIXELS / 2)

/** Transition to the next level. */
#define RIGHT_SIDE_TRANSITION (LEVEL_WIDTH_PIXELS - TILE_SIZE)

/**
 * The type of tile in a level.
 *
 * @since 2017/01/14
 */
typedef enum TileType
{
	/** Air. */
	TILETYPE_AIR,
	
	/** Grass. */
	TILETYPE_GRASS,
	
	/** Wood. */
	TILETYPE_WOOD,
	
	/** Cloud. */
	TILETYPE_CLOUD,
	
	/** The number of tiles used. */
	NUM_TILETYPES,
} TileType;

/**
 * A single tile within a level.
 *
 * @since 2017/01/14
 */
typedef struct LevelTile
{
	/** The type of tile used here. */
	TileType type;
} LevelTile;

/**
 * Tile information structure.
 *
 * @since 2017/01/14
 */
typedef struct TileInfo
{
	/** Basic color used. */
	uint32_t color;
	
	/** Is the tile solid? */
	boolean issolid;
} TileInfo;

/** Tile information. */
extern const TileInfo tileinfo[NUM_TILETYPES];

/** Tiles within the level. */
extern LevelTile leveldata[LEVEL_WIDTH * LEVEL_HEIGHT];

/** The current level number. */
extern int currentlevelnum;

/**
 * Initializes a new level.
 *
 * @param levelnum The level number to use.
 * @since 2017/01/14
 */
void InitializeLevel(int levelnum);

/**
 * Respawns the player.
 *
 * @since 2017/01/14
 */
void RespawnPlayer();

/**
 * Returns the height of the ground at the given coordinate.
 *
 * @param The coordinate to get the ground height for, a negative value is
 * returned if there is no ground.
 * @since 2017/01/15
 */
int32_t GroundHeight(int32_t x);

#endif

