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

import java.util.Random;

/**
 * This contains the game simulation which contains the higher level details
 * and per-level floors.
 *
 * @since 2017/01/11
 */
public class Simulation
{
	/** The frame rate of the simulation. */
	public static final int FRAME_RATE =
		20;
	
	/** Nanoseconds per frame. */
	public static final long NANOS_PER_FRAME =
		1_000_000_000L / FRAME_RATE;
	
	/** The current floor. */
	private volatile Floor _floor;
	
	/** Has the simulation ended? */
	private volatile boolean _ended;
	
	/** The current game frame. */
	private volatile int _frame;
	
	/**
	 * Returns the current game frame.
	 *
	 * @return The current game frame.
	 * @since 2017/01/11
	 */
	public int currentFrame()
	{
		return this._frame;
	}
	
	/**
	 * Has this simulation ended?
	 *
	 * @return {@code true} if it has ended.
	 * @since 2017/01/11
	 */
	public boolean hasEnded()
	{
		return this._ended;
	}
	
	/**
	 * Runs a single frame.
	 *
	 * @since 2017/01/11
	 */
	public void runFrame()
	{
		// Run a single loop instance
		int frame = this._frame;
		try
		{
		}
		
		// Always update the game frame.
		finally
		{
			this._frame = frame + 1;
		}
	}
}

