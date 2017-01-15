// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_LEVEL_H
#define TIFL_LEVEL_H

#include "global.h"

/** The size of tiles. */
#define TILE_SIZE 16

/** The view width. */
#define VIEW_WIDTH 40

/** The width of the level. */
#define LEVEL_WIDTH (VIEW_WIDTH * 4)

/** The height of levels. */
#define LEVEL_HEIGHT 15

/** Fixed point tile size. */
#define FIXED_TILE_SIZE FIXED_C(TILE_SIZE)

/** The width of the level in fixed point. */
#define FIXED_LEVEL_WIDTH (LEVEL_WIDTH << FIXEDSHIFT)

/** The view width in fixed values. */
#define FIXED_VIEW_WIDTH FIXED_C(VIEW_WIDTH)

/** Half of the view width. */
#define FIXED_HALF_VIEW_WIDTH (FIXED_VIEW_WIDTH >> 1)

/** The level width in pixels. */
#define FIXED_LEVEL_WIDTH_PIXELS (FIXED_LEVEL_WIDTH * TILE_SIZE)

/** The view width in pixels. */
#define FIXED_VIEW_WIDTH_PIXELS (FIXED_VIEW_WIDTH * TILE_SIZE)

/** Half the view width in pixels. */
#define FIXED_HALF_VIEW_WIDTH_PIXELS (FIXED_HALF_VIEW_WIDTH * TILE_SIZE)

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
} TileInfo;

/** Tile information. */
extern const TileInfo tileinfo[NUM_TILETYPES];

/** Tiles within the level. */
extern LevelTile leveldata[LEVEL_WIDTH][LEVEL_HEIGHT];

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

#endif

