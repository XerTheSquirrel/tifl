// -*- Mode: Java; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

package net.multiphasicapps.tifl;

/**
 * This contains the actual level data for the game and is used to represent
 * the level.
 *
 * Entities are not contained within the level but in {@link Simulation}.
 *
 * @since 2017/01/11
 */
public class Grid
{
	/** The size of blocks in pixels. */
	public static final int BLOCK_SIZE =
		64;
	
	/** The number of blocks within the level. */
	public static final int GRID_SIZE =
		64;
	
	/** The level data. */
	protected final char[] grid =
		new char[GRID_SIZE * GRID_SIZE];
	
	/**
	 * Returns the raw level grid data.
	 *
	 * @return The raw level grid data.
	 * @since 2017/01/11
	 */
	public char[] grid()
	{
		return this.grid;
	}
}

