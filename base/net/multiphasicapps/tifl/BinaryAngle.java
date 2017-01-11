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

import java.io.DataInputStream;
import java.io.IOException;

/**
 * This is used to calculate binary angles and such, binary angles are treated
 * as unsigned
 *
 * @since 2017/01/11
 */
public final class BinaryAngle
{
	/** A single degree in a bam. */
	public static final int DEGREES_1 =
		0x00b60b60;
	
	/** 45 degrees. */
	public static final int DEGREES_45 =
		0x20000000;
	
	/** 90 degrees. */
	public static final int DEGREES_90 =
		0x40000000;
	
	/** 135 degrees. */
	public static final int DEGREES_135 =
		0x60000000;
	
	/** 180 degrees. */
	public static final int DEGREES_180 =
		0x80000000;
	
	/** 225 degrees. */
	public static final int DEGREES_225 =
		0xA0000000;
	
	/** 270 degrees. */
	public static final int DEGREES_270 =
		0xC0000000;
	
	/** 315 degrees. */
	public static final int DEGREES_315 =
		0xE0000000;
	
	/** 90 degree angle mask. */
	public static final int DEGREES_90_MASK =
		0x3FFFFFFF;
	
	/** The table of unsigned sine values. */
	private static final char[] _SINE_TABLE;
	
	/**
	 * Initializes data tables.
	 *
	 * @since 2017/01/11
	 */
	static
	{
		// Load the sine table
		char[] sine = new char[32768];
		try (DataInputStream dis = new DataInputStream(
			BinaryAngle.class.getResourceAsStream("sinetable")))
		{
			for (int i = 0, n = sine.length; i < n; i++)
				sine[i] = (char)dis.readShort();
		}
		
		// Failed to load the sine table
		catch (IOException e)
		{
			throw new RuntimeException(e);
		}
		
		// Set table
		_SINE_TABLE = sine;
	}
	
	/**
	 * Not used.
	 *
	 * @since 2017/01/11
	 */
	private BinaryAngle()
	{
	}
	
	/**
	 * Returns the cosine of the given BAM.
	 *
	 * @param __bam The BAM to get the cosine for.
	 * @return The cosine of the given BAM, in fixed radians.
	 * @since 2017/01/11
	 */
	public static int cos(int __bam)
	{
		return sin(__bam + DEGREES_90);
	}
	
	/**
	 * Converts degrees to bams.
	 *
	 * @param __degs The degrees to convert.
	 * @return The binary angle for the given degrees.
	 * @since 2017/01/11
	 */
	public static int degreeToBam(int __deg)
	{
		return __deg * DEGREES_1;
	}
	
	/**
	 * Returns the sine of the given BAM.
	 *
	 * @param __bam The BAM to get the sine for.
	 * @return The sine of the given BAM, in fixed radians.
	 * @since 2017/01/11
	 */
	public static int sin(int __bam)
	{
		// Get lower angle bits
		char[] sinetable = _SINE_TABLE;
		int nbits = (__bam & DEGREES_90_MASK) >> 15;
	
		// 0 - 180
		if (__bam >= 0)
			// 0 - 90: Approaches 1
			if (__bam < DEGREES_90)
				return sinetable[nbits];
		
			// 90 - 180: Approaches zero
			else
				return FixedPoint.ONE - sinetable[nbits];
	
		// 180 - 360
		else
			// 180 - 270: Approaches negative 1
			if (__bam > DEGREES_270)
				return -sinetable[nbits];
		
			// 270 - 360: Approaches zero
			else
				return -sinetable[32768 - nbits];
	}
	
	/**
	 * Returns the tangent of the given BAM.
	 *
	 * @param __bam The BAM to get the tangent for.
	 * @return The tangent of the given BAM, in fixed radians.
	 * @since 2017/01/11
	 */
	public static int tan(int __bam)
	{
		return FixedPoint.divide(sin(__bam), cos(__bam));
	}
}

