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
		(int)(0x1_0000_0000L / 360L);
	
	/** 45 degrees. */
	public static final int DEGREES_45 =
		DEGREES_1 * 45;
	
	/** 90 degrees. */
	public static final int DEGREES_90 =
		DEGREES_1 * 90;
	
	/** 135 degrees. */
	public static final int DEGREES_135 =
		DEGREES_1 * 135;
	
	/** 180 degrees. */
	public static final int DEGREES_180 =
		DEGREES_1 * 180;
	
	/** 225 degrees. */
	public static final int DEGREES_225 =
		DEGREES_1 * 225;
	
	/** 270 degrees. */
	public static final int DEGREES_270 =
		DEGREES_1 * 270;
	
	/** 315 degrees. */
	public static final int DEGREES_315 =
		DEGREES_1 * 315;
	
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
		char[] sine = new char[12867];
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
	 * Returns the tangent of the given BAM.
	 *
	 * @param __bam The BAM to get the tangent for.
	 * @return The tangent of the given BAM.
	 * @since 2017/01/11
	 */
	public static int tan(int __bam)
	{
		throw new Error("TODO");
	}
}

