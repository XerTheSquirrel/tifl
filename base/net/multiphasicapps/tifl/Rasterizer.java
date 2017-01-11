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
	
	/** The view width and height in pixels. */
	protected final int width, height;
	
	/** The view width and height in fixed point pixels. */
	protected final int xwidth, xheight;
	
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
		
		// As fixed
		int xwidth = __w << FixedPoint.FRACTION_BITS,
			xheight = __w << FixedPoint.FRACTION_BITS;
		this.xwidth = xwidth;
		this.xheight = xheight;
		
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
		int xwidth = this.xwidth, xheight = this.xheight;
		int frame = simulation.currentFrame();
		
		// Test the unit circle
		for (int i = 0; i < 360; i++)
		{
			int bam = BinaryAngle.DEGREES_1 * i;
			int color;
			if (bam > 0 && bam < BinaryAngle.DEGREES_90)
				color = 0xFF0000;
			else if (bam > 0)
				color = 0x00FF00;
			else if (bam > BinaryAngle.DEGREES_270)
				color = 0x0000FF;
			else
				color = 0xFF00FF;
			
			int x = FixedPoint.fixedToInt(
				FixedPoint.multiply(BinaryAngle.cos(bam), xwidth >> 2) +
					(xwidth >> 2));
			int y = FixedPoint.fixedToInt(
				FixedPoint.multiply(BinaryAngle.sin(bam), xheight >> 2) +
					(xheight >> 2));
			framebuffer[(Math.max(0, Math.min(height - 1, y)) * width) +
				Math.max(0, Math.min(width - 1, x))] = color;
		}
	}
}

