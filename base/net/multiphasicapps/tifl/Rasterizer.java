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
 * This contains the rasterizer for the game which renders it.
 *
 * @since 2017/01/11
 */
public class Rasterizer
{
	/** The screen width. */
	public static final int SCREEN_WIDTH =
		320;
	
	/** The screen height. */
	public static final int SCREEN_HEIGHT =
		200;
	
	/** The field of view in bams. */
	public static final int FIELD_OF_VIEW_BAMS =
		BinaryAngle.degreeToBam(60);
	
	/** The game framebuffer. */
	protected final int[] framebuffer;
	
	/** The view width. */
	protected final int width;
	
	/** The view height. */
	protected final int height;
	
	/** The distance to the projection plane. */
	protected final int fixedprojectionplanedist;
	
	/** The angle of rays (in bams). */
	protected final int bamrayangle;
	
	/** The simulation to draw. */
	private volatile Simulation _simulation;
	
	/**
	 * Initializes the rasterizer with the default resolution.
	 *
	 * @since 2017/01/11
	 */
	public Rasterizer()
	{
		this(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	/**
	 * Initializes the rasterizer with the given resolution.
	 *
	 * @param __w The width.
	 * @param __h The height.
	 * @throws IllegalArgumentException If any dimension is zero or negative.
	 * @since 2017/01/11
	 */
	public Rasterizer(int __w, int __h)
		throws IllegalArgumentException
	{
		this(new FramebufferProvider()
			{
				/**
				 * {@inheritDoc}
				 * @since 2017/01/11
				 */
				@Override
				public int[] createFramebuffer(int __w, int __h)
				{
					return new int[__w * __h];
				}
			}, __w, __h);
	}
	
	/**
	 * Initializes the rasterizer with the default resolution and the given
	 * framebuffer provider.
	 *
	 * @param __fbp The framebuffer provider to use.
	 * @throws NullPointerException On null arguments.
	 * @since 2017/01/11
	 */
	public Rasterizer(FramebufferProvider __fbp)
		throws NullPointerException
	{
		this(__fbp, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	/**
	 * Initializes the rasterizer with the given resolution and the given
	 * framebuffer provider.
	 *
	 * @param __fbp The framebuffer provider to use.
	 * @param __w The width.
	 * @param __h The height.
	 * @throws IllegalArgumentException If any dimension is zero or negative.
	 * @throws NullPointerException On null arguments.
	 * @since 2017/01/11
	 */
	public Rasterizer(FramebufferProvider __fbp, int __w, int __h)
		throws IllegalArgumentException, NullPointerException
	{
		// Check
		if (__fbp == null)
			throw new NullPointerException();
		if (__w <= 0 || __h <= 0)
			throw new IllegalArgumentException();
		
		// Set
		this.width = __w;
		this.height = __h;
		
		// Setup framebuffer
		int[] framebuffer = __fbp.createFramebuffer(__w, __h);
		this.framebuffer = framebuffer;
		
		// The distance to the projection plane
		this.fixedprojectionplanedist = FixedPoint.divide(
			__w << (FixedPoint.FRACTION_BITS - 1),
			BinaryAngle.tan(FIELD_OF_VIEW_BAMS));
		
		// The offset angle for each ray
		this.bamrayangle = FIELD_OF_VIEW_BAMS / __w;
	}
	
	/**
	 * Returns the raw framebuffer.
	 *
	 * @return The framebuffer
	 * @since 2017/01/11
	 */
	public int[] framebuffer()
	{
		return this.framebuffer;
	}
	
	/**
	 * Returns the height.
	 *
	 * @return The height.
	 * @since 2017/01/11
	 */
	public int height()
	{
		return this.height;
	}
	
	/**
	 * Sets the game simulation to draw for.
	 *
	 * @param __s The simulation to draw for.
	 * @since 2017/01/11
	 */
	public void setSimulation(Simulation __s)
	{
		this._simulation = __s;
	}
	
	/**
	 * Returns the width.
	 *
	 * @return The width.
	 * @since 2017/01/11
	 */
	public int width()
	{
		return this.width;
	}
	
	/**
	 * Updates and renders the game.
	 *
	 * @since 2017/01/11
	 */
	public void update()
	{
		// Need a simulation to draw on
		Simulation simulation = this._simulation;
		if (simulation == null)
			return;
		
		// Get framebuffer
		int[] framebuffer = this.framebuffer;
		int width = this.width, height = this.height;
		int frame = simulation.currentFrame();
		
		// Initialize framebuffer with something, to see how it works
		double br = 255.0 / width, at = 0.0;
		for (int i = 0, p = (width * height); i < p; i++)
		{
			int bid = (((int)at) + frame) & 0xFF;
			framebuffer[i] = (bid << 16) | (bid << 8) | (bid);
			
			// Switch directions?
			at += br;
			if (((i + 1) % width) == 0)
				br = -br;
		}
	}
}

