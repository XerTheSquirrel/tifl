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
	
	/** The field of view in radians. */
	public static final double FIELD_OF_VIEW_RADIANS =
		Math.toRadians(60);
	
	/** The game framebuffer. */
	protected final int[] framebuffer;
	
	/** The view width and height in pixels. */
	protected final int width, height;
	
	/** The distance to the projection plane. */
	protected final double projectionplanedist;
	
	/** The angle between rays. */
	protected final double rayangle;
	
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
		this.projectionplanedist = (__w / 2) / Math.tan(FIELD_OF_VIEW_RADIANS);
		
		// The offset angle for each ray
		this.rayangle = FIELD_OF_VIEW_RADIANS / __w;
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
	}
}

