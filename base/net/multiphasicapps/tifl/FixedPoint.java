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
 * Fixed point math that is beyond addition and subtraction.
 *
 * @since 2017/01/11
 */
public final class FixedPoint
{
	/** The number of bits in the fraction. */
	public static final int FRACTION_BITS =
		16;
	
	/** The value one. */
	public static final int ONE =
		1 << FRACTION_BITS;
	
	/**
	 * Not used.
	 *
	 * @since 2017/01/11
	 */
	private FixedPoint()
	{
	}
	
	/**
	 * Divides two fixed point values.
	 *
	 * @param __a The first value.
	 * @param __b The second value.
	 * @return The result of division.
	 * @since 2017/01/11
	 */
	public static int divide(int __a, int __b)
	{
		return (int)(((long)__a) << FRACTION_BITS) / __b;
	}
}

