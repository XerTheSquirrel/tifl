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
 * The floor is anything that is specific to a single instance of the level
 * and contains the actual grid.
 *
 * @since 2017/01/11
 */
public class Floor
{
	/** The level grid. */
	protected final Grid grid =
		new Grid();
	
	/** The floor ID. */
	protected final int id;
	
	/**
	 * Initializes the floor.
	 *
	 * @param __id The floor ID.
	 * @throws IllegalArgumentException If the floor is zero or negative.
	 * @since 2017/01/11
	 */
	public Floor(int __id)
		throws IllegalArgumentException
	{
		// Check
		if (__id <= 0)
			throw new IllegalArgumentException();
		
		// Set
		this.id = __id;
	}
	
	/**
	 * Returns the level grid.
	 *
	 * @return The level grid.
	 * @since 2017/01/11
	 */
	public Grid grid()
	{
		return this.grid;
	}
	
	/**
	 * Returns the ID of the floor.
	 *
	 * @return The floor ID.
	 * @since 2017/01/11
	 */
	public int id()
	{
		return this.id;
	}
}

