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
	 * This returns the approximated distance between two points, the returned
	 * result forms the shape of an octogon. For short distances this should
	 * be sufficient enough without causing much error.
	 *
	 * @param __x1 X1.
	 * @param __y1 Y1.
	 * @param __x2 X2.
	 * @param __y2 Y2.
	 * @return The octogonal distance.
	 * @since 2017/01/11
	 */
	public static int octogonalDistance(int __x1, int __y1, int __x2, int __y2)
	{
		int dx, dy, mm;	

		// Calculate difference between values
		dx = __x2 - __x1;
		if (dx < 0)
			dx = -dx;
		dy = __y2 - __y1;
		if (dy < 0)
			dy = -dy;

		// Maximum distance
		if (dx > dy)
			mm = dx;
		else
			mm = dy;

		// Difference between them
		return ((dx + dy) + mm) >> 1;
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

